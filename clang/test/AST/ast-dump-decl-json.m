// RUN: %clang_cc1 -triple x86_64-apple-darwin -Wno-unused -fblocks -ast-dump=json -ast-dump-filter Test %s | FileCheck %s

@protocol P
@end

@interface A
@end

@interface TestObjCIvarDecl : A
@end

@implementation TestObjCIvarDecl {
  int varDefault;
  @private int varPrivate;
  @protected int varProtected;
  @public int varPublic;
  @package int varPackage;
}
@end

@interface testObjCMethodDecl : A {
}
- (int) TestObjCMethodDecl: (int)i, ...;
@end

@implementation testObjCMethodDecl
- (int) TestObjCMethodDecl: (int)i, ... {
  return 0;
}
@end

@protocol TestObjCProtocolDecl
- (void) foo;
@end

@interface TestObjCClass : A <P>
- (void) foo;
@end

@implementation TestObjCClass : A {
  int i;
}
- (void) foo {
}
@end

@interface TestObjCClass (TestObjCCategoryDecl) <P>
- (void) bar;
@end

@interface TestGenericInterface<T> : A<P> {
}
@end

@implementation TestObjCClass (TestObjCCategoryDecl)
- (void) bar {
}
@end

@compatibility_alias TestObjCCompatibleAliasDecl A;

@interface TestObjCProperty: A
@property(getter=getterFoo, setter=setterFoo:) int foo;
@property int bar;
@end

@implementation TestObjCProperty {
  int i;
}
@synthesize foo=i;
@synthesize bar;
@end

void TestBlockDecl(int x) {
  ^(int y, ...){ x; };
}

@interface B
+ (int) foo;
@end

void f(void) {
  __typeof__(B.foo) Test;
}


// CHECK:       "kind": "ObjCInterfaceDecl",
// CHECK:       "name": "TestObjCIvarDecl",
// CHECK-NEXT:  "mangledName": "_OBJC_CLASS_$_TestObjCIvarDecl",
// CHECK-NEXT:  "super": {
// CHECK-NEXT:   "id": "0x{{.*}}",
// CHECK-NEXT:   "kind": "ObjCInterfaceDecl",
// CHECK-NEXT:   "name": "A"
// CHECK-NEXT:  },
// CHECK-NEXT:  "implementation": {
// CHECK-NEXT:   "id": "0x{{.*}}",
// CHECK-NEXT:   "kind": "ObjCImplementationDecl",
// CHECK-NEXT:   "name": "TestObjCIvarDecl"
// CHECK-NEXT:  }
// CHECK-NEXT: }


