#ifndef LOVEBIRD_LEXER_CHARS_HPP
#define LOVEBIRD_LEXER_CHARS_HPP

#include "lovebird/runtime/builtin.hpp"

namespace lovebird {

inline bool is_whitespace(Char ch) {
    return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
}

inline bool is_uppercase_letter(Char ch) {
    return ch >= 'A' && ch <= 'Z';
}

inline bool is_lowercase_letter(Char ch) {
    return ch >= 'a' && ch <= 'z';
}

inline bool is_letter(Char ch) {
    return is_uppercase_letter(ch) || is_lowercase_letter(ch);
}

inline bool is_binary_digit(Char ch) {
    return ch == '0' || ch == '1';
}

inline bool is_octal_digit(Char ch) {
    return ch >= '0' && ch <= '7';
}

inline bool is_decimal_digit(Char ch) {
    return ch >= '0' && ch <= '9';
}

inline bool is_hexadecimal_digit(Char ch) {
    return is_decimal_digit(ch) || (ch >= 'A' && ch <= 'F') ||
           (ch >= 'a' && ch <= 'f');
}

inline bool is_name_start(Char ch) {
    return is_letter(ch) || ch == '_';
}

inline bool is_name_continue(Char ch) {
    return is_letter(ch) || is_decimal_digit(ch) || ch == '_';
}

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_LEXER_CHARS_HPP
