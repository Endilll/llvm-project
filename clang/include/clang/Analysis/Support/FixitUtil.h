//===- FixitUtil.h ----------------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_ANALYSIS_SUPPORT_FIXITUTIL_H
#define LLVM_CLANG_ANALYSIS_SUPPORT_FIXITUTIL_H

#include <optional>
#include <string>

#include "clang/Basic/LLVM.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Lex/Lexer.h"

namespace llvm {
class StringRef;
}  // namespace llvm

namespace clang {
class DeclaratorDecl;
class Expr;
class LangOptions;
class Qualifiers;
class SourceManager;

// Returns the text of the pointee type of `T` from a `VarDecl` of a pointer
// type. The text is obtained through from `TypeLoc`s.  Since `TypeLoc` does not
// have source ranges of qualifiers ( The `QualTypeLoc` looks hacky too me
// :( ), `Qualifiers` of the pointee type is returned separately through the
// output parameter `QualifiersToAppend`.
std::optional<std::string>
getPointeeTypeText(const DeclaratorDecl *VD, const SourceManager &SM,
                   const LangOptions &LangOpts,
                   std::optional<Qualifiers> *QualifiersToAppend);

// returns text of pointee to pointee (T*&)
std::optional<std::string>
getPointee2TypeText(const DeclaratorDecl *VD, const SourceManager &SM,
                    const LangOptions &LangOpts,
                    std::optional<Qualifiers> *QualifiersToAppend);

SourceLocation getBeginLocOfNestedIdentifier(const DeclaratorDecl *D);

// Returns the literal text in `SourceRange SR`, if `SR` is a valid range.
std::optional<StringRef> getRangeText(SourceRange SR, const SourceManager &SM,
                                      const LangOptions &LangOpts);

// Returns the literal text of the identifier of the given variable declaration.
std::optional<StringRef> getVarDeclIdentifierText(const DeclaratorDecl *VD,
                                                  const SourceManager &SM,
                                                  const LangOptions &LangOpts);

// Return text representation of an `Expr`.
std::optional<StringRef> getExprText(const Expr *E, const SourceManager &SM,
                                     const LangOptions &LangOpts);

// Return the source location just past the last character of the AST `Node`.
template <typename NodeTy>
std::optional<SourceLocation> getPastLoc(const NodeTy *Node,
                                         const SourceManager &SM,
                                         const LangOptions &LangOpts) {
  SourceLocation Loc =
      Lexer::getLocForEndOfToken(Node->getEndLoc(), 0, SM, LangOpts);
  if (Loc.isValid())
    return Loc;
  return std::nullopt;
}

// Returns the begin location of the identifier of the given variable
// declaration.
SourceLocation getVarDeclIdentifierLoc(const DeclaratorDecl *VD);

} // end namespace clang

#endif /* LLVM_CLANG_ANALYSIS_SUPPORT_FIXITUTIL_H */
