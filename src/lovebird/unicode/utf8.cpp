#include "lovebird/unicode/utf8.hpp"

#include <cstddef>
#include <string_view>

namespace lovebird {

namespace {

bool is_overlong_encoding(Char ch, std::size_t length) {
    switch (length) {
        case 2:
            if (ch < 0x80) {
                return true;
            }
            break;
        case 3:
            if (ch < 0x800) {
                return true;
            }
            break;
        case 4:
            if (ch < 0x10000) {
                return true;
            }
            break;
    }
    return false;
}

void reject_overlong(Char ch, std::size_t length) {
    if (is_overlong_encoding(ch, length)) {
        throw Utf8DecodeError{"overlong encoding"};
    }
}

void reject_surrogate(Char ch) {
    if (ch >= 0xD800 && ch <= 0xDFFF) {
        throw Utf8DecodeError{"surrogate code point"};
    }
}

void reject_out_of_range(Char ch) {
    if (ch >= 0x110000) {
        throw Utf8DecodeError{"out-of-range code point"};
    }
}

}  // namespace

std::size_t utf8_sequence_length(char lead_byte) {
    // The lead byte of a UTF-8 sequence tells us how many bytes it has:
    // 0xxxxxxx - 1 byte
    // 110xxxxx - 2 bytes
    // 1110xxxx - 3 bytes
    // 11110xxx - 4 bytes
    if ((lead_byte & 0x80) == 0x00) {
        return 1;
    } else if ((lead_byte & 0xE0) == 0xC0) {
        return 2;
    } else if ((lead_byte & 0xF0) == 0xE0) {
        return 3;
    } else if ((lead_byte & 0xF8) == 0xF0) {
        return 4;
    } else {
        throw Utf8DecodeError{"invalid lead byte"};
    }
}

Char decode_utf8_sequence(std::string_view str) {
    if (str.empty()) {
        throw Utf8DecodeError{"empty input"};
    }

    char lead_byte = str[0];

    std::size_t length = utf8_sequence_length(lead_byte);
    if (str.length() < length) {
        throw Utf8DecodeError{"incomplete UTF-8 sequence"};
    }

    Char ch;

    // Decode the lead byte.
    switch (length) {
        case 1:
            // ASCII can just be returned.
            return lead_byte;
        case 2:
            ch = lead_byte & 0x1F;
            break;
        case 3:
            ch = lead_byte & 0x0F;
            break;
        case 4:
            ch = lead_byte & 0x07;
            break;
    }

    // Shift-accumulate the continuation bytes.
    for (std::size_t i = 1; i < length; ++i) {
        char cont_byte = str[i];
        if ((cont_byte & 0xC0) != 0x80) {
            throw Utf8DecodeError{"invalid continuation byte"};
        }
        ch <<= 6;
        ch |= cont_byte & 0x3F;
    }

    // Check for invalid code points.
    reject_overlong(ch, length);
    reject_surrogate(ch);
    reject_out_of_range(ch);

    return ch;
}

}  // namespace lovebird
