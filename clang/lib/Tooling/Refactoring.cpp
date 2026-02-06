//===--- Refactoring.cpp - Framework for clang refactoring tools ----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  Implements tools to support refactorings.
//
//===----------------------------------------------------------------------===//

#include "clang/Tooling/Refactoring.h"

#include <map>
#include <memory>
#include <string>
#include <utility>

#include "clang/Basic/DiagnosticIDs.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Basic/FileEntry.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/LLVM.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Format/Format.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Core/Replacement.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/LangOptions.h"
#include "llvm/ADT/ArrayRef.h"

namespace clang {
class PCHContainerOperations;

namespace tooling {
class CompilationDatabase;

RefactoringTool::RefactoringTool(
    const CompilationDatabase &Compilations, ArrayRef<std::string> SourcePaths,
    std::shared_ptr<PCHContainerOperations> PCHContainerOps)
    : ClangTool(Compilations, SourcePaths, std::move(PCHContainerOps)) {}

std::map<std::string, Replacements> &RefactoringTool::getReplacements() {
  return FileToReplaces;
}

int RefactoringTool::runAndSave(FrontendActionFactory *ActionFactory) {
  if (int Result = run(ActionFactory)) {
    return Result;
  }

  LangOptions DefaultLangOptions;
  DiagnosticOptions DiagOpts;
  TextDiagnosticPrinter DiagnosticPrinter(llvm::errs(), DiagOpts);
  DiagnosticsEngine Diagnostics(DiagnosticIDs::create(), DiagOpts,
                                &DiagnosticPrinter, false);
  SourceManager Sources(Diagnostics, getFiles());
  Rewriter Rewrite(Sources, DefaultLangOptions);

  if (!applyAllReplacements(Rewrite)) {
    llvm::errs() << "Skipped some replacements.\n";
  }

  return saveRewrittenFiles(Rewrite);
}

bool RefactoringTool::applyAllReplacements(Rewriter &Rewrite) {
  bool Result = true;
  for (const auto &Entry : groupReplacementsByFile(
           Rewrite.getSourceMgr().getFileManager(), FileToReplaces))
    Result = tooling::applyAllReplacements(Entry.second, Rewrite) && Result;
  return Result;
}

int RefactoringTool::saveRewrittenFiles(Rewriter &Rewrite) {
  return Rewrite.overwriteChangedFiles() ? 1 : 0;
}

bool formatAndApplyAllReplacements(
    const std::map<std::string, Replacements> &FileToReplaces,
    Rewriter &Rewrite, StringRef Style) {
  SourceManager &SM = Rewrite.getSourceMgr();
  FileManager &Files = SM.getFileManager();

  bool Result = true;
  for (const auto &FileAndReplaces : groupReplacementsByFile(
           Rewrite.getSourceMgr().getFileManager(), FileToReplaces)) {
    const std::string &FilePath = FileAndReplaces.first;
    auto &CurReplaces = FileAndReplaces.second;

    FileEntryRef Entry = llvm::cantFail(Files.getFileRef(FilePath));
    FileID ID = SM.getOrCreateFileID(Entry, SrcMgr::C_User);
    StringRef Code = SM.getBufferData(ID);

    auto CurStyle = format::getStyle(Style, FilePath, "LLVM");
    if (!CurStyle) {
      llvm::errs() << llvm::toString(CurStyle.takeError()) << "\n";
      return false;
    }

    auto NewReplacements =
        format::formatReplacements(Code, CurReplaces, *CurStyle);
    if (!NewReplacements) {
      llvm::errs() << llvm::toString(NewReplacements.takeError()) << "\n";
      return false;
    }
    Result = applyAllReplacements(*NewReplacements, Rewrite) && Result;
  }
  return Result;
}

} // end namespace tooling
} // end namespace clang
