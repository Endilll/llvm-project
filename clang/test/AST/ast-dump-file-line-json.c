// RUN: %clang_cc1 -triple x86_64-unknown-unknown -ast-dump=json %s | FileCheck %s

#line 4 "test.c"
int a;

#line 32 "bar.h"
int b;
int c;

#line 11 "test.c"
int d;
int e;

// CHECK:         "kind": "VarDecl",
// CHECK-NEXT:    "loc": {
// CHECK-NEXT:     "offset": {{[0-9]+}},
// CHECK-NEXT:     "file": "{{.*}}",
// CHECK-NEXT:     "line": 4,
// CHECK-NEXT:     "presumedFile": "{{.*}}",
// CHECK-NEXT:     "col": {{[0-9]+}},
// CHECK-NEXT:     "tokLen": {{[0-9]+}}
// CHECK-NEXT:    },
// CHECK-LABEL:   "name": "a",
// CHECK-NEXT:    "mangledName": "a",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "VarDecl",
// CHECK-NEXT:    "loc": {
// CHECK-NEXT:     "offset": {{[0-9]+}},
// CHECK-NEXT:     "line": 7,
// CHECK-NEXT:     "presumedFile": "bar.h",
// CHECK-NEXT:     "presumedLine": 32,
// CHECK-NEXT:     "col": {{[0-9]+}},
// CHECK-NEXT:     "tokLen": {{[0-9]+}}
// CHECK-NEXT:    },
// CHECK-LABEL:   "name": "b",
// CHECK-NEXT:    "mangledName": "b",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "VarDecl",
// CHECK-NEXT:    "loc": {
// CHECK-NEXT:     "offset": {{[0-9]+}},
// CHECK-NEXT:     "line": 8,
// CHECK-NEXT:     "presumedLine": 33,
// CHECK-NEXT:     "col": {{[0-9]+}},
// CHECK-NEXT:     "tokLen": {{[0-9]+}}
// CHECK-NEXT:    },
// CHECK-LABEL:   "name": "c",
// CHECK-NEXT:    "mangledName": "c",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "VarDecl",
// CHECK-NEXT:    "loc": {
// CHECK-NEXT:     "offset": {{[0-9]+}},
// CHECK-NEXT:     "line": 11,
// CHECK-NEXT:     "presumedFile": "{{.*}}",
// CHECK-NEXT:     "col": {{[0-9]+}},
// CHECK-NEXT:     "tokLen": {{[0-9]+}}
// CHECK-NEXT:    },
// CHECK-LABEL:   "name": "d",
// CHECK-NEXT:    "mangledName": "d",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "VarDecl",
// CHECK-NEXT:    "loc": {
// CHECK-NEXT:     "offset": {{[0-9]+}},
// CHECK-NEXT:     "line": 12,
// CHECK-NEXT:     "col": {{[0-9]+}},
// CHECK-NEXT:     "tokLen": {{[0-9]+}}
// CHECK-NEXT:    },
// CHECK-LABEL:   "name": "e",
// CHECK-NEXT:    "mangledName": "e",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }
