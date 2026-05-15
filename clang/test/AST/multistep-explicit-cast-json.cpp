// RUN: %clang_cc1 -triple x86_64-linux-gnu -fsyntax-only -ast-dump=json %s | FileCheck %s

unsigned char implicitcast_0(unsigned int x) {
  return x;
}

signed char implicitcast_1(unsigned int x) {
  return x;
}

unsigned char implicitcast_2(signed int x) {
  return x;
}

signed char implicitcast_3(signed int x) {
  return x;
}

//----------------------------------------------------------------------------//

unsigned char cstylecast_0(unsigned int x) {
  return (unsigned char)x;
}

signed char cstylecast_1(unsigned int x) {
  return (signed char)x;
}

unsigned char cstylecast_2(signed int x) {
  return (unsigned char)x;
}

signed char cstylecast_3(signed int x) {
  return (signed char)x;
}

//----------------------------------------------------------------------------//

unsigned char cxxstaticcast_0(unsigned int x) {
  return static_cast<unsigned char>(x);
}

signed char cxxstaticcast_1(unsigned int x) {
  return static_cast<signed char>(x);
}

unsigned char cxxstaticcast_2(signed int x) {
  return static_cast<unsigned char>(x);
}

signed char cxxstaticcast_3(signed int x) {
  return static_cast<signed char>(x);
}

//----------------------------------------------------------------------------//

using UnsignedChar = unsigned char;
using SignedChar = signed char;
using UnsignedInt = unsigned int;
using SignedInt = signed int;

UnsignedChar cxxfunctionalcast_0(UnsignedInt x) {
  return UnsignedChar(x);
}

SignedChar cxxfunctionalcast_1(UnsignedInt x) {
  return SignedChar(x);
}

UnsignedChar cxxfunctionalcast_2(SignedInt x) {
  return UnsignedChar(x);
}

SignedChar cxxfunctionalcast_3(SignedInt x) {
  return SignedChar(x);
}

// CHECK-LABEL: "name": "implicitcast_0",
// CHECK:       "kind": "ImplicitCastExpr",
// CHECK:       "type": {
// CHECK-NEXT:   "qualType": "unsigned char"
// CHECK-NEXT:  },
// CHECK-NEXT:  "valueCategory": "prvalue",
// CHECK-NEXT:  "castKind": "IntegralCast",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ImplicitCastExpr",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "unsigned int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "castKind": "LValueToRValue",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "DeclRefExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "unsigned int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "lvalue",
// CHECK-NEXT:      "referencedDecl": {
// CHECK-NEXT:       "id": "0x{{.*}}",
// CHECK-NEXT:       "kind": "ParmVarDecl",
// CHECK-NEXT:       "name": "x",
// CHECK-NEXT:       "type": {
// CHECK-NEXT:        "qualType": "unsigned int"
// CHECK-NEXT:       }
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }

// CHECK-LABEL: "name": "implicitcast_1",
// CHECK:       "kind": "ImplicitCastExpr",
// CHECK:       "type": {
// CHECK-NEXT:   "qualType": "signed char"
// CHECK-NEXT:  },
// CHECK-NEXT:  "valueCategory": "prvalue",
// CHECK-NEXT:  "castKind": "IntegralCast",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ImplicitCastExpr",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "unsigned int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "castKind": "LValueToRValue",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "DeclRefExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "unsigned int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "lvalue",
// CHECK-NEXT:      "referencedDecl": {
// CHECK-NEXT:       "id": "0x{{.*}}",
// CHECK-NEXT:       "kind": "ParmVarDecl",
// CHECK-NEXT:       "name": "x",
// CHECK-NEXT:       "type": {
// CHECK-NEXT:        "qualType": "unsigned int"
// CHECK-NEXT:       }
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }

// CHECK-LABEL: "name": "implicitcast_2",
// CHECK:       "kind": "ImplicitCastExpr",
// CHECK:       "type": {
// CHECK-NEXT:   "qualType": "unsigned char"
// CHECK-NEXT:  },
// CHECK-NEXT:  "valueCategory": "prvalue",
// CHECK-NEXT:  "castKind": "IntegralCast",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ImplicitCastExpr",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "castKind": "LValueToRValue",
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
// CHECK-NEXT:       "kind": "ParmVarDecl",
// CHECK-NEXT:       "name": "x",
// CHECK-NEXT:       "type": {
// CHECK-NEXT:        "qualType": "int"
// CHECK-NEXT:       }
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }

// CHECK-LABEL: "name": "implicitcast_3",
// CHECK:       "kind": "ImplicitCastExpr",
// CHECK:       "type": {
// CHECK-NEXT:   "qualType": "signed char"
// CHECK-NEXT:  },
// CHECK-NEXT:  "valueCategory": "prvalue",
// CHECK-NEXT:  "castKind": "IntegralCast",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ImplicitCastExpr",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "castKind": "LValueToRValue",
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
// CHECK-NEXT:       "kind": "ParmVarDecl",
// CHECK-NEXT:       "name": "x",
// CHECK-NEXT:       "type": {
// CHECK-NEXT:        "qualType": "int"
// CHECK-NEXT:       }
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }

// CHECK-LABEL: "name": "cstylecast_0",
// CHECK:       "kind": "CStyleCastExpr",
// CHECK:       "type": {
// CHECK-NEXT:   "qualType": "unsigned char"
// CHECK-NEXT:  },
// CHECK-NEXT:  "valueCategory": "prvalue",
// CHECK-NEXT:  "castKind": "NoOp",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ImplicitCastExpr",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "unsigned char"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "castKind": "IntegralCast",
// CHECK-NEXT:    "isPartOfExplicitCast": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "unsigned int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "isPartOfExplicitCast": true,
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "unsigned int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "x",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "unsigned int"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }

// CHECK-LABEL: "name": "cstylecast_1",
// CHECK:       "kind": "CStyleCastExpr",
// CHECK:       "type": {
// CHECK-NEXT:   "qualType": "signed char"
// CHECK-NEXT:  },
// CHECK-NEXT:  "valueCategory": "prvalue",
// CHECK-NEXT:  "castKind": "NoOp",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ImplicitCastExpr",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "signed char"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "castKind": "IntegralCast",
// CHECK-NEXT:    "isPartOfExplicitCast": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "unsigned int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "isPartOfExplicitCast": true,
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "unsigned int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "x",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "unsigned int"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }

// CHECK-LABEL: "name": "cstylecast_2",
// CHECK:       "kind": "CStyleCastExpr",
// CHECK:       "type": {
// CHECK-NEXT:   "qualType": "unsigned char"
// CHECK-NEXT:  },
// CHECK-NEXT:  "valueCategory": "prvalue",
// CHECK-NEXT:  "castKind": "NoOp",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ImplicitCastExpr",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "unsigned char"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "castKind": "IntegralCast",
// CHECK-NEXT:    "isPartOfExplicitCast": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "isPartOfExplicitCast": true,
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
// CHECK-NEXT:         "name": "x",
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

// CHECK-LABEL: "name": "cstylecast_3",
// CHECK:       "kind": "CStyleCastExpr",
// CHECK:       "type": {
// CHECK-NEXT:   "qualType": "signed char"
// CHECK-NEXT:  },
// CHECK-NEXT:  "valueCategory": "prvalue",
// CHECK-NEXT:  "castKind": "NoOp",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ImplicitCastExpr",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "signed char"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "castKind": "IntegralCast",
// CHECK-NEXT:    "isPartOfExplicitCast": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "isPartOfExplicitCast": true,
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
// CHECK-NEXT:         "name": "x",
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

// CHECK-LABEL: "name": "cxxstaticcast_0",
// CHECK:       "kind": "CXXStaticCastExpr",
// CHECK:       "type": {
// CHECK-NEXT:   "qualType": "unsigned char"
// CHECK-NEXT:  },
// CHECK-NEXT:  "valueCategory": "prvalue",
// CHECK-NEXT:  "castKind": "NoOp",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ImplicitCastExpr",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "unsigned char"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "castKind": "IntegralCast",
// CHECK-NEXT:    "isPartOfExplicitCast": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "unsigned int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "isPartOfExplicitCast": true,
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "unsigned int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "x",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "unsigned int"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }

// CHECK-LABEL: "name": "cxxstaticcast_1",
// CHECK:       "kind": "CXXStaticCastExpr",
// CHECK:       "type": {
// CHECK-NEXT:   "qualType": "signed char"
// CHECK-NEXT:  },
// CHECK-NEXT:  "valueCategory": "prvalue",
// CHECK-NEXT:  "castKind": "NoOp",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ImplicitCastExpr",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "signed char"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "castKind": "IntegralCast",
// CHECK-NEXT:    "isPartOfExplicitCast": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "unsigned int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "isPartOfExplicitCast": true,
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "qualType": "unsigned int"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "x",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "qualType": "unsigned int"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }

// CHECK-LABEL: "name": "cxxstaticcast_2",
// CHECK:       "kind": "CXXStaticCastExpr",
// CHECK:       "type": {
// CHECK-NEXT:   "qualType": "unsigned char"
// CHECK-NEXT:  },
// CHECK-NEXT:  "valueCategory": "prvalue",
// CHECK-NEXT:  "castKind": "NoOp",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ImplicitCastExpr",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "unsigned char"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "castKind": "IntegralCast",
// CHECK-NEXT:    "isPartOfExplicitCast": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "isPartOfExplicitCast": true,
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
// CHECK-NEXT:         "name": "x",
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

// CHECK-LABEL: "name": "cxxstaticcast_3",
// CHECK:       "kind": "CXXStaticCastExpr",
// CHECK:       "type": {
// CHECK-NEXT:   "qualType": "signed char"
// CHECK-NEXT:  },
// CHECK-NEXT:  "valueCategory": "prvalue",
// CHECK-NEXT:  "castKind": "NoOp",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ImplicitCastExpr",
// CHECK:         "type": {
// CHECK-NEXT:     "qualType": "signed char"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "castKind": "IntegralCast",
// CHECK-NEXT:    "isPartOfExplicitCast": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "isPartOfExplicitCast": true,
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
// CHECK-NEXT:         "name": "x",
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

// CHECK-LABEL: "name": "cxxfunctionalcast_0",
// CHECK:       "kind": "CXXFunctionalCastExpr",
// CHECK:       "type": {
// CHECK-NEXT:   "desugaredQualType": "unsigned char",
// CHECK-NEXT:   "qualType": "UnsignedChar",
// CHECK-NEXT:   "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:  },
// CHECK-NEXT:  "valueCategory": "prvalue",
// CHECK-NEXT:  "castKind": "NoOp",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ImplicitCastExpr",
// CHECK:         "type": {
// CHECK-NEXT:     "desugaredQualType": "unsigned char",
// CHECK-NEXT:     "qualType": "UnsignedChar",
// CHECK-NEXT:     "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "castKind": "IntegralCast",
// CHECK-NEXT:    "isPartOfExplicitCast": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "desugaredQualType": "unsigned int",
// CHECK-NEXT:       "qualType": "UnsignedInt",
// CHECK-NEXT:       "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "isPartOfExplicitCast": true,
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "desugaredQualType": "unsigned int",
// CHECK-NEXT:         "qualType": "UnsignedInt",
// CHECK-NEXT:         "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "x",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "desugaredQualType": "unsigned int",
// CHECK-NEXT:          "qualType": "UnsignedInt",
// CHECK-NEXT:          "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }

// CHECK-LABEL: "name": "cxxfunctionalcast_1",
// CHECK:       "kind": "CXXFunctionalCastExpr",
// CHECK:       "type": {
// CHECK-NEXT:   "desugaredQualType": "signed char",
// CHECK-NEXT:   "qualType": "SignedChar",
// CHECK-NEXT:   "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:  },
// CHECK-NEXT:  "valueCategory": "prvalue",
// CHECK-NEXT:  "castKind": "NoOp",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ImplicitCastExpr",
// CHECK:         "type": {
// CHECK-NEXT:     "desugaredQualType": "signed char",
// CHECK-NEXT:     "qualType": "SignedChar",
// CHECK-NEXT:     "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "castKind": "IntegralCast",
// CHECK-NEXT:    "isPartOfExplicitCast": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "desugaredQualType": "unsigned int",
// CHECK-NEXT:       "qualType": "UnsignedInt",
// CHECK-NEXT:       "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "isPartOfExplicitCast": true,
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "desugaredQualType": "unsigned int",
// CHECK-NEXT:         "qualType": "UnsignedInt",
// CHECK-NEXT:         "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "x",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "desugaredQualType": "unsigned int",
// CHECK-NEXT:          "qualType": "UnsignedInt",
// CHECK-NEXT:          "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }

