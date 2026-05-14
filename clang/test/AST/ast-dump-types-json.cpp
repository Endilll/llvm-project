// RUN: %clang_cc1 -triple x86_64-unknown-unknown -fsyntax-only -ast-dump=json -ast-dump-filter Test %s | FileCheck %s

namespace NS {
struct S {};
}

struct T {
  int I;
  void F();
};

typedef struct T TestElaboratedType1;
typedef NS::S TestElaboratedType2;

#define CDECL __attribute__((cdecl))
typedef void (CDECL *TestMacroQualifiedType)();

typedef void (T::* TestMemberFunctionPointerType)();
typedef int T::*TestMemberDataPointerType;

typedef int TestQualTypePrinting(const char* c);

typedef int TestUsingShadowDeclType;
namespace TestNamespaceWithUsingShadowType {
using ::TestUsingShadowDeclType;
}

// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "TypedefDecl",
// CHECK-LABEL: "name": "TestElaboratedType1",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "struct T"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "RecordType",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "struct T"
// CHECK-NEXT:    },
// CHECK-NEXT:    "decl": {
// CHECK-NEXT:     "id": "0x{{.*}}",
// CHECK-NEXT:     "kind": "CXXRecordDecl",
// CHECK-NEXT:     "name": "T"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "TypedefDecl",
// CHECK-LABEL: "name": "TestElaboratedType2",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "NS::S"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "RecordType",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "NS::S"
// CHECK-NEXT:    },
// CHECK-NEXT:    "qualifier": "NS::",
// CHECK-NEXT:    "decl": {
// CHECK-NEXT:     "id": "0x{{.*}}",
// CHECK-NEXT:     "kind": "CXXRecordDecl",
// CHECK-NEXT:     "name": "S"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "TypedefDecl",
// CHECK-LABEL: "name": "TestMacroQualifiedType",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "CDECL void (*)()"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "PointerType",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "CDECL void (*)()"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "MacroQualifiedType",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "CDECL void ()"
// CHECK-NEXT:      },
// CHECK-NEXT:      "macroName": "CDECL",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "AttributedType",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "void () __attribute__((cdecl))"
// CHECK-NEXT:        },
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ParenType",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "void ()"
// CHECK-NEXT:          },
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "FunctionProtoType",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "void ()"
// CHECK-NEXT:            },
// CHECK-NEXT:            "cc": "cdecl",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "BuiltinType",
// CHECK-NEXT:              "type": {
// CHECK-NEXT:               "qualType": "void"
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


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "TypedefDecl",
// CHECK-LABEL: "name": "TestMemberFunctionPointerType",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (T::*)()"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "MemberPointerType",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void (T::*)()"
// CHECK-NEXT:    },
// CHECK-NEXT:    "isFunction": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "RecordType",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "T"
// CHECK-NEXT:      },
// CHECK-NEXT:      "decl": {
// CHECK-NEXT:       "id": "0x{{.*}}",
// CHECK-NEXT:       "kind": "CXXRecordDecl",
// CHECK-NEXT:       "name": "T"
// CHECK-NEXT:      }
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ParenType",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "void ()"
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "FunctionProtoType",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "void ()"
// CHECK-NEXT:        },
// CHECK-NEXT:        "cc": "cdecl",
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
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "TypedefDecl",
// CHECK-LABEL: "name": "TestMemberDataPointerType",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int T::*"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "MemberPointerType",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int T::*"
// CHECK-NEXT:    },
// CHECK-NEXT:    "isData": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "RecordType",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "T"
// CHECK-NEXT:      },
// CHECK-NEXT:      "decl": {
// CHECK-NEXT:       "id": "0x{{.*}}",
// CHECK-NEXT:       "kind": "CXXRecordDecl",
// CHECK-NEXT:       "name": "T"
// CHECK-NEXT:      }
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "BuiltinType",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "TypedefDecl",
// CHECK-LABEL: "name": "TestQualTypePrinting",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int (const char *)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FunctionProtoType",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int (const char *)"
// CHECK-NEXT:    },
// CHECK-NEXT:    "cc": "cdecl",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "BuiltinType",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      }
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "PointerType",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "const char *"
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "QualType",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "const char"
// CHECK-NEXT:        },
// CHECK-NEXT:        "qualifiers": "const",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "BuiltinType",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "char"
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


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "TypedefDecl",
// CHECK-LABEL: "name": "TestUsingShadowDeclType",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "BuiltinType",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "NamespaceDecl",
// CHECK-LABEL: "name": "TestNamespaceWithUsingShadowType",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "UsingDecl",
// CHECK:         "name": "::TestUsingShadowDeclType"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "UsingShadowDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "target": {
// CHECK-NEXT:     "id": "0x{{.*}}",
// CHECK-NEXT:     "kind": "TypedefDecl",
// CHECK-NEXT:     "name": "TestUsingShadowDeclType"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "TypedefDecl",
// CHECK:           "name": "TestUsingShadowDeclType",
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
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }
