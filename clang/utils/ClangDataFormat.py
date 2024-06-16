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

# def __lldb_init_module(debugger, internal_dict):
#     debugger.HandleCommand("type recognizer add -F ClangDataFormat.test clang::QualType")
#     # debugger.HandleCommand(
#     #     "type summary add -F ClangDataFormat.SourceLocation_summary clang::SourceLocation"
#     # )
#     # debugger.HandleCommand(
#     #     "type summary add -F ClangDataFormat.QualType_summary clang::QualType"
#     # )

# def test(qualty, internal_dict):
#     print("Called python ClangDataFormat.test()")
#     return qualty.GetType()


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

    debugger.HandleCommand("type synthetic add --python-class ClangDataFormat.PointerIntPairProvider -x '^llvm::PointerIntPair<.+>$'")
    debugger.HandleCommand("type synthetic add --python-class ClangDataFormat.PunnedPointerProvider  -x '^llvm::detail::PunnedPointer<.+>$'")
    debugger.HandleCommand("type synthetic add --python-class ClangDataFormat.StringMapEntryProvider -x '^llvm::StringMapEntry<.+>$'")
    debugger.HandleCommand("type synthetic add --python-class ClangDataFormat.PointerUnionProvider   -x '^llvm::PointerUnion<.+>$'")
    debugger.HandleCommand("type synthetic add --python-class ClangDataFormat.QualTypeProvider       -x '^clang::QualType$'")
    debugger.HandleCommand("type synthetic add --python-class ClangDataFormat.TypeProvider           -x '^clang::Type$'")
    debugger.HandleCommand("type synthetic add --python-class ClangDataFormat.TemplateTypeParmTypeProvider -x '^clang::TemplateTypeParmType$'")
    # debugger.HandleCommand("type synthetic add --python-class ClangDataFormat.DeclProvider           -x '^clang::Decl$'")
    debugger.HandleCommand("type synthetic add --python-class ClangDataFormat.DeclContextProvider    -x '^clang::DeclContext$'")
    debugger.HandleCommand("type synthetic add --python-class ClangDataFormat.DeclarationNameProvider -x '^clang::DeclarationName$'")

    debugger.HandleCommand("type recognizer add -F ClangDataFormat.recognize_type 'clang::Type'")


class StringMapEntryProvider(SBSyntheticValueProvider):
    @trace
    def __init__(self, value: SBValue, internal_dict: Dict[Any, Any] = {}):
        self.value: SBValue = value
        self.num_children_underlying: int = 0

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
        print(f" index: {index}, self.num_children_underlying: {self.num_children_underlying}", end="")
        if index == self.num_children_underlying + 0:
            print(" returning Key", end= "")
            # Adding a child for Key
            return self.key_value
        if index == self.num_children_underlying + 1:
            print(" returning Value", end= "")
            # Adding a child for Value
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
        assert key_length_value.IsValid()
        raw_key_length: int = key_length_value.unsigned

        char_type: SBType = target.GetBasicType(lldb.eBasicTypeChar)
        assert char_type.IsValid()
        char_array_type: SBType = char_type.GetArrayType(raw_key_length + 1)
        assert char_array_type.IsValid()

        addr: SBAddress = value.addr
        assert addr.IsValid()
        addr.OffsetAddress(value.type.size)

        self.key_value: SBValue = target.CreateValueFromAddress("Key", addr, char_array_type)
        assert self.key_value.IsValid()
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


class DeclProvider(SBSyntheticValueProvider):
    @trace
    def __init__(self, value: SBValue, internal_dict: Dict[Any, Any] = {}):
        self.value = value

    @trace
    def has_children(self) -> bool:
        return True

    @trace
    def num_children(self, max_children: int) -> int:
        return min(1, max_children)

    @trace
    def get_child_index(self, name: str) -> int:
        print(f" name: {name}", end="")
        if name == "DeclKind":
            return 0
        return -1

    @trace
    def get_child_at_index(self, index: int) -> Optional[SBValue]:
        print(f" index: {index}", end="")
        if index == 0:
            return self.decl_kind_value
        return None

    @trace
    def update(self) -> bool:
        type: SBType = self.value.type
        if type.is_pointer:
            type: SBType = type.GetPointeeType()
        self.decl_kind_value: SBValue = self.value.GetChildMemberWithName("DeclKind")
        decl_kind_type: SBType = type.FindDirectNestedType("Kind")  # type: ignore
        if decl_kind_type.IsValid():
            self.decl_kind_value: SBValue = cast_value(decl_kind_type, self.decl_kind_value)
        self.decl_value: SBValue = self.value.GetChildMemberWithName("decl")
        return False


class DeclContextProvider(SBSyntheticValueProvider):
    class DeclInfo(NamedTuple):
        '''
        Name of the data member that holds bit-fields, e.g. EnumDeclBits.
        '''
        bits_name: str = ""

    decl_kind_mapping = {
        "Enum" : DeclInfo("EnumDeclBits"),
        "OMPThreadPrivate" : DeclInfo()
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
        assert decl_kind_name in self.decl_kind_mapping
        bits_name: str = self.decl_kind_mapping[decl_kind_name].bits_name
        if bits_name != "":
          self.derived_bits_value = anon_union_value.GetChildMemberWithName(bits_name)
          assert self.derived_bits_value is not None and self.derived_bits_value.IsValid()
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
            return self.info_decl_union_value
        return self.value.GetChildAtIndex(index)

    @trace
    def update(self) -> bool:
        union_value: SBValue = self.value.GetChildAtIndex(2)
        assert union_value.IsValid()
        raw_union_value = union_value.GetValueAsUnsigned()

        canonical_type_value: SBValue = self.value.GetChildMemberWithName("CanonicalType")
        assert canonical_type_value.IsValid()
        raw_canonical_type_value = union_value.GetValueAsUnsigned()

        if raw_union_value == raw_canonical_type_value and QualTypeProvider.is_unqualified(canonical_type_value):
            self.info_decl_union_value: SBValue = union_value.GetChildMemberWithName("CanTTPTInfo")
        else:
            self.info_decl_union_value: SBValue = union_value.GetChildMemberWithName("TTPDecl")

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
      "Enum": TypeInfo("clang::EnumType"),
      "TemplateTypeParm": TypeInfo("clang::TemplateTypeParmType"),
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
            return self.num_children_underlying
        return self.value.GetIndexOfChildWithName(name)

    @trace
    def get_child_at_index(self, index: int) -> Optional[SBValue]:
        print(f" index: {index}", end="")
        if index == 1:
            # Replacing big anonymous union with the right Bits
            return self.type_bits_value
        if index == self.num_children_underlying:
            return self.derived_bits_value
        return self.value.GetChildAtIndex(index)

    @trace
    def update(self) -> bool:
        self.num_children_underlying = self.value.GetNumChildren()

        anon_union_value: SBValue = self.value.children[1]
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
