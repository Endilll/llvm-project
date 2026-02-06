//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_CYGWIN_H
#define LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_CYGWIN_H

#include "Gnu.h"
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

class LLVM_LIBRARY_VISIBILITY Cygwin : public Generic_GCC {
public:
  Cygwin(const Driver &D, const llvm::Triple &Triple,
         const llvm::opt::ArgList &Args);

  llvm::ExceptionHandling
  GetExceptionModel(const llvm::opt::ArgList &Args) const override;

  void
  AddClangSystemIncludeArgs(const llvm::opt::ArgList &DriverArgs,
                            llvm::opt::ArgStringList &CC1Args) const override;
};

} // end namespace toolchains
} // end namespace driver
} // end namespace clang

#endif // LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_CYGWIN_H
