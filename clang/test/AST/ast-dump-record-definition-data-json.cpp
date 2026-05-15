// RUN: %clang_cc1 -triple x86_64-unknown-unknown -fclang-abi-compat=7.0 -std=c++17 -ast-dump=json %s | FileCheck %s

void f() {
  auto IsNotGenericLambda = [](){};
  auto IsGenericLambda = [](auto){};
}

struct CanPassInRegisters {
  CanPassInRegisters(const CanPassInRegisters&) = default;
};

struct CantPassInRegisters {
  CantPassInRegisters(const CantPassInRegisters&) = delete;
};

struct IsEmpty {
};

struct IsNotEmpty {
  int a;
};

struct IsAggregate {
  int a;
};

struct IsNotAggregate {
private:
  int a;
};

struct IsStandardLayout {
  void f();
};

struct IsNotStandardLayout {
  virtual void f();
};

struct IsTriviallyCopyable {
};

struct IsNotTriviallyCopyable {
  IsNotTriviallyCopyable(const IsNotTriviallyCopyable&) {}
};

struct IsPOD {
  int a;
};

struct IsNotPOD {
  int &a;
};

struct IsTrivial {
  IsTrivial() = default;
};

struct IsNotTrivial {
  IsNotTrivial() {}
};

struct IsPolymorphic {
  virtual void f();
};

struct IsNotPolymorphic {
  void f();
};

struct IsAbstract {
  virtual void f() = 0;
};

struct IsNotAbstract {
  virtual void f();
};

struct IsLiteral {
  ~IsLiteral() = default;
};

struct IsNotLiteral {
  ~IsNotLiteral() {}
};

struct HasUserDeclaredConstructor {
  HasUserDeclaredConstructor() {}
};

struct HasNoUserDeclaredConstructor {
};

struct HasConstexprNonCopyMoveConstructor {
  constexpr HasConstexprNonCopyMoveConstructor() {}
};

struct HasNoConstexprNonCopyMoveConstructor {
  HasNoConstexprNonCopyMoveConstructor() {}
};

struct HasMutableFields {
  mutable int i;
};

struct HasNoMutableFields {
  int i;
};

struct HasVariantMembers {
  union {
    int i;
  };
};

struct HasNoVariantMembers {
};

struct AllowsConstDefaultInit {
  int i = 12;
};

struct DoesNotAllowConstDefaultInit {
  int i;
};


// CHECK:       "kind": "CXXRecordDecl",
// CHECK-NEXT:  "name": "__NSConstantString_tag"
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK-NEXT:  "name": "__va_list_tag"
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "isImplicit": true,
// CHECK-NEXT:  "tagUsed": "class",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canConstDefaultInit": true,
// CHECK-NEXT:   "canPassInRegisters": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "defaultedIsConstexpr": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "isEmpty": true,
// CHECK-NEXT:   "isLambda": true,
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "isTriviallyCopyable": true,
// CHECK-NEXT:   "moveAssign": {},
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXMethodDecl",
// CHECK:         "name": "operator()",
// CHECK-NEXT:    "mangledName": "_ZZ1fvENK3$_0clEv",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void () const"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inline": true,
// CHECK-NEXT:    "constexpr": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CompoundStmt",
// CHECK:         ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXConversionDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "operator void (*)()",
// CHECK-NEXT:    "mangledName": "_ZZ1fvENK3$_0cvPFvvEEv",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void (*() const noexcept)()"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inline": true,
// CHECK-NEXT:    "constexpr": true
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXMethodDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "__invoke",
// CHECK-NEXT:    "mangledName": "_ZZ1fvEN3$_08__invokeEv",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void ()"
// CHECK-NEXT:    },
// CHECK-NEXT:    "storageClass": "static",
// CHECK-NEXT:    "inline": true
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXDestructorDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "isReferenced": true,
// CHECK-NEXT:    "name": "~(lambda at {{.*}}:4:29)",
// CHECK-NEXT:    "mangledName": "_ZZ1fvEN3$_0D1Ev",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void () noexcept"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inline": true,
// CHECK-NEXT:    "explicitlyDefaulted": "default"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "isImplicit": true,
// CHECK-NEXT:  "tagUsed": "class",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canConstDefaultInit": true,
// CHECK-NEXT:   "canPassInRegisters": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "defaultedIsConstexpr": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "isEmpty": true,
// CHECK-NEXT:   "isGenericLambda": true,
// CHECK-NEXT:   "isLambda": true,
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "isTriviallyCopyable": true,
// CHECK-NEXT:   "moveAssign": {},
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FunctionTemplateDecl",
// CHECK:         "name": "operator()",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "TemplateTypeParmDecl",
// CHECK:           "isImplicit": true,
// CHECK-NEXT:      "name": "auto:1",
// CHECK-NEXT:      "tagUsed": "class",
// CHECK-NEXT:      "depth": 0,
// CHECK-NEXT:      "index": 0
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXMethodDecl",
// CHECK:           "name": "operator()",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "auto (auto) const"
// CHECK-NEXT:      },
// CHECK-NEXT:      "inline": true,
// CHECK-NEXT:      "constexpr": true,
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ParmVarDecl",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "auto"
// CHECK-NEXT:        }
// CHECK-NEXT:       },
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
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "operator auto (*)(type-parameter-0-0)",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "TemplateTypeParmDecl",
// CHECK:           "isImplicit": true,
// CHECK-NEXT:      "name": "auto:1",
// CHECK-NEXT:      "tagUsed": "class",
// CHECK-NEXT:      "depth": 0,
// CHECK-NEXT:      "index": 0
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXConversionDecl",
// CHECK:           "isImplicit": true,
// CHECK-NEXT:      "name": "operator auto (*)(type-parameter-0-0)",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "auto (*() const noexcept)(auto)"
// CHECK-NEXT:      },
// CHECK-NEXT:      "inline": true,
// CHECK-NEXT:      "constexpr": true
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FunctionTemplateDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "__invoke",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "TemplateTypeParmDecl",
// CHECK:           "isImplicit": true,
// CHECK-NEXT:      "name": "auto:1",
// CHECK-NEXT:      "tagUsed": "class",
// CHECK-NEXT:      "depth": 0,
// CHECK-NEXT:      "index": 0
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CXXMethodDecl",
// CHECK:           "isImplicit": true,
// CHECK-NEXT:      "name": "__invoke",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "auto (auto)"
// CHECK-NEXT:      },
// CHECK-NEXT:      "storageClass": "static",
// CHECK-NEXT:      "inline": true,
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ParmVarDecl",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "auto"
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXDestructorDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "isReferenced": true,
// CHECK-NEXT:    "name": "~(lambda at {{.*}}:5:26)",
// CHECK-NEXT:    "mangledName": "_ZZ1fvEN3$_1D1Ev",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void () noexcept"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inline": true,
// CHECK-NEXT:    "explicitlyDefaulted": "default"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "CanPassInRegisters",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canConstDefaultInit": true,
// CHECK-NEXT:   "canPassInRegisters": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "trivial": true,
// CHECK-NEXT:    "userDeclared": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "defaultedIsConstexpr": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "hasUserDeclaredConstructor": true,
// CHECK-NEXT:   "isAggregate": true,
// CHECK-NEXT:   "isEmpty": true,
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "isTriviallyCopyable": true,
// CHECK-NEXT:   "moveAssign": {},
// CHECK-NEXT:   "moveCtor": {}
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "isReferenced": true,
// CHECK-NEXT:    "name": "CanPassInRegisters",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXConstructorDecl",
// CHECK:         "name": "CanPassInRegisters",
// CHECK-NEXT:    "mangledName": "_ZN18CanPassInRegistersC1ERKS_",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void (const CanPassInRegisters &)"
// CHECK-NEXT:    },
// CHECK-NEXT:    "constexpr": true,
// CHECK-NEXT:    "explicitlyDefaulted": "default",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ParmVarDecl",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "const CanPassInRegisters &"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "CantPassInRegisters",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canConstDefaultInit": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "trivial": true,
// CHECK-NEXT:    "userDeclared": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "defaultedIsConstexpr": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "hasUserDeclaredConstructor": true,
// CHECK-NEXT:   "isAggregate": true,
// CHECK-NEXT:   "isEmpty": true,
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "isTriviallyCopyable": true,
// CHECK-NEXT:   "moveAssign": {},
// CHECK-NEXT:   "moveCtor": {}
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "isReferenced": true,
// CHECK-NEXT:    "name": "CantPassInRegisters",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXConstructorDecl",
// CHECK:         "name": "CantPassInRegisters",
// CHECK-NEXT:    "mangledName": "_ZN19CantPassInRegistersC1ERKS_",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void (const CantPassInRegisters &)"
// CHECK-NEXT:    },
// CHECK-NEXT:    "explicitlyDeleted": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ParmVarDecl",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "const CantPassInRegisters &"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "IsEmpty",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canConstDefaultInit": true,
// CHECK-NEXT:   "canPassInRegisters": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "defaultedIsConstexpr": true,
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "isConstexpr": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "hasConstexprNonCopyMoveConstructor": true,
// CHECK-NEXT:   "isAggregate": true,
// CHECK-NEXT:   "isEmpty": true,
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isPOD": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "isTrivial": true,
// CHECK-NEXT:   "isTriviallyCopyable": true,
// CHECK-NEXT:   "moveAssign": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "IsEmpty",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "IsNotEmpty",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canPassInRegisters": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "isAggregate": true,
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isPOD": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "isTrivial": true,
// CHECK-NEXT:   "isTriviallyCopyable": true,
// CHECK-NEXT:   "moveAssign": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "IsNotEmpty",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FieldDecl",
// CHECK:         "name": "a",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "IsAggregate",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canPassInRegisters": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "isAggregate": true,
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isPOD": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "isTrivial": true,
// CHECK-NEXT:   "isTriviallyCopyable": true,
// CHECK-NEXT:   "moveAssign": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "IsAggregate",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FieldDecl",
// CHECK:         "name": "a",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "IsNotAggregate",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canPassInRegisters": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "isTrivial": true,
// CHECK-NEXT:   "isTriviallyCopyable": true,
// CHECK-NEXT:   "moveAssign": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "IsNotAggregate",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "AccessSpecDecl",
// CHECK:         "access": "private"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FieldDecl",
// CHECK:         "name": "a",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "IsStandardLayout",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canConstDefaultInit": true,
// CHECK-NEXT:   "canPassInRegisters": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "defaultedIsConstexpr": true,
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "isConstexpr": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "hasConstexprNonCopyMoveConstructor": true,
// CHECK-NEXT:   "isAggregate": true,
// CHECK-NEXT:   "isEmpty": true,
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isPOD": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "isTrivial": true,
// CHECK-NEXT:   "isTriviallyCopyable": true,
// CHECK-NEXT:   "moveAssign": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "IsStandardLayout",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXMethodDecl",
// CHECK:         "name": "f",
// CHECK-NEXT:    "mangledName": "_ZN16IsStandardLayout1fEv",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void ()"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "IsNotStandardLayout",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canConstDefaultInit": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "nonTrivial": true,
// CHECK-NEXT:    "simple": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "nonTrivial": true,
// CHECK-NEXT:    "simple": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "defaultedIsConstexpr": true,
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "isConstexpr": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "nonTrivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "hasConstexprNonCopyMoveConstructor": true,
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isPolymorphic": true,
// CHECK-NEXT:   "moveAssign": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "nonTrivial": true,
// CHECK-NEXT:    "simple": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "nonTrivial": true,
// CHECK-NEXT:    "simple": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "IsNotStandardLayout",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXMethodDecl",
// CHECK:         "name": "f",
// CHECK-NEXT:    "mangledName": "_ZN19IsNotStandardLayout1fEv",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void ()"
// CHECK-NEXT:    },
// CHECK-NEXT:    "virtual": true
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXMethodDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "operator=",
// CHECK-NEXT:    "mangledName": "_ZN19IsNotStandardLayoutaSERKS_",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "IsNotStandardLayout &(const IsNotStandardLayout &)"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inline": true,
// CHECK-NEXT:    "constexpr": true,
// CHECK-NEXT:    "explicitlyDefaulted": "default",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ParmVarDecl",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "const IsNotStandardLayout &"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXMethodDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "operator=",
// CHECK-NEXT:    "mangledName": "_ZN19IsNotStandardLayoutaSEOS_",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "IsNotStandardLayout &(IsNotStandardLayout &&)"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inline": true,
// CHECK-NEXT:    "constexpr": true,
// CHECK-NEXT:    "explicitlyDefaulted": "default",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ParmVarDecl",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "IsNotStandardLayout &&"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXDestructorDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "~IsNotStandardLayout",
// CHECK-NEXT:    "mangledName": "_ZN19IsNotStandardLayoutD1Ev",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void ()"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inline": true,
// CHECK-NEXT:    "explicitlyDefaulted": "default"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "IsTriviallyCopyable",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canConstDefaultInit": true,
// CHECK-NEXT:   "canPassInRegisters": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "defaultedIsConstexpr": true,
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "isConstexpr": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "hasConstexprNonCopyMoveConstructor": true,
// CHECK-NEXT:   "isAggregate": true,
// CHECK-NEXT:   "isEmpty": true,
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isPOD": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "isTrivial": true,
// CHECK-NEXT:   "isTriviallyCopyable": true,
// CHECK-NEXT:   "moveAssign": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "IsTriviallyCopyable",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "IsNotTriviallyCopyable",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canConstDefaultInit": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "nonTrivial": true,
// CHECK-NEXT:    "userDeclared": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "defaultedIsConstexpr": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "hasUserDeclaredConstructor": true,
// CHECK-NEXT:   "isEmpty": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "moveAssign": {},
// CHECK-NEXT:   "moveCtor": {}
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "isReferenced": true,
// CHECK-NEXT:    "name": "IsNotTriviallyCopyable",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXConstructorDecl",
// CHECK:         "name": "IsNotTriviallyCopyable",
// CHECK-NEXT:    "mangledName": "_ZN22IsNotTriviallyCopyableC1ERKS_",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void (const IsNotTriviallyCopyable &)"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ParmVarDecl",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "const IsNotTriviallyCopyable &"
// CHECK-NEXT:      }
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CompoundStmt",
// CHECK:         ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "IsPOD",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canPassInRegisters": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "isAggregate": true,
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isPOD": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "isTrivial": true,
// CHECK-NEXT:   "isTriviallyCopyable": true,
// CHECK-NEXT:   "moveAssign": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "IsPOD",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FieldDecl",
// CHECK:         "name": "a",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "IsNotPOD",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canPassInRegisters": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "isAggregate": true,
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isTrivial": true,
// CHECK-NEXT:   "isTriviallyCopyable": true,
// CHECK-NEXT:   "moveAssign": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "IsNotPOD",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FieldDecl",
// CHECK:         "name": "a",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int &"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "IsTrivial",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canConstDefaultInit": true,
// CHECK-NEXT:   "canPassInRegisters": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "defaultedIsConstexpr": true,
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "isConstexpr": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "hasConstexprNonCopyMoveConstructor": true,
// CHECK-NEXT:   "hasUserDeclaredConstructor": true,
// CHECK-NEXT:   "isAggregate": true,
// CHECK-NEXT:   "isEmpty": true,
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "isTrivial": true,
// CHECK-NEXT:   "isTriviallyCopyable": true,
// CHECK-NEXT:   "moveAssign": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "IsTrivial",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXConstructorDecl",
// CHECK:         "name": "IsTrivial",
// CHECK-NEXT:    "mangledName": "_ZN9IsTrivialC1Ev",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void ()"
// CHECK-NEXT:    },
// CHECK-NEXT:    "constexpr": true,
// CHECK-NEXT:    "explicitlyDefaulted": "default"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "IsNotTrivial",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canConstDefaultInit": true,
// CHECK-NEXT:   "canPassInRegisters": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "defaultedIsConstexpr": true,
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "nonTrivial": true,
// CHECK-NEXT:    "userProvided": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "hasUserDeclaredConstructor": true,
// CHECK-NEXT:   "isEmpty": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "isTriviallyCopyable": true,
// CHECK-NEXT:   "moveAssign": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "IsNotTrivial",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXConstructorDecl",
// CHECK:         "name": "IsNotTrivial",
// CHECK-NEXT:    "mangledName": "_ZN12IsNotTrivialC1Ev",
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


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "IsPolymorphic",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canConstDefaultInit": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "nonTrivial": true,
// CHECK-NEXT:    "simple": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "nonTrivial": true,
// CHECK-NEXT:    "simple": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "defaultedIsConstexpr": true,
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "isConstexpr": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "nonTrivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "hasConstexprNonCopyMoveConstructor": true,
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isPolymorphic": true,
// CHECK-NEXT:   "moveAssign": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "nonTrivial": true,
// CHECK-NEXT:    "simple": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "nonTrivial": true,
// CHECK-NEXT:    "simple": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "IsPolymorphic",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXMethodDecl",
// CHECK:         "name": "f",
// CHECK-NEXT:    "mangledName": "_ZN13IsPolymorphic1fEv",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void ()"
// CHECK-NEXT:    },
// CHECK-NEXT:    "virtual": true
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXMethodDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "operator=",
// CHECK-NEXT:    "mangledName": "_ZN13IsPolymorphicaSERKS_",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "IsPolymorphic &(const IsPolymorphic &)"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inline": true,
// CHECK-NEXT:    "constexpr": true,
// CHECK-NEXT:    "explicitlyDefaulted": "default",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ParmVarDecl",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "const IsPolymorphic &"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXMethodDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "operator=",
// CHECK-NEXT:    "mangledName": "_ZN13IsPolymorphicaSEOS_",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "IsPolymorphic &(IsPolymorphic &&)"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inline": true,
// CHECK-NEXT:    "constexpr": true,
// CHECK-NEXT:    "explicitlyDefaulted": "default",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ParmVarDecl",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "IsPolymorphic &&"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXDestructorDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "~IsPolymorphic",
// CHECK-NEXT:    "mangledName": "_ZN13IsPolymorphicD1Ev",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void ()"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inline": true,
// CHECK-NEXT:    "explicitlyDefaulted": "default"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "IsNotPolymorphic",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canConstDefaultInit": true,
// CHECK-NEXT:   "canPassInRegisters": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "defaultedIsConstexpr": true,
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "isConstexpr": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "hasConstexprNonCopyMoveConstructor": true,
// CHECK-NEXT:   "isAggregate": true,
// CHECK-NEXT:   "isEmpty": true,
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isPOD": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "isTrivial": true,
// CHECK-NEXT:   "isTriviallyCopyable": true,
// CHECK-NEXT:   "moveAssign": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "IsNotPolymorphic",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXMethodDecl",
// CHECK:         "name": "f",
// CHECK-NEXT:    "mangledName": "_ZN16IsNotPolymorphic1fEv",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void ()"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "IsAbstract",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canConstDefaultInit": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "nonTrivial": true,
// CHECK-NEXT:    "simple": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "nonTrivial": true,
// CHECK-NEXT:    "simple": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "defaultedIsConstexpr": true,
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "isConstexpr": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "nonTrivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "hasConstexprNonCopyMoveConstructor": true,
// CHECK-NEXT:   "isAbstract": true,
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isPolymorphic": true,
// CHECK-NEXT:   "moveAssign": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "nonTrivial": true,
// CHECK-NEXT:    "simple": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "nonTrivial": true,
// CHECK-NEXT:    "simple": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "IsAbstract",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXMethodDecl",
// CHECK:         "name": "f",
// CHECK-NEXT:    "mangledName": "_ZN10IsAbstract1fEv",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void ()"
// CHECK-NEXT:    },
// CHECK-NEXT:    "virtual": true,
// CHECK-NEXT:    "pure": true
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXMethodDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "operator=",
// CHECK-NEXT:    "mangledName": "_ZN10IsAbstractaSERKS_",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "IsAbstract &(const IsAbstract &)"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inline": true,
// CHECK-NEXT:    "constexpr": true,
// CHECK-NEXT:    "explicitlyDefaulted": "default",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ParmVarDecl",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "const IsAbstract &"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXMethodDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "operator=",
// CHECK-NEXT:    "mangledName": "_ZN10IsAbstractaSEOS_",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "IsAbstract &(IsAbstract &&)"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inline": true,
// CHECK-NEXT:    "constexpr": true,
// CHECK-NEXT:    "explicitlyDefaulted": "default",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ParmVarDecl",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "IsAbstract &&"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXDestructorDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "~IsAbstract",
// CHECK-NEXT:    "mangledName": "_ZN10IsAbstractD1Ev",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void ()"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inline": true,
// CHECK-NEXT:    "explicitlyDefaulted": "default"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "IsNotAbstract",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canConstDefaultInit": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "nonTrivial": true,
// CHECK-NEXT:    "simple": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "nonTrivial": true,
// CHECK-NEXT:    "simple": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "defaultedIsConstexpr": true,
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "isConstexpr": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "nonTrivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "hasConstexprNonCopyMoveConstructor": true,
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isPolymorphic": true,
// CHECK-NEXT:   "moveAssign": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "nonTrivial": true,
// CHECK-NEXT:    "simple": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "nonTrivial": true,
// CHECK-NEXT:    "simple": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "IsNotAbstract",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXMethodDecl",
// CHECK:         "name": "f",
// CHECK-NEXT:    "mangledName": "_ZN13IsNotAbstract1fEv",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void ()"
// CHECK-NEXT:    },
// CHECK-NEXT:    "virtual": true
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXMethodDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "operator=",
// CHECK-NEXT:    "mangledName": "_ZN13IsNotAbstractaSERKS_",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "IsNotAbstract &(const IsNotAbstract &)"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inline": true,
// CHECK-NEXT:    "constexpr": true,
// CHECK-NEXT:    "explicitlyDefaulted": "default",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ParmVarDecl",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "const IsNotAbstract &"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXMethodDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "operator=",
// CHECK-NEXT:    "mangledName": "_ZN13IsNotAbstractaSEOS_",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "IsNotAbstract &(IsNotAbstract &&)"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inline": true,
// CHECK-NEXT:    "constexpr": true,
// CHECK-NEXT:    "explicitlyDefaulted": "default",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ParmVarDecl",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "IsNotAbstract &&"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXDestructorDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "~IsNotAbstract",
// CHECK-NEXT:    "mangledName": "_ZN13IsNotAbstractD1Ev",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void ()"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inline": true,
// CHECK-NEXT:    "explicitlyDefaulted": "default"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "IsLiteral",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canConstDefaultInit": true,
// CHECK-NEXT:   "canPassInRegisters": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "defaultedIsConstexpr": true,
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "isConstexpr": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "trivial": true,
// CHECK-NEXT:    "userDeclared": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "hasConstexprNonCopyMoveConstructor": true,
// CHECK-NEXT:   "isAggregate": true,
// CHECK-NEXT:   "isEmpty": true,
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "isTrivial": true,
// CHECK-NEXT:   "isTriviallyCopyable": true,
// CHECK-NEXT:   "moveAssign": {},
// CHECK-NEXT:   "moveCtor": {}
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "isReferenced": true,
// CHECK-NEXT:    "name": "IsLiteral",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXDestructorDecl",
// CHECK:         "name": "~IsLiteral",
// CHECK-NEXT:    "mangledName": "_ZN9IsLiteralD1Ev",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void ()"
// CHECK-NEXT:    },
// CHECK-NEXT:    "explicitlyDefaulted": "default"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "IsNotLiteral",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canConstDefaultInit": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "defaultedIsConstexpr": true,
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "isConstexpr": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "nonTrivial": true,
// CHECK-NEXT:    "userDeclared": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "hasConstexprNonCopyMoveConstructor": true,
// CHECK-NEXT:   "isAggregate": true,
// CHECK-NEXT:   "isEmpty": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "moveAssign": {},
// CHECK-NEXT:   "moveCtor": {}
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "isReferenced": true,
// CHECK-NEXT:    "name": "IsNotLiteral",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXDestructorDecl",
// CHECK:         "name": "~IsNotLiteral",
// CHECK-NEXT:    "mangledName": "_ZN12IsNotLiteralD1Ev",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void () noexcept"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CompoundStmt",
// CHECK:         ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "HasUserDeclaredConstructor",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canConstDefaultInit": true,
// CHECK-NEXT:   "canPassInRegisters": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "defaultedIsConstexpr": true,
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "nonTrivial": true,
// CHECK-NEXT:    "userProvided": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "hasUserDeclaredConstructor": true,
// CHECK-NEXT:   "isEmpty": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "isTriviallyCopyable": true,
// CHECK-NEXT:   "moveAssign": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "HasUserDeclaredConstructor",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXConstructorDecl",
// CHECK:         "name": "HasUserDeclaredConstructor",
// CHECK-NEXT:    "mangledName": "_ZN26HasUserDeclaredConstructorC1Ev",
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


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "HasNoUserDeclaredConstructor",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canConstDefaultInit": true,
// CHECK-NEXT:   "canPassInRegisters": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "defaultedIsConstexpr": true,
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "isConstexpr": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "hasConstexprNonCopyMoveConstructor": true,
// CHECK-NEXT:   "isAggregate": true,
// CHECK-NEXT:   "isEmpty": true,
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isPOD": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "isTrivial": true,
// CHECK-NEXT:   "isTriviallyCopyable": true,
// CHECK-NEXT:   "moveAssign": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "HasNoUserDeclaredConstructor",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "HasConstexprNonCopyMoveConstructor",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canConstDefaultInit": true,
// CHECK-NEXT:   "canPassInRegisters": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "defaultedIsConstexpr": true,
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "isConstexpr": true,
// CHECK-NEXT:    "nonTrivial": true,
// CHECK-NEXT:    "userProvided": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "hasConstexprNonCopyMoveConstructor": true,
// CHECK-NEXT:   "hasUserDeclaredConstructor": true,
// CHECK-NEXT:   "isEmpty": true,
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "isTriviallyCopyable": true,
// CHECK-NEXT:   "moveAssign": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "HasConstexprNonCopyMoveConstructor",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXConstructorDecl",
// CHECK:         "name": "HasConstexprNonCopyMoveConstructor",
// CHECK-NEXT:    "mangledName": "_ZN34HasConstexprNonCopyMoveConstructorC1Ev",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void ()"
// CHECK-NEXT:    },
// CHECK-NEXT:    "constexpr": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CompoundStmt",
// CHECK:         ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "HasNoConstexprNonCopyMoveConstructor",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canConstDefaultInit": true,
// CHECK-NEXT:   "canPassInRegisters": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "defaultedIsConstexpr": true,
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "nonTrivial": true,
// CHECK-NEXT:    "userProvided": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "hasUserDeclaredConstructor": true,
// CHECK-NEXT:   "isEmpty": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "isTriviallyCopyable": true,
// CHECK-NEXT:   "moveAssign": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "HasNoConstexprNonCopyMoveConstructor",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXConstructorDecl",
// CHECK:         "name": "HasNoConstexprNonCopyMoveConstructor",
// CHECK-NEXT:    "mangledName": "_ZN36HasNoConstexprNonCopyMoveConstructorC1Ev",
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


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "HasMutableFields",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canConstDefaultInit": true,
// CHECK-NEXT:   "canPassInRegisters": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "hasMutableFields": true,
// CHECK-NEXT:   "isAggregate": true,
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isPOD": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "isTrivial": true,
// CHECK-NEXT:   "isTriviallyCopyable": true,
// CHECK-NEXT:   "moveAssign": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "HasMutableFields",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FieldDecl",
// CHECK:         "name": "i",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "mutable": true
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "HasNoMutableFields",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canPassInRegisters": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "isAggregate": true,
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isPOD": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "isTrivial": true,
// CHECK-NEXT:   "isTriviallyCopyable": true,
// CHECK-NEXT:   "moveAssign": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "HasNoMutableFields",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   },
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


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "HasVariantMembers",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canPassInRegisters": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "hasVariantMembers": true,
// CHECK-NEXT:   "isAggregate": true,
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isPOD": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "isTrivial": true,
// CHECK-NEXT:   "isTriviallyCopyable": true,
// CHECK-NEXT:   "moveAssign": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "HasVariantMembers",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "tagUsed": "union",
// CHECK-NEXT:    "completeDefinition": true,
// CHECK-NEXT:    "definitionData": {
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
// CHECK-NEXT:      "exists": true,
// CHECK-NEXT:      "needsImplicit": true,
// CHECK-NEXT:      "trivial": true
// CHECK-NEXT:     },
// CHECK-NEXT:     "dtor": {
// CHECK-NEXT:      "irrelevant": true,
// CHECK-NEXT:      "needsImplicit": true,
// CHECK-NEXT:      "simple": true,
// CHECK-NEXT:      "trivial": true
// CHECK-NEXT:     },
// CHECK-NEXT:     "hasVariantMembers": true,
// CHECK-NEXT:     "isAggregate": true,
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
// CHECK-NEXT:      "kind": "FieldDecl",
// CHECK:         ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FieldDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "HasVariantMembers::(anonymous union at {{.*}}:111:3)"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "IndirectFieldDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "i"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "HasNoVariantMembers",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canConstDefaultInit": true,
// CHECK-NEXT:   "canPassInRegisters": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "defaultedIsConstexpr": true,
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "isConstexpr": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "hasConstexprNonCopyMoveConstructor": true,
// CHECK-NEXT:   "isAggregate": true,
// CHECK-NEXT:   "isEmpty": true,
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isPOD": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "isTrivial": true,
// CHECK-NEXT:   "isTriviallyCopyable": true,
// CHECK-NEXT:   "moveAssign": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "HasNoVariantMembers",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "AllowsConstDefaultInit",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canConstDefaultInit": true,
// CHECK-NEXT:   "canPassInRegisters": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "defaultedIsConstexpr": true,
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "isConstexpr": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "nonTrivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "hasConstexprNonCopyMoveConstructor": true,
// CHECK-NEXT:   "isAggregate": true,
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "isTriviallyCopyable": true,
// CHECK-NEXT:   "moveAssign": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "AllowsConstDefaultInit",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FieldDecl",
// CHECK-NEXT:    "loc": {
// CHECK-NEXT:     "offset": 1706,
// CHECK-NEXT:     "line": 120,
// CHECK-NEXT:     "col": 7,
// CHECK-NEXT:     "tokLen": 1
// CHECK-NEXT:    },
// CHECK:         "name": "i",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "hasInClassInitializer": true,
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


