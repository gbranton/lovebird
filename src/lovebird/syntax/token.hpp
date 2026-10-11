#ifndef LOVEBIRD_SYNTAX_TOKEN_HPP
#define LOVEBIRD_SYNTAX_TOKEN_HPP

#include <format>
#include <iosfwd>
#include <string_view>

#include "lovebird/source/source_position.hpp"

namespace lovebird {

enum class TokenKind {
    end,  // End of input

    name,

    // Literals

    literal_int,

    // Keywords

    keyword_break,
    keyword_case,
    keyword_const,
    keyword_continue,
    keyword_default,
    keyword_else,
    keyword_embed,
    keyword_enum,
    keyword_fn,
    keyword_for,
    keyword_if,
    keyword_impl,
    keyword_import,
    keyword_in,
    keyword_let,
    keyword_module,
    keyword_mut,
    keyword_package,
    keyword_pub,
    keyword_return,
    keyword_self,
    keyword_struct,
    keyword_switch,
    keyword_trait,
    keyword_type,
    keyword_unsafe,

    // Punctuation

    paren_left,     // (
    paren_right,    // )
    bracket_left,   // [
    bracket_right,  // ]
    brace_left,     // {
    brace_right,    // }

    period,     // .
    comma,      // ,
    semicolon,  // ;
    colon,      // :
    bang,       // !
    question,   // ?
    at,         // @
    hash,       // #
    arrow,      // ->

    plus,     // +
    minus,    // -
    star,     // *
    slash,    // /
    percent,  // %

    equal,          // =
    plus_equal,     // +=
    minus_equal,    // -=
    star_equal,     // *=
    slash_equal,    // /=
    percent_equal,  // %=

    tilde,            // ~
    ampersand,        // &
    pipe,             // |
    caret,            // ^
    less_less,        // <<
    greater_greater,  // >>

    ampersand_equal,        // &=
    pipe_equal,             // |=
    caret_equal,            // ^=
    less_less_equal,        // <<=
    greater_greater_equal,  // >>=

    ampersand_ampersand,  // &&
    pipe_pipe,            // ||

    equal_equal,    // ==
    bang_equal,     // !=
    less,           // <
    less_equal,     // <=
    greater,        // >
    greater_equal,  // >=
};

std::string_view to_string(TokenKind);

struct Token {
    TokenKind kind;
    std::string_view view;
    SourceSpan span;
};

std::ostream& operator<<(std::ostream&, const Token&);

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
