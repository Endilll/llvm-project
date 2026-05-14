// RUN: %clang_cc1 -triple x86_64-unknown-unknown -Wno-strict-prototypes -ast-dump=json -ast-dump-filter Test %s | FileCheck %s

#include "Inputs/json-dump-include.h"

int TestLocation;
TestTypedefFromInclude TestLocationFromInclude;

struct TestIndent {
  int x;
};

struct TestChildren {
  int x;
  struct y {
    int z;
  };
};

void testLabelDecl(void) {
  __label__ TestLabelDecl;
  TestLabelDecl: goto TestLabelDecl;
}

typedef int TestTypedefDecl;

enum TestEnumDecl {
  testEnumDecl
};

struct TestEnumDeclAnon {
  enum {
    testEnumDeclAnon
  } e;
};

enum TestEnumDeclForward;

struct TestRecordDecl {
  int i;
};

struct TestRecordDeclEmpty {
};

struct TestRecordDeclAnon1 {
  struct {
  } testRecordDeclAnon1;
};

struct TestRecordDeclAnon2 {
  struct {
  };
};

struct TestRecordDeclForward;

enum testEnumConstantDecl {
  TestEnumConstantDecl,
  TestEnumConstantDeclInit = 1
};

struct TestIndirectFieldDecl {
  struct {
    int Field;
  };
};

// FIXME: It would be nice to dump the enum and its enumerators.
int TestFunctionDecl(int x, enum { e } y) {
  return x;
}

// FIXME: It would be nice to dump 'Enum' and 'e'.
int TestFunctionDecl2(enum Enum { e } x) { return x; }
int TestFunctionDeclProto(int x);
void TestFunctionDeclNoProto();
extern int TestFunctionDeclSC(void);
inline int TestFunctionDeclInline(void);

struct testFieldDecl {
  int TestFieldDecl;
  int TestFieldDeclWidth : 1;
};

int TestVarDecl;
extern int TestVarDeclSC;
__thread int TestVarDeclThread;
int TestVarDeclInit = 0;

void testParmVarDecl(int TestParmVarDecl);


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "TypedefDecl",
// CHECK:       "isReferenced": true,
// CHECK-LABEL: "name": "TestTypedefFromInclude",
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
// CHECK:  "kind": "VarDecl",
// CHECK-LABEL: "name": "TestLocation",
// CHECK-NEXT:  "mangledName": "TestLocation",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  }
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "VarDecl",
// CHECK-LABEL: "name": "TestLocationFromInclude",
// CHECK-NEXT:  "mangledName": "TestLocationFromInclude",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "desugaredQualType": "int",
// CHECK-NEXT:   "qualType": "TestTypedefFromInclude",
// CHECK-NEXT:   "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:  }
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "RecordDecl",
// CHECK-LABEL:  "name": "TestIndent",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FieldDecl",
// CHECK:         "name": "x",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "RecordDecl",
// CHECK-LABEL: "name": "TestChildren",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FieldDecl",
// CHECK:         "name": "x",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "RecordDecl",
// CHECK:         "parentDeclContextId": "0x{{.*}}",
// CHECK-NEXT:    "name": "y",
// CHECK-NEXT:    "tagUsed": "struct",
// CHECK-NEXT:    "completeDefinition": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "FieldDecl",
// CHECK:           "name": "z",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "LabelDecl",
// CHECK:       "isUsed": true,
// CHECK-NEXT:  "name": "TestLabelDecl"
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "TypedefDecl",
// CHECK-LABEL: "name": "TestTypedefDecl",
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
// CHECK:       "kind": "EnumDecl",
// CHECK-LABEL: "name": "TestEnumDecl",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "EnumConstantDecl",
// CHECK:         "name": "testEnumDecl",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "RecordDecl",
// CHECK-LABEL: "name": "TestEnumDeclAnon",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "EnumDecl",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "EnumConstantDecl",
// CHECK:           "name": "testEnumDeclAnon",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FieldDecl",
// CHECK:         "name": "e",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "enum (unnamed at {{.*}}:31:3)"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "EnumDecl",
// CHECK-LABEL:  "name": "TestEnumDeclForward"
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "RecordDecl",
// CHECK-LABEL: "name": "TestRecordDecl",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FieldDecl",
// CHECK:         "name": "i",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "RecordDecl",
// CHECK:       "name": "TestRecordDeclEmpty",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "RecordDecl",
// CHECK-LABEL: "name": "TestRecordDeclAnon1",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "RecordDecl",
// CHECK:         "tagUsed": "struct",
// CHECK-NEXT:    "completeDefinition": true
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FieldDecl",
// CHECK:         "name": "testRecordDeclAnon1",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "struct (unnamed at {{.*}}:46:3)"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "RecordDecl",
// CHECK-LABEL: "name": "TestRecordDeclAnon2",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "RecordDecl",
// CHECK:         "tagUsed": "struct",
// CHECK-NEXT:    "completeDefinition": true
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FieldDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "struct TestRecordDeclAnon2::(anonymous at {{.*}}:51:3)"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "RecordDecl",
// CHECK-LABEL: "name": "TestRecordDeclForward",
// CHECK-NEXT:  "tagUsed": "struct"
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "EnumConstantDecl",
// CHECK-LABEL: "name": "TestEnumConstantDecl",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  }
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "EnumConstantDecl",
// CHECK-LABEL: "name": "TestEnumConstantDeclInit",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ConstantExpr",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "value": "1",
// CHECK-NEXT:    "inner": [
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


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "RecordDecl",
// CHECK-LABEL: "name": "TestIndirectFieldDecl",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "RecordDecl",
// CHECK:         "tagUsed": "struct",
// CHECK-NEXT:    "completeDefinition": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "FieldDecl",
// CHECK:           "name": "Field",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FieldDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "struct TestIndirectFieldDecl::(anonymous at {{.*}}:63:3)"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "IndirectFieldDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "Field"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "FunctionDecl",
// CHECK-LABEL: "name": "TestFunctionDecl",
// CHECK-NEXT:  "mangledName": "TestFunctionDecl",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int (int, enum (unnamed at {{.*}}:69:29))"
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
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "name": "y",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "enum (unnamed at {{.*}}:69:29)"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CompoundStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ReturnStmt",
// CHECK:           "inner": [
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
// CHECK-NEXT:           "name": "x",
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


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "FunctionDecl",
// CHECK-LABEL: "name": "TestFunctionDecl2",
// CHECK-NEXT:  "mangledName": "TestFunctionDecl2",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int (enum Enum)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isUsed": true,
// CHECK-NEXT:    "name": "x",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "enum Enum"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CompoundStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ReturnStmt",
// CHECK:           "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "IntegralCast",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "enum Enum"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "LValueToRValue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "enum Enum"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "ParmVarDecl",
// CHECK-NEXT:             "name": "x",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "enum Enum"
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


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "FunctionDecl",
// CHECK-LABEL: "name": "TestFunctionDeclProto",
// CHECK-NEXT:  "mangledName": "TestFunctionDeclProto",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int (int)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "name": "x",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "FunctionDecl",
// CHECK-LABEL: "name": "TestFunctionDeclNoProto",
// CHECK-NEXT:  "mangledName": "TestFunctionDeclNoProto",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void ()"
// CHECK-NEXT:  }
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "FunctionDecl",
// CHECK-LABEL: "name": "TestFunctionDeclSC",
// CHECK-NEXT:  "mangledName": "TestFunctionDeclSC",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int (void)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "storageClass": "extern"
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "FunctionDecl",
// CHECK-LABEL: "name": "TestFunctionDeclInline",
// CHECK-NEXT:  "mangledName": "TestFunctionDeclInline",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int (void)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inline": true
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "FieldDecl",
// CHECK-LABEL: "name": "TestFieldDecl",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  }
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "FieldDecl",
// CHECK-LABEL: "name": "TestFieldDeclWidth",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "isBitfield": true,
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ConstantExpr",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "value": "1",
// CHECK-NEXT:    "inner": [
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


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "VarDecl",
// CHECK-LABEL: "name": "TestVarDecl",
// CHECK-NEXT:  "mangledName": "TestVarDecl",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  }
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "VarDecl",
// CHECK-LABEL: "name": "TestVarDeclSC",
// CHECK-NEXT:  "mangledName": "TestVarDeclSC",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "storageClass": "extern"
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "VarDecl",
// CHECK-LABEL: "name": "TestVarDeclThread",
// CHECK-NEXT:  "mangledName": "TestVarDeclThread",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "tls": "static"
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "VarDecl",
// CHECK-LABEL: "name": "TestVarDeclInit",
// CHECK-NEXT:  "mangledName": "TestVarDeclInit",
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


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "ParmVarDecl",
// CHECK-LABEL: "name": "TestParmVarDecl",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  }
// CHECK-NEXT: }