// CHECK:       "kind": "CXXRecordDecl",
// CHECK:       "name": "DoesNotAllowConstDefaultInit",
// CHECK-NEXT:  "tagUsed": "struct",
// CHECK-NEXT:  "completeDefinition": true,
// CHECK-NEXT:  "definitionData": {
// CHECK-NEXT:   "canPassInRegisters": true,
// CHECK-NEXT:   "copyAssign": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "copyCtor": {
// CHECK-NEXT:    "hasConstParam": true,
// CHECK-NEXT:    "implicitHasConstParam": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "defaultCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "dtor": {
// CHECK-NEXT:    "irrelevant": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "isAggregate": true,
// CHECK-NEXT:   "isLiteral": true,
// CHECK-NEXT:   "isPOD": true,
// CHECK-NEXT:   "isStandardLayout": true,
// CHECK-NEXT:   "isTrivial": true,
// CHECK-NEXT:   "isTriviallyCopyable": true,
// CHECK-NEXT:   "moveAssign": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   },
// CHECK-NEXT:   "moveCtor": {
// CHECK-NEXT:    "exists": true,
// CHECK-NEXT:    "needsImplicit": true,
// CHECK-NEXT:    "simple": true,
// CHECK-NEXT:    "trivial": true
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CXXRecordDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "DoesNotAllowConstDefaultInit",
// CHECK-NEXT:    "tagUsed": "struct"
// CHECK-NEXT:   },
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
