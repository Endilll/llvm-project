// RUN: %clang_cc1 -std=c++17 -triple x86_64-unknown-unknown -ast-dump=json %s | FileCheck -strict-whitespace %s

template <typename Ty>
void a(Ty);

template <typename... Ty>
void b(Ty...);

template <class Ty, typename Uy>
void c(Ty);

template <>
void c<float, int>(float);

template <typename Ty, template<typename> typename Uy>
void d(Ty, Uy<Ty>);

template <class Ty>
void e(Ty);

template <int N>
void f(int i = N);

template <typename Ty = int>
void g(Ty);

template <typename = void>
void h();

template <typename Ty>
struct R {};

template <>
struct R<int> {};

template <typename Ty, class Uy>
struct S {};

template <typename Ty>
struct S<Ty, int> {};

template <auto>
struct T {};

template <decltype(auto)>
struct U {};

template <typename Ty>
struct V {
  template <typename Uy>
  void f();
};

template <typename Ty>
template <typename Uy>
void V<Ty>::f() {}

template <template <typename> class = R>
void i();

template <int A> class W{
};
W(int)->W<1>;

// CHECK:         "kind": "FunctionTemplateDecl",
// CHECK:         "name": "a",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "TemplateTypeParmDecl",
// CHECK:           "isReferenced": true,
// CHECK-NEXT:      "name": "Ty",
// CHECK-NEXT:      "tagUsed": "typename",
// CHECK-NEXT:      "depth": 0,
// CHECK-NEXT:      "index": 0
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "FunctionDecl",
// CHECK:           "name": "a",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "void (Ty)"
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ParmVarDecl",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "Ty"
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FunctionTemplateDecl",
// CHECK:         "name": "b",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "TemplateTypeParmDecl",
// CHECK:           "isReferenced": true,
// CHECK-NEXT:      "name": "Ty",
// CHECK-NEXT:      "tagUsed": "typename",
// CHECK-NEXT:      "depth": 0,
// CHECK-NEXT:      "index": 0,
// CHECK-NEXT:      "isParameterPack": true
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "FunctionDecl",
// CHECK:           "name": "b",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "void (Ty...)"
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ParmVarDecl",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "Ty..."
// CHECK-NEXT:        },
// CHECK-NEXT:        "isParameterPack": true
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FunctionTemplateDecl",
// CHECK:         "name": "c",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "TemplateTypeParmDecl",
// CHECK:           "isReferenced": true,
// CHECK-NEXT:      "name": "Ty",
// CHECK-NEXT:      "tagUsed": "class",
// CHECK-NEXT:      "depth": 0,
// CHECK-NEXT:      "index": 0
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "TemplateTypeParmDecl",
// CHECK:           "name": "Uy",
// CHECK-NEXT:      "tagUsed": "typename",
// CHECK-NEXT:      "depth": 0,
// CHECK-NEXT:      "index": 1
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "FunctionDecl",
// CHECK:           "name": "c",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "void (Ty)"
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ParmVarDecl",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "Ty"
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "FunctionDecl",
// CHECK-NEXT:      "name": "c",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "void (float)"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FunctionDecl",
// CHECK:         "previousDecl": "0x{{.*}}",
// CHECK-NEXT:    "name": "c",
// CHECK-NEXT:    "mangledName": "_Z1cIfiEvT_",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void (float)"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "kind": "TemplateArgument",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "float"
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "BuiltinType",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "float"
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "kind": "TemplateArgument",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "BuiltinType",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ParmVarDecl",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "float"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FunctionTemplateDecl",
// CHECK:         "name": "d",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "TemplateTypeParmDecl",
// CHECK:           "isReferenced": true,
// CHECK-NEXT:      "name": "Ty",
// CHECK-NEXT:      "tagUsed": "typename",
// CHECK-NEXT:      "depth": 0,
// CHECK-NEXT:      "index": 0
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "TemplateTemplateParmDecl",
// CHECK:           "isReferenced": true,
// CHECK-NEXT:      "name": "Uy",
// CHECK-NEXT:      "depth": 0,
// CHECK-NEXT:      "index": 1,
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "TemplateTypeParmDecl",
// CHECK:             "tagUsed": "typename",
// CHECK-NEXT:        "depth": 1,
// CHECK-NEXT:        "index": 0
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "FunctionDecl",
// CHECK:           "name": "d",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "void (Ty, Uy<Ty>)"
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ParmVarDecl",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "Ty"
// CHECK-NEXT:        }
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ParmVarDecl",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "Uy<Ty>"
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FunctionTemplateDecl",
// CHECK:         "name": "e",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "TemplateTypeParmDecl",
// CHECK:           "isReferenced": true,
// CHECK-NEXT:      "name": "Ty",
// CHECK-NEXT:      "tagUsed": "class",
// CHECK-NEXT:      "depth": 0,
// CHECK-NEXT:      "index": 0
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "FunctionDecl",
// CHECK:           "name": "e",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "void (Ty)"
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ParmVarDecl",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "Ty"
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FunctionTemplateDecl",
// CHECK:         "name": "f",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "NonTypeTemplateParmDecl",
// CHECK:           "isReferenced": true,
// CHECK-NEXT:      "name": "N",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "depth": 0,
// CHECK-NEXT:      "index": 0
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "FunctionDecl",
// CHECK:           "name": "f",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "void (int)"
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ParmVarDecl",
// CHECK:             "name": "i",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "init": "c",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "NonTypeTemplateParmDecl",
// CHECK-NEXT:           "name": "N",
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
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FunctionTemplateDecl",
// CHECK:         "name": "g",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "TemplateTypeParmDecl",
// CHECK:           "isReferenced": true,
// CHECK-NEXT:      "name": "Ty",
// CHECK-NEXT:      "tagUsed": "typename",
// CHECK-NEXT:      "depth": 0,
// CHECK-NEXT:      "index": 0,
// CHECK-NEXT:      "defaultArg": {
// CHECK-NEXT:       "kind": "TemplateArgument",
// CHECK-NEXT:       "type": {
// CHECK-NEXT:        "qualType": "int"
// CHECK-NEXT:       }
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "kind": "TemplateArgument",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "BuiltinType",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "FunctionDecl",
// CHECK:           "name": "g",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "void (Ty)"
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ParmVarDecl",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "Ty"
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FunctionTemplateDecl",
// CHECK:         "name": "h",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "TemplateTypeParmDecl",
// CHECK:           "tagUsed": "typename",
// CHECK-NEXT:      "depth": 0,
// CHECK-NEXT:      "index": 0,
// CHECK-NEXT:      "defaultArg": {
// CHECK-NEXT:       "kind": "TemplateArgument",
// CHECK-NEXT:       "type": {
// CHECK-NEXT:        "qualType": "void"
// CHECK-NEXT:       }
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "kind": "TemplateArgument",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "void"
// CHECK-NEXT:        },
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "BuiltinType",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "void"
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "FunctionDecl",
// CHECK:           "name": "h",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "void ()"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ClassTemplateDecl",
// CHECK:         "name": "R",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "TemplateTypeParmDecl",
// CHECK:           "name": "Ty",
// CHECK-NEXT:      "tagUsed": "typename",
// CHECK-NEXT:      "depth": 0,
// CHECK-NEXT:      "index": 0
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXRecordDecl",
// CHECK:           "name": "R",
// CHECK-NEXT:      "tagUsed": "struct",
// CHECK-NEXT:      "completeDefinition": true,
// CHECK-NEXT:      "definitionData": {
// CHECK-NEXT:       "canConstDefaultInit": true,
// CHECK-NEXT:       "copyAssign": {
// CHECK-NEXT:        "hasConstParam": true,
// CHECK-NEXT:        "implicitHasConstParam": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "copyCtor": {
// CHECK-NEXT:        "hasConstParam": true,
// CHECK-NEXT:        "implicitHasConstParam": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "defaultCtor": {
// CHECK-NEXT:        "defaultedIsConstexpr": true,
// CHECK-NEXT:        "exists": true,
// CHECK-NEXT:        "isConstexpr": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "dtor": {
// CHECK-NEXT:        "irrelevant": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "hasConstexprNonCopyMoveConstructor": true,
// CHECK-NEXT:       "isAggregate": true,
// CHECK-NEXT:       "isEmpty": true,
// CHECK-NEXT:       "isLiteral": true,
// CHECK-NEXT:       "isPOD": true,
// CHECK-NEXT:       "isStandardLayout": true,
// CHECK-NEXT:       "isTrivial": true,
// CHECK-NEXT:       "isTriviallyCopyable": true,
// CHECK-NEXT:       "moveAssign": {
// CHECK-NEXT:        "exists": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "moveCtor": {
// CHECK-NEXT:        "exists": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       }
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CXXRecordDecl",
// CHECK:             "isImplicit": true,
// CHECK-NEXT:        "name": "R",
// CHECK-NEXT:        "tagUsed": "struct"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ClassTemplateSpecializationDecl",
// CHECK-NEXT:      "name": "R"
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ClassTemplateSpecializationDecl",
// CHECK:         "name": "R",
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
// CHECK-NEXT:      "kind": "TemplateArgument",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "BuiltinType",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXRecordDecl",
// CHECK:           "isImplicit": true,
// CHECK-NEXT:      "name": "R",
// CHECK-NEXT:      "tagUsed": "struct"
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ClassTemplateDecl",
// CHECK:         "name": "S",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "TemplateTypeParmDecl",
// CHECK:           "name": "Ty",
// CHECK-NEXT:      "tagUsed": "typename",
// CHECK-NEXT:      "depth": 0,
// CHECK-NEXT:      "index": 0
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "TemplateTypeParmDecl",
// CHECK:           "name": "Uy",
// CHECK-NEXT:      "tagUsed": "class",
// CHECK-NEXT:      "depth": 0,
// CHECK-NEXT:      "index": 1
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXRecordDecl",
// CHECK:           "name": "S",
// CHECK-NEXT:      "tagUsed": "struct",
// CHECK-NEXT:      "completeDefinition": true,
// CHECK-NEXT:      "definitionData": {
// CHECK-NEXT:       "canConstDefaultInit": true,
// CHECK-NEXT:       "copyAssign": {
// CHECK-NEXT:        "hasConstParam": true,
// CHECK-NEXT:        "implicitHasConstParam": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "copyCtor": {
// CHECK-NEXT:        "hasConstParam": true,
// CHECK-NEXT:        "implicitHasConstParam": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "defaultCtor": {
// CHECK-NEXT:        "defaultedIsConstexpr": true,
// CHECK-NEXT:        "exists": true,
// CHECK-NEXT:        "isConstexpr": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "dtor": {
// CHECK-NEXT:        "irrelevant": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "hasConstexprNonCopyMoveConstructor": true,
// CHECK-NEXT:       "isAggregate": true,
// CHECK-NEXT:       "isEmpty": true,
// CHECK-NEXT:       "isLiteral": true,
// CHECK-NEXT:       "isPOD": true,
// CHECK-NEXT:       "isStandardLayout": true,
// CHECK-NEXT:       "isTrivial": true,
// CHECK-NEXT:       "isTriviallyCopyable": true,
// CHECK-NEXT:       "moveAssign": {
// CHECK-NEXT:        "exists": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "moveCtor": {
// CHECK-NEXT:        "exists": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       }
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CXXRecordDecl",
// CHECK:             "isImplicit": true,
// CHECK-NEXT:        "name": "S",
// CHECK-NEXT:        "tagUsed": "struct"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ClassTemplatePartialSpecializationDecl",
// CHECK:         "name": "S",
// CHECK-NEXT:    "tagUsed": "struct",
// CHECK-NEXT:    "completeDefinition": true,
// CHECK-NEXT:    "definitionData": {
// CHECK-NEXT:     "canConstDefaultInit": true,
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
// CHECK-NEXT:      "kind": "TemplateArgument",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "type-parameter-0-0"
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "TemplateTypeParmType",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "type-parameter-0-0"
// CHECK-NEXT:        },
// CHECK-NEXT:        "isDependent": true,
// CHECK-NEXT:        "isInstantiationDependent": true,
// CHECK-NEXT:        "depth": 0,
// CHECK-NEXT:        "index": 0,
// CHECK-NEXT:        "decl": {
// CHECK-NEXT:         "id": "0x0"
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "kind": "TemplateArgument",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "BuiltinType",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "TemplateTypeParmDecl",
// CHECK:           "isReferenced": true,
// CHECK-NEXT:      "name": "Ty",
// CHECK-NEXT:      "tagUsed": "typename",
// CHECK-NEXT:      "depth": 0,
// CHECK-NEXT:      "index": 0
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXRecordDecl",
// CHECK:           "isImplicit": true,
// CHECK-NEXT:      "name": "S",
// CHECK-NEXT:      "tagUsed": "struct"
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ClassTemplateDecl",
// CHECK:         "name": "T",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "NonTypeTemplateParmDecl",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "auto"
// CHECK-NEXT:      },
// CHECK-NEXT:      "depth": 0,
// CHECK-NEXT:      "index": 0
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXRecordDecl",
// CHECK:           "name": "T",
// CHECK-NEXT:      "tagUsed": "struct",
// CHECK-NEXT:      "completeDefinition": true,
// CHECK-NEXT:      "definitionData": {
// CHECK-NEXT:       "canConstDefaultInit": true,
// CHECK-NEXT:       "copyAssign": {
// CHECK-NEXT:        "hasConstParam": true,
// CHECK-NEXT:        "implicitHasConstParam": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "copyCtor": {
// CHECK-NEXT:        "hasConstParam": true,
// CHECK-NEXT:        "implicitHasConstParam": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "defaultCtor": {
// CHECK-NEXT:        "defaultedIsConstexpr": true,
// CHECK-NEXT:        "exists": true,
// CHECK-NEXT:        "isConstexpr": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "dtor": {
// CHECK-NEXT:        "irrelevant": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "hasConstexprNonCopyMoveConstructor": true,
// CHECK-NEXT:       "isAggregate": true,
// CHECK-NEXT:       "isEmpty": true,
// CHECK-NEXT:       "isLiteral": true,
// CHECK-NEXT:       "isPOD": true,
// CHECK-NEXT:       "isStandardLayout": true,
// CHECK-NEXT:       "isTrivial": true,
// CHECK-NEXT:       "isTriviallyCopyable": true,
// CHECK-NEXT:       "moveAssign": {
// CHECK-NEXT:        "exists": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "moveCtor": {
// CHECK-NEXT:        "exists": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       }
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CXXRecordDecl",
// CHECK:             "isImplicit": true,
// CHECK-NEXT:        "name": "T",
// CHECK-NEXT:        "tagUsed": "struct"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ClassTemplateDecl",
// CHECK:         "name": "U",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "NonTypeTemplateParmDecl",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "decltype(auto)"
// CHECK-NEXT:      },
// CHECK-NEXT:      "depth": 0,
// CHECK-NEXT:      "index": 0
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXRecordDecl",
// CHECK:           "name": "U",
// CHECK-NEXT:      "tagUsed": "struct",
// CHECK-NEXT:      "completeDefinition": true,
// CHECK-NEXT:      "definitionData": {
// CHECK-NEXT:       "canConstDefaultInit": true,
// CHECK-NEXT:       "copyAssign": {
// CHECK-NEXT:        "hasConstParam": true,
// CHECK-NEXT:        "implicitHasConstParam": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "copyCtor": {
// CHECK-NEXT:        "hasConstParam": true,
// CHECK-NEXT:        "implicitHasConstParam": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "defaultCtor": {
// CHECK-NEXT:        "defaultedIsConstexpr": true,
// CHECK-NEXT:        "exists": true,
// CHECK-NEXT:        "isConstexpr": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "dtor": {
// CHECK-NEXT:        "irrelevant": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "hasConstexprNonCopyMoveConstructor": true,
// CHECK-NEXT:       "isAggregate": true,
// CHECK-NEXT:       "isEmpty": true,
// CHECK-NEXT:       "isLiteral": true,
// CHECK-NEXT:       "isPOD": true,
// CHECK-NEXT:       "isStandardLayout": true,
// CHECK-NEXT:       "isTrivial": true,
// CHECK-NEXT:       "isTriviallyCopyable": true,
// CHECK-NEXT:       "moveAssign": {
// CHECK-NEXT:        "exists": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "moveCtor": {
// CHECK-NEXT:        "exists": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       }
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CXXRecordDecl",
// CHECK:             "isImplicit": true,
// CHECK-NEXT:        "name": "U",
// CHECK-NEXT:        "tagUsed": "struct"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ClassTemplateDecl",
// CHECK:         "isReferenced": true,
// CHECK-NEXT:    "name": "V",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "TemplateTypeParmDecl",
// CHECK:           "name": "Ty",
// CHECK-NEXT:      "tagUsed": "typename",
// CHECK-NEXT:      "depth": 0,
// CHECK-NEXT:      "index": 0
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXRecordDecl",
// CHECK:           "isReferenced": true,
// CHECK-NEXT:      "name": "V",
// CHECK-NEXT:      "tagUsed": "struct",
// CHECK-NEXT:      "completeDefinition": true,
// CHECK-NEXT:      "definitionData": {
// CHECK-NEXT:       "canConstDefaultInit": true,
// CHECK-NEXT:       "copyAssign": {
// CHECK-NEXT:        "hasConstParam": true,
// CHECK-NEXT:        "implicitHasConstParam": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "copyCtor": {
// CHECK-NEXT:        "hasConstParam": true,
// CHECK-NEXT:        "implicitHasConstParam": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "defaultCtor": {
// CHECK-NEXT:        "defaultedIsConstexpr": true,
// CHECK-NEXT:        "exists": true,
// CHECK-NEXT:        "isConstexpr": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "dtor": {
// CHECK-NEXT:        "irrelevant": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "hasConstexprNonCopyMoveConstructor": true,
// CHECK-NEXT:       "isAggregate": true,
// CHECK-NEXT:       "isEmpty": true,
// CHECK-NEXT:       "isLiteral": true,
// CHECK-NEXT:       "isPOD": true,
// CHECK-NEXT:       "isStandardLayout": true,
// CHECK-NEXT:       "isTrivial": true,
// CHECK-NEXT:       "isTriviallyCopyable": true,
// CHECK-NEXT:       "moveAssign": {
// CHECK-NEXT:        "exists": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "moveCtor": {
// CHECK-NEXT:        "exists": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       }
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CXXRecordDecl",
// CHECK:             "isImplicit": true,
// CHECK-NEXT:        "name": "V",
// CHECK-NEXT:        "tagUsed": "struct"
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "FunctionTemplateDecl",
// CHECK:             "name": "f",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "TemplateTypeParmDecl",
// CHECK:               "name": "Uy",
// CHECK-NEXT:          "tagUsed": "typename",
// CHECK-NEXT:          "depth": 1,
// CHECK-NEXT:          "index": 0
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CXXMethodDecl",
// CHECK:               "name": "f",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "void ()"
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
// CHECK-NEXT:    "kind": "FunctionTemplateDecl",
// CHECK:         "parentDeclContextId": "0x{{.*}}",
// CHECK-NEXT:    "previousDecl": "0x{{.*}}",
// CHECK-NEXT:    "name": "f",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "TemplateTypeParmDecl",
// CHECK:           "name": "Uy",
// CHECK-NEXT:      "tagUsed": "typename",
// CHECK-NEXT:      "depth": 1,
// CHECK-NEXT:      "index": 0
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXMethodDecl",
// CHECK:           "parentDeclContextId": "0x{{.*}}",
// CHECK-NEXT:      "previousDecl": "0x{{.*}}",
// CHECK-NEXT:      "name": "f",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "void ()"
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CompoundStmt",
// CHECK:           ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FunctionTemplateDecl",
// CHECK:         "name": "i",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "TemplateTemplateParmDecl",
// CHECK:           "depth": 0,
// CHECK-NEXT:      "index": 0,
// CHECK-NEXT:      "defaultArg": {
// CHECK-NEXT:       "kind": "TemplateArgument"
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "TemplateTypeParmDecl",
// CHECK:             "tagUsed": "typename",
// CHECK-NEXT:        "depth": 1,
// CHECK-NEXT:        "index": 0
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "kind": "TemplateArgument",
// CHECK:           ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "FunctionDecl",
// CHECK:           "name": "i",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "void ()"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ClassTemplateDecl",
// CHECK:         "isReferenced": true,
// CHECK-NEXT:    "name": "W",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "NonTypeTemplateParmDecl",
// CHECK:           "name": "A",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "depth": 0,
// CHECK-NEXT:      "index": 0
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXRecordDecl",
// CHECK:           "name": "W",
// CHECK-NEXT:      "tagUsed": "class",
// CHECK-NEXT:      "completeDefinition": true,
// CHECK-NEXT:      "definitionData": {
// CHECK-NEXT:       "canConstDefaultInit": true,
// CHECK-NEXT:       "copyAssign": {
// CHECK-NEXT:        "hasConstParam": true,
// CHECK-NEXT:        "implicitHasConstParam": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "copyCtor": {
// CHECK-NEXT:        "hasConstParam": true,
// CHECK-NEXT:        "implicitHasConstParam": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "defaultCtor": {
// CHECK-NEXT:        "defaultedIsConstexpr": true,
// CHECK-NEXT:        "exists": true,
// CHECK-NEXT:        "isConstexpr": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "dtor": {
// CHECK-NEXT:        "irrelevant": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "hasConstexprNonCopyMoveConstructor": true,
// CHECK-NEXT:       "isAggregate": true,
// CHECK-NEXT:       "isEmpty": true,
// CHECK-NEXT:       "isLiteral": true,
// CHECK-NEXT:       "isPOD": true,
// CHECK-NEXT:       "isStandardLayout": true,
// CHECK-NEXT:       "isTrivial": true,
// CHECK-NEXT:       "isTriviallyCopyable": true,
// CHECK-NEXT:       "moveAssign": {
// CHECK-NEXT:        "exists": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       },
// CHECK-NEXT:       "moveCtor": {
// CHECK-NEXT:        "exists": true,
// CHECK-NEXT:        "needsImplicit": true,
// CHECK-NEXT:        "simple": true,
// CHECK-NEXT:        "trivial": true
// CHECK-NEXT:       }
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "CXXRecordDecl",
// CHECK:             "isImplicit": true,
// CHECK-NEXT:        "name": "W",
// CHECK-NEXT:        "tagUsed": "class"
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ClassTemplateSpecializationDecl",
// CHECK:           "isReferenced": true,
// CHECK-NEXT:      "name": "W",
// CHECK-NEXT:      "tagUsed": "class",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "kind": "TemplateArgument",
// CHECK-NEXT:        "value": 1
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FunctionTemplateDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "<deduction guide for W>",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "NonTypeTemplateParmDecl",
// CHECK:           "name": "A",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "depth": 0,
// CHECK-NEXT:      "index": 0
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXDeductionGuideDecl",
// CHECK:           "isImplicit": true,
// CHECK-NEXT:      "name": "<deduction guide for W>",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "auto () -> W<A>"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FunctionTemplateDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "<deduction guide for W>",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "NonTypeTemplateParmDecl",
// CHECK:           "name": "A",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "depth": 0,
// CHECK-NEXT:      "index": 0
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXDeductionGuideDecl",
// CHECK:           "isImplicit": true,
// CHECK-NEXT:      "name": "<deduction guide for W>",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "auto (W<A>) -> W<A>"
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ParmVarDecl",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "W<A>"
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXDeductionGuideDecl",
// CHECK:         "name": "<deduction guide for W>",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "auto (int) -> W<1>"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ParmVarDecl",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }
