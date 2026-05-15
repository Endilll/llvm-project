// RUN: %clang_cc1 -triple x86_64-pc-win32 -Wno-unused -fblocks -ast-dump=json -ast-dump-filter Test %s | FileCheck %s

typedef long NSInteger;
typedef unsigned long NSUInteger;
typedef signed char BOOL;

@interface NSNumber
@end
@interface NSNumber (NSNumberCreation)
+ (NSNumber *)numberWithChar:(char)value;
+ (NSNumber *)numberWithUnsignedChar:(unsigned char)value;
+ (NSNumber *)numberWithShort:(short)value;
+ (NSNumber *)numberWithUnsignedShort:(unsigned short)value;
+ (NSNumber *)numberWithInt:(int)value;
+ (NSNumber *)numberWithUnsignedInt:(unsigned int)value;
+ (NSNumber *)numberWithLong:(long)value;
+ (NSNumber *)numberWithUnsignedLong:(unsigned long)value;
+ (NSNumber *)numberWithLongLong:(long long)value;
+ (NSNumber *)numberWithUnsignedLongLong:(unsigned long long)value;
+ (NSNumber *)numberWithFloat:(float)value;
+ (NSNumber *)numberWithDouble:(double)value;
+ (NSNumber *)numberWithBool:(BOOL)value;
+ (NSNumber *)numberWithInteger:(NSInteger)value;
+ (NSNumber *)numberWithUnsignedInteger:(NSUInteger)value;
@end

@interface I
{
@public
  int public;
}
- (int) conformsToProtocol : (Protocol *)protocl;
- (void)method1;
+ (void)method2;
@end

@interface J
@property unsigned prop;
@end

@protocol Proto
@end

@interface NSMutableDictionary
- (id)objectForKeyedSubscript:(id)key;
- (void)setObject:(id)object forKeyedSubscript:(id)key;
@end

@interface NSMutableArray
- (id)objectAtIndexedSubscript:(int)index;
- (void)setObject:(id)object atIndexedSubscript:(int)index;
@end

void TestObjCEncode(void) {
  @encode(int);
  @encode(typeof(^{;}));
}

void TestObjCMessage(I *Obj) {
  [Obj method1];
  [I method2];
}

void TestObjCBoxed(void) {
  @(1 + 1);
}

void TestObjCSelector(void) {
  SEL s = @selector(dealloc);
}

void TestObjCProtocol(id Obj) {
  [Obj conformsToProtocol:@protocol(Proto)];
}

void TestObjCPropertyRef(J *Obj) {
  Obj.prop = 12;
  int i = Obj.prop;
}

void TestObjCSubscriptRef(NSMutableArray *Array, NSMutableDictionary *Dict) {
 Array[0] = (void*)0;
 id i = Array[0];
 
 Dict[@"key"] = (void*)0;
 i = Dict[@"key"];
}

void TestObjCIVarRef(I *Ptr) {
  Ptr->public = 0;
}

