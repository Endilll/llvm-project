//===--- LLVM.h - Import various common LLVM datatypes ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
/// \file
/// Forward-declares and imports various common LLVM datatypes that
/// clang wants to use unqualified.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_BASIC_LLVM_H
#define LLVM_CLANG_BASIC_LLVM_H

// Do not proliferate #includes here, require clients to #include their
// dependencies.
// Casting.h has complex templates that cannot be easily forward declared.
#include "llvm/Support/Casting.h"

namespace llvm {
  class raw_ostream;
}


namespace clang {
  // Casting operators.
  using llvm::isa;
  using llvm::isa_and_nonnull;
  using llvm::isa_and_present;
  using llvm::cast;
  using llvm::dyn_cast;
  using llvm::dyn_cast_or_null;
  using llvm::dyn_cast_if_present;
  using llvm::cast_or_null;
  using llvm::cast_if_present;
} // end namespace clang.

#endif
