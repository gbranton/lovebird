#include "lovebird/unicode/utf8.hpp"

#include <string_view>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace lovebird {

using ::testing::StrEq;
using ::testing::ThrowsMessage;

TEST(Utf8Test, SequenceLength) {
    auto test_case = [](std::string_view sequence, int expected) {
        EXPECT_EQ(expected, utf8_sequence_length(sequence.at(0)));
    };
    test_case("g", 1);
    test_case("λ", 2);
    test_case("☃", 3);
    test_case("𝄞", 4);
}

TEST(Utf8Test, InvalidLeadByte) {
    auto test_case = [](char lead_byte) {
        EXPECT_THAT([&] { utf8_sequence_length(lead_byte); },
                    ThrowsMessage<Utf8DecodeError>(StrEq("invalid lead byte")));
    };
    test_case(0x80);
    test_case(0xBF);
    test_case(0xFF);
}

TEST(Utf8Test, DecodeSequence) {
    auto test_case = [](std::string_view sequence, Char expected) {
        EXPECT_EQ(expected, decode_utf8_sequence(sequence));
    };
    test_case("\n", 0x0A);
    test_case("T", 0x54);
    test_case("¢", 0x00A2);
    test_case("π", 0x03C0);
    test_case("★", 0x2605);
    test_case("水", 0x6C34);
    test_case("𝐀", 0x1D400);
    test_case("🌍", 0x1F30D);
}

TEST(Utf8Test, EmptyInput) {
    EXPECT_THAT([&] { decode_utf8_sequence(""); },
                ThrowsMessage<Utf8DecodeError>(StrEq("empty input")));
}

TEST(Utf8Test, IncompleteSequence) {
    auto test_case = [](std::string_view sequence) {
        EXPECT_THAT(
            [&] { decode_utf8_sequence(sequence); },
            ThrowsMessage<Utf8DecodeError>(StrEq("incomplete UTF-8 sequence")));
    };
    test_case("\xCF");
    test_case("\xE6\xB0");
    test_case("\xF0\x9F\x8C");
}

TEST(Utf8Test, InvalidContinuationByte) {
    auto test_case = [](std::string_view sequence) {
        EXPECT_THAT(
            [&] { decode_utf8_sequence(sequence); },
            ThrowsMessage<Utf8DecodeError>(StrEq("invalid continuation byte")));
    };
    test_case("\xCF\x46");
    test_case("\xE6\xB0\xC2");
    test_case("\xF0\x9F\x8C\xFF");
}

TEST(Utf8Test, RejectOverlong) {
    auto test_case = [](std::string_view sequence) {
        EXPECT_THAT([&] { decode_utf8_sequence(sequence); },
                    ThrowsMessage<Utf8DecodeError>(StrEq("overlong encoding")));
    };
    test_case("\xC1\x93");
    test_case("\xF0\x80\x83\xA9");
}

TEST(Utf8Test, RejectSurrogate) {
    auto test_case = [](std::string_view sequence) {
        EXPECT_THAT(
            [&] { decode_utf8_sequence(sequence); },
            ThrowsMessage<Utf8DecodeError>(StrEq("surrogate code point")));
    };
    test_case("\xED\xA0\x80");
    test_case("\xED\xBF\xBF");
}

TEST(Utf8Test, RejectOutOfRange) {
    auto test_case = [](std::string_view sequence) {
        EXPECT_THAT(
            [&] { decode_utf8_sequence(sequence); },
            ThrowsMessage<Utf8DecodeError>(StrEq("out-of-range code point")));
    };
    test_case("\xF4\x90\x80\x80");  // U+110000 (first out-of-range code point)
    test_case("\xF4\xBF\xBF\xBF");  // U+13FFFF
}

}  // namespace lovebird
