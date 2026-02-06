//==-- DebugContainerModeling.cpp ---------------------------------*- C++ -*--//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Defines a checker for debugging iterator modeling.
//
//===----------------------------------------------------------------------===//

#include <memory>
#include <string>
#include <utility>

#include "clang/AST/Expr.h"
#include "clang/Basic/LLVM.h"
#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugReporter.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CallDescription.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CallEvent.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/SVals.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/SymExpr.h"
#include "llvm/ADT/APSInt.h"
#include "llvm/ADT/StringRef.h"
#include "Iterator.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/BasicValueFactory.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/ProgramState.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/SValBuilder.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "llvm/Support/Casting.h"

namespace clang {
namespace ento {
class ExplodedNode;
class MemRegion;
}  // namespace ento
}  // namespace clang

using namespace clang;
using namespace ento;
using namespace iterator;

namespace {

class DebugContainerModeling
  : public Checker<eval::Call> {

  const BugType DebugMsgBugType{this, "Checking analyzer assumptions", "debug",
                                /*SuppressOnSink=*/true};

  template <typename Getter>
  void analyzerContainerDataField(const CallExpr *CE, CheckerContext &C,
                                  Getter get) const;
  void analyzerContainerBegin(const CallExpr *CE, CheckerContext &C) const;
  void analyzerContainerEnd(const CallExpr *CE, CheckerContext &C) const;
  ExplodedNode *reportDebugMsg(llvm::StringRef Msg, CheckerContext &C) const;

  typedef void (DebugContainerModeling::*FnCheck)(const CallExpr *,
                                                 CheckerContext &) const;

  CallDescriptionMap<FnCheck> Callbacks = {
      {{CDM::SimpleFunc, {"clang_analyzer_container_begin"}, 1},
       &DebugContainerModeling::analyzerContainerBegin},
      {{CDM::SimpleFunc, {"clang_analyzer_container_end"}, 1},
       &DebugContainerModeling::analyzerContainerEnd},
  };

public:
  bool evalCall(const CallEvent &Call, CheckerContext &C) const;
};

} // namespace

bool DebugContainerModeling::evalCall(const CallEvent &Call,
                                      CheckerContext &C) const {
  const auto *CE = dyn_cast_or_null<CallExpr>(Call.getOriginExpr());
  if (!CE)
    return false;

  const FnCheck *Handler = Callbacks.lookup(Call);
  if (!Handler)
    return false;

  (this->**Handler)(CE, C);
  return true;
}

template <typename Getter>
void DebugContainerModeling::analyzerContainerDataField(const CallExpr *CE,
                                                        CheckerContext &C,
                                                        Getter get) const {
  if (CE->getNumArgs() == 0) {
    reportDebugMsg("Missing container argument", C);
    return;
  }

  auto State = C.getState();
  const MemRegion *Cont = C.getSVal(CE->getArg(0)).getAsRegion();
  if (Cont) {
    const auto *Data = getContainerData(State, Cont);
    if (Data) {
      SymbolRef Field = get(Data);
      if (Field) {
        State = State->BindExpr(CE, C.getLocationContext(),
                                nonloc::SymbolVal(Field));

        // Progpagate interestingness from the container's data (marked
        // interesting by an `ExprInspection` debug call to the container
        // itself.
        const NoteTag *InterestingTag =
          C.getNoteTag(
              [Cont, Field](PathSensitiveBugReport &BR) -> std::string {
                if (BR.isInteresting(Field)) {
                  BR.markInteresting(Cont);
                }
                return "";
              });
        C.addTransition(State, InterestingTag);
        return;
      }
    }
  }

  auto &BVF = C.getSValBuilder().getBasicValueFactory();
  State = State->BindExpr(CE, C.getLocationContext(),
                   nonloc::ConcreteInt(BVF.getValue(llvm::APSInt::get(0))));
}

void DebugContainerModeling::analyzerContainerBegin(const CallExpr *CE,
                                                    CheckerContext &C) const {
  analyzerContainerDataField(CE, C, [](const ContainerData *D) {
      return D->getBegin();
    });
}

void DebugContainerModeling::analyzerContainerEnd(const CallExpr *CE,
                                                  CheckerContext &C) const {
  analyzerContainerDataField(CE, C, [](const ContainerData *D) {
      return D->getEnd();
    });
}

ExplodedNode *DebugContainerModeling::reportDebugMsg(llvm::StringRef Msg,
                                                     CheckerContext &C) const {
  ExplodedNode *N = C.generateNonFatalErrorNode();
  if (!N)
    return nullptr;

  auto &BR = C.getBugReporter();
  BR.emitReport(
      std::make_unique<PathSensitiveBugReport>(DebugMsgBugType, Msg, N));
  return N;
}

void ento::registerDebugContainerModeling(CheckerManager &mgr) {
  mgr.registerChecker<DebugContainerModeling>();
}

bool ento::shouldRegisterDebugContainerModeling(const CheckerManager &mgr) {
  return true;
}
