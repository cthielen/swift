//===--- ASTBridging.h - header for the swift SILBridging module ----------===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2022 - 2025 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://swift.org/LICENSE.txt for license information
// See https://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//

#ifndef SWIFT_AST_ASTBRIDGING_H
#define SWIFT_AST_ASTBRIDGING_H

/// `ASTBridging.h` is imported into Swift. Be *very* careful with what you
/// include here and keep these includes minimal!
///
/// See include guidelines and caveats in `BasicBridging.h`.
#include "swift/AST/AccessorKind.h"
#include "swift/AST/AttrKind.h"
#include "swift/AST/DiagnosticKind.h"
#include "swift/AST/DiagnosticList.h"
#include "swift/AST/GenericTypeParamKind.h"
#include "swift/AST/Identifier.h"
#include "swift/AST/LayoutConstraintKind.h"
#include "swift/AST/PlatformKind.h"
#include "swift/Basic/BasicBridging.h"

#ifdef NOT_COMPILED_WITH_SWIFT_PURE_BRIDGING_MODE
#include "swift/AST/Attr.h"
#include "swift/AST/Decl.h"
#endif

SWIFT_BEGIN_NULLABILITY_ANNOTATIONS

namespace llvm {
template<typename T> class ArrayRef;
}

namespace swift {
enum class AccessorKind;
class AvailabilityDomainOrIdentifier;
class Argument;
class ASTContext;
struct ASTNode;
struct CaptureListEntry;
class DeclAttributes;
class DeclBaseName;
class DeclNameLoc;
class DeclNameRef;
class DiagnosticArgument;
class DiagnosticEngine;
enum class DifferentiabilityKind : uint8_t;
class Fingerprint;
class Identifier;
class IfConfigClauseRangeInfo;
class GenericSignature;
class GenericSignatureImpl;
struct LabeledStmtInfo;
class LayoutConstraint;
class LayoutConstraintInfo;
struct LifetimeDescriptor;
enum class MacroRole : uint32_t;
class MacroIntroducedDeclName;
enum class MacroIntroducedDeclNameKind;
enum class ParamSpecifier : uint8_t;
class ParsedAutoDiffParameter;
class ProtocolConformanceRef;
class RegexLiteralPatternFeature;
class RegexLiteralPatternFeatureKind;
enum class ReferenceOwnership : uint8_t;
class RequirementRepr;
class Type;
class CanType;
class TypeBase;
class StmtConditionElement;
class SubstitutionMap;
enum class RequirementReprKind : unsigned;
}

struct BridgedASTType;
class BridgedCanType;
class BridgedASTContext;
struct BridgedSubstitutionMap;
struct BridgedGenericSignature;
struct BridgedConformance;
class BridgedParameterList;

// Forward declare the underlying AST node type for each wrapper.
namespace swift {
#define AST_BRIDGING_WRAPPER(Name) class Name;
#include "swift/AST/ASTBridgingWrappers.def"
} // end namespace swift

// Define the bridging wrappers for each AST node.
#define AST_BRIDGING_WRAPPER(Name) BRIDGING_WRAPPER_NONNULL(swift::Name, Name)
#define AST_BRIDGING_WRAPPER_CONST(Name)                                       \
  BRIDGING_WRAPPER_CONST_NONNULL(swift::Name, Name)
#include "swift/AST/ASTBridgingWrappers.def"

// For nullable nodes, also define a nullable variant.
#define AST_BRIDGING_WRAPPER_NULLABLE(Name)                                    \
  BRIDGING_WRAPPER_NULLABLE(swift::Name, Name)
#define AST_BRIDGING_WRAPPER_CONST_NULLABLE(Name)                              \
  BRIDGING_WRAPPER_CONST_NULLABLE(swift::Name, Name)
#define AST_BRIDGING_WRAPPER_NONNULL(Name)
#define AST_BRIDGING_WRAPPER_CONST_NONNULL(Name)
#include "swift/AST/ASTBridgingWrappers.def"

//===----------------------------------------------------------------------===//
// MARK: Identifier
//===----------------------------------------------------------------------===//

struct BridgedLocatedIdentifier {
  SWIFT_NAME("name")
  swift::Identifier Name;

  SWIFT_NAME("nameLoc")
  swift::SourceLoc NameLoc;
};

struct BridgedConsumedLookupResult {
  SWIFT_NAME("name")
  swift::Identifier Name;

  SWIFT_NAME("nameLoc")
  swift::SourceLoc NameLoc;

  SWIFT_NAME("flag")
  SwiftInt Flag;

  BRIDGED_INLINE BridgedConsumedLookupResult(swift::Identifier name,
                                             swift::SourceLoc sourceLoc,
                                             SwiftInt flag);
};

class BridgedDeclNameRef {
  void *_Nullable opaque;

public:
  BRIDGED_INLINE BridgedDeclNameRef();

  BRIDGED_INLINE BridgedDeclNameRef(swift::DeclNameRef name);

  BRIDGED_INLINE swift::DeclNameRef unbridged() const;
};

SWIFT_NAME("BridgedDeclNameRef.createParsed(_:baseName:argumentLabels:)")
BridgedDeclNameRef BridgedDeclNameRef_createParsed(BridgedASTContext cContext,
                                                   swift::DeclBaseName baseName,
                                                   BridgedArrayRef cLabels);

SWIFT_NAME("BridgedDeclNameRef.createParsed(_:)")
BridgedDeclNameRef
BridgedDeclNameRef_createParsed(swift::DeclBaseName baseName);

class BridgedDeclNameLoc {
  const void *_Nullable LocationInfo;
  size_t NumArgumentLabels;

public:
  BridgedDeclNameLoc() : LocationInfo(nullptr), NumArgumentLabels(0) {}

  BRIDGED_INLINE BridgedDeclNameLoc(swift::DeclNameLoc loc);

  BRIDGED_INLINE swift::DeclNameLoc unbridged() const;
};

SWIFT_NAME("BridgedDeclNameLoc.createParsed(_:baseNameLoc:lParenLoc:"
           "argumentLabelLocs:rParenLoc:)")
BridgedDeclNameLoc BridgedDeclNameLoc_createParsed(BridgedASTContext cContext,
                                                   swift::SourceLoc baseNameLoc,
                                                   swift::SourceLoc lParenLoc,
                                                   BridgedArrayRef cLabelLocs,
                                                   swift::SourceLoc rParenLoc);

SWIFT_NAME("BridgedDeclNameLoc.createParsed(_:moduleSelectorLoc:baseNameLoc:"
           "lParenLoc:argumentLabelLocs:rParenLoc:)")
BridgedDeclNameLoc BridgedDeclNameLoc_createParsed(
    BridgedASTContext cContext, swift::SourceLoc cModuleSelectorLoc,
    swift::SourceLoc baseNameLoc, swift::SourceLoc lParenLoc,
    BridgedArrayRef cLabelLocs, swift::SourceLoc rParenLoc);

SWIFT_NAME("BridgedDeclNameLoc.createParsed(_:)")
BridgedDeclNameLoc
BridgedDeclNameLoc_createParsed(swift::SourceLoc baseNameLoc);

SWIFT_NAME("BridgedDeclNameLoc.createParsed(_:moduleSelectorLoc:baseNameLoc:)")
BridgedDeclNameLoc
BridgedDeclNameLoc_createParsed(BridgedASTContext cContext,
                                swift::SourceLoc moduleSelectorLoc,
                                swift::SourceLoc baseNameLoc);

//===----------------------------------------------------------------------===//
// MARK: ASTContext
//===----------------------------------------------------------------------===//

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedEndianness : size_t {
  EndianLittle,
  EndianBig,
};

class BridgedASTContext {
  swift::ASTContext * _Nonnull Ctx;

public:
  SWIFT_UNAVAILABLE("Use init(raw:) instead")
  BRIDGED_INLINE BridgedASTContext(swift::ASTContext &ctx);

  SWIFT_UNAVAILABLE("Use '.raw' instead")
  BRIDGED_INLINE swift::ASTContext &unbridged() const;

  SWIFT_COMPUTED_PROPERTY
  void *_Nonnull getRaw() const { return Ctx; }

  SWIFT_COMPUTED_PROPERTY
  unsigned getMajorLanguageVersion() const;

  SWIFT_COMPUTED_PROPERTY
  unsigned getLangOptsTargetPointerBitWidth() const;

  SWIFT_COMPUTED_PROPERTY
  bool getLangOptsAttachCommentsToDecls() const;

  SWIFT_COMPUTED_PROPERTY
  BridgedEndianness getLangOptsTargetEndianness() const;

  SWIFT_COMPUTED_PROPERTY
  BridgedAvailabilityMacroMap getAvailabilityMacroMap() const;
};

#define IDENTIFIER_WITH_NAME(Name, _)                                          \
  SWIFT_NAME("getter:BridgedASTContext.id_" #Name "(self:)")                   \
  BRIDGED_INLINE swift::Identifier BridgedASTContext_id_##Name(                \
      BridgedASTContext bridged);
#include "swift/AST/KnownIdentifiers.def"

SWIFT_NAME("BridgedASTContext.init(raw:)")
BRIDGED_INLINE
BridgedASTContext BridgedASTContext_fromRaw(void * _Nonnull ptr);

SWIFT_NAME("BridgedASTContext.allocate(self:size:alignment:)")
BRIDGED_INLINE
void *_Nullable BridgedASTContext_allocate(BridgedASTContext bridged,
                                           size_t size, size_t alignment);

SWIFT_NAME("BridgedASTContext.allocateCopy(self:string:)")
BRIDGED_INLINE
BridgedStringRef
BridgedASTContext_allocateCopyString(BridgedASTContext cContext,
                                     BridgedStringRef cStr);

SWIFT_NAME("BridgedASTContext.getIdentifier(self:_:)")
swift::Identifier BridgedASTContext_getIdentifier(BridgedASTContext cContext,
                                                  BridgedStringRef cStr);

SWIFT_NAME("BridgedASTContext.getDollarIdentifier(self:_:)")
swift::Identifier
BridgedASTContext_getDollarIdentifier(BridgedASTContext cContext, size_t idx);

SWIFT_NAME("BridgedASTContext.langOptsHasFeature(self:_:)")
bool BridgedASTContext_langOptsHasFeature(BridgedASTContext cContext,
                                          BridgedFeature feature);

SWIFT_NAME("BridgedASTContext.langOptsCustomConditionSet(self:_:)")
bool BridgedASTContext_langOptsCustomConditionSet(BridgedASTContext cContext,
                                                  BridgedStringRef cName);

SWIFT_NAME("BridgedASTContext.langOptsHasFeatureNamed(self:_:)")
bool BridgedASTContext_langOptsHasFeatureNamed(BridgedASTContext cContext,
                                               BridgedStringRef cName);

SWIFT_NAME("BridgedASTContext.langOptsHasAttributeNamed(self:_:)")
bool BridgedASTContext_langOptsHasAttributeNamed(BridgedASTContext cContext,
                                                 BridgedStringRef cName);

SWIFT_NAME("BridgedASTContext.langOptsIsActiveTargetOS(self:_:)")
bool BridgedASTContext_langOptsIsActiveTargetOS(BridgedASTContext cContext,
                                                BridgedStringRef cName);

SWIFT_NAME("BridgedASTContext.langOptsIsActiveTargetArchitecture(self:_:)")
bool BridgedASTContext_langOptsIsActiveTargetArchitecture(BridgedASTContext cContext,
                                                          BridgedStringRef cName);

SWIFT_NAME("BridgedASTContext.langOptsIsActiveTargetEnvironment(self:_:)")
bool BridgedASTContext_langOptsIsActiveTargetEnvironment(BridgedASTContext cContext,
                                                         BridgedStringRef cName);

SWIFT_NAME("BridgedASTContext.langOptsIsActiveTargetRuntime(self:_:)")
bool BridgedASTContext_langOptsIsActiveTargetRuntime(BridgedASTContext cContext,
                                                     BridgedStringRef cName);

SWIFT_NAME("BridgedASTContext.langOptsIsActiveTargetPtrAuth(self:_:)")
bool BridgedASTContext_langOptsIsActiveTargetPtrAuth(BridgedASTContext cContext,
                                                     BridgedStringRef cName);

SWIFT_NAME("BridgedASTContext.langOptsGetTargetAtomicBitWidths(self:_:)")
SwiftInt BridgedASTContext_langOptsGetTargetAtomicBitWidths(BridgedASTContext cContext,
                                                      SwiftInt* _Nullable * _Nonnull cComponents);

SWIFT_NAME("BridgedASTContext.langOptsGetLanguageVersion(self:_:)")
SwiftInt BridgedASTContext_langOptsGetLanguageVersion(BridgedASTContext cContext,
                                                      SwiftInt* _Nullable * _Nonnull cComponents);

SWIFT_NAME("BridgedASTContext.langOptsGetCompilerVersion(self:_:)")
SwiftInt BridgedASTContext_langOptsGetCompilerVersion(BridgedASTContext cContext,
                                                      SwiftInt* _Nullable * _Nonnull cComponents);

/* Deallocate an array of Swift int values that was allocated in C++. */
void deallocateIntBuffer(SwiftInt * _Nullable cComponents);

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedCanImportVersion : size_t {
  CanImportUnversioned,
  CanImportVersion,
  CanImportUnderlyingVersion,
};

SWIFT_NAME("BridgedASTContext.canImport(self:importPath:location:versionKind:versionComponents:numVersionComponents:)")
bool BridgedASTContext_canImport(BridgedASTContext cContext,
                                 BridgedStringRef importPath,
                                 swift::SourceLoc canImportLoc,
                                 BridgedCanImportVersion versionKind,
                                 const SwiftInt *_Nullable versionComponents,
                                 SwiftInt numVersionComponents);

//===----------------------------------------------------------------------===//
// MARK: AST nodes
//===----------------------------------------------------------------------===//

void registerBridgedDecl(BridgedStringRef bridgedClassName, SwiftMetatype metatype);

struct OptionalBridgedDeclObj {
  OptionalSwiftObject obj;

  OptionalBridgedDeclObj(OptionalSwiftObject obj) : obj(obj) {}

#ifdef NOT_COMPILED_WITH_SWIFT_PURE_BRIDGING_MODE
  template <class D> D *_Nullable getAs() const {
    if (obj)
      return llvm::cast<D>(static_cast<swift::Decl *>(obj));
    return nullptr;
  }
#endif
};

struct BridgedDeclObj {
  SwiftObject obj;

#ifdef NOT_COMPILED_WITH_SWIFT_PURE_BRIDGING_MODE
  template <class D> D *_Nonnull getAs() const {
    return llvm::cast<D>(static_cast<swift::Decl *>(obj));
  }
  swift::Decl * _Nonnull unbridged() const {
    return getAs<swift::Decl>();
  }
#endif

  BridgedDeclObj(SwiftObject obj) : obj(obj) {}
  BridgedOwnedString getDebugDescription() const;
  BRIDGED_INLINE swift::SourceLoc getLoc() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedDeclObj getModuleContext() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE OptionalBridgedDeclObj getParent() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedStringRef Type_getName() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedStringRef Value_getUserFacingName() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE swift::SourceLoc Value_getNameLoc() const;
  BRIDGED_INLINE bool hasClangNode() const;
  BRIDGED_INLINE bool Value_isObjC() const;
  BRIDGED_INLINE bool AbstractStorage_isConst() const;
  BRIDGED_INLINE bool GenericType_isGenericAtAnyLevel() const;
  BRIDGED_INLINE bool NominalType_isGlobalActor() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE OptionalBridgedDeclObj NominalType_getValueTypeDestructor() const;
  BRIDGED_INLINE bool Enum_hasRawType() const;
  BRIDGED_INLINE bool Struct_hasUnreferenceableStorage() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedASTType Class_getSuperclass() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedDeclObj Class_getDestructor() const;
  BRIDGED_INLINE bool ProtocolDecl_requiresClass() const;
  BRIDGED_INLINE bool AbstractFunction_isOverridden() const;
  BRIDGED_INLINE bool Destructor_isIsolated() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedStringRef AccessorDecl_getKindName() const;
};

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedASTNodeKind : uint8_t {
  BridgedASTNodeKindExpr,
  BridgedASTNodeKindStmt,
  BridgedASTNodeKindDecl
};

class BridgedASTNode {
  intptr_t opaque;

  BRIDGED_INLINE BridgedASTNode(void *_Nonnull pointer,
                                BridgedASTNodeKind kind);

  void *_Nonnull getPointer() const {
    return reinterpret_cast<void *>(opaque & ~0x7);
  }

public:
  SWIFT_NAME("decl(_:)")
  static BridgedASTNode createDecl(BridgedDecl d) {
    return BridgedASTNode(d.unbridged(), BridgedASTNodeKindDecl);
  }
  SWIFT_NAME("stmt(_:)")
  static BridgedASTNode createStmt(BridgedStmt s) {
    return BridgedASTNode(s.unbridged(), BridgedASTNodeKindStmt);
  }
  SWIFT_NAME("expr(_:)")
  static BridgedASTNode createExor(BridgedExpr e) {
    return BridgedASTNode(e.unbridged(), BridgedASTNodeKindExpr);
  }

  SWIFT_COMPUTED_PROPERTY
  BridgedASTNodeKind getKind() const {
    return static_cast<BridgedASTNodeKind>(opaque & 0x7);
  }

  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedExpr castToExpr() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedStmt castToStmt() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedDecl castToDecl() const;

  BRIDGED_INLINE swift::ASTNode unbridged() const;
};

// Declare `.asDecl` on each BridgedXXXDecl type, which upcasts a wrapper for
// a Decl subclass to a BridgedDecl.
#define DECL(Id, Parent)                                                       \
  SWIFT_NAME("getter:Bridged" #Id "Decl.asDecl(self:)")                        \
  BridgedDecl Bridged##Id##Decl_asDecl(Bridged##Id##Decl decl);
#define ABSTRACT_DECL(Id, Parent) DECL(Id, Parent)
#include "swift/AST/DeclNodes.def"

// Declare `.asDeclContext` on each BridgedXXXDecl type that's also a
// DeclContext.
#define DECL(Id, Parent)
#define CONTEXT_DECL(Id, Parent)                                               \
  SWIFT_NAME("getter:Bridged" #Id "Decl.asDeclContext(self:)")                 \
  BridgedDeclContext Bridged##Id##Decl_asDeclContext(Bridged##Id##Decl decl);
#define ABSTRACT_CONTEXT_DECL(Id, Parent) CONTEXT_DECL(Id, Parent)
#include "swift/AST/DeclNodes.def"

// Declare `.asStmt` on each BridgedXXXStmt type, which upcasts a wrapper for
// a Stmt subclass to a BridgedStmt.
#define STMT(Id, Parent)                                                       \
  SWIFT_NAME("getter:Bridged" #Id "Stmt.asStmt(self:)")                        \
  BridgedStmt Bridged##Id##Stmt_asStmt(Bridged##Id##Stmt stmt);
#define ABSTRACT_STMT(Id, Parent) STMT(Id, Parent)
#include "swift/AST/StmtNodes.def"

// Declare `.asExpr` on each BridgedXXXExpr type, which upcasts a wrapper for
// a Expr subclass to a BridgedExpr.
#define EXPR(Id, Parent)                                                       \
  SWIFT_NAME("getter:Bridged" #Id "Expr.asExpr(self:)")                        \
  BridgedExpr Bridged##Id##Expr_asExpr(Bridged##Id##Expr expr);
#define ABSTRACT_EXPR(Id, Parent) EXPR(Id, Parent)
#include "swift/AST/ExprNodes.def"

// Declare `.asTypeRepr` on each BridgedXXXTypeRepr type, which upcasts a
// wrapper for a TypeRepr subclass to a BridgedTypeRepr.
#define TYPEREPR(Id, Parent)                                                   \
  SWIFT_NAME("getter:Bridged" #Id "TypeRepr.asTypeRepr(self:)")                \
  BridgedTypeRepr Bridged##Id##TypeRepr_asTypeRepr(                            \
      Bridged##Id##TypeRepr typeRepr);
#define ABSTRACT_TYPEREPR(Id, Parent) TYPEREPR(Id, Parent)
#include "swift/AST/TypeReprNodes.def"

// Declare `.asPattern` on each BridgedXXXPattern type, which upcasts a wrapper
// for a Pattern subclass to a BridgedPattern.
#define PATTERN(Id, Parent)                                                    \
  SWIFT_NAME("getter:Bridged" #Id "Pattern.asPattern(self:)")                  \
  BridgedPattern Bridged##Id##Pattern_asPattern(Bridged##Id##Pattern pattern);
#include "swift/AST/PatternNodes.def"

// Declare `.asDeclAttribute` on each BridgedXXXAttr type, which upcasts a
// wrapper for a DeclAttribute subclass to a BridgedDeclAttribute.
#define SIMPLE_DECL_ATTR(...)
#define DECL_ATTR(_, CLASS, ...)                                               \
  SWIFT_NAME("getter:Bridged" #CLASS "Attr.asDeclAttribute(self:)")            \
  BridgedDeclAttribute Bridged##CLASS##Attr_asDeclAttribute(                   \
      Bridged##CLASS##Attr attr);
#include "swift/AST/DeclAttr.def"

// Declare `.asTypeAttr` on each BridgedXXXTypeAttr type, which upcasts a
// wrapper for a TypeAttr subclass to a BridgedTypeAttr.
#define SIMPLE_TYPE_ATTR(...)
#define TYPE_ATTR(SPELLING, CLASS)                                             \
  SWIFT_NAME("getter:Bridged" #CLASS "TypeAttr.asTypeAttribute(self:)")        \
  BridgedTypeAttribute Bridged##CLASS##TypeAttr_asTypeAttribute(               \
      Bridged##CLASS##TypeAttr attr);
#include "swift/AST/TypeAttr.def"

struct BridgedPatternBindingEntry {
  BridgedPattern pattern;
  swift::SourceLoc equalLoc;
  BridgedNullableExpr init;
  BridgedNullablePatternBindingInitializer initContext;
};

//===----------------------------------------------------------------------===//
// MARK: Diagnostic Engine
//===----------------------------------------------------------------------===//

class BridgedDiagnosticArgument {
  int64_t storage[3];

public:
  BRIDGED_INLINE BridgedDiagnosticArgument(const swift::DiagnosticArgument &arg);
  BRIDGED_INLINE const swift::DiagnosticArgument &unbridged() const;

  BridgedDiagnosticArgument(SwiftInt i);
  BridgedDiagnosticArgument(BridgedStringRef s);
};

class BridgedFixIt {
public:
  swift::CharSourceRange replacementRange;
  BridgedStringRef replacementText;
};

class BridgedDiagnosticFixIt {
public:
  int64_t storage[7];

  BridgedDiagnosticFixIt(swift::SourceLoc start, uint32_t length,
                         BridgedStringRef text);
};

class BridgedDiagnostic {
public:
  struct Impl;

  SWIFT_UNAVAILABLE("Unavailable in Swift")
  Impl *_Nonnull Raw;

  SWIFT_UNAVAILABLE("Unavailable in Swift")
  BridgedDiagnostic(Impl *_Nonnull raw) : Raw(raw) {}

  SWIFT_UNAVAILABLE("Unavailable in Swift")
  Impl *_Nonnull unbridged() const { return Raw; }
};

// FIXME: Can we bridge InFlightDiagnostic?
SWIFT_NAME("BridgedDiagnosticEngine.diagnose(self:at:_:_:highlightAt:"
           "highlightLength:fixIts:)")
void BridgedDiagnosticEngine_diagnose(
    BridgedDiagnosticEngine, swift::SourceLoc loc, swift::DiagID diagID,
    BridgedArrayRef arguments, swift::SourceLoc highlightStart,
    uint32_t hightlightLength, BridgedArrayRef fixIts);

SWIFT_NAME("BridgedDiagnosticEngine.getLocationFromExternalSource(self:path:line:column:)")
swift::SourceLoc BridgedDiagnostic_getLocationFromExternalSource(
    BridgedDiagnosticEngine bridgedEngine, BridgedStringRef path, SwiftInt line,
    SwiftInt column);

SWIFT_NAME("getter:BridgedDiagnosticEngine.hadAnyError(self:)")
bool BridgedDiagnosticEngine_hadAnyError(BridgedDiagnosticEngine);

/// Create a new diagnostic with the given severity, location, and diagnostic
/// text.
///
/// \returns a diagnostic instance that can be extended with additional
/// information and then must be finished via \c SwiftDiagnostic_finish.
SWIFT_NAME("BridgedDiagnostic.init(at:message:severity:engine:)")
BridgedDiagnostic BridgedDiagnostic_create(swift::SourceLoc loc,
                                           BridgedStringRef cText,
                                           swift::DiagnosticKind severity,
                                           BridgedDiagnosticEngine cDiags);

/// Highlight a source range as part of the diagnostic.
SWIFT_NAME("BridgedDiagnostic.highlight(self:start:end:)")
void BridgedDiagnostic_highlight(BridgedDiagnostic cDiag,
                                 swift::SourceLoc LStartLoc,
                                 swift::SourceLoc endLoc);

/// Add a Fix-It to replace a source range as part of the diagnostic.
SWIFT_NAME("BridgedDiagnostic.fixItReplace(self:start:end:replacement:)")
void BridgedDiagnostic_fixItReplace(BridgedDiagnostic cDiag,
                                    swift::SourceLoc startLoc,
                                    swift::SourceLoc endLoc,
                                    BridgedStringRef cReplaceText);

/// Finish the given diagnostic and emit it.
SWIFT_NAME("BridgedDiagnostic.finish(self:)")
void BridgedDiagnostic_finish(BridgedDiagnostic cDiag);

//===----------------------------------------------------------------------===//
// MARK: DeclContexts
//===----------------------------------------------------------------------===//

SWIFT_NAME("getter:BridgedDeclContext.isLocalContext(self:)")
BRIDGED_INLINE bool
BridgedDeclContext_isLocalContext(BridgedDeclContext cDeclContext);

SWIFT_NAME("getter:BridgedDeclContext.isTypeContext(self:)")
BRIDGED_INLINE bool BridgedDeclContext_isTypeContext(BridgedDeclContext dc);

SWIFT_NAME("getter:BridgedDeclContext.isModuleScopeContext(self:)")
BRIDGED_INLINE bool
BridgedDeclContext_isModuleScopeContext(BridgedDeclContext dc);

SWIFT_NAME("getter:BridgedDeclContext.isClosureExpr(self:)")
BRIDGED_INLINE bool BridgedDeclContext_isClosureExpr(BridgedDeclContext dc);

SWIFT_NAME("BridgedDeclContext.castToClosureExpr(self:)")
BRIDGED_INLINE BridgedClosureExpr
BridgedDeclContext_castToClosureExpr(BridgedDeclContext dc);

SWIFT_NAME("getter:BridgedDeclContext.astContext(self:)")
BRIDGED_INLINE BridgedASTContext
BridgedDeclContext_getASTContext(BridgedDeclContext dc);

SWIFT_NAME("getter:BridgedDeclContext.parentSourceFile(self:)")
BRIDGED_INLINE BridgedSourceFile
BridgedDeclContext_getParentSourceFile(BridgedDeclContext dc);

SWIFT_NAME("getter:BridgedSourceFile.isScriptMode(self:)")
BRIDGED_INLINE bool BridgedSourceFile_isScriptMode(BridgedSourceFile sf);

SWIFT_NAME("BridgedPatternBindingInitializer.create(declContext:)")
BridgedPatternBindingInitializer
BridgedPatternBindingInitializer_create(BridgedDeclContext cDeclContext);

SWIFT_NAME("getter:BridgedPatternBindingInitializer.asDeclContext(self:)")
BridgedDeclContext BridgedPatternBindingInitializer_asDeclContext(
    BridgedPatternBindingInitializer cInit);

SWIFT_NAME("BridgedDefaultArgumentInitializer.create(declContext:index:)")
BridgedDefaultArgumentInitializer
BridgedDefaultArgumentInitializer_create(BridgedDeclContext cDeclContext,
                                         size_t index);

SWIFT_NAME("getter:BridgedDefaultArgumentInitializer.asDeclContext(self:)")
BridgedDeclContext DefaultArgumentInitializer_asDeclContext(
    BridgedDefaultArgumentInitializer cInit);

SWIFT_NAME("BridgedCustomAttributeInitializer.create(declContext:)")
BridgedCustomAttributeInitializer
BridgedCustomAttributeInitializer_create(BridgedDeclContext cDeclContext);

SWIFT_NAME("getter:BridgedCustomAttributeInitializer.asDeclContext(self:)")
BridgedDeclContext BridgedCustomAttributeInitializer_asDeclContext(
    BridgedCustomAttributeInitializer cInit);

SWIFT_NAME("getter:BridgedClosureExpr.asDeclContext(self:)")
BridgedDeclContext
BridgedClosureExpr_asDeclContext(BridgedClosureExpr cClosure);

//===----------------------------------------------------------------------===//
// MARK: Availability
//===----------------------------------------------------------------------===//

BRIDGED_OPTIONAL(swift::PlatformKind, PlatformKind)

SWIFT_NAME("BridgedOptionalPlatformKind.init(from:)")
BridgedOptionalPlatformKind PlatformKind_fromString(BridgedStringRef cStr);

SWIFT_NAME("BridgedOptionalPlatformKind.init(from:)")
BridgedOptionalPlatformKind
PlatformKind_fromIdentifier(swift::Identifier ident);

SWIFT_NAME("BridgedAvailabilityMacroMap.has(self:name:)")
bool BridgedAvailabilityMacroMap_hasName(BridgedAvailabilityMacroMap map,
                                         BridgedStringRef name);

SWIFT_NAME("BridgedAvailabilityMacroMap.has(self:name:version:)")
bool BridgedAvailabilityMacroMap_hasNameAndVersion(
    BridgedAvailabilityMacroMap map, BridgedStringRef name,
    BridgedVersionTuple version);

SWIFT_NAME("BridgedAvailabilityMacroMap.get(self:name:version:)")
BridgedArrayRef
BridgedAvailabilityMacroMap_getSpecs(BridgedAvailabilityMacroMap map,
                                     BridgedStringRef name,
                                     BridgedVersionTuple version);

struct BridgedAvailabilityMacroDefinition {
  BridgedStringRef name;
  BridgedVersionTuple version;
  BridgedArrayRef specs;
};

struct BridgedAvailabilityDomainOrIdentifier {
  void *_Nullable opaque;

  BridgedAvailabilityDomainOrIdentifier() : opaque(nullptr) {};
  BRIDGED_INLINE BridgedAvailabilityDomainOrIdentifier(
      swift::AvailabilityDomainOrIdentifier domain);
  BRIDGED_INLINE swift::AvailabilityDomainOrIdentifier unbridged() const;
};

SWIFT_NAME("getter:BridgedAvailabilityDomainOrIdentifier.isDomain(self:)")
BRIDGED_INLINE bool BridgedAvailabilityDomainOrIdentifier_isDomain(
    BridgedAvailabilityDomainOrIdentifier cVal);

SWIFT_NAME("getter:BridgedAvailabilityDomainOrIdentifier.asIdentifier(self:)")
BRIDGED_INLINE swift::Identifier
BridgedAvailabilityDomainOrIdentifier_getAsIdentifier(
    BridgedAvailabilityDomainOrIdentifier cVal);

SWIFT_NAME("BridgedAvailabilitySpec.createWildcard(_:loc:)")
BridgedAvailabilitySpec
BridgedAvailabilitySpec_createWildcard(BridgedASTContext cContext,
                                       swift::SourceLoc loc);

SWIFT_NAME(
    "BridgedAvailabilitySpec.createForDomainIdentifier(_:name:nameLoc:version:"
    "versionRange:)")
BridgedAvailabilitySpec BridgedAvailabilitySpec_createForDomainIdentifier(
    BridgedASTContext cContext, swift::Identifier name, swift::SourceLoc loc,
    BridgedVersionTuple cVersion, swift::SourceRange versionRange);

SWIFT_NAME("BridgedAvailabilitySpec.clone(self:_:)")
BridgedAvailabilitySpec
BridgedAvailabilitySpec_clone(BridgedAvailabilitySpec spec,
                              BridgedASTContext cContext);

SWIFT_NAME("BridgedAvailabilitySpec.setMacroLoc(self:_:)")
void BridgedAvailabilitySpec_setMacroLoc(BridgedAvailabilitySpec spec,
                                         swift::SourceLoc loc);

SWIFT_NAME("getter:BridgedAvailabilitySpec.domainOrIdentifier(self:)")
BridgedAvailabilityDomainOrIdentifier
BridgedAvailabilitySpec_getDomainOrIdentifier(BridgedAvailabilitySpec spec);

SWIFT_NAME("getter:BridgedAvailabilitySpec.sourceRange(self:)")
swift::SourceRange
BridgedAvailabilitySpec_getSourceRange(BridgedAvailabilitySpec spec);

SWIFT_NAME("getter:BridgedAvailabilitySpec.isWildcard(self:)")
bool BridgedAvailabilitySpec_isWildcard(BridgedAvailabilitySpec spec);

SWIFT_NAME("getter:BridgedAvailabilitySpec.rawVersion(self:)")
BridgedVersionTuple
BridgedAvailabilitySpec_getRawVersion(BridgedAvailabilitySpec spec);

SWIFT_NAME("getter:BridgedAvailabilitySpec.versionRange(self:)")
swift::SourceRange
BridgedAvailabilitySpec_getVersionRange(BridgedAvailabilitySpec spec);

//===----------------------------------------------------------------------===//
// MARK: AutoDiff
//===----------------------------------------------------------------------===//

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedDifferentiabilityKind {
  BridgedDifferentiabilityKindNonDifferentiable = 0,
  BridgedDifferentiabilityKindForward = 1,
  BridgedDifferentiabilityKindReverse = 2,
  BridgedDifferentiabilityKindNormal = 3,
  BridgedDifferentiabilityKindLinear = 4,
};

swift::DifferentiabilityKind unbridged(BridgedDifferentiabilityKind cKind);

class BridgedParsedAutoDiffParameter {
private:
  swift::SourceLoc loc;
  enum Kind {
    Named,
    Ordered,
    Self,
  } kind;
  union Value {
    swift::Identifier name;
    unsigned index;

    Value(swift::Identifier name) : name(name) {}
    Value(unsigned index) : index(index) {}
    Value() : name() {}
  } value;

  BridgedParsedAutoDiffParameter(swift::SourceLoc loc, Kind kind, Value value)
      : loc(loc), kind(kind), value(value) {}

public:
  SWIFT_NAME("forNamed(_:loc:)")
  static BridgedParsedAutoDiffParameter forNamed(swift::Identifier name,
                                                 swift::SourceLoc loc) {
    return BridgedParsedAutoDiffParameter(loc, Kind::Named, name);
  }
  SWIFT_NAME("forOrdered(_:loc:)")
  static BridgedParsedAutoDiffParameter forOrdered(size_t index,
                                                   swift::SourceLoc loc) {
    return BridgedParsedAutoDiffParameter(loc, Kind::Ordered, index);
  }
  SWIFT_NAME("forSelf(loc:)")
  static BridgedParsedAutoDiffParameter forSelf(swift::SourceLoc loc) {
    return BridgedParsedAutoDiffParameter(loc, Kind::Self, {});
  }

  swift::ParsedAutoDiffParameter unbridged() const;
};

//===----------------------------------------------------------------------===//
// MARK: DeclAttributes
//===----------------------------------------------------------------------===//

BRIDGED_OPTIONAL(swift::DeclAttrKind, DeclAttrKind)

SWIFT_NAME("BridgedOptionalDeclAttrKind.init(from:)")
BridgedOptionalDeclAttrKind
BridgedOptionalDeclAttrKind_fromString(BridgedStringRef cStr);

struct BridgedDeclAttributes {
  BridgedNullableDeclAttribute chain;

  BridgedDeclAttributes() : chain(nullptr) {};

  BRIDGED_INLINE BridgedDeclAttributes(swift::DeclAttributes attrs);

  BRIDGED_INLINE swift::DeclAttributes unbridged() const;
};

SWIFT_NAME("BridgedDeclAttribute.shouldBeRejectedByParser(_:)")
bool BridgedDeclAttribute_shouldBeRejectedByParser(swift::DeclAttrKind kind);

SWIFT_NAME("BridgedDeclAttribute.isDeclModifier(_:)")
bool BridgedDeclAttribute_isDeclModifier(swift::DeclAttrKind kind);

SWIFT_NAME("BridgedDeclAttributes.add(self:_:)")
void BridgedDeclAttributes_add(BridgedDeclAttributes *_Nonnull attrs,
                               BridgedDeclAttribute add);

SWIFT_NAME("BridgedDeclAttribute.createSimple(_:kind:atLoc:nameLoc:)")
BridgedDeclAttribute BridgedDeclAttribute_createSimple(
    BridgedASTContext cContext, swift::DeclAttrKind kind,
    swift::SourceLoc atLoc, swift::SourceLoc nameLoc);

SWIFT_NAME("BridgedABIAttr.createParsed(_:atLoc:range:abiDecl:)")
BridgedABIAttr BridgedABIAttr_createParsed(BridgedASTContext cContext,
                                           swift::SourceLoc atLoc,
                                           swift::SourceRange range,
                                           BridgedNullableDecl abiDecl);

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedAvailableAttrKind {
  BridgedAvailableAttrKindDefault,
  BridgedAvailableAttrKindDeprecated,
  BridgedAvailableAttrKindUnavailable,
  BridgedAvailableAttrKindNoAsync,
};

SWIFT_NAME("BridgedAvailableAttr.createParsed(_:atLoc:range:domainIdentifier:"
           "domainLoc:kind:message:renamed:introduced:introducedRange:"
           "deprecated:deprecatedRange:obsoleted:obsoletedRange:isSPI:)")
BridgedAvailableAttr BridgedAvailableAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, swift::Identifier domainIdentifier,
    swift::SourceLoc domainLoc, BridgedAvailableAttrKind cKind,
    BridgedStringRef cMessage, BridgedStringRef cRenamed,
    BridgedVersionTuple cIntroduced, swift::SourceRange introducedRange,
    BridgedVersionTuple cDeprecated, swift::SourceRange deprecatedRange,
    BridgedVersionTuple cObsoleted, swift::SourceRange obsoletedRange,
    bool isSPI);

SWIFT_NAME("BridgedAvailableAttr.createUnavailableInEmbedded(_:atLoc:range:)")
BridgedAvailableAttr
BridgedAvailableAttr_createUnavailableInEmbedded(BridgedASTContext cContext,
                                                 swift::SourceLoc atLoc,
                                                 swift::SourceRange range);

SWIFT_NAME("BridgedAvailableAttr.setIsGroupMember(self:)")
void BridgedAvailableAttr_setIsGroupMember(BridgedAvailableAttr cAttr);
SWIFT_NAME("BridgedAvailableAttr.setIsGroupedWithWildcard(self:)")
void BridgedAvailableAttr_setIsGroupedWithWildcard(BridgedAvailableAttr cAttr);
SWIFT_NAME("BridgedAvailableAttr.setIsGroupTerminator(self:)")
void BridgedAvailableAttr_setIsGroupTerminator(BridgedAvailableAttr cAttr);

BRIDGED_OPTIONAL(swift::AccessLevel, AccessLevel)

SWIFT_NAME("BridgedAccessControlAttr.createParsed(_:range:accessLevel:)")
BridgedAccessControlAttr
BridgedAccessControlAttr_createParsed(BridgedASTContext cContext,
                                      swift::SourceRange range,
                                      swift::AccessLevel accessLevel);

SWIFT_NAME("BridgedAlignmentAttr.createParsed(_:atLoc:range:value:)")
BridgedAlignmentAttr
BridgedAlignmentAttr_createParsed(BridgedASTContext cContext,
                                  swift::SourceLoc atLoc,
                                  swift::SourceRange range, size_t cValue);

SWIFT_NAME("BridgedAllowFeatureSuppressionAttr.createParsed(_:atLoc:range:inverted:features:)")
BridgedAllowFeatureSuppressionAttr
BridgedAllowFeatureSuppressionAttr_createParsed(BridgedASTContext cContext,
                                                swift::SourceLoc atLoc,
                                                swift::SourceRange range,
                                                bool inverted,
                                                BridgedArrayRef cFeatures);

SWIFT_NAME(
    "BridgedBackDeployedAttr.createParsed(_:atLoc:range:platform:version:)")
BridgedBackDeployedAttr BridgedBackDeployedAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, swift::PlatformKind platform,
    BridgedVersionTuple cVersion);

SWIFT_NAME("BridgedCDeclAttr.createParsed(_:atLoc:range:name:underscored:)")
BridgedCDeclAttr BridgedCDeclAttr_createParsed(BridgedASTContext cContext,
                                               swift::SourceLoc atLoc,
                                               swift::SourceRange range,
                                               BridgedStringRef cName,
                                               bool underscored);

SWIFT_NAME(
    "BridgedCustomAttr.createParsed(_:atLoc:type:initContext:argumentList:)")
BridgedCustomAttr BridgedCustomAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc, BridgedTypeRepr cType,
    BridgedNullableCustomAttributeInitializer cInitContext,
    BridgedNullableArgumentList cArgumentList);

SWIFT_NAME("BridgedDerivativeAttr.createParsed(_:atLoc:range:baseType:"
           "originalName:originalNameLoc:accessorKind:params:)")
BridgedDerivativeAttr BridgedDerivativeAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, BridgedNullableTypeRepr cBaseType,
    BridgedDeclNameRef cOriginalName, BridgedDeclNameLoc cOriginalNameLoc,
    swift::AccessorKind AccessorKind, BridgedArrayRef cParams);

SWIFT_NAME("BridgedDerivativeAttr.createParsed(_:atLoc:range:baseType:"
           "originalName:originalNameLoc:params:)")
BridgedDerivativeAttr BridgedDerivativeAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, BridgedNullableTypeRepr cBaseType,
    BridgedDeclNameRef cOriginalName, BridgedDeclNameLoc cOriginalNameLoc,
    BridgedArrayRef cParams);

SWIFT_NAME("BridgedDifferentiableAttr.createParsed(_:atLoc:range:kind:params:"
           "genericWhereClause:)")
BridgedDifferentiableAttr BridgedDifferentiableAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, BridgedDifferentiabilityKind cKind,
    BridgedArrayRef cParams,
    BridgedNullableTrailingWhereClause cGenericWhereClause);

SWIFT_NAME("BridgedDocumentationAttr.createParsed(_:atLoc:range:metadata:"
           "accessLevel:)")
BridgedDocumentationAttr BridgedDocumentationAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, BridgedStringRef cMetadata,
    BridgedOptionalAccessLevel accessLevel);

SWIFT_NAME(
    "BridgedDynamicReplacementAttr.createParsed(_:atLoc:attrNameLoc:lParenLoc:"
    "replacedFunction:rParenLoc:)")
BridgedDynamicReplacementAttr BridgedDynamicReplacementAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceLoc attrNameLoc, swift::SourceLoc lParenLoc,
    BridgedDeclNameRef cReplacedFunction, swift::SourceLoc rParenLoc);

SWIFT_NAME("BridgedEffectsAttr.createParsed(_:atLoc:range:effectKind:)")
BridgedEffectsAttr BridgedEffectsAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, swift::EffectsKind effectKind);

SWIFT_NAME("BridgedEffectsAttr.createParsed(_:atLoc:range:customString:"
           "customStringLoc:)")
BridgedEffectsAttr BridgedEffectsAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, BridgedStringRef cCustomString,
    swift::SourceLoc customStringLoc);

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedExclusivityAttrMode {
  BridgedExclusivityAttrModeChecked,
  BridgedExclusivityAttrModeUnchecked,
};

SWIFT_NAME("BridgedExclusivityAttr.createParsed(_:atLoc:range:mode:)")
BridgedExclusivityAttr BridgedExclusivityAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, BridgedExclusivityAttrMode cMode);

SWIFT_NAME("BridgedExposeAttr.createParsed(_:atLoc:range:name:kind:)")
BridgedExposeAttr BridgedExposeAttr_createParsed(BridgedASTContext cContext,
                                                 swift::SourceLoc atLoc,
                                                 swift::SourceRange range,
                                                 BridgedStringRef cName,
                                                 swift::ExposureKind kind);

SWIFT_NAME("BridgedExternAttr.createParsed(_:atLoc:range:lParenLoc:rParenLoc:"
           "kind:moduleName:name:)")
BridgedExternAttr BridgedExternAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, swift::SourceLoc lParenLoc,
    swift::SourceLoc rParenLoc, swift::ExternKind kind,
    BridgedStringRef cModuleName, BridgedStringRef cName);

SWIFT_NAME("BridgedImplementsAttr.createParsed(_:atLoc:range:protocolType:"
           "memberName:memberNameLoc:)")
BridgedImplementsAttr BridgedImplementsAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, BridgedTypeRepr cProtocolType,
    BridgedDeclNameRef cMemberName, BridgedDeclNameLoc cMemberNameLoc);

SWIFT_NAME("BridgedInlineAttr.createParsed(_:atLoc:range:kind:)")
BridgedInlineAttr BridgedInlineAttr_createParsed(BridgedASTContext cContext,
                                                 swift::SourceLoc atLoc,
                                                 swift::SourceRange range,
                                                 swift::InlineKind kind);

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedParsedLifetimeDependenceKind {
  BridgedParsedLifetimeDependenceKindDefault,
  BridgedParsedLifetimeDependenceKindBorrow,
  BridgedParsedLifetimeDependenceKindInherit,
  BridgedParsedLifetimeDependenceKindInout
};

class BridgedLifetimeDescriptor {
  union Value {
    swift::Identifier name;
    unsigned index;

    Value(swift::Identifier name) : name(name) {}
    Value(unsigned index) : index(index) {}
    Value() : name() {}
  } value;

  enum DescriptorKind {
    Named,
    Ordered,
    Self,
  } kind;

  BridgedParsedLifetimeDependenceKind dependenceKind;
  swift::SourceLoc loc;

  BridgedLifetimeDescriptor(Value value, DescriptorKind kind,
                            BridgedParsedLifetimeDependenceKind dependenceKind,
                            swift::SourceLoc loc)
      : value(value), kind(kind), dependenceKind(dependenceKind), loc(loc) {}

public:
  SWIFT_NAME("forNamed(_:dependenceKind:loc:)")
  static BridgedLifetimeDescriptor
  forNamed(swift::Identifier name,
           BridgedParsedLifetimeDependenceKind dependenceKind,
           swift::SourceLoc loc) {
    return BridgedLifetimeDescriptor(name, DescriptorKind::Named,
                                     dependenceKind, loc);
  }
  SWIFT_NAME("forOrdered(_:dependenceKind:loc:)")
  static BridgedLifetimeDescriptor
  forOrdered(size_t index, BridgedParsedLifetimeDependenceKind dependenceKind,
             swift::SourceLoc loc) {
    return BridgedLifetimeDescriptor(index, DescriptorKind::Ordered,
                                     dependenceKind, loc);
  }
  SWIFT_NAME("forSelf(dependenceKind:loc:)")
  static BridgedLifetimeDescriptor
  forSelf(BridgedParsedLifetimeDependenceKind dependenceKind,
          swift::SourceLoc loc) {
    return BridgedLifetimeDescriptor({}, DescriptorKind::Self, dependenceKind,
                                     loc);
  }

  swift::LifetimeDescriptor unbridged();
};

SWIFT_NAME("BridgedLifetimeEntry.createParsed(_:range:sources:)")
BridgedLifetimeEntry
BridgedLifetimeEntry_createParsed(BridgedASTContext cContext,
                                  swift::SourceRange range,
                                  BridgedArrayRef cSources);

SWIFT_NAME("BridgedLifetimeEntry.createParsed(_:range:sources:target:)")
BridgedLifetimeEntry BridgedLifetimeEntry_createParsed(
    BridgedASTContext cContext, swift::SourceRange range,
    BridgedArrayRef cSources, BridgedLifetimeDescriptor cTarget);

SWIFT_NAME(
    "BridgedLifetimeAttr.createParsed(_:atLoc:range:entry:isUnderscored:)")
BridgedLifetimeAttr BridgedLifetimeAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, BridgedLifetimeEntry cEntry, bool isUnderscored);

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedMacroSyntax {
  BridgedMacroSyntaxFreestanding,
  BridgedMacroSyntaxAttached,
};

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedMacroIntroducedDeclNameKind {
  BridgedMacroIntroducedDeclNameKindNamed,
  BridgedMacroIntroducedDeclNameKindOverloaded,
  BridgedMacroIntroducedDeclNameKindPrefixed,
  BridgedMacroIntroducedDeclNameKindSuffixed,
  BridgedMacroIntroducedDeclNameKindArbitrary,
};

BRIDGED_INLINE swift::MacroIntroducedDeclNameKind
unbridge(BridgedMacroIntroducedDeclNameKind kind);

struct BridgedMacroIntroducedDeclName {
  BridgedMacroIntroducedDeclNameKind kind;
  BridgedDeclNameRef name;

  BRIDGED_INLINE swift::MacroIntroducedDeclName unbridged() const;
};

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedMacroRole {
#define MACRO_ROLE(Name, Description) BridgedMacroRole##Name,
#include "swift/Basic/MacroRoles.def"
  BridgedMacroRoleNone,
};

BRIDGED_INLINE swift::MacroRole unbridge(BridgedMacroRole cRole);

SWIFT_NAME("BridgedMacroRole.init(from:)")
BridgedMacroRole BridgedMacroRole_fromString(BridgedStringRef str);

SWIFT_NAME("getter:BridgedMacroRole.isAttached(self:)")
BRIDGED_INLINE bool BridgedMacroRole_isAttached(BridgedMacroRole role);

SWIFT_NAME("BridgedMacroRoleAttr.createParsed(_:atLoc:range:syntax:lParenLoc:"
           "role:names:conformances:rParenLoc:)")
BridgedMacroRoleAttr BridgedMacroRoleAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, BridgedMacroSyntax cSyntax,
    swift::SourceLoc lParenLoc, BridgedMacroRole cRole, BridgedArrayRef cNames,
    BridgedArrayRef cConformances, swift::SourceLoc rParenLoc);

SWIFT_NAME("BridgedOriginallyDefinedInAttr.createParsed(_:atLoc:range:"
           "moduleName:platform:version:)")
BridgedOriginallyDefinedInAttr BridgedOriginallyDefinedInAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, BridgedStringRef cModuleName,
    swift::PlatformKind platform, BridgedVersionTuple cVersion);

SWIFT_NAME("BridgedStorageRestrictionsAttr.createParsed(_:atLoc:range:"
           "initializes:accesses:)")
BridgedStorageRestrictionsAttr BridgedStorageRestrictionsAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, BridgedArrayRef cInitializes,
    BridgedArrayRef cAccesses);

SWIFT_NAME(
    "BridgedSwiftNativeObjCRuntimeBaseAttr.createParsed(_:atLoc:range:name:)")
BridgedSwiftNativeObjCRuntimeBaseAttr
BridgedSwiftNativeObjCRuntimeBaseAttr_createParsed(BridgedASTContext cContext,
                                                   swift::SourceLoc atLoc,
                                                   swift::SourceRange range,
                                                   swift::Identifier name);

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedNonSendableKind {
  BridgedNonSendableKindSpecific,
  BridgedNonSendableKindAssumed,
};

SWIFT_NAME("BridgedNonSendableAttr.createParsed(_:atLoc:range:kind:)")
BridgedNonSendableAttr BridgedNonSendableAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, BridgedNonSendableKind cKind);

SWIFT_NAME("BridgedNonisolatedAttr.createParsed(_:atLoc:range:modifier:)")
BridgedNonisolatedAttr BridgedNonisolatedAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, swift::NonIsolatedModifier modifier);

SWIFT_NAME("BridgedInheritActorContextAttr.createParsed(_:atLoc:range:modifier:)")
BridgedInheritActorContextAttr BridgedInheritActorContextAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, swift::InheritActorContextModifier modifier);

SWIFT_NAME("BridgedObjCAttr.createParsedUnnamed(_:atLoc:attrNameLoc:)")
BridgedObjCAttr
BridgedObjCAttr_createParsedUnnamed(BridgedASTContext cContext,
                                    swift::SourceLoc atLoc,
                                    swift::SourceLoc attrNameLoc);

SWIFT_NAME("BridgedObjCAttr.createParsedNullary(_:atLoc:attrNameLoc:lParenLoc:"
           "nameLoc:name:rParenLoc:)")
BridgedObjCAttr BridgedObjCAttr_createParsedNullary(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceLoc attrNameLoc, swift::SourceLoc lParenLoc,
    swift::SourceLoc nameLoc, swift::Identifier name,
    swift::SourceLoc rParenLoc);

SWIFT_NAME("BridgedObjCAttr.createParsedSelector(_:atLoc:attrNameLoc:lParenLoc:"
           "nameLocs:names:rParenLoc:)")
BridgedObjCAttr BridgedObjCAttr_createParsedSelector(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceLoc attrNameLoc, swift::SourceLoc lParenLoc,
    BridgedArrayRef cNameLocs, BridgedArrayRef cNames,
    swift::SourceLoc rParenLoc);

SWIFT_NAME("BridgedObjCImplementationAttr.createParsed(_:atLoc:range:name:isEarlyAdopter:)")
BridgedObjCImplementationAttr BridgedObjCImplementationAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, swift::Identifier name, bool isEarlyAdopter);

SWIFT_NAME("BridgedObjCRuntimeNameAttr.createParsed(_:atLoc:range:name:)")
BridgedObjCRuntimeNameAttr BridgedObjCRuntimeNameAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, swift::Identifier name);

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedOptimizationMode {
  BridgedOptimizationModeForSpeed,
  BridgedOptimizationModeForSize,
  BridgedOptimizationModeNoOptimization,
};

SWIFT_NAME("BridgedOptimizeAttr.createParsed(_:atLoc:range:mode:)")
BridgedOptimizeAttr BridgedOptimizeAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, BridgedOptimizationMode cMode);

SWIFT_NAME("BridgedPrivateImportAttr.createParsed(_:atLoc:attrNameLoc:"
           "lParenLoc:fileName:rParenLoc:)")
BridgedPrivateImportAttr BridgedPrivateImportAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceLoc attrNameLoc, swift::SourceLoc lParenLoc,
    BridgedStringRef cFileName, swift::SourceLoc rParenLoc);

SWIFT_NAME(
    "BridgedProjectedValuePropertyAttr.createParsed(_:atLoc:range:name:)")
BridgedProjectedValuePropertyAttr
BridgedProjectedValuePropertyAttr_createParsed(BridgedASTContext cContext,
                                               swift::SourceLoc atLoc,
                                               swift::SourceRange range,
                                               swift::Identifier name);

SWIFT_NAME("BridgedRawDocCommentAttr.createParsed(_:range:)")
BridgedRawDocCommentAttr
BridgedRawDocCommentAttr_createParsed(BridgedASTContext cContext,
                                      swift::CharSourceRange range);

SWIFT_NAME("BridgedRawLayoutAttr.createParsed(_:atLoc:range:size:alignment:)")
BridgedRawLayoutAttr BridgedStorageRestrictionsAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, size_t size, size_t alignment);

SWIFT_NAME("BridgedRawLayoutAttr.createParsed(_:atLoc:range:like:moveAsLike:)")
BridgedRawLayoutAttr BridgedStorageRestrictionsAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, BridgedTypeRepr cLikeType, bool moveAsLike);

SWIFT_NAME("BridgedRawLayoutAttr.createParsed(_:atLoc:range:likeArrayOf:count:"
           "moveAsLike:)")
BridgedRawLayoutAttr BridgedStorageRestrictionsAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, BridgedTypeRepr cLikeType,
    BridgedTypeRepr cCountType, bool moveAsLike);

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedReferenceOwnership {
  BridgedReferenceOwnershipStrong,
  BridgedReferenceOwnershipWeak,
  BridgedReferenceOwnershipUnowned,
  BridgedReferenceOwnershipUnmanaged,
};

swift::ReferenceOwnership unbridged(BridgedReferenceOwnership kind);

SWIFT_NAME("BridgedReferenceOwnershipAttr.createParsed(_:atLoc:range:kind:)")
BridgedReferenceOwnershipAttr BridgedReferenceOwnershipAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, BridgedReferenceOwnership cKind);

SWIFT_NAME("BridgedSectionAttr.createParsed(_:atLoc:range:name:)")
BridgedSectionAttr BridgedSectionAttr_createParsed(BridgedASTContext cContext,
                                                   swift::SourceLoc atLoc,
                                                   swift::SourceRange range,
                                                   BridgedStringRef cName);

SWIFT_NAME("BridgedSemanticsAttr.createParsed(_:atLoc:range:value:)")
BridgedSemanticsAttr BridgedSemanticsAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, BridgedStringRef cValue);

SWIFT_NAME("BridgedSetterAccessAttr.createParsed(_:range:accessLevel:)")
BridgedSetterAccessAttr
BridgedSetterAccessAttr_createParsed(BridgedASTContext cContext,
                                     swift::SourceRange range,
                                     swift::AccessLevel accessLevel);

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedSpecializationKind : uint8_t {
  BridgedSpecializationKindFull,
  BridgedSpecializationKindPartial,
};

SWIFT_NAME("BridgedSpecializeAttr.createParsed(_:atLoc:range:whereClause:"
           "exported:kind:taretFunction:spiGroups:availableAttrs:)")
BridgedSpecializeAttr BridgedSpecializeAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, BridgedNullableTrailingWhereClause cWhereClause,
    bool exported, BridgedSpecializationKind cKind,
    BridgedDeclNameRef cTargetFunction, BridgedArrayRef cSPIGroups,
    BridgedArrayRef cAvailableAttrs);

SWIFT_NAME("BridgedSpecializedAttr.createParsed(_:atLoc:range:whereClause:"
           "exported:kind:taretFunction:spiGroups:availableAttrs:)")
BridgedSpecializedAttr BridgedSpecializedAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, BridgedNullableTrailingWhereClause cWhereClause,
    bool exported, BridgedSpecializationKind cKind,
    BridgedDeclNameRef cTargetFunction, BridgedArrayRef cSPIGroups,
    BridgedArrayRef cAvailableAttrs);

SWIFT_NAME(
    "BridgedSPIAccessControlAttr.createParsed(_:atLoc:range:spiGroupName:)")
BridgedSPIAccessControlAttr BridgedSPIAccessControlAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, swift::Identifier SPIGroupName);

SWIFT_NAME("BridgedSILGenNameAttr.createParsed(_:atLoc:range:name:isRaw:)")
BridgedSILGenNameAttr BridgedSILGenNameAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, BridgedStringRef cName, bool isRaw);

SWIFT_NAME(
    "BridgedTransposeAttr.createParsed(_:atLoc:range:baseType:originalName:"
    "originalNameLoc:params:)")
BridgedTransposeAttr BridgedTransposeAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, BridgedNullableTypeRepr cBaseType,
    BridgedDeclNameRef cOriginalName, BridgedDeclNameLoc cOriginalNameLoc,
    BridgedArrayRef cParams);

SWIFT_NAME("BridgedTypeEraserAttr.createParsed(_:atLoc:range:typeExpr:)")
BridgedTypeEraserAttr BridgedTypeEraserAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, BridgedTypeExpr cTypeExpr);

SWIFT_NAME(
    "BridgedUnavailableFromAsyncAttr.createParsed(_:atLoc:range:message:)")
BridgedUnavailableFromAsyncAttr BridgedUnavailableFromAsyncAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceRange range, BridgedStringRef cMessage);

//===----------------------------------------------------------------------===//
// MARK: Decls
//===----------------------------------------------------------------------===//

struct BridgedFingerprint;

SWIFT_NAME("BridgedDecl.attachParsedAttrs(self:_:)")
void BridgedDecl_attachParsedAttrs(BridgedDecl decl, BridgedDeclAttributes attrs);

SWIFT_NAME("BridgedDecl.forEachDeclToHoist(self:_:)")
void BridgedDecl_forEachDeclToHoist(BridgedDecl decl,
                                    BridgedSwiftClosure closure);

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedStaticSpelling {
  BridgedStaticSpellingNone,
  BridgedStaticSpellingStatic,
  BridgedStaticSpellingClass
};

struct BridgedAccessorRecord {
  swift::SourceLoc lBraceLoc;
  BridgedArrayRef accessors;
  swift::SourceLoc rBraceLoc;
};

SWIFT_NAME("BridgedAccessorDecl.createParsed(_:declContext:kind:storage:"
           "declLoc:accessorKeywordLoc:parameterList:asyncSpecifierLoc:"
           "throwsSpecifierLoc:thrownType:)")
BridgedAccessorDecl BridgedAccessorDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    swift::AccessorKind Kind, BridgedAbstractStorageDecl cStorage,
    swift::SourceLoc declLoc, swift::SourceLoc accessorKeywordLoc,
    BridgedNullableParameterList cParamList, swift::SourceLoc asyncLoc,
    swift::SourceLoc throwsLoc, BridgedNullableTypeRepr cThrownType);

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedVarDeclIntroducer {
  BridgedVarDeclIntroducerLet = 0,
  BridgedVarDeclIntroducerVar = 1,
  BridgedVarDeclIntroducerInOut = 2,
  BridgedVarDeclIntroducerBorrowing = 3,
};

SWIFT_NAME("BridgedPatternBindingDecl.createParsed(_:declContext:attributes:"
           "staticLoc:staticSpelling:introducerLoc:introducer:entries:)")
BridgedPatternBindingDecl BridgedPatternBindingDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    BridgedDeclAttributes cAttrs, swift::SourceLoc staticLoc,
    BridgedStaticSpelling cStaticSpelling, swift::SourceLoc introducerLoc,
    BridgedVarDeclIntroducer cIntorducer, BridgedArrayRef cBindingEntries);

SWIFT_NAME("BridgedParamDecl.createParsed(_:declContext:specifierLoc:argName:"
           "argNameLoc:paramName:paramNameLoc:defaultValue:"
           "defaultValueInitContext:)")
BridgedParamDecl BridgedParamDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    swift::SourceLoc specifierLoc, swift::Identifier argName,
    swift::SourceLoc argNameLoc, swift::Identifier paramName,
    swift::SourceLoc paramNameLoc, BridgedNullableExpr defaultValue,
    BridgedNullableDefaultArgumentInitializer cDefaultArgumentInitContext);

SWIFT_NAME("BridgedParamDecl.setTypeRepr(self:_:)")
BRIDGED_INLINE void BridgedParamDecl_setTypeRepr(BridgedParamDecl cDecl,
                                                 BridgedTypeRepr cType);

/// The various spellings of ownership modifier that can be used in source.
enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedParamSpecifier {
  BridgedParamSpecifierDefault,
  BridgedParamSpecifierInOut,
  BridgedParamSpecifierBorrowing,
  BridgedParamSpecifierConsuming,
  BridgedParamSpecifierLegacyShared,
  BridgedParamSpecifierLegacyOwned,
  BridgedParamSpecifierImplicitlyCopyableConsuming,
};

BRIDGED_INLINE swift::ParamSpecifier unbridge(BridgedParamSpecifier kind);

SWIFT_NAME("BridgedParamDecl.setSpecifier(self:_:)")
BRIDGED_INLINE void
BridgedParamDecl_setSpecifier(BridgedParamDecl cDecl,
                              BridgedParamSpecifier cSpecifier);

SWIFT_NAME("BridgedParamDecl.setImplicit(self:)")
BRIDGED_INLINE void BridgedParamDecl_setImplicit(BridgedParamDecl cDecl);

SWIFT_NAME("BridgedConstructorDecl.setParsedBody(self:_:)")
void BridgedConstructorDecl_setParsedBody(BridgedConstructorDecl decl,
                                          BridgedBraceStmt body);

SWIFT_NAME("BridgedFuncDecl.setParsedBody(self:_:)")
void BridgedFuncDecl_setParsedBody(BridgedFuncDecl decl, BridgedBraceStmt body);

SWIFT_NAME("BridgedDestructorDecl.setParsedBody(self:_:)")
void BridgedDestructorDecl_setParsedBody(BridgedDestructorDecl decl,
                                         BridgedBraceStmt body);

SWIFT_NAME("BridgedFuncDecl.createParsed(_:declContext:staticLoc:"
           "staticSpelling:funcKeywordLoc:"
           "name:nameLoc:genericParamList:parameterList:asyncSpecifierLoc:"
           "throwsSpecifierLoc:thrownType:returnType:genericWhereClause:)")
BridgedFuncDecl BridgedFuncDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    swift::SourceLoc staticLoc, BridgedStaticSpelling cStaticSpelling,
    swift::SourceLoc funcKeywordLoc, swift::Identifier name,
    swift::SourceLoc nameLoc, BridgedNullableGenericParamList genericParamList,
    BridgedParameterList parameterList, swift::SourceLoc asyncLoc,
    swift::SourceLoc throwsLoc, BridgedNullableTypeRepr thrownType,
    BridgedNullableTypeRepr returnType,
    BridgedNullableTrailingWhereClause opaqueGenericWhereClause);

SWIFT_NAME(
    "BridgedConstructorDecl.createParsed(_:declContext:initKeywordLoc:"
    "failabilityMarkLoc:isIUO:genericParamList:parameterList:"
    "asyncSpecifierLoc:throwsSpecifierLoc:thrownType:genericWhereClause:)")
BridgedConstructorDecl BridgedConstructorDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    swift::SourceLoc initKeywordLoc, swift::SourceLoc failabilityMarkLoc,
    bool isIUO, BridgedNullableGenericParamList genericParams,
    BridgedParameterList parameterList, swift::SourceLoc asyncLoc,
    swift::SourceLoc throwsLoc, BridgedNullableTypeRepr thrownType,
    BridgedNullableTrailingWhereClause genericWhereClause);

SWIFT_NAME(
    "BridgedDestructorDecl.createParsed(_:declContext:deinitKeywordLoc:)")
BridgedDestructorDecl
BridgedDestructorDecl_createParsed(BridgedASTContext cContext,
                                   BridgedDeclContext cDeclContext,
                                   swift::SourceLoc deinitKeywordLoc);

SWIFT_NAME(
    "BridgedTypeAliasDecl.createParsed(_:declContext:typealiasKeywordLoc:name:"
    "nameLoc:genericParamList:equalLoc:underlyingType:genericWhereClause:)")
BridgedTypeAliasDecl BridgedTypeAliasDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    swift::SourceLoc aliasKeywordLoc, swift::Identifier name,
    swift::SourceLoc nameLoc, BridgedNullableGenericParamList genericParamList,
    swift::SourceLoc equalLoc, BridgedTypeRepr underlyingType,
    BridgedNullableTrailingWhereClause genericWhereClause);

SWIFT_NAME("BridgedExtensionDecl.setParsedMembers(self:_:fingerprint:)")
void BridgedExtensionDecl_setParsedMembers(BridgedExtensionDecl decl,
                                           BridgedArrayRef members,
                                           BridgedFingerprint fingerprint);

SWIFT_NAME(
    "BridgedEnumDecl.createParsed(_:declContext:enumKeywordLoc:name:nameLoc:"
    "genericParamList:inheritedTypes:genericWhereClause:braceRange:)")
BridgedNominalTypeDecl BridgedEnumDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    swift::SourceLoc enumKeywordLoc, swift::Identifier name,
    swift::SourceLoc nameLoc, BridgedNullableGenericParamList genericParamList,
    BridgedArrayRef cInheritedTypes,
    BridgedNullableTrailingWhereClause genericWhereClause,
    swift::SourceRange braceRange);

SWIFT_NAME(
    "BridgedEnumCaseDecl.createParsed(declContext:caseKeywordLoc:elements:)")
BridgedEnumCaseDecl
BridgedEnumCaseDecl_createParsed(BridgedDeclContext cDeclContext,
                                 swift::SourceLoc caseKeywordLoc,
                                 BridgedArrayRef cElements);

SWIFT_NAME("BridgedEnumElementDecl.createParsed(_:declContext:name:nameLoc:"
           "parameterList:equalsLoc:rawValue:)")
BridgedEnumElementDecl BridgedEnumElementDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    swift::Identifier name, swift::SourceLoc nameLoc,
    BridgedNullableParameterList parameterList, swift::SourceLoc equalsLoc,
    BridgedNullableExpr opaqueRawValue);

SWIFT_NAME("BridgedStructDecl.createParsed(_:declContext:structKeywordLoc:name:"
           "nameLoc:genericParamList:inheritedTypes:genericWhereClause:"
           "braceRange:)")
BridgedNominalTypeDecl BridgedStructDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    swift::SourceLoc structKeywordLoc, swift::Identifier name,
    swift::SourceLoc nameLoc, BridgedNullableGenericParamList genericParamList,
    BridgedArrayRef cInheritedTypes,
    BridgedNullableTrailingWhereClause genericWhereClause,
    swift::SourceRange braceRange);

SWIFT_NAME(
    "BridgedClassDecl.createParsed(_:declContext:classKeywordLoc:name:nameLoc:"
    "genericParamList:inheritedTypes:genericWhereClause:braceRange:isActor:)")
BridgedNominalTypeDecl BridgedClassDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    swift::SourceLoc classKeywordLoc, swift::Identifier name,
    swift::SourceLoc nameLoc, BridgedNullableGenericParamList genericParamList,
    BridgedArrayRef cInheritedTypes,
    BridgedNullableTrailingWhereClause genericWhereClause,
    swift::SourceRange braceRange, bool isActor);

SWIFT_NAME(
    "BridgedProtocolDecl.createParsed(_:declContext:protocolKeywordLoc:name:"
    "nameLoc:primaryAssociatedTypeNames:inheritedTypes:"
    "genericWhereClause:braceRange:)")
BridgedNominalTypeDecl BridgedProtocolDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    swift::SourceLoc protocolKeywordLoc, swift::Identifier name,
    swift::SourceLoc nameLoc, BridgedArrayRef cPrimaryAssociatedTypeNames,
    BridgedArrayRef cInheritedTypes,
    BridgedNullableTrailingWhereClause genericWhereClause,
    swift::SourceRange braceRange);

SWIFT_NAME("BridgedAssociatedTypeDecl.createParsed(_:declContext:"
           "associatedtypeKeywordLoc:name:nameLoc:inheritedTypes:defaultType:"
           "genericWhereClause:)")
BridgedAssociatedTypeDecl BridgedAssociatedTypeDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    swift::SourceLoc associatedtypeKeywordLoc, swift::Identifier name,
    swift::SourceLoc nameLoc, BridgedArrayRef cInheritedTypes,
    BridgedNullableTypeRepr opaqueDefaultType,
    BridgedNullableTrailingWhereClause genericWhereClause);

SWIFT_NAME(
    "BridgedMacroDecl.createParsed(_:declContext:macroKeywordLoc:name:nameLoc:"
    "genericParamList:paramList:arrowLoc:resultType:definition:"
    "genericWhereClause:)")
BridgedMacroDecl BridgedMacroDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    swift::SourceLoc macroLoc, swift::Identifier name, swift::SourceLoc nameLoc,
    BridgedNullableGenericParamList cGenericParams,
    BridgedParameterList cParams, swift::SourceLoc arrowLoc,
    BridgedNullableTypeRepr cResultType, BridgedNullableExpr cDefinition,
    BridgedNullableTrailingWhereClause genericWhereClause);

SWIFT_NAME("BridgedMacroExpansionDecl.createParsed(_:poundLoc:macroNameRef:"
           "macroNameLoc:leftAngleLoc:genericArgs:rightAngleLoc:args:)")
BridgedMacroExpansionDecl BridgedMacroExpansionDecl_createParsed(
    BridgedDeclContext cDeclContext, swift::SourceLoc poundLoc,
    BridgedDeclNameRef cMacroNameRef, BridgedDeclNameLoc cMacroNameLoc,
    swift::SourceLoc leftAngleLoc, BridgedArrayRef cGenericArgs,
    swift::SourceLoc rightAngleLoc, BridgedNullableArgumentList cArgList);

SWIFT_NAME(
    "BridgedExtensionDecl.createParsed(_:declContext:extensionKeywordLoc:"
    "extendedType:inheritedTypes:genericWhereClause:braceRange:)")
BridgedExtensionDecl BridgedExtensionDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    swift::SourceLoc extensionKeywordLoc, BridgedTypeRepr opaqueExtendedType,
    BridgedArrayRef cInheritedTypes,
    BridgedNullableTrailingWhereClause genericWhereClause,
    swift::SourceRange braceRange);

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedOperatorFixity {
  BridgedOperatorFixityInfix,
  BridgedOperatorFixityPrefix,
  BridgedOperatorFixityPostfix,
};

SWIFT_NAME("BridgedMissingDecl.create(_:declContext:loc:)")
BridgedMissingDecl BridgedMissingDecl_create(BridgedASTContext cContext,
                                             BridgedDeclContext cDeclContext,
                                             swift::SourceLoc loc);

SWIFT_NAME("BridgedOperatorDecl.createParsed(_:declContext:fixity:"
           "operatorKeywordLoc:name:nameLoc:colonLoc:precedenceGroupName:"
           "precedenceGroupLoc:)")
BridgedOperatorDecl BridgedOperatorDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    BridgedOperatorFixity cFixity, swift::SourceLoc operatorKeywordLoc,
    swift::Identifier name, swift::SourceLoc nameLoc, swift::SourceLoc colonLoc,
    swift::Identifier precedenceGroupName, swift::SourceLoc precedenceGroupLoc);

SWIFT_NAME("BridgedPrecedenceGroupDecl.createParsed(declContext:"
           "precedencegroupKeywordLoc:name:nameLoc:leftBraceLoc:"
           "associativityLabelLoc:associativityValueLoc:associativity:"
           "assignmentLabelLoc:assignmentValueLoc:isAssignment:"
           "higherThanKeywordLoc:higherThanNames:lowerThanKeywordLoc:"
           "lowerThanNames:rightBraceLoc:)")
BridgedPrecedenceGroupDecl BridgedPrecedenceGroupDecl_createParsed(
    BridgedDeclContext cDeclContext, swift::SourceLoc precedencegroupKeywordLoc,
    swift::Identifier name, swift::SourceLoc nameLoc,
    swift::SourceLoc leftBraceLoc, swift::SourceLoc associativityKeywordLoc,
    swift::SourceLoc associativityValueLoc, swift::Associativity associativity,
    swift::SourceLoc assignmentKeywordLoc, swift::SourceLoc assignmentValueLoc,
    bool isAssignment, swift::SourceLoc higherThanKeywordLoc,
    BridgedArrayRef cHigherThanNames, swift::SourceLoc lowerThanKeywordLoc,
    BridgedArrayRef cLowerThanNames, swift::SourceLoc rightBraceLoc);

enum ENUM_EXTENSIBILITY_ATTR(open) BridgedImportKind {
  BridgedImportKindModule,
  BridgedImportKindType,
  BridgedImportKindStruct,
  BridgedImportKindClass,
  BridgedImportKindEnum,
  BridgedImportKindProtocol,
  BridgedImportKindVar,
  BridgedImportKindFunc,
};

SWIFT_NAME("BridgedImportDecl.createParsed(_:declContext:importKeywordLoc:"
           "importKind:importKindLoc:path:)")
BridgedImportDecl BridgedImportDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    swift::SourceLoc importKeywordLoc, BridgedImportKind cImportKind,
    swift::SourceLoc importKindLoc, BridgedArrayRef cImportPathElements);

enum ENUM_EXTENSIBILITY_ATTR(open) BridgedUsingSpecifier {
  BridgedUsingSpecifierMainActor,
  BridgedUsingSpecifierNonisolated,
};

SWIFT_NAME("BridgedUsingDecl.createParsed(_:declContext:usingKeywordLoc:"
           "specifierLoc:specifier:)")
BridgedUsingDecl BridgedUsingDecl_createParsed(BridgedASTContext cContext,
                                               BridgedDeclContext cDeclContext,
                                               swift::SourceLoc usingKeywordLoc,
                                               swift::SourceLoc specifierLoc,
                                               BridgedUsingSpecifier specifier);

SWIFT_NAME("BridgedSubscriptDecl.createParsed(_:declContext:staticLoc:"
           "staticSpelling:subscriptKeywordLoc:genericParamList:parameterList:"
           "arrowLoc:returnType:genericWhereClause:)")
BridgedSubscriptDecl BridgedSubscriptDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    swift::SourceLoc staticLoc, BridgedStaticSpelling cStaticSpelling,
    swift::SourceLoc subscriptKeywordLoc,
    BridgedNullableGenericParamList cGenericParamList,
    BridgedParameterList cParamList, swift::SourceLoc arrowLoc,
    BridgedTypeRepr returnType,
    BridgedNullableTrailingWhereClause genericWhereClause);