// CHECK-LABEL: "name": "cxxfunctionalcast_2",
// CHECK:       "kind": "CXXFunctionalCastExpr",
// CHECK:       "type": {
// CHECK-NEXT:   "desugaredQualType": "unsigned char",
// CHECK-NEXT:   "qualType": "UnsignedChar",
// CHECK-NEXT:   "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:  },
// CHECK-NEXT:  "valueCategory": "prvalue",
// CHECK-NEXT:  "castKind": "NoOp",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ImplicitCastExpr",
// CHECK:         "type": {
// CHECK-NEXT:     "desugaredQualType": "unsigned char",
// CHECK-NEXT:     "qualType": "UnsignedChar",
// CHECK-NEXT:     "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "castKind": "IntegralCast",
// CHECK-NEXT:    "isPartOfExplicitCast": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "desugaredQualType": "int",
// CHECK-NEXT:       "qualType": "SignedInt",
// CHECK-NEXT:       "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "isPartOfExplicitCast": true,
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "desugaredQualType": "int",
// CHECK-NEXT:         "qualType": "SignedInt",
// CHECK-NEXT:         "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "x",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "desugaredQualType": "int",
// CHECK-NEXT:          "qualType": "SignedInt",
// CHECK-NEXT:          "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }

// CHECK-LABEL: "name": "cxxfunctionalcast_3",
// CHECK:       "kind": "CXXFunctionalCastExpr",
// CHECK:       "type": {
// CHECK-NEXT:   "desugaredQualType": "signed char",
// CHECK-NEXT:   "qualType": "SignedChar",
// CHECK-NEXT:   "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:  },
// CHECK-NEXT:  "valueCategory": "prvalue",
// CHECK-NEXT:  "castKind": "NoOp",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ImplicitCastExpr",
// CHECK:         "type": {
// CHECK-NEXT:     "desugaredQualType": "signed char",
// CHECK-NEXT:     "qualType": "SignedChar",
// CHECK-NEXT:     "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:    },
// CHECK-NEXT:    "valueCategory": "prvalue",
// CHECK-NEXT:    "castKind": "IntegralCast",
// CHECK-NEXT:    "isPartOfExplicitCast": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ImplicitCastExpr",
// CHECK:           "type": {
// CHECK-NEXT:       "desugaredQualType": "int",
// CHECK-NEXT:       "qualType": "SignedInt",
// CHECK-NEXT:       "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:      },
// CHECK-NEXT:      "valueCategory": "prvalue",
// CHECK-NEXT:      "castKind": "LValueToRValue",
// CHECK-NEXT:      "isPartOfExplicitCast": true,
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "DeclRefExpr",
// CHECK:             "type": {
// CHECK-NEXT:         "desugaredQualType": "int",
// CHECK-NEXT:         "qualType": "SignedInt",
// CHECK-NEXT:         "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:        },
// CHECK-NEXT:        "valueCategory": "lvalue",
// CHECK-NEXT:        "referencedDecl": {
// CHECK-NEXT:         "id": "0x{{.*}}",
// CHECK-NEXT:         "kind": "ParmVarDecl",
// CHECK-NEXT:         "name": "x",
// CHECK-NEXT:         "type": {
// CHECK-NEXT:          "desugaredQualType": "int",
// CHECK-NEXT:          "qualType": "SignedInt",
// CHECK-NEXT:          "typeAliasDeclId": "0x{{.*}}"
// CHECK-NEXT:         }
// CHECK-NEXT:        }
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }
