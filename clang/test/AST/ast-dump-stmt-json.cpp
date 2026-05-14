// RUN: %clang_cc1 -std=c++2a -triple x86_64-linux-gnu -fcxx-exceptions -ast-dump=json %s | FileCheck %s

namespace n {
void function() {}
int Variable;
}
using n::function;
using n::Variable;
void TestFunction() {
  void (*f)() = &function;
  Variable = 4;
}

void TestCatch1() {
  try {
  }
  catch (int x) {
  }
}

void TestCatch2() {
  try {
  }
  catch (...) {
  }
}

void TestAllocationExprs() {
  int *p;
  p = new int;
  delete p;
  p = new int[2];
  delete[] p;
  p = ::new int;
  ::delete p;
}

// Don't crash on dependent exprs that haven't been resolved yet.
template <typename T>
void TestDependentAllocationExpr() {
  T *p = new T;
  delete p;
}

template <typename T>
class DependentScopeMemberExprWrapper {
  T member;
};

template <typename T>
void TestDependentScopeMemberExpr() {
  DependentScopeMemberExprWrapper<T> obj;
  obj.member = T();
  obj.member = T{};
  (&obj)->member = T();
}

template <typename T>
class OtherDependentScopeMemberExprWrapper {
  template <typename U>
  static U member;
};

template <typename T>
void TestDependentScopeTemplateMemberExpr() {
  OtherDependentScopeMemberExprWrapper<T> obj;
  obj.template member<int> = int();
}

union U {
  int i;
  long l;
};

void TestUnionInitList()
{
  U us[3] = {1};
}

void TestSwitch(int i) {
  switch (int a; i)
    ;
}

void TestIf(bool b) {
  if (const int i = 12; i)
    ;

  if constexpr (sizeof(b) == 1)
    ;

  if constexpr (sizeof(b) == 1)
    ;
  else
    ;
}

struct Container {
  int *begin() const;
  int *end() const;
};

void TestIteration() {
  for (int i = 0; int j = i; ++i)
    ;

  int vals[10];
  for (int v : vals)
    ;

  Container C;
  for (int v : C)
    ;

  for (int a; int v : vals)
    ;
}

template <typename Ty>
void TestDependentGenericSelectionExpr(Ty T) {
  _Generic(T, int : 1, default : 0);
  _Generic(T, default : 0);
}


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "isUsed": true,
// CHECK-NEXT:  "name": "function",
// CHECK-NEXT:  "mangledName": "_ZN1n8functionEv",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void ()"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CompoundStmt",
// CHECK:       ]
// CHECK-NEXT: }


// CHECK:       "kind": "UsingDecl",
// CHECK-LABEL: "name": "n::function"
// CHECK-NEXT: }


// CHECK:       "kind": "UsingShadowDecl",
// CHECK:       "isImplicit": true,
// CHECK-NEXT:  "target": {
// CHECK-NEXT:   "id": "0x{{.*}}",
// CHECK-NEXT:   "kind": "FunctionDecl",
// CHECK-NEXT:   "name": "function",
// CHECK-NEXT:   "type": {
// CHECK-NEXT:    "qualType": "void ()"
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FunctionDecl",
// CHECK:         "isUsed": true,
// CHECK-NEXT:    "name": "function",
// CHECK-NEXT:    "mangledName": "_ZN1n8functionEv",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void ()"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CompoundStmt",
// CHECK:         ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }

// CHECK:       "kind": "UsingDecl",
// CHECK:       "name": "n::Variable"
// CHECK-NEXT: }

// CHECK:       "kind": "UsingShadowDecl",
// CHECK:       "isImplicit": true,
// CHECK-NEXT:  "target": {
// CHECK-NEXT:   "id": "0x{{.*}}",
// CHECK-NEXT:   "kind": "VarDecl",
// CHECK-NEXT:   "name": "Variable",
// CHECK-NEXT:   "type": {
// CHECK-NEXT:    "qualType": "int"
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "VarDecl",
// CHECK:         "isUsed": true,
// CHECK-NEXT:    "name": "Variable",
// CHECK-NEXT:    "mangledName": "_ZN1n8VariableE",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "TestFunction",
// CHECK-NEXT:  "mangledName": "_Z12TestFunctionv",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void ()"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CompoundStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "DeclStmt",
// CHECK:           "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "VarDecl",
// CHECK:             "name": "f",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "void (*)()"
// CHECK-NEXT:        },
// CHECK-NEXT:        "init": "c",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "UnaryOperator",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "void (*)()"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "isPostfix": false,
// CHECK-NEXT:          "opcode": "&",
// CHECK-NEXT:          "canOverflow": false,
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "void ()"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "FunctionDecl",
// CHECK-NEXT:             "name": "function",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "void ()"
// CHECK-NEXT:             }
// CHECK-NEXT:            },
// CHECK-NEXT:            "foundReferencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "UsingShadowDecl",
// CHECK-NEXT:             "name": "function"
// CHECK-NEXT:            }
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "BinaryOperator",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "lvalue",
// CHECK-NEXT:      "opcode": "=",
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
// CHECK-NEXT:         "name": "Variable",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "int"
// CHECK-NEXT:         }
// CHECK-NEXT:        },
// CHECK-NEXT:        "foundReferencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "UsingShadowDecl",
// CHECK-NEXT:         "name": "Variable"
// CHECK-NEXT:        }
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "IntegerLiteral",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "value": "4"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "TestCatch1",
// CHECK-NEXT:  "mangledName": "_Z10TestCatch1v",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void ()"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CompoundStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXTryStmt",
// CHECK:           "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CompoundStmt",
// CHECK:             "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CXXCatchStmt",
// CHECK:             "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "VarDecl",
// CHECK:               "name": "x",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          }
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CompoundStmt",
// CHECK:             ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "TestCatch2",
// CHECK-NEXT:  "mangledName": "_Z10TestCatch2v",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void ()"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CompoundStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXTryStmt",
// CHECK:           "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CompoundStmt",
// CHECK:             "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CXXCatchStmt",
// CHECK:             "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x0"
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CompoundStmt",
// CHECK:             ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "TestAllocationExprs",
// CHECK-NEXT:  "mangledName": "_Z19TestAllocationExprsv",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void ()"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CompoundStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "DeclStmt",
// CHECK:           "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "VarDecl",
// CHECK:             "isUsed": true,
// CHECK-NEXT:        "name": "p",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "int *"
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "BinaryOperator",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int *"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "lvalue",
// CHECK-NEXT:      "opcode": "=",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "VarDecl",
// CHECK-NEXT:         "name": "p",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "int *"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CXXNewExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "operatorNewDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "FunctionDecl",
// CHECK-NEXT:         "name": "operator new",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "void *(__size_t)"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXDeleteExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "void"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "operatorDeleteDecl": {
// CHECK-NEXT:       "id": "0x{{.*}}",
// CHECK-NEXT:       "kind": "FunctionDecl",
// CHECK-NEXT:       "name": "operator delete",
// CHECK-NEXT:       "type": {
// CHECK-NEXT:        "qualType": "void (void *, __size_t) noexcept"
// CHECK-NEXT:       }
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "LValueToRValue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "VarDecl",
// CHECK-NEXT:           "name": "p",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "int *"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "BinaryOperator",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int *"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "lvalue",
// CHECK-NEXT:      "opcode": "=",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "VarDecl",
// CHECK-NEXT:         "name": "p",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "int *"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CXXNewExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "isArray": true,
// CHECK-NEXT:        "operatorNewDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "FunctionDecl",
// CHECK-NEXT:         "name": "operator new[]",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "void *(__size_t)"
// CHECK-NEXT:         }
// CHECK-NEXT:        },
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "desugaredQualType": "unsigned long",
// CHECK-NEXT:           "qualType": "__size_t"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "IntegralCast",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "IntegerLiteral",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "value": "2"
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXDeleteExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "void"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "isArray": true,
// CHECK-NEXT:      "isArrayAsWritten": true,
// CHECK-NEXT:      "operatorDeleteDecl": {
// CHECK-NEXT:       "id": "0x{{.*}}",
// CHECK-NEXT:       "kind": "FunctionDecl",
// CHECK-NEXT:       "name": "operator delete[]",
// CHECK-NEXT:       "type": {
// CHECK-NEXT:        "qualType": "void (void *) noexcept"
// CHECK-NEXT:       }
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "LValueToRValue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "VarDecl",
// CHECK-NEXT:           "name": "p",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "int *"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "BinaryOperator",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int *"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "lvalue",
// CHECK-NEXT:      "opcode": "=",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "VarDecl",
// CHECK-NEXT:         "name": "p",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "int *"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CXXNewExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "isGlobal": true,
// CHECK-NEXT:        "operatorNewDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "FunctionDecl",
// CHECK-NEXT:         "name": "operator new",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "void *(__size_t)"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXDeleteExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "void"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "isGlobal": true,
// CHECK-NEXT:      "operatorDeleteDecl": {
// CHECK-NEXT:       "id": "0x{{.*}}",
// CHECK-NEXT:       "kind": "FunctionDecl",
// CHECK-NEXT:       "name": "operator delete",
// CHECK-NEXT:       "type": {
// CHECK-NEXT:        "qualType": "void (void *, __size_t) noexcept"
// CHECK-NEXT:       }
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "LValueToRValue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "VarDecl",
// CHECK-NEXT:           "name": "p",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "int *"
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


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "isImplicit": true,
// CHECK-NEXT:  "isUsed": true,
// CHECK-NEXT:  "name": "operator new",
// CHECK-NEXT:  "mangledName": "_Znwm",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void *(__size_t)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "desugaredQualType": "unsigned long",
// CHECK-NEXT:     "qualType": "__size_t"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "VisibilityAttr",
// CHECK:         "implicit": true,
// CHECK-NEXT:    "visibility": "default"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ReturnsNonNullAttr",
// CHECK:         "implicit": true
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "AllocSizeAttr",
// CHECK:         "implicit": true
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "isImplicit": true,
// CHECK-NEXT:  "name": "operator new",
// CHECK-NEXT:  "mangledName": "_ZnwmSt11align_val_t",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void *(__size_t, std::align_val_t)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "desugaredQualType": "unsigned long",
// CHECK-NEXT:     "qualType": "__size_t"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "std::align_val_t"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "VisibilityAttr",
// CHECK:         "implicit": true,
// CHECK-NEXT:    "visibility": "default"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ReturnsNonNullAttr",
// CHECK:         "implicit": true
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "AllocSizeAttr",
// CHECK:         "implicit": true
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "AllocAlignAttr",
// CHECK:         "implicit": true
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "isImplicit": true,
// CHECK-NEXT:  "isUsed": true,
// CHECK-NEXT:  "name": "operator new[]",
// CHECK-NEXT:  "mangledName": "_Znam",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void *(__size_t)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "desugaredQualType": "unsigned long",
// CHECK-NEXT:     "qualType": "__size_t"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "VisibilityAttr",
// CHECK:         "implicit": true,
// CHECK-NEXT:    "visibility": "default"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ReturnsNonNullAttr",
// CHECK:         "implicit": true
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "AllocSizeAttr",
// CHECK:         "implicit": true
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "isImplicit": true,
// CHECK-NEXT:  "name": "operator new[]",
// CHECK-NEXT:  "mangledName": "_ZnamSt11align_val_t",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void *(__size_t, std::align_val_t)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "desugaredQualType": "unsigned long",
// CHECK-NEXT:     "qualType": "__size_t"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "std::align_val_t"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "VisibilityAttr",
// CHECK:         "implicit": true,
// CHECK-NEXT:    "visibility": "default"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ReturnsNonNullAttr",
// CHECK:         "implicit": true
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "AllocSizeAttr",
// CHECK:         "implicit": true
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "AllocAlignAttr",
// CHECK:        "implicit": true
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "isImplicit": true,
// CHECK-NEXT:  "name": "operator delete",
// CHECK-NEXT:  "mangledName": "_ZdlPv",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (void *) noexcept"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void *"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "VisibilityAttr",
// CHECK:         "implicit": true,
// CHECK-NEXT:    "visibility": "default"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "isImplicit": true,
// CHECK-NEXT:  "name": "operator delete",
// CHECK-NEXT:  "mangledName": "_ZdlPvSt11align_val_t",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (void *, std::align_val_t) noexcept"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void *"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "std::align_val_t"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "VisibilityAttr",
// CHECK:         "implicit": true,
// CHECK-NEXT:    "visibility": "default"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "isImplicit": true,
// CHECK-NEXT:  "isUsed": true,
// CHECK-NEXT:  "name": "operator delete",
// CHECK-NEXT:  "mangledName": "_ZdlPvm",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (void *, __size_t) noexcept"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void *"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "desugaredQualType": "unsigned long",
// CHECK-NEXT:     "qualType": "__size_t"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "VisibilityAttr",
// CHECK:         "implicit": true,
// CHECK-NEXT:    "visibility": "default"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "isImplicit": true,
// CHECK-NEXT:  "name": "operator delete",
// CHECK-NEXT:  "mangledName": "_ZdlPvmSt11align_val_t",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (void *, __size_t, std::align_val_t) noexcept"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void *"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "desugaredQualType": "unsigned long",
// CHECK-NEXT:     "qualType": "__size_t"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "std::align_val_t"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "VisibilityAttr",
// CHECK:         "implicit": true,
// CHECK-NEXT:    "visibility": "default"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:      "kind": "FunctionDecl",
// CHECK:      "isImplicit": true,
// CHECK-NEXT:  "isUsed": true,
// CHECK-NEXT:  "name": "operator delete[]",
// CHECK-NEXT:  "mangledName": "_ZdaPv",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (void *) noexcept"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void *"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "VisibilityAttr",
// CHECK:         "implicit": true,
// CHECK-NEXT:    "visibility": "default"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "isImplicit": true,
// CHECK-NEXT:  "name": "operator delete[]",
// CHECK-NEXT:  "mangledName": "_ZdaPvSt11align_val_t",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (void *, std::align_val_t) noexcept"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void *"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:        "isImplicit": true,
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "std::align_val_t"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "VisibilityAttr",
// CHECK:         "implicit": true,
// CHECK-NEXT:    "visibility": "default"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "isImplicit": true,
// CHECK-NEXT:  "name": "operator delete[]",
// CHECK-NEXT:  "mangledName": "_ZdaPvm",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (void *, __size_t) noexcept"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void *"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "desugaredQualType": "unsigned long",
// CHECK-NEXT:     "qualType": "__size_t"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "VisibilityAttr",
// CHECK:         "implicit": true,
// CHECK-NEXT:    "visibility": "default"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "isImplicit": true,
// CHECK-NEXT:  "name": "operator delete[]",
// CHECK-NEXT:  "mangledName": "_ZdaPvmSt11align_val_t",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (void *, __size_t, std::align_val_t) noexcept"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void *"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "desugaredQualType": "unsigned long",
// CHECK-NEXT:     "qualType": "__size_t"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "std::align_val_t"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "VisibilityAttr",
// CHECK:         "implicit": true,
// CHECK-NEXT:    "visibility": "default"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionTemplateDecl",
// CHECK:       "name": "TestDependentAllocationExpr",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "TemplateTypeParmDecl",
// CHECK:         "isReferenced": true,
// CHECK-NEXT:    "name": "T",
// CHECK-NEXT:    "tagUsed": "typename",
// CHECK-NEXT:    "depth": 0,
// CHECK-NEXT:    "index": 0
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FunctionDecl",
// CHECK:         "name": "TestDependentAllocationExpr",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void ()"
// CHECK-NEXT:    },
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
// CHECK:               "isReferenced": true,
// CHECK-NEXT:          "name": "p",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "T *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "init": "c",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXNewExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "T *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue"
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CXXDeleteExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "void"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "T *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "VarDecl",
// CHECK-NEXT:           "name": "p",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "T *"
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


// CHECK:       "kind": "FunctionTemplateDecl",
// CHECK:       "name": "TestDependentScopeMemberExpr",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "TemplateTypeParmDecl",
// CHECK:         "isReferenced": true,
// CHECK-NEXT:    "name": "T",
// CHECK-NEXT:    "tagUsed": "typename",
// CHECK-NEXT:    "depth": 0,
// CHECK-NEXT:    "index": 0
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FunctionDecl",
// CHECK:         "name": "TestDependentScopeMemberExpr",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void ()"
// CHECK-NEXT:    },
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
// CHECK:               "isReferenced": true,
// CHECK-NEXT:          "name": "obj",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "DependentScopeMemberExprWrapper<T>"
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "BinaryOperator",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "<dependent type>"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "opcode": "=",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXDependentScopeMemberExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "<dependent type>"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "isArrow": false,
// CHECK-NEXT:          "member": "member",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "DependentScopeMemberExprWrapper<T>"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "VarDecl",
// CHECK-NEXT:             "name": "obj",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "DependentScopeMemberExprWrapper<T>"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXUnresolvedConstructExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "T"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue"
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "BinaryOperator",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "<dependent type>"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "opcode": "=",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXDependentScopeMemberExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "<dependent type>"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "isArrow": false,
// CHECK-NEXT:          "member": "member",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "DependentScopeMemberExprWrapper<T>"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "VarDecl",
// CHECK-NEXT:             "name": "obj",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "DependentScopeMemberExprWrapper<T>"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXUnresolvedConstructExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "T"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "list": true,
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "InitListExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "void"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue"
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "BinaryOperator",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "<dependent type>"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "opcode": "=",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXDependentScopeMemberExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "<dependent type>"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "isArrow": true,
// CHECK-NEXT:          "member": "member",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ParenExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "<dependent type>"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "UnaryOperator",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "<dependent type>"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "isPostfix": false,
// CHECK-NEXT:              "opcode": "&",
// CHECK-NEXT:              "canOverflow": false,
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "DeclRefExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "DependentScopeMemberExprWrapper<T>"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "lvalue",
// CHECK-NEXT:                "referencedDecl": {
// CHECK-NEXT:                 "id": "0x{{.*}}",
// CHECK-NEXT:                 "kind": "VarDecl",
// CHECK-NEXT:                 "name": "obj",
// CHECK-NEXT:                 "type": {
// CHECK-NEXT:                  "qualType": "DependentScopeMemberExprWrapper<T>"
// CHECK-NEXT:                 }
// CHECK-NEXT:                }
// CHECK-NEXT:               }
// CHECK-NEXT:              ]
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXUnresolvedConstructExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "T"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue"
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionTemplateDecl",
// CHECK:       "name": "TestDependentScopeTemplateMemberExpr",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "TemplateTypeParmDecl",
// CHECK:         "isReferenced": true,
// CHECK-NEXT:    "name": "T",
// CHECK-NEXT:    "tagUsed": "typename",
// CHECK-NEXT:    "depth": 0,
// CHECK-NEXT:    "index": 0
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FunctionDecl",
// CHECK:         "name": "TestDependentScopeTemplateMemberExpr",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void ()"
// CHECK-NEXT:    },
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
// CHECK:               "isReferenced": true,
// CHECK-NEXT:          "name": "obj",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "OtherDependentScopeMemberExprWrapper<T>"
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "BinaryOperator",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "<dependent type>"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "opcode": "=",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXDependentScopeMemberExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "<dependent type>"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "isArrow": false,
// CHECK-NEXT:          "member": "member",
// CHECK-NEXT:          "hasTemplateKeyword": true,
// CHECK-NEXT:          "hasExplicitTemplateArgs": true,
// CHECK-NEXT:          "explicitTemplateArgs": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "kind": "TemplateArgument",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int"
// CHECK-NEXT:            }
// CHECK-NEXT:           }
// CHECK-NEXT:          ],
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "OtherDependentScopeMemberExprWrapper<T>"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "VarDecl",
// CHECK-NEXT:             "name": "obj",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "OtherDependentScopeMemberExprWrapper<T>"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXScalarValueInitExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue"
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
// CHECK:       "name": "TestUnionInitList",
// CHECK-NEXT:  "mangledName": "_Z17TestUnionInitListv",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void ()"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CompoundStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "DeclStmt",
// CHECK:           "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "VarDecl",
// CHECK:             "name": "us",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "U[3]"
// CHECK-NEXT:        },
// CHECK-NEXT:        "init": "c",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "InitListExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "U[3]"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "array_filler": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "InitListExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "U"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "field": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "FieldDecl",
// CHECK-NEXT:             "name": "i",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "int"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "InitListExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "U"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "field": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "FieldDecl",
// CHECK-NEXT:             "name": "i",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "int"
// CHECK-NEXT:             }
// CHECK-NEXT:            },
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "IntegerLiteral",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "int"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "value": "1"
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
// CHECK:       "name": "TestSwitch",
// CHECK-NEXT:  "mangledName": "_Z10TestSwitchi",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (int)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isUsed": true,
// CHECK-NEXT:    "name": "i",
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
// CHECK-NEXT:      "kind": "SwitchStmt",
// CHECK:           "hasInit": true,
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclStmt",
// CHECK:             "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "VarDecl",
// CHECK:               "name": "a",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          }
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
// CHECK-NEXT:           "kind": "ParmVarDecl",
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
// CHECK-NEXT:        "kind": "NullStmt",
// CHECK:           ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "TestIf",
// CHECK-NEXT:  "mangledName": "_Z6TestIfb",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (bool)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isReferenced": true,
// CHECK-NEXT:    "name": "b",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "bool"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CompoundStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "IfStmt",
// CHECK:           "hasInit": true,
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclStmt",
// CHECK:             "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "VarDecl",
// CHECK:               "isReferenced": true,
// CHECK-NEXT:          "name": "i",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "const int"
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
// CHECK-NEXT:            "value": "12"
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "IntegralToBoolean",
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
// CHECK-NEXT:             "qualType": "const int"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "VarDecl",
// CHECK-NEXT:             "name": "i",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "const int"
// CHECK-NEXT:             }
// CHECK-NEXT:            },
// CHECK-NEXT:            "nonOdrUseReason": "constant"
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "NullStmt",
// CHECK:           ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "IfStmt",
// CHECK:           "isConstexpr": true,
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ConstantExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "value": "true",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "BinaryOperator",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "bool"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "opcode": "==",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "UnaryExprOrTypeTraitExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "desugaredQualType": "unsigned long",
// CHECK-NEXT:             "qualType": "__size_t"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "name": "sizeof",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "ParenExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "bool"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "lvalue",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "DeclRefExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "bool"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "lvalue",
// CHECK-NEXT:                "referencedDecl": {
// CHECK-NEXT:                 "id": "0x{{.*}}",
// CHECK-NEXT:                 "kind": "ParmVarDecl",
// CHECK-NEXT:                 "name": "b",
// CHECK-NEXT:                 "type": {
// CHECK-NEXT:                  "qualType": "bool"
// CHECK-NEXT:                 }
// CHECK-NEXT:                },
// CHECK-NEXT:                "nonOdrUseReason": "unevaluated"
// CHECK-NEXT:               }
// CHECK-NEXT:              ]
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "desugaredQualType": "unsigned long",
// CHECK-NEXT:             "qualType": "__size_t"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "IntegralCast",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "IntegerLiteral",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "int"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "value": "1"
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "NullStmt",
// CHECK:           ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "IfStmt",
// CHECK:           "hasElse": true,
// CHECK-NEXT:      "isConstexpr": true,
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ConstantExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "value": "true",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "BinaryOperator",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "bool"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "opcode": "==",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "UnaryExprOrTypeTraitExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "desugaredQualType": "unsigned long",
// CHECK-NEXT:             "qualType": "__size_t"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "name": "sizeof",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "ParenExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "bool"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "lvalue",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "DeclRefExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "bool"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "lvalue",
// CHECK-NEXT:                "referencedDecl": {
// CHECK-NEXT:                 "id": "0x{{.*}}",
// CHECK-NEXT:                 "kind": "ParmVarDecl",
// CHECK-NEXT:                 "name": "b",
// CHECK-NEXT:                 "type": {
// CHECK-NEXT:                  "qualType": "bool"
// CHECK-NEXT:                 }
// CHECK-NEXT:                },
// CHECK-NEXT:                "nonOdrUseReason": "unevaluated"
// CHECK-NEXT:               }
// CHECK-NEXT:              ]
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "desugaredQualType": "unsigned long",
// CHECK-NEXT:             "qualType": "__size_t"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "IntegralCast",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "IntegerLiteral",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "int"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "value": "1"
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
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


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "TestIteration",
// CHECK-NEXT:  "mangledName": "_Z13TestIterationv",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void ()"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CompoundStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ForStmt",
// CHECK:           "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclStmt",
// CHECK:             "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "VarDecl",
// CHECK:               "isUsed": true,
// CHECK-NEXT:          "name": "i",
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
// CHECK-NEXT:            "value": "0"
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclStmt",
// CHECK:             "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "VarDecl",
// CHECK:               "isUsed": true,
// CHECK-NEXT:          "name": "j",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          },
// CHECK-NEXT:          "init": "c",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "LValueToRValue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "DeclRefExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "int"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "lvalue",
// CHECK-NEXT:              "referencedDecl": {
// CHECK-NEXT:               "id": "0x{{.*}}",
// CHECK-NEXT:               "kind": "VarDecl",
// CHECK-NEXT:               "name": "i",
// CHECK-NEXT:               "type": {
// CHECK-NEXT:                "qualType": "int"
// CHECK-NEXT:               }
// CHECK-NEXT:              }
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "IntegralToBoolean",
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
// CHECK-NEXT:             "kind": "VarDecl",
// CHECK-NEXT:             "name": "j",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "int"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "UnaryOperator",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "isPostfix": false,
// CHECK-NEXT:        "opcode": "++",
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
// CHECK-NEXT:        "kind": "NullStmt",
// CHECK:           ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "DeclStmt",
// CHECK:           "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "VarDecl",
// CHECK:             "isUsed": true,
// CHECK-NEXT:        "name": "vals",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "int[10]"
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXForRangeStmt",
// CHECK:           "inner": [
// CHECK-NEXT:       {},
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclStmt",
// CHECK:             "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "VarDecl",
// CHECK:               "isImplicit": true,
// CHECK-NEXT:          "isUsed": true,
// CHECK-NEXT:          "name": "__range1",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "int (&)[10]"
// CHECK-NEXT:          },
// CHECK-NEXT:          "init": "c",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int[10]"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "VarDecl",
// CHECK-NEXT:             "name": "vals",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "int[10]"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclStmt",
// CHECK:             "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "VarDecl",
// CHECK:               "isImplicit": true,
// CHECK-NEXT:          "isUsed": true,
// CHECK-NEXT:          "name": "__begin1",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "int *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "init": "c",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "ArrayToPointerDecay",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "DeclRefExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "int[10]"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "lvalue",
// CHECK-NEXT:              "referencedDecl": {
// CHECK-NEXT:               "id": "0x{{.*}}",
// CHECK-NEXT:               "kind": "VarDecl",
// CHECK-NEXT:               "name": "__range1",
// CHECK-NEXT:               "type": {
// CHECK-NEXT:                "qualType": "int (&)[10]"
// CHECK-NEXT:               }
// CHECK-NEXT:              }
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclStmt",
// CHECK:             "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "VarDecl",
// CHECK:               "isImplicit": true,
// CHECK-NEXT:          "isUsed": true,
// CHECK-NEXT:          "name": "__end1",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "int *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "init": "c",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "BinaryOperator",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "opcode": "+",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "ImplicitCastExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "int *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "castKind": "ArrayToPointerDecay",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "DeclRefExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "int[10]"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "lvalue",
// CHECK-NEXT:                "referencedDecl": {
// CHECK-NEXT:                 "id": "0x{{.*}}",
// CHECK-NEXT:                 "kind": "VarDecl",
// CHECK-NEXT:                 "name": "__range1",
// CHECK-NEXT:                 "type": {
// CHECK-NEXT:                  "qualType": "int (&)[10]"
// CHECK-NEXT:                 }
// CHECK-NEXT:                }
// CHECK-NEXT:               }
// CHECK-NEXT:              ]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "IntegerLiteral",
// CHECK:                   "type": {
// CHECK-NEXT:               "desugaredQualType": "long",
// CHECK-NEXT:               "qualType": "__ptrdiff_t"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "value": "10"
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "BinaryOperator",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "opcode": "!=",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "LValueToRValue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "VarDecl",
// CHECK-NEXT:             "name": "__begin1",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "int *"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "LValueToRValue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "VarDecl",
// CHECK-NEXT:             "name": "__end1",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "int *"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "UnaryOperator",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "isPostfix": false,
// CHECK-NEXT:        "opcode": "++",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "VarDecl",
// CHECK-NEXT:           "name": "__begin1",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "int *"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclStmt",
// CHECK:             "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "VarDecl",
// CHECK:               "name": "v",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          },
// CHECK-NEXT:          "init": "c",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "LValueToRValue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "UnaryOperator",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "int"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "lvalue",
// CHECK-NEXT:              "isPostfix": false,
// CHECK-NEXT:              "opcode": "*",
// CHECK-NEXT:              "canOverflow": false,
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "ImplicitCastExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "int *"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "prvalue",
// CHECK-NEXT:                "castKind": "LValueToRValue",
// CHECK-NEXT:                "inner": [
// CHECK-NEXT:                 {
// CHECK-NEXT:                  "id": "0x{{.*}}",
// CHECK-NEXT:                  "kind": "DeclRefExpr",
// CHECK:                       "type": {
// CHECK-NEXT:                   "qualType": "int *"
// CHECK-NEXT:                  },
// CHECK-NEXT:                  "valueCategory": "lvalue",
// CHECK-NEXT:                  "referencedDecl": {
// CHECK-NEXT:                   "id": "0x{{.*}}",
// CHECK-NEXT:                   "kind": "VarDecl",
// CHECK-NEXT:                   "name": "__begin1",
// CHECK-NEXT:                   "type": {
// CHECK-NEXT:                    "qualType": "int *"
// CHECK-NEXT:                   }
// CHECK-NEXT:                  }
// CHECK-NEXT:                 }
// CHECK-NEXT:                ]
// CHECK-NEXT:               }
// CHECK-NEXT:              ]
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "NullStmt",
// CHECK:           ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "DeclStmt",
// CHECK:           "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "VarDecl",
// CHECK:             "isUsed": true,
// CHECK-NEXT:        "name": "C",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "Container"
// CHECK-NEXT:        },
// CHECK-NEXT:        "init": "call",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXConstructExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "Container"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "ctorType": {
// CHECK-NEXT:           "qualType": "void () noexcept"
// CHECK-NEXT:          },
// CHECK-NEXT:          "hadMultipleCandidates": true,
// CHECK-NEXT:          "constructionKind": "complete"
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXForRangeStmt",
// CHECK:           "inner": [
// CHECK-NEXT:       {},
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclStmt",
// CHECK:             "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "VarDecl",
// CHECK:               "isImplicit": true,
// CHECK-NEXT:          "isUsed": true,
// CHECK-NEXT:          "name": "__range1",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "Container &"
// CHECK-NEXT:          },
// CHECK-NEXT:          "init": "c",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "Container"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "VarDecl",
// CHECK-NEXT:             "name": "C",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "Container"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclStmt",
// CHECK:             "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "VarDecl",
// CHECK:               "isImplicit": true,
// CHECK-NEXT:          "isUsed": true,
// CHECK-NEXT:          "name": "__begin1",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "int *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "init": "c",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXMemberCallExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "MemberExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "<bound member function type>"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "name": "begin",
// CHECK-NEXT:              "isArrow": false,
// CHECK-NEXT:              "referencedMemberDecl": "0x{{.*}}",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "ImplicitCastExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "const Container"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "lvalue",
// CHECK-NEXT:                "castKind": "NoOp",
// CHECK-NEXT:                "inner": [
// CHECK-NEXT:                 {
// CHECK-NEXT:                  "id": "0x{{.*}}",
// CHECK-NEXT:                  "kind": "DeclRefExpr",
// CHECK:                       "type": {
// CHECK-NEXT:                   "qualType": "Container"
// CHECK-NEXT:                  },
// CHECK-NEXT:                  "valueCategory": "lvalue",
// CHECK-NEXT:                  "referencedDecl": {
// CHECK-NEXT:                   "id": "0x{{.*}}",
// CHECK-NEXT:                   "kind": "VarDecl",
// CHECK-NEXT:                   "name": "__range1",
// CHECK-NEXT:                   "type": {
// CHECK-NEXT:                    "qualType": "Container &"
// CHECK-NEXT:                   }
// CHECK-NEXT:                  }
// CHECK-NEXT:                 }
// CHECK-NEXT:                ]
// CHECK-NEXT:               }
// CHECK-NEXT:              ]
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclStmt",
// CHECK:             "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "VarDecl",
// CHECK:               "isImplicit": true,
// CHECK-NEXT:          "isUsed": true,
// CHECK-NEXT:          "name": "__end1",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "int *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "init": "c",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXMemberCallExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "MemberExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "<bound member function type>"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "name": "end",
// CHECK-NEXT:              "isArrow": false,
// CHECK-NEXT:              "referencedMemberDecl": "0x{{.*}}",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "ImplicitCastExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "const Container"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "lvalue",
// CHECK-NEXT:                "castKind": "NoOp",
// CHECK-NEXT:                "inner": [
// CHECK-NEXT:                 {
// CHECK-NEXT:                  "id": "0x{{.*}}",
// CHECK-NEXT:                  "kind": "DeclRefExpr",
// CHECK:                       "type": {
// CHECK-NEXT:                   "qualType": "Container"
// CHECK-NEXT:                  },
// CHECK-NEXT:                  "valueCategory": "lvalue",
// CHECK-NEXT:                  "referencedDecl": {
// CHECK-NEXT:                   "id": "0x{{.*}}",
// CHECK-NEXT:                   "kind": "VarDecl",
// CHECK-NEXT:                   "name": "__range1",
// CHECK-NEXT:                   "type": {
// CHECK-NEXT:                    "qualType": "Container &"
// CHECK-NEXT:                   }
// CHECK-NEXT:                  }
// CHECK-NEXT:                 }
// CHECK-NEXT:                ]
// CHECK-NEXT:               }
// CHECK-NEXT:              ]
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "BinaryOperator",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "opcode": "!=",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "LValueToRValue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "VarDecl",
// CHECK-NEXT:             "name": "__begin1",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "int *"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "LValueToRValue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "VarDecl",
// CHECK-NEXT:             "name": "__end1",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "int *"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "UnaryOperator",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "isPostfix": false,
// CHECK-NEXT:        "opcode": "++",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "VarDecl",
// CHECK-NEXT:           "name": "__begin1",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "int *"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclStmt",
// CHECK:             "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "VarDecl",
// CHECK:               "name": "v",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          },
// CHECK-NEXT:          "init": "c",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "LValueToRValue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "UnaryOperator",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "int"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "lvalue",
// CHECK-NEXT:              "isPostfix": false,
// CHECK-NEXT:              "opcode": "*",
// CHECK-NEXT:              "canOverflow": false,
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "ImplicitCastExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "int *"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "prvalue",
// CHECK-NEXT:                "castKind": "LValueToRValue",
// CHECK-NEXT:                "inner": [
// CHECK-NEXT:                 {
// CHECK-NEXT:                  "id": "0x{{.*}}",
// CHECK-NEXT:                  "kind": "DeclRefExpr",
// CHECK:                       "type": {
// CHECK-NEXT:                   "qualType": "int *"
// CHECK-NEXT:                  },
// CHECK-NEXT:                  "valueCategory": "lvalue",
// CHECK-NEXT:                  "referencedDecl": {
// CHECK-NEXT:                   "id": "0x{{.*}}",
// CHECK-NEXT:                   "kind": "VarDecl",
// CHECK-NEXT:                   "name": "__begin1",
// CHECK-NEXT:                   "type": {
// CHECK-NEXT:                    "qualType": "int *"
// CHECK-NEXT:                   }
// CHECK-NEXT:                  }
// CHECK-NEXT:                 }
// CHECK-NEXT:                ]
// CHECK-NEXT:               }
// CHECK-NEXT:              ]
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "NullStmt",
// CHECK:           ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXForRangeStmt",
// CHECK:           "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclStmt",
// CHECK:             "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "VarDecl",
// CHECK:               "name": "a",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclStmt",
// CHECK:             "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "VarDecl",
// CHECK:               "isImplicit": true,
// CHECK-NEXT:          "isUsed": true,
// CHECK-NEXT:          "name": "__range1",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "int (&)[10]"
// CHECK-NEXT:          },
// CHECK-NEXT:          "init": "c",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int[10]"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "VarDecl",
// CHECK-NEXT:             "name": "vals",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "int[10]"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclStmt",
// CHECK:             "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "VarDecl",
// CHECK:               "isImplicit": true,
// CHECK-NEXT:          "isUsed": true,
// CHECK-NEXT:          "name": "__begin1",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "int *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "init": "c",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "ArrayToPointerDecay",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "DeclRefExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "int[10]"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "lvalue",
// CHECK-NEXT:              "referencedDecl": {
// CHECK-NEXT:               "id": "0x{{.*}}",
// CHECK-NEXT:               "kind": "VarDecl",
// CHECK-NEXT:               "name": "__range1",
// CHECK-NEXT:               "type": {
// CHECK-NEXT:                "qualType": "int (&)[10]"
// CHECK-NEXT:               }
// CHECK-NEXT:              }
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclStmt",
// CHECK:             "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "VarDecl",
// CHECK:               "isImplicit": true,
// CHECK-NEXT:          "isUsed": true,
// CHECK-NEXT:          "name": "__end1",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "int *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "init": "c",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "BinaryOperator",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "opcode": "+",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "ImplicitCastExpr",
// CHECK:                  "type": {
// CHECK-NEXT:               "qualType": "int *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "castKind": "ArrayToPointerDecay",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "DeclRefExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "int[10]"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "lvalue",
// CHECK-NEXT:                "referencedDecl": {
// CHECK-NEXT:                 "id": "0x{{.*}}",
// CHECK-NEXT:                 "kind": "VarDecl",
// CHECK-NEXT:                 "name": "__range1",
// CHECK-NEXT:                 "type": {
// CHECK-NEXT:                  "qualType": "int (&)[10]"
// CHECK-NEXT:                 }
// CHECK-NEXT:                }
// CHECK-NEXT:               }
// CHECK-NEXT:              ]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "IntegerLiteral",
// CHECK:                   "type": {
// CHECK-NEXT:               "desugaredQualType": "long"
// CHECK-NEXT:               "qualType": "__ptrdiff_t"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "value": "10"
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "BinaryOperator",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "opcode": "!=",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "LValueToRValue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "VarDecl",
// CHECK-NEXT:             "name": "__begin1",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "int *"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "LValueToRValue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "VarDecl",
// CHECK-NEXT:             "name": "__end1",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "int *"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "UnaryOperator",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "isPostfix": false,
// CHECK-NEXT:        "opcode": "++",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "VarDecl",
// CHECK-NEXT:           "name": "__begin1",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "int *"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclStmt",
// CHECK:             "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "VarDecl",
// CHECK:               "name": "v",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          },
// CHECK-NEXT:          "init": "c",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "LValueToRValue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "UnaryOperator",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "int"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "lvalue",
// CHECK-NEXT:              "isPostfix": false,
// CHECK-NEXT:              "opcode": "*",
// CHECK-NEXT:              "canOverflow": false,
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "ImplicitCastExpr",
// CHECK:                    "type": {
// CHECK-NEXT:                 "qualType": "int *"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "prvalue",
// CHECK-NEXT:                "castKind": "LValueToRValue",
// CHECK-NEXT:                "inner": [
// CHECK-NEXT:                 {
// CHECK-NEXT:                  "id": "0x{{.*}}",
// CHECK-NEXT:                  "kind": "DeclRefExpr",
// CHECK:                       "type": {
// CHECK-NEXT:                   "qualType": "int *"
// CHECK-NEXT:                  },
// CHECK-NEXT:                  "valueCategory": "lvalue",
// CHECK-NEXT:                  "referencedDecl": {
// CHECK-NEXT:                   "id": "0x{{.*}}",
// CHECK-NEXT:                   "kind": "VarDecl",
// CHECK-NEXT:                   "name": "__begin1",
// CHECK-NEXT:                   "type": {
// CHECK-NEXT:                    "qualType": "int *"
// CHECK-NEXT:                   }
// CHECK-NEXT:                  }
// CHECK-NEXT:                 }
// CHECK-NEXT:                ]
// CHECK-NEXT:               }
// CHECK-NEXT:              ]
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "NullStmt",
// CHECK:          ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionTemplateDecl",
// CHECK:       "name": "TestDependentGenericSelectionExpr",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "TemplateTypeParmDecl",
// CHECK:         "isReferenced": true,
// CHECK-NEXT:    "name": "Ty",
// CHECK-NEXT:    "tagUsed": "typename",
// CHECK-NEXT:    "depth": 0,
// CHECK-NEXT:    "index": 0
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FunctionDecl",
// CHECK:         "name": "TestDependentGenericSelectionExpr",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void (Ty)"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ParmVarDecl",
// CHECK:           "isReferenced": true,
// CHECK-NEXT:      "name": "T",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "Ty"
// CHECK-NEXT:      }
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CompoundStmt",
// CHECK:           "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "GenericSelectionExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "<dependent type>"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "resultDependent": true,
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "Ty"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "ParmVarDecl",
// CHECK-NEXT:           "name": "T",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "Ty"
// CHECK-NEXT:           }
// CHECK-NEXT:          },
// CHECK-NEXT:          "nonOdrUseReason": "unevaluated"
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "TemplateTypeParmType",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "Ty"
// CHECK-NEXT:          },
// CHECK-NEXT:          "isDependent": true,
// CHECK-NEXT:          "isInstantiationDependent": true,
// CHECK-NEXT:          "depth": 0,
// CHECK-NEXT:          "index": 0,
// CHECK-NEXT:          "decl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "TemplateTypeParmDecl",
// CHECK-NEXT:           "name": "Ty"
// CHECK-NEXT:          }
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "associationKind": "case",
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
// CHECK-NEXT:            "value": "1"
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "associationKind": "default",
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
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "GenericSelectionExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "<dependent type>"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "resultDependent": true,
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "Ty"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "ParmVarDecl",
// CHECK-NEXT:           "name": "T",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "Ty"
// CHECK-NEXT:           }
// CHECK-NEXT:          },
// CHECK-NEXT:          "nonOdrUseReason": "unevaluated"
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "TemplateTypeParmType",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "Ty"
// CHECK-NEXT:          },
// CHECK-NEXT:          "isDependent": true,
// CHECK-NEXT:          "isInstantiationDependent": true,
// CHECK-NEXT:          "depth": 0,
// CHECK-NEXT:          "index": 0,
// CHECK-NEXT:          "decl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "TemplateTypeParmDecl",
// CHECK-NEXT:           "name": "Ty"
// CHECK-NEXT:          }
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "associationKind": "default",
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
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }
