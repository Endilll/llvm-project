// RUN: %clang_cc1 -triple x86_64-unknown-unknown -Wno-unused-value -fcxx-exceptions -std=gnu++17 -ast-dump=json -ast-dump-filter Test %s | FileCheck %s

namespace std {
using size_t = decltype(sizeof(0));

class type_info {
public:
  virtual ~type_info();
  bool operator==(const type_info& rhs) const noexcept;
  bool operator!=(const type_info& rhs) const noexcept;
  type_info(const type_info& rhs) = delete; // cannot be copied
  type_info& operator=(const type_info& rhs) = delete; // cannot be copied
};

class bad_typeid {
public:
  bad_typeid() noexcept;
  bad_typeid(const bad_typeid&) noexcept;
  virtual ~bad_typeid();
  bad_typeid& operator=(const bad_typeid&) noexcept;
  const char* what() const noexcept;
};
} // namespace std
void *operator new(std::size_t, void *ptr);

struct S {
  virtual ~S() = default;

  void func(int);
  template <typename Ty>
  Ty foo();

  int i;
};

struct T : S {};

template <typename>
struct U {};

void TestThrow() {
  throw 12;
  throw;
}

void TestPointerToMember(S obj1, S *obj2, int S::* data, void (S::*call)(int)) {
  obj1.*data;
  obj2->*data;
  (obj1.*call)(12);
  (obj2->*call)(12);
}

void TestCasting(const S *s) {
  const_cast<S *>(s);
  static_cast<const T *>(s);
  dynamic_cast<const T *>(s);
  reinterpret_cast<const int *>(s);
}

template <typename... Ts>
void TestUnaryExpressions(int *p) {
  sizeof...(Ts);
  noexcept(p - p);

  ::new int;
  new (int);
  new int{12};
  new int[2];
  new int[2]{1, 2};
  new (p) int;
  new (p) int{12};

  ::delete p;
  delete [] p;
}

void TestPostfixExpressions(S a, S *p, U<int> *r) {
  a.func(0);
  p->func(0);
  p->template foo<int>();
  a.template foo<float>();
  p->~S();
  a.~S();
  a.~decltype(a)();
  p->::S::~S();
  r->template U<int>::~U();
  typeid(a);
  typeid(S);
  typeid(const volatile S);
}

template <typename... Ts>
void TestPrimaryExpressions(Ts... a) {
  struct V {
    void f() {
      this;

      [this]{};
      [*this]{};
    }
  };

  int b, c;

  [](){};
  [](int a, ...){};
  [a...]{};
  [=]{};
  [=] { return b; };
  [&]{};
  [&] { return c; };
  [b, &c]{ return b + c; };
  [a..., x = 12]{};
  []() constexpr {};
  []() mutable {};
  []() noexcept {};
  []() -> int { return 0; };

  (a + ...);
  (... + a);
  (a + ... + b);
}

namespace NS {
struct X {};
void f(X);
void y(...);
} // namespace NS

void TestADLCall() {
  NS::X x;
  f(x);
  y(x);
}

void TestNonADLCall() {
  NS::X x;
  NS::f(x);
}

void TestNonADLCall2() {
  NS::X x;
  using NS::f;
  f(x);
  y(x);
}

namespace test_adl_call_three {
using namespace NS;
void TestNonADLCall3() {
  X x;
  f(x);
}
} // namespace test_adl_call_three


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "TestThrow",
// CHECK-NEXT:  "mangledName": "_Z9TestThrowv",
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
// CHECK-NEXT:      "kind": "CXXThrowExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "void"
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
// CHECK-NEXT:        "value": "12"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXThrowExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "void"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue"
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "TestPointerToMember",
// CHECK-NEXT:  "mangledName": "_Z19TestPointerToMember1SPS_MS_iMS_FviE",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (S, S *, int S::*, void (S::*)(int))"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isUsed": true,
// CHECK-NEXT:    "name": "obj1",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "S"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isUsed": true,
// CHECK-NEXT:    "name": "obj2",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "S *"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isUsed": true,
// CHECK-NEXT:    "name": "data",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int S::*"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isUsed": true,
// CHECK-NEXT:    "name": "call",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void (S::*)(int)"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CompoundStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "BinaryOperator",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "lvalue",
// CHECK-NEXT:      "opcode": ".*",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "S"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "obj1",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "S"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int S::*"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "LValueToRValue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int S::*"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "ParmVarDecl",
// CHECK-NEXT:           "name": "data",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "int S::*"
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
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "lvalue",
// CHECK-NEXT:      "opcode": "->*",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "S *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "LValueToRValue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:              "type": {
// CHECK-NEXT:           "qualType": "S *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "ParmVarDecl",
// CHECK-NEXT:           "name": "obj2",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "S *"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int S::*"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "LValueToRValue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int S::*"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "ParmVarDecl",
// CHECK-NEXT:           "name": "data",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "int S::*"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXMemberCallExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "void"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ParenExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "<bound member function type>"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "BinaryOperator",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "<bound member function type>"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "opcode": ".*",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "S"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "ParmVarDecl",
// CHECK-NEXT:             "name": "obj1",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "S"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "void (S::*)(int)"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "LValueToRValue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "DeclRefExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "void (S::*)(int)"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "lvalue",
// CHECK-NEXT:              "referencedDecl": {
// CHECK-NEXT:               "id": "0x{{.*}}",
// CHECK-NEXT:               "kind": "ParmVarDecl",
// CHECK-NEXT:               "name": "call",
// CHECK-NEXT:               "type": {
// CHECK-NEXT:                "qualType": "void (S::*)(int)"
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
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXMemberCallExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "void"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ParenExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "<bound member function type>"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "BinaryOperator",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "<bound member function type>"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "opcode": "->*",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "S *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "LValueToRValue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "DeclRefExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "S *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "lvalue",
// CHECK-NEXT:              "referencedDecl": {
// CHECK-NEXT:               "id": "0x{{.*}}",
// CHECK-NEXT:               "kind": "ParmVarDecl",
// CHECK-NEXT:               "name": "obj2",
// CHECK-NEXT:               "type": {
// CHECK-NEXT:                "qualType": "S *"
// CHECK-NEXT:               }
// CHECK-NEXT:              }
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "void (S::*)(int)"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "LValueToRValue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "DeclRefExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "void (S::*)(int)"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "lvalue",
// CHECK-NEXT:              "referencedDecl": {
// CHECK-NEXT:               "id": "0x{{.*}}",
// CHECK-NEXT:               "kind": "ParmVarDecl",
// CHECK-NEXT:               "name": "call",
// CHECK-NEXT:               "type": {
// CHECK-NEXT:                "qualType": "void (S::*)(int)"
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
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "TestCasting",
// CHECK-NEXT:  "mangledName": "_Z11TestCastingPK1S",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (const S *)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isUsed": true,
// CHECK-NEXT:    "name": "s",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "const S *"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CompoundStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXConstCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "S *"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "NoOp",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "const S *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "LValueToRValue",
// CHECK-NEXT:        "isPartOfExplicitCast": true,
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "const S *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "ParmVarDecl",
// CHECK-NEXT:           "name": "s",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "const S *"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXStaticCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "const T *"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "BaseToDerived",
// CHECK-NEXT:      "path": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "name": "S"
// CHECK-NEXT:       }
// CHECK-NEXT:      ],
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "const S *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "LValueToRValue",
// CHECK-NEXT:        "isPartOfExplicitCast": true,
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "const S *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "ParmVarDecl",
// CHECK-NEXT:           "name": "s",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "const S *"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXDynamicCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "const T *"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "Dynamic",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "const S *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "LValueToRValue",
// CHECK-NEXT:        "isPartOfExplicitCast": true,
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "const S *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "ParmVarDecl",
// CHECK-NEXT:           "name": "s",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "const S *"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXReinterpretCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "const int *"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "BitCast",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "const S *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "LValueToRValue",
// CHECK-NEXT:        "isPartOfExplicitCast": true,
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "const S *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "ParmVarDecl",
// CHECK-NEXT:           "name": "s",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "const S *"
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
// CHECK:       "name": "TestUnaryExpressions",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "TemplateTypeParmDecl",
// CHECK:         "isReferenced": true,
// CHECK-NEXT:    "name": "Ts",
// CHECK-NEXT:    "tagUsed": "typename",
// CHECK-NEXT:    "depth": 0,
// CHECK-NEXT:    "index": 0,
// CHECK-NEXT:    "isParameterPack": true
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FunctionDecl",
// CHECK:         "name": "TestUnaryExpressions",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void (int *)"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ParmVarDecl",
// CHECK:           "isReferenced": true,
// CHECK-NEXT:      "name": "p",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int *"
// CHECK-NEXT:      }
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CompoundStmt",
// CHECK:           "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "SizeOfPackExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "desugaredQualType": "unsigned long",
// CHECK-NEXT:         "qualType": "__size_t"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "name": "Ts"
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CXXNoexceptExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "bool"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "BinaryOperator",
// CHECK:               "type": {
// CHECK-NEXT:           "desugaredQualType": "long",
// CHECK-NEXT:           "qualType": "__ptrdiff_t"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "opcode": "-",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "LValueToRValue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "DeclRefExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "int *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "lvalue",
// CHECK-NEXT:              "referencedDecl": {
// CHECK-NEXT:               "id": "0x{{.*}}",
// CHECK-NEXT:               "kind": "ParmVarDecl",
// CHECK-NEXT:               "name": "p",
// CHECK-NEXT:               "type": {
// CHECK-NEXT:                "qualType": "int *"
// CHECK-NEXT:               }
// CHECK-NEXT:              },
// CHECK-NEXT:              "nonOdrUseReason": "unevaluated"
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "LValueToRValue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "DeclRefExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "int *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "lvalue",
// CHECK-NEXT:              "referencedDecl": {
// CHECK-NEXT:               "id": "0x{{.*}}",
// CHECK-NEXT:               "kind": "ParmVarDecl",
// CHECK-NEXT:               "name": "p",
// CHECK-NEXT:               "type": {
// CHECK-NEXT:                "qualType": "int *"
// CHECK-NEXT:               }
// CHECK-NEXT:              },
// CHECK-NEXT:              "nonOdrUseReason": "unevaluated"
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
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
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CXXNewExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "initStyle": "list",
// CHECK-NEXT:        "operatorNewDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "FunctionDecl",
// CHECK-NEXT:         "name": "operator new",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "void *(__size_t)"
// CHECK-NEXT:         }
// CHECK-NEXT:        },
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "InitListExpr",
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
// CHECK-NEXT:            "value": "12"
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
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
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CXXNewExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "isArray": true,
// CHECK-NEXT:        "initStyle": "list",
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
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "InitListExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int[2]"
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
// CHECK-NEXT:           },
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
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CXXNewExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "isPlacement": true,
// CHECK-NEXT:        "operatorNewDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "FunctionDecl",
// CHECK-NEXT:         "name": "operator new",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "void *(std::size_t, void *)"
// CHECK-NEXT:         }
// CHECK-NEXT:        },
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "void *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "BitCast",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "LValueToRValue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "DeclRefExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "int *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "lvalue",
// CHECK-NEXT:              "referencedDecl": {
// CHECK-NEXT:               "id": "0x{{.*}}",
// CHECK-NEXT:               "kind": "ParmVarDecl",
// CHECK-NEXT:               "name": "p",
// CHECK-NEXT:               "type": {
// CHECK-NEXT:                "qualType": "int *"
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
// CHECK-NEXT:        "kind": "CXXNewExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "isPlacement": true,
// CHECK-NEXT:        "initStyle": "list",
// CHECK-NEXT:        "operatorNewDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "FunctionDecl",
// CHECK-NEXT:         "name": "operator new",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "void *(std::size_t, void *)"
// CHECK-NEXT:         }
// CHECK-NEXT:        },
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "InitListExpr",
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
// CHECK-NEXT:            "value": "12"
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "void *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "BitCast",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "int *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "LValueToRValue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "DeclRefExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "int *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "lvalue",
// CHECK-NEXT:              "referencedDecl": {
// CHECK-NEXT:               "id": "0x{{.*}}",
// CHECK-NEXT:               "kind": "ParmVarDecl",
// CHECK-NEXT:               "name": "p",
// CHECK-NEXT:               "type": {
// CHECK-NEXT:                "qualType": "int *"
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
// CHECK-NEXT:        "kind": "CXXDeleteExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "void"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "isGlobal": true,
// CHECK-NEXT:        "operatorDeleteDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "FunctionDecl",
// CHECK-NEXT:         "name": "operator delete",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "void (void *, __size_t) noexcept"
// CHECK-NEXT:         }
// CHECK-NEXT:        },
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
// CHECK-NEXT:             "kind": "ParmVarDecl",
// CHECK-NEXT:             "name": "p",
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
// CHECK-NEXT:        "kind": "CXXDeleteExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "void"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "isArray": true,
// CHECK-NEXT:        "isArrayAsWritten": true,
// CHECK-NEXT:        "operatorDeleteDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "FunctionDecl",
// CHECK-NEXT:         "name": "operator delete[]",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "void (void *) noexcept"
// CHECK-NEXT:         }
// CHECK-NEXT:        },
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
// CHECK-NEXT:             "kind": "ParmVarDecl",
// CHECK-NEXT:             "name": "p",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "int *"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
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
// CHECK:       "name": "TestPostfixExpressions",
// CHECK-NEXT:  "mangledName": "_Z22TestPostfixExpressions1SPS_P1UIiE",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (S, S *, U<int> *)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isUsed": true,
// CHECK-NEXT:    "name": "a",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "S"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isUsed": true,
// CHECK-NEXT:    "name": "p",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "S *"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isUsed": true,
// CHECK-NEXT:    "name": "r",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "U<int> *"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CompoundStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXMemberCallExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "void"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "MemberExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "<bound member function type>"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "name": "func",
// CHECK-NEXT:        "isArrow": false,
// CHECK-NEXT:        "referencedMemberDecl": "0x{{.*}}",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "S"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "ParmVarDecl",
// CHECK-NEXT:           "name": "a",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "S"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "IntegerLiteral",
// CHECK:            "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "value": "0"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXMemberCallExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "void"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "MemberExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "<bound member function type>"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "name": "func",
// CHECK-NEXT:        "isArrow": true,
// CHECK-NEXT:        "referencedMemberDecl": "0x{{.*}}",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "S *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "LValueToRValue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "S *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "ParmVarDecl",
// CHECK-NEXT:             "name": "p",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "S *"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
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
// CHECK-NEXT:        "value": "0"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXMemberCallExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "MemberExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "<bound member function type>"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "name": "foo",
// CHECK-NEXT:        "isArrow": true,
// CHECK-NEXT:        "referencedMemberDecl": "0x{{.*}}",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "S *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "LValueToRValue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "S *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "ParmVarDecl",
// CHECK-NEXT:             "name": "p",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "S *"
// CHECK-NEXT:             }
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
// CHECK-NEXT:      "kind": "CXXMemberCallExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "float"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "MemberExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "<bound member function type>"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "name": "foo",
// CHECK-NEXT:        "isArrow": false,
// CHECK-NEXT:        "referencedMemberDecl": "0x{{.*}}",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "S"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "ParmVarDecl",
// CHECK-NEXT:           "name": "a",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "S"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXMemberCallExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "void"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "MemberExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "<bound member function type>"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "name": "~S",
// CHECK-NEXT:        "isArrow": true,
// CHECK-NEXT:        "referencedMemberDecl": "0x{{.*}}",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "S *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "LValueToRValue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "S *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "ParmVarDecl",
// CHECK-NEXT:             "name": "p",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "S *"
// CHECK-NEXT:             }
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
// CHECK-NEXT:      "kind": "CXXMemberCallExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "void"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "MemberExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "<bound member function type>"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "name": "~S",
// CHECK-NEXT:        "isArrow": false,
// CHECK-NEXT:        "referencedMemberDecl": "0x{{.*}}",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "S"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "ParmVarDecl",
// CHECK-NEXT:           "name": "a",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "S"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXMemberCallExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "void"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "MemberExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "<bound member function type>"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "name": "~S",
// CHECK-NEXT:        "isArrow": false,
// CHECK-NEXT:        "referencedMemberDecl": "0x{{.*}}",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "S"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "ParmVarDecl",
// CHECK-NEXT:           "name": "a",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "S"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXMemberCallExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "void"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "MemberExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "<bound member function type>"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "name": "~S",
// CHECK-NEXT:        "isArrow": true,
// CHECK-NEXT:        "referencedMemberDecl": "0x{{.*}}",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "S *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "LValueToRValue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "S *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "ParmVarDecl",
// CHECK-NEXT:             "name": "p",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "S *"
// CHECK-NEXT:             }
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
// CHECK-NEXT:      "kind": "CXXMemberCallExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "void"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "MemberExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "<bound member function type>"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "name": "~U",
// CHECK-NEXT:        "isArrow": true,
// CHECK-NEXT:        "referencedMemberDecl": "0x{{.*}}",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "U<int> *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "LValueToRValue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "U<int> *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "ParmVarDecl",
// CHECK-NEXT:             "name": "r",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "U<int> *"
// CHECK-NEXT:             }
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
// CHECK-NEXT:      "kind": "CXXTypeidExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "const std::type_info"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "lvalue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "S"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "a",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "S"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXTypeidExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "const std::type_info"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "lvalue",
// CHECK-NEXT:      "typeArg": {
// CHECK-NEXT:       "qualType": "S"
// CHECK-NEXT:      }
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXTypeidExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "const std::type_info"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "lvalue",
// CHECK-NEXT:      "typeArg": {
// CHECK-NEXT:       "qualType": "const volatile S"
// CHECK-NEXT:      },
// CHECK-NEXT:      "adjustedTypeArg": {
// CHECK-NEXT:       "qualType": "S"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionTemplateDecl",
// CHECK:       "name": "TestPrimaryExpressions",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "TemplateTypeParmDecl",
// CHECK:         "isReferenced": true,
// CHECK-NEXT:    "name": "Ts",
// CHECK-NEXT:    "tagUsed": "typename",
// CHECK-NEXT:    "depth": 0,
// CHECK-NEXT:    "index": 0,
// CHECK-NEXT:    "isParameterPack": true
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FunctionDecl",
// CHECK:         "name": "TestPrimaryExpressions",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void (Ts...)"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ParmVarDecl",
// CHECK:           "isReferenced": true,
// CHECK-NEXT:      "name": "a",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "Ts..."
// CHECK-NEXT:      },
// CHECK-NEXT:      "isParameterPack": true
// CHECK-NEXT:     },
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
// CHECK-NEXT:          "kind": "CXXRecordDecl",
// CHECK:               "name": "V",
// CHECK-NEXT:          "tagUsed": "struct",
// CHECK-NEXT:          "completeDefinition": true,
// CHECK-NEXT:          "definitionData": {
// CHECK-NEXT:           "canConstDefaultInit": true,
// CHECK-NEXT:           "copyAssign": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "copyCtor": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "defaultCtor": {
// CHECK-NEXT:            "defaultedIsConstexpr": true,
// CHECK-NEXT:            "exists": true,
// CHECK-NEXT:            "isConstexpr": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "dtor": {
// CHECK-NEXT:            "irrelevant": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "hasConstexprNonCopyMoveConstructor": true,
// CHECK-NEXT:           "isAggregate": true,
// CHECK-NEXT:           "isEmpty": true,
// CHECK-NEXT:           "isLiteral": true,
// CHECK-NEXT:           "isPOD": true,
// CHECK-NEXT:           "isStandardLayout": true,
// CHECK-NEXT:           "isTrivial": true,
// CHECK-NEXT:           "isTriviallyCopyable": true,
// CHECK-NEXT:           "moveAssign": {
// CHECK-NEXT:            "exists": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "moveCtor": {
// CHECK-NEXT:            "exists": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           }
// CHECK-NEXT:          },
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXRecordDecl",
// CHECK:                 "isImplicit": true,
// CHECK-NEXT:            "name": "V",
// CHECK-NEXT:            "tagUsed": "struct"
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXMethodDecl",
// CHECK:                 "name": "f",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "void ()"
// CHECK-NEXT:            },
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "CompoundStmt",
// CHECK:                   "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "CXXThisExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "V *"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "prvalue"
// CHECK-NEXT:               },
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "LambdaExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "(lambda at {{.*}}:98:7)"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "prvalue",
// CHECK-NEXT:                "hasExplicitParameters": false,
// CHECK-NEXT:                "inner": [
// CHECK-NEXT:                 {
// CHECK-NEXT:                  "id": "0x{{.*}}",
// CHECK-NEXT:                  "kind": "CXXRecordDecl",
// CHECK:                       "isImplicit": true,
// CHECK-NEXT:                  "tagUsed": "class",
// CHECK-NEXT:                  "completeDefinition": true,
// CHECK-NEXT:                  "definitionData": {
// CHECK-NEXT:                   "canConstDefaultInit": true,
// CHECK-NEXT:                   "copyAssign": {
// CHECK-NEXT:                    "hasConstParam": true,
// CHECK-NEXT:                    "implicitHasConstParam": true,
// CHECK-NEXT:                    "needsImplicit": true,
// CHECK-NEXT:                    "trivial": true
// CHECK-NEXT:                   },
// CHECK-NEXT:                   "copyCtor": {
// CHECK-NEXT:                    "hasConstParam": true,
// CHECK-NEXT:                    "implicitHasConstParam": true,
// CHECK-NEXT:                    "needsImplicit": true,
// CHECK-NEXT:                    "simple": true,
// CHECK-NEXT:                    "trivial": true
// CHECK-NEXT:                   },
// CHECK-NEXT:                   "defaultCtor": {},
// CHECK-NEXT:                   "dtor": {
// CHECK-NEXT:                    "irrelevant": true,
// CHECK-NEXT:                    "needsImplicit": true,
// CHECK-NEXT:                    "simple": true,
// CHECK-NEXT:                    "trivial": true
// CHECK-NEXT:                   },
// CHECK-NEXT:                   "isLambda": true,
// CHECK-NEXT:                   "isStandardLayout": true,
// CHECK-NEXT:                   "isTriviallyCopyable": true,
// CHECK-NEXT:                   "moveAssign": {},
// CHECK-NEXT:                   "moveCtor": {
// CHECK-NEXT:                    "exists": true,
// CHECK-NEXT:                    "needsImplicit": true,
// CHECK-NEXT:                    "simple": true,
// CHECK-NEXT:                    "trivial": true
// CHECK-NEXT:                   }
// CHECK-NEXT:                  },
// CHECK-NEXT:                  "inner": [
// CHECK-NEXT:                   {
// CHECK-NEXT:                    "id": "0x{{.*}}",
// CHECK-NEXT:                    "kind": "CXXMethodDecl",
// CHECK:                         "name": "operator()",
// CHECK-NEXT:                    "type": {
// CHECK-NEXT:                     "qualType": "auto () const -> auto"
// CHECK-NEXT:                    },
// CHECK-NEXT:                    "inline": true,
// CHECK-NEXT:                    "inner": [
// CHECK-NEXT:                     {
// CHECK-NEXT:                      "id": "0x{{.*}}",
// CHECK-NEXT:                      "kind": "CompoundStmt",
// CHECK:                         ]
// CHECK-NEXT:                   },
// CHECK-NEXT:                   {
// CHECK-NEXT:                    "id": "0x{{.*}}",
// CHECK-NEXT:                    "kind": "FieldDecl",
// CHECK:                         "isImplicit": true,
// CHECK-NEXT:                    "type": {
// CHECK-NEXT:                     "qualType": "V *"
// CHECK-NEXT:                    }
// CHECK-NEXT:                   }
// CHECK-NEXT:                  ]
// CHECK-NEXT:                 },
// CHECK-NEXT:                 {
// CHECK-NEXT:                  "id": "0x{{.*}}",
// CHECK-NEXT:                  "kind": "ParenListExpr",
// CHECK:                       "type": {
// CHECK-NEXT:                   "qualType": "NULL TYPE"
// CHECK-NEXT:                  },
// CHECK-NEXT:                  "valueCategory": "prvalue",
// CHECK-NEXT:                  "inner": [
// CHECK-NEXT:                   {
// CHECK-NEXT:                    "id": "0x{{.*}}",
// CHECK-NEXT:                    "kind": "CXXThisExpr",
// CHECK:                         "type": {
// CHECK-NEXT:                     "qualType": "V *"
// CHECK-NEXT:                    },
// CHECK-NEXT:                    "valueCategory": "prvalue"
// CHECK-NEXT:                   }
// CHECK-NEXT:                  ]
// CHECK-NEXT:                 },
// CHECK-NEXT:                 {
// CHECK-NEXT:                  "id": "0x{{.*}}",
// CHECK-NEXT:                  "kind": "CompoundStmt",
// CHECK:                     ]
// CHECK-NEXT:               },
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "LambdaExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "(lambda at {{.*}}:99:7)"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "prvalue",
// CHECK-NEXT:                "hasExplicitParameters": false,
// CHECK-NEXT:                "inner": [
// CHECK-NEXT:                 {
// CHECK-NEXT:                  "id": "0x{{.*}}",
// CHECK-NEXT:                  "kind": "CXXRecordDecl",
// CHECK:                       "isImplicit": true,
// CHECK-NEXT:                  "tagUsed": "class",
// CHECK-NEXT:                  "completeDefinition": true,
// CHECK-NEXT:                  "definitionData": {
// CHECK-NEXT:                   "canConstDefaultInit": true,
// CHECK-NEXT:                   "copyAssign": {
// CHECK-NEXT:                    "hasConstParam": true,
// CHECK-NEXT:                    "implicitHasConstParam": true,
// CHECK-NEXT:                    "needsImplicit": true,
// CHECK-NEXT:                    "trivial": true
// CHECK-NEXT:                   },
// CHECK-NEXT:                   "copyCtor": {
// CHECK-NEXT:                    "hasConstParam": true,
// CHECK-NEXT:                    "implicitHasConstParam": true,
// CHECK-NEXT:                    "needsImplicit": true,
// CHECK-NEXT:                    "simple": true,
// CHECK-NEXT:                    "trivial": true
// CHECK-NEXT:                   },
// CHECK-NEXT:                   "defaultCtor": {
// CHECK-NEXT:                    "defaultedIsConstexpr": true
// CHECK-NEXT:                   },
// CHECK-NEXT:                   "dtor": {
// CHECK-NEXT:                    "irrelevant": true,
// CHECK-NEXT:                    "needsImplicit": true,
// CHECK-NEXT:                    "simple": true,
// CHECK-NEXT:                    "trivial": true
// CHECK-NEXT:                   },
// CHECK-NEXT:                   "isLambda": true,
// CHECK-NEXT:                   "isStandardLayout": true,
// CHECK-NEXT:                   "isTriviallyCopyable": true,
// CHECK-NEXT:                   "moveAssign": {},
// CHECK-NEXT:                   "moveCtor": {
// CHECK-NEXT:                    "exists": true,
// CHECK-NEXT:                    "needsImplicit": true,
// CHECK-NEXT:                    "simple": true,
// CHECK-NEXT:                    "trivial": true
// CHECK-NEXT:                   }
// CHECK-NEXT:                  },
// CHECK-NEXT:                  "inner": [
// CHECK-NEXT:                   {
// CHECK-NEXT:                    "id": "0x{{.*}}",
// CHECK-NEXT:                    "kind": "CXXMethodDecl",
// CHECK:                         "name": "operator()",
// CHECK-NEXT:                    "type": {
// CHECK-NEXT:                     "qualType": "auto () const -> auto"
// CHECK-NEXT:                    },
// CHECK-NEXT:                    "inline": true,
// CHECK-NEXT:                    "inner": [
// CHECK-NEXT:                     {
// CHECK-NEXT:                      "id": "0x{{.*}}",
// CHECK-NEXT:                      "kind": "CompoundStmt",
// CHECK:                         ]
// CHECK-NEXT:                   },
// CHECK-NEXT:                   {
// CHECK-NEXT:                    "id": "0x{{.*}}",
// CHECK-NEXT:                    "kind": "FieldDecl",
// CHECK:                         "isImplicit": true,
// CHECK-NEXT:                    "type": {
// CHECK-NEXT:                     "qualType": "V"
// CHECK-NEXT:                    }
// CHECK-NEXT:                   }
// CHECK-NEXT:                  ]
// CHECK-NEXT:                 },
// CHECK-NEXT:                 {
// CHECK-NEXT:                  "id": "0x{{.*}}",
// CHECK-NEXT:                  "kind": "ParenListExpr",
// CHECK:                       "type": {
// CHECK-NEXT:                   "qualType": "NULL TYPE"
// CHECK-NEXT:                  },
// CHECK-NEXT:                  "valueCategory": "prvalue",
// CHECK-NEXT:                  "inner": [
// CHECK-NEXT:                   {
// CHECK-NEXT:                    "id": "0x{{.*}}",
// CHECK-NEXT:                    "kind": "UnaryOperator",
// CHECK:                         "type": {
// CHECK-NEXT:                     "qualType": "V"
// CHECK-NEXT:                    },
// CHECK-NEXT:                    "valueCategory": "lvalue",
// CHECK-NEXT:                    "isPostfix": false,
// CHECK-NEXT:                    "opcode": "*",
// CHECK-NEXT:                    "canOverflow": false,
// CHECK-NEXT:                    "inner": [
// CHECK-NEXT:                     {
// CHECK-NEXT:                      "id": "0x{{.*}}",
// CHECK-NEXT:                      "kind": "CXXThisExpr",
// CHECK:                           "type": {
// CHECK-NEXT:                       "qualType": "V *"
// CHECK-NEXT:                      },
// CHECK-NEXT:                      "valueCategory": "prvalue"
// CHECK-NEXT:                     }
// CHECK-NEXT:                    ]
// CHECK-NEXT:                   }
// CHECK-NEXT:                  ]
// CHECK-NEXT:                 },
// CHECK-NEXT:                 {
// CHECK-NEXT:                  "id": "0x{{.*}}",
// CHECK-NEXT:                  "kind": "CompoundStmt",
// CHECK:                     ]
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
// CHECK:               "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "VarDecl",
// CHECK:               "isReferenced": true,
// CHECK-NEXT:          "name": "c",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "LambdaExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "(lambda at {{.*}}:105:3)"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "hasExplicitParameters": true,
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXRecordDecl",
// CHECK:               "isImplicit": true,
// CHECK-NEXT:          "tagUsed": "class",
// CHECK-NEXT:          "completeDefinition": true,
// CHECK-NEXT:          "definitionData": {
// CHECK-NEXT:           "canConstDefaultInit": true,
// CHECK-NEXT:           "copyAssign": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "copyCtor": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "defaultCtor": {
// CHECK-NEXT:            "defaultedIsConstexpr": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "dtor": {
// CHECK-NEXT:            "irrelevant": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "isEmpty": true,
// CHECK-NEXT:           "isLambda": true,
// CHECK-NEXT:           "isLiteral": true,
// CHECK-NEXT:           "isStandardLayout": true,
// CHECK-NEXT:           "isTriviallyCopyable": true,
// CHECK-NEXT:           "moveAssign": {},
// CHECK-NEXT:           "moveCtor": {
// CHECK-NEXT:            "exists": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           }
// CHECK-NEXT:          },
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXMethodDecl",
// CHECK:                 "name": "operator()",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "auto () const"
// CHECK-NEXT:            },
// CHECK-NEXT:            "inline": true,
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "CompoundStmt",
// CHECK:                 ]
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXConversionDecl",
// CHECK:                 "isImplicit": true,
// CHECK-NEXT:            "name": "operator auto (*)()",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "auto (*() const noexcept)()"
// CHECK-NEXT:            },
// CHECK-NEXT:            "inline": true,
// CHECK-NEXT:            "constexpr": true
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXMethodDecl",
// CHECK:                 "isImplicit": true,
// CHECK-NEXT:            "name": "__invoke",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "auto ()"
// CHECK-NEXT:            },
// CHECK-NEXT:            "storageClass": "static",
// CHECK-NEXT:            "inline": true
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CompoundStmt",
// CHECK:             ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "LambdaExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "(lambda at {{.*}}:106:3)"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "hasExplicitParameters": true,
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXRecordDecl",
// CHECK:               "isImplicit": true,
// CHECK-NEXT:          "tagUsed": "class",
// CHECK-NEXT:          "completeDefinition": true,
// CHECK-NEXT:          "definitionData": {
// CHECK-NEXT:           "canConstDefaultInit": true,
// CHECK-NEXT:           "copyAssign": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "copyCtor": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "defaultCtor": {
// CHECK-NEXT:            "defaultedIsConstexpr": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "dtor": {
// CHECK-NEXT:            "irrelevant": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "isEmpty": true,
// CHECK-NEXT:           "isLambda": true,
// CHECK-NEXT:           "isLiteral": true,
// CHECK-NEXT:           "isStandardLayout": true,
// CHECK-NEXT:           "isTriviallyCopyable": true,
// CHECK-NEXT:           "moveAssign": {},
// CHECK-NEXT:           "moveCtor": {
// CHECK-NEXT:            "exists": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           }
// CHECK-NEXT:          },
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXMethodDecl",
// CHECK:                 "name": "operator()",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "auto (int, ...) const"
// CHECK-NEXT:            },
// CHECK-NEXT:            "inline": true,
// CHECK-NEXT:            "variadic": true,
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "ParmVarDecl",
// CHECK:                   "name": "a",
// CHECK-NEXT:              "type": {
// CHECK-NEXT:               "qualType": "int"
// CHECK-NEXT:              }
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "CompoundStmt",
// CHECK:                 ]
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXConversionDecl",
// CHECK:                 "isImplicit": true,
// CHECK-NEXT:            "name": "operator auto (*)(int, ...)",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "auto (*() const noexcept)(int, ...)"
// CHECK-NEXT:            },
// CHECK-NEXT:            "inline": true,
// CHECK-NEXT:            "constexpr": true
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXMethodDecl",
// CHECK:                 "isImplicit": true,
// CHECK-NEXT:            "name": "__invoke",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "auto (int, ...)"
// CHECK-NEXT:            },
// CHECK-NEXT:            "storageClass": "static",
// CHECK-NEXT:            "inline": true,
// CHECK-NEXT:            "variadic": true,
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "ParmVarDecl",
// CHECK:                   "name": "a",
// CHECK-NEXT:              "type": {
// CHECK-NEXT:               "qualType": "int"
// CHECK-NEXT:              }
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CompoundStmt",
// CHECK:             ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "LambdaExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "(lambda at {{.*}}:107:3)"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "hasExplicitParameters": false,
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXRecordDecl",
// CHECK:               "isImplicit": true,
// CHECK-NEXT:          "tagUsed": "class",
// CHECK-NEXT:          "completeDefinition": true,
// CHECK-NEXT:          "definitionData": {
// CHECK-NEXT:           "canConstDefaultInit": true,
// CHECK-NEXT:           "copyAssign": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "copyCtor": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "defaultCtor": {},
// CHECK-NEXT:           "dtor": {
// CHECK-NEXT:            "irrelevant": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "isLambda": true,
// CHECK-NEXT:           "isStandardLayout": true,
// CHECK-NEXT:           "isTriviallyCopyable": true,
// CHECK-NEXT:           "moveAssign": {},
// CHECK-NEXT:           "moveCtor": {
// CHECK-NEXT:            "exists": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           }
// CHECK-NEXT:          },
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXMethodDecl",
// CHECK:                 "name": "operator()",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "auto () const -> auto"
// CHECK-NEXT:            },
// CHECK-NEXT:            "inline": true,
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "CompoundStmt",
// CHECK:                 ]
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "FieldDecl",
// CHECK:                 "isImplicit": true,
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "Ts..."
// CHECK-NEXT:            }
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ParenListExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "NULL TYPE"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "Ts"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "ParmVarDecl",
// CHECK-NEXT:             "name": "a",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "Ts..."
// CHECK-NEXT:             }
// CHECK-NEXT:            }
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CompoundStmt",
// CHECK:             ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "LambdaExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "(lambda at {{.*}}:108:3)"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "hasExplicitParameters": false,
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXRecordDecl",
// CHECK:               "isImplicit": true,
// CHECK-NEXT:          "tagUsed": "class",
// CHECK-NEXT:          "completeDefinition": true,
// CHECK-NEXT:          "definitionData": {
// CHECK-NEXT:           "canConstDefaultInit": true,
// CHECK-NEXT:           "copyAssign": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "copyCtor": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "defaultCtor": {
// CHECK-NEXT:            "defaultedIsConstexpr": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "dtor": {
// CHECK-NEXT:            "irrelevant": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "isEmpty": true,
// CHECK-NEXT:           "isLambda": true,
// CHECK-NEXT:           "isLiteral": true,
// CHECK-NEXT:           "isStandardLayout": true,
// CHECK-NEXT:           "isTriviallyCopyable": true,
// CHECK-NEXT:           "moveAssign": {},
// CHECK-NEXT:           "moveCtor": {
// CHECK-NEXT:            "exists": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           }
// CHECK-NEXT:          },
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXMethodDecl",
// CHECK:                 "name": "operator()",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "auto () const -> auto"
// CHECK-NEXT:            },
// CHECK-NEXT:            "inline": true,
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "CompoundStmt",
// CHECK:                 ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CompoundStmt",
// CHECK:             ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "LambdaExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "(lambda at {{.*}}:109:3)"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "hasExplicitParameters": false,
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXRecordDecl",
// CHECK:               "isImplicit": true,
// CHECK-NEXT:          "tagUsed": "class",
// CHECK-NEXT:          "completeDefinition": true,
// CHECK-NEXT:          "definitionData": {
// CHECK-NEXT:           "canConstDefaultInit": true,
// CHECK-NEXT:           "copyAssign": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "copyCtor": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "defaultCtor": {
// CHECK-NEXT:            "defaultedIsConstexpr": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "dtor": {
// CHECK-NEXT:            "irrelevant": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "isEmpty": true,
// CHECK-NEXT:           "isLambda": true,
// CHECK-NEXT:           "isLiteral": true,
// CHECK-NEXT:           "isStandardLayout": true,
// CHECK-NEXT:           "isTriviallyCopyable": true,
// CHECK-NEXT:           "moveAssign": {},
// CHECK-NEXT:           "moveCtor": {
// CHECK-NEXT:            "exists": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           }
// CHECK-NEXT:          },
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXMethodDecl",
// CHECK:                 "name": "operator()",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "auto () const -> auto"
// CHECK-NEXT:            },
// CHECK-NEXT:            "inline": true,
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "CompoundStmt",
// CHECK:                   "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "ReturnStmt",
// CHECK:                     "inner": [
// CHECK-NEXT:                 {
// CHECK-NEXT:                  "id": "0x{{.*}}",
// CHECK-NEXT:                  "kind": "DeclRefExpr",
// CHECK:                       "type": {
// CHECK-NEXT:                   "qualType": "const int"
// CHECK-NEXT:                  },
// CHECK-NEXT:                  "valueCategory": "lvalue",
// CHECK-NEXT:                  "referencedDecl": {
// CHECK-NEXT:                   "id": "0x{{.*}}",
// CHECK-NEXT:                   "kind": "VarDecl",
// CHECK-NEXT:                   "name": "b",
// CHECK-NEXT:                   "type": {
// CHECK-NEXT:                    "qualType": "int"
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
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CompoundStmt",
// CHECK:               "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ReturnStmt",
// CHECK:                 "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "DeclRefExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "const int"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "lvalue",
// CHECK-NEXT:              "referencedDecl": {
// CHECK-NEXT:               "id": "0x{{.*}}",
// CHECK-NEXT:               "kind": "VarDecl",
// CHECK-NEXT:               "name": "b",
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
// CHECK-NEXT:        "kind": "LambdaExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "(lambda at {{.*}}:110:3)"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "hasExplicitParameters": false,
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXRecordDecl",
// CHECK:               "isImplicit": true,
// CHECK-NEXT:          "tagUsed": "class",
// CHECK-NEXT:          "completeDefinition": true,
// CHECK-NEXT:          "definitionData": {
// CHECK-NEXT:           "canConstDefaultInit": true,
// CHECK-NEXT:           "copyAssign": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "copyCtor": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "defaultCtor": {
// CHECK-NEXT:            "defaultedIsConstexpr": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "dtor": {
// CHECK-NEXT:            "irrelevant": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "isEmpty": true,
// CHECK-NEXT:           "isLambda": true,
// CHECK-NEXT:           "isLiteral": true,
// CHECK-NEXT:           "isStandardLayout": true,
// CHECK-NEXT:           "isTriviallyCopyable": true,
// CHECK-NEXT:           "moveAssign": {},
// CHECK-NEXT:           "moveCtor": {
// CHECK-NEXT:            "exists": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           }
// CHECK-NEXT:          },
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXMethodDecl",
// CHECK:                 "name": "operator()",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "auto () const -> auto"
// CHECK-NEXT:            },
// CHECK-NEXT:            "inline": true,
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "CompoundStmt",
// CHECK:                 ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CompoundStmt",
// CHECK:             ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "LambdaExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "(lambda at {{.*}}:111:3)"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "hasExplicitParameters": false,
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXRecordDecl",
// CHECK:               "isImplicit": true,
// CHECK-NEXT:          "tagUsed": "class",
// CHECK-NEXT:          "completeDefinition": true,
// CHECK-NEXT:          "definitionData": {
// CHECK-NEXT:           "canConstDefaultInit": true,
// CHECK-NEXT:           "copyAssign": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "copyCtor": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "defaultCtor": {
// CHECK-NEXT:            "defaultedIsConstexpr": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "dtor": {
// CHECK-NEXT:            "irrelevant": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "isEmpty": true,
// CHECK-NEXT:           "isLambda": true,
// CHECK-NEXT:           "isLiteral": true,
// CHECK-NEXT:           "isStandardLayout": true,
// CHECK-NEXT:           "isTriviallyCopyable": true,
// CHECK-NEXT:           "moveAssign": {},
// CHECK-NEXT:           "moveCtor": {
// CHECK-NEXT:            "exists": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           }
// CHECK-NEXT:          },
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXMethodDecl",
// CHECK:                 "name": "operator()",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "auto () const -> auto"
// CHECK-NEXT:            },
// CHECK-NEXT:            "inline": true,
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "CompoundStmt",
// CHECK:                   "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "ReturnStmt",
// CHECK:                     "inner": [
// CHECK-NEXT:                 {
// CHECK-NEXT:                  "id": "0x{{.*}}",
// CHECK-NEXT:                  "kind": "DeclRefExpr",
// CHECK:                       "type": {
// CHECK-NEXT:                   "qualType": "int"
// CHECK-NEXT:                  },
// CHECK-NEXT:                  "valueCategory": "lvalue",
// CHECK-NEXT:                  "referencedDecl": {
// CHECK-NEXT:                   "id": "0x{{.*}}",
// CHECK-NEXT:                   "kind": "VarDecl",
// CHECK-NEXT:                   "name": "c",
// CHECK-NEXT:                   "type": {
// CHECK-NEXT:                    "qualType": "int"
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
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CompoundStmt",
// CHECK:               "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ReturnStmt",
// CHECK:                 "inner": [
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
// CHECK-NEXT:               "name": "c",
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
// CHECK-NEXT:        "kind": "LambdaExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "(lambda at {{.*}}:112:3)"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "hasExplicitParameters": false,
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXRecordDecl",
// CHECK:               "isImplicit": true,
// CHECK-NEXT:          "tagUsed": "class",
// CHECK-NEXT:          "completeDefinition": true,
// CHECK-NEXT:          "definitionData": {
// CHECK-NEXT:           "canConstDefaultInit": true,
// CHECK-NEXT:           "copyAssign": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "copyCtor": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "defaultCtor": {},
// CHECK-NEXT:           "dtor": {
// CHECK-NEXT:            "irrelevant": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "isLambda": true,
// CHECK-NEXT:           "isLiteral": true,
// CHECK-NEXT:           "isTriviallyCopyable": true,
// CHECK-NEXT:           "moveAssign": {},
// CHECK-NEXT:           "moveCtor": {
// CHECK-NEXT:            "exists": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           }
// CHECK-NEXT:          },
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXMethodDecl",
// CHECK:                 "name": "operator()",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "auto () const -> auto"
// CHECK-NEXT:            },
// CHECK-NEXT:            "inline": true,
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "CompoundStmt",
// CHECK:                   "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "ReturnStmt",
// CHECK:                     "inner": [
// CHECK-NEXT:                 {
// CHECK-NEXT:                  "id": "0x{{.*}}",
// CHECK-NEXT:                  "kind": "BinaryOperator",
// CHECK:                       "type": {
// CHECK-NEXT:                   "qualType": "int"
// CHECK-NEXT:                  },
// CHECK-NEXT:                  "valueCategory": "prvalue",
// CHECK-NEXT:                  "opcode": "+",
// CHECK-NEXT:                  "inner": [
// CHECK-NEXT:                   {
// CHECK-NEXT:                    "id": "0x{{.*}}",
// CHECK-NEXT:                    "kind": "ImplicitCastExpr",
// CHECK:                         "type": {
// CHECK-NEXT:                     "qualType": "int"
// CHECK-NEXT:                    },
// CHECK-NEXT:                    "valueCategory": "prvalue",
// CHECK-NEXT:                    "castKind": "LValueToRValue",
// CHECK-NEXT:                    "inner": [
// CHECK-NEXT:                     {
// CHECK-NEXT:                      "id": "0x{{.*}}",
// CHECK-NEXT:                      "kind": "DeclRefExpr",
// CHECK:                           "type": {
// CHECK-NEXT:                       "qualType": "const int"
// CHECK-NEXT:                      },
// CHECK-NEXT:                      "valueCategory": "lvalue",
// CHECK-NEXT:                      "referencedDecl": {
// CHECK-NEXT:                       "id": "0x{{.*}}",
// CHECK-NEXT:                       "kind": "VarDecl",
// CHECK-NEXT:                       "name": "b",
// CHECK-NEXT:                       "type": {
// CHECK-NEXT:                        "qualType": "int"
// CHECK-NEXT:                       }
// CHECK-NEXT:                      }
// CHECK-NEXT:                     }
// CHECK-NEXT:                    ]
// CHECK-NEXT:                   },
// CHECK-NEXT:                   {
// CHECK-NEXT:                    "id": "0x{{.*}}",
// CHECK-NEXT:                    "kind": "ImplicitCastExpr",
// CHECK:                         "type": {
// CHECK-NEXT:                     "qualType": "int"
// CHECK-NEXT:                    },
// CHECK-NEXT:                    "valueCategory": "prvalue",
// CHECK-NEXT:                    "castKind": "LValueToRValue",
// CHECK-NEXT:                    "inner": [
// CHECK-NEXT:                     {
// CHECK-NEXT:                      "id": "0x{{.*}}",
// CHECK-NEXT:                      "kind": "DeclRefExpr",
// CHECK:                           "type": {
// CHECK-NEXT:                       "qualType": "int"
// CHECK-NEXT:                      },
// CHECK-NEXT:                      "valueCategory": "lvalue",
// CHECK-NEXT:                      "referencedDecl": {
// CHECK-NEXT:                       "id": "0x{{.*}}",
// CHECK-NEXT:                       "kind": "VarDecl",
// CHECK-NEXT:                       "name": "c",
// CHECK-NEXT:                       "type": {
// CHECK-NEXT:                        "qualType": "int"
// CHECK-NEXT:                       }
// CHECK-NEXT:                      }
// CHECK-NEXT:                     }
// CHECK-NEXT:                    ]
// CHECK-NEXT:                   }
// CHECK-NEXT:                  ]
// CHECK-NEXT:                 }
// CHECK-NEXT:                ]
// CHECK-NEXT:               }
// CHECK-NEXT:              ]
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "FieldDecl",
// CHECK:                 "isImplicit": true,
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "int"
// CHECK-NEXT:            }
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "FieldDecl",
// CHECK:                 "isImplicit": true,
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "int &"
// CHECK-NEXT:            }
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
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
// CHECK-NEXT:             "name": "b",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "int"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
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
// CHECK-NEXT:           "name": "c",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "int"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CompoundStmt",
// CHECK:               "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ReturnStmt",
// CHECK:                 "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "BinaryOperator",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "int"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "opcode": "+",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "ImplicitCastExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "int"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "prvalue",
// CHECK-NEXT:                "castKind": "LValueToRValue",
// CHECK-NEXT:                "inner": [
// CHECK-NEXT:                 {
// CHECK-NEXT:                  "id": "0x{{.*}}",
// CHECK-NEXT:                  "kind": "DeclRefExpr",
// CHECK:                       "type": {
// CHECK-NEXT:                   "qualType": "const int"
// CHECK-NEXT:                  },
// CHECK-NEXT:                  "valueCategory": "lvalue",
// CHECK-NEXT:                  "referencedDecl": {
// CHECK-NEXT:                   "id": "0x{{.*}}",
// CHECK-NEXT:                   "kind": "VarDecl",
// CHECK-NEXT:                   "name": "b",
// CHECK-NEXT:                   "type": {
// CHECK-NEXT:                    "qualType": "int"
// CHECK-NEXT:                   }
// CHECK-NEXT:                  }
// CHECK-NEXT:                 }
// CHECK-NEXT:                ]
// CHECK-NEXT:               },
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "ImplicitCastExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "int"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "prvalue",
// CHECK-NEXT:                "castKind": "LValueToRValue",
// CHECK-NEXT:                "inner": [
// CHECK-NEXT:                 {
// CHECK-NEXT:                  "id": "0x{{.*}}",
// CHECK-NEXT:                  "kind": "DeclRefExpr",
// CHECK:                       "type": {
// CHECK-NEXT:                   "qualType": "int"
// CHECK-NEXT:                  },
// CHECK-NEXT:                  "valueCategory": "lvalue",
// CHECK-NEXT:                  "referencedDecl": {
// CHECK-NEXT:                   "id": "0x{{.*}}",
// CHECK-NEXT:                   "kind": "VarDecl",
// CHECK-NEXT:                   "name": "c",
// CHECK-NEXT:                   "type": {
// CHECK-NEXT:                    "qualType": "int"
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
// CHECK-NEXT:        "kind": "LambdaExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "(lambda at {{.*}}:113:3)"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "hasExplicitParameters": false,
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXRecordDecl",
// CHECK:               "isImplicit": true,
// CHECK-NEXT:          "tagUsed": "class",
// CHECK-NEXT:          "completeDefinition": true,
// CHECK-NEXT:          "definitionData": {
// CHECK-NEXT:           "canConstDefaultInit": true,
// CHECK-NEXT:           "copyAssign": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "copyCtor": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "defaultCtor": {},
// CHECK-NEXT:           "dtor": {
// CHECK-NEXT:            "irrelevant": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "isLambda": true,
// CHECK-NEXT:           "isStandardLayout": true,
// CHECK-NEXT:           "isTriviallyCopyable": true,
// CHECK-NEXT:           "moveAssign": {},
// CHECK-NEXT:           "moveCtor": {
// CHECK-NEXT:            "exists": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           }
// CHECK-NEXT:          },
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXMethodDecl",
// CHECK:                 "name": "operator()",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "auto () const -> auto"
// CHECK-NEXT:            },
// CHECK-NEXT:            "inline": true,
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "CompoundStmt",
// CHECK:                ]
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "FieldDecl",
// CHECK:                 "isImplicit": true,
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "Ts..."
// CHECK-NEXT:            }
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "FieldDecl",
// CHECK:                 "isImplicit": true,
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "int"
// CHECK-NEXT:            }
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ParenListExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "NULL TYPE"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "Ts"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "ParmVarDecl",
// CHECK-NEXT:             "name": "a",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "Ts..."
// CHECK-NEXT:             }
// CHECK-NEXT:            }
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "IntegerLiteral",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "value": "12"
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CompoundStmt",
// CHECK:             ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "LambdaExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "(lambda at {{.*}}:114:3)"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "hasExplicitParameters": true,
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXRecordDecl",
// CHECK:               "isImplicit": true,
// CHECK-NEXT:          "tagUsed": "class",
// CHECK-NEXT:          "completeDefinition": true,
// CHECK-NEXT:          "definitionData": {
// CHECK-NEXT:           "canConstDefaultInit": true,
// CHECK-NEXT:           "copyAssign": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "copyCtor": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "defaultCtor": {
// CHECK-NEXT:            "defaultedIsConstexpr": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "dtor": {
// CHECK-NEXT:            "irrelevant": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "isEmpty": true,
// CHECK-NEXT:           "isLambda": true,
// CHECK-NEXT:           "isLiteral": true,
// CHECK-NEXT:           "isStandardLayout": true,
// CHECK-NEXT:           "isTriviallyCopyable": true,
// CHECK-NEXT:           "moveAssign": {},
// CHECK-NEXT:           "moveCtor": {
// CHECK-NEXT:            "exists": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           }
// CHECK-NEXT:          },
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXMethodDecl",
// CHECK:                 "name": "operator()",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "auto () const"
// CHECK-NEXT:            },
// CHECK-NEXT:            "inline": true,
// CHECK-NEXT:            "constexpr": true,
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "CompoundStmt",
// CHECK:                 ]
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXConversionDecl",
// CHECK:                 "isImplicit": true,
// CHECK-NEXT:            "name": "operator auto (*)()",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "auto (*() const noexcept)()"
// CHECK-NEXT:            },
// CHECK-NEXT:            "inline": true,
// CHECK-NEXT:            "constexpr": true
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXMethodDecl",
// CHECK:                 "isImplicit": true,
// CHECK-NEXT:            "name": "__invoke",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "auto ()"
// CHECK-NEXT:            },
// CHECK-NEXT:            "storageClass": "static",
// CHECK-NEXT:            "inline": true,
// CHECK-NEXT:            "constexpr": true
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CompoundStmt",
// CHECK:             ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "LambdaExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "(lambda at {{.*}}:115:3)"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "hasExplicitParameters": true,
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXRecordDecl",
// CHECK:               "isImplicit": true,
// CHECK-NEXT:          "tagUsed": "class",
// CHECK-NEXT:          "completeDefinition": true,
// CHECK-NEXT:          "definitionData": {
// CHECK-NEXT:           "canConstDefaultInit": true,
// CHECK-NEXT:           "copyAssign": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "copyCtor": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "defaultCtor": {
// CHECK-NEXT:            "defaultedIsConstexpr": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "dtor": {
// CHECK-NEXT:            "irrelevant": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "isEmpty": true,
// CHECK-NEXT:           "isLambda": true,
// CHECK-NEXT:           "isLiteral": true,
// CHECK-NEXT:           "isStandardLayout": true,
// CHECK-NEXT:           "isTriviallyCopyable": true,
// CHECK-NEXT:           "moveAssign": {},
// CHECK-NEXT:           "moveCtor": {
// CHECK-NEXT:            "exists": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           }
// CHECK-NEXT:          },
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXMethodDecl",
// CHECK:                 "name": "operator()",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "auto ()"
// CHECK-NEXT:            },
// CHECK-NEXT:            "inline": true,
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "CompoundStmt",
// CHECK:                 ]
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXConversionDecl",
// CHECK-NEXT:            "loc": {
// CHECK-NEXT:             "offset": 2147,
// CHECK-NEXT:             "col": 3,
// CHECK-NEXT:             "tokLen": 1
// CHECK-NEXT:            },
// CHECK:                 "isImplicit": true,
// CHECK-NEXT:            "name": "operator auto (*)()",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "auto (*() const noexcept)()"
// CHECK-NEXT:            },
// CHECK-NEXT:            "inline": true,
// CHECK-NEXT:            "constexpr": true
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXMethodDecl",
// CHECK:                 "isImplicit": true,
// CHECK-NEXT:            "name": "__invoke",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "auto ()"
// CHECK-NEXT:            },
// CHECK-NEXT:            "storageClass": "static",
// CHECK-NEXT:            "inline": true
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CompoundStmt",
// CHECK:             ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "LambdaExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "(lambda at {{.*}}:116:3)"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "hasExplicitParameters": true,
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXRecordDecl",
// CHECK:               "isImplicit": true,
// CHECK-NEXT:          "tagUsed": "class",
// CHECK-NEXT:          "completeDefinition": true,
// CHECK-NEXT:          "definitionData": {
// CHECK-NEXT:           "canConstDefaultInit": true,
// CHECK-NEXT:           "copyAssign": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "copyCtor": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "defaultCtor": {
// CHECK-NEXT:            "defaultedIsConstexpr": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "dtor": {
// CHECK-NEXT:            "irrelevant": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "isEmpty": true,
// CHECK-NEXT:           "isLambda": true,
// CHECK-NEXT:           "isLiteral": true,
// CHECK-NEXT:           "isStandardLayout": true,
// CHECK-NEXT:           "isTriviallyCopyable": true,
// CHECK-NEXT:           "moveAssign": {},
// CHECK-NEXT:           "moveCtor": {
// CHECK-NEXT:            "exists": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           }
// CHECK-NEXT:          },
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXMethodDecl",
// CHECK:                 "name": "operator()",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "auto () const noexcept"
// CHECK-NEXT:            },
// CHECK-NEXT:            "inline": true,
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "CompoundStmt",
// CHECK:                 ]
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXConversionDecl",
// CHECK:                 "isImplicit": true,
// CHECK-NEXT:            "name": "operator auto (*)() noexcept",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "auto (*() const noexcept)() noexcept"
// CHECK-NEXT:            },
// CHECK-NEXT:            "inline": true,
// CHECK-NEXT:            "constexpr": true
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXMethodDecl",
// CHECK:                "isImplicit": true,
// CHECK-NEXT:            "name": "__invoke",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "auto () noexcept"
// CHECK-NEXT:            },
// CHECK-NEXT:            "storageClass": "static",
// CHECK-NEXT:            "inline": true
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CompoundStmt",
// CHECK:             ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "LambdaExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "(lambda at {{.*}}:117:3)"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "hasExplicitParameters": true,
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXRecordDecl",
// CHECK:               "isImplicit": true,
// CHECK-NEXT:          "tagUsed": "class",
// CHECK-NEXT:          "completeDefinition": true,
// CHECK-NEXT:          "definitionData": {
// CHECK-NEXT:           "canConstDefaultInit": true,
// CHECK-NEXT:           "copyAssign": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "copyCtor": {
// CHECK-NEXT:            "hasConstParam": true,
// CHECK-NEXT:            "implicitHasConstParam": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "defaultCtor": {
// CHECK-NEXT:            "defaultedIsConstexpr": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "dtor": {
// CHECK-NEXT:            "irrelevant": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           },
// CHECK-NEXT:           "isEmpty": true,
// CHECK-NEXT:           "isLambda": true,
// CHECK-NEXT:           "isLiteral": true,
// CHECK-NEXT:           "isStandardLayout": true,
// CHECK-NEXT:           "isTriviallyCopyable": true,
// CHECK-NEXT:           "moveAssign": {},
// CHECK-NEXT:           "moveCtor": {
// CHECK-NEXT:            "exists": true,
// CHECK-NEXT:            "needsImplicit": true,
// CHECK-NEXT:            "simple": true,
// CHECK-NEXT:            "trivial": true
// CHECK-NEXT:           }
// CHECK-NEXT:          },
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXMethodDecl",
// CHECK:                 "name": "operator()",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "auto () const -> int"
// CHECK-NEXT:            },
// CHECK-NEXT:            "inline": true,
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "CompoundStmt",
// CHECK:                   "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "ReturnStmt",
// CHECK:                     "inner": [
// CHECK-NEXT:                 {
// CHECK-NEXT:                  "id": "0x{{.*}}",
// CHECK-NEXT:                  "kind": "IntegerLiteral",
// CHECK:                       "type": {
// CHECK-NEXT:                   "qualType": "int"
// CHECK-NEXT:                  },
// CHECK-NEXT:                  "valueCategory": "prvalue",
// CHECK-NEXT:                  "value": "0"
// CHECK-NEXT:                 }
// CHECK-NEXT:                ]
// CHECK-NEXT:               }
// CHECK-NEXT:              ]
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXConversionDecl",
// CHECK:                 "isImplicit": true,
// CHECK-NEXT:            "name": "operator int (*)()",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "auto (*() const noexcept)() -> int"
// CHECK-NEXT:            },
// CHECK-NEXT:            "inline": true,
// CHECK-NEXT:            "constexpr": true
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CXXMethodDecl",
// CHECK:                 "isImplicit": true,
// CHECK-NEXT:            "name": "__invoke",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "auto () -> int"
// CHECK-NEXT:            },
// CHECK-NEXT:            "storageClass": "static",
// CHECK-NEXT:            "inline": true
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CompoundStmt",
// CHECK:               "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ReturnStmt",
// CHECK:                 "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "IntegerLiteral",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "int"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "value": "0"
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CXXFoldExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "<dependent type>"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {},
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "Ts"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "ParmVarDecl",
// CHECK-NEXT:           "name": "a",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "Ts..."
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         },
// CHECK-NEXT:         {}
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CXXFoldExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "<dependent type>"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {},
// CHECK-NEXT:         {},
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "Ts"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "ParmVarDecl",
// CHECK-NEXT:           "name": "a",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "Ts..."
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CXXFoldExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "<dependent type>"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {},
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "Ts"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "ParmVarDecl",
// CHECK-NEXT:           "name": "a",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "Ts..."
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         },
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
// CHECK-NEXT:           "name": "b",
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


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "TestADLCall",
// CHECK-NEXT:  "mangledName": "_Z11TestADLCallv",
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
// CHECK-NEXT:        "name": "x",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "NS::X"
// CHECK-NEXT:        },
// CHECK-NEXT:        "init": "call",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXConstructExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "NS::X"
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
// CHECK-NEXT:      "kind": "CallExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "void"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "adl": true,
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "void (*)(X)"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "FunctionToPointerDecay",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "void (X)"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "FunctionDecl",
// CHECK-NEXT:           "name": "f",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "void (X)"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CXXConstructExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "X"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "ctorType": {
// CHECK-NEXT:         "qualType": "void (const X &) noexcept"
// CHECK-NEXT:        },
// CHECK-NEXT:        "hadMultipleCandidates": true,
// CHECK-NEXT:        "constructionKind": "complete",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "const X"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "castKind": "NoOp",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "NS::X"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "VarDecl",
// CHECK-NEXT:             "name": "x",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "NS::X"
// CHECK-NEXT:             }
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
// CHECK-NEXT:      "kind": "CallExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "void"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "adl": true,
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "void (*)(...)"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "FunctionToPointerDecay",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "void (...)"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "FunctionDecl",
// CHECK-NEXT:           "name": "y",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "void (...)"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CXXConstructExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "NS::X"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "ctorType": {
// CHECK-NEXT:         "qualType": "void (const X &) noexcept"
// CHECK-NEXT:        },
// CHECK-NEXT:        "hadMultipleCandidates": true,
// CHECK-NEXT:        "constructionKind": "complete",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "const X"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "castKind": "NoOp",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "NS::X"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "VarDecl",
// CHECK-NEXT:             "name": "x",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "NS::X"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
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
// CHECK:       "name": "TestNonADLCall",
// CHECK-NEXT:  "mangledName": "_Z14TestNonADLCallv",
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
// CHECK-NEXT:        "name": "x",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "NS::X"
// CHECK-NEXT:        },
// CHECK-NEXT:        "init": "call",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXConstructExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "NS::X"
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
// CHECK-NEXT:         "qualType": "void (*)(X)"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "FunctionToPointerDecay",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "void (X)"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "FunctionDecl",
// CHECK-NEXT:           "name": "f",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "void (X)"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CXXConstructExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "X"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "ctorType": {
// CHECK-NEXT:         "qualType": "void (const X &) noexcept"
// CHECK-NEXT:        },
// CHECK-NEXT:        "hadMultipleCandidates": true,
// CHECK-NEXT:        "constructionKind": "complete",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "const X"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "castKind": "NoOp",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "NS::X"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "VarDecl",
// CHECK-NEXT:             "name": "x",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "NS::X"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
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
// CHECK:       "name": "TestNonADLCall2",
// CHECK-NEXT:  "mangledName": "_Z15TestNonADLCall2v",
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
// CHECK-NEXT:        "name": "x",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "NS::X"
// CHECK-NEXT:        },
// CHECK-NEXT:        "init": "call",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXConstructExpr",
// CHECK:              "type": {
// CHECK-NEXT:           "qualType": "NS::X"
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
// CHECK-NEXT:      "kind": "DeclStmt",
// CHECK:          "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "UsingDecl",
// CHECK:             "name": "NS::f"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
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
// CHECK-NEXT:         "qualType": "void (*)(X)"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "FunctionToPointerDecay",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "void (X)"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "FunctionDecl",
// CHECK-NEXT:           "name": "f",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "void (X)"
// CHECK-NEXT:           }
// CHECK-NEXT:          },
// CHECK-NEXT:          "foundReferencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "UsingShadowDecl",
// CHECK-NEXT:           "name": "f"
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CXXConstructExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "X"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "ctorType": {
// CHECK-NEXT:         "qualType": "void (const X &) noexcept"
// CHECK-NEXT:        },
// CHECK-NEXT:        "hadMultipleCandidates": true,
// CHECK-NEXT:        "constructionKind": "complete",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "const X"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "castKind": "NoOp",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "NS::X"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "VarDecl",
// CHECK-NEXT:             "name": "x",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "NS::X"
// CHECK-NEXT:             }
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
// CHECK-NEXT:      "kind": "CallExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "void"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "adl": true,
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "void (*)(...)"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "FunctionToPointerDecay",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "void (...)"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "FunctionDecl",
// CHECK-NEXT:           "name": "y",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "void (...)"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CXXConstructExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "NS::X"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "ctorType": {
// CHECK-NEXT:         "qualType": "void (const X &) noexcept"
// CHECK-NEXT:        },
// CHECK-NEXT:        "hadMultipleCandidates": true,
// CHECK-NEXT:        "constructionKind": "complete",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "const X"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "castKind": "NoOp",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "NS::X"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "VarDecl",
// CHECK-NEXT:             "name": "x",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "NS::X"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
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
// CHECK:       "name": "TestNonADLCall3",
// CHECK-NEXT:  "mangledName": "_ZN19test_adl_call_three15TestNonADLCall3Ev",
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
// CHECK-NEXT:        "name": "x",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "X"
// CHECK-NEXT:        },
// CHECK-NEXT:        "init": "call",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXConstructExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "X"
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
// CHECK-NEXT:         "qualType": "void (*)(X)"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "FunctionToPointerDecay",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "void (X)"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "FunctionDecl",
// CHECK-NEXT:           "name": "f",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "void (X)"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CXXConstructExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "X"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "ctorType": {
// CHECK-NEXT:         "qualType": "void (const X &) noexcept"
// CHECK-NEXT:        },
// CHECK-NEXT:        "hadMultipleCandidates": true,
// CHECK-NEXT:        "constructionKind": "complete",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "const X"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "castKind": "NoOp",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "X"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "VarDecl",
// CHECK-NEXT:             "name": "x",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "X"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
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
