//===- NestedNameSpecifier.cpp - C++ nested name specifiers ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  This file defines the NestedNameSpecifier class, which represents
//  a C++ nested-name-specifier.
//
//===----------------------------------------------------------------------===//

#include "clang/AST/NestedNameSpecifier.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclBase.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/DependenceFlags.h"
#include "clang/AST/NestedNameSpecifierBase.h"
#include "clang/AST/PrettyPrinter.h"
#include "clang/AST/TemplateName.h"
#include "clang/AST/Type.h"
#include "clang/AST/TypeLoc.h"
#include "clang/Basic/LLVM.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/SourceLocation.h"
#include "llvm/ADT/FoldingSet.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/MemAlloc.h"
#include "llvm/Support/raw_ostream.h"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <optional>

using namespace clang;

CXXRecordDecl *NestedNameSpecifier::getAsRecordDecl() const {
  switch (getKind()) {
  case Kind::MicrosoftSuper:
    return getAsMicrosoftSuper();
  case Kind::Type:
    return getAsType()->getAsCXXRecordDecl();
  case Kind::Global:
  case Kind::Namespace:
  case Kind::Null:
    return nullptr;
  }
  llvm_unreachable("Invalid NNS Kind!");
}

auto NestedNameSpecifier::getKind() const -> Kind {
  if (!isStoredKind()) {
    switch (getFlagKind()) {
    case FlagKind::Null:
      return Kind::Null;
    case FlagKind::Global:
      return Kind::Global;
    case FlagKind::Invalid:
      llvm_unreachable("use of invalid NestedNameSpecifier");
    }
    llvm_unreachable("unhandled FlagKind");
  }
  switch (auto [K, Ptr] = getStored(); K) {
  case StoredKind::Type:
    return Kind::Type;
  case StoredKind::NamespaceWithGlobal:
  case StoredKind::NamespaceWithNamespace:
    return Kind::Namespace;
  case StoredKind::NamespaceOrSuper:
    switch (static_cast<const Decl *>(Ptr)->getKind()) {
    case Decl::Namespace:
    case Decl::NamespaceAlias:
      return Kind::Namespace;
    case Decl::CXXRecord:
    case Decl::ClassTemplateSpecialization:
    case Decl::ClassTemplatePartialSpecialization:
      return Kind::MicrosoftSuper;
    default:
      llvm_unreachable("unexpected decl kind");
    }
  }
  llvm_unreachable("unknown StoredKind");
}

NestedNameSpecifier::NestedNameSpecifier(const Type *T)
    : NestedNameSpecifier({StoredKind::Type, T}) {
  assert(getKind() == Kind::Type);
}

auto NestedNameSpecifier::MakeNamespacePtrKind(
    const ASTContext &Ctx, const NamespaceBaseDecl *Namespace,
    NestedNameSpecifier Prefix) -> PtrKind {
  switch (Prefix.getKind()) {
  case Kind::Null:
    return {StoredKind::NamespaceOrSuper, Namespace};
  case Kind::Global:
    return {StoredKind::NamespaceWithGlobal, Namespace};
  case Kind::Namespace:
    return {StoredKind::NamespaceWithNamespace,
            MakeNamespaceAndPrefixStorage(Ctx, Namespace, Prefix)};
  case Kind::MicrosoftSuper:
  case Kind::Type:
    llvm_unreachable("invalid prefix for namespace");
  }
  llvm_unreachable("unhandled kind");
}

/// Builds a nested name specifier that names a namespace.
NestedNameSpecifier::NestedNameSpecifier(const ASTContext &Ctx,
                                         const NamespaceBaseDecl *Namespace,
                                         NestedNameSpecifier Prefix)
    : NestedNameSpecifier(MakeNamespacePtrKind(Ctx, Namespace, Prefix)) {
  assert(getKind() == Kind::Namespace);
}

/// Builds a nested name specifier that names a class through microsoft's
/// __super specifier.
NestedNameSpecifier::NestedNameSpecifier(CXXRecordDecl *RD)
    : NestedNameSpecifier({StoredKind::NamespaceOrSuper, RD}) {
  assert(getKind() == Kind::MicrosoftSuper);
}

NestedNameSpecifier NestedNameSpecifier::getCanonical() const {
  switch (getKind()) {
  case NestedNameSpecifier::Kind::Null:
  case NestedNameSpecifier::Kind::Global:
  case NestedNameSpecifier::Kind::MicrosoftSuper:
    // These are canonical and unique.
    return *this;
  case NestedNameSpecifier::Kind::Namespace: {
    // A namespace is canonical; build a nested-name-specifier with
    // this namespace and no prefix.
    const NamespaceBaseDecl *ND = getAsNamespaceAndPrefix().Namespace;
    return NestedNameSpecifier(
        {StoredKind::NamespaceOrSuper, ND->getNamespace()->getCanonicalDecl()});
  }
  case NestedNameSpecifier::Kind::Type:
    return NestedNameSpecifier(
        getAsType()->getCanonicalTypeInternal().getTypePtr());
  }
  llvm_unreachable("unhandled kind");
}

bool NestedNameSpecifier::isCanonical() const {
  return *this == getCanonical();
}

TypeLoc NestedNameSpecifierLoc::castAsTypeLoc() const {
  return TypeLoc(Qualifier.getAsType(), LoadPointer(/*Offset=*/0));
}

TypeLoc NestedNameSpecifierLoc::getAsTypeLoc() const {
  if (Qualifier.getKind() != NestedNameSpecifier::Kind::Type)
    return TypeLoc();
  return castAsTypeLoc();
}

unsigned
NestedNameSpecifierLoc::getLocalDataLength(NestedNameSpecifier Qualifier) {
  // Location of the trailing '::'.
  unsigned Length = sizeof(SourceLocation::UIntTy);

  switch (Qualifier.getKind()) {
  case NestedNameSpecifier::Kind::Global:
    // Nothing more to add.
    break;

  case NestedNameSpecifier::Kind::Namespace:
  case NestedNameSpecifier::Kind::MicrosoftSuper:
    // The location of the identifier or namespace name.
    Length += sizeof(SourceLocation::UIntTy);
    break;

  case NestedNameSpecifier::Kind::Type:
    // The "void*" that points at the TypeLoc data.
    // Note: the 'template' keyword is part of the TypeLoc.
    Length += sizeof(void *);
    break;

  case NestedNameSpecifier::Kind::Null:
    llvm_unreachable("Expected a non-NULL qualifier");
  }

  return Length;
}

NamespaceAndPrefixLoc NestedNameSpecifierLoc::castAsNamespaceAndPrefix() const {
  auto [Namespace, Prefix] = Qualifier.getAsNamespaceAndPrefix();
  return {Namespace, NestedNameSpecifierLoc(Prefix, Data)};
}

NamespaceAndPrefixLoc NestedNameSpecifierLoc::getAsNamespaceAndPrefix() const {
  if (Qualifier.getKind() != NestedNameSpecifier::Kind::Namespace)
    return {};
  return castAsNamespaceAndPrefix();
}

unsigned NestedNameSpecifierLoc::getDataLength(NestedNameSpecifier Qualifier) {
  unsigned Length = 0;
  for (; Qualifier; Qualifier = Qualifier.getAsNamespaceAndPrefix().Prefix) {
    Length += getLocalDataLength(Qualifier);
    if (Qualifier.getKind() != NestedNameSpecifier::Kind::Namespace)
      break;
  }
  return Length;
}

unsigned NestedNameSpecifierLoc::getDataLength() const {
  return getDataLength(Qualifier);
}

SourceRange NestedNameSpecifierLoc::getLocalSourceRange() const {
  switch (auto Kind = Qualifier.getKind()) {
  case NestedNameSpecifier::Kind::Null:
    return SourceRange();
  case NestedNameSpecifier::Kind::Global:
    return LoadSourceLocation(/*Offset=*/0);
  case NestedNameSpecifier::Kind::Namespace:
  case NestedNameSpecifier::Kind::MicrosoftSuper: {
    unsigned Offset =
        Kind == NestedNameSpecifier::Kind::Namespace
            ? getDataLength(Qualifier.getAsNamespaceAndPrefix().Prefix)
            : 0;
    return SourceRange(
        LoadSourceLocation(Offset),
        LoadSourceLocation(Offset + sizeof(SourceLocation::UIntTy)));
  }
  case NestedNameSpecifier::Kind::Type: {
    // The "void*" that points at the TypeLoc data.
    // Note: the 'template' keyword is part of the TypeLoc.
    void *TypeData = LoadPointer(/*Offset=*/0);
    TypeLoc TL(Qualifier.getAsType(), TypeData);
    return SourceRange(TL.getBeginLoc(), LoadSourceLocation(sizeof(void *)));
  }
  }

  llvm_unreachable("Invalid NNS Kind!");
}

SourceRange NestedNameSpecifierLoc::getSourceRange() const {
  return SourceRange(getBeginLoc(), getEndLoc());
}

SourceLocation NestedNameSpecifierLoc::getEndLoc() const {
  return getLocalSourceRange().getEnd();
}

/// Retrieve the location of the beginning of this
/// component of the nested-name-specifier.
SourceLocation NestedNameSpecifierLoc::getLocalBeginLoc() const {
  return getLocalSourceRange().getBegin();
}

/// Retrieve the location of the end of this component of the
/// nested-name-specifier.
SourceLocation NestedNameSpecifierLoc::getLocalEndLoc() const {
  return getLocalSourceRange().getEnd();
}

SourceRange NestedNameSpecifierLocBuilder::getSourceRange() const {
  return NestedNameSpecifierLoc(Representation, Buffer).getSourceRange();
}

NamespaceAndPrefix NestedNameSpecifier::getAsNamespaceAndPrefix() const {
  auto [Kind, Ptr] = getStored();
  switch (Kind) {
  case StoredKind::NamespaceOrSuper:
  case StoredKind::NamespaceWithGlobal:
    return {static_cast<const NamespaceBaseDecl *>(Ptr),
            Kind == StoredKind::NamespaceWithGlobal
                ? NestedNameSpecifier::getGlobal()
                : std::nullopt};
  case StoredKind::NamespaceWithNamespace:
    return *static_cast<const NamespaceAndPrefixStorage *>(Ptr);
  case StoredKind::Type:;
  }
  llvm_unreachable("unexpected stored kind");
}

const NamespaceAndPrefixStorage *
NestedNameSpecifier::MakeNamespaceAndPrefixStorage(
    const ASTContext &Ctx, const NamespaceBaseDecl *Namespace,
    NestedNameSpecifier Prefix) {
  llvm::FoldingSetNodeID ID;
  NamespaceAndPrefixStorage::Profile(ID, Namespace, Prefix);

  void *InsertPos = nullptr;
  NamespaceAndPrefixStorage *S =
      Ctx.NamespaceAndPrefixStorages.FindNodeOrInsertPos(ID, InsertPos);
  if (!S) {
    S = new (Ctx, alignof(NamespaceAndPrefixStorage))
        NamespaceAndPrefixStorage(Namespace, Prefix);
    Ctx.NamespaceAndPrefixStorages.InsertNode(S, InsertPos);
  }
  return S;
}

bool NestedNameSpecifier::isFullyQualified() const {
  switch (getKind()) {
  case NestedNameSpecifier::Kind::Global:
    return true;
  case NestedNameSpecifier::Kind::Null:
  case NestedNameSpecifier::Kind::MicrosoftSuper:
    return false;
  case NestedNameSpecifier::Kind::Namespace:
    return getAsNamespaceAndPrefix().Prefix.isFullyQualified();
  case NestedNameSpecifier::Kind::Type:
    return getAsType()->getPrefix().isFullyQualified();
  }
  llvm_unreachable("Invalid NNS Kind!");
}

NestedNameSpecifierDependence NestedNameSpecifier::getDependence() const {
  switch (getKind()) {
  case Kind::Null:
  case Kind::Global:
  case Kind::Namespace:
    return NestedNameSpecifierDependence::None;
  case Kind::MicrosoftSuper: {
    CXXRecordDecl *RD = getAsMicrosoftSuper();
    return RD->isDependentContext()
               ? NestedNameSpecifierDependence::DependentInstantiation |
                     NestedNameSpecifierDependence::Dependent
               : NestedNameSpecifierDependence::None;
  }
  case Kind::Type:
    return toNestedNameSpecifierDependence(getAsType()->getDependence());
  }
  llvm_unreachable("Invalid NNS Kind!");
}

/// Print this nested name specifier to the given output
/// stream.
void NestedNameSpecifier::print(raw_ostream &OS, const PrintingPolicy &Policy,
                                bool ResolveTemplateArguments,
                                bool PrintFinalScopeResOp) const {
  switch (getKind()) {
  case Kind::Namespace: {
    auto [Namespace, Prefix] = getAsNamespaceAndPrefix();
    Prefix.print(OS, Policy);
    if (const auto *NS = dyn_cast<NamespaceDecl>(Namespace)) {
      assert(!NS->isAnonymousNamespace());
      OS << NS->getName();
    } else {
      OS << cast<NamespaceAliasDecl>(Namespace)->getName();
    }
    break;
  }
  case Kind::Global:
    OS << "::";
    return;
  case Kind::MicrosoftSuper:
    OS << "__super";
    break;
  case Kind::Type: {
    PrintingPolicy InnerPolicy(Policy);
    InnerPolicy.SuppressTagKeyword = true;
    QualType(getAsType(), 0).print(OS, InnerPolicy);
    break;
  }
  case Kind::Null:
    return;
  }
  if (PrintFinalScopeResOp)
    OS << "::";
}

