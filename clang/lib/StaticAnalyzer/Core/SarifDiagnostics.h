//==- SarifDiagnostics.h - SARIF Diagnostics for Paths -------------*- C++ -*-//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_STATICANALYZER_CORE_SARIFDIAGNOSTICS_H
#define LLVM_CLANG_LIB_STATICANALYZER_CORE_SARIFDIAGNOSTICS_H

#include <string>

#include "clang/StaticAnalyzer/Core/PathDiagnosticConsumers.h"

namespace clang {
class Preprocessor;
namespace ento {
struct PathDiagnosticConsumerOptions;
}  // namespace ento
}  // namespace clang

namespace clang::ento {

void createSarifDiagnosticConsumerImpl(PathDiagnosticConsumerOptions DiagOpts,
                                       PathDiagnosticConsumers &C,
                                       const std::string &Output,
                                       const Preprocessor &PP);

} // namespace clang::ento

#endif
