#ifndef LOVEBIRD_LEXER_KEYWORD_TABLE_HPP
#define LOVEBIRD_LEXER_KEYWORD_TABLE_HPP

#include <algorithm>
#include <array>
#include <optional>
#include <string_view>
#include <utility>

#include "lovebird/syntax/token.hpp"

namespace lovebird {

constexpr auto keyword_table =
    std::to_array<std::pair<std::string_view, TokenKind>>({
        {"break", TokenKind::keyword_break},
        {"case", TokenKind::keyword_case},
        {"const", TokenKind::keyword_const},
        {"continue", TokenKind::keyword_continue},
        {"default", TokenKind::keyword_default},
        {"else", TokenKind::keyword_else},
        {"embed", TokenKind::keyword_embed},
        {"enum", TokenKind::keyword_enum},
        {"fn", TokenKind::keyword_fn},
        {"for", TokenKind::keyword_for},
        {"if", TokenKind::keyword_if},
        {"impl", TokenKind::keyword_impl},
        {"import", TokenKind::keyword_import},
        {"in", TokenKind::keyword_in},
        {"let", TokenKind::keyword_let},
        {"module", TokenKind::keyword_module},
        {"mut", TokenKind::keyword_mut},
        {"package", TokenKind::keyword_package},
        {"pub", TokenKind::keyword_pub},
        {"return", TokenKind::keyword_return},
        {"self", TokenKind::keyword_self},
        {"struct", TokenKind::keyword_struct},
        {"switch", TokenKind::keyword_switch},
        {"trait", TokenKind::keyword_trait},
        {"type", TokenKind::keyword_type},
        {"unsafe", TokenKind::keyword_unsafe},
    });

constexpr std::optional<TokenKind> look_up_keyword(std::string_view sv) {
    auto it = std::ranges::find_if(
        keyword_table, [sv](const auto& key) { return sv == key.first; });
    if (it == keyword_table.end()) {
        return std::nullopt;
    }
    return it->second;
}

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_LEXER_KEYWORD_TABLE_HPP
