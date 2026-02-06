//===--- M68k.h - M68k-specific Tool Helpers -----------------*- C++-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_ARCH_M680X0_H
#define LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_ARCH_M680X0_H

#include <string>
#include <vector>

namespace llvm {
class StringRef;
class Triple;
namespace opt {
class ArgList;
}  // namespace opt
}  // namespace llvm

namespace clang {
namespace driver {
class Driver;

namespace tools {
namespace m68k {

std::string getM68kTargetCPU(const llvm::opt::ArgList &Args);

void getM68kTargetFeatures(const Driver &D, const llvm::Triple &Triple,
                           const llvm::opt::ArgList &Args,
                           std::vector<llvm::StringRef> &Features);

} // end namespace m68k
} // end namespace tools
} // end namespace driver
} // end namespace clang

#endif // LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_ARCH_M680X0_H
