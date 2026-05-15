// RUN: %clang_cc1 -triple x86_64-pc-linux -std=c++17 -ast-dump=json %s | FileCheck %s

enum {
  One,
  Two
};

enum E {
  Three,
  Four
};

enum F : short {
  Five,
  Six
};

enum struct G {
  Seven,
  Eight
};

enum class H {
  Nine,
  Ten
};

enum class I : int {
  Eleven,
  Twelve
};


// CHECK:       "kind": "EnumDecl",
// CHECK:       "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "EnumConstantDecl",
// CHECK:         "name": "One",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "(unnamed enum at {{.*}}:3:1)"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "EnumConstantDecl",
// CHECK:         "name": "Two",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "(unnamed enum at {{.*}}:3:1)"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }

// CHECK:       "kind": "EnumDecl",
// CHECK:       "name": "E",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "EnumConstantDecl",
// CHECK:         "name": "Three",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "E"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "EnumConstantDecl",
// CHECK:         "name": "Four",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "E"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }

// CHECK:       "kind": "EnumDecl",
// CHECK:       "name": "F",
// CHECK-NEXT:  "fixedUnderlyingType": {
// CHECK-NEXT:   "qualType": "short"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "EnumConstantDecl",
// CHECK:         "name": "Five",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "F"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "EnumConstantDecl",
// CHECK:         "name": "Six",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "F"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }

// CHECK:       "kind": "EnumDecl",
// CHECK:       "name": "G",
// CHECK-NEXT:  "fixedUnderlyingType": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "scopedEnumTag": "struct",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "EnumConstantDecl",
// CHECK:         "name": "Seven",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "G"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "EnumConstantDecl",
// CHECK:         "name": "Eight",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "G"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }

// CHECK:       "kind": "EnumDecl",
// CHECK:       "name": "H",
// CHECK-NEXT:  "fixedUnderlyingType": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "scopedEnumTag": "class",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "EnumConstantDecl",
// CHECK:         "name": "Nine",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "H"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "EnumConstantDecl",
// CHECK:         "name": "Ten",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "H"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }

// CHECK:       "kind": "EnumDecl",
// CHECK:       "name": "I",
// CHECK-NEXT:  "fixedUnderlyingType": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "scopedEnumTag": "class",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "EnumConstantDecl",
// CHECK:         "name": "Eleven",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "I"
// CHECK-NEXT:    }
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "EnumConstantDecl",
// CHECK:         "name": "Twelve",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "I"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }
