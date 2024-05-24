//===--- TextDiagnostic.h - Text Diagnostic Pretty-Printing -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This is a utility class that provides support for textual pretty-printing of
// diagnostics. It is used to implement the different code paths which require
// such functionality in a consistent way.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_FRONTEND_TEXTDIAGNOSTIC_H
#define LLVM_CLANG_FRONTEND_TEXTDIAGNOSTIC_H

#include "clang/Frontend/DiagnosticRenderer.h"
#include "llvm/Support/raw_ostream.h"

namespace clang {

/// Class to encapsulate the logic for formatting and printing a textual
/// diagnostic message.
///
/// This class provides an interface for building and emitting a textual
/// diagnostic, including all of the macro backtraces, caret diagnostics, FixIt
/// Hints, and code snippets. In the presence of macros this involves
/// a recursive process, synthesizing notes for each macro expansion.
///
/// The purpose of this class is to isolate the implementation of printing
/// beautiful text diagnostics from any particular interfaces. The Clang
/// DiagnosticClient is implemented through this class as is diagnostic
/// printing coming out of libclang.
class TextDiagnostic : public DiagnosticRenderer {
  llvm::raw_ostream &OS;
  const Preprocessor *PP;

public:
  TextDiagnostic(llvm::raw_ostream &OS, const LangOptions &LangOpts,
                 DiagnosticOptions *DiagOpts, const Preprocessor *PP = nullptr);

  ~TextDiagnostic() override;

  struct StyleRange {
    unsigned Start;
    unsigned End;
    enum llvm::raw_ostream::Colors Color;
    StyleRange(unsigned S, unsigned E, enum llvm::raw_ostream::Colors C)
        : Start(S), End(E), Color(C){};
  };

  /// Print the diagonstic level to a llvm::raw_ostream.
  ///
  /// This is a static helper that handles colorizing the level and formatting
  /// it into an arbitrary output stream. This is used internally by the
  /// TextDiagnostic emission code, but it can also be used directly by
  /// consumers that don't have a source manager or other state that the full
  /// TextDiagnostic logic requires.
  static void printDiagnosticLevel(llvm::raw_ostream &OS,
                                   DiagnosticsEngine::Level Level,
                                   bool ShowColors);

  /// Pretty-print a diagnostic message to a llvm::raw_ostream.
  ///
  /// This is a static helper to handle the line wrapping, colorizing, and
  /// rendering of a diagnostic message to a particular ostream. It is
  /// publicly visible so that clients which do not have sufficient state to
  /// build a complete TextDiagnostic object can still get consistent
  /// formatting of their diagnostic messages.
  ///
  /// \param OS Where the message is printed
  /// \param IsSupplemental true if this is a continuation note diagnostic
  /// \param Message The text actually printed
  /// \param CurrentColumn The starting column of the first line, accounting
  ///                      for any prefix.
  /// \param Columns The number of columns to use in line-wrapping, 0 disables
  ///                all line-wrapping.
  /// \param ShowColors Enable colorizing of the message.
  static void printDiagnosticMessage(llvm::raw_ostream &OS, bool IsSupplemental,
                                     llvm::StringRef Message, unsigned CurrentColumn,
                                     unsigned Columns, bool ShowColors);

protected:
  void emitDiagnosticMessage(FullSourceLoc Loc, PresumedLoc PLoc,
                             DiagnosticsEngine::Level Level, llvm::StringRef Message,
                             llvm::ArrayRef<CharSourceRange> Ranges,
                             DiagOrStoredDiag D) override;

  void emitDiagnosticLoc(FullSourceLoc Loc, PresumedLoc PLoc,
                         DiagnosticsEngine::Level Level,
                         llvm::ArrayRef<CharSourceRange> Ranges) override;

  void emitCodeContext(FullSourceLoc Loc, DiagnosticsEngine::Level Level,
                       llvm::SmallVectorImpl<CharSourceRange> &Ranges,
                       llvm::ArrayRef<FixItHint> Hints) override {
    emitSnippetAndCaret(Loc, Level, Ranges, Hints);
  }

  void emitIncludeLocation(FullSourceLoc Loc, PresumedLoc PLoc) override;

  void emitImportLocation(FullSourceLoc Loc, PresumedLoc PLoc,
                          llvm::StringRef ModuleName) override;

  void emitBuildingModuleLocation(FullSourceLoc Loc, PresumedLoc PLoc,
                                  llvm::StringRef ModuleName) override;

private:
  void emitFilename(llvm::StringRef Filename, const SourceManager &SM);

  void emitSnippetAndCaret(FullSourceLoc Loc, DiagnosticsEngine::Level Level,
                           llvm::SmallVectorImpl<CharSourceRange> &Ranges,
                           llvm::ArrayRef<FixItHint> Hints);

  void emitSnippet(llvm::StringRef SourceLine, unsigned MaxLineNoDisplayWidth,
                   unsigned LineNo, unsigned DisplayLineNo,
                   llvm::ArrayRef<StyleRange> Styles);

  void emitParseableFixits(llvm::ArrayRef<FixItHint> Hints, const SourceManager &SM);
};

} // end namespace clang

#endif
