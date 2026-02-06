//===--- LFILinux.h - LFI ToolChain Implementations -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_LFI_LINUX_H
#define LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_LFI_LINUX_H

#include <string>
#include <vector>

#include "Linux.h"
#include "llvm/Option/Option.h"
#include "llvm/Support/Compiler.h"

namespace llvm {
class Triple;
namespace opt {
class ArgList;
}  // namespace opt
}  // namespace llvm

namespace clang {
namespace driver {
class Driver;

namespace toolchains {

class LLVM_LIBRARY_VISIBILITY LFILinux : public Linux {
public:
  LFILinux(const Driver &D, const llvm::Triple &Triple,
           const llvm::opt::ArgList &Args)
      : Linux(D, Triple, Args) {
    ExtraOpts.push_back("-z");
    ExtraOpts.push_back("separate-code");
  }

  CXXStdlibType GetDefaultCXXStdlibType() const override;

  void AddCXXStdlibLibArgs(const llvm::opt::ArgList &Args,
                           llvm::opt::ArgStringList &CmdArgs) const override;
};

} // end namespace toolchains
} // end namespace driver
} // end namespace clang

#endif // LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_LFI_LINUX_H