SWIFT_NAME("BridgedTopLevelCodeDecl.create(_:declContext:)")
BridgedTopLevelCodeDecl
BridgedTopLevelCodeDecl_create(BridgedASTContext cContext,
                               BridgedDeclContext cDeclContext);

SWIFT_NAME("BridgedTopLevelCodeDecl.setBody(self:body:)")
void BridgedTopLevelCodeDecl_setBody(BridgedTopLevelCodeDecl cDecl,
                                     BridgedBraceStmt cBody);

SWIFT_NAME("BridgedTopLevelCodeDecl.dump(self:)")
void BridgedTopLevelCodeDecl_dump(BridgedTopLevelCodeDecl decl);

SWIFT_NAME("BridgedDecl.dump(self:)")
void BridgedDecl_dump(BridgedDecl decl);

//===----------------------------------------------------------------------===//
// MARK: AbstractStorageDecl
//===----------------------------------------------------------------------===//

SWIFT_NAME("BridgedAbstractStorageDecl.setAccessors(self:_:)")
void BridgedAbstractStorageDecl_setAccessors(
    BridgedAbstractStorageDecl cStorage, BridgedAccessorRecord accessors);

//===----------------------------------------------------------------------===//
// MARK: AccessorDecl
//===----------------------------------------------------------------------===//

SWIFT_NAME("BridgedAccessorDecl.setParsedBody(self:_:)")
void BridgedAccessorDecl_setParsedBody(BridgedAccessorDecl decl,
                                       BridgedBraceStmt body);

//===----------------------------------------------------------------------===//
// MARK: NominalTypeDecl
//===----------------------------------------------------------------------===//

SWIFT_NAME("BridgedNominalTypeDecl.getName(self:)")
BRIDGED_INLINE
BridgedStringRef BridgedNominalTypeDecl_getName(BridgedNominalTypeDecl decl);

SWIFT_NAME("BridgedNominalTypeDecl.isStructWithUnreferenceableStorage(self:)")
bool BridgedNominalTypeDecl_isStructWithUnreferenceableStorage(
    BridgedNominalTypeDecl decl);

SWIFT_NAME("BridgedNominalTypeDecl.isGlobalActor(self:)")
BRIDGED_INLINE
bool BridgedNominalTypeDecl_isGlobalActor(BridgedNominalTypeDecl decl);

SWIFT_NAME("BridgedNominalTypeDecl.hasValueDeinit(self:)")
BRIDGED_INLINE
bool BridgedNominalTypeDecl_hasValueDeinit(BridgedNominalTypeDecl decl);

SWIFT_NAME("BridgedNominalTypeDecl.isClass(self:)")
BRIDGED_INLINE
bool BridgedNominalTypeDecl_isClass(BridgedNominalTypeDecl decl);

SWIFT_NAME("BridgedNominalTypeDecl.setParsedMembers(self:_:fingerprint:)")
void BridgedNominalTypeDecl_setParsedMembers(BridgedNominalTypeDecl decl,
                                             BridgedArrayRef members,
                                             BridgedFingerprint fingerprint);

SWIFT_NAME("BridgedNominalTypeDecl.getSourceLocation(self:)")
BRIDGED_INLINE swift::SourceLoc
BridgedNominalTypeDecl_getSourceLocation(BridgedNominalTypeDecl decl);

//===----------------------------------------------------------------------===//
// MARK: SubscriptDecl
//===----------------------------------------------------------------------===//

SWIFT_NAME("getter:BridgedSubscriptDecl.asAbstractStorageDecl(self:)")
BRIDGED_INLINE
BridgedAbstractStorageDecl
BridgedSubscriptDecl_asAbstractStorageDecl(BridgedSubscriptDecl decl);

//===----------------------------------------------------------------------===//
// MARK: VarDecl
//===----------------------------------------------------------------------===//

SWIFT_NAME("BridgedVarDecl.createImplicitStringInterpolationVar(_:)")
BridgedVarDecl BridgedVarDec_createImplicitStringInterpolationVar(
    BridgedDeclContext cDeclContext);

SWIFT_NAME("getter:BridgedVarDecl.asAbstractStorageDecl(self:)")
BRIDGED_INLINE
BridgedAbstractStorageDecl
BridgedVarDecl_asAbstractStorageDecl(BridgedVarDecl decl);

//===----------------------------------------------------------------------===//
// MARK: Exprs
//===----------------------------------------------------------------------===//

struct BridgedCallArgument {
  swift::SourceLoc labelLoc;
  swift::Identifier label;
  BridgedExpr argExpr;

  BRIDGED_INLINE swift::Argument unbridged() const;
};

SWIFT_NAME("BridgedArgumentList.createImplicitUnlabeled(_:exprs:)")
BridgedArgumentList
BridgedArgumentList_createImplicitUnlabeled(BridgedASTContext cContext,
                                            BridgedArrayRef cExprs);

SWIFT_NAME("BridgedArgumentList.createParsed(_:lParenLoc:args:rParenLoc:"
           "firstTrailingClosureIndex:)")
BridgedArgumentList BridgedArgumentList_createParsed(
    BridgedASTContext cContext, swift::SourceLoc lParenLoc,
    BridgedArrayRef cArgs, swift::SourceLoc rParenLoc,
    size_t cFirstTrailingClosureIndex);

SWIFT_NAME("BridgedArrayExpr.createParsed(_:lSquareLoc:elements:commaLocs:"
           "rSquareLoc:)")
BridgedArrayExpr BridgedArrayExpr_createParsed(BridgedASTContext cContext,
                                               swift::SourceLoc lLoc,
                                               BridgedArrayRef elements,
                                               BridgedArrayRef commas,
                                               swift::SourceLoc rLoc);

SWIFT_NAME(
    "BridgedArrowExpr.createParsed(_:asyncLoc:throwsLoc:thrownType:arrowLoc:)")
BridgedArrowExpr BridgedArrowExpr_createParsed(BridgedASTContext cContext,
                                               swift::SourceLoc asyncLoc,
                                               swift::SourceLoc throwsLoc,
                                               BridgedNullableExpr cThrownType,
                                               swift::SourceLoc arrowLoc);

SWIFT_NAME("BridgedAssignExpr.createParsed(_:equalsLoc:)")
BridgedAssignExpr BridgedAssignExpr_createParsed(BridgedASTContext cContext,
                                                 swift::SourceLoc equalsLoc);

SWIFT_NAME("BridgedAwaitExpr.createParsed(_:awaitLoc:subExpr:)")
BridgedAwaitExpr BridgedAwaitExpr_createParsed(BridgedASTContext cContext,
                                               swift::SourceLoc awaitLoc,
                                               BridgedExpr cSubExpr);

SWIFT_NAME("BridgedBindOptionalExpr.createParsed(_:subExpr:questionLoc:)")
BridgedBindOptionalExpr
BridgedBindOptionalExpr_createParsed(BridgedASTContext cContext,
                                     BridgedExpr cSubExpr,
                                     swift::SourceLoc questionLoc);

SWIFT_NAME("BridgedBooleanLiteralExpr.createParsed(_:value:loc:)")
BridgedBooleanLiteralExpr
BridgedBooleanLiteralExpr_createParsed(BridgedASTContext cContext, bool value,
                                       swift::SourceLoc tokenLoc);

SWIFT_NAME("BridgedBorrowExpr.createParsed(_:borrowLoc:subExpr:)")
BridgedBorrowExpr BridgedBorrowExpr_createParsed(BridgedASTContext cContext,
                                                 swift::SourceLoc borrowLoc,
                                                 BridgedExpr cSubExpr);

SWIFT_NAME("BridgedCallExpr.createParsed(_:fn:args:)")
BridgedCallExpr BridgedCallExpr_createParsed(BridgedASTContext cContext,
                                             BridgedExpr fn,
                                             BridgedArgumentList args);

class BridgedCaptureListEntry {
  swift::PatternBindingDecl *_Nonnull PBD;

public:
  BRIDGED_INLINE BridgedCaptureListEntry(swift::CaptureListEntry CLE);
  BRIDGED_INLINE swift::CaptureListEntry unbridged() const;

  BRIDGED_INLINE
  SWIFT_COMPUTED_PROPERTY
  BridgedVarDecl getVarDecl() const;
};

SWIFT_NAME("BridgedCaptureListEntry.createParsed(_:declContext:ownership:"
           "ownershipRange:name:nameLoc:equalLoc:initializer:)")
BridgedCaptureListEntry BridegedCaptureListEntry_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    BridgedReferenceOwnership cOwnershipKind, swift::SourceRange ownershipRange,
    swift::Identifier name, swift::SourceLoc nameLoc, swift::SourceLoc equalLoc,
    BridgedExpr cInitializer);

SWIFT_NAME("BridgedCaptureListExpr.createParsed(_:captureList:closure:)")
BridgedCaptureListExpr BridgedCaptureListExpr_createParsed(BridgedASTContext cContext,
                                                           BridgedArrayRef cCaptureList,
                                                           BridgedClosureExpr cClosure);

SWIFT_NAME("BridgedClosureExpr.createParsed(_:declContext:attributes:"
           "bracketRange:capturedSelfDecl:parameterList:asyncLoc:throwsLoc:"
           "thrownType:arrowLoc:explicitResultType:inLoc:)")
BridgedClosureExpr BridgedClosureExpr_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    BridgedDeclAttributes cAttributes, swift::SourceRange bracketRange,
    BridgedNullableVarDecl cCapturedSelfDecl,
    BridgedNullableParameterList cParameterList, swift::SourceLoc asyncLoc,
    swift::SourceLoc throwsLoc, BridgedNullableTypeRepr cThrownType,
    swift::SourceLoc arrowLoc, BridgedNullableTypeRepr cExplicitResultType,
    swift::SourceLoc inLoc);

SWIFT_NAME("BridgedClosureExpr.getParameterList(self:)")
BridgedParameterList
BridgedClosureExpr_getParameterList(BridgedClosureExpr cClosure);

SWIFT_NAME("BridgedClosureExpr.setParameterList(self:_:)")
void BridgedClosureExpr_setParameterList(BridgedClosureExpr cClosure,
                                         BridgedParameterList cParams);

SWIFT_NAME("getter:BridgedClosureExpr.hasAnonymousClosureVars(self:)")
bool BridgedClosureExpr_hasAnonymousClosureVars(BridgedClosureExpr cClosure);

SWIFT_NAME("BridgedClosureExpr.setHasAnonymousClosureVars(self:)")
void BridgedClosureExpr_setHasAnonymousClosureVars(BridgedClosureExpr cClosure);

SWIFT_NAME("BridgedClosureExpr.setBody(self:_:)")
void BridgedClosureExpr_setBody(BridgedClosureExpr cClosure,
                                BridgedBraceStmt cBody);

SWIFT_NAME("BridgedCoerceExpr.createParsed(_:asLoc:type:)")
BridgedCoerceExpr BridgedCoerceExpr_createParsed(BridgedASTContext cContext,
                                                 swift::SourceLoc asLoc,
                                                 BridgedTypeRepr cType);

SWIFT_NAME(
    "BridgedConditionalCheckedCastExpr.createParsed(_:asLoc:questionLoc:type:)")
BridgedConditionalCheckedCastExpr
BridgedConditionalCheckedCastExpr_createParsed(BridgedASTContext cContext,
                                               swift::SourceLoc asLoc,
                                               swift::SourceLoc questionLoc,
                                               BridgedTypeRepr cType);

SWIFT_NAME("BridgedConsumeExpr.createParsed(_:consumeLoc:subExpr:)")
BridgedConsumeExpr BridgedConsumeExpr_createParsed(BridgedASTContext cContext,
                                                   swift::SourceLoc consumeLoc,
                                                   BridgedExpr cSubExpr);

SWIFT_NAME("BridgedCopyExpr.createParsed(_:copyLoc:subExpr:)")
BridgedCopyExpr BridgedCopyExpr_createParsed(BridgedASTContext cContext,
                                             swift::SourceLoc copyLoc,
                                             BridgedExpr cSubExpr);

SWIFT_NAME("BridgedDeclRefExpr.create(_:decl:loc:isImplicit:)")
BridgedDeclRefExpr BridgedDeclRefExpr_create(BridgedASTContext cContext,
                                             BridgedDecl cDecl,
                                             BridgedDeclNameLoc cLoc,
                                             bool IsImplicit);

SWIFT_NAME("BridgedDictionaryExpr.createParsed(_:lBracketLoc:elements:"
           "colonLocs:rBracketLoc:)")
BridgedDictionaryExpr BridgedDictionaryExpr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc lBracketLoc,
    BridgedArrayRef cElements, BridgedArrayRef cCommaLocs,
    swift::SourceLoc rBracketLoc);

SWIFT_NAME("BridgedDiscardAssignmentExpr.createParsed(_:loc:)")
BridgedDiscardAssignmentExpr
BridgedDiscardAssignmentExpr_createParsed(BridgedASTContext cContext,
                                          swift::SourceLoc loc);

SWIFT_NAME("BridgedDotSelfExpr.createParsed(_:subExpr:dotLoc:selfLoc:)")
BridgedDotSelfExpr BridgedDotSelfExpr_createParsed(BridgedASTContext cContext,
                                                   BridgedExpr cSubExpr,
                                                   swift::SourceLoc dotLoc,
                                                   swift::SourceLoc selfLoc);

SWIFT_NAME("BridgedEditorPlaceholderExpr.createParsed(_:placeholder:loc:"
           "placeholderType:expansionType:)")
BridgedEditorPlaceholderExpr BridgedEditorPlaceholderExpr_createParsed(
    BridgedASTContext cContext, swift::Identifier placeholderId,
    swift::SourceLoc loc, BridgedNullableTypeRepr cPlaceholderTyR,
    BridgedNullableTypeRepr cExpansionTyR);

SWIFT_NAME("BridgedErrorExpr.create(_:loc:)")
BridgedErrorExpr BridgedErrorExpr_create(BridgedASTContext cContext,
                                         swift::SourceRange range);

SWIFT_NAME("BridgedFloatLiteralExpr.createParsed(_:value:loc:)")
BridgedFloatLiteralExpr
BridgedFloatLiteralExpr_createParsed(BridgedASTContext cContext,
                                     BridgedStringRef cStr,
                                     swift::SourceLoc tokenLoc);

SWIFT_NAME("BridgedFloatLiteralExpr.setNegative(self:loc:)")
BRIDGED_INLINE void
BridgedFloatLiteralExpr_setNegative(BridgedFloatLiteralExpr cExpr,
                                    swift::SourceLoc loc);

SWIFT_NAME("BridgedForceTryExpr.createParsed(_:tryLoc:subExpr:exclaimLoc:)")
BridgedForceTryExpr
BridgedForceTryExpr_createParsed(BridgedASTContext cContext,
                                 swift::SourceLoc tryLoc, BridgedExpr cSubExpr,
                                 swift::SourceLoc exclaimLoc);

SWIFT_NAME("BridgedForceValueExpr.createParsed(_:subExpr:exclaimLoc:)")
BridgedForceValueExpr
BridgedForceValueExpr_createParsed(BridgedASTContext cContext,
                                   BridgedExpr cSubExpr,
                                   swift::SourceLoc exclaimLoc);

SWIFT_NAME(
    "BridgedForcedCheckedCastExpr.createParsed(_:asLoc:exclaimLoc:type:)")
BridgedForcedCheckedCastExpr BridgedForcedCheckedCastExpr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc asLoc,
    swift::SourceLoc exclaimLoc, BridgedTypeRepr cType);

SWIFT_NAME("BridgedUnresolvedSpecializeExpr.createParsed(_:subExpr:lAngleLoc:"
           "arguments:rAngleLoc:)")
BridgedUnresolvedSpecializeExpr BridgedUnresolvedSpecializeExpr_createParsed(
    BridgedASTContext cContext, BridgedExpr cSubExpr,
    swift::SourceLoc lAngleLoc, BridgedArrayRef cArguments,
    swift::SourceLoc rAngleLoc);

SWIFT_NAME("BridgedUnsafeExpr.createParsed(_:unsafeLoc:subExpr:)")
BridgedUnsafeExpr BridgedUnsafeExpr_createParsed(BridgedASTContext cContext,
                                                 swift::SourceLoc unsafeLoc,
                                                 BridgedExpr cSubExpr);

SWIFT_NAME("BridgedInOutExpr.createParsed(_:loc:subExpr:)")
BridgedInOutExpr BridgedInOutExpr_createParsed(BridgedASTContext cContext,
                                               swift::SourceLoc loc,
                                               BridgedExpr cSubExpr);

SWIFT_NAME("BridgedIntegerLiteralExpr.createParsed(_:value:loc:)")
BridgedIntegerLiteralExpr
BridgedIntegerLiteralExpr_createParsed(BridgedASTContext cContext,
                                       BridgedStringRef cStr,
                                       swift::SourceLoc tokenLoc);

SWIFT_NAME("BridgedIntegerLiteralExpr.setNegative(self:loc:)")
BRIDGED_INLINE void
BridgedIntegerLiteralExpr_setNegative(BridgedIntegerLiteralExpr cExpr,
                                      swift::SourceLoc loc);

SWIFT_NAME("BridgedInterpolatedStringLiteralExpr.createParsed(_:loc:"
           "literalCapacity:interpolationCount:appendingExpr:)")
BridgedInterpolatedStringLiteralExpr
BridgedInterpolatedStringLiteralExpr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc loc, size_t literalCapacity,
    size_t interpolationCount, BridgedTapExpr cAppendingExpr);

SWIFT_NAME("BridgedIsExpr.createParsed(_:isLoc:type:)")
BridgedIsExpr BridgedIsExpr_createParsed(BridgedASTContext cContext,
                                         swift::SourceLoc isLoc,
                                         BridgedTypeRepr cType);

SWIFT_NAME("BridgedKeyPathDotExpr.createParsed(_:loc:)")
BridgedKeyPathDotExpr
BridgedKeyPathDotExpr_createParsed(BridgedASTContext cContext,
                                   swift::SourceLoc loc);

SWIFT_NAME("BridgedKeyPathExpr.createParsed(_:backslashLoc:parsedRoot:"
           "parsedPath:hasLeadingDot:)")
BridgedKeyPathExpr BridgedKeyPathExpr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc backslashLoc,
    BridgedNullableExpr cParsedRoot, BridgedNullableExpr cParsedPath,
    bool hasLeadingDot);

SWIFT_NAME("BridgedKeyPathExpr.createParsedPoundKeyPath(_:poundLoc:lParenLoc:"
           "names:nameLocs:rParenLoc:)")
BridgedKeyPathExpr BridgedKeyPathExpr_createParsedPoundKeyPath(
    BridgedASTContext cContext, swift::SourceLoc poundLoc,
    swift::SourceLoc lParenLoc, BridgedArrayRef cNames,
    BridgedArrayRef cNameLocs, swift::SourceLoc rParenLoc);

SWIFT_NAME("BridgedMacroExpansionExpr.createParsed(_:poundLoc:macroNameRef:"
           "macroNameLoc:leftAngleLoc:genericArgs:rightAngleLoc:args:)")
BridgedMacroExpansionExpr BridgedMacroExpansionExpr_createParsed(
    BridgedDeclContext cDeclContext, swift::SourceLoc poundLoc,
    BridgedDeclNameRef cMacroNameRef, BridgedDeclNameLoc cMacroNameLoc,
    swift::SourceLoc leftAngleLoc, BridgedArrayRef cGenericArgs,
    swift::SourceLoc rightAngleLoc, BridgedNullableArgumentList cArgList);

