// RUN: %clang_cc1 -std=gnu11 -triple x86_64-unknown-unknown -ast-dump=json %s | FileCheck %s

int TestLocation = 0;
int TestIndent = 1 + (1);

void TestDeclStmt(void) {
  int x = 0;
  int y, z;
}

int TestOpaqueValueExpr = 0 ?: 1;

void TestUnaryOperatorExpr(void) {
  char T1 = 1;
  int T2 = 1;

  T1++;
  T2++;

  -T1;
  -T2;

  ~T1;
  ~T2;
}

void TestGenericSelectionExpressions(int i) {
  _Generic(i, int : 12);
  _Generic(i, int : 12, default : 0);
  _Generic(i, default : 0, int : 12);
  _Generic(i, int : 12, float : 10, default : 100);

  int j = _Generic(i, int : 12);
}

void TestLabelsAndGoto(void) {
  // Note: case and default labels are handled by TestSwitch().

label1:
  ;

  goto label2;

label2:
  0;

  void *ptr = &&label1;

  goto *ptr;
}

void TestSwitch(int i) {
  switch (i) {
  case 0:
    break;
  case 1:
  case 2:
    break;
  default:
    break;
  case 3 ... 5:
    break;
  }
}

void TestIf(_Bool b) {
  if (b)
    ;

  if (b) {}

  if (b)
    ;
  else
    ;

  if (b) {}
  else {}

  if (b)
    ;
  else if (b)
    ;

  if (b)
    ;
  else if (b)
    ;
  else
    ;
}

void TestIteration(_Bool b) {
  while (b)
    ;

  do
    ;
  while (b);

  for (int i = 0; i < 10; ++i)
    ;

  for (b; b; b)
    ;

  for (; b; b = !b)
    ;

  for (; b;)
    ;

  for (;; b = !b)
    ;

  for (;;)
    ;
}

void TestJumps(void) {
  // goto and computed goto was tested in TestLabelsAndGoto().

  while (1) {
    continue;
    break;
  }
  return;

  return TestSwitch(1);
}

void TestMiscStmts(void) {
  ({int a = 10; a;});
}

void TestLineNumbers(void) {
  int a;

#define FOO(x) x

#line 100000
  int b;

#line 200000
  FOO(1);

#undef FOO
}

void TestVLA(int n) {
  double vla[n];
}


// CHECK:       "kind": "VarDecl",
// CHECK-NEXT:  "loc": {
// CHECK-NEXT:   "offset": {{[0-9]+}},
// CHECK-NEXT:   "file": "{{.*}}",
// CHECK-NEXT:   "line": 3,
// CHECK-NEXT:   "col": 5,
// CHECK-NEXT:   "tokLen": 12
// CHECK-NEXT:  },
// CHECK-NEXT:  "range": {
// CHECK-NEXT:   "begin": {
// CHECK-NEXT:    "offset": {{[0-9]+}},
// CHECK-NEXT:    "col": 1,
// CHECK-NEXT:    "tokLen": 3
// CHECK-NEXT:   },
// CHECK-NEXT:   "end": {
// CHECK-NEXT:    "offset": {{[0-9]+}},
// CHECK-NEXT:    "col": 20,
// CHECK-NEXT:    "tokLen": 1
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "name": "TestLocation",
// CHECK-NEXT:  "mangledName": "TestLocation",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "init": "c",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "IntegerLiteral",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "value": "0"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "VarDecl",
// CHECK-LABEL: "name": "TestIndent",
// CHECK-NEXT:  "mangledName": "TestIndent",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "init": "c",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "BinaryOperator",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "opcode": "+",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "IntegerLiteral",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "value": "1"
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ParenExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "IntegerLiteral",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "value": "1"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CompoundStmt",
// CHECK:       "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "DeclStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "VarDecl",
// CHECK:           "name": "x",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "init": "c",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "IntegerLiteral",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "value": "0"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "DeclStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "VarDecl",
// CHECK:           "name": "y",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      }
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "VarDecl",
// CHECK:           "name": "z",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "VarDecl",
// CHECK-LABEL: "name": "TestOpaqueValueExpr",
// CHECK-NEXT:  "mangledName": "TestOpaqueValueExpr",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "init": "c",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "BinaryConditionalOperator",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "IntegerLiteral",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "value": "0"
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "OpaqueValueExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "IntegerLiteral",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "value": "0"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "OpaqueValueExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "IntegerLiteral",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "value": "0"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "IntegerLiteral",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "value": "1"
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CompoundStmt",
// CHECK:       "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "DeclStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "VarDecl",
// CHECK:           "isUsed": true,
// CHECK-NEXT:      "name": "T1",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "char"
// CHECK-NEXT:      },
// CHECK-NEXT:      "init": "c",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "char"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "IntegralCast",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "IntegerLiteral",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "value": "1"
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "DeclStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "VarDecl",
// CHECK:           "isUsed": true,
// CHECK-NEXT:      "name": "T2",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "init": "c",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "IntegerLiteral",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "value": "1"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "UnaryOperator",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "char"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "isPostfix": true,
// CHECK-NEXT:    "opcode": "++",
// CHECK-NEXT:    "canOverflow": false,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "DeclRefExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "char"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "lvalue",
// CHECK-NEXT:      "referencedDecl": {
// CHECK-NEXT:       "id": "0x{{.*}}",
// CHECK-NEXT:       "kind": "VarDecl",
// CHECK-NEXT:       "name": "T1",
// CHECK-NEXT:       "type": {
// CHECK-NEXT:        "qualType": "char"
// CHECK-NEXT:       }
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "UnaryOperator",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "isPostfix": true,
// CHECK-NEXT:    "opcode": "++",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "DeclRefExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "lvalue",
// CHECK-NEXT:      "referencedDecl": {
// CHECK-NEXT:       "id": "0x{{.*}}",
// CHECK-NEXT:       "kind": "VarDecl",
// CHECK-NEXT:       "name": "T2",
// CHECK-NEXT:       "type": {
// CHECK-NEXT:        "qualType": "int"
// CHECK-NEXT:       }
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "UnaryOperator",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "isPostfix": false,
// CHECK-NEXT:    "opcode": "-",
// CHECK-NEXT:    "canOverflow": false,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "IntegralCast",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "char"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "LValueToRValue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "char"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "VarDecl",
// CHECK-NEXT:           "name": "T1",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "char"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "UnaryOperator",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "isPostfix": false,
// CHECK-NEXT:    "opcode": "-",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "VarDecl",
// CHECK-NEXT:         "name": "T2",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "int"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "UnaryOperator",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "isPostfix": false,
// CHECK-NEXT:    "opcode": "~",
// CHECK-NEXT:    "canOverflow": false,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "IntegralCast",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "char"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "LValueToRValue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "char"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "VarDecl",
// CHECK-NEXT:           "name": "T1",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "char"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "UnaryOperator",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "isPostfix": false,
// CHECK-NEXT:    "opcode": "~",
// CHECK-NEXT:    "canOverflow": false,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "VarDecl",
// CHECK-NEXT:         "name": "T2",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "int"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CompoundStmt",
// CHECK:       "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "GenericSelectionExpr",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "i",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "int"
// CHECK-NEXT:         }
// CHECK-NEXT:        },
// CHECK-NEXT:        "nonOdrUseReason": "unevaluated"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "BuiltinType",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      }
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "associationKind": "case",
// CHECK-NEXT:      "selected": true,
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "BuiltinType",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        }
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "IntegerLiteral",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "value": "12"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "GenericSelectionExpr",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "i",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "int"
// CHECK-NEXT:         }
// CHECK-NEXT:        },
// CHECK-NEXT:        "nonOdrUseReason": "unevaluated"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "BuiltinType",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      }
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "associationKind": "case",
// CHECK-NEXT:      "selected": true,
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "BuiltinType",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        }
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "IntegerLiteral",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "value": "12"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "associationKind": "default",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "IntegerLiteral",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "value": "0"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "GenericSelectionExpr",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "i",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "int"
// CHECK-NEXT:         }
// CHECK-NEXT:        },
// CHECK-NEXT:        "nonOdrUseReason": "unevaluated"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "BuiltinType",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      }
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "associationKind": "default",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "IntegerLiteral",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "value": "0"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "associationKind": "case",
// CHECK-NEXT:      "selected": true,
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "BuiltinType",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        }
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "IntegerLiteral",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "value": "12"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "GenericSelectionExpr",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "i",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "int"
// CHECK-NEXT:         }
// CHECK-NEXT:        },
// CHECK-NEXT:        "nonOdrUseReason": "unevaluated"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "BuiltinType",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      }
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "associationKind": "case",
// CHECK-NEXT:      "selected": true,
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "BuiltinType",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        }
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "IntegerLiteral",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "value": "12"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "associationKind": "case",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "BuiltinType",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "float"
// CHECK-NEXT:        }
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "IntegerLiteral",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "value": "10"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "associationKind": "default",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "IntegerLiteral",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "value": "100"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "DeclStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "VarDecl",
// CHECK:           "name": "j",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "init": "c",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "GenericSelectionExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "LValueToRValue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "ParmVarDecl",
// CHECK-NEXT:             "name": "i",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "int"
// CHECK-NEXT:             }
// CHECK-NEXT:            },
// CHECK-NEXT:            "nonOdrUseReason": "unevaluated"
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "BuiltinType",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          }
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "associationKind": "case",
// CHECK-NEXT:          "selected": true,
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "BuiltinType",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "int"
// CHECK-NEXT:            }
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "IntegerLiteral",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "value": "12"
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


// CHECK:       "kind": "CompoundStmt",
// CHECK:       "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "LabelStmt",
// CHECK:         "name": "label1",
// CHECK-NEXT:    "declId": "0x{{.*}}",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "NullStmt",
// CHECK:         ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "GotoStmt",
// CHECK:         "targetLabelDeclId": "0x{{.*}}"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "LabelStmt",
// CHECK:         "name": "label2",
// CHECK-NEXT:    "declId": "0x{{.*}}",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "IntegerLiteral",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "value": "0"
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "DeclStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "VarDecl",
// CHECK:           "isUsed": true,
// CHECK-NEXT:      "name": "ptr",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "void *"
// CHECK-NEXT:      },
// CHECK-NEXT:      "init": "c",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "AddrLabelExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "void *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "name": "label1",
// CHECK-NEXT:        "labelDeclId": "0x{{.*}}"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "IndirectGotoStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "const void *"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "NoOp",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "void *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "LValueToRValue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "void *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "VarDecl",
// CHECK-NEXT:           "name": "ptr",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "void *"
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


// CHECK:       "kind": "CompoundStmt",
// CHECK:       "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "SwitchStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "i",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "int"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CompoundStmt",
// CHECK:           "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CaseStmt",
// CHECK:             "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ConstantExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "IntegerLiteral",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "value": "0"
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "BreakStmt",
// CHECK:             ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CaseStmt",
// CHECK:             "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ConstantExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "IntegerLiteral",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "value": "1"
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CaseStmt",
// CHECK:               "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ConstantExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "IntegerLiteral",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "int"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "value": "2"
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "BreakStmt",
// CHECK:               ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DefaultStmt",
// CHECK:             "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "BreakStmt",
// CHECK:             ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CaseStmt",
// CHECK:             "isGNURange": true,
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ConstantExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "IntegerLiteral",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "value": "3"
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ConstantExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "IntegerLiteral",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "value": "5"
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "BreakStmt",
// CHECK:             ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CompoundStmt",
// CHECK:       "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "IfStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "_Bool"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "_Bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "b",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "_Bool"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "NullStmt",
// CHECK:         ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "IfStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "_Bool"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "_Bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "b",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "_Bool"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CompoundStmt",
// CHECK:         ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "IfStmt",
// CHECK:         "hasElse": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "_Bool"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "_Bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "b",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "_Bool"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "NullStmt",
// CHECK:           "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "NullStmt",
// CHECK:         ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "IfStmt",
// CHECK:         "hasElse": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "_Bool"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "_Bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "b",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "_Bool"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CompoundStmt",
// CHECK:           "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CompoundStmt",
// CHECK:         ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "IfStmt",
// CHECK:         "hasElse": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "_Bool"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "_Bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "b",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "_Bool"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "NullStmt",
// CHECK:           "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "IfStmt",
// CHECK:           "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "_Bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "LValueToRValue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "_Bool"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "ParmVarDecl",
// CHECK-NEXT:           "name": "b",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "_Bool"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "NullStmt",
// CHECK:           ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "IfStmt",
// CHECK:         "hasElse": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "_Bool"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "_Bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "b",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "_Bool"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "NullStmt",
// CHECK:           "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "IfStmt",
// CHECK:           "hasElse": true,
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "_Bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "LValueToRValue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "_Bool"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "ParmVarDecl",
// CHECK-NEXT:           "name": "b",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "_Bool"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "NullStmt",
// CHECK:             "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "NullStmt",
// CHECK:           ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CompoundStmt",
// CHECK:       "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "WhileStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "_Bool"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "_Bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "b",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "_Bool"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "NullStmt",
// CHECK:         ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "DoStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "NullStmt",
// CHECK:           "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "_Bool"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "_Bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "b",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "_Bool"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ForStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "DeclStmt",
// CHECK:           "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "VarDecl",
// CHECK:             "isUsed": true,
// CHECK-NEXT:        "name": "i",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "init": "c",
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
// CHECK-NEXT:     {},
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
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "LValueToRValue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "VarDecl",
// CHECK-NEXT:           "name": "i",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "int"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "IntegerLiteral",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "value": "10"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "UnaryOperator",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "isPostfix": false,
// CHECK-NEXT:      "opcode": "++",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "VarDecl",
// CHECK-NEXT:         "name": "i",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "int"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "NullStmt",
// CHECK:         ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ForStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "_Bool"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "_Bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "b",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "_Bool"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {},
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "_Bool"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "_Bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "b",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "_Bool"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "_Bool"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "_Bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "b",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "_Bool"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "NullStmt",
// CHECK:         ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ForStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {},
// CHECK-NEXT:     {},
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "_Bool"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "_Bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "b",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "_Bool"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "BinaryOperator",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "_Bool"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "opcode": "=",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "_Bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "b",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "_Bool"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "_Bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "IntegralToBoolean",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "UnaryOperator",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "isPostfix": false,
// CHECK-NEXT:          "opcode": "!",
// CHECK-NEXT:          "canOverflow": false,
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "_Bool"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "LValueToRValue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "DeclRefExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "_Bool"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "lvalue",
// CHECK-NEXT:              "referencedDecl": {
// CHECK-NEXT:               "id": "0x{{.*}}",
// CHECK-NEXT:               "kind": "ParmVarDecl",
// CHECK-NEXT:               "name": "b",
// CHECK-NEXT:               "type": {
// CHECK-NEXT:                "qualType": "_Bool"
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
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "NullStmt",
// CHECK:         ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ForStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {},
// CHECK-NEXT:     {},
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "_Bool"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "_Bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "b",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "_Bool"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {},
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "NullStmt",
// CHECK:         ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ForStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {},
// CHECK-NEXT:     {},
// CHECK-NEXT:     {},
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "BinaryOperator",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "_Bool"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "opcode": "=",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "_Bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "b",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "_Bool"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "_Bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "IntegralToBoolean",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "UnaryOperator",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "isPostfix": false,
// CHECK-NEXT:          "opcode": "!",
// CHECK-NEXT:          "canOverflow": false,
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "_Bool"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "LValueToRValue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "DeclRefExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "_Bool"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "lvalue",
// CHECK-NEXT:              "referencedDecl": {
// CHECK-NEXT:               "id": "0x{{.*}}",
// CHECK-NEXT:               "kind": "ParmVarDecl",
// CHECK-NEXT:               "name": "b",
// CHECK-NEXT:               "type": {
// CHECK-NEXT:                "qualType": "_Bool"
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
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "NullStmt",
// CHECK:         ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ForStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {},
// CHECK-NEXT:     {},
// CHECK-NEXT:     {},
// CHECK-NEXT:     {},
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "NullStmt",
// CHECK:         ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CompoundStmt",
// CHECK:       "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "WhileStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "IntegerLiteral",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "value": "1"
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CompoundStmt",
// CHECK:           "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ContinueStmt",
// CHECK:             "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "BreakStmt",
// CHECK:           ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ReturnStmt",
// CHECK:         "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ReturnStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CallExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "void"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "void (*)(int)"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "FunctionToPointerDecay",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "void (int)"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "FunctionDecl",
// CHECK-NEXT:           "name": "TestSwitch",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "void (int)"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "IntegerLiteral",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "value": "1"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CompoundStmt",
// CHECK:       "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "StmtExpr",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CompoundStmt",
// CHECK:           "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclStmt",
// CHECK:             "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "VarDecl",
// CHECK:               "isUsed": true,
// CHECK-NEXT:          "name": "a",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          },
// CHECK-NEXT:          "init": "c",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "IntegerLiteral",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "value": "10"
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "LValueToRValue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "VarDecl",
// CHECK-NEXT:           "name": "a",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "int"
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


// CHECK:       "kind": "CompoundStmt",
// CHECK:       "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "DeclStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "VarDecl",
// CHECK:           "name": "a",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "DeclStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "VarDecl",
// CHECK:           "name": "b",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "IntegerLiteral",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "value": "1"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CompoundStmt",
// CHECK:       "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "DeclStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "VarDecl",
// CHECK:           "name": "vla",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "double[n]"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }
