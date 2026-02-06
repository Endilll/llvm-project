//===--- InterpreterUtils.h - Incremental Utils --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements some common utils used in the incremental library.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_INTERPRETER_UTILS_H
#define LLVM_CLANG_INTERPRETER_UTILS_H



#include <stdint.h>
#include <string>

#include "clang/Sema/Sema.h"
#include "llvm/ADT/StringRef.h"

namespace clang {
class ASTContext;
class Decl;
class DeclContext;
class Expr;
class IntegerLiteral;
class NamedDecl;
class NamespaceDecl;
class QualType;

IntegerLiteral *IntegerLiteralExpr(ASTContext &C, uint64_t Val);

Expr *CStyleCastPtrExpr(Sema &S, QualType Ty, Expr *E);

Expr *CStyleCastPtrExpr(Sema &S, QualType Ty, uintptr_t Ptr);

Sema::DeclGroupPtrTy CreateDGPtrFrom(Sema &S, Decl *D);

NamespaceDecl *LookupNamespace(Sema &S, llvm::StringRef Name,
                               const DeclContext *Within = nullptr);

NamedDecl *LookupNamed(Sema &S, llvm::StringRef Name,
                       const DeclContext *Within = nullptr);

std::string GetFullTypeName(ASTContext &Ctx, QualType QT);
} // namespace clang

#endif