enum ENUM_EXTENSIBILITY_ATTR(open) BridgedMagicIdentifierLiteralKind : uint8_t {
#define MAGIC_IDENTIFIER(NAME, STRING)                                         \
  BridgedMagicIdentifierLiteralKind##NAME,
#include "swift/AST/MagicIdentifierKinds.def"
  BridgedMagicIdentifierLiteralKindNone,
};

SWIFT_NAME("BridgedMagicIdentifierLiteralKind.init(from:)")
BridgedMagicIdentifierLiteralKind
BridgedMagicIdentifierLiteralKind_fromString(BridgedStringRef cStr);

SWIFT_NAME("BridgedMagicIdentifierLiteralExpr.createParsed(_:kind:loc:)")
BridgedMagicIdentifierLiteralExpr
BridgedMagicIdentifierLiteralExpr_createParsed(
    BridgedASTContext cContext, BridgedMagicIdentifierLiteralKind cKind,
    swift::SourceLoc loc);

SWIFT_NAME("BridgedNilLiteralExpr.createParsed(_:nilKeywordLoc:)")
BridgedNilLiteralExpr
BridgedNilLiteralExpr_createParsed(BridgedASTContext cContext,
                                   swift::SourceLoc nilKeywordLoc);

enum ENUM_EXTENSIBILITY_ATTR(open) BridgedObjCSelectorKind {
  BridgedObjCSelectorKindMethod,
  BridgedObjCSelectorKindGetter,
  BridgedObjCSelectorKindSetter,
};

SWIFT_NAME("BridgedObjCSelectorExpr.createParsed(_:kind:keywordLoc:lParenLoc:"
           "modifierLoc:subExpr:rParenLoc:)")
BridgedObjCSelectorExpr BridgedObjCSelectorExpr_createParsed(
    BridgedASTContext cContext, BridgedObjCSelectorKind cKind,
    swift::SourceLoc keywordLoc, swift::SourceLoc lParenLoc,
    swift::SourceLoc modifierLoc, BridgedExpr cSubExpr,
    swift::SourceLoc rParenLoc);

enum ENUM_EXTENSIBILITY_ATTR(open) BridgedObjectLiteralKind : size_t {
#define POUND_OBJECT_LITERAL(Name, Desc, Proto) BridgedObjectLiteralKind_##Name,
#include "swift/AST/TokenKinds.def"
  BridgedObjectLiteralKind_none,
};

SWIFT_NAME("BridgedObjectLiteralKind.init(from:)")
BridgedObjectLiteralKind
BridgedObjectLiteralKind_fromString(BridgedStringRef cStr);

SWIFT_NAME("BridgedObjectLiteralExpr.createParsed(_:poundLoc:kind:args:)")
BridgedObjectLiteralExpr BridgedObjectLiteralExpr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc poundLoc,
    BridgedObjectLiteralKind cKind, BridgedArgumentList args);

SWIFT_NAME("BridgedOptionalTryExpr.createParsed(_:tryLoc:subExpr:questionLoc:)")
BridgedOptionalTryExpr BridgedOptionalTryExpr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc tryLoc, BridgedExpr cSubExpr,
    swift::SourceLoc questionLoc);

SWIFT_NAME("BridgedPackElementExpr.createParsed(_:eachLoc:packRefExpr:)")
BridgedPackElementExpr
BridgedPackElementExpr_createParsed(BridgedASTContext cContext,
                                    swift::SourceLoc eachLoc,
                                    BridgedExpr cPackRefExpr);

SWIFT_NAME("BridgedPackExpansionExpr.createParsed(_:repeatLoc:patternExpr:)")
BridgedPackExpansionExpr
BridgedPackExpansionExpr_createParsed(BridgedASTContext cContext,
                                      swift::SourceLoc repeatLoc,
                                      BridgedExpr cPatternExpr);

SWIFT_NAME("BridgedParenExpr.createParsed(_:leftParenLoc:expr:rightParenLoc:)")
BridgedParenExpr BridgedParenExpr_createParsed(BridgedASTContext cContext,
                                               swift::SourceLoc lParen,
                                               BridgedExpr cExpr,
                                               swift::SourceLoc rParen);

SWIFT_NAME("BridgedPostfixUnaryExpr.createParsed(_:operator:operand:)")
BridgedPostfixUnaryExpr
BridgedPostfixUnaryExpr_createParsed(BridgedASTContext cContext,
                                     BridgedExpr oper, BridgedExpr operand);

SWIFT_NAME("BridgedPrefixUnaryExpr.createParsed(_:operator:operand:)")
BridgedPrefixUnaryExpr
BridgedPrefixUnaryExpr_createParsed(BridgedASTContext cContext,
                                    BridgedExpr oper, BridgedExpr operand);

class BridgedRegexLiteralPatternFeatureKind final {
  unsigned RawValue;

public:
  BRIDGED_INLINE
  SWIFT_NAME("init(rawValue:)")
  BridgedRegexLiteralPatternFeatureKind(SwiftInt rawValue);

  using UnbridgedTy = swift::RegexLiteralPatternFeatureKind;

  BRIDGED_INLINE
  BridgedRegexLiteralPatternFeatureKind(UnbridgedTy kind);

  BRIDGED_INLINE
  UnbridgedTy unbridged() const;
};

class BridgedRegexLiteralPatternFeature final {
  swift::CharSourceRange Range;
  BridgedRegexLiteralPatternFeatureKind Kind;

public:
  SWIFT_NAME("init(kind:at:)")
  BridgedRegexLiteralPatternFeature(BridgedRegexLiteralPatternFeatureKind kind,
                                    swift::CharSourceRange range)
      : Range(range), Kind(kind) {}

  using UnbridgedTy = swift::RegexLiteralPatternFeature;

  BRIDGED_INLINE
  BridgedRegexLiteralPatternFeature(UnbridgedTy feature);

  BRIDGED_INLINE
  UnbridgedTy unbridged() const;
};

class BridgedRegexLiteralPatternFeatures final {
  BridgedRegexLiteralPatternFeature *_Nullable Data;
  SwiftInt Count;

public:
  BridgedRegexLiteralPatternFeatures() : Data(nullptr), Count(0) {}

  SWIFT_NAME("init(baseAddress:count:)")
  BridgedRegexLiteralPatternFeatures(
      BridgedRegexLiteralPatternFeature *_Nullable data, SwiftInt count)
      : Data(data), Count(count) {}

  using UnbridgedTy = llvm::ArrayRef<BridgedRegexLiteralPatternFeature>;

  BRIDGED_INLINE
  UnbridgedTy unbridged() const;

  SWIFT_IMPORT_UNSAFE
  BridgedRegexLiteralPatternFeature *_Nullable getData() const {
    return Data;
  }
  SwiftInt getCount() const {
    return Count;
  }
};

SWIFT_NAME("BridgedRegexLiteralExpr.createParsed(_:loc:regexText:)")
BridgedRegexLiteralExpr
BridgedRegexLiteralExpr_createParsed(BridgedASTContext cContext,
                                     swift::SourceLoc loc,
                                     BridgedStringRef cRegexText);

SWIFT_NAME("BridgedSequenceExpr.createParsed(_:exprs:)")
BridgedSequenceExpr BridgedSequenceExpr_createParsed(BridgedASTContext cContext,
                                                     BridgedArrayRef exprs);

SWIFT_NAME("BridgedSingleValueStmtExpr.createWithWrappedBranches(_:stmt:"
           "declContext:mustBeExpr:)")
BridgedSingleValueStmtExpr BridgedSingleValueStmtExpr_createWithWrappedBranches(
    BridgedASTContext cContext, BridgedStmt S, BridgedDeclContext cDeclContext,
    bool mustBeExpr);

SWIFT_NAME("BridgedStringLiteralExpr.createParsed(_:value:loc:)")
BridgedStringLiteralExpr
BridgedStringLiteralExpr_createParsed(BridgedASTContext cContext,
                                      BridgedStringRef cStr,
                                      swift::SourceLoc tokenLoc);

SWIFT_NAME("BridgedSuperRefExpr.createParsed(_:superLoc:)")
BridgedSuperRefExpr BridgedSuperRefExpr_createParsed(BridgedASTContext cContext,
                                                     swift::SourceLoc superLoc);

SWIFT_NAME("BridgedSubscriptExpr.createParsed(_:baseExpr:args:)")
BridgedSubscriptExpr
BridgedSubscriptExpr_createParsed(BridgedASTContext cContext,
                                  BridgedExpr cBaseExpr,
                                  BridgedArgumentList cArgs);

SWIFT_NAME("BridgedTapExpr.create(_:body:)")
BridgedTapExpr BridgedTapExpr_create(BridgedASTContext cContext,
                                     BridgedBraceStmt cBody);

SWIFT_NAME("BridgedTernaryExpr.createParsed(_:questionLoc:thenExpr:colonLoc:)")
BridgedTernaryExpr BridgedTernaryExpr_createParsed(BridgedASTContext cContext,
                                                   swift::SourceLoc questionLoc,
                                                   BridgedExpr cThenExpr,
                                                   swift::SourceLoc colonLoc);

SWIFT_NAME("BridgedTryExpr.createParsed(_:tryLoc:subExpr:)")
BridgedTryExpr BridgedTryExpr_createParsed(BridgedASTContext cContext,
                                           swift::SourceLoc tryLoc,
                                           BridgedExpr cSubExpr);

SWIFT_NAME("BridgedTupleExpr.createParsed(_:leftParenLoc:exprs:labels:"
           "labelLocs:rightParenLoc:)")
BridgedTupleExpr BridgedTupleExpr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc lParen, BridgedArrayRef subs,
    BridgedArrayRef names, BridgedArrayRef cNameLocs, swift::SourceLoc rParen);

SWIFT_NAME("BridgedTupleExpr.createParsedDictionaryElement(_:key:value:)")
BridgedTupleExpr BridgedTupleExpr_createParsedDictionaryElement(
    BridgedASTContext cContext, BridgedExpr cKeyExpr, BridgedExpr cValueExpr);

SWIFT_NAME("BridgedTypeExpr.createParsed(_:type:)")
BridgedTypeExpr BridgedTypeExpr_createParsed(BridgedASTContext cContext,
                                             BridgedTypeRepr cType);

enum ENUM_EXTENSIBILITY_ATTR(open) BridgedDeclRefKind : size_t {
  BridgedDeclRefKindOrdinary,
  BridgedDeclRefKindBinaryOperator,
  BridgedDeclRefKindPostfixOperator,
  BridgedDeclRefKindPrefixOperator,
};

SWIFT_NAME("BridgedUnresolvedDeclRefExpr.createParsed(_:name:kind:loc:)")
BridgedUnresolvedDeclRefExpr BridgedUnresolvedDeclRefExpr_createParsed(
    BridgedASTContext cContext, BridgedDeclNameRef cName,
    BridgedDeclRefKind cKind, BridgedDeclNameLoc cLoc);

SWIFT_NAME("BridgedUnresolvedDotExpr.createParsed(_:base:dotLoc:name:nameLoc:)")
BridgedUnresolvedDotExpr BridgedUnresolvedDotExpr_createParsed(
    BridgedASTContext cContext, BridgedExpr base, swift::SourceLoc dotLoc,
    BridgedDeclNameRef cName, BridgedDeclNameLoc cNameLoc);

SWIFT_NAME("BridgedUnresolvedMemberExpr.createParsed(_:dotLoc:name:nameLoc:)")
BridgedUnresolvedMemberExpr BridgedUnresolvedMemberExpr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc dotLoc,
    BridgedDeclNameRef cName, BridgedDeclNameLoc cNameLoc);

SWIFT_NAME("BridgedUnresolvedPatternExpr.createParsed(_:pattern:)")
BridgedUnresolvedPatternExpr
BridgedUnresolvedPatternExpr_createParsed(BridgedASTContext cContext,
                                          BridgedPattern cPattern);

SWIFT_NAME("BridgedExpr.setImplicit(self:)")
void BridgedExpr_setImplicit(BridgedExpr cExpr);

SWIFT_NAME("BridgedExpr.dump(self:)")
void BridgedExpr_dump(BridgedExpr expr);

//===----------------------------------------------------------------------===//
// MARK: Stmts
//===----------------------------------------------------------------------===//

struct BridgedLabeledStmtInfo {
  SWIFT_NAME("name")
  swift::Identifier Name;
  SWIFT_NAME("loc")
  swift::SourceLoc Loc;

  BRIDGED_INLINE swift::LabeledStmtInfo unbridged() const;
};

class BridgedStmtConditionElement {
  void *_Nonnull Raw;

public:
  BRIDGED_INLINE BridgedStmtConditionElement(swift::StmtConditionElement elem);

  BRIDGED_INLINE swift::StmtConditionElement unbridged() const;
};

SWIFT_NAME("BridgedStmtConditionElement.createBoolean(expr:)")
BridgedStmtConditionElement
BridgedStmtConditionElement_createBoolean(BridgedExpr expr);

SWIFT_NAME("BridgedStmtConditionElement.createPatternBinding(_:introducerLoc:"
           "pattern:initializer:)")
BridgedStmtConditionElement BridgedStmtConditionElement_createPatternBinding(
    BridgedASTContext cContext, swift::SourceLoc introducerLoc,
    BridgedPattern cPattern, BridgedExpr cInitializer);

SWIFT_NAME("BridgedStmtConditionElement.createPoundAvailable(info:)")
BridgedStmtConditionElement BridgedStmtConditionElement_createPoundAvailable(
    BridgedPoundAvailableInfo info);

SWIFT_NAME("BridgedPoundAvailableInfo.createParsed(_:poundLoc:lParenLoc:specs:"
           "rParenLoc:isUnavailable:)")
BridgedPoundAvailableInfo BridgedPoundAvailableInfo_createParsed(
    BridgedASTContext cContext, swift::SourceLoc poundLoc,
    swift::SourceLoc lParenLoc, BridgedArrayRef cSpecs,
    swift::SourceLoc rParenLoc, bool isUnavailability);

SWIFT_NAME("BridgedStmtConditionElement.createHasSymbol(_:poundLoc:lParenLoc:"
           "symbol:rParenLoc:)")
BridgedStmtConditionElement BridgedStmtConditionElement_createHasSymbol(
    BridgedASTContext cContext, swift::SourceLoc poundLoc,
    swift::SourceLoc lParenLoc, BridgedNullableExpr cSymbolExpr,
    swift::SourceLoc rParenLoc);

struct BridgedCaseLabelItemInfo {
  SWIFT_NAME("isDefault")
  bool IsDefault;
  SWIFT_NAME("pattern")
  BridgedPattern ThePattern;
  SWIFT_NAME("whereLoc")
  swift::SourceLoc WhereLoc;
  SWIFT_NAME("guardExpr")
  BridgedNullableExpr GuardExpr;
};

SWIFT_NAME("BridgedBraceStmt.createParsed(_:lBraceLoc:elements:rBraceLoc:)")
BridgedBraceStmt BridgedBraceStmt_createParsed(BridgedASTContext cContext,
                                               swift::SourceLoc lBLoc,
                                               BridgedArrayRef elements,
                                               swift::SourceLoc rBLoc);

SWIFT_NAME("BridgedBraceStmt.createImplicit(_:lBraceLoc:element:rBraceLoc:)")
BridgedBraceStmt BridgedBraceStmt_createImplicit(BridgedASTContext cContext,
                                                 swift::SourceLoc lBLoc,
                                                 BridgedASTNode element,
                                                 swift::SourceLoc rBLoc);

SWIFT_NAME("BridgedBreakStmt.createParsed(_:loc:targetName:targetLoc:)")
BridgedBreakStmt BridgedBreakStmt_createParsed(BridgedDeclContext cDeclContext,
                                               swift::SourceLoc loc,
                                               swift::Identifier targetName,
                                               swift::SourceLoc targetLoc);

SWIFT_NAME("BridgedCaseStmt.createParsedSwitchCase(_:introducerLoc:"
           "caseLabelItems:unknownAttrLoc:terminatorLoc:body:)")
BridgedCaseStmt BridgedCaseStmt_createParsedSwitchCase(
    BridgedASTContext cContext, swift::SourceLoc introducerLoc,
    BridgedArrayRef cCaseLabelItems, swift::SourceLoc unknownAttrLoc,
    swift::SourceLoc terminatorLoc, BridgedBraceStmt cBody);

SWIFT_NAME(
    "BridgedCaseStmt.createParsedDoCatch(_:catchLoc:caseLabelItems:body:)")
BridgedCaseStmt BridgedCaseStmt_createParsedDoCatch(
    BridgedASTContext cContext, swift::SourceLoc catchLoc,
    BridgedArrayRef cCaseLabelItems, BridgedBraceStmt cBody);

SWIFT_NAME("BridgedContinueStmt.createParsed(_:loc:targetName:targetLoc:)")
BridgedContinueStmt BridgedContinueStmt_createParsed(
    BridgedDeclContext cDeclContext, swift::SourceLoc loc,
    swift::Identifier targetName, swift::SourceLoc targetLoc);

SWIFT_NAME("BridgedDeferStmt.createParsed(_:deferLoc:)")
BridgedDeferStmt BridgedDeferStmt_createParsed(BridgedDeclContext cDeclContext,
                                               swift::SourceLoc deferLoc);

SWIFT_NAME("getter:BridgedDeferStmt.tempDecl(self:)")
BridgedFuncDecl BridgedDeferStmt_getTempDecl(BridgedDeferStmt bridged);

SWIFT_NAME("BridgedDiscardStmt.createParsed(_:discardLoc:subExpr:)")
BridgedDiscardStmt BridgedDiscardStmt_createParsed(BridgedASTContext cContext,
                                                   swift::SourceLoc discardLoc,
                                                   BridgedExpr cSubExpr);

SWIFT_NAME("BridgedDoStmt.createParsed(_:labelInfo:doLoc:body:)")
BridgedDoStmt BridgedDoStmt_createParsed(BridgedASTContext cContext,
                                         BridgedLabeledStmtInfo cLabelInfo,
                                         swift::SourceLoc doLoc,
                                         BridgedBraceStmt cBody);

SWIFT_NAME(
    "BridgedDoCatchStmt.createParsed(_:labelInfo:doLoc:throwsLoc:thrownType:"
    "body:catches:)")
BridgedDoCatchStmt BridgedDoCatchStmt_createParsed(
    BridgedDeclContext cDeclContext, BridgedLabeledStmtInfo cLabelInfo,
    swift::SourceLoc doLoc, swift::SourceLoc throwsLoc,
    BridgedNullableTypeRepr cThrownType, BridgedStmt cBody,
    BridgedArrayRef cCatches);

SWIFT_NAME("BridgedFallthroughStmt.createParsed(loc:declContext:)")
BridgedFallthroughStmt
BridgedFallthroughStmt_createParsed(swift::SourceLoc loc,
                                    BridgedDeclContext cDC);

SWIFT_NAME("BridgedForEachStmt.createParsed(_:labelInfo:forLoc:tryLoc:awaitLoc:"
           "unsafeLoc:pattern:inLoc:sequence:whereLoc:whereExpr:body:)")
BridgedForEachStmt BridgedForEachStmt_createParsed(
    BridgedASTContext cContext, BridgedLabeledStmtInfo cLabelInfo,
    swift::SourceLoc forLoc, swift::SourceLoc tryLoc, swift::SourceLoc awaitLoc,
    swift::SourceLoc unsafeLoc, BridgedPattern cPat, swift::SourceLoc inLoc,
    BridgedExpr cSequence, swift::SourceLoc whereLoc,
    BridgedNullableExpr cWhereExpr, BridgedBraceStmt cBody);

SWIFT_NAME("BridgedGuardStmt.createParsed(_:guardLoc:conds:body:)")
BridgedGuardStmt BridgedGuardStmt_createParsed(BridgedASTContext cContext,
                                               swift::SourceLoc guardLoc,
                                               BridgedArrayRef cConds,
                                               BridgedBraceStmt cBody);

SWIFT_NAME("BridgedIfStmt.createParsed(_:labelInfo:ifLoc:conditions:then:"
           "elseLoc:else:)")
BridgedIfStmt BridgedIfStmt_createParsed(
    BridgedASTContext cContext, BridgedLabeledStmtInfo cLabelInfo,
    swift::SourceLoc ifLoc, BridgedArrayRef cConds, BridgedBraceStmt cThen,
    swift::SourceLoc elseLoc, BridgedNullableStmt cElse);

SWIFT_NAME("BridgedPoundAssertStmt.createParsed(_:range:condition:message:)")
BridgedPoundAssertStmt BridgedPoundAssertStmt_createParsed(
    BridgedASTContext cContext, swift::SourceRange range,
    BridgedExpr cConditionExpr, BridgedStringRef cMessage);

SWIFT_NAME("BridgedRepeatWhileStmt.createParsed(_:labelInfo:repeatLoc:cond:"
           "whileLoc:body:)")
BridgedRepeatWhileStmt BridgedRepeatWhileStmt_createParsed(
    BridgedASTContext cContext, BridgedLabeledStmtInfo cLabelInfo,
    swift::SourceLoc repeatLoc, BridgedExpr cCond, swift::SourceLoc whileLoc,
    BridgedStmt cBody);

SWIFT_NAME("BridgedReturnStmt.createParsed(_:loc:expr:)")
BridgedReturnStmt BridgedReturnStmt_createParsed(BridgedASTContext cContext,
                                                 swift::SourceLoc loc,
                                                 BridgedNullableExpr expr);

SWIFT_NAME("BridgedSwitchStmt.createParsed(_:labelInfo:switchLoc:subjectExpr:"
           "lBraceLoc:cases:rBraceLoc:)")
BridgedSwitchStmt BridgedSwitchStmt_createParsed(
    BridgedASTContext cContext, BridgedLabeledStmtInfo cLabelInfo,
    swift::SourceLoc switchLoc, BridgedExpr cSubjectExpr,
    swift::SourceLoc lBraceLoc, BridgedArrayRef cCases,
    swift::SourceLoc rBraceLoc);

SWIFT_NAME("BridgedThenStmt.createParsed(_:thenLoc:result:)")
BridgedThenStmt BridgedThenStmt_createParsed(BridgedASTContext cContext,
                                             swift::SourceLoc thenLoc,
                                             BridgedExpr cResult);

SWIFT_NAME("BridgedThrowStmt.createParsed(_:throwLoc:subExpr:)")
BridgedThrowStmt BridgedThrowStmt_createParsed(BridgedASTContext cContext,
                                               swift::SourceLoc throwLoc,
                                               BridgedExpr cSubExpr);

SWIFT_NAME("BridgedWhileStmt.createParsed(_:labelInfo:whileLoc:cond:body:)")
BridgedWhileStmt BridgedWhileStmt_createParsed(
    BridgedASTContext cContext, BridgedLabeledStmtInfo cLabelInfo,
    swift::SourceLoc whileLoc, BridgedArrayRef cCond, BridgedStmt cBody);

SWIFT_NAME(
    "BridgedYieldStmt.createParsed(_:yieldLoc:lParenLoc:yields:rParenLoc:)")
BridgedYieldStmt BridgedYieldStmt_createParsed(BridgedASTContext cContext,
                                               swift::SourceLoc yieldLoc,
                                               swift::SourceLoc lParenLoc,
                                               BridgedArrayRef cYields,
                                               swift::SourceLoc rParenLoc);

SWIFT_NAME("BridgedStmt.dump(self:)")
void BridgedStmt_dump(BridgedStmt statement);

//===----------------------------------------------------------------------===//
// MARK: TypeAttributes
//===----------------------------------------------------------------------===//

class BridgedTypeOrCustomAttr {
public:
  enum Kind : uint8_t {
    TypeAttr,
    CustomAttr,
  } kind;

private:
  intptr_t opaque;

  void *_Nonnull getPointer() const {
    return reinterpret_cast<void *>(opaque & ~0x7);
  }

  BRIDGED_INLINE BridgedTypeOrCustomAttr(void *_Nonnull pointer, Kind kind);

public:
  SWIFT_NAME("typeAttr(_:)")
  static BridgedTypeOrCustomAttr createTypeAttr(BridgedTypeAttribute typeAttr) {
    return BridgedTypeOrCustomAttr(typeAttr.unbridged(), Kind::TypeAttr);
  }
  SWIFT_NAME("customAttr(_:)")
  static BridgedTypeOrCustomAttr
  createCust0kAttr(BridgedCustomAttr customAttr) {
    return BridgedTypeOrCustomAttr(customAttr.unbridged(), Kind::CustomAttr);
  }

  Kind getKind() const { return static_cast<Kind>(opaque & 0x7); }

  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedTypeAttribute
  castToTypeAttr() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedCustomAttr castToCustomAttr() const;
};

BRIDGED_OPTIONAL(swift::TypeAttrKind, TypeAttrKind)

SWIFT_NAME("BridgedOptionalTypeAttrKind.init(from:)")
BridgedOptionalTypeAttrKind
BridgedOptionalTypeAttrKind_fromString(BridgedStringRef cStr);

SWIFT_NAME("BridgedTypeAttribute.createSimple(_:kind:atLoc:nameLoc:)")
BridgedTypeAttribute BridgedTypeAttribute_createSimple(
    BridgedASTContext cContext, swift::TypeAttrKind kind,
    swift::SourceLoc atLoc, swift::SourceLoc nameLoc);

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedIsolatedTypeAttrIsolationKind {
  BridgedIsolatedTypeAttrIsolationKind_DynamicIsolation,
};

SWIFT_NAME("BridgedConventionTypeAttr.createParsed(_:atLoc:nameLoc:parensRange:"
           "name:nameLoc:witnessMethodProtocol:clangType:clangTypeLoc:)")
BridgedConventionTypeAttr BridgedConventionTypeAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc, swift::SourceLoc kwLoc,
    swift::SourceRange parens, BridgedStringRef cName, swift::SourceLoc nameLoc,
    BridgedDeclNameRef cWitnessMethodProtocol, BridgedStringRef cClangType,
    swift::SourceLoc clangTypeLoc);

SWIFT_NAME("BridgedDifferentiableTypeAttr.createParsed(_:atLoc:nameLoc:"
           "parensRange:kind:kindLoc:)")
BridgedDifferentiableTypeAttr BridgedDifferentiableTypeAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceLoc nameLoc, swift::SourceRange parensRange,
    BridgedDifferentiabilityKind cKind, swift::SourceLoc kindLoc);

SWIFT_NAME("BridgedIsolatedTypeAttr.createParsed(_:atLoc:nameLoc:parensRange:"
           "isolationKind:isolationKindLoc:)")
BridgedIsolatedTypeAttr BridgedIsolatedTypeAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc,
    swift::SourceLoc nameLoc, swift::SourceRange parensRange,

    BridgedIsolatedTypeAttrIsolationKind cIsolation,
    swift::SourceLoc isolationLoc);

SWIFT_NAME("BridgedOpaqueReturnTypeOfTypeAttr.createParsed(_:atLoc:nameLoc:"
           "parensRange:"
           "mangled:mangledLoc:index:indexLoc:)")
BridgedOpaqueReturnTypeOfTypeAttr
BridgedOpaqueReturnTypeOfTypeAttr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc atLoc, swift::SourceLoc kwLoc,
    swift::SourceRange parens, BridgedStringRef cMangled,
    swift::SourceLoc mangledDoc, size_t index, swift::SourceLoc indexLoc);

//===----------------------------------------------------------------------===//
// MARK: TypeReprs
//===----------------------------------------------------------------------===//

SWIFT_NAME("BridgedUnqualifiedIdentTypeRepr.createParsed(_:loc:name:)")
BridgedUnqualifiedIdentTypeRepr BridgedUnqualifiedIdentTypeRepr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc loc, swift::Identifier id);

SWIFT_NAME(
    "BridgedArrayTypeRepr.createParsed(_:base:leftSquareLoc:rightSquareLoc:)")
BridgedArrayTypeRepr BridgedArrayTypeRepr_createParsed(
    BridgedASTContext cContext, BridgedTypeRepr base,
    swift::SourceLoc lSquareLoc, swift::SourceLoc rSquareLoc);

SWIFT_NAME("BridgedAttributedTypeRepr.createParsed(_:base:attributes:)")
BridgedAttributedTypeRepr
BridgedAttributedTypeRepr_createParsed(BridgedASTContext cContext,
                                       BridgedTypeRepr base,
                                       BridgedArrayRef cAttributes);

SWIFT_NAME("BridgedCompositionTypeRepr.createEmpty(_:anyKeywordLoc:)")
BridgedCompositionTypeRepr
BridgedCompositionTypeRepr_createEmpty(BridgedASTContext cContext,
                                       swift::SourceLoc anyLoc);

SWIFT_NAME("BridgedCompositionTypeRepr.createParsed(_:types:ampersandLoc:)")
BridgedCompositionTypeRepr
BridgedCompositionTypeRepr_createParsed(BridgedASTContext cContext,
                                        BridgedArrayRef types,
                                        swift::SourceLoc firstAmpLoc);

SWIFT_NAME("BridgedCompileTimeLiteralTypeRepr.createParsed(_:base:specifierLoc:)")
BridgedCompileTimeLiteralTypeRepr
BridgedCompileTimeLiteralTypeRepr_createParsed(BridgedASTContext cContext,
                                               BridgedTypeRepr base,
                                               swift::SourceLoc specifierLoc);

SWIFT_NAME("BridgedDeclRefTypeRepr.createParsed(_:base:name:nameLoc:"
           "genericArguments:angleRange:)")
BridgedDeclRefTypeRepr BridgedDeclRefTypeRepr_createParsed(
    BridgedASTContext cContext, BridgedTypeRepr cBase, swift::Identifier name,
    swift::SourceLoc loc, BridgedArrayRef cGenericArguments,
    swift::SourceRange angleRange);

SWIFT_NAME("BridgedDictionaryTypeRepr.createParsed(_:leftSquareLoc:keyType:"
           "colonLoc:valueType:rightSquareLoc:)")
BridgedDictionaryTypeRepr BridgedDictionaryTypeRepr_createParsed(
    BridgedASTContext cContext, swift::SourceLoc lSquareLoc,
    BridgedTypeRepr keyType, swift::SourceLoc colonloc,
    BridgedTypeRepr valueType, swift::SourceLoc rSquareLoc);

SWIFT_NAME("BridgedErrorTypeRepr.create(_:range:)")
BridgedErrorTypeRepr BridgedErrorTypeRepr_create(BridgedASTContext cContext,
                                                 swift::SourceRange range);

SWIFT_NAME("BridgedFunctionTypeRepr.createParsed(_:argsType:asyncLoc:throwsLoc:"
           "thrownType:arrowLoc:resultType:)")
BridgedFunctionTypeRepr BridgedFunctionTypeRepr_createParsed(
    BridgedASTContext cContext, BridgedTypeRepr argsTy,
    swift::SourceLoc asyncLoc, swift::SourceLoc throwsLoc,
    BridgedNullableTypeRepr thrownType, swift::SourceLoc arrowLoc,
    BridgedTypeRepr resultType);

SWIFT_NAME("BridgedUnqualifiedIdentTypeRepr.createParsed(_:name:nameLoc:"
           "genericArgs:leftAngleLoc:rightAngleLoc:)")
BridgedUnqualifiedIdentTypeRepr BridgedUnqualifiedIdentTypeRepr_createParsed(
    BridgedASTContext cContext, swift::Identifier name,
    swift::SourceLoc nameLoc, BridgedArrayRef genericArgs,
    swift::SourceLoc lAngleLoc, swift::SourceLoc rAngleLoc);

SWIFT_NAME("BridgedOptionalTypeRepr.createParsed(_:base:questionLoc:)")
BridgedOptionalTypeRepr
BridgedOptionalTypeRepr_createParsed(BridgedASTContext cContext,
                                     BridgedTypeRepr base,
                                     swift::SourceLoc questionLoc);

SWIFT_NAME("BridgedImplicitlyUnwrappedOptionalTypeRepr.createParsed(_:base:"
           "exclaimLoc:)")
BridgedImplicitlyUnwrappedOptionalTypeRepr
BridgedImplicitlyUnwrappedOptionalTypeRepr_createParsed(
    BridgedASTContext cContext, BridgedTypeRepr base,
    swift::SourceLoc exclamationLoc);

SWIFT_NAME("BridgedInlineArrayTypeRepr.createParsed(_:count:element:brackets:)")
BridgedInlineArrayTypeRepr BridgedInlineArrayTypeRepr_createParsed(
    BridgedASTContext cContext, BridgedTypeRepr cCountType,
    BridgedTypeRepr cElementType, swift::SourceRange bracketsRange);

SWIFT_NAME("BridgedInverseTypeRepr.createParsed(_:tildeLoc:constraint:)")
BridgedInverseTypeRepr
BridgedInverseTypeRepr_createParsed(BridgedASTContext cContext,
                                    swift::SourceLoc tildeLoc,
                                    BridgedTypeRepr cConstraint);

SWIFT_NAME("BridgedIsolatedTypeRepr.createParsed(_:base:specifierLoc:)")
BridgedIsolatedTypeRepr
BridgedIsolatedTypeRepr_createParsed(BridgedASTContext cContext,
                                     BridgedTypeRepr base,
                                     swift::SourceLoc specifierLoc);

SWIFT_NAME("BridgedLifetimeDependentTypeRepr.createParsed(_:base:entry:)")
BridgedLifetimeDependentTypeRepr
BridgedLifetimeDependentTypeRepr_createParsed(BridgedASTContext cContext,
                                              BridgedTypeRepr base,
                                              BridgedLifetimeEntry cEntry);

SWIFT_NAME("BridgedMetatypeTypeRepr.createParsed(_:base:typeKeywordLoc:)")
BridgedMetatypeTypeRepr
BridgedMetatypeTypeRepr_createParsed(BridgedASTContext cContext,
                                     BridgedTypeRepr baseType,
                                     swift::SourceLoc typeLoc);

SWIFT_NAME(
    "BridgedOwnershipTypeRepr.createParsed(_:base:specifier:specifierLoc:)")
BridgedOwnershipTypeRepr BridgedOwnershipTypeRepr_createParsed(
    BridgedASTContext cContext, BridgedTypeRepr base,
    BridgedParamSpecifier cSpecifier, swift::SourceLoc specifierLoc);

SWIFT_NAME("BridgedPlaceholderTypeRepr.createParsed(_:loc:)")
BridgedPlaceholderTypeRepr
BridgedPlaceholderTypeRepr_createParsed(BridgedASTContext cContext,
                                        swift::SourceLoc loc);

SWIFT_NAME("BridgedProtocolTypeRepr.createParsed(_:base:protocolKeywordLoc:)")
BridgedProtocolTypeRepr
BridgedProtocolTypeRepr_createParsed(BridgedASTContext cContext,
                                     BridgedTypeRepr baseType,
                                     swift::SourceLoc protoLoc);

SWIFT_NAME("BridgedPackElementTypeRepr.createParsed(_:base:eachKeywordLoc:)")
BridgedPackElementTypeRepr BridgedPackElementTypeRepr_createParsed(
    BridgedASTContext cContext, BridgedTypeRepr base, swift::SourceLoc eachLoc);

SWIFT_NAME(
    "BridgedPackExpansionTypeRepr.createParsed(_:base:repeatKeywordLoc:)")
BridgedPackExpansionTypeRepr
BridgedPackExpansionTypeRepr_createParsed(BridgedASTContext cContext,
                                          BridgedTypeRepr base,
                                          swift::SourceLoc repeatLoc);

SWIFT_NAME("BridgedSendingTypeRepr.createParsed(_:base:specifierLoc:)")
BridgedSendingTypeRepr
BridgedSendingTypeRepr_createParsed(BridgedASTContext cContext,
                                    BridgedTypeRepr base,
                                    swift::SourceLoc specifierLoc);

SWIFT_NAME("BridgedCallerIsolatedTypeRepr.createParsed(_:base:specifierLoc:)")
BridgedCallerIsolatedTypeRepr
BridgedCallerIsolatedTypeRepr_createParsed(BridgedASTContext cContext,
                                           BridgedTypeRepr base,
                                           swift::SourceLoc specifierLoc);

SWIFT_NAME(
    "BridgedTupleTypeRepr.createParsed(_:elements:leftParenLoc:rightParenLoc:)")
BridgedTupleTypeRepr BridgedTupleTypeRepr_createParsed(
    BridgedASTContext cContext, BridgedArrayRef elements,
    swift::SourceLoc lParenLoc, swift::SourceLoc rParenLoc);

SWIFT_NAME(
    "BridgedNamedOpaqueReturnTypeRepr.createParsed(_:base:genericParamList:)")
BridgedNamedOpaqueReturnTypeRepr BridgedNamedOpaqueReturnTypeRepr_createParsed(
    BridgedASTContext cContext, BridgedTypeRepr baseTy,
    BridgedGenericParamList genericParams);

SWIFT_NAME("BridgedOpaqueReturnTypeRepr.createParsed(_:someKeywordLoc:base:)")
BridgedOpaqueReturnTypeRepr
BridgedOpaqueReturnTypeRepr_createParsed(BridgedASTContext cContext,
                                         swift::SourceLoc opaqueLoc,
                                         BridgedTypeRepr baseTy);

SWIFT_NAME("BridgedExistentialTypeRepr.createParsed(_:anyKeywordLoc:base:)")
BridgedExistentialTypeRepr
BridgedExistentialTypeRepr_createParsed(BridgedASTContext cContext,
                                        swift::SourceLoc anyLoc,
                                        BridgedTypeRepr baseTy);

SWIFT_NAME("BridgedVarargTypeRepr.createParsed(_:base:ellipsisLoc:)")
BridgedVarargTypeRepr
BridgedVarargTypeRepr_createParsed(BridgedASTContext cContext,
                                   BridgedTypeRepr base,
                                   swift::SourceLoc ellipsisLoc);

SWIFT_NAME(
    "BridgedIntegerTypeRepr.createParsed(_:string:loc:minusLoc:)")
BridgedIntegerTypeRepr BridgedIntegerTypeRepr_createParsed(
    BridgedASTContext cContext, BridgedStringRef cString, swift::SourceLoc loc,
    swift::SourceLoc minusLoc);

SWIFT_NAME("BridgedTypeRepr.dump(self:)")
void BridgedTypeRepr_dump(BridgedTypeRepr type);

//===----------------------------------------------------------------------===//
// MARK: Patterns
//===----------------------------------------------------------------------===//

SWIFT_NAME("getter:BridgedPattern.singleVar(self:)")
BridgedNullableVarDecl BridgedPattern_getSingleVar(BridgedPattern cPattern);

SWIFT_NAME("BridgedAnyPattern.createParsed(_:loc:)")
BridgedAnyPattern BridgedAnyPattern_createParsed(BridgedASTContext cContext,
                                                 swift::SourceLoc loc);

SWIFT_NAME("BridgedAnyPattern.createImplicit(_:)")
BridgedAnyPattern BridgedAnyPattern_createImplicit(BridgedASTContext cContext);

SWIFT_NAME("BridgedBindingPattern.createParsed(_:keywordLoc:isLet:subPattern:)")
BridgedBindingPattern
BridgedBindingPattern_createParsed(BridgedASTContext cContext,
                                   swift::SourceLoc keywordLoc, bool isLet,
                                   BridgedPattern cSubPattern);

SWIFT_NAME("BridgedBindingPattern.createImplicitCatch(_:loc:)")
BridgedBindingPattern
BridgedBindingPattern_createImplicitCatch(BridgedDeclContext cDeclContext,
                                          swift::SourceLoc loc);

SWIFT_NAME("BridgedExprPattern.createParsed(_:expr:)")
BridgedExprPattern
BridgedExprPattern_createParsed(BridgedDeclContext cDeclContext,
                                BridgedExpr cExpr);

SWIFT_NAME("BridgedIsPattern.createParsed(_:isLoc:typeExpr:)")
BridgedIsPattern BridgedIsPattern_createParsed(BridgedASTContext cContext,
                                               swift::SourceLoc isLoc,
                                               BridgedTypeExpr cTypeExpr);

SWIFT_NAME("BridgedNamedPattern.createParsed(_:declContext:name:loc:)")
BridgedNamedPattern
BridgedNamedPattern_createParsed(BridgedASTContext astContext,
                                 BridgedDeclContext declContext,
                                 swift::Identifier name, swift::SourceLoc loc);

SWIFT_NAME(
    "BridgedParenPattern.createParsed(_:lParenLoc:subPattern:rParenLoc:)")
BridgedParenPattern BridgedParenPattern_createParsed(
    BridgedASTContext cContext, swift::SourceLoc lParenLoc,
    BridgedPattern cSubPattern, swift::SourceLoc rParenLoc);

struct BridgedTuplePatternElt {
  swift::Identifier Label;
  swift::SourceLoc LabelLoc;
  BridgedPattern ThePattern;
};

SWIFT_NAME("BridgedTuplePattern.createParsed(_:lParenLoc:elements:rParenLoc:)")
BridgedTuplePattern BridgedTuplePattern_createParsed(
    BridgedASTContext cContext, swift::SourceLoc lParenLoc,
    BridgedArrayRef cElements, swift::SourceLoc rParenLoc);

SWIFT_NAME("BridgedTypedPattern.createParsed(_:pattern:type:)")
BridgedTypedPattern BridgedTypedPattern_createParsed(BridgedASTContext cContext,
                                                     BridgedPattern cPattern,
                                                     BridgedTypeRepr cType);

SWIFT_NAME("BridgedTypedPattern.createPropagated(_:pattern:type:)")
BridgedTypedPattern BridgedTypedPattern_createPropagated(
    BridgedASTContext cContext, BridgedPattern cPattern, BridgedTypeRepr cType);

SWIFT_NAME("BridgedPattern.setImplicit(self:)")
void BridgedPattern_setImplicit(BridgedPattern cPattern);

SWIFT_NAME("getter:BridgedPattern.boundName(self:)")
swift::Identifier BridgedPattern_getBoundName(BridgedPattern cPattern);

//===----------------------------------------------------------------------===//
// MARK: Generics
//===----------------------------------------------------------------------===//

class BridgedLayoutConstraint {
  swift::LayoutConstraintInfo *_Nullable raw;

public:
  SWIFT_UNAVAILABLE("Use the factory methods")
  BRIDGED_INLINE BridgedLayoutConstraint();

  SWIFT_UNAVAILABLE("Use the factory methods")
  BRIDGED_INLINE BridgedLayoutConstraint(swift::LayoutConstraint constraint);

  BRIDGED_INLINE
  SWIFT_COMPUTED_PROPERTY
  bool getIsNull() const;

  SWIFT_COMPUTED_PROPERTY
  swift::LayoutConstraintKind getKind() const;

  BRIDGED_INLINE
  SWIFT_COMPUTED_PROPERTY
  bool getIsKnownLayout() const;

  BRIDGED_INLINE
  SWIFT_COMPUTED_PROPERTY
  bool getIsTrivial() const;

  SWIFT_UNAVAILABLE("Unavailable in Swift")
  BRIDGED_INLINE swift::LayoutConstraint unbridged() const;
};

SWIFT_NAME("BridgedLayoutConstraint.getLayoutConstraint(_:id:)")
BridgedLayoutConstraint
BridgedLayoutConstraint_getLayoutConstraint(BridgedASTContext cContext,
                                            swift::Identifier ID);

SWIFT_NAME("BridgedLayoutConstraint.getLayoutConstraint(_:kind:)")
BridgedLayoutConstraint
BridgedLayoutConstraint_getLayoutConstraint(BridgedASTContext cContext,
                                            swift::LayoutConstraintKind kind);

SWIFT_NAME(
    "BridgedLayoutConstraint.getLayoutConstraint(_:kind:size:alignment:)")
BridgedLayoutConstraint
BridgedLayoutConstraint_getLayoutConstraint(BridgedASTContext cContext,
                                            swift::LayoutConstraintKind kind,
                                            size_t size, size_t alignment);

struct BridgedRequirementRepr {
  swift::SourceLoc SeparatorLoc;
  swift::RequirementReprKind Kind;
  BridgedTypeRepr FirstType;
  BridgedNullableTypeRepr SecondType;
  BridgedLayoutConstraint LayoutConstraint;
  swift::SourceLoc LayoutConstraintLoc;
  bool IsExpansionPattern;

  swift::RequirementRepr unbridged() const;
};

SWIFT_NAME("BridgedRequirementRepr.createTypeConstraint(subject:colonLoc:"
           "constraint:isExpansionPattern:)")
BridgedRequirementRepr BridgedRequirementRepr_createTypeConstraint(
    BridgedTypeRepr cSubject, swift::SourceLoc colonLoc,
    BridgedTypeRepr cConstraint, bool isExpansionPattern);
SWIFT_NAME("BridgedRequirementRepr.createSameType(firstType:equalLoc:"
           "secondType:isExpansionPattern:)")
BridgedRequirementRepr BridgedRequirementRepr_createSameType(
    BridgedTypeRepr cFirstType, swift::SourceLoc equalLoc,
    BridgedTypeRepr cSecondType, bool isExpansionPattern);
SWIFT_NAME("BridgedRequirementRepr.createLayoutConstraint(subject:colonLoc:"
           "layout:layoutLoc:isExpansionPattern:)")
BridgedRequirementRepr BridgedRequirementRepr_createLayoutConstraint(
    BridgedTypeRepr cSubject, swift::SourceLoc colonLoc,
    BridgedLayoutConstraint cLayout, swift::SourceLoc layoutLoc,
    bool isExpansionPattern);

SWIFT_NAME("BridgedGenericParamList.createParsed(_:leftAngleLoc:parameters:"
           "genericWhereClause:rightAngleLoc:)")
BridgedGenericParamList BridgedGenericParamList_createParsed(
    BridgedASTContext cContext, swift::SourceLoc leftAngleLoc,
    BridgedArrayRef cParameters,
    BridgedNullableTrailingWhereClause genericWhereClause,
    swift::SourceLoc rightAngleLoc);

SWIFT_NAME(
    "BridgedGenericTypeParamDecl.createParsed(_:declContext:specifierLoc:"
    "name:nameLoc:inheritedType:index:paramKind:)")
BridgedGenericTypeParamDecl BridgedGenericTypeParamDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    swift::SourceLoc specifierLoc, swift::Identifier name,
    swift::SourceLoc nameLoc, BridgedNullableTypeRepr opaqueInheritedType,
    size_t index, swift::GenericTypeParamKind paramKind);

SWIFT_NAME(
    "BridgedTrailingWhereClause.createParsed(_:whereKeywordLoc:requirements:)")
BridgedTrailingWhereClause
BridgedTrailingWhereClause_createParsed(BridgedASTContext cContext,
                                        swift::SourceLoc whereKeywordLoc,
                                        BridgedArrayRef cRequirements);

SWIFT_NAME("BridgedParameterList.createParsed(_:leftParenLoc:parameters:"
           "rightParenLoc:)")
BridgedParameterList BridgedParameterList_createParsed(
    BridgedASTContext cContext, swift::SourceLoc leftParenLoc,
    BridgedArrayRef cParameters, swift::SourceLoc rightParenLoc);

SWIFT_NAME("getter:BridgedParameterList.size(self:)")
size_t BridgedParameterList_size(BridgedParameterList cParameterList);

SWIFT_NAME("BridgedParameterList.get(self:_:)")
BridgedParamDecl BridgedParameterList_get(BridgedParameterList cParameterList,
                                          size_t i);

//===----------------------------------------------------------------------===//
// MARK: Misc
//===----------------------------------------------------------------------===//

struct BridgedTupleTypeElement {
  swift::Identifier Name;
  swift::SourceLoc NameLoc;
  swift::Identifier SecondName;
  swift::SourceLoc SecondNameLoc;
  swift::SourceLoc UnderscoreLoc;
  swift::SourceLoc ColonLoc;
  BridgedTypeRepr Type;
  swift::SourceLoc TrailingCommaLoc;
};

enum ENUM_EXTENSIBILITY_ATTR(open) BridgedMacroDefinitionKind : size_t {
  /// An expanded macro.
  BridgedExpandedMacro = 0,
  /// An external macro, spelled with either the old spelling (Module.Type)
  /// or the new spelling `#externalMacro(module: "Module", type: "Type")`.
  BridgedExternalMacro,
  /// The builtin definition for "externalMacro".
  BridgedBuiltinExternalMacro,
  /// The builtin definition for the "isolation" macro.
  BridgedBuiltinIsolationMacro,
};

struct BridgedASTType {
  enum class TraitResult {
    IsNot,
    CanBe,
    Is
  };

  enum class MetatypeRepresentation {
    Thin,
    Thick,
    ObjC
  };

  swift::TypeBase * _Nullable type;

  BRIDGED_INLINE swift::Type unbridged() const;
  BridgedOwnedString getDebugDescription() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedCanType getCanonicalType() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedDiagnosticArgument asDiagnosticArgument() const;
  BRIDGED_INLINE bool hasArchetype() const;
  BRIDGED_INLINE bool isLegalFormalType() const;
  BRIDGED_INLINE bool isGenericAtAnyLevel() const;
  BRIDGED_INLINE bool hasTypeParameter() const;
  BRIDGED_INLINE bool hasLocalArchetype() const;
  BRIDGED_INLINE bool hasDynamicSelf() const;
  BRIDGED_INLINE bool isArchetype() const;
  BRIDGED_INLINE bool archetypeRequiresClass() const;
  BRIDGED_INLINE bool isExistentialArchetype() const;
  BRIDGED_INLINE bool isExistentialArchetypeWithError() const;
  BRIDGED_INLINE bool isExistential() const;
  BRIDGED_INLINE bool isDynamicSelf() const;
  BRIDGED_INLINE bool isClassExistential() const;
  BRIDGED_INLINE bool isGenericTypeParam() const;
  BRIDGED_INLINE bool isEscapable() const;
  BRIDGED_INLINE bool isNoEscape() const;
  BRIDGED_INLINE bool isInteger() const;
  BRIDGED_INLINE bool isUnownedStorageType() const;
  BRIDGED_INLINE bool isMetatypeType() const;
  BRIDGED_INLINE bool isExistentialMetatypeType() const;
  BRIDGED_INLINE bool isTuple() const;
  BRIDGED_INLINE bool isFunction() const;
  BRIDGED_INLINE bool isLoweredFunction() const;
  BRIDGED_INLINE bool isNoEscapeFunction() const;
  BRIDGED_INLINE bool isThickFunction() const;
  BRIDGED_INLINE bool isAsyncFunction() const;
  BRIDGED_INLINE bool isCalleeConsumedFunction() const;
  BRIDGED_INLINE bool isBuiltinInteger() const;
  BRIDGED_INLINE bool isBuiltinFloat() const;
  BRIDGED_INLINE bool isBuiltinVector() const;
  BRIDGED_INLINE bool isBuiltinFixedArray() const;
  BRIDGED_INLINE bool isBox() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedASTType getBuiltinVectorElementType() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedCanType getBuiltinFixedArrayElementType() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedCanType getBuiltinFixedArraySizeType() const;
  BRIDGED_INLINE bool isBuiltinFixedWidthInteger(SwiftInt width) const;
  BRIDGED_INLINE bool isOptional() const;
  BRIDGED_INLINE bool isBuiltinType() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE OptionalBridgedDeclObj getNominalOrBoundGenericNominal() const;
  BRIDGED_INLINE TraitResult canBeClass() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE OptionalBridgedDeclObj getAnyNominal() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedASTType getInstanceTypeOfMetatype() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedASTType getStaticTypeOfDynamicSelf() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedASTType getSuperClassType() const;
  BRIDGED_INLINE MetatypeRepresentation getRepresentationOfMetatype() const;
  BRIDGED_INLINE BridgedOptionalInt getValueOfIntegerType() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedSubstitutionMap getContextSubstitutionMap() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedGenericSignature getInvocationGenericSignatureOfFunctionType() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedASTType subst(BridgedSubstitutionMap substMap) const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedConformance checkConformance(BridgedDeclObj proto) const;  
};

class BridgedCanType {
  swift::TypeBase * _Nullable type;

public:
  BRIDGED_INLINE BridgedCanType();
  BRIDGED_INLINE BridgedCanType(swift::CanType ty);
  BRIDGED_INLINE swift::CanType unbridged() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedASTType getRawType() const;
};

struct BridgedASTTypeArray {
  BridgedArrayRef typeArray;

  SwiftInt getCount() const { return SwiftInt(typeArray.Length); }

  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE
  BridgedASTType getAt(SwiftInt index) const;
};

struct BridgedConformance {
  void * _Nullable opaqueValue;

  BRIDGED_INLINE BridgedConformance(swift::ProtocolConformanceRef conformance);
  BRIDGED_INLINE swift::ProtocolConformanceRef unbridged() const;

  BridgedOwnedString getDebugDescription() const;
  BRIDGED_INLINE bool isConcrete() const;
  BRIDGED_INLINE bool isValid() const;
  BRIDGED_INLINE bool isSpecializedConformance() const;
  BRIDGED_INLINE bool isInheritedConformance() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedASTType getType() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedDeclObj getRequirement() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedConformance getGenericConformance() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedConformance getInheritedConformance() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedSubstitutionMap getSpecializedSubstitutions() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedConformance getAssociatedConformance(BridgedASTType assocType,
                                                                                 BridgedDeclObj proto) const;
};

struct BridgedConformanceArray {
  BridgedArrayRef pcArray;

  SwiftInt getCount() const { return SwiftInt(pcArray.Length); }

  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE
  BridgedConformance getAt(SwiftInt index) const;
};

struct BridgedSubstitutionMap {
  uint64_t storage[1];

  static SWIFT_IMPORT_UNSAFE BridgedSubstitutionMap get(BridgedGenericSignature genSig,
                                                        BridgedArrayRef replacementTypes);
  BRIDGED_INLINE BridgedSubstitutionMap(swift::SubstitutionMap map);
  BRIDGED_INLINE swift::SubstitutionMap unbridged() const;
  BRIDGED_INLINE BridgedSubstitutionMap();
  BridgedOwnedString getDebugDescription() const;
  BRIDGED_INLINE bool isEmpty() const;
  BRIDGED_INLINE bool isEqualTo(BridgedSubstitutionMap rhs) const;
  BRIDGED_INLINE bool hasAnySubstitutableParams() const;
  BRIDGED_INLINE SwiftInt getNumConformances() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedConformance getConformance(SwiftInt index) const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedASTTypeArray getReplacementTypes() const;
};

struct BridgedGenericSignature {
  const swift::GenericSignatureImpl * _Nullable impl;

  BRIDGED_INLINE swift::GenericSignature unbridged() const;
  BridgedOwnedString getDebugDescription() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedASTTypeArray getGenericParams() const;
  SWIFT_IMPORT_UNSAFE BRIDGED_INLINE BridgedASTType mapTypeIntoContext(BridgedASTType type) const;
};

struct BridgedFingerprint {
  uint64_t v1;
  uint64_t v2;

  BRIDGED_INLINE swift::Fingerprint unbridged() const;
};

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedPoundKeyword : uint8_t {
#define POUND_KEYWORD(NAME) BridgedPoundKeyword_##NAME,
#include "swift/AST/TokenKinds.def"
  BridgedPoundKeyword_None,
};

SWIFT_NAME("BridgedPoundKeyword.init(from:)")
BridgedPoundKeyword BridgedPoundKeyword_fromString(BridgedStringRef cStr);

//===----------------------------------------------------------------------===//
// MARK: #if handling
//===----------------------------------------------------------------------===//

/// Bridged version of IfConfigClauseRangeInfo::ClauseKind.
enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedIfConfigClauseKind : size_t {
  IfConfigActive,
  IfConfigInactive,
  IfConfigEnd
};

/// Bridged version of IfConfigClauseRangeInfo.
struct BridgedIfConfigClauseRangeInfo {
  swift::SourceLoc directiveLoc;
  swift::SourceLoc bodyLoc;
  swift::SourceLoc endLoc;
  BridgedIfConfigClauseKind kind;

  BRIDGED_INLINE swift::IfConfigClauseRangeInfo unbridged() const;
};

//===----------------------------------------------------------------------===//
// MARK: Plugins
//===----------------------------------------------------------------------===//

SWIFT_BEGIN_ASSUME_NONNULL

typedef void *PluginHandle;
typedef const void *PluginCapabilityPtr;

/// Set a capability data to the plugin object. Since the data is just a opaque
/// pointer, it's not used in AST at all.
void Plugin_setCapability(PluginHandle handle,
                          PluginCapabilityPtr _Nullable data);

/// Get a capability data set by \c Plugin_setCapability .
PluginCapabilityPtr _Nullable Plugin_getCapability(PluginHandle handle);

/// Lock the plugin. Clients should lock it during sending and recving the
/// response.
void Plugin_lock(PluginHandle handle);

/// Unlock the plugin.
void Plugin_unlock(PluginHandle handle);

/// Launch the plugin if it's not running.
bool Plugin_spawnIfNeeded(PluginHandle handle);

/// Sends the message to the plugin, returns true if there was an error.
/// Clients should receive the response  by \c Plugin_waitForNextMessage .
bool Plugin_sendMessage(PluginHandle handle, const BridgedData data);

/// Receive a message from the plugin.
bool Plugin_waitForNextMessage(PluginHandle handle, BridgedData *data);

SWIFT_END_ASSUME_NONNULL

SWIFT_END_NULLABILITY_ANNOTATIONS

#ifndef PURE_BRIDGING_MODE
// In _not_ PURE_BRIDGING_MODE, bridging functions are inlined and therefore
// included in the header file. This is because they rely on C++ headers that
// we don't want to pull in when using "pure bridging mode".
#include "ASTBridgingImpl.h"
#endif

#endif // SWIFT_AST_ASTBRIDGING_H
