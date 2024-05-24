// RUN: %check_clang_tidy %s llvm-twine-local %t

namespace llvm {
class Twine {
public:
  llvm::Twine(const char *);
  llvm::Twine(int);
  llvm::Twine();
  llvm::Twine &operator+(const llvm::Twine &);
};
}

using namespace llvm;

void foo(const llvm::Twine &x);
void bar(llvm::Twine x);

static llvm::Twine Moo = llvm::Twine("bark") + "bah";
// CHECK-MESSAGES: :[[@LINE-1]]:14: warning: twine variables are prone to use-after-free bugs
// CHECK-MESSAGES: note: FIX-IT applied suggested code changes
// CHECK-FIXES: static std::string Moo = (llvm::Twine("bark") + "bah").str();

int main() {
  const llvm::Twine t = llvm::Twine("a") + "b" + llvm::Twine(42);
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: twine variables are prone to use-after-free bugs
// CHECK-MESSAGES: note: FIX-IT applied suggested code changes
// CHECK-FIXES: std::string t = (llvm::Twine("a") + "b" + llvm::Twine(42)).str();
  foo(llvm::Twine("a") + "b");

  llvm::Twine Prefix = false ? "__INT_FAST" : "__UINT_FAST";
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: twine variables are prone to use-after-free bugs
// CHECK-MESSAGES: note: FIX-IT applied suggested code changes
// CHECK-FIXES: const char * Prefix = false ? "__INT_FAST" : "__UINT_FAST";

  const llvm::Twine t2 = llvm::Twine();
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: twine variables are prone to use-after-free bugs
// CHECK-MESSAGES: note: FIX-IT applied suggested code changes
// CHECK-FIXES: std::string t2 = (llvm::Twine()).str();
  foo(llvm::Twine() + "b");

  const llvm::Twine t3 = llvm::Twine(42);
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: twine variables are prone to use-after-free bugs
// CHECK-MESSAGES: note: FIX-IT applied suggested code changes
// CHECK-FIXES: std::string t3 = (llvm::Twine(42)).str();

  const llvm::Twine t4 = llvm::Twine(42) + "b";
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: twine variables are prone to use-after-free bugs
// CHECK-MESSAGES: note: FIX-IT applied suggested code changes
// CHECK-FIXES: std::string t4 = (llvm::Twine(42) + "b").str();

  const llvm::Twine t5 = llvm::Twine() + "b";
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: twine variables are prone to use-after-free bugs
// CHECK-MESSAGES: note: FIX-IT applied suggested code changes
// CHECK-FIXES: std::string t5 = (llvm::Twine() + "b").str();

  const llvm::Twine t6 = true ? llvm::Twine() : llvm::Twine(42);
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: twine variables are prone to use-after-free bugs
// CHECK-MESSAGES: note: FIX-IT applied suggested code changes
// CHECK-FIXES: std::string t6 = (true ? llvm::Twine() : llvm::Twine(42)).str();

  const llvm::Twine t7 = false ? llvm::Twine() : llvm::Twine("b");
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: twine variables are prone to use-after-free bugs
// CHECK-MESSAGES: note: FIX-IT applied suggested code changes
// CHECK-FIXES: std::string t7 = (false ? llvm::Twine() : llvm::Twine("b")).str();
}
