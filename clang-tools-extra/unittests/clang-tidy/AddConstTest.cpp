#include "../clang-tidy/utils/FixItHintUtils.h"
#include "ClangTidyDiagnosticConsumer.h"
#include "ClangTidyTest.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Tooling/Tooling.h"
#include "gtest/gtest.h"
#include <optional>

namespace clang {
namespace tidy {

namespace {
using namespace clang::ast_matchers;
using namespace utils::fixit;

template <QualifierTarget CT, QualifierPolicy CP>
class ConstTransform : public ClangTidyCheck {
public:
  ConstTransform(llvm::StringRef CheckName, ClangTidyContext *Context)
      : ClangTidyCheck(CheckName, Context) {}

  void registerMatchers(MatchFinder *Finder) override {
    Finder->addMatcher(varDecl(hasName("target")).bind("var"), this);
  }

  void check(const MatchFinder::MatchResult &Result) override {
    const auto *D = Result.Nodes.getNodeAs<VarDecl>("var");
    using utils::fixit::addQualifierToVarDecl;
    std::optional<FixItHint> Fix = addQualifierToVarDecl(
        *D, *Result.Context, DeclSpec::TQ::TQ_const, CT, CP);
    auto Diag = diag(D->getBeginLoc(), "doing const transformation");
    if (Fix)
      Diag << *Fix;
  }
};
} // namespace

namespace test {
using PointeeLTransform =
    ConstTransform<QualifierTarget::Pointee, QualifierPolicy::Left>;
using PointeeRTransform =
    ConstTransform<QualifierTarget::Pointee, QualifierPolicy::Right>;

using ValueLTransform =
    ConstTransform<QualifierTarget::Value, QualifierPolicy::Left>;
using ValueRTransform =
    ConstTransform<QualifierTarget::Value, QualifierPolicy::Right>;

// ----------------------------------------------------------------------------
// Test Value-like types. Everything with indirection is done later.
// ----------------------------------------------------------------------------

TEST(Values, Builtin) {
  llvm::StringRef Snippet = "int target = 0;";

  EXPECT_EQ("const int target = 0;", runCheckOnCode<ValueLTransform>(Snippet));
  EXPECT_EQ("const int target = 0;",
            runCheckOnCode<PointeeLTransform>(Snippet));

  EXPECT_EQ("int const target = 0;", runCheckOnCode<ValueRTransform>(Snippet));
  EXPECT_EQ("int const target = 0;",
            runCheckOnCode<PointeeRTransform>(Snippet));
}
TEST(Values, TypedefBuiltin) {
  llvm::StringRef T = "typedef int MyInt;";
  llvm::StringRef S = "MyInt target = 0;";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("const MyInt target = 0;"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("const MyInt target = 0;"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));

  EXPECT_EQ(Cat("MyInt const target = 0;"),
            runCheckOnCode<ValueRTransform>(Cat(S)));
  EXPECT_EQ(Cat("MyInt const target = 0;"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}
TEST(Values, TypedefBuiltinPointer) {
  llvm::StringRef T = "typedef int* MyInt;";
  llvm::StringRef S = "MyInt target = nullptr;";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("const MyInt target = nullptr;"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("const MyInt target = nullptr;"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));

  EXPECT_EQ(Cat("MyInt const target = nullptr;"),
            runCheckOnCode<ValueRTransform>(Cat(S)));
  EXPECT_EQ(Cat("MyInt const target = nullptr;"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}
TEST(Values, UsingBuiltin) {
  llvm::StringRef T = "using MyInt = int;";
  llvm::StringRef S = "MyInt target = 0;";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("const MyInt target = 0;"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("const MyInt target = 0;"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));

  EXPECT_EQ(Cat("MyInt const target = 0;"),
            runCheckOnCode<ValueRTransform>(Cat(S)));
  EXPECT_EQ(Cat("MyInt const target = 0;"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}
TEST(Values, UsingBuiltinPointer) {
  llvm::StringRef T = "using MyInt = int*;";
  llvm::StringRef S = "MyInt target = nullptr;";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("const MyInt target = nullptr;"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("const MyInt target = nullptr;"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));

  EXPECT_EQ(Cat("MyInt const target = nullptr;"),
            runCheckOnCode<ValueRTransform>(Cat(S)));
  EXPECT_EQ(Cat("MyInt const target = nullptr;"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}
TEST(Values, AutoValue) {
  llvm::StringRef T = "int f() { return 42; }\n";
  llvm::StringRef S = "auto target = f();";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("const auto target = f();"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("const auto target = f();"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));

  EXPECT_EQ(Cat("auto const target = f();"),
            runCheckOnCode<ValueRTransform>(Cat(S)));
  EXPECT_EQ(Cat("auto const target = f();"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}
TEST(Values, AutoPointer) {
  llvm::StringRef T = "int* f() { return nullptr; }\n";
  llvm::StringRef S = "auto target = f();";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("const auto target = f();"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("const auto target = f();"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));

  EXPECT_EQ(Cat("auto const target = f();"),
            runCheckOnCode<ValueRTransform>(Cat(S)));
  EXPECT_EQ(Cat("auto const target = f();"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}
TEST(Values, AutoReference) {
  llvm::StringRef T = "static int global = 42; int& f() { return global; }\n";
  llvm::StringRef S = "auto target = f();";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("const auto target = f();"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("const auto target = f();"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));

  EXPECT_EQ(Cat("auto const target = f();"),
            runCheckOnCode<ValueRTransform>(Cat(S)));
  EXPECT_EQ(Cat("auto const target = f();"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}
TEST(Values, DeclTypeValue) {
  llvm::StringRef T = "int f() { return 42; }\n";
  llvm::StringRef S = "decltype(f()) target = f();";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("const decltype(f()) target = f();"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("const decltype(f()) target = f();"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));

  EXPECT_EQ(Cat("decltype(f()) const target = f();"),
            runCheckOnCode<ValueRTransform>(Cat(S)));
  EXPECT_EQ(Cat("decltype(f()) const target = f();"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}
TEST(Values, DeclTypePointer) {
  // The pointer itself will be changed to 'const'. There is no
  // way to make the pointee 'const' with this syntax.
  llvm::StringRef T = "int* f() { return nullptr; }\n";
  llvm::StringRef S = "decltype(f()) target = f();";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("const decltype(f()) target = f();"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("const decltype(f()) target = f();"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));

  EXPECT_EQ(Cat("decltype(f()) const target = f();"),
            runCheckOnCode<ValueRTransform>(Cat(S)));
  EXPECT_EQ(Cat("decltype(f()) const target = f();"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}
TEST(Values, DeclTypeReference) {
  // Same as pointer, but the reference itself will be marked 'const'.
  // This has no effect and will result in a warning afterwards. The
  // transformation itself is still correct.
  llvm::StringRef T = "static int global = 42; int& f() { return global; }\n";
  llvm::StringRef S = "decltype(f()) target = f();";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("const decltype(f()) target = f();"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("const decltype(f()) target = f();"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));

  EXPECT_EQ(Cat("decltype(f()) const target = f();"),
            runCheckOnCode<ValueRTransform>(Cat(S)));
  EXPECT_EQ(Cat("decltype(f()) const target = f();"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}
TEST(Values, Parens) {
  llvm::StringRef Snippet = "int ((target)) = 0;";

  EXPECT_EQ("const int ((target)) = 0;",
            runCheckOnCode<ValueLTransform>(Snippet));
  EXPECT_EQ("const int ((target)) = 0;",
            runCheckOnCode<PointeeLTransform>(Snippet));

  EXPECT_EQ("int const ((target)) = 0;",
            runCheckOnCode<ValueRTransform>(Snippet));
  EXPECT_EQ("int const ((target)) = 0;",
            runCheckOnCode<PointeeRTransform>(Snippet));
}

// ----------------------------------------------------------------------------
// Test builtin-arrays
// ----------------------------------------------------------------------------

TEST(Arrays, Builtin) {
  llvm::StringRef Snippet = "int target[][1] = {{1}, {2}, {3}};";

  EXPECT_EQ("const int target[][1] = {{1}, {2}, {3}};",
            runCheckOnCode<PointeeLTransform>(Snippet));
  EXPECT_EQ("const int target[][1] = {{1}, {2}, {3}};",
            runCheckOnCode<ValueLTransform>(Snippet));

  EXPECT_EQ("int const target[][1] = {{1}, {2}, {3}};",
            runCheckOnCode<PointeeRTransform>(Snippet));
  EXPECT_EQ("int const target[][1] = {{1}, {2}, {3}};",
            runCheckOnCode<ValueRTransform>(Snippet));
}
TEST(Arrays, BuiltinParens) {
  llvm::StringRef Snippet = "int ((target))[][1] = {{1}, {2}, {3}};";

  EXPECT_EQ("const int ((target))[][1] = {{1}, {2}, {3}};",
            runCheckOnCode<PointeeLTransform>(Snippet));
  EXPECT_EQ("const int ((target))[][1] = {{1}, {2}, {3}};",
            runCheckOnCode<ValueLTransform>(Snippet));

  EXPECT_EQ("int const ((target))[][1] = {{1}, {2}, {3}};",
            runCheckOnCode<PointeeRTransform>(Snippet));
  EXPECT_EQ("int const ((target))[][1] = {{1}, {2}, {3}};",
            runCheckOnCode<ValueRTransform>(Snippet));
}
TEST(Arrays, Pointers) {
  llvm::StringRef Snippet = "int x; int* target[] = {&x, &x, &x};";

  EXPECT_EQ("int x; const int* target[] = {&x, &x, &x};",
            runCheckOnCode<PointeeLTransform>(Snippet));
  EXPECT_EQ("int x; int const* target[] = {&x, &x, &x};",
            runCheckOnCode<PointeeRTransform>(Snippet));

  EXPECT_EQ("int x; int* const target[] = {&x, &x, &x};",
            runCheckOnCode<ValueLTransform>(Snippet));
  EXPECT_EQ("int x; int* const target[] = {&x, &x, &x};",
            runCheckOnCode<ValueRTransform>(Snippet));
}
TEST(Arrays, PointerPointers) {
  llvm::StringRef Snippet = "int* x = nullptr; int** target[] = {&x, &x, &x};";

  EXPECT_EQ("int* x = nullptr; int* const* target[] = {&x, &x, &x};",
            runCheckOnCode<PointeeLTransform>(Snippet));
  EXPECT_EQ("int* x = nullptr; int** const target[] = {&x, &x, &x};",
            runCheckOnCode<ValueLTransform>(Snippet));

  EXPECT_EQ("int* x = nullptr; int* const* target[] = {&x, &x, &x};",
            runCheckOnCode<PointeeRTransform>(Snippet));
  EXPECT_EQ("int* x = nullptr; int** const target[] = {&x, &x, &x};",
            runCheckOnCode<ValueRTransform>(Snippet));
}
TEST(Arrays, PointersParens) {
  llvm::StringRef Snippet = "int x; int* (target)[] = {&x, &x, &x};";

  EXPECT_EQ("int x; const int* (target)[] = {&x, &x, &x};",
            runCheckOnCode<PointeeLTransform>(Snippet));
  EXPECT_EQ("int x; int const* (target)[] = {&x, &x, &x};",
            runCheckOnCode<PointeeRTransform>(Snippet));

  EXPECT_EQ("int x; int* const (target)[] = {&x, &x, &x};",
            runCheckOnCode<ValueLTransform>(Snippet));
  EXPECT_EQ("int x; int* const (target)[] = {&x, &x, &x};",
            runCheckOnCode<ValueRTransform>(Snippet));
}

// ----------------------------------------------------------------------------
// Test reference types. This does not include pointers and arrays.
// ----------------------------------------------------------------------------

TEST(Reference, LValueBuiltin) {
  llvm::StringRef Snippet = "int x = 42; int& target = x;";

  EXPECT_EQ("int x = 42; const int& target = x;",
            runCheckOnCode<ValueLTransform>(Snippet));
  EXPECT_EQ("int x = 42; const int& target = x;",
            runCheckOnCode<PointeeLTransform>(Snippet));

  EXPECT_EQ("int x = 42; int const& target = x;",
            runCheckOnCode<ValueRTransform>(Snippet));
  EXPECT_EQ("int x = 42; int const& target = x;",
            runCheckOnCode<PointeeRTransform>(Snippet));
}
TEST(Reference, RValueBuiltin) {
  llvm::StringRef Snippet = "int&& target = 42;";
  EXPECT_EQ("const int&& target = 42;",
            runCheckOnCode<ValueLTransform>(Snippet));
  EXPECT_EQ("const int&& target = 42;",
            runCheckOnCode<PointeeLTransform>(Snippet));

  EXPECT_EQ("int const&& target = 42;",
            runCheckOnCode<ValueRTransform>(Snippet));
  EXPECT_EQ("int const&& target = 42;",
            runCheckOnCode<PointeeRTransform>(Snippet));
}
TEST(Reference, LValueToPointer) {
  llvm::StringRef Snippet = "int* p; int *& target = p;";
  EXPECT_EQ("int* p; int * const& target = p;",
            runCheckOnCode<ValueLTransform>(Snippet));
  EXPECT_EQ("int* p; int * const& target = p;",
            runCheckOnCode<PointeeLTransform>(Snippet));

  EXPECT_EQ("int* p; int * const& target = p;",
            runCheckOnCode<ValueRTransform>(Snippet));
  EXPECT_EQ("int* p; int * const& target = p;",
            runCheckOnCode<PointeeRTransform>(Snippet));
}
TEST(Reference, LValueParens) {
  llvm::StringRef Snippet = "int x = 42; int ((& target)) = x;";

  EXPECT_EQ("int x = 42; const int ((& target)) = x;",
            runCheckOnCode<ValueLTransform>(Snippet));
  EXPECT_EQ("int x = 42; const int ((& target)) = x;",
            runCheckOnCode<PointeeLTransform>(Snippet));

  EXPECT_EQ("int x = 42; int  const((& target)) = x;",
            runCheckOnCode<ValueRTransform>(Snippet));
  EXPECT_EQ("int x = 42; int  const((& target)) = x;",
            runCheckOnCode<PointeeRTransform>(Snippet));
}
TEST(Reference, ToArray) {
  llvm::StringRef ArraySnippet = "int a[4] = {1, 2, 3, 4};";
  llvm::StringRef Snippet = "int (&target)[4] = a;";
  auto Cat = [&ArraySnippet](llvm::StringRef S) { return (ArraySnippet + S).str(); };

  EXPECT_EQ(Cat("const int (&target)[4] = a;"),
            runCheckOnCode<ValueLTransform>(Cat(Snippet)));
  EXPECT_EQ(Cat("const int (&target)[4] = a;"),
            runCheckOnCode<PointeeLTransform>(Cat(Snippet)));

  EXPECT_EQ(Cat("int  const(&target)[4] = a;"),
            runCheckOnCode<ValueRTransform>(Cat(Snippet)));
  EXPECT_EQ(Cat("int  const(&target)[4] = a;"),
            runCheckOnCode<PointeeRTransform>(Cat(Snippet)));
}
TEST(Reference, Auto) {
  llvm::StringRef T = "static int global = 42; int& f() { return global; }\n";
  llvm::StringRef S = "auto& target = f();";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("const auto& target = f();"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("auto const& target = f();"),
            runCheckOnCode<ValueRTransform>(Cat(S)));

  EXPECT_EQ(Cat("const auto& target = f();"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));
  EXPECT_EQ(Cat("auto const& target = f();"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}

// ----------------------------------------------------------------------------
// Test pointers types.
// ----------------------------------------------------------------------------

TEST(Pointers, SingleBuiltin) {
  llvm::StringRef Snippet = "int* target = nullptr;";

  EXPECT_EQ("int* const target = nullptr;",
            runCheckOnCode<ValueLTransform>(Snippet));
  EXPECT_EQ("int* const target = nullptr;",
            runCheckOnCode<ValueRTransform>(Snippet));

  EXPECT_EQ("const int* target = nullptr;",
            runCheckOnCode<PointeeLTransform>(Snippet));
  EXPECT_EQ("int const* target = nullptr;",
            runCheckOnCode<PointeeRTransform>(Snippet));
}
TEST(Pointers, MultiBuiltin) {
  llvm::StringRef Snippet = "int** target = nullptr;";

  EXPECT_EQ("int** const target = nullptr;",
            runCheckOnCode<ValueLTransform>(Snippet));
  EXPECT_EQ("int** const target = nullptr;",
            runCheckOnCode<ValueRTransform>(Snippet));

  EXPECT_EQ("int* const* target = nullptr;",
            runCheckOnCode<PointeeLTransform>(Snippet));
  EXPECT_EQ("int* const* target = nullptr;",
            runCheckOnCode<PointeeRTransform>(Snippet));
}
TEST(Pointers, ToArray) {
  llvm::StringRef ArraySnippet = "int a[4] = {1, 2, 3, 4};";
  llvm::StringRef Snippet = "int (*target)[4] = &a;";
  auto Cat = [&ArraySnippet](llvm::StringRef S) { return (ArraySnippet + S).str(); };

  EXPECT_EQ(Cat("int (*const target)[4] = &a;"),
            runCheckOnCode<ValueLTransform>(Cat(Snippet)));
  EXPECT_EQ(Cat("const int (*target)[4] = &a;"),
            runCheckOnCode<PointeeLTransform>(Cat(Snippet)));

  EXPECT_EQ(Cat("int (*const target)[4] = &a;"),
            runCheckOnCode<ValueRTransform>(Cat(Snippet)));
  EXPECT_EQ(Cat("int  const(*target)[4] = &a;"),
            runCheckOnCode<PointeeRTransform>(Cat(Snippet)));
}
TEST(Pointers, Parens) {
  llvm::StringRef Snippet = "int ((**target)) = nullptr;";

  EXPECT_EQ("int ((**const target)) = nullptr;",
            runCheckOnCode<ValueLTransform>(Snippet));
  EXPECT_EQ("int ((**const target)) = nullptr;",
            runCheckOnCode<ValueRTransform>(Snippet));

  EXPECT_EQ("int ((* const*target)) = nullptr;",
            runCheckOnCode<PointeeLTransform>(Snippet));
  EXPECT_EQ("int ((* const*target)) = nullptr;",
            runCheckOnCode<PointeeRTransform>(Snippet));
}
TEST(Pointers, Auto) {
  llvm::StringRef T = "int* f() { return nullptr; }\n";
  llvm::StringRef S = "auto* target = f();";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("auto* const target = f();"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("auto* const target = f();"),
            runCheckOnCode<ValueRTransform>(Cat(S)));

  EXPECT_EQ(Cat("const auto* target = f();"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));
  EXPECT_EQ(Cat("auto const* target = f();"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}
TEST(Pointers, AutoParens) {
  llvm::StringRef T = "int* f() { return nullptr; }\n";
  llvm::StringRef S = "auto (((* target))) = f();";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("auto (((* const target))) = f();"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("auto (((* const target))) = f();"),
            runCheckOnCode<ValueRTransform>(Cat(S)));

  EXPECT_EQ(Cat("const auto (((* target))) = f();"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));
  EXPECT_EQ(Cat("auto  const(((* target))) = f();"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}
TEST(Pointers, FunctionPointer) {
  llvm::StringRef S = "int (*target)(float, int, double) = nullptr;";

  EXPECT_EQ("int (*const target)(float, int, double) = nullptr;",
            runCheckOnCode<ValueLTransform>(S));
  EXPECT_EQ("int (*const target)(float, int, double) = nullptr;",
            runCheckOnCode<ValueRTransform>(S));

  EXPECT_EQ("int (*const target)(float, int, double) = nullptr;",
            runCheckOnCode<PointeeLTransform>(S));
  EXPECT_EQ("int (*const target)(float, int, double) = nullptr;",
            runCheckOnCode<PointeeRTransform>(S));

  S = "int (((*target)))(float, int, double) = nullptr;";
  EXPECT_EQ("int (((*const target)))(float, int, double) = nullptr;",
            runCheckOnCode<PointeeRTransform>(S));
}
TEST(Pointers, MemberFunctionPointer) {
  llvm::StringRef T = "struct A { int f() { return 1; } };";
  llvm::StringRef S = "int (A::*target)() = &A::f;";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("int (A::*const target)() = &A::f;"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("int (A::*const target)() = &A::f;"),
            runCheckOnCode<ValueRTransform>(Cat(S)));

  EXPECT_EQ(Cat("int (A::*const target)() = &A::f;"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));
  EXPECT_EQ(Cat("int (A::*const target)() = &A::f;"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));

  S = "int (A::*((target)))() = &A::f;";
  EXPECT_EQ(Cat("int (A::*const ((target)))() = &A::f;"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
}
TEST(Pointers, MemberDataPointer) {
  llvm::StringRef T = "struct A { int member = 0; };";
  llvm::StringRef S = "int A::*target = &A::member;";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("int A::*const target = &A::member;"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("int A::*const target = &A::member;"),
            runCheckOnCode<ValueRTransform>(Cat(S)));

  EXPECT_EQ(Cat("int A::*const target = &A::member;"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));
  EXPECT_EQ(Cat("int A::*const target = &A::member;"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));

  S = "int A::*((target)) = &A::member;";
  EXPECT_EQ(Cat("int A::*const ((target)) = &A::member;"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}

// ----------------------------------------------------------------------------
// Test TagTypes (struct, class, unions, enums)
// ----------------------------------------------------------------------------

TEST(TagTypes, Struct) {
  llvm::StringRef T = "struct Foo { int data; int method(); };\n";
  llvm::StringRef S = "struct Foo target{0};";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("const struct Foo target{0};"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("const struct Foo target{0};"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));

  EXPECT_EQ(Cat("struct Foo const target{0};"),
            runCheckOnCode<ValueRTransform>(Cat(S)));
  EXPECT_EQ(Cat("struct Foo const target{0};"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));

  S = "Foo target{0};";
  EXPECT_EQ(Cat("const Foo target{0};"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("const Foo target{0};"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));

  EXPECT_EQ(Cat("Foo const target{0};"),
            runCheckOnCode<ValueRTransform>(Cat(S)));
  EXPECT_EQ(Cat("Foo const target{0};"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));

  S = "Foo (target){0};";
  EXPECT_EQ(Cat("const Foo (target){0};"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("const Foo (target){0};"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));

  EXPECT_EQ(Cat("Foo const (target){0};"),
            runCheckOnCode<ValueRTransform>(Cat(S)));
  EXPECT_EQ(Cat("Foo const (target){0};"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));

  S = "struct S { int i; } target = { 0 };";
  EXPECT_EQ("const struct S { int i; } target = { 0 };",
            runCheckOnCode<ValueLTransform>(S));
  EXPECT_EQ("const struct S { int i; } target = { 0 };",
            runCheckOnCode<PointeeLTransform>(S));

  EXPECT_EQ("struct S { int i; } const target = { 0 };",
            runCheckOnCode<PointeeRTransform>(S));
  EXPECT_EQ("struct S { int i; } const target = { 0 };",
            runCheckOnCode<PointeeRTransform>(S));

  S = "struct { int i; } target = { 0 };";
  EXPECT_EQ("const struct { int i; } target = { 0 };",
            runCheckOnCode<ValueLTransform>(S));
  EXPECT_EQ("const struct { int i; } target = { 0 };",
            runCheckOnCode<PointeeLTransform>(S));

  EXPECT_EQ("struct { int i; } const target = { 0 };",
            runCheckOnCode<PointeeRTransform>(S));
  EXPECT_EQ("struct { int i; } const target = { 0 };",
            runCheckOnCode<PointeeRTransform>(S));
}
TEST(TagTypes, Class) {
  llvm::StringRef T = "class Foo { int data; int method(); };\n";
  llvm::StringRef S = "class Foo target;";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("const class Foo target;"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("const class Foo target;"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));

  EXPECT_EQ(Cat("class Foo const target;"),
            runCheckOnCode<ValueRTransform>(Cat(S)));
  EXPECT_EQ(Cat("class Foo const target;"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));

  S = "Foo target;";
  EXPECT_EQ(Cat("const Foo target;"), runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("const Foo target;"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));

  EXPECT_EQ(Cat("Foo const target;"), runCheckOnCode<ValueRTransform>(Cat(S)));
  EXPECT_EQ(Cat("Foo const target;"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));

  S = "Foo (target);";
  EXPECT_EQ(Cat("const Foo (target);"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("const Foo (target);"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));

  EXPECT_EQ(Cat("Foo const (target);"),
            runCheckOnCode<ValueRTransform>(Cat(S)));
  EXPECT_EQ(Cat("Foo const (target);"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}
TEST(TagTypes, Enum) {
  llvm::StringRef T = "enum Foo { N_ONE, N_TWO, N_THREE };\n";
  llvm::StringRef S = "enum Foo target;";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("const enum Foo target;"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("const enum Foo target;"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));

  EXPECT_EQ(Cat("enum Foo const target;"),
            runCheckOnCode<ValueRTransform>(Cat(S)));
  EXPECT_EQ(Cat("enum Foo const target;"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));

  S = "Foo target;";
  EXPECT_EQ(Cat("const Foo target;"), runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("const Foo target;"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));

  EXPECT_EQ(Cat("Foo const target;"), runCheckOnCode<ValueRTransform>(Cat(S)));
  EXPECT_EQ(Cat("Foo const target;"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));

  S = "Foo (target);";
  EXPECT_EQ(Cat("const Foo (target);"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("const Foo (target);"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));

  EXPECT_EQ(Cat("Foo const (target);"),
            runCheckOnCode<ValueRTransform>(Cat(S)));
  EXPECT_EQ(Cat("Foo const (target);"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}
TEST(TagTypes, Union) {
  llvm::StringRef T = "union Foo { int yay; float nej; };\n";
  llvm::StringRef S = "union Foo target;";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("const union Foo target;"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("const union Foo target;"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));

  EXPECT_EQ(Cat("union Foo const target;"),
            runCheckOnCode<ValueRTransform>(Cat(S)));
  EXPECT_EQ(Cat("union Foo const target;"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));

  S = "Foo target;";
  EXPECT_EQ(Cat("const Foo target;"), runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("const Foo target;"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));

  EXPECT_EQ(Cat("Foo const target;"), runCheckOnCode<ValueRTransform>(Cat(S)));
  EXPECT_EQ(Cat("Foo const target;"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));

  S = "Foo (target);";
  EXPECT_EQ(Cat("const Foo (target);"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("const Foo (target);"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));

  EXPECT_EQ(Cat("Foo const (target);"),
            runCheckOnCode<ValueRTransform>(Cat(S)));
  EXPECT_EQ(Cat("Foo const (target);"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}

// ----------------------------------------------------------------------------
// Test Macro expansions.
// ----------------------------------------------------------------------------

TEST(Macro, AllInMacro) {
  llvm::StringRef T = "#define DEFINE_VARIABLE int target = 42\n";
  llvm::StringRef S = "DEFINE_VARIABLE;";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("DEFINE_VARIABLE;"), runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("DEFINE_VARIABLE;"), runCheckOnCode<ValueRTransform>(Cat(S)));

  EXPECT_EQ(Cat("DEFINE_VARIABLE;"), runCheckOnCode<PointeeLTransform>(Cat(S)));
  EXPECT_EQ(Cat("DEFINE_VARIABLE;"), runCheckOnCode<PointeeRTransform>(Cat(S)));
}
TEST(Macro, MacroParameter) {
  llvm::StringRef T = "#define DEFINE_VARIABLE(X) int X = 42\n";
  llvm::StringRef S = "DEFINE_VARIABLE(target);";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("DEFINE_VARIABLE(target);"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("DEFINE_VARIABLE(target);"),
            runCheckOnCode<ValueRTransform>(Cat(S)));

  EXPECT_EQ(Cat("DEFINE_VARIABLE(target);"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));
  EXPECT_EQ(Cat("DEFINE_VARIABLE(target);"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}
TEST(Macro, MacroTypeValue) {
  llvm::StringRef T = "#define BAD_TYPEDEF int\n";
  llvm::StringRef S = "BAD_TYPEDEF target = 42;";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("BAD_TYPEDEF target = 42;"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("BAD_TYPEDEF target = 42;"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));

  EXPECT_EQ(Cat("BAD_TYPEDEF const target = 42;"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
  EXPECT_EQ(Cat("BAD_TYPEDEF const target = 42;"),
            runCheckOnCode<ValueRTransform>(Cat(S)));
}
TEST(Macro, MacroTypePointer) {
  llvm::StringRef T = "#define BAD_TYPEDEF int *\n";
  llvm::StringRef S = "BAD_TYPEDEF target = nullptr;";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("BAD_TYPEDEF const target = nullptr;"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("BAD_TYPEDEF const target = nullptr;"),
            runCheckOnCode<ValueRTransform>(Cat(S)));

  // FIXME: Failing even all parts seem to bail-out in for isMacroID()
  // The macro itself is changed here and below which is not intended.
  EXPECT_NE(Cat("BAD_TYPEDEF target = nullptr;"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
  EXPECT_EQ(Cat("BAD_TYPEDEF target = nullptr;"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));
}
TEST(Macro, MacroTypeReference) {
  llvm::StringRef T = "static int g = 42;\n#define BAD_TYPEDEF int&\n";
  llvm::StringRef S = "BAD_TYPEDEF target = g;";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("BAD_TYPEDEF target = g;"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  // FIXME: Failing even all parts seem to bail-out in for isMacroID()
  EXPECT_NE(Cat("BAD_TYPEDEF target = g;"),
            runCheckOnCode<ValueRTransform>(Cat(S)));

  EXPECT_EQ(Cat("BAD_TYPEDEF target = g;"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));
  // FIXME: Failing even all parts seem to bail-out in for isMacroID()
  EXPECT_NE(Cat("BAD_TYPEDEF target = g;"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}
// This failed in LLVM.
TEST(Macro, Variable) {
  llvm::StringRef M = "#define DEBUG(X) do { if (1) { X; } } while (0)\n";
  llvm::StringRef F = "void foo() ";
  llvm::StringRef V = "{ DEBUG(int target = 42;); }";

  auto Cat = [&](llvm::StringRef S) { return (M + F + V).str(); };

  EXPECT_EQ(Cat("{ DEBUG(const int target = 42;); }"),
            runCheckOnCode<ValueLTransform>(Cat(V)));
  EXPECT_EQ(Cat("{ DEBUG(int const target = 42;); }"),
            runCheckOnCode<ValueRTransform>(Cat(V)));
}
TEST(Macro, RangeLoop) {
  llvm::StringRef M = "#define DEBUG(X) do { if (1) { X; }} while (false)\n";
  llvm::StringRef F = "void foo() { char array[] = {'a', 'b', 'c'}; ";
  llvm::StringRef V = "DEBUG( for(auto& target: array) 10 + target; );";
  llvm::StringRef E = "}";

  auto Cat = [&](llvm::StringRef S) { return (M + F + V + E).str(); };

  EXPECT_EQ(Cat("DEBUG( for(const auto& target: array); );"),
            runCheckOnCode<ValueLTransform>(Cat(V)));
  EXPECT_EQ(Cat("DEBUG( for(auto const& target: array); );"),
            runCheckOnCode<ValueRTransform>(Cat(V)));
}

// ----------------------------------------------------------------------------
// Test template code.
// ----------------------------------------------------------------------------

TEST(Template, TemplateVariable) {
  llvm::StringRef T = "template <typename T> T target = 3.1415;";

  EXPECT_EQ("template <typename T> const T target = 3.1415;",
            runCheckOnCode<ValueLTransform>(T));
  EXPECT_EQ("template <typename T> T const target = 3.1415;",
            runCheckOnCode<ValueRTransform>(T));

  EXPECT_EQ("template <typename T> const T target = 3.1415;",
            runCheckOnCode<PointeeLTransform>(T));
  EXPECT_EQ("template <typename T> T const target = 3.1415;",
            runCheckOnCode<PointeeRTransform>(T));
}
TEST(Template, FunctionValue) {
  llvm::StringRef T = "template <typename T> void f(T v) \n";
  llvm::StringRef S = "{ T target = v; }";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("{ const T target = v; }"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("{ T const target = v; }"),
            runCheckOnCode<ValueRTransform>(Cat(S)));

  EXPECT_EQ(Cat("{ const T target = v; }"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));
  EXPECT_EQ(Cat("{ T const target = v; }"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}
TEST(Template, FunctionPointer) {
  llvm::StringRef T = "template <typename T> void f(T* v) \n";
  llvm::StringRef S = "{ T* target = v; }";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("{ T* const target = v; }"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("{ T* const target = v; }"),
            runCheckOnCode<ValueRTransform>(Cat(S)));

  EXPECT_EQ(Cat("{ const T* target = v; }"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));
  EXPECT_EQ(Cat("{ T const* target = v; }"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}
TEST(Template, FunctionReference) {
  llvm::StringRef T = "template <typename T> void f(T& v) \n";
  llvm::StringRef S = "{ T& target = v; }";
  auto Cat = [&T](llvm::StringRef S) { return (T + S).str(); };

  EXPECT_EQ(Cat("{ const T& target = v; }"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("{ T const& target = v; }"),
            runCheckOnCode<ValueRTransform>(Cat(S)));

  EXPECT_EQ(Cat("{ const T& target = v; }"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));
  EXPECT_EQ(Cat("{ T const& target = v; }"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}
TEST(Template, MultiInstantiationsFunction) {
  llvm::StringRef T = "template <typename T> void f(T v) \n";
  llvm::StringRef S = "{ T target = v; }";
  llvm::StringRef InstantStart = "void calls() {\n";
  llvm::StringRef InstValue = "f<int>(42);\n";
  llvm::StringRef InstConstValue = "f<const int>(42);\n";
  llvm::StringRef InstPointer = "f<int*>(nullptr);\n";
  llvm::StringRef InstPointerConst = "f<int* const>(nullptr);\n";
  llvm::StringRef InstConstPointer = "f<const int*>(nullptr);\n";
  llvm::StringRef InstConstPointerConst = "f<const int* const>(nullptr);\n";
  llvm::StringRef InstRef = "int i = 42;\nf<int&>(i);\n";
  llvm::StringRef InstConstRef = "f<const int&>(i);\n";
  llvm::StringRef InstantEnd = "}";
  auto Cat = [&](llvm::StringRef Target) {
    return (T + Target + InstantStart + InstValue + InstConstValue +
            InstPointer + InstPointerConst + InstConstPointer +
            InstConstPointerConst + InstRef + InstConstRef + InstantEnd)
        .str();
  };

  EXPECT_EQ(Cat("{ const T target = v; }"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("{ T const target = v; }"),
            runCheckOnCode<ValueRTransform>(Cat(S)));

  EXPECT_EQ(Cat("{ const T target = v; }"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));
  EXPECT_EQ(Cat("{ T const target = v; }"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}

TEST(Template, StructValue) {
  llvm::StringRef T = "template <typename T> struct S { void f(T& v) \n";
  llvm::StringRef S = "{ T target = v; }";
  llvm::StringRef End = "\n};";
  auto Cat = [&T, &End](llvm::StringRef S) { return (T + S + End).str(); };

  EXPECT_EQ(Cat("{ const T target = v; }"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("{ T const target = v; }"),
            runCheckOnCode<ValueRTransform>(Cat(S)));

  EXPECT_EQ(Cat("{ const T target = v; }"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));
  EXPECT_EQ(Cat("{ T const target = v; }"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}
TEST(Template, StructPointer) {
  llvm::StringRef T = "template <typename T> struct S { void f(T* v) \n";
  llvm::StringRef S = "{ T* target = v; }";
  llvm::StringRef End = "\n};";
  auto Cat = [&T, &End](llvm::StringRef S) { return (T + S + End).str(); };

  EXPECT_EQ(Cat("{ T* const target = v; }"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("{ T* const target = v; }"),
            runCheckOnCode<ValueRTransform>(Cat(S)));

  EXPECT_EQ(Cat("{ const T* target = v; }"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));
  EXPECT_EQ(Cat("{ T const* target = v; }"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}
TEST(Template, StructReference) {
  llvm::StringRef T = "template <typename T> struct S { void f(T& v) \n";
  llvm::StringRef S = "{ T& target = v; }";
  llvm::StringRef End = "\n};";
  auto Cat = [&T, &End](llvm::StringRef S) { return (T + S + End).str(); };

  EXPECT_EQ(Cat("{ const T& target = v; }"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("{ T const& target = v; }"),
            runCheckOnCode<ValueRTransform>(Cat(S)));

  EXPECT_EQ(Cat("{ const T& target = v; }"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));
  EXPECT_EQ(Cat("{ T const& target = v; }"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}
TEST(Template, DependentReturnFunction) {
  llvm::StringRef TS = "template <typename T> struct TS { using value_type = T; };";
  llvm::StringRef T = "template <typename T> void foo() ";
  llvm::StringRef S = "{ typename T::value_type target; }";
  auto Cat = [&TS, &T](llvm::StringRef S) { return (TS + T + S).str(); };

  EXPECT_EQ(Cat("{ const typename T::value_type target; }"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("{ typename T::value_type const target; }"),
            runCheckOnCode<ValueRTransform>(Cat(S)));

  EXPECT_EQ(Cat("{ const typename T::value_type target; }"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));
  EXPECT_EQ(Cat("{ typename T::value_type const target; }"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}
TEST(Template, DependentReturnPointerFunction) {
  llvm::StringRef TS = "template <typename T> struct TS { using value_type = T; };";
  llvm::StringRef T = "template <typename T> void foo() ";
  llvm::StringRef S = "{ typename T::value_type *target; }";
  auto Cat = [&TS, &T](llvm::StringRef S) { return (TS + T + S).str(); };

  EXPECT_EQ(Cat("{ typename T::value_type *const target; }"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("{ typename T::value_type *const target; }"),
            runCheckOnCode<ValueRTransform>(Cat(S)));

  EXPECT_EQ(Cat("{ const typename T::value_type *target; }"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));
  EXPECT_EQ(Cat("{ typename T::value_type  const*target; }"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}
TEST(Template, DependentReturnReferenceFunction) {
  llvm::StringRef TS = "template <typename T> struct TS { using value_type = T; };";
  llvm::StringRef T = "template <typename T> void foo(T& f) ";
  llvm::StringRef S = "{ typename T::value_type &target = f; }";
  auto Cat = [&TS, &T](llvm::StringRef S) { return (TS + T + S).str(); };

  EXPECT_EQ(Cat("{ const typename T::value_type &target = f; }"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("{ typename T::value_type  const&target = f; }"),
            runCheckOnCode<ValueRTransform>(Cat(S)));

  EXPECT_EQ(Cat("{ const typename T::value_type &target = f; }"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));
  EXPECT_EQ(Cat("{ typename T::value_type  const&target = f; }"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}
TEST(Template, VectorLikeType) {
  llvm::StringRef TS = "template <typename T> struct TS { TS(const T&) {} }; ";
  llvm::StringRef T = "void foo() ";
  llvm::StringRef S = "{ TS<int> target(42); }";
  auto Cat = [&TS, &T](llvm::StringRef S) { return (TS + T + S).str(); };

  EXPECT_EQ(Cat("{ const TS<int> target(42); }"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("{ TS<int> const target(42); }"),
            runCheckOnCode<ValueRTransform>(Cat(S)));

  EXPECT_EQ(Cat("{ const TS<int> target(42); }"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));
  EXPECT_EQ(Cat("{ TS<int> const target(42); }"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}
TEST(Template, SpecializedTemplate) {
  llvm::StringRef TS = "template <typename T = int> struct TS { TS(const T&) {} }; ";
  llvm::StringRef TS2 = "template <> struct TS<double> { TS(const double&) {} }; ";
  llvm::StringRef T = "void foo() ";
  llvm::StringRef S = "{ TS<double> target(42.42); }";
  auto Cat = [&](llvm::StringRef S) { return (TS + TS2 + T + S).str(); };

  EXPECT_EQ(Cat("{ const TS<double> target(42.42); }"),
            runCheckOnCode<ValueLTransform>(Cat(S)));
  EXPECT_EQ(Cat("{ TS<double> const target(42.42); }"),
            runCheckOnCode<ValueRTransform>(Cat(S)));

  EXPECT_EQ(Cat("{ const TS<double> target(42.42); }"),
            runCheckOnCode<PointeeLTransform>(Cat(S)));
  EXPECT_EQ(Cat("{ TS<double> const target(42.42); }"),
            runCheckOnCode<PointeeRTransform>(Cat(S)));
}

// -----------------------------------------------------------------------------
// ObjC Pointers
// -----------------------------------------------------------------------------

TEST(ObjC, SimplePointers) {
  llvm::StringRef S = "int * target = 0;";
  EXPECT_EQ(runCheckOnCode<PointeeLTransform>(S, nullptr, "input.m"),
            "const int * target = 0;");
  EXPECT_EQ(runCheckOnCode<PointeeRTransform>(S, nullptr, "input.m"),
            "int  const* target = 0;");
  EXPECT_EQ(runCheckOnCode<ValueLTransform>(S, nullptr, "input.m"),
            "int * const target = 0;");
  EXPECT_EQ(runCheckOnCode<ValueRTransform>(S, nullptr, "input.m"),
            "int * const target = 0;");
}
TEST(ObjC, ClassPointer) {
  llvm::StringRef TB = "@class Object;\nint main() {\n";
  llvm::StringRef S = "Object *target;";
  llvm::StringRef TE = "\n}";
  auto Cat = [&](llvm::StringRef S) { return (TB + S + TE).str(); };

  // FIXME: Not done properly for some reason.
  EXPECT_NE(runCheckOnCode<PointeeLTransform>(Cat(S), nullptr, "input.m"),
            Cat("const Object *target;"));
  EXPECT_NE(runCheckOnCode<PointeeRTransform>(Cat(S), nullptr, "input.m"),
            Cat("Object  const*target;"));
  EXPECT_NE(runCheckOnCode<ValueLTransform>(Cat(S), nullptr, "input.m"),
            Cat("Object *const target;"));
  EXPECT_NE(runCheckOnCode<ValueRTransform>(Cat(S), nullptr, "input.m"),
            Cat("Object *const target;"));
}
TEST(ObjC, InterfacePointer) {
  llvm::StringRef TB = "@interface I\n";
  llvm::StringRef S = "- (void) foo: (int *) target;";
  llvm::StringRef TE = "\n@end";
  auto Cat = [&](llvm::StringRef S) { return (TB + S + TE).str(); };

  EXPECT_EQ(runCheckOnCode<PointeeLTransform>(Cat(S), nullptr, "input.m"),
            Cat("- (void) foo: (const int *) target;"));
  EXPECT_EQ(runCheckOnCode<PointeeRTransform>(Cat(S), nullptr, "input.m"),
            Cat("- (void) foo: (int  const*) target;"));
  // FIXME: These transformations are incorrect. ObjC seems to need
  // RParenSkipping which is not implemented.
  EXPECT_NE(runCheckOnCode<ValueLTransform>(Cat(S), nullptr, "input.m"),
            Cat("- (void) foo: (int * const) target;"));
  EXPECT_NE(runCheckOnCode<ValueRTransform>(Cat(S), nullptr, "input.m"),
            Cat("- (void) foo: (int * const) target;"));
}

} // namespace test
} // namespace tidy
} // namespace clang
