// RUN: %clang_cc1 -triple x86_64-pc-linux -Wdocumentation -ast-dump=json %s | FileCheck %s

int global_decl;
extern __attribute__((alias("global_decl"))) int global_alias;

void cleanup_function(int*);
void some() {
  __attribute__((cleanup(cleanup_function))) int var;
}

__attribute__((deprecated)) int deprecated_var0;
__attribute__((deprecated("reason"))) int deprecated_var1;
__attribute__((deprecated("reason", "replacement"))) int deprecated_var2;

__attribute__((unavailable)) int unavailable_var0;
__attribute__((unavailable("reason"))) int unavailable_var1;

__attribute__ ((section ("SECTION_NAME"))) int section_var;

__attribute__ ((visibility ("hidden"))) int visibility_var;

__thread __attribute__ ((tls_model ("local-exec"))) int tls_model_var;

__attribute__((availability(macos, introduced=10.15, deprecated=12.0, obsoleted=13.0, strict, replacement="new_func"))) int availability_var_all;
__attribute__((availability(macos, unavailable, message="use new API"))) int availability_var_unavailable;

#pragma clang attribute push(__attribute__((availability(macos, introduced=11.0))), apply_to=variable)
int availability_var_pragma;
#pragma clang attribute pop

// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "VarDecl",
// CHECK:       "isUsed": true,
// CHECK-LABEL: "name": "global_decl",
// CHECK-NEXT:  "mangledName": "global_decl",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  }
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "VarDecl",
// CHECK-LABEL: "name": "global_alias",
// CHECK-NEXT:  "mangledName": "global_alias",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "storageClass": "extern",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "AliasAttr",
// CHECK:         "aliasee": "global_decl"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "VarDecl",
// CHECK-LABEL: "name": "var",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "CleanupAttr",
// CHECK:         "cleanup_function": {
// CHECK-NEXT:     "id": "0x{{.*}}",
// CHECK-NEXT:     "kind": "FunctionDecl",
// CHECK:          "name": "cleanup_function",
// CHECK-NEXT:     "type": {
// CHECK-NEXT:      "qualType": "void (int *)"
// CHECK-NEXT:     }
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "VarDecl",
// CHECK:       "name": "deprecated_var0",
// CHECK-NEXT:  "mangledName": "deprecated_var0",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "DeprecatedAttr",


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "VarDecl",
// CHECK-LABEL: "name": "deprecated_var1",
// CHECK-NEXT:  "mangledName": "deprecated_var1",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "DeprecatedAttr",
// CHECK:         "message": "reason"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "VarDecl",
// CHECK-LABEL: "name": "deprecated_var2",
// CHECK-NEXT:  "mangledName": "deprecated_var2",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "DeprecatedAttr",
// CHECK:         "message": "reason",
// CHECK-NEXT:    "replacement": "replacement"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "VarDecl",
// CHECK-LABEL: "name": "unavailable_var0",
// CHECK-NEXT:  "mangledName": "unavailable_var0",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "UnavailableAttr",


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "VarDecl",
// CHECK-LABEL: "name": "unavailable_var1",
// CHECK-NEXT:  "mangledName": "unavailable_var1",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "UnavailableAttr",
// CHECK:         "message": "reason"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "VarDecl",
// CHECK-LABEL: "name": "section_var",
// CHECK-NEXT:  "mangledName": "section_var",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "SectionAttr",
// CHECK:         "section_name": "SECTION_NAME"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "VarDecl",
// CHECK-LABEL: "name": "visibility_var",
// CHECK-NEXT:  "mangledName": "visibility_var",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "VisibilityAttr",
// CHECK:         "visibility": "hidden"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "VarDecl",
// CHECK-LABEL: "name": "tls_model_var",
// CHECK-NEXT:  "mangledName": "tls_model_var",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "tls": "static",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "TLSModelAttr",
// CHECK:         "tls_model": "local-exec"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "VarDecl",
// CHECK-LABEL: "name": "availability_var_all",
// CHECK-NEXT:  "mangledName": "availability_var_all",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "AvailabilityAttr",
// CHECK:         "platform": "macos",
// CHECK-NEXT:    "introduced": "10.15",
// CHECK-NEXT:    "deprecated": "12.0",
// CHECK-NEXT:    "obsoleted": "13.0",
// CHECK-NEXT:    "strict": true,
// CHECK-NEXT:    "replacement": "new_func"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "VarDecl",
// CHECK-LABEL: "name": "availability_var_unavailable",
// CHECK-NEXT:  "mangledName": "availability_var_unavailable",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "AvailabilityAttr",
// CHECK:         "platform": "macos",
// CHECK-NEXT:    "unavailable": true,
// CHECK-NEXT:    "message": "use new API"
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK-NOT: {{^}}Dumping
// CHECK:       "kind": "VarDecl",
// CHECK-LABEL: "name": "availability_var_pragma",
// CHECK-NEXT:  "mangledName": "availability_var_pragma",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "AvailabilityAttr",
// CHECK:         "platform": "macos",
// CHECK-NEXT:    "introduced": "11.0",
// CHECK-NEXT:    "priority": 1
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }
