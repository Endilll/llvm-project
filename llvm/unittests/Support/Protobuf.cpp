//===- unittest/Support/Protobuf.cpp --------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/Protobuf.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <cstdint>
#include <limits>

using namespace testing;

namespace {
struct ProtobufEmitterWrapper {
  llvm::SmallVector<std::uint8_t, 128> Output;
  llvm::protobuf::Builder<decltype(std::back_inserter(Output))> Builder{
      std::back_inserter(Output)};
};

enum class Fields {
  One = 1,
  Big = 100'000,
  TooBig = llvm::protobuf::MaxField + 1,
  TooBig2 = 1'000'000'000,
  Zero = 0,
  Negative = -1,
};
} // namespace

TEST(ProtobufEmitter, Bool) {
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitBool(Fields::One, false);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitBool(Fields::One, true);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x01));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitBoolRepeated(Fields::One,
                                     std::array{true, false, true});
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x08, 0x01, 0x08, 0x00, 0x08, 0x01));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitBoolPacked(Fields::One, std::array{true, false, true});
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x0A, 0x03, 0x01, 0x00, 0x01));
  }
}

TEST(ProtobufEmitter, String) {
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitString(Fields::One, "");
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x0A, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitString(Fields::One, "test");
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x0A, 0x04, 0x74, 0x65, 0x73, 0x74));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitString(Fields::One, "にほんご");
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x0A, 0x0C, 0xE3, 0x81, 0xAB, 0xE3, 0x81, 0xBB,
                            0xE3, 0x82, 0x93, 0xE3, 0x81, 0x94));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitStringRepeated(Fields::One, std::array{"", "test"});
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x0A, 0x00, 0x0A, 0x04, 0x74, 0x65, 0x73, 0x74));
  }
}

TEST(ProtobufEmitter, Bytes) {
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitBytes(Fields::One, std::array<std::uint8_t, 0>{});
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x0A, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitBytes(Fields::One, std::array<std::uint8_t, 1>{0xFF});
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x0A, 0x01, 0xFF));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitBytes(Fields::One,
                              std::array<std::uint8_t, 3>{0x00, 0x01, 0x02});
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x0A, 0x03, 0x00, 0x01, 0x02));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitBytesRepeated(
        Fields::One,
        std::array<std::array<std::uint8_t, 2>, 2>{{{0, 1}, {2, 3}}});
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x0A, 0x02, 0x00, 0x01, 0x0A, 0x02, 0x02, 0x03));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitBytes(Fields::One, "test");
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x0A, 0x04, 0x74, 0x65, 0x73, 0x74));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitBytesRepeated(Fields::One, std::array{"", "test"});
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x0A, 0x00, 0x0A, 0x04, 0x74, 0x65, 0x73, 0x74));
  }
}

TEST(ProtobufEmitter, Int32) {
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitInt32(Fields::One, 0);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitInt32(Fields::One, 1);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x01));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitInt32(Fields::One, 65537);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x81, 0x80, 0x04));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitInt32(Fields::One,
                              std::numeric_limits<std::int32_t>::max());
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x08, 0xFF, 0xFF, 0xFF, 0xFF, 0x07));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitInt32(Fields::One, -1);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                            0xFF, 0xFF, 0xFF, 0xFF, 0x01));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitInt32(Fields::One, -65537);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0xFF, 0xFF, 0xFB, 0xFF, 0xFF,
                                            0xFF, 0xFF, 0xFF, 0xFF, 0x01));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitInt32(Fields::One,
                              std::numeric_limits<std::int32_t>::min());
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x80, 0x80, 0x80, 0x80, 0xF8,
                                            0xFF, 0xFF, 0xFF, 0xFF, 0x01));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitInt32Repeated(Fields::One, std::array{0, -1, 65537});
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x00, 0x08, 0xFF, 0xFF, 0xFF,
                                            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                            0x01, 0x08, 0x81, 0x80, 0x04));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitInt32Packed(Fields::One, std::array{0, -1, 65537});
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x0A, 0x0E, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                            0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x81, 0x80, 0x04));
  }
}

TEST(ProtobufEmitter, Int64) {
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitInt64(Fields::One, 0);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitInt64(Fields::One, 1);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x01));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitInt64(Fields::One, 65537);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x81, 0x80, 0x04));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitInt64(Fields::One, 2'147'483'649);
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x08, 0x81, 0x80, 0x80, 0x80, 0x08));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitInt64(Fields::One,
                              std::numeric_limits<std::int64_t>::max());
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                            0xFF, 0xFF, 0xFF, 0x7F));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitInt64(Fields::One, -1);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                            0xFF, 0xFF, 0xFF, 0xFF, 0x01));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitInt64(Fields::One, -65537);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0xFF, 0xFF, 0xFB, 0xFF, 0xFF,
                                            0xFF, 0xFF, 0xFF, 0xFF, 0x01));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitInt64(Fields::One, -2'147'483'649);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7,
                                            0xFF, 0xFF, 0xFF, 0xFF, 0x01));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitInt64(Fields::One,
                              std::numeric_limits<std::int64_t>::min());
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x80, 0x80, 0x80, 0x80, 0x80,
                                            0x80, 0x80, 0x80, 0x80, 0x01));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitInt64Repeated(Fields::One,
                                      std::array{1l, -65537l, 2'147'483'649l});
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x08, 0x01, 0x08, 0xFF, 0xFF, 0xFB, 0xFF, 0xFF,
                            0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x08, 0x81, 0x80,
                            0x80, 0x80, 0x08));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitInt64Packed(Fields::One,
                                    std::array{1l, -65537l, 2'147'483'649l});
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x0A, 0x10, 0x01, 0xFF, 0xFF, 0xFB, 0xFF, 0xFF,
                            0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x81, 0x80, 0x80,
                            0x80, 0x08));
  }
}

TEST(ProtobufEmitter, UInt32) {
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitUInt32(Fields::One, 0);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitUInt32(Fields::One, 1);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x01));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitUInt32(Fields::One, 65537);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x81, 0x80, 0x04));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitUInt32(Fields::One,
                               std::numeric_limits<std::uint32_t>::max());
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x08, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitUInt32Repeated(Fields::One, std::array{0u, 1u, 65537u});
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x08, 0x00, 0x08, 0x01, 0x08, 0x81, 0x80, 0x04));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitUInt32Packed(Fields::One, std::array{0u, 1u, 65537u});
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x0A, 0x05, 0x00, 0x01, 0x81, 0x80, 0x04));
  }
}

TEST(ProtobufEmitter, UInt64) {
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitUInt64(Fields::One, 0);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitUInt64(Fields::One, 1);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x01));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitUInt64(Fields::One, 65537);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x81, 0x80, 0x04));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitUInt64(Fields::One, 2'147'483'649);
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x08, 0x81, 0x80, 0x80, 0x80, 0x08));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitUInt64(Fields::One,
                               std::numeric_limits<std::uint64_t>::max());
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                            0xFF, 0xFF, 0xFF, 0xFF, 0x01));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitUInt64Repeated(
        Fields::One, std::array{1ul, 65537ul, 2'147'483'649ul});
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x08, 0x01, 0x08, 0x81, 0x80, 0x04, 0x08, 0x81,
                            0x80, 0x80, 0x80, 0x08));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitUInt64Packed(Fields::One,
                                     std::array{1ul, 65537ul, 2'147'483'649ul});
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x0A, 0x09, 0x01, 0x81, 0x80, 0x04,
                                            0x81, 0x80, 0x80, 0x80, 0x08));
  }
}

TEST(ProtobufEmitter, SInt32) {
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSInt32(Fields::One, 0);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSInt32(Fields::One, 1);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x02));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSInt32(Fields::One, 65537);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x82, 0x80, 0x08));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSInt32(Fields::One,
                               std::numeric_limits<std::int32_t>::max());
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x08, 0xFE, 0xFF, 0xFF, 0xFF, 0x0F));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSInt32(Fields::One, -1);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x01));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSInt32(Fields::One, -65537);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x81, 0x80, 0x08));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSInt32(Fields::One,
                               std::numeric_limits<std::int32_t>::min());
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x08, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSInt32Repeated(Fields::One, std::array{0, -1, 65537});
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x08, 0x00, 0x08, 0x01, 0x08, 0x82, 0x80, 0x08));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSInt32Packed(Fields::One, std::array{0, -1, 65537});
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x0A, 0x05, 0x00, 0x01, 0x82, 0x80, 0x08));
  }
}

TEST(ProtobufEmitter, SInt64) {
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSInt64(Fields::One, 0);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSInt64(Fields::One, 1);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x02));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSInt64(Fields::One, 65537);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x82, 0x80, 0x08));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSInt64(Fields::One, 2'147'483'649);
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x08, 0x82, 0x80, 0x80, 0x80, 0x10));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSInt64(Fields::One,
                               std::numeric_limits<std::int64_t>::max());
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF,
                                            0xFF, 0xFF, 0xFF, 0xFF, 0x01));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSInt64(Fields::One, -1);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x01));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSInt64(Fields::One, -65537);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0x81, 0x80, 0x08));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSInt64(Fields::One, -2'147'483'649);
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x08, 0x81, 0x80, 0x80, 0x80, 0x10));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSInt64(Fields::One,
                               std::numeric_limits<std::int64_t>::min());
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x08, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                            0xFF, 0xFF, 0xFF, 0xFF, 0x01));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSInt64Repeated(Fields::One,
                                       std::array{1l, -65537l, 2'147'483'649l});
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x08, 0x02, 0x08, 0x81, 0x80, 0x08, 0x08, 0x82,
                            0x80, 0x80, 0x80, 0x10));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSInt64Packed(Fields::One,
                                     std::array{1l, -65537l, 2'147'483'649l});
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x0A, 0x09, 0x02, 0x81, 0x80, 0x08,
                                            0x82, 0x80, 0x80, 0x80, 0x10));
  }
}

TEST(ProtobufEmitter, Fixed32) {
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitFixed32(Fields::One, 0);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x0D, 0x00, 0x00, 0x00, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitFixed32(Fields::One, 1);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x0D, 0x01, 0x00, 0x00, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitFixed32(Fields::One, 65537);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x0D, 0x01, 0x00, 0x01, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitFixed32(Fields::One,
                                std::numeric_limits<std::uint32_t>::max());
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x0D, 0xFF, 0xFF, 0xFF, 0xFF));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitFixed32Repeated(Fields::One,
                                        std::array{0u, 1u, 65537u});
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x0D, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x01, 0x00,
                            0x00, 0x00, 0x0D, 0x01, 0x00, 0x01, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitFixed32Packed(Fields::One, std::array{0u, 1u, 65537u});
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x0A, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
                            0x00, 0x00, 0x01, 0x00, 0x01, 0x00));
  }
}

TEST(ProtobufEmitter, Fixed64) {
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitFixed64(Fields::One, 0);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x09, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitFixed64(Fields::One, 1);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x09, 0x01, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitFixed64(Fields::One, 65537);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x09, 0x01, 0x00, 0x01, 0x00, 0x00,
                                            0x00, 0x00, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitFixed64(Fields::One, 2'147'483'649);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x09, 0x01, 0x00, 0x00, 0x80, 0x00,
                                            0x00, 0x00, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitFixed64(Fields::One,
                                std::numeric_limits<std::uint64_t>::max());
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x09, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                            0xFF, 0xFF, 0xFF));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitFixed64Repeated(
        Fields::One, std::array{1ul, 65537ul, 2'147'483'649ul});
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x09, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x09, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x09, 0x01, 0x00, 0x00, 0x80, 0x00,
                            0x00, 0x00, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitFixed64Packed(
        Fields::One, std::array{1ul, 65537ul, 2'147'483'649ul});
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x0A, 0x18, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x01, 0x00, 0x00, 0x80, 0x00, 0x00,
                            0x00, 0x00));
  }
}

TEST(ProtobufEmitter, SFixed32) {
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSFixed32(Fields::One, 0);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x0D, 0x00, 0x00, 0x00, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSFixed32(Fields::One, 1);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x0D, 0x01, 0x00, 0x00, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSFixed32(Fields::One, 65537);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x0D, 0x01, 0x00, 0x01, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSFixed32(Fields::One,
                                 std::numeric_limits<std::int32_t>::max());
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x0D, 0xFF, 0xFF, 0xFF, 0x7F));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSFixed32(Fields::One, -1);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x0D, 0xFF, 0xFF, 0xFF, 0xFF));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSFixed32(Fields::One, -65537);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x0D, 0xFF, 0xFF, 0xFE, 0xFF));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSFixed32(Fields::One,
                                 std::numeric_limits<std::int32_t>::min());
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x0D, 0x00, 0x00, 0x00, 0x80));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSFixed32Repeated(Fields::One, std::array{0, -1, 65537});
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x0D, 0x00, 0x00, 0x00, 0x00, 0x0D, 0xFF, 0xFF,
                            0xFF, 0xFF, 0x0D, 0x01, 0x00, 0x01, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSFixed32Packed(Fields::One, std::array{0, -1, 65537});
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x0A, 0x0C, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
                            0xFF, 0xFF, 0x01, 0x00, 0x01, 0x00));
  }
}

TEST(ProtobufEmitter, SFixed64) {
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSFixed64(Fields::One, 0);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x09, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSFixed64(Fields::One, 1);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x09, 0x01, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSFixed64(Fields::One, 65537);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x09, 0x01, 0x00, 0x01, 0x00, 0x00,
                                            0x00, 0x00, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSFixed64(Fields::One, 2'147'483'649);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x09, 0x01, 0x00, 0x00, 0x80, 0x00,
                                            0x00, 0x00, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSFixed64(Fields::One,
                                 std::numeric_limits<std::int64_t>::max());
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x09, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                            0xFF, 0xFF, 0x7F));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSFixed64(Fields::One, -1);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x09, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                            0xFF, 0xFF, 0xFF));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSFixed64(Fields::One, -65537);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x09, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF,
                                            0xFF, 0xFF, 0xFF));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSFixed64(Fields::One, -2'147'483'649);
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x09, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF,
                                            0xFF, 0xFF, 0xFF));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSFixed64(Fields::One,
                                 std::numeric_limits<std::int64_t>::min());
    EXPECT_THAT(Wrapper.Output, ElementsAre(0x09, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x80));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSFixed64Repeated(
        Fields::One, std::array{1l, -65537l, 2'147'483'649l});
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x09, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x09, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF,
                            0xFF, 0xFF, 0x09, 0x01, 0x00, 0x00, 0x80, 0x00,
                            0x00, 0x00, 0x00));
  }
  {
    ProtobufEmitterWrapper Wrapper;
    Wrapper.Builder.emitSFixed64Packed(Fields::One,
                                       std::array{1l, -65537l, 2'147'483'649l});
    EXPECT_THAT(Wrapper.Output,
                ElementsAre(0x0A, 0x18, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF,
                            0xFF, 0xFF, 0x01, 0x00, 0x00, 0x80, 0x00, 0x00,
                            0x00, 0x00));
  }
}