// CHECK:       "kind": "ObjCImplementationDecl",
// CHECK:       "name": "TestObjCIvarDecl",
// CHECK-NEXT:  "super": {
// CHECK-NEXT:   "id": "0x0"
// CHECK-NEXT:  },
// CHECK-NEXT:  "interface": {
// CHECK-NEXT:   "id": "0x{{.*}}",
// CHECK-NEXT:   "kind": "ObjCInterfaceDecl",
// CHECK-NEXT:   "name": "TestObjCIvarDecl"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCIvarDecl",
// CHECK:         "name": "varDefault",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "access": "private"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCIvarDecl",
// CHECK:         "name": "varPrivate",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "access": "private"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCIvarDecl",
// CHECK:         "name": "varProtected",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "access": "protected"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCIvarDecl",
// CHECK:         "name": "varPublic",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "access": "public"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCIvarDecl",
// CHECK:         "name": "varPackage",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "access": "package"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "ObjCMethodDecl",
// CHECK:       "name": "TestObjCMethodDecl:",
// CHECK-NEXT:  "mangledName": "-[testObjCMethodDecl TestObjCMethodDecl:]",
// CHECK-NEXT:  "returnType": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "instance": true,
// CHECK-NEXT:  "variadic": true,
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "name": "i",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "ObjCMethodDecl",
// CHECK:       "name": "TestObjCMethodDecl:",
// CHECK-NEXT:  "mangledName": "-[testObjCMethodDecl TestObjCMethodDecl:]",
// CHECK-NEXT:  "returnType": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "instance": true,
// CHECK-NEXT:  "variadic": true,
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ImplicitParamDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "self",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "testObjCMethodDecl *"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ImplicitParamDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "_cmd",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "desugaredQualType": "SEL *",
// CHECK-NEXT:     "qualType": "SEL",
// CHECK-NEXT:     "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "name": "i",
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
// CHECK-NEXT:      "kind": "ReturnStmt",
// CHECK:           "inner": [
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
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "ObjCProtocolDecl",
// CHECK:       "name": "TestObjCProtocolDecl",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCMethodDecl",
// CHECK:         "name": "foo",
// CHECK-NEXT:    "mangledName": "-[TestObjCProtocolDecl foo]",
// CHECK-NEXT:    "returnType": {
// CHECK-NEXT:     "qualType": "void"
// CHECK-NEXT:    },
// CHECK-NEXT:    "instance": true
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "ObjCInterfaceDecl",
// CHECK:       "name": "TestObjCClass",
// CHECK-NEXT:  "mangledName": "_OBJC_CLASS_$_TestObjCClass",
// CHECK-NEXT:  "super": {
// CHECK-NEXT:   "id": "0x{{.*}}",
// CHECK-NEXT:   "kind": "ObjCInterfaceDecl",
// CHECK-NEXT:   "name": "A"
// CHECK-NEXT:  },
// CHECK-NEXT:  "implementation": {
// CHECK-NEXT:   "id": "0x{{.*}}",
// CHECK-NEXT:   "kind": "ObjCImplementationDecl",
// CHECK-NEXT:   "name": "TestObjCClass"
// CHECK-NEXT:  },
// CHECK-NEXT:  "protocols": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCProtocolDecl",
// CHECK-NEXT:    "name": "P"
// CHECK-NEXT:   }
// CHECK-NEXT:  ],
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCMethodDecl",
// CHECK:         "name": "foo",
// CHECK-NEXT:    "mangledName": "-[TestObjCClass foo]",
// CHECK-NEXT:    "returnType": {
// CHECK-NEXT:     "qualType": "void"
// CHECK-NEXT:    },
// CHECK-NEXT:    "instance": true
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "ObjCImplementationDecl",
// CHECK:       "name": "TestObjCClass",
// CHECK-NEXT:  "super": {
// CHECK-NEXT:   "id": "0x{{.*}}",
// CHECK-NEXT:   "kind": "ObjCInterfaceDecl",
// CHECK-NEXT:   "name": "A"
// CHECK-NEXT:  },
// CHECK-NEXT:  "interface": {
// CHECK-NEXT:   "id": "0x{{.*}}",
// CHECK-NEXT:   "kind": "ObjCInterfaceDecl",
// CHECK-NEXT:   "name": "TestObjCClass"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCIvarDecl",
// CHECK:         "name": "i",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "access": "private"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCMethodDecl",
// CHECK:         "name": "foo",
// CHECK-NEXT:    "mangledName": "-[TestObjCClass foo]",
// CHECK-NEXT:    "returnType": {
// CHECK-NEXT:     "qualType": "void"
// CHECK-NEXT:    },
// CHECK-NEXT:    "instance": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitParamDecl",
// CHECK:           "isImplicit": true,
// CHECK-NEXT:      "name": "self",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "TestObjCClass *"
// CHECK-NEXT:      }
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitParamDecl",
// CHECK:           "isImplicit": true,
// CHECK-NEXT:      "name": "_cmd",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "desugaredQualType": "SEL *",
// CHECK-NEXT:       "qualType": "SEL",
// CHECK-NEXT:       "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:      }
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CompoundStmt",
// CHECK:         ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "ObjCCategoryDecl",
// CHECK:       "name": "TestObjCCategoryDecl",
// CHECK-NEXT:  "interface": {
// CHECK-NEXT:   "id": "0x{{.*}}",
// CHECK-NEXT:   "kind": "ObjCInterfaceDecl",
// CHECK-NEXT:   "name": "TestObjCClass"
// CHECK-NEXT:  },
// CHECK-NEXT:  "implementation": {
// CHECK-NEXT:   "id": "0x{{.*}}",
// CHECK-NEXT:   "kind": "ObjCCategoryImplDecl",
// CHECK-NEXT:   "name": "TestObjCCategoryDecl"
// CHECK-NEXT:  },
// CHECK-NEXT:  "protocols": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCProtocolDecl",
// CHECK-NEXT:    "name": "P"
// CHECK-NEXT:   }
// CHECK-NEXT:  ],
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCMethodDecl",
// CHECK:         "name": "bar",
// CHECK-NEXT:    "mangledName": "-[TestObjCClass(TestObjCCategoryDecl) bar]",
// CHECK-NEXT:    "returnType": {
// CHECK-NEXT:     "qualType": "void"
// CHECK-NEXT:    },
// CHECK-NEXT:    "instance": true
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "ObjCInterfaceDecl",
// CHECK:       "name": "TestGenericInterface",
// CHECK-NEXT:  "mangledName": "_OBJC_CLASS_$_TestGenericInterface",
// CHECK-NEXT:  "super": {
// CHECK-NEXT:   "id": "0x{{.*}}",
// CHECK-NEXT:   "kind": "ObjCInterfaceDecl",
// CHECK-NEXT:   "name": "A"
// CHECK-NEXT:  },
// CHECK-NEXT:  "implementation": {
// CHECK-NEXT:   "id": "0x0"
// CHECK-NEXT:  },
// CHECK-NEXT:  "protocols": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCProtocolDecl",
// CHECK-NEXT:    "name": "P"
// CHECK-NEXT:   }
// CHECK-NEXT:  ],
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCTypeParamDecl",
// CHECK:         "name": "T",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "id",
// CHECK-NEXT:     "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "ObjCCategoryImplDecl",
// CHECK:       "name": "TestObjCCategoryDecl",
// CHECK-NEXT:  "interface": {
// CHECK-NEXT:   "id": "0x{{.*}}",
// CHECK-NEXT:   "kind": "ObjCInterfaceDecl",
// CHECK-NEXT:   "name": "TestObjCClass"
// CHECK-NEXT:  },
// CHECK-NEXT:  "categoryDecl": {
// CHECK-NEXT:   "id": "0x{{.*}}",
// CHECK-NEXT:   "kind": "ObjCCategoryDecl",
// CHECK-NEXT:   "name": "TestObjCCategoryDecl"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCMethodDecl",
// CHECK:         "name": "bar",
// CHECK-NEXT:    "mangledName": "-[TestObjCClass(TestObjCCategoryDecl) bar]",
// CHECK-NEXT:    "returnType": {
// CHECK-NEXT:     "qualType": "void"
// CHECK-NEXT:    },
// CHECK-NEXT:    "instance": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitParamDecl",
// CHECK:           "isImplicit": true,
// CHECK-NEXT:      "name": "self",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "TestObjCClass *"
// CHECK-NEXT:      }
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitParamDecl",
// CHECK:           "isImplicit": true,
// CHECK-NEXT:      "name": "_cmd",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "desugaredQualType": "SEL *",
// CHECK-NEXT:       "qualType": "SEL",
// CHECK-NEXT:       "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:      }
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "CompoundStmt",
// CHECK:         ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "ObjCCompatibleAliasDecl",
// CHECK:       "name": "TestObjCCompatibleAliasDecl",
// CHECK-NEXT:  "interface": {
// CHECK-NEXT:   "id": "0x{{.*}}",
// CHECK-NEXT:   "kind": "ObjCInterfaceDecl",
// CHECK-NEXT:   "name": "A"
// CHECK-NEXT:  }
// CHECK-NEXT: }


// CHECK:       "kind": "ObjCInterfaceDecl",
// CHECK:       "name": "TestObjCProperty",
// CHECK-NEXT:  "mangledName": "_OBJC_CLASS_$_TestObjCProperty",
// CHECK-NEXT:  "super": {
// CHECK-NEXT:   "id": "0x{{.*}}",
// CHECK-NEXT:   "kind": "ObjCInterfaceDecl",
// CHECK-NEXT:   "name": "A"
// CHECK-NEXT:  },
// CHECK-NEXT:  "implementation": {
// CHECK-NEXT:   "id": "0x{{.*}}",
// CHECK-NEXT:   "kind": "ObjCImplementationDecl",
// CHECK-NEXT:   "name": "TestObjCProperty"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCPropertyDecl",
// CHECK:         "name": "foo",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "getter": {
// CHECK-NEXT:     "id": "0x{{.*}}",
// CHECK-NEXT:     "kind": "ObjCMethodDecl",
// CHECK-NEXT:     "name": "getterFoo"
// CHECK-NEXT:    },
// CHECK-NEXT:    "setter": {
// CHECK-NEXT:     "id": "0x{{.*}}",
// CHECK-NEXT:     "kind": "ObjCMethodDecl",
// CHECK-NEXT:     "name": "setterFoo:"
// CHECK-NEXT:    },
// CHECK-NEXT:    "assign": true,
// CHECK-NEXT:    "readwrite": true,
// CHECK-NEXT:    "atomic": true,
// CHECK-NEXT:    "unsafe_unretained": true
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCPropertyDecl",
// CHECK:         "name": "bar",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "assign": true,
// CHECK-NEXT:    "readwrite": true,
// CHECK-NEXT:    "atomic": true,
// CHECK-NEXT:    "unsafe_unretained": true
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCMethodDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "getterFoo",
// CHECK-NEXT:    "mangledName": "-[TestObjCProperty getterFoo]",
// CHECK-NEXT:    "returnType": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "instance": true
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCMethodDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "setterFoo:",
// CHECK-NEXT:    "mangledName": "-[TestObjCProperty setterFoo:]",
// CHECK-NEXT:    "returnType": {
// CHECK-NEXT:     "qualType": "void"
// CHECK-NEXT:    },
// CHECK-NEXT:    "instance": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ParmVarDecl",
// CHECK:           "name": "foo",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCMethodDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "bar",
// CHECK-NEXT:    "mangledName": "-[TestObjCProperty bar]",
// CHECK-NEXT:    "returnType": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "instance": true
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCMethodDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "setBar:",
// CHECK-NEXT:    "mangledName": "-[TestObjCProperty setBar:]",
// CHECK-NEXT:    "returnType": {
// CHECK-NEXT:     "qualType": "void"
// CHECK-NEXT:    },
// CHECK-NEXT:    "instance": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ParmVarDecl",
// CHECK:           "name": "bar",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "ObjCImplementationDecl",
// CHECK:       "name": "TestObjCProperty",
// CHECK-NEXT:  "super": {
// CHECK-NEXT:   "id": "0x0"
// CHECK-NEXT:  },
// CHECK-NEXT:  "interface": {
// CHECK-NEXT:   "id": "0x{{.*}}",
// CHECK-NEXT:   "kind": "ObjCInterfaceDecl",
// CHECK-NEXT:   "name": "TestObjCProperty"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCIvarDecl",
// CHECK:         "name": "i",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "access": "private"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCPropertyImplDecl",
// CHECK:         "name": "foo",
// CHECK-NEXT:    "implKind": "synthesize",
// CHECK-NEXT:    "propertyDecl": {
// CHECK-NEXT:     "id": "0x{{.*}}",
// CHECK-NEXT:     "kind": "ObjCPropertyDecl",
// CHECK-NEXT:     "name": "foo"
// CHECK-NEXT:    },
// CHECK-NEXT:    "ivarDecl": {
// CHECK-NEXT:     "id": "0x{{.*}}",
// CHECK-NEXT:     "kind": "ObjCIvarDecl",
// CHECK-NEXT:     "name": "i",
// CHECK-NEXT:     "type": {
// CHECK-NEXT:      "qualType": "int"
// CHECK-NEXT:     }
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCIvarDecl",
// CHECK:         "name": "bar",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "synthesized": true,
// CHECK-NEXT:    "access": "private"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCPropertyImplDecl",
// CHECK:         "name": "bar",
// CHECK-NEXT:    "implKind": "synthesize",
// CHECK-NEXT:    "propertyDecl": {
// CHECK-NEXT:     "id": "0x{{.*}}",
// CHECK-NEXT:     "kind": "ObjCPropertyDecl",
// CHECK-NEXT:     "name": "bar"
// CHECK-NEXT:    },
// CHECK-NEXT:    "ivarDecl": {
// CHECK-NEXT:     "id": "0x{{.*}}",
// CHECK-NEXT:     "kind": "ObjCIvarDecl",
// CHECK-NEXT:     "name": "bar",
// CHECK-NEXT:     "type": {
// CHECK-NEXT:      "qualType": "int"
// CHECK-NEXT:     }
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCMethodDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "getterFoo",
// CHECK-NEXT:    "mangledName": "-[TestObjCProperty getterFoo]",
// CHECK-NEXT:    "returnType": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "instance": true
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCMethodDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "setterFoo:",
// CHECK-NEXT:    "mangledName": "-[TestObjCProperty setterFoo:]",
// CHECK-NEXT:    "returnType": {
// CHECK-NEXT:     "qualType": "void"
// CHECK-NEXT:    },
// CHECK-NEXT:    "instance": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ParmVarDecl",
// CHECK:           "name": "foo",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCMethodDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "bar",
// CHECK-NEXT:    "mangledName": "-[TestObjCProperty bar]",
// CHECK-NEXT:    "returnType": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "instance": true
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ObjCMethodDecl",
// CHECK:         "isImplicit": true,
// CHECK-NEXT:    "name": "setBar:",
// CHECK-NEXT:    "mangledName": "-[TestObjCProperty setBar:]",
// CHECK-NEXT:    "returnType": {
// CHECK-NEXT:     "qualType": "void"
// CHECK-NEXT:    },
// CHECK-NEXT:    "instance": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ParmVarDecl",
// CHECK:           "name": "bar",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "TestBlockDecl",
// CHECK-NEXT:  "mangledName": "_TestBlockDecl",
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
// CHECK-NEXT:       "qualType": "void (^)(int, ...)"
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
// CHECK-NEXT:         "qualType": "void (^)(int, ...)"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "BlockDecl",
// CHECK:               "variadic": true,
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ParmVarDecl",
// CHECK:                 "name": "y",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "int"
// CHECK-NEXT:            }
// CHECK-NEXT:           },
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


// CHECK:       "kind": "VarDecl",
// CHECK:       "name": "Test",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "desugaredQualType": "int",
// CHECK-NEXT:   "qualType": "typeof (B.foo)"
// CHECK-NEXT:  }
// CHECK-NEXT: }
