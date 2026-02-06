//===--- VE.h - VE-specific Tool Helpers ------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_ARCH_VE_H
#define LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_ARCH_VE_H

#include <vector>

namespace llvm {
class StringRef;
namespace opt {
class ArgList;
}  // namespace opt
}  // namespace llvm

namespace clang {
namespace driver {
class Driver;

namespace tools {
namespace ve {

void getVETargetFeatures(const Driver &D, const llvm::opt::ArgList &Args,
                         std::vector<llvm::StringRef> &Features);

} // end namespace ve
} // end namespace tools
} // end namespace driver
} // end namespace clang

#endif // LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_ARCH_VE_H