void TestObjCBoolLiteral(void) {
  __objc_yes;
  __objc_no;
}


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "TestObjCEncode",
// CHECK-NEXT:  "mangledName": "TestObjCEncode",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (void)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CompoundStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "char *"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "ArrayToPointerDecay",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ObjCEncodeExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "char[2]"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "encodedType": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "char *"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "ArrayToPointerDecay",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ObjCEncodeExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "char[3]"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "encodedType": {
// CHECK-NEXT:         "desugaredQualType": "void (^)(void)",
// CHECK-NEXT:         "qualType": "typeof (^{ })"
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "TestObjCMessage",
// CHECK-NEXT:  "mangledName": "TestObjCMessage",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (I *)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isUsed": true,
// CHECK-NEXT:    "name": "Obj",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "I *"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CompoundStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ObjCMessageExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "void"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "selector": "method1",
// CHECK-NEXT:      "receiverKind": "instance",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "I *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "LValueToRValue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "I *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "ParmVarDecl",
// CHECK-NEXT:           "name": "Obj",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "I *"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ObjCMessageExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "void"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "selector": "method2",
// CHECK-NEXT:      "receiverKind": "class",
// CHECK-NEXT:      "classType": {
// CHECK-NEXT:       "qualType": "I"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "TestObjCBoxed",
// CHECK-NEXT:  "mangledName": "TestObjCBoxed",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (void)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CompoundStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ObjCBoxedExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "NSNumber *"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "selector": "numberWithInt:",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ParenExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "BinaryOperator",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "opcode": "+",
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
// CHECK-NEXT:            "value": "1"
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
// CHECK:       "name": "TestObjCSelector",
// CHECK-NEXT:  "mangledName": "TestObjCSelector",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (void)"
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
// CHECK-NEXT:        "loc": {
// CHECK-NEXT:         "offset": {{[0-9]+}},
// CHECK-NEXT:         "col": 7,
// CHECK-NEXT:         "tokLen": 1
// CHECK-NEXT:        },
// CHECK:             "name": "s",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "desugaredQualType": "SEL *",
// CHECK-NEXT:         "qualType": "SEL",
// CHECK-NEXT:         "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:        },
// CHECK-NEXT:        "init": "c",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ObjCSelectorExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "desugaredQualType": "SEL *",
// CHECK-NEXT:           "qualType": "SEL",
// CHECK-NEXT:           "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "selector": "dealloc"
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
// CHECK:       "name": "TestObjCProtocol",
// CHECK-NEXT:  "mangledName": "TestObjCProtocol",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (id)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isUsed": true,
// CHECK-NEXT:    "name": "Obj",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "id",
// CHECK-NEXT:     "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CompoundStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ObjCMessageExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "selector": "conformsToProtocol:",
// CHECK-NEXT:      "receiverKind": "instance",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ImplicitCastExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "id",
// CHECK-NEXT:         "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "castKind": "LValueToRValue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "DeclRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "id",
// CHECK-NEXT:           "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "referencedDecl": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "ParmVarDecl",
// CHECK-NEXT:           "name": "Obj",
// CHECK-NEXT:           "type": {
// CHECK-NEXT:            "qualType": "id",
// CHECK-NEXT:            "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:           }
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ObjCProtocolExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "Protocol *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "protocol": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ObjCProtocolDecl",
// CHECK-NEXT:         "name": "Proto"
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "TestObjCPropertyRef",
// CHECK-NEXT:  "mangledName": "TestObjCPropertyRef",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (J *)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isUsed": true,
// CHECK-NEXT:    "name": "Obj",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "J *"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CompoundStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "PseudoObjectExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "unsigned int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "BinaryOperator",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "opcode": "=",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ObjCPropertyRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "<pseudo-object type>"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "propertyKind": "explicit",
// CHECK-NEXT:          "property": {
// CHECK-NEXT:           "id": "0x{{.*}}",
// CHECK-NEXT:           "kind": "ObjCPropertyDecl",
// CHECK-NEXT:           "name": "prop"
// CHECK-NEXT:          },
// CHECK-NEXT:          "isMessagingSetter": true,
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "OpaqueValueExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "J *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "ImplicitCastExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "J *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "castKind": "LValueToRValue",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "DeclRefExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "J *"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "lvalue",
// CHECK-NEXT:                "referencedDecl": {
// CHECK-NEXT:                 "id": "0x{{.*}}",
// CHECK-NEXT:                 "kind": "ParmVarDecl",
// CHECK-NEXT:                 "name": "Obj",
// CHECK-NEXT:                 "type": {
// CHECK-NEXT:                  "qualType": "J *"
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
// CHECK-NEXT:          "kind": "OpaqueValueExpr",
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
// CHECK-NEXT:        "kind": "OpaqueValueExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "J *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "J *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "LValueToRValue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "J *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "ParmVarDecl",
// CHECK-NEXT:             "name": "Obj",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "J *"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "OpaqueValueExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "IntegerLiteral",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "value": "12"
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "OpaqueValueExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "unsigned int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "unsigned int"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "IntegralCast",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "OpaqueValueExpr",
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
// CHECK-NEXT:              "value": "12"
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ObjCMessageExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "void"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "selector": "setProp:",
// CHECK-NEXT:        "receiverKind": "instance",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "OpaqueValueExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "J *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "J *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "LValueToRValue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "DeclRefExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "J *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "lvalue",
// CHECK-NEXT:              "referencedDecl": {
// CHECK-NEXT:               "id": "0x{{.*}}",
// CHECK-NEXT:               "kind": "ParmVarDecl",
// CHECK-NEXT:               "name": "Obj",
// CHECK-NEXT:               "type": {
// CHECK-NEXT:                "qualType": "J *"
// CHECK-NEXT:               }
// CHECK-NEXT:              }
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "OpaqueValueExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "unsigned int"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "unsigned int"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "IntegralCast",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "OpaqueValueExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "int"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "IntegerLiteral",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "int"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "prvalue",
// CHECK-NEXT:                "value": "12"
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
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "DeclStmt",
// CHECK:           "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "VarDecl",
// CHECK:             "name": "i",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "init": "c",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "int"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "IntegralCast",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "PseudoObjectExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "unsigned int"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "ObjCPropertyRefExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "<pseudo-object type>"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "lvalue",
// CHECK-NEXT:              "propertyKind": "explicit",
// CHECK-NEXT:              "property": {
// CHECK-NEXT:               "id": "0x{{.*}}",
// CHECK-NEXT:               "kind": "ObjCPropertyDecl",
// CHECK-NEXT:               "name": "prop"
// CHECK-NEXT:              },
// CHECK-NEXT:              "isMessagingGetter": true,
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "OpaqueValueExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "J *"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "prvalue",
// CHECK-NEXT:                "inner": [
// CHECK-NEXT:                 {
// CHECK-NEXT:                  "id": "0x{{.*}}",
// CHECK-NEXT:                  "kind": "ImplicitCastExpr",
// CHECK:                       "type": {
// CHECK-NEXT:                   "qualType": "J *"
// CHECK-NEXT:                  },
// CHECK-NEXT:                  "valueCategory": "prvalue",
// CHECK-NEXT:                  "castKind": "LValueToRValue",
// CHECK-NEXT:                  "inner": [
// CHECK-NEXT:                   {
// CHECK-NEXT:                    "id": "0x{{.*}}",
// CHECK-NEXT:                    "kind": "DeclRefExpr",
// CHECK:                         "type": {
// CHECK-NEXT:                     "qualType": "J *"
// CHECK-NEXT:                    },
// CHECK-NEXT:                    "valueCategory": "lvalue",
// CHECK-NEXT:                    "referencedDecl": {
// CHECK-NEXT:                     "id": "0x{{.*}}",
// CHECK-NEXT:                     "kind": "ParmVarDecl",
// CHECK-NEXT:                     "name": "Obj",
// CHECK-NEXT:                     "type": {
// CHECK-NEXT:                      "qualType": "J *"
// CHECK-NEXT:                     }
// CHECK-NEXT:                    }
// CHECK-NEXT:                   }
// CHECK-NEXT:                  ]
// CHECK-NEXT:                 }
// CHECK-NEXT:                ]
// CHECK-NEXT:               }
// CHECK-NEXT:              ]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "OpaqueValueExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "J *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "ImplicitCastExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "J *"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "prvalue",
// CHECK-NEXT:                "castKind": "LValueToRValue",
// CHECK-NEXT:                "inner": [
// CHECK-NEXT:                 {
// CHECK-NEXT:                  "id": "0x{{.*}}",
// CHECK-NEXT:                  "kind": "DeclRefExpr",
// CHECK:                       "type": {
// CHECK-NEXT:                   "qualType": "J *"
// CHECK-NEXT:                  },
// CHECK-NEXT:                  "valueCategory": "lvalue",
// CHECK-NEXT:                  "referencedDecl": {
// CHECK-NEXT:                   "id": "0x{{.*}}",
// CHECK-NEXT:                   "kind": "ParmVarDecl",
// CHECK-NEXT:                   "name": "Obj",
// CHECK-NEXT:                   "type": {
// CHECK-NEXT:                    "qualType": "J *"
// CHECK-NEXT:                   }
// CHECK-NEXT:                  }
// CHECK-NEXT:                 }
// CHECK-NEXT:                ]
// CHECK-NEXT:               }
// CHECK-NEXT:              ]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "ObjCMessageExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "unsigned int"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "selector": "prop",
// CHECK-NEXT:              "receiverKind": "instance",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "OpaqueValueExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "J *"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "prvalue",
// CHECK-NEXT:                "inner": [
// CHECK-NEXT:                 {
// CHECK-NEXT:                  "id": "0x{{.*}}",
// CHECK-NEXT:                  "kind": "ImplicitCastExpr",
// CHECK:                       "type": {
// CHECK-NEXT:                   "qualType": "J *"
// CHECK-NEXT:                  },
// CHECK-NEXT:                  "valueCategory": "prvalue",
// CHECK-NEXT:                  "castKind": "LValueToRValue",
// CHECK-NEXT:                  "inner": [
// CHECK-NEXT:                   {
// CHECK-NEXT:                    "id": "0x{{.*}}",
// CHECK-NEXT:                    "kind": "DeclRefExpr",
// CHECK:                         "type": {
// CHECK-NEXT:                     "qualType": "J *"
// CHECK-NEXT:                    },
// CHECK-NEXT:                    "valueCategory": "lvalue",
// CHECK-NEXT:                    "referencedDecl": {
// CHECK-NEXT:                     "id": "0x{{.*}}",
// CHECK-NEXT:                     "kind": "ParmVarDecl",
// CHECK-NEXT:                     "name": "Obj",
// CHECK-NEXT:                     "type": {
// CHECK-NEXT:                      "qualType": "J *"
// CHECK-NEXT:                     }
// CHECK-NEXT:                    }
// CHECK-NEXT:                   }
// CHECK-NEXT:                  ]
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
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "TestObjCSubscriptRef",
// CHECK-NEXT:  "mangledName": "TestObjCSubscriptRef",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (NSMutableArray *, NSMutableDictionary *)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isUsed": true,
// CHECK-NEXT:    "name": "Array",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "NSMutableArray *"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isUsed": true,
// CHECK-NEXT:    "name": "Dict",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "NSMutableDictionary *"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CompoundStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "PseudoObjectExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "id",
// CHECK-NEXT:       "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "BinaryOperator",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "void *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "opcode": "=",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ObjCSubscriptRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "<pseudo-object type>"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "subscriptKind": "array",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "OpaqueValueExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "NSMutableArray *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "ImplicitCastExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "NSMutableArray *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "castKind": "LValueToRValue",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "DeclRefExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "NSMutableArray *"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "lvalue",
// CHECK-NEXT:                "referencedDecl": {
// CHECK-NEXT:                 "id": "0x{{.*}}",
// CHECK-NEXT:                 "kind": "ParmVarDecl",
// CHECK-NEXT:                 "name": "Array",
// CHECK-NEXT:                 "type": {
// CHECK-NEXT:                  "qualType": "NSMutableArray *"
// CHECK-NEXT:                 }
// CHECK-NEXT:                }
// CHECK-NEXT:               }
// CHECK-NEXT:              ]
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "OpaqueValueExpr",
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
// CHECK-NEXT:              "value": "0"
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "OpaqueValueExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "void *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CStyleCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "void *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "NullToPointer",
// CHECK-NEXT:            "inner": [
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
// CHECK-NEXT:        "kind": "OpaqueValueExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "NSMutableArray *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "NSMutableArray *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "LValueToRValue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "NSMutableArray *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "ParmVarDecl",
// CHECK-NEXT:             "name": "Array",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "NSMutableArray *"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "OpaqueValueExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
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
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "OpaqueValueExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "void *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CStyleCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "void *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "NullToPointer",
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
// CHECK-NEXT:        "kind": "OpaqueValueExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "id",
// CHECK-NEXT:         "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "id",
// CHECK-NEXT:           "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "NullToPointer",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "OpaqueValueExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "void *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "CStyleCastExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "void *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "castKind": "NullToPointer",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "IntegerLiteral",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "int"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "prvalue",
// CHECK-NEXT:                "value": "0"
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
// CHECK-NEXT:        "kind": "ObjCMessageExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "void"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "selector": "setObject:atIndexedSubscript:",
// CHECK-NEXT:        "receiverKind": "instance",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "OpaqueValueExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "NSMutableArray *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "NSMutableArray *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "LValueToRValue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "DeclRefExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "NSMutableArray *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "lvalue",
// CHECK-NEXT:              "referencedDecl": {
// CHECK-NEXT:               "id": "0x{{.*}}",
// CHECK-NEXT:               "kind": "ParmVarDecl",
// CHECK-NEXT:               "name": "Array",
// CHECK-NEXT:               "type": {
// CHECK-NEXT:                "qualType": "NSMutableArray *"
// CHECK-NEXT:               }
// CHECK-NEXT:              }
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "OpaqueValueExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "id",
// CHECK-NEXT:           "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "id",
// CHECK-NEXT:             "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "NullToPointer",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "OpaqueValueExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "void *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "CStyleCastExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "void *"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "prvalue",
// CHECK-NEXT:                "castKind": "NullToPointer",
// CHECK-NEXT:                "inner": [
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
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "OpaqueValueExpr",
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
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     },
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
// CHECK-NEXT:         "qualType": "id",
// CHECK-NEXT:         "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:        },
// CHECK-NEXT:        "init": "c",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "PseudoObjectExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "id",
// CHECK-NEXT:           "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ObjCSubscriptRefExpr",
/// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "<pseudo-object type>"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "subscriptKind": "array",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "OpaqueValueExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "NSMutableArray *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "ImplicitCastExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "NSMutableArray *"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "prvalue",
// CHECK-NEXT:                "castKind": "LValueToRValue",
// CHECK-NEXT:                "inner": [
// CHECK-NEXT:                 {
// CHECK-NEXT:                  "id": "0x{{.*}}",
// CHECK-NEXT:                  "kind": "DeclRefExpr",
// CHECK:                       "type": {
// CHECK-NEXT:                   "qualType": "NSMutableArray *"
// CHECK-NEXT:                  },
// CHECK-NEXT:                  "valueCategory": "lvalue",
// CHECK-NEXT:                  "referencedDecl": {
// CHECK-NEXT:                   "id": "0x{{.*}}",
// CHECK-NEXT:                   "kind": "ParmVarDecl",
// CHECK-NEXT:                   "name": "Array",
// CHECK-NEXT:                   "type": {
// CHECK-NEXT:                    "qualType": "NSMutableArray *"
// CHECK-NEXT:                   }
// CHECK-NEXT:                  }
// CHECK-NEXT:                 }
// CHECK-NEXT:                ]
// CHECK-NEXT:               }
// CHECK-NEXT:              ]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "OpaqueValueExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "int"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "IntegerLiteral",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "int"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "prvalue",
// CHECK-NEXT:                "value": "0"
// CHECK-NEXT:               }
// CHECK-NEXT:              ]
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "OpaqueValueExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "NSMutableArray *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "ImplicitCastExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "NSMutableArray *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "castKind": "LValueToRValue",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "DeclRefExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "NSMutableArray *"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "lvalue",
// CHECK-NEXT:                "referencedDecl": {
// CHECK-NEXT:                 "id": "0x{{.*}}",
// CHECK-NEXT:                 "kind": "ParmVarDecl",
// CHECK-NEXT:                 "name": "Array",
// CHECK-NEXT:                 "type": {
// CHECK-NEXT:                  "qualType": "NSMutableArray *"
// CHECK-NEXT:                 }
// CHECK-NEXT:                }
// CHECK-NEXT:               }
// CHECK-NEXT:              ]
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "OpaqueValueExpr",
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
// CHECK-NEXT:              "value": "0"
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ObjCMessageExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "id",
// CHECK-NEXT:             "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "selector": "objectAtIndexedSubscript:",
// CHECK-NEXT:            "receiverKind": "instance",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "OpaqueValueExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "NSMutableArray *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "ImplicitCastExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "NSMutableArray *"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "prvalue",
// CHECK-NEXT:                "castKind": "LValueToRValue",
// CHECK-NEXT:                "inner": [
// CHECK-NEXT:                 {
// CHECK-NEXT:                  "id": "0x{{.*}}",
// CHECK-NEXT:                  "kind": "DeclRefExpr",
// CHECK:                       "type": {
// CHECK-NEXT:                   "qualType": "NSMutableArray *"
// CHECK-NEXT:                  },
// CHECK-NEXT:                  "valueCategory": "lvalue",
// CHECK-NEXT:                  "referencedDecl": {
// CHECK-NEXT:                   "id": "0x{{.*}}",
// CHECK-NEXT:                   "kind": "ParmVarDecl",
// CHECK-NEXT:                   "name": "Array",
// CHECK-NEXT:                   "type": {
// CHECK-NEXT:                    "qualType": "NSMutableArray *"
// CHECK-NEXT:                   }
// CHECK-NEXT:                  }
// CHECK-NEXT:                 }
// CHECK-NEXT:                ]
// CHECK-NEXT:               }
// CHECK-NEXT:              ]
// CHECK-NEXT:             },
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "OpaqueValueExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "int"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "IntegerLiteral",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "int"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "prvalue",
// CHECK-NEXT:                "value": "0"
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
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "PseudoObjectExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "id",
// CHECK-NEXT:       "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "BinaryOperator",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "void *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "opcode": "=",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ObjCSubscriptRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "<pseudo-object type>"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "subscriptKind": "dictionary",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "OpaqueValueExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "NSMutableDictionary *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "ImplicitCastExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "NSMutableDictionary *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "castKind": "LValueToRValue",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "DeclRefExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "NSMutableDictionary *"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "lvalue",
// CHECK-NEXT:                "referencedDecl": {
// CHECK-NEXT:                 "id": "0x{{.*}}",
// CHECK-NEXT:                 "kind": "ParmVarDecl",
// CHECK-NEXT:                 "name": "Dict",
// CHECK-NEXT:                 "type": {
// CHECK-NEXT:                  "qualType": "NSMutableDictionary *"
// CHECK-NEXT:                 }
// CHECK-NEXT:                }
// CHECK-NEXT:               }
// CHECK-NEXT:              ]
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "OpaqueValueExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "NSString *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "ObjCStringLiteral",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "NSString *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "StringLiteral",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "char[4]"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "lvalue",
// CHECK-NEXT:                "value": "\"key\""
// CHECK-NEXT:               }
// CHECK-NEXT:              ]
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "OpaqueValueExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "void *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "CStyleCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "void *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "NullToPointer",
// CHECK-NEXT:            "inner": [
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
// CHECK-NEXT:        "kind": "OpaqueValueExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "NSMutableDictionary *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "NSMutableDictionary *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "LValueToRValue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "NSMutableDictionary *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "ParmVarDecl",
// CHECK-NEXT:             "name": "Dict",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "NSMutableDictionary *"
// CHECK-NEXT:             }
// CHECK-NEXT:            }
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "OpaqueValueExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "NSString *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ObjCStringLiteral",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "NSString *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "StringLiteral",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "char[4]"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "value": "\"key\""
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "OpaqueValueExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "void *"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "CStyleCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "void *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "NullToPointer",
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
// CHECK-NEXT:        "kind": "OpaqueValueExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "id",
// CHECK-NEXT:         "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "id",
// CHECK-NEXT:           "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "NullToPointer",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "OpaqueValueExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "void *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "CStyleCastExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "void *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "castKind": "NullToPointer",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "IntegerLiteral",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "int"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "prvalue",
// CHECK-NEXT:                "value": "0"
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
// CHECK-NEXT:        "kind": "ObjCMessageExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "void"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "selector": "setObject:forKeyedSubscript:",
// CHECK-NEXT:        "receiverKind": "instance",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "OpaqueValueExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "NSMutableDictionary *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "NSMutableDictionary *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "LValueToRValue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "DeclRefExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "NSMutableDictionary *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "lvalue",
// CHECK-NEXT:              "referencedDecl": {
// CHECK-NEXT:               "id": "0x{{.*}}",
// CHECK-NEXT:               "kind": "ParmVarDecl",
// CHECK-NEXT:               "name": "Dict",
// CHECK-NEXT:               "type": {
// CHECK-NEXT:                "qualType": "NSMutableDictionary *"
// CHECK-NEXT:               }
// CHECK-NEXT:              }
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "OpaqueValueExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "id",
// CHECK-NEXT:           "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "id",
// CHECK-NEXT:             "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "NullToPointer",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "OpaqueValueExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "void *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "CStyleCastExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "void *"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "prvalue",
// CHECK-NEXT:                "castKind": "NullToPointer",
// CHECK-NEXT:                "inner": [
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
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "id",
// CHECK-NEXT:           "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "BitCast",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "OpaqueValueExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "NSString *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "ObjCStringLiteral",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "NSString *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "StringLiteral",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "char[4]"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "lvalue",
// CHECK-NEXT:                "value": "\"key\""
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
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "BinaryOperator",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "id",
// CHECK-NEXT:       "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "opcode": "=",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "id",
// CHECK-NEXT:         "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "VarDecl",
// CHECK-NEXT:         "name": "i",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "id",
// CHECK-NEXT:          "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       },
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "PseudoObjectExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "id",
// CHECK-NEXT:         "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "prvalue",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ObjCSubscriptRefExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "<pseudo-object type>"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "lvalue",
// CHECK-NEXT:          "subscriptKind": "dictionary",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "OpaqueValueExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "NSMutableDictionary *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "ImplicitCastExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "NSMutableDictionary *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "castKind": "LValueToRValue",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "DeclRefExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "NSMutableDictionary *"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "lvalue",
// CHECK-NEXT:                "referencedDecl": {
// CHECK-NEXT:                 "id": "0x{{.*}}",
// CHECK-NEXT:                 "kind": "ParmVarDecl",
// CHECK-NEXT:                 "name": "Dict",
// CHECK-NEXT:                 "type": {
// CHECK-NEXT:                  "qualType": "NSMutableDictionary *"
// CHECK-NEXT:                 }
// CHECK-NEXT:                }
// CHECK-NEXT:               }
// CHECK-NEXT:              ]
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "OpaqueValueExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "NSString *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "ObjCStringLiteral",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "NSString *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "StringLiteral",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "char[4]"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "lvalue",
// CHECK-NEXT:                "value": "\"key\""
// CHECK-NEXT:               }
// CHECK-NEXT:              ]
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "OpaqueValueExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "NSMutableDictionary *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "NSMutableDictionary *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "LValueToRValue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "DeclRefExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "NSMutableDictionary *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "lvalue",
// CHECK-NEXT:              "referencedDecl": {
// CHECK-NEXT:               "id": "0x{{.*}}",
// CHECK-NEXT:               "kind": "ParmVarDecl",
// CHECK-NEXT:               "name": "Dict",
// CHECK-NEXT:               "type": {
// CHECK-NEXT:                "qualType": "NSMutableDictionary *"
// CHECK-NEXT:               }
// CHECK-NEXT:              }
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "OpaqueValueExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "NSString *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ObjCStringLiteral",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "NSString *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "StringLiteral",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "char[4]"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "lvalue",
// CHECK-NEXT:              "value": "\"key\""
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ObjCMessageExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "id",
// CHECK-NEXT:           "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "selector": "objectForKeyedSubscript:",
// CHECK-NEXT:          "receiverKind": "instance",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "OpaqueValueExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "NSMutableDictionary *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "ImplicitCastExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "NSMutableDictionary *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "castKind": "LValueToRValue",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "DeclRefExpr",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "NSMutableDictionary *"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "lvalue",
// CHECK-NEXT:                "referencedDecl": {
// CHECK-NEXT:                 "id": "0x{{.*}}",
// CHECK-NEXT:                 "kind": "ParmVarDecl",
// CHECK-NEXT:                 "name": "Dict",
// CHECK-NEXT:                 "type": {
// CHECK-NEXT:                  "qualType": "NSMutableDictionary *"
// CHECK-NEXT:                 }
// CHECK-NEXT:                }
// CHECK-NEXT:               }
// CHECK-NEXT:              ]
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           },
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "ImplicitCastExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "id",
// CHECK-NEXT:             "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "prvalue",
// CHECK-NEXT:            "castKind": "BitCast",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "OpaqueValueExpr",
// CHECK:                   "type": {
// CHECK-NEXT:               "qualType": "NSString *"
// CHECK-NEXT:              },
// CHECK-NEXT:              "valueCategory": "prvalue",
// CHECK-NEXT:              "inner": [
// CHECK-NEXT:               {
// CHECK-NEXT:                "id": "0x{{.*}}",
// CHECK-NEXT:                "kind": "ObjCStringLiteral",
// CHECK:                     "type": {
// CHECK-NEXT:                 "qualType": "NSString *"
// CHECK-NEXT:                },
// CHECK-NEXT:                "valueCategory": "prvalue",
// CHECK-NEXT:                "inner": [
// CHECK-NEXT:                 {
// CHECK-NEXT:                  "id": "0x{{.*}}",
// CHECK-NEXT:                  "kind": "StringLiteral",
// CHECK:                       "type": {
// CHECK-NEXT:                   "qualType": "char[4]"
// CHECK-NEXT:                  },
// CHECK-NEXT:                  "valueCategory": "lvalue",
// CHECK-NEXT:                  "value": "\"key\""
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
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:      "kind": "FunctionDecl",
// CHECK:       "name": "TestObjCIVarRef",
// CHECK-NEXT:  "mangledName": "TestObjCIVarRef",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (I *)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ParmVarDecl",
// CHECK:         "isUsed": true,
// CHECK-NEXT:    "name": "Ptr",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "I *"
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
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "opcode": "=",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "ObjCIvarRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "decl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ObjCIvarDecl",
// CHECK-NEXT:         "name": "public",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "int"
// CHECK-NEXT:         }
// CHECK-NEXT:        },
// CHECK-NEXT:        "isArrow": true,
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ImplicitCastExpr",
// CHECK:               "type": {
// CHECK-NEXT:           "qualType": "I *"
// CHECK-NEXT:          },
// CHECK-NEXT:          "valueCategory": "prvalue",
// CHECK-NEXT:          "castKind": "LValueToRValue",
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "DeclRefExpr",
// CHECK:                 "type": {
// CHECK-NEXT:             "qualType": "I *"
// CHECK-NEXT:            },
// CHECK-NEXT:            "valueCategory": "lvalue",
// CHECK-NEXT:            "referencedDecl": {
// CHECK-NEXT:             "id": "0x{{.*}}",
// CHECK-NEXT:             "kind": "ParmVarDecl",
// CHECK-NEXT:             "name": "Ptr",
// CHECK-NEXT:             "type": {
// CHECK-NEXT:              "qualType": "I *"
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
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:       "kind": "FunctionDecl",
// CHECK:       "name": "TestObjCBoolLiteral",
// CHECK-NEXT:  "mangledName": "TestObjCBoolLiteral",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (void)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CompoundStmt",
// CHECK:         "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ObjCBoolLiteralExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "desugaredQualType": "signed char",
// CHECK-NEXT:       "qualType": "BOOL",
// CHECK-NEXT:       "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "value": "__objc_yes"
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ObjCBoolLiteralExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "desugaredQualType": "signed char",
// CHECK-NEXT:       "qualType": "BOOL",
// CHECK-NEXT:       "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "value": "__objc_no"
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }
