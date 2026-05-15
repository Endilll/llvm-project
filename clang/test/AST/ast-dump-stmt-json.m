// RUN: %clang_cc1 -triple x86_64-pc-win32 -Wno-unused -fblocks -fobjc-exceptions -ast-dump=json -ast-dump-filter Test %s | FileCheck %s

void TestBlockExpr(int x) {
  ^{ x; };
}

void TestExprWithCleanup(int x) {
  ^{ x; };
}

@interface A
@end

void TestObjCAtCatchStmt(void) {
  @try {
  } @catch(A *a) {
  } @catch(...) {
  } @finally {
  }
}


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "TestBlockExpr",
// CHECK-LABEL: "mangledName": "TestBlockExpr",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (int)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isUsed": true,
// CHECK-NEXT:    "name": "x",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CompoundStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ExprWithCleanups",
// CHECK-NEXT:      "range": {
// CHECK-NEXT:       "begin": {
// CHECK-NEXT:        "offset": {{[0-9]+}},
// CHECK-NEXT:        "line": 4,
// CHECK-NEXT:        "col": 3,
// CHECK-NEXT:        "tokLen": 1
// CHECK-NEXT:       },
// CHECK-NEXT:       "end": {
// CHECK-NEXT:        "offset": {{[0-9]+}},
// CHECK-NEXT:        "col": 9,
// CHECK-NEXT:        "tokLen": 1
// CHECK-NEXT:       }
// CHECK-NEXT:      },
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "void (^)(void)"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "cleanupsHaveSideEffects": true,
// CHECK-NEXT:      "cleanups": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "BlockDecl"
// CHECK-NEXT:       }
// CHECK-NEXT:      ],
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "BlockExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "void (^)(void)"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "BlockDecl",
// CHECK:               "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "kind": "Capture",
// CHECK-NEXT:            "var": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "ParmVarDecl",
// CHECK-NEXT:             "name": "x",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "int"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CompoundStmt",
// CHECK:                 "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "ImplicitCastExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "int"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "castKind": "LValueToRValue",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "DeclRefExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "const int"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "lvalue",
// CHECK-NEXT:                "referencedDecl": {
// CHECK-NEXT:                 "id": "0x{{.*}}",
// CHECK-NEXT:                 "kind": "ParmVarDecl",
// CHECK-NEXT:                 "name": "x",
// CHECK-NEXT:                 "type": {
// CHECK-NEXT:                  "qualType": "int"
// CHECK-NEXT:                 }
// CHECK-NEXT:                }
// CHECK-NEXT:               }
// CHECK-NEXT:              ]
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "TestExprWithCleanup",
// CHECK-LABEL: "mangledName": "TestExprWithCleanup",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (int)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isUsed": true,
// CHECK-NEXT:    "name": "x",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CompoundStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ExprWithCleanups",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "void (^)(void)"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "cleanupsHaveSideEffects": true,
// CHECK-NEXT:      "cleanups": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "BlockDecl"
// CHECK-NEXT:       }
// CHECK-NEXT:      ],
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "BlockExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "void (^)(void)"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "BlockDecl",
// CHECK:               "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "kind": "Capture",
// CHECK-NEXT:            "var": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "ParmVarDecl",
// CHECK-NEXT:             "name": "x",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "int"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CompoundStmt",
// CHECK:                 "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "ImplicitCastExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "int"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "castKind": "LValueToRValue",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "DeclRefExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "const int"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "lvalue",
// CHECK-NEXT:                "referencedDecl": {
// CHECK-NEXT:                 "id": "0x{{.*}}",
// CHECK-NEXT:                 "kind": "ParmVarDecl",
// CHECK-NEXT:                 "name": "x",
// CHECK-NEXT:                 "type": {
// CHECK-NEXT:                  "qualType": "int"
// CHECK-NEXT:                 }
// CHECK-NEXT:                }
// CHECK-NEXT:               }
// CHECK-NEXT:              ]
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "TestObjCAtCatchStmt",
// CHECK-LABEL: "mangledName": "TestObjCAtCatchStmt",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (void)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CompoundStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ObjCAtTryStmt",
// CHECK:           "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CompoundStmt",
// CHECK:             "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ObjCAtCatchStmt",
// CHECK:             "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "VarDecl",
// CHECK:               "name": "a",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "A *"
// CHECK-NEXT:          }
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CompoundStmt",
// CHECK:             ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ObjCAtCatchStmt",
// CHECK:             "isCatchAll": true,
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CompoundStmt",
// CHECK:             ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ObjCAtFinallyStmt",
// CHECK:             "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CapturedStmt",
// CHECK:               "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CapturedDecl",
// CHECK:                 "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "CompoundStmt",
// CHECK:                   "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "ImplicitParamDecl",
// CHECK:                   "isImplicit": true,
// CHECK-NEXT:              "name": "__context",
// CHECK-NEXT:              "type": {
// CHECK-NEXT:               "qualType": "struct (unnamed at {{.*}}:18:14) *"
// CHECK-NEXT:              }
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }
