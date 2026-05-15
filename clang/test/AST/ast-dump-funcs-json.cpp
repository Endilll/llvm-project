// RUN: %clang_cc1 -triple x86_64-unknown-unknown -ast-dump=json -ast-dump-filter Test %s | FileCheck %s

struct S {
  void Test1();
  void Test2() const;
  void Test3() volatile;
  void Test4() &;
  void Test5() &&;
  virtual void Test6(float, int = 12);
  virtual void Test7() = 0;
};

struct T : S { // T is not referenced, but S is
  void Test6(float, int = 100) override;
};

struct U {
  void Test1();
};
void U::Test1() {} // parent

void Test1();
void Test2(void);
void Test3(int a, int b);
void Test4(int a, int b = 12);
constexpr void Test5(void);
static void Test6(void);
extern void Test7(void);
inline void Test8(void);
void Test9(void) noexcept;
void Test10(void) noexcept(false);
void Test11(void) noexcept(1);

template <typename T>
T Test12(T&);

void Test13(int) {}
void Test14(int, ...) {}

int main() {
  Test1(); // Causes this to be marked 'used'
}

template <typename ...Ts>
struct TestFriends {
    friend Ts...;
};


// CHECK:       "kind": "CXXMethodDecl",
// CHECK:       "name": "Test1",
// CHECK-NEXT:  "mangledName": "_ZN1S5Test1Ev",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void ()"
// CHECK-NEXT:  }
// CHECK-NEXT: }


// CHECK:       "kind": "CXXMethodDecl",
// CHECK:       "name": "Test2",
// CHECK-NEXT:  "mangledName": "_ZNK1S5Test2Ev",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void () const"
// CHECK-NEXT:  }
// CHECK-NEXT: }


// CHECK:       "kind": "CXXMethodDecl",
// CHECK:       "name": "Test3",
// CHECK-NEXT:  "mangledName": "_ZNV1S5Test3Ev",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void () volatile"
// CHECK-NEXT:  }
// CHECK-NEXT: }


// CHECK:       "kind": "CXXMethodDecl",
// CHECK:       "name": "Test4",
// CHECK-NEXT:  "mangledName": "_ZNR1S5Test4Ev",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void () &"
// CHECK-NEXT:  }
// CHECK-NEXT: }


// CHECK:       "kind": "CXXMethodDecl",
// CHECK:       "name": "Test5",
// CHECK-NEXT:  "mangledName": "_ZNO1S5Test5Ev",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void () &&"
// CHECK-NEXT:  }
// CHECK-NEXT: }


// CHECK:       "kind": "CXXMethodDecl",
// CHECK:       "name": "Test6",
// CHECK-NEXT:  "mangledName": "_ZN1S5Test6Efi",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (float, int)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "virtual": true,
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "float"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "init": "c",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "IntegerLiteral",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "value": "12"
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXMethodDecl",
// CHECK:       "name": "Test7",
// CHECK-NEXT:  "mangledName": "_ZN1S5Test7Ev",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void ()"
// CHECK-NEXT:  },
// CHECK-NEXT:  "virtual": true,
// CHECK-NEXT:  "pure": true
// CHECK-NEXT: }


// CHECK:       "kind": "CXXMethodDecl",
// CHECK:       "name": "Test6",
// CHECK-NEXT:  "mangledName": "_ZN1T5Test6Efi",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (float, int)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "float"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "init": "c",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "IntegerLiteral",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "value": "100"
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "OverrideAttr",
// CHECK:       ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXMethodDecl",
// CHECK:       "name": "Test1",
// CHECK-NEXT:  "mangledName": "_ZN1U5Test1Ev",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void ()"
// CHECK-NEXT:  }
// CHECK-NEXT: }


// CHECK:       "kind": "CXXMethodDecl",
// CHECK:       "parentDeclContextId": "0x{{.*}}",
// CHECK-NEXT:  "previousDecl": "0x{{.*}}",
// CHECK-NEXT:  "name": "Test1",
// CHECK-NEXT:  "mangledName": "_ZN1U5Test1Ev",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void ()"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CompoundStmt",
// CHECK:       ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "isUsed": true,
// CHECK-NEXT:  "name": "Test1",
// CHECK-NEXT:  "mangledName": "_Z5Test1v",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void ()"
// CHECK-NEXT:  }
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "Test2",
// CHECK-NEXT:  "mangledName": "_Z5Test2v",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void ()"
// CHECK-NEXT:  }
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "Test3",
// CHECK-NEXT:  "mangledName": "_Z5Test3ii",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (int, int)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "name": "a",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "name": "b",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "Test4",
// CHECK-NEXT:  "mangledName": "_Z5Test4ii",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (int, int)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "name": "a",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "name": "b",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "init": "c",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "IntegerLiteral",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "value": "12"
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "Test5",
// CHECK-NEXT:  "mangledName": "_Z5Test5v",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void ()"
// CHECK-NEXT:  },
// CHECK-NEXT:  "constexpr": true
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "Test6",
// CHECK-NEXT:  "mangledName": "_ZL5Test6v",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void ()"
// CHECK-NEXT:  },
// CHECK-NEXT:  "storageClass": "static"
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "Test7",
// CHECK-NEXT:  "mangledName": "_Z5Test7v",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void ()"
// CHECK-NEXT:  },
// CHECK-NEXT:  "storageClass": "extern"
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "Test8",
// CHECK-NEXT:  "mangledName": "_Z5Test8v",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void ()"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inline": true
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "Test9",
// CHECK-NEXT:  "mangledName": "_Z5Test9v",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void () noexcept"
// CHECK-NEXT:  }
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "Test10",
// CHECK-NEXT:  "mangledName": "_Z6Test10v",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void () noexcept(false)"
// CHECK-NEXT:  }
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "Test11",
// CHECK-NEXT:  "mangledName": "_Z6Test11v",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void () noexcept(1)"
// CHECK-NEXT:  }
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionTemplateDecl",
// CHECK:       "name": "Test12",
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
// CHECK:         "name": "Test12",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "T (T &)"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ParmVarDecl",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "T &"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "Test13",
// CHECK-NEXT:  "mangledName": "_Z6Test13i",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (int)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CompoundStmt",
// CHECK:       ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "Test14",
// CHECK-NEXT:  "mangledName": "_Z6Test14iz",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (int, ...)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "variadic": true,
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CompoundStmt",
// CHECK:       ]
// CHECK-NEXT: }


// CHECK:       "kind": "ClassTemplateDecl",
// CHECK:       "name": "TestFriends",
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
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "name": "TestFriends",
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
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXRecordDecl",
// CHECK:           "isImplicit": true,
// CHECK-NEXT:      "name": "TestFriends",
// CHECK-NEXT:      "tagUsed": "struct"
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "FriendDecl",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "Ts"
// CHECK-NEXT:      },
// CHECK-NEXT:      "isPackExpansion": true
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }
