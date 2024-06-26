// RUN: %clang_cc1 -emit-llvm -debug-info-kind=limited %s -o - | FileCheck %s
// Insure that dbg.declare lines for locals refer to correct line number records.
void foo(void) {
  int l = 0;    // line #4: CHECK: #dbg_declare({{.*%l.*}} [[variable_l:![0-9]+]]
  int p = 0;    // line #5: CHECK: #dbg_declare({{.*%p.*}} [[variable_p:![0-9]+]]
}
// Now match the line number records:
// CHECK: {{^}}[[variable_l]] = !DILocation(line: 4,
// CHECK: {{^}}[[variable_p]] = !DILocation(line: 5,
