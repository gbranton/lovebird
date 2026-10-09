#include "lovebird/lexer/lexer.hpp"

#include <format>
#include <optional>
#include <string_view>
#include <vector>

#include "lovebird/diagnostic/diagnostic.hpp"
#include "lovebird/diagnostic/diagnostic_emitter.hpp"
#include "lovebird/lexer/chars.hpp"
#include "lovebird/lexer/keyword_table.hpp"
#include "lovebird/syntax/token.hpp"

namespace lovebird {

Lexer::Lexer(std::string_view source, DiagnosticEmitter& diagnostic_emitter)
    : stream_{source},
      diagnostic_emitter_{diagnostic_emitter} {
}

std::optional<Token> Lexer::lex() {
    skip_trivia();

    if (stream_.done()) {
        return std::nullopt;
    }

    token_start_ = stream_.iterator();

    if (auto tok = lex_name_or_keyword()) {
        return tok;
    }
    if (auto tok = lex_punctuation()) {
        return tok;
    }

    emit_unexpected_char_diagnostic();
    return std::nullopt;
}

std::string_view Lexer::token_view() const {
    return {token_start_.ptr(), stream_.iterator().ptr()};
}

Token Lexer::make_token(TokenKind kind) {
    return {
        .kind = kind,
        .view = token_view(),
        .span =
            {
                .begin = token_start_.pos(),
                .end = stream_.iterator().pos(),
            },
    };
}

void Lexer::skip_trivia() {
    while (skip_whitespace() || skip_comment()) {
    }
}

bool Lexer::skip_whitespace() {
    bool skipped = false;
    while (stream_.match(is_whitespace)) {
        skipped = true;
    }
    return skipped;
}

bool Lexer::skip_comment() {
    if (!stream_.match('/', '/')) {
        return false;
    }
    while (stream_.match([](Char ch) { return ch != '\n'; })) {
    }
    return true;
}

std::optional<Token> Lexer::lex_name_or_keyword() {
    if (!stream_.match(is_name_start)) {
        return std::nullopt;
    }
    while (stream_.match(is_name_continue)) {
    }

    if (auto keyword = look_up_keyword(token_view())) {
        return make_token(*keyword);
    }

    return make_token(TokenKind::name);
}

std::optional<Token> Lexer::lex_punctuation() {
    TokenKind kind;
    int length = 1;

    switch (*stream_.current()) {
        case '(':
            kind = TokenKind::paren_left;
            break;

        case ')':
            kind = TokenKind::paren_right;
            break;

        case '[':
            kind = TokenKind::bracket_left;
            break;

        case ']':
            kind = TokenKind::bracket_right;
            break;

        case '{':
            kind = TokenKind::brace_left;
            break;

        case '}':
            kind = TokenKind::brace_right;
            break;

        case '.':
            kind = TokenKind::period;
            break;

        case ',':
            kind = TokenKind::comma;
            break;

        case ';':
            kind = TokenKind::semicolon;
            break;

        case ':':
            kind = TokenKind::colon;
            break;

        case '!':
            if (stream_.peek(1) == '=') {
                kind = TokenKind::bang_equal;
                length = 2;
            } else {
                kind = TokenKind::bang;
            }
            break;

        case '?':
            kind = TokenKind::question;
            break;

        case '@':
            kind = TokenKind::at;
            break;

        case '#':
            kind = TokenKind::hash;
            break;

        case '+':
            if (stream_.peek(1) == '=') {
                kind = TokenKind::plus_equal;
                length = 2;
            } else {
                kind = TokenKind::plus;
            }
            break;

        case '-':
            if (stream_.peek(1) == '>') {
                kind = TokenKind::arrow;
                length = 2;
            } else if (stream_.peek(1) == '=') {
                kind = TokenKind::minus_equal;
                length = 2;
            } else {
                kind = TokenKind::minus;
            }
            break;

        case '*':
            if (stream_.peek(1) == '=') {
                kind = TokenKind::star_equal;
                length = 2;
            } else {
                kind = TokenKind::star;
            }
            break;

        case '/':
            if (stream_.peek(1) == '=') {
                kind = TokenKind::slash_equal;
                length = 2;
            } else {
                kind = TokenKind::slash;
            }
            break;

        case '%':
            if (stream_.peek(1) == '=') {
                kind = TokenKind::percent_equal;
                length = 2;
            } else {
                kind = TokenKind::percent;
            }
            break;

        case '=':
            if (stream_.peek(1) == '=') {
                kind = TokenKind::equal_equal;
                length = 2;
            } else {
                kind = TokenKind::equal;
            }
            break;

        case '~':
            kind = TokenKind::tilde;
            break;

        case '&':
            if (stream_.peek(1) == '=') {
                kind = TokenKind::ampersand_equal;
                length = 2;
            } else if (stream_.peek(1) == '&') {
                kind = TokenKind::ampersand_ampersand;
                length = 2;
            } else {
                kind = TokenKind::ampersand;
            }
            break;

        case '|':
            if (stream_.peek(1) == '=') {
                kind = TokenKind::pipe_equal;
                length = 2;
            } else if (stream_.peek(1) == '|') {
                kind = TokenKind::pipe_pipe;
                length = 2;
            } else {
                kind = TokenKind::pipe;
            }
            break;

        case '^':
            if (stream_.peek(1) == '=') {
                kind = TokenKind::caret_equal;
                length = 2;
            } else {
                kind = TokenKind::caret;
            }
            break;

        case '<':
            if (stream_.peek(1) == '<') {
                if (stream_.peek(2) == '=') {
                    kind = TokenKind::less_less_equal;
                    length = 3;
                } else {
                    kind = TokenKind::less_less;
                    length = 2;
                }
            } else if (stream_.peek(1) == '=') {
                kind = TokenKind::less_equal;
                length = 2;
            } else {
                kind = TokenKind::less;
            }
            break;

        case '>':
            if (stream_.peek(1) == '>') {
                if (stream_.peek(2) == '=') {
                    kind = TokenKind::greater_greater_equal;
                    length = 3;
                } else {
                    kind = TokenKind::greater_greater;
                    length = 2;
                }
            } else if (stream_.peek(1) == '=') {
                kind = TokenKind::greater_equal;
                length = 2;
            } else {
                kind = TokenKind::greater;
            }
            break;

        default:
            return std::nullopt;
    }

    for (int i = 0; i < length; ++i) {
        stream_.next();
    }

    return make_token(kind);
}

void Lexer::emit_unexpected_char_diagnostic() {
    auto message = std::format("unexpected character: U+{:04X}",
                               stream_.current().value());
    diagnostic_emitter_.emit(Diagnostic{.message = message});
}

std::vector<Token> lex_all(std::string_view source,
                           DiagnosticEmitter& diagnostic_emitter) {
    Lexer lexer{source, diagnostic_emitter};
    std::vector<Token> tokens;
    while (auto token = lexer.lex()) {
        tokens.push_back(*token);
    }
    return tokens;
}

}  // namespace lovebird
