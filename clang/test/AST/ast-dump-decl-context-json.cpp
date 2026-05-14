// RUN: %clang_cc1 -triple x86_64-unknown-unknown -ast-dump=json -ast-dump-filter Test %s | FileCheck %s

namespace Test {

namespace NS {
void Function();
}
void NS::Function() {}

struct S {
  void Method();
};
void S::Method() {}

} // namespace Test


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "NamespaceDecl",
// CHECK-LABEL: "name": "Test",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "NamespaceDecl",
// CHECK:         "isReferenced": true,
// CHECK-NEXT:    "name": "NS",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "FunctionDecl",
// CHECK:           "name": "Function",
// CHECK-NEXT:      "mangledName": "_ZN4Test2NS8FunctionEv",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "void ()"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FunctionDecl",
// CHECK:         "parentDeclContextId": "0x{{.*}}",
// CHECK-NEXT:    "previousDecl": "0x{{.*}}",
// CHECK:         "name": "Function",
// CHECK-NEXT:    "mangledName": "_ZN4Test2NS8FunctionEv",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void ()"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CompoundStmt",
// CHECK:         ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isReferenced": true,
// CHECK-NEXT:    "name": "S",
// CHECK-NEXT:    "tagUsed": "struct",
// CHECK-NEXT:    "completeDefinition": true,
// CHECK-NEXT:    "definitionData": {
// CHECK-NEXT:     "canConstDefaultInit": true,
// CHECK-NEXT:     "canPassInRegisters": true,
// CHECK-NEXT:     "copyAssign": {
// CHECK-NEXT:      "hasConstParam": true,
// CHECK-NEXT:      "implicitHasConstParam": true,
// CHECK-NEXT:      "needsImplicit": true,
// CHECK-NEXT:      "simple": true,
// CHECK-NEXT:      "trivial": true
// CHECK-NEXT:     },
// CHECK-NEXT:     "copyCtor": {
// CHECK-NEXT:      "hasConstParam": true,
// CHECK-NEXT:      "implicitHasConstParam": true,
// CHECK-NEXT:      "needsImplicit": true,
// CHECK-NEXT:      "simple": true,
// CHECK-NEXT:      "trivial": true
// CHECK-NEXT:     },
// CHECK-NEXT:     "defaultCtor": {
// CHECK-NEXT:      "defaultedIsConstexpr": true,
// CHECK-NEXT:      "exists": true,
// CHECK-NEXT:      "isConstexpr": true,
// CHECK-NEXT:      "needsImplicit": true,
// CHECK-NEXT:      "trivial": true
// CHECK-NEXT:     },
// CHECK-NEXT:     "dtor": {
// CHECK-NEXT:      "irrelevant": true,
// CHECK-NEXT:      "needsImplicit": true,
// CHECK-NEXT:      "simple": true,
// CHECK-NEXT:      "trivial": true
// CHECK-NEXT:     },
// CHECK-NEXT:     "hasConstexprNonCopyMoveConstructor": true,
// CHECK-NEXT:     "isAggregate": true,
// CHECK-NEXT:     "isEmpty": true,
// CHECK-NEXT:     "isLiteral": true,
// CHECK-NEXT:     "isPOD": true,
// CHECK-NEXT:     "isStandardLayout": true,
// CHECK-NEXT:     "isTrivial": true,
// CHECK-NEXT:     "isTriviallyCopyable": true,
// CHECK-NEXT:     "moveAssign": {
// CHECK-NEXT:      "exists": true,
// CHECK-NEXT:      "needsImplicit": true,
// CHECK-NEXT:      "simple": true,
// CHECK-NEXT:      "trivial": true
// CHECK-NEXT:     },
// CHECK-NEXT:     "moveCtor": {
// CHECK-NEXT:      "exists": true,
// CHECK-NEXT:      "needsImplicit": true,
// CHECK-NEXT:      "simple": true,
// CHECK-NEXT:      "trivial": true
// CHECK-NEXT:     }
// CHECK-NEXT:    },
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXRecordDecl",
// CHECK:           "isImplicit": true,
// CHECK-NEXT:      "name": "S",
// CHECK-NEXT:      "tagUsed": "struct"
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXMethodDecl",
// CHECK:           "name": "Method",
// CHECK-NEXT:      "mangledName": "_ZN4Test1S6MethodEv",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "void ()"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXMethodDecl",
// CHECK:         "parentDeclContextId": "0x{{.*}}",
// CHECK-NEXT:    "previousDecl": "0x{{.*}}",
// CHECK-NEXT:    "name": "Method",
// CHECK-NEXT:    "mangledName": "_ZN4Test1S6MethodEv",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void ()"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CompoundStmt",
