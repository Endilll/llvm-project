"""lldb data formatters for clang classes.

Usage
--
import this file in your ~/.lldbinit by adding this line:

command script import /path/to/ClangDataFormat.py

After that, instead of getting this:

(lldb) p Tok.Loc
(clang::SourceLocation) $0 = {
  (unsigned int) ID = 123582
}

you'll get:

(lldb) p Tok.Loc
(clang::SourceLocation) $4 = "/usr/include/i386/_types.h:37:1" (offset: 123582, file, local)
"""

import lldb
from lldb import SBAddress, SBData, SBError, SBSyntheticValueProvider, SBTarget, SBType, SBValue

from typing import Any, Callable, Dict, NamedTuple, Optional, Tuple, Union


trace_call_depth : int = 0


def trace(func_or_cls_name: Union[Callable, str]) -> Callable:
    def format_big_number(number: int):
        num = "".join(reversed(str(number)))
        res = ""
        for i in range(0, len(num), 3):
            try:
                res += num[i]
                res += num[i+1]
                res += num[i+2]
                res += " "
            except IndexError:
                break
        return "".join(reversed(res))

    def outer_decorator(func: Callable) -> Callable:
        def decorator(*args, **kwargs) -> Callable:

            from time import perf_counter_ns
            global trace_call_depth

            if isinstance(func_or_cls_name, str):
                class_name = func_or_cls_name + "."
            elif len(args) == 0 or isinstance(args[0], (str, SBType)):
                class_name = ""
            elif isinstance(args[0], object):
                class_name = args[0].__class__.__name__ + "."
            elif isinstance(args[0], type):
                class_name = args[0].__name__ = "."
            else:
                print("\ntrace: something wrong passed as the first argument", end="")
                raise RuntimeError()

            func_name = func.__name__

            print("\n" + " "*13 + "  "*trace_call_depth + f" {class_name}{func_name}()", end="")
            trace_call_depth += 1
            start = perf_counter_ns()
            try:
                ret = func(*args, **kwargs)
            except Exception as e:
                import traceback
                print(f"\n{e}", end="")
                traceback.print_exc()
                raise e
            finally:
                end = perf_counter_ns()
                trace_call_depth -= 1
                print(f"\n{format_big_number(end - start).rjust(13)}" + "  "*trace_call_depth + f" {class_name}{func_name}() exit", end="")

            return ret

        return decorator

    if isinstance(func_or_cls_name, str):
        return outer_decorator
    else:
        return outer_decorator(func_or_cls_name)


def __lldb_init_module(debugger:lldb.SBDebugger, internal_dict: Dict[Any, Any]):
    # debugger.HandleCommand("type summary add -F ClangDataFormat.SourceLocation_summary clang::SourceLocation")
    # debugger.HandleCommand("type summary add -F ClangDataFormat.QualTypeProvider.get_summary clang::QualType")
    # debugger.HandleCommand("type summary add --python-function ClangDataFormat.StringMapEntryProvider.get_summary -x '^llvm::StringMapEntry<.+>$'")

    debugger.HandleCommand("type synthetic add --python-class ClangDataFormat.DeclContextProvider     -x '^clang::DeclContext$'")
    debugger.HandleCommand("type synthetic add --python-class ClangDataFormat.DeclarationNameProvider -x '^clang::DeclarationName$'")
    debugger.HandleCommand("type synthetic add --python-class ClangDataFormat.ExprProvider            -x '^clang::Expr$'")
    debugger.HandleCommand("type synthetic add --python-class ClangDataFormat.PointerIntPairProvider  -x '^llvm::PointerIntPair<.+>$'")
    debugger.HandleCommand("type synthetic add --python-class ClangDataFormat.PointerUnionProvider    -x '^llvm::PointerUnion<.+>$'")
    debugger.HandleCommand("type synthetic add --python-class ClangDataFormat.PunnedPointerProvider   -x '^llvm::detail::PunnedPointer<.+>$'")
    debugger.HandleCommand("type synthetic add --python-class ClangDataFormat.QualTypeProvider        -x '^clang::QualType$'")
    debugger.HandleCommand("type synthetic add --python-class ClangDataFormat.StmtProvider            -x '^clang::Stmt$'")
    debugger.HandleCommand("type synthetic add --python-class ClangDataFormat.StringMapEntryProvider  -x '^llvm::StringMapEntry<.+>$'")
    debugger.HandleCommand("type synthetic add --python-class ClangDataFormat.TemplateTypeParmTypeProvider -x '^clang::TemplateTypeParmType$'")
    debugger.HandleCommand("type synthetic add --python-class ClangDataFormat.TypeProvider            -x '^clang::Type$'")

    if debugger.GetVersionString().startswith("lldb version 20"):
      # clang::Decl has vtable, so it's handled by built-in LLDB machinery
      debugger.HandleCommand("type recognizer add -F ClangDataFormat.recognize_stmt 'clang::Expr'")
      debugger.HandleCommand("type recognizer add -F ClangDataFormat.recognize_stmt 'clang::Stmt'")
      debugger.HandleCommand("type recognizer add -F ClangDataFormat.recognize_type 'clang::Type'")


class StringMapEntryProvider(SBSyntheticValueProvider):
    @trace
    def __init__(self, value: SBValue, internal_dict: Dict[Any, Any] = {}):
        self.value: SBValue = value
        self.num_children_underlying: int = 0
        self.key_value: Optional[SBValue] = None
        self.value_value: SBValue = None  # type: ignore

    @trace
    def has_children(self) -> bool:
        return True

    @trace
    def num_children(self, max_children: int) -> int:
        # Adding 'Key' and 'Value' children
        return min(self.num_children_underlying + 2, max_children)

    @trace
    def get_child_index(self, name: str) -> int:
        print(f" name: {name}", end="")
        if name == "Key":
            return self.num_children_underlying + 0
        if name == "Value":
            return self.num_children_underlying + 1
        return self.value.GetIndexOfChildWithName(name)

    @trace
    def get_child_at_index(self, index: int) -> Optional[SBValue]:
        print(f" index: {index}", end="")
        if index == self.num_children_underlying + 0:
            # Adding a child for key
            return self.key_value
        if index == self.num_children_underlying + 1:
            # Adding a child for value
            return self.value_value
        return self.value.GetChildAtIndex(index)

    @trace
    def update(self) -> bool:
        self.num_children_underlying = self.value.GetNumChildren()

        value: SBValue = self.value
        if value.type.is_pointer:
            value = value.Dereference()
        target: SBTarget = value.target

        key_length_value: SBValue = value.GetChildMemberWithName("keyLength")
        if key_length_value.IsValid():
            raw_key_length: int = key_length_value.unsigned

            char_type: SBType = target.GetBasicType(lldb.eBasicTypeChar)
            assert char_type.IsValid()
            char_array_type: SBType = char_type.GetArrayType(raw_key_length + 1)
            assert char_array_type.IsValid()

            addr: SBAddress = value.addr
            assert addr.IsValid()
            addr.OffsetAddress(value.type.size)

            self.key_value = target.CreateValueFromAddress("Key", addr, char_array_type)
            assert self.key_value is not None and self.key_value.IsValid()
            self.key_value.SetSyntheticChildrenGenerated(True)

        base_value = value.GetChildAtIndex(0)
        assert base_value.IsValid()
        second_value = base_value.GetChildMemberWithName("second")
        assert second_value.IsValid()
        self.value_value: SBValue = second_value.Clone("Value")
        assert self.value_value.IsValid()
        self.value_value.SetSyntheticChildrenGenerated(True)

        return False

    @classmethod
    @trace("StringMapEntryProvider")
    def get_summary(cls, value: SBValue, internal_dict: Dict[Any, Any], options: lldb.SBTypeSummaryOptions) -> str:
        provider: StringMapEntryProvider = cls(value)
        provider.update()
        return f"{{{provider.key_value.summary} : {provider.value_value.summary}}}"


class DeclContextProvider(SBSyntheticValueProvider):
    class DeclInfo(NamedTuple):
        '''
        Name of the data member that holds bit-fields, e.g. EnumDeclBits.
        '''
        bits_name: str = ""

    decl_kind_mapping = {
        "Tag"                 : DeclInfo("TagDeclBits"),
        "Enum"                : DeclInfo("EnumDeclBits"),
        "Record"              : DeclInfo("RecordDeclBits"),
        "OMPDeclareReduction" : DeclInfo("OMPDeclareReductionDeclBits"),
        "Function"            : DeclInfo("FunctionDeclBits"),
        "CXXDeductionGuide"   : DeclInfo("FunctionDeclBits"),
        "CXXConstructor"      : DeclInfo("CXXConstructorDeclBits"),
        "ObjCMethod"          : DeclInfo("ObjCMethodDeclBits"),
        "ObjCContainer"       : DeclInfo("ObjCContainerDeclBits"),
        "LinkageSpec"         : DeclInfo("LinkageSpecDeclBits"),
        "Block"               : DeclInfo("BlockDeclBits"),
    }

    @trace
    def __init__(self, value: SBValue, internal_dict: Dict[Any, Any] = {}):
        self.value: SBValue = value
        self.num_children_underlying: int = 0
        self.decl_context_bits_value: SBValue = None  # type: ignore
        self.derived_bits_value: Optional[SBValue] = None

    @trace
    def has_children(self) -> bool:
        return True

    @trace
    def num_children(self, max_children: int) -> int:
        # Single anonymous union can be interpreted as (up to) 2 different Bits
        return min(self.num_children_underlying + 1, max_children)

    @trace
    def get_child_index(self, name: str) -> int:
        print(f" name: {name}", end="")
        if name == "DeclContextBits":
            return 1
        if self.derived_bits_value is not None and name == self.derived_bits_value.name:
            return self.num_children_underlying + 0
        return self.value.GetIndexOfChildWithName(name)

    @trace
    def get_child_at_index(self, index: int) -> Optional[SBValue]:
        print(f" index: {index}", end="")
        if index == 1:
            # Replacing big anonymous union with the right Bits
            return self.decl_context_bits_value
        if index == self.num_children_underlying + 0:
            # Adding a child for e.g. EnumDeclBits
            return self.derived_bits_value
        
        return self.value.GetChildAtIndex(index)

    @trace
    def update(self) -> bool:
        self.num_children_underlying = self.value.GetNumChildren()

        anon_union_value: SBValue = self.value.GetChildAtIndex(1)
        assert anon_union_value.type.name == "clang::DeclContext::(anonymous union)"
        self.decl_context_bits_value = anon_union_value.GetChildMemberWithName("DeclContextBits")
        assert self.decl_context_bits_value.IsValid()
        decl_kind_value: SBValue = self.decl_context_bits_value.GetChildMemberWithName('DeclKind')
        assert decl_kind_value.IsValid()
        decl_kind_name: str = decl_kind_value.value
        if decl_kind_name in self.decl_kind_mapping:
            bits_name: str = self.decl_kind_mapping[decl_kind_name].bits_name
            if bits_name != "":
                self.derived_bits_value = anon_union_value.GetChildMemberWithName(bits_name)
                assert self.derived_bits_value is not None
                assert self.derived_bits_value.IsValid()
        return False


class DeclarationNameProvider(SBSyntheticValueProvider):
    @trace
    def __init__(self, value: SBValue, internal_dict: Dict[Any, Any] = {}):
        self.value: SBValue = value
        self.num_children_underlying: int = 0
        self.name_kind_value: SBValue = None  # type: ignore

    @trace
    def has_children(self) -> bool:
        return True

    @trace
    def num_children(self, max_children: int) -> int:
        # Ptr member is very much like PointerIntPair, so it gives us 2 children
        return min(self.num_children_underlying + 1, max_children)

    @trace
    def get_child_index(self, name: str) -> int:
        print(f" name: {name}", end="")
        if name == "Pointer":
            return 0
        if name == "NameKind":
            return self.num_children_underlying + 0
        return self.value.GetIndexOfChildWithName(name)

    @trace
    def get_child_at_index(self, index: int) -> Optional[SBValue]:
        print(f" index: {index}", end="")
        if index == 0:
            # Replacing bit-packed pointer with normal pointer value
            return self.pointee_value
        if index == self.num_children_underlying + 0:
            # Adding the enum value stored in pointer bits
            return self.name_kind_value
        return self.value.GetChildAtIndex(index)

    @trace
    def update(self) -> bool:
        self.num_children_underlying = self.value.GetNumChildren()
        
        ptr_value: SBValue = self.value.GetChildAtIndex(0)
        assert ptr_value.IsValid()
        assert ptr_value.name == "Ptr"
        assert ptr_value.type.name == "uintptr_t"

        name_kind_enum: SBType = self.value.type.FindDirectNestedType("StoredNameKind")
        assert name_kind_enum.IsValid()
        ptr_mask_value: SBValue = get_enumerator_by_name(name_kind_enum, "PtrMask")
        raw_ptr_value: int = ptr_value.GetValueAsUnsigned()

        raw_int_mask: int = ptr_mask_value.unsigned
        raw_int_value: int = raw_ptr_value & raw_int_mask
        self.name_kind_value = create_value_from_raw_int("NameKind", raw_int_value, name_kind_enum, self.value.target)

        if self.name_kind_value.value == "StoredIdentifier":
            pointee_type: SBType = self.value.target.FindFirstType("clang::IdentifierInfo")
        elif self.name_kind_value.value in ("StoredObjCZeroArgSelector", "StoredObjCOneArgSelector"):
            pointee_type: SBType = self.value.target.FindFirstType("clang::Selector")
        elif self.name_kind_value.value in ("StoredCXXConstructorName", "StoredCXXDestructorName", "StoredCXXConversionFunctionName"):
            pointee_type: SBType = self.value.target.FindFirstType("clang::detail::CXXSpecialNameExtra")
        elif self.name_kind_value.value == "StoredCXXOperatorName":
            pointee_type: SBType = self.value.target.FindFirstType("clang::detail::CXXOperatorIdName")
        elif self.name_kind_value.value == "StoredDeclarationNameExtra":
            pointee_type: SBType = self.value.target.FindFirstType("clang::detail::DeclarationNameExtra")
        else:
            raise NotImplementedError(self.name_kind_value.value)
        assert pointee_type.IsValid()
        raw_pointer_mask: int = ~(ptr_mask_value.unsigned)
        raw_pointer_value: int = raw_ptr_value & raw_pointer_mask
        address: SBAddress = SBAddress(raw_pointer_value, self.value.target)
        self.pointee_value: SBValue = self.value.target.CreateValueFromAddress("Pointer", address, pointee_type)
        self.pointee_value.SetSyntheticChildrenGenerated(True)

        return False


class QualTypeProvider(SBSyntheticValueProvider):
    @trace
    def __init__(self, value: SBValue, internal_dict: Dict[Any, Any] = {}):
        self.value: SBValue = value

    @trace
    def num_children(self, max_children: int) -> int:
        return min(4, max_children)

    @trace
    def get_child_index(self, name: str) -> int:
        print(f" name: {name}", end="")
        if name == "Type":
            return 0
        if name == "LocalConstQualified":
            return 1
        if name == "LocalRestrictQualified":
            return 2
        if name == "LocalVolatileQualified":
            return 3
        if name == "Value":
            return 1001
        return -1

    @trace
    def get_child_at_index(self, index: int) -> Optional[SBValue]:
        print(f" index: {index}", end="")
        if index == 0:
            return self.type_value
        if index == 1:
            return self.is_const_value
        if index == 2:
            return self.is_restrict_value
        if index == 3:
            return self.is_volatile_value
        if index == 1001:
            return self.value.GetChildMemberWithName("Value")
        return None

    @trace
    def update(self) -> bool:
        pointer_int_pair = PointerIntPairProvider(self.value.GetChildMemberWithName("Value"))
        pointer_int_pair.update()
        self.type_value = pointer_int_pair.pointer_value.Dereference().Clone("Type")
        raw_int_value = pointer_int_pair.raw_int_value
        qualifiers_enum = self.value.target.FindFirstType("clang::Qualifiers::TQ")
        assert qualifiers_enum.IsValid()
        for enumerator in qualifiers_enum.enum_members:
            if enumerator.name == "Const":
                self.is_const_value: SBValue = create_bool_value("LocalConstQualified", raw_int_value & enumerator.unsigned, self.value.target)
            elif enumerator.name == "Restrict":
                self.is_restrict_value: SBValue = create_bool_value("LocalRestrictQualified", raw_int_value & enumerator.unsigned, self.value.target)
            elif enumerator.name == "Volatile":
                self.is_volatile_value: SBValue = create_bool_value("LocalVolatileQualified", raw_int_value & enumerator.unsigned, self.value.target)
        return False

    @trace
    def has_children(self) -> bool:
        return True

    @classmethod
    @trace("QualTypeProvider")
    def get_summary(cls, value: SBValue, internal_dict: Dict[Any, Any] = {}, options = None) -> str:
        return "Endill QualType placeholder"

    @classmethod
    @trace("QualTypeProvider")
    def is_unqualified(cls, value: SBValue) -> bool:
        assert value.type.name == "clang::QualType"
        pointer_int_pair = PointerIntPairProvider(value.GetChildMemberWithName("Value"))
        pointer_int_pair.update()
        raw_int_value = pointer_int_pair.raw_int_value
        qualifiers_enum = value.target.FindFirstType("clang::Qualifiers::TQ")
        assert qualifiers_enum.IsValid()
        result = False
        for enumerator in qualifiers_enum.enum_members:
            result = result or raw_int_value & enumerator.unsigned
        return result


class TemplateTypeParmTypeProvider(SBSyntheticValueProvider):
    @trace
    def __init__(self, value: SBValue, internal_dict: Dict[Any, Any] = {}):
        self.value = value
        self.info_decl_union_value: SBValue = None  # type: ignore

    @trace
    def has_children(self) -> bool:
        return True

    @trace
    def num_children(self, max_children: int) -> int:
        return self.value.GetNumChildren(max_children)

    @trace
    def get_child_index(self, name: str) -> int:
        print(f" name: {name}", end="")

        if self.info_decl_union_value is not None:
            assert self.info_decl_union_value.type.IsValid()
            if name == "CanTTPTInfo" and self.info_decl_union_value.type.name == "clang::TemplateTypeParmType::CanonicalTTPTInfo":
                return 2
            if name == "TTPDecl" and self.info_decl_union_value.type.name == "clang::TemplateTypeParmDecl *":
                return 2
        return self.value.GetIndexOfChildWithName(name)

    @trace
    def get_child_at_index(self, index: int) -> Optional[SBValue]:
        print(f" index: {index}", end="")
        if index == 2:
            # Replacing anonymous union with the active member of it
            return self.info_decl_union_value
        return self.value.GetChildAtIndex(index)

    @trace
    def update(self) -> bool:
        anon_union_value: SBValue = self.value.GetChildAtIndex(2)
        assert anon_union_value.IsValid()
        assert anon_union_value.type.name == "clang::TemplateTypeParmType::(anonymous union)"
        raw_union_value = anon_union_value.GetValueAsUnsigned()

        canonical_type_value: SBValue = self.value.GetChildMemberWithName("CanonicalType")
        assert canonical_type_value.IsValid()
        raw_canonical_type_value = anon_union_value.GetValueAsUnsigned()

        if raw_union_value == raw_canonical_type_value and QualTypeProvider.is_unqualified(canonical_type_value):
            self.info_decl_union_value: SBValue = anon_union_value.GetChildMemberWithName("CanTTPTInfo")
        else:
            self.info_decl_union_value: SBValue = anon_union_value.GetChildMemberWithName("TTPDecl")

        return False


class TypeProvider(SBSyntheticValueProvider):
    class TypeInfo(NamedTuple):
        '''
        Qualified name of the type. Typically used to call FindFirstType.
        '''
        qual_name: str

        '''
        Name of the data member that holds bit-fields, e.g. AutoTypeBits.
        '''
        bits_name: str = ""

    type_class_mapping = {
        "Adjusted"                  : TypeInfo("clang::AdjustedType"),
        "Decayed"                   : TypeInfo("clang::DecayedType"),
        "ConstantArray"             : TypeInfo("clang::ConstantArrayType", "ConstantArrayTypeBits"),
        "ArrayParameter"            : TypeInfo("clang::ArrayParameterType"),
        "DependentSizedArray"       : TypeInfo("clang::DependentSizedArrayType"),
        "IncompleteArray"           : TypeInfo("clang::IncompleteArrayType"),
        "VariableArray"             : TypeInfo("clang::VariableArrayType"),
        "Atomic"                    : TypeInfo("clang::AtomicType"),
        "Attributed"                : TypeInfo("clang::AttributedType", "AttributedTypeBits"),
        "BTFTagAttributed"          : TypeInfo("clang::BTFTagAttributedType"),
        "BitInt"                    : TypeInfo("clang::BitIntType"),
        "BlockPointer"              : TypeInfo("clang::BlockPointerType"),
        "CountAttributed"           : TypeInfo("clang::CountAttributedType", "CountAttributedTypeBits"),
        "Builtin"                   : TypeInfo("clang::BuiltinType", "BuiltinTypeBits"),
        "Complex"                   : TypeInfo("clang::ComplexType"),
        "Decltype"                  : TypeInfo("clang::DecltypeType"),
        "Auto"                      : TypeInfo("clang::AutoType", "AutoTypeBits"),
        "DeducedTemplateSpecialization" : TypeInfo("clang::DeducedTemplateSpecializationType"),
        "DependentAddressSpace"     : TypeInfo("clang::DependentAddressSpaceType"),
        "DependentBitInt"           : TypeInfo("clang::DependentBitIntType"),
        "DependentName"             : TypeInfo("clang::DependentNameType"),
        "DependentSizedExtVector"   : TypeInfo("clang::DependentSizedExtVectorType"),
        "DependentTemplateSpecialization" : TypeInfo("clang::DependentTemplateSpecializationType", "DependentTemplateSpecializationTypeBits"),
        "DependentVector"           : TypeInfo("clang::DependentVectorType", "VectorTypeBits"),
        "Elaborated"                : TypeInfo("clang::ElaboratedType", "ElaboratedTypeBits"),
        "FunctionNoProto"           : TypeInfo("clang::FunctionNoProtoType"),
        "FunctionProto"             : TypeInfo("clang::FunctionProtoType", "FunctionTypeBits"),
        "InjectedClassName"         : TypeInfo("clang::InjectedClassNameType"),
        "MacroQualified"            : TypeInfo("clang::MacroQualifiedType"),
        "ConstantMatrix"            : TypeInfo("clang::ConstantMatrixType"),
        "DependentSizedMatrix"      : TypeInfo("clang::DependentSizedMatrixType"),
        "MemberPointer"             : TypeInfo("clang::MemberPointerType"),
        "ObjCObjectPointer"         : TypeInfo("clang::ObjCObjectPointerType"),
        "ObjCObject"                : TypeInfo("clang::ObjCObjectType", "ObjCObjectTypeBits"),
        "ObjCInterface"             : TypeInfo("clang::ObjCInterfaceType"),
        "ObjCTypeParam"             : TypeInfo("clang::ObjCTypeParamType"),
        "PackExpansion"             : TypeInfo("clang::PackExpansionType", "PackExpansionTypeBits"),
        "PackIndexing"              : TypeInfo("clang::PackIndexingType"),
        "Paren"                     : TypeInfo("clang::ParenType"),
        "Pipe"                      : TypeInfo("clang::PipeType"),
        "Pointer"                   : TypeInfo("clang::PointerType"),
        "LValueReference"           : TypeInfo("clang::LValueReferenceType"),
        "RValueReference"           : TypeInfo("clang::RValueReferenceType"),
        "SubstTemplateTypeParmPack" : TypeInfo("clang::SubstTemplateTypeParmPackType", "SubstTemplateTypeParmPackTypeBits"),
        "SubstTemplateTypeParm"     : TypeInfo("clang::SubstTemplateTypeParmType", "SubstTemplateTypeParmTypeBits"),
        "Enum"                      : TypeInfo("clang::EnumType"),
        "Record"                    : TypeInfo("clang::RecordType"),
        "TemplateSpecialization"    : TypeInfo("clang::TemplateSpecializationType", "TemplateSpecializationTypeBits"),
        "TemplateTypeParm"          : TypeInfo("clang::TemplateTypeParmType"),
        "TypeOfExpr"                : TypeInfo("clang::TypeOfExprType", "TypeOfBits"),
        "TypeOf"                    : TypeInfo("clang::TypeOfType", "TypeOfBits"),
        "Typedef"                   : TypeInfo("clang::TypedefType", "TypedefBits"),
        "UnaryTransform"            : TypeInfo("clang::UnaryTransformType"),
        "UnresolvedUsing"           : TypeInfo("clang::UnresolvedUsingType"),
        "Using"                     : TypeInfo("clang::UsingType", "UsingBits"),
        "Vector"                    : TypeInfo("clang::VectorType", "VectorTypeBits"),
        "ExtVector"                 : TypeInfo("clang::ExtVectorType"),
    }

    @trace
    def __init__(self, value: SBValue, internal_dict: Dict[Any, Any] = {}):
        self.value: SBValue = value
        self.num_children_underlying: int = 0
        self.type_bits_value: SBValue = None  # type: ignore
        self.derived_bits_value: Optional[SBValue] = None

    @trace
    def num_children(self, max_children: int) -> int:
        # Single anonymous union can be interpreted as (up to) 2 different Bits
        return min(self.num_children_underlying + 1, max_children)

    @trace
    def get_child_index(self, name: str) -> int:
        print(f" name: {name}", end="")
        if name == "TypeBits":
            return 1
        if self.derived_bits_value is not None and name == self.derived_bits_value.name:
            return self.num_children_underlying + 0
        return self.value.GetIndexOfChildWithName(name)

    @trace
    def get_child_at_index(self, index: int) -> Optional[SBValue]:
        print(f" index: {index}", end="")
        if index == 1:
            # Replacing big anonymous union with the right Bits
            return self.type_bits_value
        if index == self.num_children_underlying + 0:
            return self.derived_bits_value
        return self.value.GetChildAtIndex(index)

    @trace
    def update(self) -> bool:
        self.num_children_underlying = self.value.GetNumChildren()

        anon_union_value: SBValue = self.value.GetChildAtIndex(1)
        assert anon_union_value.IsValid()
        assert anon_union_value.type.name == "clang::Type::(anonymous union)"

        self.type_bits_value: SBValue = anon_union_value.GetChildMemberWithName("TypeBits")
        assert self.type_bits_value.IsValid()
        type_class_value: SBValue = self.type_bits_value.GetChildMemberWithName('TC')
        assert type_class_value.IsValid()
        type_class_name: str = type_class_value.value
        assert type_class_name in self.type_class_mapping
        bits_name: str = self.type_class_mapping[type_class_name].bits_name
        if bits_name != "":
          self.derived_bits_value = anon_union_value.GetChildMemberWithName(bits_name)
          assert self.derived_bits_value is not None and self.derived_bits_value.IsValid()
        return False

    @trace
    def has_children(self) -> bool:
        return True


@trace("TypeRecognizers")
def recognize_type(value: SBValue, internal_dict) -> SBValue:
    prefer_synthetic: bool = value.GetPreferSyntheticValue()
    value.SetPreferSyntheticValue(False)

    anon_union_value: SBValue = value.children[1]
    assert anon_union_value.type.name == "clang::Type::(anonymous union)"
    type_bits: SBValue = anon_union_value.GetChildMemberWithName("TypeBits")
    assert type_bits.IsValid()
    type_class_value: SBValue = type_bits.GetChildMemberWithName('TC')
    assert type_class_value.IsValid()
    type_class_name: str = type_class_value.value
    assert type_class_name in TypeProvider.type_class_mapping
    qual_name: str = TypeProvider.type_class_mapping[type_class_name].qual_name
    derived_type: SBType = value.target.FindFirstType(qual_name)
    assert derived_type.IsValid()
    if value.type.IsPointerType():
        derived_type = derived_type.GetPointerType()
    elif value.type.IsReferenceType():
        derived_type = derived_type.GetReferenceType()
    derived_value: SBValue = value.target.CreateValueFromAddress(value.name, value.addr, derived_type)
    assert derived_value.IsValid()

    value.SetPreferSyntheticValue(prefer_synthetic)
    return derived_value


class StmtProvider(SBSyntheticValueProvider):
    class StmtInfo(NamedTuple):
        '''
        Qualified name of the type. Typically used to call FindFirstType.
        '''
        qual_name: str

        '''
        Name of the data member that holds bit-fields, e.g. IfStmtBits.
        '''
        bits_name: str = ""

    sclass_mapping = {
        # Statements
        "NullStmtClass"         : StmtInfo("clang::NullStmt", "NullStmtBits"),
        "CompoundStmtClass"     : StmtInfo("clang::CompoundStmt", "CompoundStmtBits"),
        "LabelStmtClass"        : StmtInfo("clang::LabelStmt", "LabelStmtBits"),
        "IfStmtClass"           : StmtInfo("clang::IfStmt", "IfStmtBits"),
        "SwitchStmtClass"       : StmtInfo("clang::SwitchStmt", "SwitchStmtBits"),
        "WhileStmtClass"        : StmtInfo("clang::WhileStmt", "WhileStmtBits"),
        "DoStmtClass"           : StmtInfo("clang::DoStmt", "DoStmtBits"),
        "ForStmtClass"          : StmtInfo("clang::ForStmt", "ForStmtBits"),
        "GotoStmtClass"         : StmtInfo("clang::GotoStmt", "GotoStmtBits"),
        "IndirectGotoStmtClass" : StmtInfo("clang::IndirectGotoStmt", "GotoStmtBits"),
        "ContinueStmtClass"     : StmtInfo("clang::ContinueStmt", "ContinueStmtBits"),
        "BreakStmtClass"        : StmtInfo("clang::BreakStmt", "BreakStmtBits"),
        "ReturnStmtClass"       : StmtInfo("clang::ReturnStmt", "ReturnStmtBits"),
        "DeclStmtClass"         : StmtInfo("class::DeclStmt"),
        "CaseStmtClass"         : StmtInfo("clang::CaseStmt", "SwitchCaseBits"),
        "DefaultStmtClass"      : StmtInfo("clang::DefaultStmt"),
        "CapturedStmtClass"     : StmtInfo("clang::CapturedStmt"),

        # Statements that might produce a value (for example, as the last non-null
        # statement in a GNU statement-expression).
        "LabelStmtClass"      : StmtInfo("clang::LabelStmt"),
        "AttributedStmtClass" : StmtInfo("clang::AttributedStmt", "AttributedStmtBits"),

        # Asm statements
        "GCCAsmStmtClass" : StmtInfo("clang::GCCAsmStmt"),
        "MSAsmStmtClass"  : StmtInfo("clang::MSAsmStmt"),

        # Obj-C statements
        "ObjCAtTryStmtClass"           : StmtInfo("clang::ObjCAtTryStmt"),
        "ObjCAtCatchStmtClass"         : StmtInfo("clang::ObjCAtCatchStmt"),
        "ObjCAtFinallyStmtClass"       : StmtInfo("clang::ObjCAtFinallyStmt"),
        "ObjCAtThrowStmtClass"         : StmtInfo("clang::ObjCAtThrowStmt"),
        "ObjCAtSynchronizedStmtClass"  : StmtInfo("clang::ObjCAtSynchronizedStmt"),
        "ObjCForCollectionStmtClass"   : StmtInfo("clang::ObjCForCollectionStmt"),
        "ObjCAutoreleasePoolStmtClass" : StmtInfo("clang::ObjCAutoreleasePoolStmt"),

        # C++ statements
        "CXXCatchStmtClass"    : StmtInfo("clang::CXXCatchStmt"),
        "CXXTryStmtClass"      : StmtInfo("clang::CXXTryStmt"),
        "CXXForRangeStmtClass" : StmtInfo("clang::CXXForRangeStmt"),

        # C++ Coroutines statements
        "CoroutineBodyStmtClass" : StmtInfo("clang::CoroutineBodyStmt"),
        "CoreturnStmtClass"      : StmtInfo("clang::CoreturnStmt"),

        # Expressions
        "PredefinedExprClass"        : StmtInfo("clang::PredefinedExpr", "PredefinedExprBits"),
        "SYCLUniqueStableNameExprClass" : StmtInfo("clang::SYCLUniqueStableNameExpr"),
        "DeclRefExprClass"           : StmtInfo("clang::DeclRefExpr", "DeclRefExprBits"),
        "IntegerLiteralClass"        : StmtInfo("clang::IntegerLiteral"),
        "FixedPointLiteralClass"     : StmtInfo("clang::FixedPointLiteral"),
        "FloatingLiteralClass"       : StmtInfo("clang::FloatingLiteral", "FloatingLiteralBits"),
        "ImaginaryLiteralClass"      : StmtInfo("clang::ImaginaryLiteral"),
        "StringLiteralClass"         : StmtInfo("clang::StringLiteral", "StringLiteralBits"),
        "CharacterLiteralClass"      : StmtInfo("clang::CharacterLiteral", "CharacterLiteralBits"),
        "ParenExprClass"             : StmtInfo("clang::ParenExpr"),
        "UnaryOperatorClass"         : StmtInfo("clang::UnaryOperator", "UnaryOperatorBits"),
        "OffsetOfExprClass"          : StmtInfo("clang::OffsetOfExpr"),
        "UnaryExprOrTypeTraitExprClass" : StmtInfo("clang::UnaryExprOrTypeTraitExpr", "UnaryExprOrTypeTraitExprBits"),
        "ArraySubscriptExprClass"    : StmtInfo("clang::ArraySubscriptExpr", "ArrayOrMatrixSubscriptExprBits"),
        "MatrixSubscriptExprClass"   : StmtInfo("clang::MatrixSubscriptExpr", "ArrayOrMatrixSubscriptExprBits"),
        "OMPIteratorExprClass"       : StmtInfo("clang::OMPIteratorExpr"),
        "CallExprClass"              : StmtInfo("clang::CallExpr", "CallExprBits"),
        "MemberExprClass"            : StmtInfo("clang::MemberExpr", "MemberExprBits"),
        "BinaryOperatorClass"        : StmtInfo("clang::BinaryOperator", "BinaryOperatorBits"),
        "CompoundAssignOperatorClass" : StmtInfo("clang::CompoundAssignOperator"),
        "ConditionalOperatorClass"   : StmtInfo("clang::ConditionalOperator"),
        "BinaryConditionalOperatorClass" : StmtInfo("clang::BinaryConditionalOperator"),
        "ImplicitCastExprClass"      : StmtInfo("clang::ImplicitCastExpr", "CastExprBits"),
        "CStyleCastExprClass"        : StmtInfo("clang::CStyleCastExpr"),
        "OMPArrayShapingExprClass"   : StmtInfo("clang::OMPArrayShapingExpr"),
        "CompoundLiteralExprClass"   : StmtInfo("clang::CompoundLiteralExpr"),
        "ExtVectorElementExprClass"  : StmtInfo("clang::ExtVectorElementExpr"),
        "InitListExprClass"          : StmtInfo("clang::InitListExpr", "InitListExprBits"),
        "DesignatedInitExprClass"    : StmtInfo("clang::DesignatedInitExpr"),
        "DesignatedInitUpdateExprClass" : StmtInfo("clang::DesignatedInitUpdateExpr"),
        "ImplicitValueInitExprClass" : StmtInfo("clang::ImplicitValueInitExpr"),
        "NoInitExprClass"            : StmtInfo("clang::NoInitExpr"),
        "ArrayInitLoopExprClass"     : StmtInfo("clang::ArrayInitLoopExpr"),
        "ArrayInitIndexExprClass"    : StmtInfo("clang::ArrayInitIndexExpr"),
        "ParenListExprClass"         : StmtInfo("clang::ParenListExpr", "ParenListExprBits"),
        "VAArgExprClass"             : StmtInfo("clang::VAArgExpr"),
        "GenericSelectionExprClass"  : StmtInfo("clang::GenericSelectionExpr", "GenericSelectionExprBits"),
        "PseudoObjectExprClass"      : StmtInfo("clang::PseudoObjectExpr", "PseudoObjectExprBits"),
        "SourceLocExprClass"         : StmtInfo("clang::SourceLocExpr", "SourceLocExprBits"),

        # Wrapper expressions
        "ConstantExprClass" : StmtInfo("clang::ConstantExpr", "ConstantExprBits"),

        # Atomic expressions
        "AtomicExprClass" : StmtInfo("clang::AtomicExpr"),

        # GNU Extensions.
        "AddrLabelExprClass" : StmtInfo("clang::AddrLabelExpr"),
        "StmtExprClass"      : StmtInfo("clang::StmtExpr", "StmtExprBits"),
        "ChooseExprClass"    : StmtInfo("clang::ChooseExpr"),
        "GNUNullExprClass"   : StmtInfo("clang::GNUNullExpr"),

        # C++ Expressions
        "CXXOperatorCallExprClass"        : StmtInfo("clang::CXXOperatorCallExpr", "CXXOperatorCallExprBits"),
        "CXXMemberCallExprClass"          : StmtInfo("clang::CXXMemberCallExpr"),
        "CXXRewrittenBinaryOperatorClass" : StmtInfo("clang::CXXRewrittenBinaryOperator", "CXXRewrittenBinaryOperatorBits"),
        "CXXStaticCastExprClass"          : StmtInfo("clang::CXXStaticCastExpr"),
        "CXXDynamicCastExprClass"         : StmtInfo("clang::CXXDynamicCastExpr"),
        "CXXReinterpretCastExprClass"     : StmtInfo("clang::CXXReinterpretCastExpr"),
        "CXXConstCastExprClass"           : StmtInfo("clang::CXXConstCastExpr"),
        "CXXAddrspaceCastExprClass"       : StmtInfo("clang::CXXAddrspaceCastExpr"),
        "CXXFunctionalCastExprClass"      : StmtInfo("clang::CXXFunctionalCastExpr"),
        "CXXTypeidExprClass"              : StmtInfo("clang::CXXTypeidExpr"),
        "UserDefinedLiteralClass"         : StmtInfo("clang::UserDefinedLiteral"),
        "CXXBoolLiteralExprClass"         : StmtInfo("clang::CXXBoolLiteralExpr", "CXXBoolLiteralExprBits"),
        "CXXNullPtrLiteralExprClass"      : StmtInfo("clang::CXXNullPtrLiteralExpr", "CXXNullPtrLiteralExprBits"),
        "CXXThisExprClass"                : StmtInfo("clang::CXXThisExpr", "CXXThisExprBits"),
        "CXXThrowExprClass"               : StmtInfo("clang::CXXThrowExpr", "CXXThrowExprBits"),
        "CXXDefaultArgExprClass"          : StmtInfo("clang::CXXDefaultArgExpr", "CXXDefaultArgExprBits"),
        "CXXDefaultInitExprClass"         : StmtInfo("clang::CXXDefaultInitExpr", "CXXDefaultInitExprBits"),
        "CXXScalarValueInitExprClass"     : StmtInfo("clang::CXXScalarValueInitExpr", "CXXScalarValueInitExprBits"),
        "CXXStdInitializerListExprClass"  : StmtInfo("clang::CXXStdInitializerListExpr"),
        "CXXNewExprClass"                 : StmtInfo("clang::CXXNewExpr", "CXXNewExprBits"),
        "CXXDeleteExprClass"              : StmtInfo("clang::CXXDeleteExpr", "CXXDeleteExprBits"),
        "CXXPseudoDestructorExprClass"    : StmtInfo("clang::CXXPseudoDestructorExpr"),
        "TypeTraitExprClass"              : StmtInfo("clang::TypeTraitExpr", "TypeTraitExprBits"),
        "ArrayTypeTraitExprClass"         : StmtInfo("clang::ArrayTypeTraitExpr"),
        "ExpressionTraitExprClass"        : StmtInfo("clang::ExpressionTraitExpr"),
        "DependentScopeDeclRefExprClass"  : StmtInfo("clang::DependentScopeDeclRefExpr", "DependentScopeDeclRefExprBits"),
        "CXXConstructExprClass"           : StmtInfo("clang::CXXConstructExpr", "CXXConstructExprBits"),
        "CXXInheritedCtorInitExprClass"   : StmtInfo("clang::CXXInheritedCtorInitExpr"),
        "CXXBindTemporaryExprClass"       : StmtInfo("clang::CXXBindTemporaryExpr"),
        "ExprWithCleanupsClass"           : StmtInfo("clang::ExprWithCleanups", "ExprWithCleanupsBits"),
        "CXXTemporaryObjectExprClass"     : StmtInfo("clang::CXXTemporaryObjectExpr"),
        "CXXUnresolvedConstructExprClass" : StmtInfo("clang::CXXUnresolvedConstructExpr", "CXXUnresolvedConstructExprBits"),
        "CXXDependentScopeMemberExprClass" : StmtInfo("clang::CXXDependentScopeMemberExpr", "CXXDependentScopeMemberExprBits"),
        "UnresolvedLookupExprClass"       : StmtInfo("clang::UnresolvedLookupExpr", "UnresolvedLookupExprBits"),
        "UnresolvedMemberExprClass"       : StmtInfo("clang::UnresolvedMemberExpr", "UnresolvedMemberExprBits"),
        "CXXNoexceptExprClass"            : StmtInfo("clang::CXXNoexceptExpr", "CXXNoexceptExprBits"),
        "PackExpansionExprClass"          : StmtInfo("clang::PackExpansionExpr"),
        "SizeOfPackExprClass"             : StmtInfo("clang::SizeOfPackExpr"),
        "PackIndexingExprClass"           : StmtInfo("clang::PackIndexingExpr"),
        "SubstNonTypeTemplateParmExprClass" : StmtInfo("clang::SubstNonTypeTemplateParmExpr", "SubstNonTypeTemplateParmExprBits"),
        "SubstNonTypeTemplateParmPackExprClass" : StmtInfo("clang::SubstNonTypeTemplateParmPackExpr"),
        "FunctionParmPackExprClass"       : StmtInfo("clang::FunctionParmPackExpr"),
        "MaterializeTemporaryExprClass"   : StmtInfo("clang::MaterializeTemporaryExpr"),
        "LambdaExprClass"                 : StmtInfo("clang::LambdaExpr", "LambdaExprBits"),
        "CXXFoldExprClass"                : StmtInfo("clang::CXXFoldExpr"),
        "CXXParenListInitExprClass"       : StmtInfo("clang::CXXParenListInitExpr"),

        # C++ Coroutines expressions
        "CoawaitExprClass"          : StmtInfo("clang::CoawaitExpr", "CoawaitBits"),
        "DependentCoawaitExprClass" : StmtInfo("clang::DependentCoawaitExpr"),
        "CoyieldExprClass"          : StmtInfo("clang::CoyieldExpr"),

        # C++20 Concepts expressions
        "ConceptSpecializationExprClass" : StmtInfo("clang::ConceptSpecializationExpr"),
        "RequiresExprClass"              : StmtInfo("clang::RequiresExpr", "RequiresExprBits"),

        # Obj-C Expressions
        "ObjCStringLiteralClass"           : StmtInfo("clang::ObjCStringLiteral"),
        "ObjCBoxedExprClass"               : StmtInfo("clang::ObjCBoxedExpr"),
        "ObjCArrayLiteralClass"            : StmtInfo("clang::ObjCArrayLiteral"),
        "ObjCDictionaryLiteralClass"       : StmtInfo("clang::ObjCDictionaryLiteral"),
        "ObjCEncodeExprClass"              : StmtInfo("clang::ObjCEncodeExpr"),
        "ObjCMessageExprClass"             : StmtInfo("clang::ObjCMessageExpr"),
        "ObjCSelectorExprClass"            : StmtInfo("clang::ObjCSelectorExpr"),
        "ObjCProtocolExprClass"            : StmtInfo("clang::ObjCProtocolExpr"),
        "ObjCIvarRefExprClass"             : StmtInfo("clang::ObjCIvarRefExpr"),
        "ObjCPropertyRefExprClass"         : StmtInfo("clang::ObjCPropertyRefExpr"),
        "ObjCIsaExprClass"                 : StmtInfo("clang::ObjCIsaExpr"),
        "ObjCIndirectCopyRestoreExprClass" : StmtInfo("clang::ObjCIndirectCopyRestoreExpr", "ObjCIndirectCopyRestoreExprBits"),
        "ObjCBoolLiteralExprClass"         : StmtInfo("clang::ObjCBoolLiteralExpr"),
        "ObjCSubscriptRefExprClass"        : StmtInfo("clang::ObjCSubscriptRefExpr"),
        "ObjCAvailabilityCheckExprClass"   : StmtInfo("clang::ObjCAvailabilityCheckExpr"),

        # Obj-C ARC Expressions
        "ObjCBridgedCastExprClass" : StmtInfo("clang::ObjCBridgedCastExpr"),

        # CUDA Expressions
        "CUDAKernelCallExprClass" : StmtInfo("clang::CUDAKernelCallExpr"),

        # Clang Extensions
        "ShuffleVectorExprClass"  : StmtInfo("clang::ShuffleVectorExpr"),
        "ConvertVectorExprClass"  : StmtInfo("clang::ConvertVectorExpr"),
        "BlockExprClass"          : StmtInfo("clang::BlockExpr"),
        "OpaqueValueExprClass"    : StmtInfo("clang::OpaqueValueExpr", "OpaqueValueExprBits"),
        "TypoExprClass"           : StmtInfo("clang::TypoExpr"),
        "RecoveryExprClass"       : StmtInfo("clang::RecoveryExpr"),
        "BuiltinBitCastExprClass" : StmtInfo("clang::BuiltinBitCastExpr"),

        # Microsoft Extensions
        "MSPropertyRefExprClass"       : StmtInfo("clang::MSPropertyRefExpr"),
        "MSPropertySubscriptExprClass" : StmtInfo("clang::MSPropertySubscriptExpr"),
        "CXXUuidofExprClass"           : StmtInfo("clang::CXXUuidofExpr"),
        "SEHTryStmtClass"              : StmtInfo("clang::SEHTryStmt"),
        "SEHExceptStmtClass"           : StmtInfo("clang::SEHExceptStmt"),
        "SEHFinallyStmtClass"          : StmtInfo("clang::SEHFinallyStmt"),
        "SEHLeaveStmtClass"            : StmtInfo("clang::SEHLeaveStmt"),
        "MSDependentExistsStmtClass"   : StmtInfo("clang::MSDependentExistsStmt"),

        # OpenCL Extensions
        "AsTypeExprClass" : StmtInfo("clang::AsTypeExpr"),

        # OpenMP Directives
        "OMPCanonicalLoopClass"              : StmtInfo("clang::OMPCanonicalLoop"),
        "OMPMetaDirectiveClass"              : StmtInfo("clang::OMPMetaDirective"),
        "OMPParallelDirectiveClass"          : StmtInfo("clang::OMPParallelDirective"),
        "OMPSimdDirectiveClass"              : StmtInfo("clang::OMPSimdDirective"),
        "OMPTileDirectiveClass"              : StmtInfo("clang::OMPTileDirective"),
        "OMPUnrollDirectiveClass"            : StmtInfo("clang::OMPUnrollDirective"),
        "OMPForDirectiveClass"               : StmtInfo("clang::OMPForDirective"),
        "OMPForSimdDirectiveClass"           : StmtInfo("clang::OMPForSimdDirective"),
        "OMPSectionsDirectiveClass"          : StmtInfo("clang::OMPSectionsDirective"),
        "OMPSectionDirectiveClass"           : StmtInfo("clang::OMPSectionDirective"),
        "OMPSingleDirectiveClass"            : StmtInfo("clang::OMPSingleDirective"),
        "OMPMasterDirectiveClass"            : StmtInfo("clang::OMPMasterDirective"),
        "OMPCriticalDirectiveClass"          : StmtInfo("clang::OMPCriticalDirective"),
        "OMPParallelForDirectiveClass"       : StmtInfo("clang::OMPParallelForDirective"),
        "OMPParallelForSimdDirectiveClass"   : StmtInfo("clang::OMPParallelForSimdDirective"),
        "OMPParallelMasterDirectiveClass"    : StmtInfo("clang::OMPParallelMasterDirective"),
        "OMPParallelSectionsDirectiveClass"  : StmtInfo("clang::OMPParallelSectionsDirective"),
        "OMPTaskDirectiveClass"              : StmtInfo("clang::OMPTaskDirective"),
        "OMPTaskyieldDirectiveClass"         : StmtInfo("clang::OMPTaskyieldDirective"),
        "OMPBarrierDirectiveClass"           : StmtInfo("clang::OMPBarrierDirective"),
        "OMPTaskwaitDirectiveClass"          : StmtInfo("clang::OMPTaskwaitDirective"),
        "OMPTaskgroupDirectiveClass"         : StmtInfo("clang::OMPTaskgroupDirective"),
        "OMPFlushDirectiveClass"             : StmtInfo("clang::OMPFlushDirective"),
        "OMPDepobjDirectiveClass"            : StmtInfo("clang::OMPDepobjDirective"),
        "OMPScanDirectiveClass"              : StmtInfo("clang::OMPScanDirective"),
        "OMPOrderedDirectiveClass"           : StmtInfo("clang::OMPOrderedDirective"),
        "OMPAtomicDirectiveClass"            : StmtInfo("clang::OMPAtomicDirective"),
        "OMPTargetDirectiveClass"            : StmtInfo("clang::OMPTargetDirective"),
        "OMPTargetDataDirectiveClass"        : StmtInfo("clang::OMPTargetDataDirective"),
        "OMPTargetEnterDataDirectiveClass"   : StmtInfo("clang::OMPTargetEnterDataDirective"),
        "OMPTargetExitDataDirectiveClass"    : StmtInfo("clang::OMPTargetExitDataDirective"),
        "OMPTargetParallelDirectiveClass"    : StmtInfo("clang::OMPTargetParallelDirective"),
        "OMPTargetParallelForDirectiveClass" : StmtInfo("clang::OMPTargetParallelForDirective"),
        "OMPTargetUpdateDirectiveClass"      : StmtInfo("clang::OMPTargetUpdateDirective"),
        "OMPTeamsDirectiveClass"             : StmtInfo("clang::OMPTeamsDirective"),
        "OMPCancellationPointDirectiveClass" : StmtInfo("clang::OMPCancellationPointDirective"),
        "OMPCancelDirectiveClass"            : StmtInfo("clang::OMPCancelDirective"),
        "OMPScopeDirectiveClass"             : StmtInfo("clang::OMPScopeDirective"),
        "OMPTaskLoopDirectiveClass"          : StmtInfo("clang::OMPTaskLoopDirective"),
        "OMPTaskLoopSimdDirectiveClass"      : StmtInfo("clang::OMPTaskLoopSimdDirective"),
        "OMPMasterTaskLoopDirectiveClass"    : StmtInfo("clang::OMPMasterTaskLoopDirective"),
        "OMPMasterTaskLoopSimdDirectiveClass"             : StmtInfo("clang::OMPMasterTaskLoopSimdDirective"),
        "OMPParallelMasterTaskLoopDirectiveClass"         : StmtInfo("clang::OMPParallelMasterTaskLoopDirective"),
        "OMPParallelMasterTaskLoopSimdDirectiveClass"     : StmtInfo("clang::OMPParallelMasterTaskLoopSimdDirective"),
        "OMPMaskedTaskLoopDirectiveClass"                 : StmtInfo("clang::OMPMaskedTaskLoopDirective"),
        "OMPMaskedTaskLoopSimdDirectiveClass"             : StmtInfo("clang::OMPMaskedTaskLoopSimdDirective"),
        "OMPParallelMaskedTaskLoopDirectiveClass"         : StmtInfo("clang::OMPParallelMaskedTaskLoopDirective"),
        "OMPParallelMaskedTaskLoopSimdDirectiveClass"     : StmtInfo("clang::OMPParallelMaskedTaskLoopSimdDirective"),
        "OMPDistributeDirectiveClass"                     : StmtInfo("clang::OMPDistributeDirective"),
        "OMPDistributeParallelForDirectiveClass"          : StmtInfo("clang::OMPDistributeParallelForDirective"),
        "OMPDistributeParallelForSimdDirectiveClass"      : StmtInfo("clang::OMPDistributeParallelForSimdDirective"),
        "OMPDistributeSimdDirectiveClass"                 : StmtInfo("clang::OMPDistributeSimdDirective"),
        "OMPTargetParallelForSimdDirectiveClass"          : StmtInfo("clang::OMPTargetParallelForSimdDirective"),
        "OMPTargetSimdDirectiveClass"                     : StmtInfo("clang::OMPTargetSimdDirective"),
        "OMPTeamsDistributeDirectiveClass"                : StmtInfo("clang::OMPTeamsDistributeDirective"),
        "OMPTeamsDistributeSimdDirectiveClass"            : StmtInfo("clang::OMPTeamsDistributeSimdDirective"),
        "OMPTeamsDistributeParallelForSimdDirectiveClass" : StmtInfo("clang::OMPTeamsDistributeParallelForSimdDirective"),
        "OMPTeamsDistributeParallelForDirectiveClass"     : StmtInfo("clang::OMPTeamsDistributeParallelForDirective"),
        "OMPTargetTeamsDirectiveClass"                    : StmtInfo("clang::OMPTargetTeamsDirective"),
        "OMPTargetTeamsDistributeDirectiveClass"          : StmtInfo("clang::OMPTargetTeamsDistributeDirective"),
        "OMPTargetTeamsDistributeParallelForDirectiveClass" : StmtInfo("clang::OMPTargetTeamsDistributeParallelForDirective"),
        "OMPTargetTeamsDistributeParallelForSimdDirectiveClass" : StmtInfo("clang::OMPTargetTeamsDistributeParallelForSimdDirective"),
        "OMPTargetTeamsDistributeSimdDirectiveClass"      : StmtInfo("clang::OMPTargetTeamsDistributeSimdDirective"),
        "OMPInteropDirectiveClass"                        : StmtInfo("clang::OMPInteropDirective"),
        "OMPDispatchDirectiveClass"                       : StmtInfo("clang::OMPDispatchDirective"),
        "OMPMaskedDirectiveClass"                         : StmtInfo("clang::OMPMaskedDirective"),
        "OMPParallelMaskedDirectiveClass"                 : StmtInfo("clang::OMPParallelMaskedDirective"),
        "OMPGenericLoopDirectiveClass"                    : StmtInfo("clang::OMPGenericLoopDirective"),
        "OMPTeamsGenericLoopDirectiveClass"               : StmtInfo("clang::OMPTeamsGenericLoopDirective"),
        "OMPTargetTeamsGenericLoopDirectiveClass"         : StmtInfo("clang::OMPTargetTeamsGenericLoopDirective"),
        "OMPParallelGenericLoopDirectiveClass"            : StmtInfo("clang::OMPParallelGenericLoopDirective"),
        "OMPTargetParallelGenericLoopDirectiveClass"      : StmtInfo("clang::OMPTargetParallelGenericLoopDirective"),
        "OMPErrorDirectiveClass"                          : StmtInfo("clang::OMPErrorDirective"),

        # OpenACC Constructs
        "OpenACCComputeConstructClass" : StmtInfo("clang::OpenACCComputeConstruct"),
        "OpenACCLoopConstructClass"    : StmtInfo("clang::OpenACCLoopConstruct"),
    }

    @trace
    def __init__(self, value: SBValue, internal_dict: Dict[Any, Any] = {}):
        self.value: SBValue = value
        self.num_children_underlying: int = 0
        self.stmt_bits_value: SBValue = None  # type: ignore
        self.derived_bits_value: Optional[SBValue] = None

    @trace
    def num_children(self, max_children: int) -> int:
        # Single anonymous union can be interpreted as (up to) 2 different Bits
        return min(self.num_children_underlying + 1, max_children)

    @trace
    def get_child_index(self, name: str) -> int:
        print(f" name: {name}", end="")
        if name == "StmtBits":
            return 0
        if self.derived_bits_value is not None and name == self.derived_bits_value.name:
            return self.num_children_underlying + 0
        return self.value.GetIndexOfChildWithName(name)

    @trace
    def get_child_at_index(self, index: int) -> Optional[SBValue]:
        print(f" index: {index}", end="")
        if index == 0:
            # Replacing big anonymous union with StmtBits, that are there for sure.
            return self.stmt_bits_value
        if index == self.num_children_underlying + 0:
            # Adding Bits of the derived class
            return self.derived_bits_value
        return self.value.GetChildAtIndex(index)

    @trace
    def update(self) -> bool:
        self.num_children_underlying = self.value.GetNumChildren()

        anon_union_value: SBValue = self.value.GetChildAtIndex(0)
        assert anon_union_value.IsValid()
        assert anon_union_value.type.name == "clang::Stmt::(anonymous union)"

        self.stmt_bits_value: SBValue = anon_union_value.GetChildMemberWithName("StmtBits")
        assert self.stmt_bits_value.IsValid()
        sclass_value: SBValue = self.stmt_bits_value.GetChildMemberWithName('sClass')
        assert sclass_value.IsValid()
        sclass_name: str = sclass_value.value
        assert sclass_name in self.sclass_mapping
        bits_name: str = self.sclass_mapping[sclass_name].bits_name
        if bits_name != "":
          self.derived_bits_value = anon_union_value.GetChildMemberWithName(bits_name)
          assert self.derived_bits_value is not None and self.derived_bits_value.IsValid()
        return False

    @trace
    def has_children(self) -> bool:
        return True


@trace("TypeRecognizers")
def recognize_stmt(value: SBValue, internal_dict) -> SBValue:
    prefer_synthetic: bool = value.GetPreferSyntheticValue()
    value.SetPreferSyntheticValue(False)

    stmt_value: SBValue = value
    if stmt_value.type.IsPointerType():
        stmt_value = stmt_value.Dereference()
    if value.type.name.startswith("clang::Expr"):
        value_stmt_value: SBValue = value.GetChildAtIndex(0)
        assert value_stmt_value.IsValid()
        assert value_stmt_value.type.name == "clang::ValueStmt"
        stmt_value = value_stmt_value.GetChildAtIndex(0)
    assert stmt_value.IsValid()
    assert stmt_value.type.name == "clang::Stmt"
    stmt_bits: SBValue = stmt_value.GetChildMemberWithName("StmtBits")
    assert stmt_bits.IsValid()
    sclass_value: SBValue = stmt_bits.GetChildMemberWithName('sClass')
    assert sclass_value.IsValid()
    sclass_name: str = sclass_value.value
    assert sclass_name in StmtProvider.sclass_mapping
    qual_name: str = StmtProvider.sclass_mapping[sclass_name].qual_name
    derived_type: SBType = value.target.FindFirstType(qual_name)
    assert derived_type.IsValid()

    if value.type.IsPointerType():
        derived_type = derived_type.GetPointerType()
    elif value.type.IsReferenceType():
        derived_type = derived_type.GetReferenceType()
    derived_value: SBValue = value.target.CreateValueFromAddress(value.name, value.addr, derived_type)
    assert derived_value.IsValid()

    value.SetPreferSyntheticValue(prefer_synthetic)
    return derived_value


