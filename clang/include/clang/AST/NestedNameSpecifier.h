//===- NestedNameSpecifier.h - C++ nested name specifiers -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  This file completes the definition of the NestedNameSpecifier class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_AST_NESTEDNAMESPECIFIER_H
#define LLVM_CLANG_AST_NESTEDNAMESPECIFIER_H

#include <optional>

#include "clang/AST/NestedNameSpecifierBase.h"
#include "llvm/ADT/DenseMapInfo.h"
#include "llvm/ADT/Hashing.h"
#include "llvm/ADT/APInt.h"

namespace llvm {

template <> struct DenseMapInfo<clang::NestedNameSpecifier> {
  static clang::NestedNameSpecifier getEmptyKey() { return std::nullopt; }

  static clang::NestedNameSpecifier getTombstoneKey() {
    return clang::NestedNameSpecifier::getInvalid();
  }

  static unsigned getHashValue(const clang::NestedNameSpecifier &V) {
    return hash_combine(V.getAsVoidPointer());
  }
};

template <> struct DenseMapInfo<clang::NestedNameSpecifierLoc> {
  using FirstInfo = DenseMapInfo<clang::NestedNameSpecifier>;
  using SecondInfo = DenseMapInfo<void *>;

  static clang::NestedNameSpecifierLoc getEmptyKey() {
    return clang::NestedNameSpecifierLoc(FirstInfo::getEmptyKey(),
                                         SecondInfo::getEmptyKey());
  }

  static clang::NestedNameSpecifierLoc getTombstoneKey() {
    return clang::NestedNameSpecifierLoc(FirstInfo::getTombstoneKey(),
                                         SecondInfo::getTombstoneKey());
  }

  static unsigned getHashValue(const clang::NestedNameSpecifierLoc &PairVal) {
    return hash_combine(
        FirstInfo::getHashValue(PairVal.getNestedNameSpecifier()),
        SecondInfo::getHashValue(PairVal.getOpaqueData()));
  }

  static bool isEqual(const clang::NestedNameSpecifierLoc &LHS,
                      const clang::NestedNameSpecifierLoc &RHS) {
    return LHS == RHS;
  }
};
} // namespace llvm

#endif // LLVM_CLANG_AST_NESTEDNAMESPECIFIER_H