LLVM_DUMP_METHOD void NestedNameSpecifier::dump(llvm::raw_ostream *OS,
                                                const LangOptions *LO) const {
  print(OS ? *OS : llvm::errs(), LO ? *LO : LangOptions());
}

LLVM_DUMP_METHOD void NestedNameSpecifier::dump(const LangOptions &LO) const {
  dump(/*OS=*/nullptr, &LO);
}
LLVM_DUMP_METHOD void NestedNameSpecifier::dump(llvm::raw_ostream &OS) const {
  dump(&OS);
}
LLVM_DUMP_METHOD void NestedNameSpecifier::dump(llvm::raw_ostream &OS,
                                                const LangOptions &LO) const {
  dump(&OS, &LO);
}

SourceLocation NestedNameSpecifierLoc::getBeginLoc() const {
  if (!Qualifier)
    return SourceLocation();

  NestedNameSpecifierLoc First = *this;
  while (NestedNameSpecifierLoc Prefix = First.getAsNamespaceAndPrefix().Prefix)
    First = Prefix;
  return First.getLocalSourceRange().getBegin();
}

static void Append(char *Start, char *End, char *&Buffer, unsigned &BufferSize,
                   unsigned &BufferCapacity) {
  if (Start == End)
    return;

  if (BufferSize + (End - Start) > BufferCapacity) {
    // Reallocate the buffer.
    unsigned NewCapacity = std::max(
        (unsigned)(BufferCapacity ? BufferCapacity * 2 : sizeof(void *) * 2),
        (unsigned)(BufferSize + (End - Start)));
    if (!BufferCapacity) {
      char *NewBuffer = static_cast<char *>(llvm::safe_malloc(NewCapacity));
      if (Buffer)
        memcpy(NewBuffer, Buffer, BufferSize);
      Buffer = NewBuffer;
    } else {
      Buffer = static_cast<char *>(llvm::safe_realloc(Buffer, NewCapacity));
    }
    BufferCapacity = NewCapacity;
  }
  assert(Buffer && Start && End && End > Start && "Illegal memory buffer copy");
  memcpy(Buffer + BufferSize, Start, End - Start);
  BufferSize += End - Start;
}

/// Save a source location to the given buffer.
static void SaveSourceLocation(SourceLocation Loc, char *&Buffer,
                               unsigned &BufferSize, unsigned &BufferCapacity) {
  SourceLocation::UIntTy Raw = Loc.getRawEncoding();
  Append(reinterpret_cast<char *>(&Raw),
         reinterpret_cast<char *>(&Raw) + sizeof(Raw), Buffer, BufferSize,
         BufferCapacity);
}

/// Save a pointer to the given buffer.
static void SavePointer(void *Ptr, char *&Buffer, unsigned &BufferSize,
                        unsigned &BufferCapacity) {
  Append(reinterpret_cast<char *>(&Ptr),
         reinterpret_cast<char *>(&Ptr) + sizeof(void *),
         Buffer, BufferSize, BufferCapacity);
}

NestedNameSpecifierLocBuilder::
NestedNameSpecifierLocBuilder(const NestedNameSpecifierLocBuilder &Other)
    : Representation(Other.Representation) {
  if (!Other.Buffer)
    return;

  if (Other.BufferCapacity == 0) {
    // Shallow copy is okay.
    Buffer = Other.Buffer;
    BufferSize = Other.BufferSize;
    return;
  }

  // Deep copy
  Append(Other.Buffer, Other.Buffer + Other.BufferSize, Buffer, BufferSize,
         BufferCapacity);
}

NestedNameSpecifierLocBuilder &
NestedNameSpecifierLocBuilder::
operator=(const NestedNameSpecifierLocBuilder &Other) {
  Representation = Other.Representation;

  if (Buffer && Other.Buffer && BufferCapacity >= Other.BufferSize) {
    // Re-use our storage.
    BufferSize = Other.BufferSize;
    memcpy(Buffer, Other.Buffer, BufferSize);
    return *this;
  }

  // Free our storage, if we have any.
  if (BufferCapacity) {
    free(Buffer);
    BufferCapacity = 0;
  }

  if (!Other.Buffer) {
    // Empty.
    Buffer = nullptr;
    BufferSize = 0;
    return *this;
  }

  if (Other.BufferCapacity == 0) {
    // Shallow copy is okay.
    Buffer = Other.Buffer;
    BufferSize = Other.BufferSize;
    return *this;
  }

  // Deep copy.
  BufferSize = 0;
  Append(Other.Buffer, Other.Buffer + Other.BufferSize, Buffer, BufferSize,
         BufferCapacity);
  return *this;
}

void NestedNameSpecifierLocBuilder::Make(ASTContext &Context, TypeLoc TL,
                                         SourceLocation ColonColonLoc) {
  assert(!Representation);
  Representation = NestedNameSpecifier(TL.getTypePtr());

  // Push source-location info into the buffer.
  SavePointer(TL.getOpaqueData(), Buffer, BufferSize, BufferCapacity);
  SaveSourceLocation(ColonColonLoc, Buffer, BufferSize, BufferCapacity);
}

void NestedNameSpecifierLocBuilder::Extend(ASTContext &Context,
                                           const NamespaceBaseDecl *Namespace,
                                           SourceLocation NamespaceLoc,
                                           SourceLocation ColonColonLoc) {
  Representation = NestedNameSpecifier(Context, Namespace, Representation);

  // Push source-location info into the buffer.
  SaveSourceLocation(NamespaceLoc, Buffer, BufferSize, BufferCapacity);
  SaveSourceLocation(ColonColonLoc, Buffer, BufferSize, BufferCapacity);
}

void NestedNameSpecifierLocBuilder::MakeGlobal(ASTContext &Context,
                                               SourceLocation ColonColonLoc) {
  assert(!Representation && "Already have a nested-name-specifier!?");
  Representation = NestedNameSpecifier::getGlobal();

  // Push source-location info into the buffer.
  SaveSourceLocation(ColonColonLoc, Buffer, BufferSize, BufferCapacity);
}

void NestedNameSpecifierLocBuilder::MakeMicrosoftSuper(
    ASTContext &Context, CXXRecordDecl *RD, SourceLocation SuperLoc,
    SourceLocation ColonColonLoc) {
  Representation = NestedNameSpecifier(RD);

  // Push source-location info into the buffer.
  SaveSourceLocation(SuperLoc, Buffer, BufferSize, BufferCapacity);
  SaveSourceLocation(ColonColonLoc, Buffer, BufferSize, BufferCapacity);
}

void NestedNameSpecifierLocBuilder::PushTrivial(ASTContext &Context,
                                                NestedNameSpecifier Qualifier,
                                                SourceRange R) {
  // Construct bogus (but well-formed) source information for the
  // nested-name-specifier.
  switch (Qualifier.getKind()) {
  case NestedNameSpecifier::Kind::Null:
    return;
  case NestedNameSpecifier::Kind::Namespace: {
    auto [_1, Prefix] = Qualifier.getAsNamespaceAndPrefix();
    PushTrivial(Context, Prefix, R.getBegin());
    SaveSourceLocation(R.getBegin(), Buffer, BufferSize, BufferCapacity);
    break;
  }
  case NestedNameSpecifier::Kind::Type: {
    TypeSourceInfo *TSInfo = Context.getTrivialTypeSourceInfo(
        QualType(Qualifier.getAsType(), 0), R.getBegin());
    SavePointer(TSInfo->getTypeLoc().getOpaqueData(), Buffer, BufferSize,
                BufferCapacity);
    break;
  }
  case NestedNameSpecifier::Kind::Global:
  case NestedNameSpecifier::Kind::MicrosoftSuper:
    break;
  }
  SaveSourceLocation(R.getEnd(), Buffer, BufferSize, BufferCapacity);
}

void NestedNameSpecifierLocBuilder::Adopt(NestedNameSpecifierLoc Other) {
  if (BufferCapacity)
    free(Buffer);

  if (!Other) {
    Representation = std::nullopt;
    BufferSize = 0;
    return;
  }

  // Rather than copying the data (which is wasteful), "adopt" the
  // pointer (which points into the ASTContext) but set the capacity to zero to
  // indicate that we don't own it.
  Representation = Other.getNestedNameSpecifier();
  Buffer = static_cast<char *>(Other.getOpaqueData());
  BufferSize = Other.getDataLength();
  BufferCapacity = 0;
}

NestedNameSpecifierLoc
NestedNameSpecifierLocBuilder::getWithLocInContext(ASTContext &Context) const {
  if (!Representation)
    return NestedNameSpecifierLoc();

  // If we adopted our data pointer from elsewhere in the AST context, there's
  // no need to copy the memory.
  if (BufferCapacity == 0)
    return NestedNameSpecifierLoc(Representation, Buffer);

  // FIXME: After copying the source-location information, should we free
  // our (temporary) buffer and adopt the ASTContext-allocated memory?
  // Doing so would optimize repeated calls to getWithLocInContext().
  void *Mem = Context.Allocate(BufferSize, alignof(void *));
  memcpy(Mem, Buffer, BufferSize);
  return NestedNameSpecifierLoc(Representation, Mem);
}
