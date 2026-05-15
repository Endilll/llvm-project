// RUN: %clang_cc1 -triple x86_64-unknown-unknown -ast-dump=json %s | FileCheck %s

float func_16(float x, float y) {
  #pragma STDC FENV_ROUND FE_TOWARDZERO
  if (x < 0) {
    #pragma STDC FENV_ROUND FE_UPWARD
    return x - y;
  }
  return x + y;
}


// CHECK:       "kind": "CompoundStmt",
// CHECK:       "fpoptions": {
// CHECK-NEXT:   "ConstRoundingMode": 0
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "IfStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "BinaryOperator",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "opcode": "<",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "float"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "LValueToRValue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "float"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "ParmVarDecl",
// CHECK-NEXT:           "name": "x",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "float"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "float"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "IntegralToFloating",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "IntegerLiteral",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "value": "0"
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CompoundStmt",
// CHECK:           "fpoptions": {
// CHECK-NEXT:       "ConstRoundingMode": 2
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ReturnStmt",
// CHECK:             "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "BinaryOperator",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "float"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "opcode": "-",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "float"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "LValueToRValue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "DeclRefExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "float"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "lvalue",
// CHECK-NEXT:              "referencedDecl": {
// CHECK-NEXT:               "id": "0x{{.*}}",
// CHECK-NEXT:               "kind": "ParmVarDecl",
// CHECK-NEXT:               "name": "x",
// CHECK-NEXT:               "type": {
// CHECK-NEXT:                "qualType": "float"
// CHECK-NEXT:               }
// CHECK-NEXT:              }
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "float"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "LValueToRValue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "DeclRefExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "float"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "lvalue",
// CHECK-NEXT:              "referencedDecl": {
// CHECK-NEXT:               "id": "0x{{.*}}",
// CHECK-NEXT:               "kind": "ParmVarDecl",
// CHECK-NEXT:               "name": "y",
// CHECK-NEXT:               "type": {
// CHECK-NEXT:                "qualType": "float"
// CHECK-NEXT:               }
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
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ReturnStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "BinaryOperator",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "float"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "opcode": "+",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "float"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "LValueToRValue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "float"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "ParmVarDecl",
// CHECK-NEXT:           "name": "x",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "float"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "float"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "LValueToRValue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "float"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "ParmVarDecl",
// CHECK-NEXT:           "name": "y",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "float"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }
