//===- llvm/Support/Protobuf.h ----------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Protobuf wire (binary) format emitter.
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/STLForwardCompat.h"
#include "llvm/ADT/StringRef.h"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <limits>
#include <numeric>
#include <type_traits>

namespace llvm::protobuf {

namespace detail {
template <typename SourceType, typename DestinationType>
struct FitsInImpl
    : std::bool_constant<
          static_cast<long long>(std::numeric_limits<DestinationType>::min()) <=
              static_cast<long long>(std::numeric_limits<SourceType>::min()) &&
          std::numeric_limits<SourceType>::max() <=
              std::numeric_limits<DestinationType>::max()> {};
} // namespace detail

template <typename SourceType, typename DestinationType>
inline constexpr bool FitsIn =
    detail::FitsInImpl<SourceType, DestinationType>::value;

// Language Guide:
// "You must give each field in your message definition a number
//  between 1 and 536,870,911."
// Out of 32 bits of a tag, 29 are reserved for field number.
inline constexpr const std::int32_t MinField = 1;
inline constexpr const std::int32_t MaxField = 536'870'911;

// Protocol Buffers wire types as per documentation.
// Out of 32 bits of a tag, 3 are reserved for wire type.
enum class WireType : std::uint8_t {
  VarInt = 0,          // VARINT
  I64 = 1,             // I64
  LengthDelimited = 2, // LEN
  StartGroup = 3,      // SGROUP, deprecated
  EndGroup = 4,        // EGROUP, deprecated
  I32 = 5,             // I32
};

/// Function that converts two's complement integers into ZigZag encoding.
/// Documentation provides the following formula: (n << 1) ^ (n >> 63),
/// which boils down to circlular rotate left by 1.
inline std::uint64_t convertToZigZag(std::int64_t Value) {
  auto UnsignedValue = static_cast<std::uint64_t>(Value);
  if (Value >= 0) {
    return UnsignedValue << 1;
  }
  return ~(UnsignedValue << 1);
}

inline int calculateVarIntLength(std::uint64_t Value) {
  if (Value >> 7 == 0) {
    return 1;
  }
  if (Value >> 14 == 0) {
    return 2;
  }
  if (Value >> 21 == 0) {
    return 3;
  }
  if (Value >> 28 == 0) {
    return 4;
  }
  if (Value >> 35 == 0) {
    return 5;
  }
  if (Value >> 42 == 0) {
    return 6;
  }
  if (Value >> 49 == 0) {
    return 7;
  }
  if (Value >> 56 == 0) {
    return 8;
  }
  if (Value >> 63 == 0) {
    return 9;
  }
  return 10;
}

template <typename ValueType>
inline int calculateVarIntPackedLength(ArrayRef<ValueType> Values) {
  if constexpr (!std::is_integral_v<ValueType>) {
    static_assert(!sizeof(ValueType *),
                  "Only data of integral type can go into packed varint field");
  }
  return std::accumulate(Values.begin(), Values.end(), 0,
                         [&](int Length, ValueType Value) {
                           return Length + calculateVarIntLength(Value);
                         });
}

template <typename ValueType>
inline int calculateSignedVarIntPackedLength(ArrayRef<ValueType> Values) {
  if constexpr (!std::is_integral_v<ValueType>) {
    static_assert(!sizeof(ValueType *),
                  "Only data of integral type can go into packed varint field");
  }
  return std::accumulate(
      Values.begin(), Values.end(), 0, [&](int Length, ValueType Value) {
        return Length + calculateVarIntLength(convertToZigZag(Value));
      });
}

/// A wrapper over `to_underlying` that accepts integral values (returning them
/// as is), with a protobuf emitter-tailored error message.
template <typename T> inline constexpr auto toIntegral(T Value) {
  if constexpr (std::is_integral_v<T>) {
    return Value;
  } else if constexpr (std::is_enum_v<T>) {
    return to_underlying(Value);
  } else {
    static_assert(!sizeof(T *), "Protobuf expects field numbers and enum "
                                "values to be integers or enums");
  }
}

/// protobuf::Emitter - Protocol Buffers wire (binary) format emitter.
///
/// This emitter is intended for generating binary protobuf streams
/// according to a known schema (typically defined in .proto file). API aims
/// to reflect schema syntax to aid in that, and consist of two layers:
/// - low-level emitting functions, modeled after supported wire types;
/// - high-level emitting function, modeled after language types.
/// Both layers are user-facing, albeit higher-level is expected to be used
/// the most.
///
/// Emitter is abstracted away from how output byte stream is going to be
/// stored, accepting an output iterator instead.
/// All emitting functions accept field numbers as enum values on top of
/// plain integers. Users are encouraged to take advantage of that, minimizing
/// magic numbers in their code.
///
/// Wire format is described in Protocol buffers documentation at
/// https://protobuf.dev/programming-guides/encoding
/// Features:
/// - Variable-length integers (VARINT) - full support
/// - Fixed 64-bit numbers (I64)        - full support
/// - Length-delimited records (LEN)    - full support
/// - Groups (SGROUP and EGROUP)        - no support [1]
/// - Fixed 32-bit numbers (I32)        - full support
/// - Submessages                       - basic support [2]
/// - `optional`                        - not reflected in wire format [3]
/// - `repeated`                        - basic support [4]
/// - `oneof`                           - not reflected in wire format
/// - `packed`                          - full support
/// - Maps                              - basic support [2]
///
/// [1] Groups are a deprecated proto2 feature. It can be easily supported if
///     needed.
/// [2] Submessages are encoded as regular length-delimited records of bytes.
///     Maps are syntactic sugar over submessages. Refer to wire format
///     documentation for details on map representation, as Emitter doesn't
///     provide additional support for maps.
/// [3] Optional fields that don't hold a value are ignored by wire format.
/// [4] Repeated fields are simply emitted multiple times into the message.
///     However, packed repeated fields are encoded as length-delimited
///     records. Emitter provides additional API for packed fields.
///     In proto2, repeated field is packed if it has `[packed=true]` option.
///     In proto3, repeated fields are packed by default (unless type doesn't
///     support packing).
template <typename OutputIt> class Emitter {
public:
  static_assert(
      std::is_same_v<typename std::iterator_traits<OutputIt>::iterator_category,
                     std::output_iterator_tag>,
      "Protobuf emitter supports only output iterators");

  Emitter(OutputIt Output) : Output{Output} {}

  //
  // Primary low-level emitting functions
  //

  /// Emits a two's complement varint up to 64 bits wide.
  /// Matches the following protobuf types: `uint32`, `uint64`, `int32`,
  /// `int64`.
  ///
  /// Also matches enums, but `emitEnum` is recommended for those types.
  /// `bool` is handled in overload.
  template <typename FieldType>
  void emitVarInt(FieldType Field, std::uint64_t Value) {
    emitTag<WireType::VarInt>(toIntegral(Field));
    emitVarIntRaw(Value);
  }

  /// Emits a ZigZag-encoded varint up to 64 bits wide.
  /// Matches the following protobuf types: `sint32`, `sint64`.
  template <typename FieldType>
  void emitSignedVarInt(FieldType Field, std::int64_t Value) {
    emitTag<WireType::VarInt>(toIntegral(Field));
    emitVarIntRaw(convertToZigZag(Value));
  }

  /// Emits a length-delimited record.
  /// Matches the following protobuf types: `string`, `bytes`, embedded message.
  ///
  /// Also matches packed repeated fields, but functions with `Packed` suffix
  /// are recommended for those. Note that in proto3 all repeated fields are
  /// packed by default (unless they are of type that doesn't support packing).
  /// Overload is provided for string-like values.
  /// Note that protobuf mandates UTF-8 for strings.
  template <typename FieldType>
  void emitLen(FieldType Field, ArrayRef<std::byte> Value) {
    emitTag<WireType::LengthDelimited>(toIntegral(Field));
    emitVarIntRaw(Value.size());
    emitRawBytes(Value);
  }

  /// Emits a fixed 32-bit number.
  /// Matches the follwing protobuf types: `fixed32`, `sfixed32`, `float`.
  ///
  /// `float` is handled in overload.
  template <typename FieldType>
  void emitI32(FieldType Field, std::uint32_t Value) {
    emitTag<WireType::I32>(toIntegral(Field));
    emitRawBytes({reinterpret_cast<const std::byte *>(&Value), sizeof(Value)});
  }

  /// Emits a fixed 64-bit number.
  /// Matches the follwing protobuf types: `fixed64`, `sfixed64`, `double`.
  ///
  /// `double` is handled in overload.
  template <typename FieldType>
  void emitI64(FieldType Field, std::uint64_t Value) {
    emitTag<WireType::I64>(toIntegral(Field));
    emitRawBytes({reinterpret_cast<const std::byte *>(&Value), sizeof(Value)});
  }

  //
  // Additional overloads for low-level emitting functions
  //

  /// Emits a fixed 32-bit number.
  /// Matches `float` protobuf type.
  template <typename FieldType> void emitI32(FieldType Field, float Value) {
    emitI32(toIntegral(Field), bit_cast<std::uint32_t>(Value));
  }

  /// Emits a fixed 64-bit number.
  /// Matches `double` protobuf type.
  template <typename FieldType> void emitI64(FieldType Field, double Value) {
    emitI64(toIntegral(Field), bit_cast<std::uint64_t>(Value));
  }

  /// Emits a length-delimited record.
  /// A convenience overload for string-like values.
  ///
  /// Note that protobuf mandates UTF-8 for strings.
  template <typename FieldType> void emitLen(FieldType Field, StringRef Value) {
    emitLen(toIntegral(Field),
            ArrayRef(reinterpret_cast<const std::byte *>(Value.begin()),
                     reinterpret_cast<const std::byte *>(Value.end())));
  }

  //
  // Packed versions of low-level emitting functions
  //

  /// Emits a range of two's complement varints as a packed field.
  /// Matches `repeated` `[packed=true]` fields of the following types:
  /// `uint32`, `uint64`, `int32`, `int64`, `bool`.
  ///
  /// Note that in proto3 `repeated` fields are packed by default.
  /// Also matches enum types, but `emitEnumPacked` is recommended for those
  /// types.
  /// `bool` is handled in overload.
  template <typename FieldType, typename ItemType>
  void emitVarIntPacked(FieldType Field, ArrayRef<ItemType> Value) {
    static_assert(std::is_integral_v<ItemType>,
                  "Protobuf varint expects integer values");
    static_assert(
        std::numeric_limits<ItemType>::digits <= 64,
        "Protobuf doesn't natively support integers wider than 64 bits");

    emitTag<WireType::LengthDelimited>(toIntegral(Field));
    emitVarIntRaw(calculateVarIntPackedLength(
        ArrayRef<ItemType>{Value.begin(), Value.size()}));
    for (auto Item : Value) {
      emitVarIntRaw(static_cast<std::uint64_t>(Item));
    }
  }

  /// Emits a range of two's complement varints as a packed field.
  /// Matches `repeated` `[packed=true]` fields of the `bool` type.
  ///
  /// Note that in proto3 `repeated` fields are packed by default.
  template <typename FieldType>
  void emitVarIntPacked(FieldType Field, ArrayRef<bool> Value) {
    emitTag<WireType::LengthDelimited>(toIntegral(Field));
    emitVarIntRaw(calculateVarIntPackedLength(Value));
    for (auto Item : Value) {
      emitVarIntRaw(static_cast<std::uint64_t>(Item));
    }
  }

  /// Emits a range of ZigZag-encoded varints as a packed field.
  /// Matches `repeated` `[packed=true]` fields of the following types:
  /// `sint32`, `sint64`.
  ///
  /// Note that in proto3 `repeated` fields are packed by default.
  template <typename FieldType, typename ValueType>
  void emitSignedVarIntPacked(FieldType Field, ArrayRef<ValueType> Value) {
    static_assert(std::is_integral_v<ValueType>,
                  "Protobuf varint expects integer values");
    static_assert(
        std::numeric_limits<ValueType>::digits <= 64,
        "Protobuf doesn't natively support integers wider than 64 bits");

    emitTag<WireType::LengthDelimited>(toIntegral(Field));
    emitVarIntRaw(calculateSignedVarIntPackedLength(Value));
    for (auto Item : Value) {
      emitVarIntRaw(convertToZigZag(static_cast<std::int64_t>(Item)));
    }
  }

  /// Emits a range of 32-bit fixed numbers as a packed field.
  /// Matches `repeated` `[packed=true]` fields of the following types:
  /// `fixed32`, `sfixed32`, `float`.
  ///
  /// Note that in proto3 `repeated` fields are packed by default.
  template <typename FieldType, typename ItemType>
  void emitI32Packed(FieldType Field, ArrayRef<ItemType> Value) {
    if constexpr (std::is_integral_v<ItemType>) {
      static_assert(sizeof(ItemType) <= 4,
                    "Protobuf I32 wire type doesn't support integer types "
                    "wider than 32 bits");
    } else if constexpr (std::is_floating_point_v<ItemType>) {
      static_assert(std::is_same_v<ItemType, float>,
                    "Protobuf I32 wire type support only 'float' "
                    "floating-point type");
    } else {
      static_assert(!sizeof(ItemType *),
                    "Unsupported type for protobuf I32 wire type");
    }

    emitTag<WireType::LengthDelimited>(toIntegral(Field));
    emitVarIntRaw(Value.size() * sizeof(std::uint32_t));
    for (auto Item : Value) {
      std::uint32_t ItemU32;
      if constexpr (std::is_integral_v<ItemType>) {
        ItemU32 = static_cast<std::uint32_t>(Item);
      } else if constexpr (std::is_same_v<ItemType, float>) {
        ItemU32 = bit_cast<std::uint32_t>(Item);
      } else {
        static_assert(!sizeof(ItemType *), "Unexpected item type");
      }
      emitRawBytes(
          {reinterpret_cast<const std::byte *>(&ItemU32), sizeof(ItemU32)});
    }
  }

  /// Emits a range of 64-bit fixed numbers as a packed field.
  /// Matches `repeated` `[packed=true]` fields of the following types:
  /// `fixed64`, `sfixed64`, `double`.
  ///
  /// Note that in proto3 `repeated` fields are packed by default.
  template <typename FieldType, typename ValueType>
  void emitI64Packed(FieldType Field, ArrayRef<ValueType> Value) {
    if constexpr (std::is_integral_v<ValueType>) {
      static_assert(sizeof(ValueType) <= 8,
                    "Protobuf I64 wire type doesn't support integer types "
                    "wider than 64 bits");
    } else if constexpr (std::is_floating_point_v<ValueType>) {
      static_assert(std::is_same_v<ValueType, double>,
                    "Protobuf I64 wire type support only 'double' "
                    "floating-point type");
    } else {
      static_assert(!sizeof(ValueType *),
                    "Unsupported type for protobuf I64 wire type");
    }

    emitTag<WireType::LengthDelimited>(toIntegral(Field));
    emitVarIntRaw(Value.size() * sizeof(std::uint64_t));
    for (auto Item : Value) {
      std::uint64_t ItemU64;
      if constexpr (std::is_integral_v<ValueType>) {
        ItemU64 = static_cast<std::uint64_t>(Item);
      } else if constexpr (std::is_same_v<ValueType, double>) {
        ItemU64 = bit_cast<std::uint64_t>(Item);
      } else {
        static_assert(!sizeof(ValueType *), "Unexpected item type");
      }
      emitRawBytes(
          {reinterpret_cast<const std::byte *>(&ItemU64), sizeof(ItemU64)});
    }
  }

  //
  // High-level functions to handle enums
  //

  template <typename FieldType, typename ValueType>
  void emitEnum(FieldType Field, ValueType &&Value) {
    if constexpr (std::is_integral_v<ValueType>) {
      static_assert(std::numeric_limits<ValueType>::digits <= 32,
                    "Protobuf doesn't support enums wider than 32 bits");
    } else if constexpr (std::is_enum_v<ValueType>) {
      static_assert(
          FitsIn<std::underlying_type_t<ValueType>, std::int32_t>,
          "Protobuf doesn't support enums that doesn't fit into std::int32_t");
    } else {
      static_assert(!sizeof(ValueType *), "Not an enum or integral type");
    }

    auto IntValue = toIntegral(Value);
    assert(std::numeric_limits<std::int32_t>::min() <= IntValue &&
           IntValue <= std::numeric_limits<std::int32_t>::max() &&
           "Enum value has to fit into signed 32-bit integer");
    emitVarInt(toIntegral(Field), static_cast<std::uint64_t>(IntValue));
  }

  template <typename FieldType, typename ValueType>
  void emitEnumRepeated(FieldType Field, ValueType &&Value) {
    static_assert(sizeof(typename ValueType::value_type *),
                  "Value is expected to be a container, but its type lacks "
                  "'value_type' member");
    for (auto Item : Value) {
      emitEnum(Field, Item);
    }
  }

  template <typename FieldType, typename ValueType>
  void emitEnumPacked(FieldType Field, ValueType &&Value) {
    static_assert(
        sizeof(typename std::remove_reference_t<ValueType>::value_type *),
        "Value is expected to be a container, but its type lacks 'value_type' "
        "member");
    using ItemType = typename std::remove_reference_t<ValueType>::value_type;
    if constexpr (std::is_integral_v<ItemType>) {
      static_assert(std::numeric_limits<ItemType>::digits <= 32,
                    "Protobuf doesn't support enums wider than 32 bits");
    } else if constexpr (std::is_enum_v<ItemType>) {
      static_assert(
          FitsIn<std::underlying_type_t<ItemType>, std::int32_t>,
          "Protobuf doesn't support enums that doesn't fit into std::int32_t");
    } else {
      static_assert(!sizeof(ItemType *), "Not an enum or integral type");
    }

    emitTag<WireType::LengthDelimited>(toIntegral(Field));
    emitVarIntRaw(Value.size());
    for (auto Item : Value) {
      auto IntValue = toIntegral(Value);
      assert(std::numeric_limits<std::int32_t>::min() <= IntValue &&
             IntValue <= std::numeric_limits<std::int32_t>::max() &&
             "Enum value has to fit into signed 32-bit integer");
      emitVarIntRaw(static_cast<std::uint64_t>(Item));
    }
  }

private:
  /// Emits tag of a record.
  /// Tag is an unsigned 32-bit varint comprised of 3 lower bits for type,
  /// and 29 upper bits for field number, encoded as unsigned varint.
  template <WireType WType> void emitTag(std::int64_t Field) {
    if constexpr (WType != WireType::VarInt && WType != WireType::I64 &&
                  WType != WireType::LengthDelimited &&
                  WType != WireType::I32) {
      static_assert(!sizeof(decltype(WType) *), "Invalid protobuf wire type");
    }
    assert(MinField <= Field && Field <= MaxField &&
           "Field number is outside of valid range");

    std::uint32_t Tag =
        (static_cast<std::uint32_t>(Field) << 3) + to_underlying(WType);
    emitVarIntRaw(Tag);
  }

  /// Emits prepared integer payload as a varint,
  /// which is a little-endian base128 encoding.
  void emitVarIntRaw(std::uint64_t Value) {
    int BytesEmitted = 0;
    while (BytesEmitted <= 9) {
      std::uint8_t Payload = Value & 0b0111'1111;
      Value >>= 7;
      if (Value != 0) {
        Payload |= 0b1000'0000;
      }
      assert((BytesEmitted < 9 || (Payload & 0b1000'0000) == 0) &&
             "Emitting the last byte while continuation flag is set");
      *Output++ = Payload;
      ++BytesEmitted;
      if (Value == 0) {
        break;
      }
    }

    assert(BytesEmitted > 0 && "No bytes were emitted");
  }

  void emitRawBytes(ArrayRef<std::byte> Bytes) {
    for (auto Byte : Bytes) {
      *Output++ = static_cast<unsigned char>(Byte);
    }
  }

private:
  OutputIt Output;
};

template <typename OutputIt> Emitter(OutputIt) -> Emitter<OutputIt>;

template <typename OutputIt> class Builder {
public:
  Builder(OutputIt Output) : Emitter{Output} {}

  template <typename FieldType> void emitDouble(FieldType Field, double Value) {
    Emitter.emitI64(Field, Value);
  }

  template <typename FieldType>
  void emitDoubleRepeated(FieldType Field, ArrayRef<double> Value) {
    for (auto Item : Value) {
      Emitter.emitI64(Field, Item);
    }
  }

  template <typename FieldType>
  void emitDoublePacked(FieldType Field, ArrayRef<double> Value) {
    Emitter.emitI64Packed(Field, Value);
  }

  template <typename FieldType> void emitFloat(FieldType Field, float Value) {
    Emitter.emitI32(Field, Value);
  }

  template <typename FieldType>
  void emitFloatRepeated(FieldType Field, ArrayRef<float> Value) {
    for (auto Item : Value) {
      Emitter.emitI32(Field, Item);
    }
  }

  template <typename FieldType>
  void emitFloatPacked(FieldType Field, ArrayRef<float> Value) {
    Emitter.emitI32Packed(Field, Value);
  }

  template <typename FieldType>
  void emitInt32(FieldType Field, std::int32_t Value) {
    Emitter.emitVarInt(Field, Value);
  }

  template <typename FieldType, typename ValueType>
  void emitInt32Repeated(FieldType Field, ValueType &&Value) {
    static_assert(sizeof(typename ValueType::value_type *),
                  "Value is expected to be a container, but its type lacks "
                  "'value_type' member");
    using ItemType = typename ValueType::value_type;
    static_assert(FitsIn<ItemType, std::int32_t>,
                  "Item type doesn't fit into std::int32_t");

    for (auto Item : Value) {
      Emitter.emitVarInt(Field, Item);
    }
  }

  template <typename FieldType, typename ValueType>
  void emitInt32Packed(FieldType Field, ValueType &&Value) {
    static_assert(sizeof(typename ValueType::value_type *),
                  "Value is expected to be a container, but its type lacks "
                  "'value_type' member");
    using ItemType = typename ValueType::value_type;
    static_assert(FitsIn<ItemType, std::int32_t>,
                  "Item type doesn't fit into std::int32_t");

    Emitter.emitVarIntPacked(Field, ArrayRef{Value.begin(), Value.size()});
  }

  template <typename FieldType>
  void emitInt64(FieldType Field, std::int64_t Value) {
    Emitter.emitVarInt(Field, Value);
  }

  template <typename FieldType, typename ValueType>
  void emitInt64Repeated(FieldType Field, ValueType &&Value) {
    static_assert(sizeof(typename ValueType::value_type *),
                  "Value is expected to be a container, but its type lacks "
                  "'value_type' member");
    using ItemType = typename ValueType::value_type;
    static_assert(FitsIn<ItemType, std::int64_t>,
                  "Item type doesn't fit into std::int64_t");

    for (auto Item : Value) {
      Emitter.emitVarInt(Field, Item);
    }
  }

  template <typename FieldType, typename ValueType>
  void emitInt64Packed(FieldType Field, ValueType &&Value) {
    static_assert(sizeof(typename ValueType::value_type *),
                  "Value is expected to be a container, but its type lacks "
                  "'value_type' member");
    using ItemType = typename ValueType::value_type;
    static_assert(FitsIn<ItemType, std::int64_t>,
                  "Item type doesn't fit into std::int64_t");

    Emitter.emitVarIntPacked(Field, ArrayRef{Value.begin(), Value.size()});
  }

  template <typename FieldType>
  void emitUInt32(FieldType Field, std::uint32_t Value) {
    Emitter.emitVarInt(Field, Value);
  }

  template <typename FieldType, typename ValueType>
  void emitUInt32Repeated(FieldType Field, ValueType &&Value) {
    static_assert(
        sizeof(typename std::remove_reference_t<ValueType>::value_type *),
        "Value is expected to be a container, but its type lacks 'value_type' "
        "member");
    using ItemType = typename std::remove_reference_t<ValueType>::value_type;
    static_assert(FitsIn<ItemType, std::uint32_t>,
                  "Item type doesn't fit into std::uint32_t");

    for (auto Item : Value) {
      Emitter.emitVarInt(Field, Item);
    }
  }

  template <typename FieldType, typename ValueType>
  void emitUInt32Packed(FieldType Field, ValueType &&Value) {
    static_assert(
        sizeof(typename std::remove_reference_t<ValueType>::value_type *),
        "Value is expected to be a container, but its type lacks 'value_type' "
        "member");
    using ItemType = typename std::remove_reference_t<ValueType>::value_type;
    static_assert(FitsIn<ItemType, std::uint32_t>,
                  "Item type doesn't fit into std::uint32_t");

    Emitter.emitVarIntPacked(Field, ArrayRef{Value.begin(), Value.size()});
  }

  template <typename FieldType>
  void emitUInt64(FieldType Field, std::uint64_t Value) {
    Emitter.emitVarInt(Field, Value);
  }

  template <typename FieldType, typename ValueType>
  void emitUInt64Repeated(FieldType Field, ValueType &&Value) {
    static_assert(
        sizeof(typename std::remove_reference_t<ValueType>::value_type *),
        "Value is expected to be a container, but its type lacks 'value_type' "
        "member");
    using ItemType = typename std::remove_reference_t<ValueType>::value_type;
    static_assert(FitsIn<ItemType, std::uint64_t>,
                  "Item type doesn't fit into std::uint64_t");

    for (auto Item : Value) {
      Emitter.emitVarInt(Field, Item);
    }
  }

  template <typename FieldType, typename ValueType>
  void emitUInt64Packed(FieldType Field, ValueType &&Value) {
    static_assert(
        sizeof(typename std::remove_reference_t<ValueType>::value_type *),
        "Value is expected to be a container, but its type lacks 'value_type' "
        "member");
    using ItemType = typename std::remove_reference_t<ValueType>::value_type;
    static_assert(FitsIn<ItemType, std::uint64_t>,
                  "Item type doesn't fit into std::uint64_t");

    Emitter.emitVarIntPacked(Field, ArrayRef{Value.begin(), Value.size()});
  }

  template <typename FieldType>
  void emitSInt32(FieldType Field, std::int32_t Value) {
    Emitter.emitSignedVarInt(Field, Value);
  }

  template <typename FieldType, typename ValueType>
  void emitSInt32Repeated(FieldType Field, ValueType &&Value) {
    static_assert(sizeof(typename ValueType::value_type *),
                  "Value is expected to be a container, but its type lacks "
                  "'value_type' member");
    using ItemType = typename ValueType::value_type;
    static_assert(FitsIn<ItemType, std::int32_t>,
                  "Item type doesn't fit into std::int32_t");

    for (auto Item : Value) {
      Emitter.emitSignedVarInt(Field, Item);
    }
  }

  template <typename FieldType, typename ValueType>
  void emitSInt32Packed(FieldType Field, ValueType &&Value) {
    static_assert(sizeof(typename ValueType::value_type *),
                  "Value is expected to be a container, but its type lacks "
                  "'value_type' member");
    using ItemType = typename ValueType::value_type;
    static_assert(FitsIn<ItemType, std::int32_t>,
                  "Item type doesn't fit into std::int32_t");

    Emitter.emitSignedVarIntPacked(
        Field, ArrayRef<ItemType>{Value.begin(), Value.size()});
  }

  template <typename FieldType>
  void emitSInt64(FieldType Field, std::int64_t Value) {
    Emitter.emitSignedVarInt(Field, Value);
  }

  template <typename FieldType, typename ValueType>
  void emitSInt64Repeated(FieldType Field, ValueType &&Value) {
    static_assert(sizeof(typename ValueType::value_type *),
                  "Value is expected to be a container, but its type lacks "
                  "'value_type' member");
    using ItemType = typename ValueType::value_type;
    static_assert(FitsIn<ItemType, std::int64_t>,
                  "Item type doesn't fit into std::int64_t");

    for (auto Item : Value) {
      Emitter.emitSignedVarInt(Field, Item);
    }
  }

  template <typename FieldType, typename ValueType>
  void emitSInt64Packed(FieldType Field, ValueType &&Value) {
    static_assert(sizeof(typename ValueType::value_type *),
                  "Value is expected to be a container, but its type lacks "
                  "'value_type' member");
    using ItemType = typename ValueType::value_type;
    static_assert(FitsIn<ItemType, std::int64_t>,
                  "Item type doesn't fit into std::int64_t");

    Emitter.emitSignedVarIntPacked(
        Field, ArrayRef<ItemType>{Value.begin(), Value.size()});
  }

  template <typename FieldType>
  void emitFixed32(FieldType Field, std::uint32_t Value) {
    Emitter.emitI32(Field, Value);
  }

  template <typename FieldType, typename ValueType>
  void emitFixed32Repeated(FieldType Field, ValueType &&Value) {
    static_assert(
        sizeof(typename std::remove_reference_t<ValueType>::value_type *),
        "Value is expected to be a container, but its type lacks 'value_type' "
        "member");

    for (auto Item : Value) {
      Emitter.emitI32(Field, Item);
    }
  }

  template <typename FieldType, typename ValueType>
  void emitFixed32Packed(FieldType Field, ValueType &&Value) {
    static_assert(
        sizeof(typename std::remove_reference_t<ValueType>::value_type *),
        "Value is expected to be a container, but its type lacks 'value_type' "
        "member");
    using ItemType = typename std::remove_reference_t<ValueType>::value_type;
    static_assert(FitsIn<ItemType, std::uint32_t>,
                  "Item type doesn't fit into std::uint32_t");

    Emitter.emitI32Packed(Field, ArrayRef{Value.begin(), Value.size()});
  }

  template <typename FieldType>
  void emitFixed64(FieldType Field, std::uint64_t Value) {
    Emitter.emitI64(Field, Value);
  }

  template <typename FieldType, typename ValueType>
  void emitFixed64Repeated(FieldType Field, ValueType &&Value) {
    static_assert(
        sizeof(typename std::remove_reference_t<ValueType>::value_type *),
        "Value is expected to be a container, but its type lacks 'value_type' "
        "member");

    for (auto Item : Value) {
      Emitter.emitI64(Field, Item);
    }
  }

  template <typename FieldType, typename ValueType>
  void emitFixed64Packed(FieldType Field, ValueType &&Value) {
    static_assert(
        sizeof(typename std::remove_reference_t<ValueType>::value_type *),
        "Value is expected to be a container, but its type lacks 'value_type' "
        "member");
    using ItemType = typename std::remove_reference_t<ValueType>::value_type;
    static_assert(FitsIn<ItemType, std::uint64_t>,
                  "Item type doesn't fit into std::uint64_t");

    Emitter.emitI64Packed(Field, ArrayRef{Value.begin(), Value.size()});
  }

  template <typename FieldType>
  void emitSFixed32(FieldType Field, std::int32_t Value) {
    Emitter.emitI32(Field, static_cast<std::uint32_t>(Value));
  }

  template <typename FieldType, typename ValueType>
  void emitSFixed32Repeated(FieldType Field, ValueType &&Value) {
    static_assert(sizeof(typename ValueType::value_type *),
                  "Value is expected to be a container, but its type lacks "
                  "'value_type' member");

    for (auto Item : Value) {
      Emitter.emitI32(Field, static_cast<std::uint32_t>(Item));
    }
  }

  template <typename FieldType, typename ValueType>
  void emitSFixed32Packed(FieldType Field, ValueType &&Value) {
    static_assert(sizeof(typename ValueType::value_type *),
                  "Value is expected to be a container, but its type lacks "
                  "'value_type' member");
    using ItemType = typename ValueType::value_type;
    static_assert(FitsIn<ItemType, std::int32_t>,
                  "Item type doesn't fit into std::int32_t");

    Emitter.emitI32Packed(Field, ArrayRef{Value.begin(), Value.size()});
  }

  template <typename FieldType>
  void emitSFixed64(FieldType Field, std::int64_t Value) {
    Emitter.emitI64(Field, static_cast<std::uint64_t>(Value));
  }

  template <typename FieldType, typename ValueType>
  void emitSFixed64Repeated(FieldType Field, ValueType &&Value) {
    static_assert(sizeof(typename ValueType::value_type *),
                  "Value is expected to be a container, but its type lacks "
                  "'value_type' member");

    for (auto Item : Value) {
      Emitter.emitI64(Field, static_cast<std::uint64_t>(Item));
    }
  }

  template <typename FieldType, typename ValueType>
  void emitSFixed64Packed(FieldType Field, ValueType &&Value) {
    static_assert(sizeof(typename ValueType::value_type *),
                  "Value is expected to be a container, but its type lacks "
                  "'value_type' member");
    using ItemType = typename ValueType::value_type;
    static_assert(FitsIn<ItemType, std::int64_t>,
                  "Item type doesn't fit into std::int64_t");

    Emitter.emitI64Packed(Field, ArrayRef{Value.begin(), Value.size()});
  }

  template <typename FieldType> void emitBool(FieldType Field, bool Value) {
    Emitter.emitVarInt(Field, static_cast<std::uint64_t>(Value));
  }

  template <typename FieldType>
  void emitBoolRepeated(FieldType Field, ArrayRef<bool> Value) {
    for (auto Item : Value) {
      emitBool(Field, Item);
    }
  }

  template <typename FieldType>
  void emitBoolPacked(FieldType Field, ArrayRef<bool> Value) {
    Emitter.emitVarIntPacked(Field, Value);
  }

  template <typename FieldType>
  void emitString(FieldType Field, StringRef Value) {
    Emitter.emitLen(Field, Value);
  }

  template <typename FieldType, typename ValueType>
  void emitStringRepeated(FieldType Field, ValueType &&Value) {
    static_assert(sizeof(typename ValueType::value_type *),
                  "Value is expected to be a container, but its type lacks "
                  "'value_type' member");
    using ItemType = typename ValueType::value_type;
    static_assert(std::is_convertible_v<ItemType, StringRef>,
                  "Container of string-like items is expected, but they are "
                  "not convertible to StringRef");

    for (auto Item : Value) {
      emitString(Field, StringRef{Item});
    }
  }

  template <
      typename FieldType, typename ValueType,
      std::enable_if_t<!std::is_convertible_v<ValueType, StringRef>, int> = 0>
  void emitBytes(FieldType Field, ValueType &&Value) {
    if constexpr (!std::is_array_v<std::remove_reference_t<ValueType>>) {
      static_assert(
          sizeof(typename std::remove_reference_t<ValueType>::iterator *),
          "Value is expected to be a container, but its type lacks 'iterator' "
          "member");
      using IteratorType =
          typename std::remove_reference_t<ValueType>::iterator;
      static_assert(
          std::is_same_v<
              typename std::iterator_traits<IteratorType>::iterator_category,
              std::random_access_iterator_tag>,
          "Only contiguous containers are supported");
    }

    Emitter.emitLen(
        Field, ArrayRef(reinterpret_cast<const std::byte *>(std::begin(Value)),
                        reinterpret_cast<const std::byte *>(std::end(Value))));
  }

  template <typename FieldType>
  void emitBytes(FieldType Field, StringRef Value) {
    Emitter.emitLen(Field, Value);
  }

  template <typename FieldType, typename ValueType>
  void emitBytesRepeated(FieldType Field, ValueType &&Value) {
    static_assert(sizeof(typename ValueType::value_type *),
                  "Value is expected to be a container, but its type lacks "
                  "'value_type' member");

    for (auto Item : Value) {
      emitBytes(Field, Item);
    }
  }

  template <typename FieldType, typename ValueType>
  void emitSubmessage(FieldType Field, ValueType &&Value) {
    emitBytes(Field, std::forward<ValueType>(Value));
  }

  template <typename FieldType, typename ValueType>
  void emitSubmessageRepeated(FieldType Field, ValueType &&Value) {
    emitBytesRepeated(Field, std::forward<ValueType>(Value));
  }

  template <typename FieldType, typename ValueType>
  void emitEnum(FieldType Field, ValueType &&Value) {
    Emitter.emitEnum(Field, std::forward<Value>(Value));
  }

  template <typename FieldType, typename ValueType>
  void emitEnumRepeated(FieldType Field, ValueType &&Value) {
    Emitter.emitEnumRepeated(Field, std::forward<Value>(Value));
  }

  template <typename FieldType, typename ValueType>
  void emitEnumPacked(FieldType Field, ValueType &&Value) {
    Emitter.emitEnumPacked(Field, std::forward<Value>(Value));
  }

private:
  Emitter<OutputIt> Emitter;
};

template <typename OutputIt> Builder(OutputIt) -> Builder<OutputIt>;

} // namespace llvm::protobuf
