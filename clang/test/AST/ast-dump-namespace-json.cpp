// RUN: %clang_cc1 -triple x86_64-pc-linux -std=c++2a -ast-dump=json %s | FileCheck %s

namespace foo {
}

namespace {
}

namespace bar {
inline namespace __1 {
}
}

namespace baz::quux {
}

namespace quux::inline frobble {
}


// CHECK:       "kind": "NamespaceDecl",
// CHECK:       "name": "foo"
// CHECK-NEXT: }

// CHECK:       "kind": "NamespaceDecl",

// CHECK:       "kind": "NamespaceDecl",
// CHECK-NEXT:  "name": ""
// CHECK-NEXT: }

// CHECK:       "kind": "NamespaceDecl",
// CHECK:       "name": "bar",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "NamespaceDecl",
// CHECK:         "name": "__1",
// CHECK-NEXT:    "isInline": true
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }

// CHECK:       "kind": "NamespaceDecl",
// CHECK:       "name": "baz",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "NamespaceDecl",
// CHECK:         "name": "quux"
// CHECK-NEXT:    "isNested": true
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }

// CHECK:       "kind": "NamespaceDecl",
// CHECK:       "name": "quux"
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "NamespaceDecl",
// CHECK:         "name": "frobble",
// CHECK-NEXT:    "isInline": true,
// CHECK-NEXT:    "isNested": true
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }
