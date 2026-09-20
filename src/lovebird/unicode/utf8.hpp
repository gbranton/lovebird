#ifndef LOVEBIRD_UNICODE_UTF8_HPP
#define LOVEBIRD_UNICODE_UTF8_HPP

#include <cstddef>
#include <stdexcept>
#include <string_view>

#include "lovebird/runtime/builtin.hpp"

namespace lovebird {

struct Utf8DecodeError : std::runtime_error {
    using runtime_error::runtime_error;
};

// Finds the length of a UTF-8 sequence in bytes, given the lead byte.
// Throws Utf8DecodeError if the lead byte is invalid.
std::size_t utf8_sequence_length(char lead_byte);

// Decodes the first UTF-8 sequence in the input as a Unicode scalar value.
// Throws Utf8DecodeError if the sequence is invalid or the input is empty.
Char decode_utf8_sequence(std::string_view str);

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_UNICODE_UTF8_HPP
