//===--- Managarm.h - Managarm ToolChain Implementations --------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_MANAGARM_H
#define LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_MANAGARM_H

#include <string>
#include <vector>

#include "Gnu.h"
#include "clang/Basic/LLVM.h"
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

class LLVM_LIBRARY_VISIBILITY Managarm : public Generic_ELF {
public:
  Managarm(const Driver &D, const llvm::Triple &Triple,
           const llvm::opt::ArgList &Args);

  bool HasNativeLLVMSupport() const override;

  std::string getMultiarchTriple(const Driver &D,
                                 const llvm::Triple &TargetTriple,
                                 StringRef SysRoot) const override;

  void
  AddClangSystemIncludeArgs(const llvm::opt::ArgList &DriverArgs,
                            llvm::opt::ArgStringList &CC1Args) const override;
  void
  addLibStdCxxIncludePaths(const llvm::opt::ArgList &DriverArgs,
                           llvm::opt::ArgStringList &CC1Args) const override;

  bool IsAArch64OutlineAtomicsDefault(
      const llvm::opt::ArgList &Args) const override {
    return true;
  }

  SanitizerMask getSupportedSanitizers() const override;
  std::string computeSysRoot() const override;

  std::string getDynamicLinker(const llvm::opt::ArgList &Args) const override;

  void addExtraOpts(llvm::opt::ArgStringList &CmdArgs) const override;

  std::vector<std::string> ExtraOpts;

protected:
  Tool *buildAssembler() const override;
  Tool *buildLinker() const override;
};

} // end namespace toolchains
} // end namespace driver
} // end namespace clang

#endif // LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_MANAGARM_H
