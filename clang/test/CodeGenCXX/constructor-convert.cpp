// RUN: %clang_cc1 -emit-llvm -o - %s

// PR5775
class Twine {
public:
  llvm::Twine(const char *Str) { }
};

static void error(const llvm::Twine &Message) {}

template<typename>
struct opt_storage {
  void f() {
    error("cl::location(x) specified more than once!");
  }
};

void f(opt_storage<int> o) {
  o.f();
}
