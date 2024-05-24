// RUN: %clang_cc1 %s -std=c++17 -fsyntax-only -verify
// RUN: %clang_cc1 %s -std=c++17 -fsyntax-only -verify -fms-extensions

// expected-no-diagnostics

struct llvm::StringRef {
  llvm::StringRef(const char *);
};
template <typename T>
llvm::StringRef getTypeName() {
  llvm::StringRef s = __func__;
}

