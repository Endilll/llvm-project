//===--- SystemZ.h - SystemZ-specific Tool Helpers --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_ARCH_SYSTEMZ_H
#define LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_ARCH_SYSTEMZ_H

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
namespace systemz {

enum class FloatABI {
  Soft,
  Hard,
};

FloatABI getSystemZFloatABI(const Driver &D, const llvm::opt::ArgList &Args);

std::string getSystemZTargetCPU(const llvm::opt::ArgList &Args,
                                const llvm::Triple &T);

void getSystemZTargetFeatures(const Driver &D, const llvm::opt::ArgList &Args,
                              std::vector<llvm::StringRef> &Features);

} // end namespace systemz
} // end namespace target
} // end namespace driver
} // end namespace clang

#endif // LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_ARCH_SYSTEMZ_H
