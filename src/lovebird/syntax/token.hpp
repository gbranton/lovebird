#ifndef LOVEBIRD_SYNTAX_TOKEN_HPP
#define LOVEBIRD_SYNTAX_TOKEN_HPP

#include <format>
#include <string_view>

#include "lovebird/source_position.hpp"

namespace lovebird {

enum class TokenKind {
    name,
};

std::string_view to_string(TokenKind);

struct Token {
    TokenKind kind;
    std::string_view view;
    SourceSpan span;
};

}  // namespace lovebird

template <>
struct std::formatter<lovebird::TokenKind> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(lovebird::TokenKind kind, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "{}", to_string(kind));
    }
};

template <>
struct std::formatter<lovebird::Token> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const lovebird::Token& tok, std::format_context& ctx) const {
        return std::format_to(ctx.out(),
                              "{{kind={}, view=`{}`, span={}}}",
                              tok.kind,
                              tok.view,
                              tok.span);
    }
};

#endif  // ifndef LOVEBIRD_SYNTAX_TOKEN_HPP