class ExprProvider(SBSyntheticValueProvider):
    @trace
    def __init__(self, value: SBValue, internal_dict: Dict[Any, Any] = {}):
        self.value: SBValue = value
        self.num_children_underlying: int = 0
        self.expr_bits_value: SBValue = None  # type: ignore

    @trace
    def num_children(self, max_children: int) -> int:
        # Adding ExprBits
        return min(self.num_children_underlying + 1, max_children)

    @trace
    def get_child_index(self, name: str) -> int:
        print(f" name: {name}", end="")
        if name == "ExprBits":
            return self.num_children_underlying + 0
        return self.value.GetIndexOfChildWithName(name)

    @trace
    def get_child_at_index(self, index: int) -> Optional[SBValue]:
        print(f" index: {index}", end="")
        if index == self.num_children_underlying + 0:
            # Adding ExprBits
            self.expr_bits_value
        return self.value.GetChildAtIndex(index)

    @trace
    def update(self) -> bool:
        self.num_children_underlying = self.value.GetNumChildren()

        value_stmt_value: SBValue = self.value.GetChildAtIndex(0)
        assert value_stmt_value.IsValid()
        assert value_stmt_value.type.name == "clang::ValueStmt"
        stmt_value = value_stmt_value.GetChildAtIndex(0)
        assert stmt_value.IsValid()
        assert stmt_value.type.name == "clang::Stmt"
        self.expr_bits_value: SBValue = stmt_value.GetChildMemberWithName("ExprBits")
        assert self.expr_bits_value.IsValid()
        return False

    @trace
    def has_children(self) -> bool:
        return True


class PointerIntPairProvider(SBSyntheticValueProvider):
    @trace
    def __init__(self, value: SBValue, internal_dict: Dict[Any, Any] = {}):
        self.value = value

    @trace
    def num_children(self, max_children: int) -> int:
        return min(2, max_children)

    @trace
    def get_child_index(self, name: str) -> int:
        print(f" name: {name}", end="")
        if name == "Pointer":
            return 0
        if name == "Int":
            return 1
        if name == "$$dereference$$":
            return 1000
        if name == "Value":
            return 1001
        return -1

    @trace
    def get_child_at_index(self, index: int) -> Optional[SBValue]:
        print(f" index: {index}", end="")
        if index == 0:
            return self.pointer_value.Dereference()
        if index == 1:
            return self.int_value
        if index == 1000:
            return self.pointer_value.Dereference()
        if index == 1001:
            return self.value.GetChildMemberWithName("Value")
        return None

    @trace
    def update(self) -> bool:
        self.raw_pointer_value, self.raw_int_value = self.extract_raw_values(self.value)
        target: lldb.SBTarget = self.value.target

        address = SBAddress(self.raw_pointer_value, target)
        pointer_type: SBType = self.value.type.template_args[0]
        self.pointer_value: SBValue = target.CreateValueFromAddress("Pointer", address, pointer_type)
        self.pointer_value.SetSyntheticChildrenGenerated(True)

        self.int_value: SBValue = create_int_value("Int", self.raw_int_value, target)

        return False

    @trace
    def has_children(self):
        return True

    @classmethod
    @trace("PointerIntPairProvider")
    def extract_raw_values(cls, value: SBValue) -> Tuple[int, int]:
        assert value.type.name.startswith("llvm::PointerIntPair")

        punned_pointer: SBValue = value.GetChildMemberWithName("Value")
        raw_punned_pointer_value: int = PunnedPointerProvider.get_raw_pointer_value(punned_pointer)

        pointer_info: SBType = value.type.template_args[4]
        mask_and_shift_constants_enum: SBType = pointer_info.FindDirectNestedType("MaskAndShiftConstants")
        assert mask_and_shift_constants_enum.IsValid()

        pointer_mask_enumerator: lldb.SBTypeEnumMember = get_enumerator_by_name(mask_and_shift_constants_enum, "PointerBitMask")
        assert pointer_mask_enumerator is not None
        raw_pointer_mask: int = pointer_mask_enumerator.unsigned
        raw_pointer_value: int = raw_punned_pointer_value & raw_pointer_mask

        int_mask_enumerator: lldb.SBTypeEnumMember = get_enumerator_by_name(mask_and_shift_constants_enum, "ShiftedIntMask")
        assert int_mask_enumerator is not None
        raw_int_mask: int = int_mask_enumerator.unsigned
        int_shift_enumerator: lldb.SBTypeEnumMember = get_enumerator_by_name(mask_and_shift_constants_enum, "IntShift")
        assert int_shift_enumerator is not None
        raw_int_shift = int_shift_enumerator.unsigned
        raw_int_value: int = (raw_punned_pointer_value & raw_int_mask) >> raw_int_shift

        return (raw_pointer_value, raw_int_value)


class PunnedPointerProvider(SBSyntheticValueProvider):
    @trace
    def __init__(self, value: SBValue, internal_dict: Dict[Any, Any] = {}):
        self.value: SBValue = value

    @trace
    def num_children(self, max_children: int) -> int:
        return min(1, max_children)

    @trace
    def get_child_index(self, name: str) -> int:
        print(f" name: {name}", end="")
        if name == "Pointer":
            return 0
        if name == "$$dereference$$":
            return 1000
        if name == "Data":
            return 1001
        return -1

    @trace
    def get_child_at_index(self, index: int) -> Optional[SBValue]:
        print(f" index: {index}", end="")
        if index == 0:
            return self.pointer_value
        if index == 1000:
            return self.pointer_value.Dereference()
        if index == 1001:
            return self.value.GetChildMemberWithName("Data")
        return None

    @trace
    def update(self) -> bool:
        self.pointer_value = self.get_pointer_value(self.value)
        return False

    @trace
    def has_children(self):
        return True

    @classmethod
    @trace("PunnedPointerProvider")
    def get_raw_pointer_value(cls, value: SBValue) -> int:
        assert value.type.name.startswith("llvm::detail::PunnedPointer")

        pointer_type: SBType = value.type.template_args[0]
        data_value: SBValue = value.GetChildMemberWithName("Data")
        error = SBError()
        if pointer_type.size == 8:
            data: SBData = data_value.GetPointeeData(item_count=8)
            raw_pointer_value: int = data.GetUnsignedInt64(error, 0)
        elif pointer_type.size == 4:
            data: SBData = data_value.GetPointeeData(item_count=4)
            raw_pointer_value: int = data.GetUnsignedInt32(error, 0)
        else:
            raise NotImplementedError("Only 32-bit and 64-bit pointers are supported")
        assert error.success

        return raw_pointer_value

    @classmethod
    @trace("PunnedPointerProvider")
    def get_pointer_value(cls, value: SBValue) -> SBValue:
        assert value.type.name.startswith("llvm::detail::PunnedPointer")

        pointer_type: SBType = value.type.template_args[0]
        raw_pointer_value = cls.get_raw_pointer_value(value)
        address = SBAddress(raw_pointer_value, value.target)
        new_value: SBValue = value.target.CreateValueFromAddress("Pointer", address, pointer_type)
        new_value.SetSyntheticChildrenGenerated(True)
        return new_value


class PointerUnionProvider(SBSyntheticValueProvider):
    @trace
    def __init__(self, value: SBValue, internal_dict: Dict[Any, Any] = {}):
        self.value = value

    @trace
    def num_children(self, max_children: int) -> int:
        return min(1, max_children)

    @trace
    def get_child_index(self, name: str) -> int:
        print(f" name: {name}", end="")
        if name == "Pointer":
            return 0
        if name == "$$dereference$$":
            return 1000
        return -1

    @trace
    def get_child_at_index(self, index: int) -> Optional[SBValue]:
        print(f" index: {index}", end="")
        if index == 0:
            return self.pointer_value
        if index == 1000:
            return self.pointer_value
        return None

    @trace
    def update(self) -> bool:
        pointer_int_pair: SBValue = self.value.GetChildMemberWithName("Val")
        raw_pointer_value, raw_int_value = PointerIntPairProvider.extract_raw_values(pointer_int_pair)
        assert raw_int_value <= len(self.value.type.template_args)
        pointer_type: SBType = self.value.type.template_args[raw_int_value]
        address = SBAddress(raw_pointer_value, self.value.target)
        self.pointer_value: SBValue = self.value.target.CreateValueFromAddress("Pointer", address, pointer_type)
        self.pointer_value.SetSyntheticChildrenGenerated(True)
        return False

    @trace
    def has_children(self):
        return True


@trace
def get_enumerator_by_name(enum: SBType, name: str) -> Optional[lldb.SBTypeEnumMember]:
    for enumerator in enum.enum_members:
        if enumerator.name == name:
            return enumerator
    return None


@trace
def get_enumerator_by_value(enum: SBType, value: int) -> Optional[lldb.SBTypeEnumMember]:
    for enumerator in enum.enum_members:
        if enumerator.unsigned == value:
            return enumerator
    return None


@trace
def create_bool_value(name: str, raw_value: int, target: SBTarget) -> SBValue:
    return create_value_from_raw_int(name, raw_value, target.GetBasicType(lldb.eBasicTypeBool), target)


@trace
def create_int_value(name: str, raw_value: int, target: SBTarget) -> SBValue:
    return create_value_from_raw_int(name, raw_value, target.GetBasicType(lldb.eBasicTypeInt), target)


@trace
def create_value_from_raw_int(name: str, raw_value: int, type: SBType, target: SBTarget) -> SBValue:
    data = SBData.CreateDataFromUInt64Array(target.byte_order, target.addr_size, [raw_value])
    value: SBValue = target.CreateValueFromData(name, data, type)
    value.SetSyntheticChildrenGenerated(True)
    return value


@trace
def cast_value(type: SBType, value: SBValue, name: Optional[str] = None) -> SBValue:
    assert type.IsValid()
    name = name if name is not None else value.name
    address: SBAddress = value.addr
    new_value: SBValue = value.target.CreateValueFromAddress(name, address, type)
    new_value.SetSyntheticChildrenGenerated(True)
    return new_value


@trace
def cast_enum(type: SBType, value: SBValue) -> SBValue:
    assert type.IsValid()
    error = SBError()
    raw_value: int = value.GetValueAsUnsigned(error)
    assert error.success
    return create_value_from_raw_int(value.name, raw_value, type, value.target)
