#include "lovebird/lexer/lexer.hpp"

#include <format>
#include <memory>
#include <optional>
#include <string_view>

#include "lovebird/diagnostic/diagnostic.hpp"
#include "lovebird/diagnostic/diagnostic_emitter.hpp"
#include "lovebird/lexer/chars.hpp"
#include "lovebird/syntax/token.hpp"

namespace lovebird {

Lexer::Lexer(std::string_view source,
             std::shared_ptr<DiagnosticEmitter> diagnostic_emitter)
    : stream_{source},
      diagnostic_emitter_{diagnostic_emitter} {
}

std::optional<Token> Lexer::lex() {
    skip_trivia();

    if (stream_.done()) {
        return std::nullopt;
    }

    token_start_ = stream_.iterator();

    if (auto tok = lex_name()) {
        return tok;
    }

    emit_unexpected_char_diagnostic();
    return std::nullopt;
}

Token Lexer::make_token(TokenKind kind) {
    return {
        .kind = kind,
        .view = {token_start_.ptr(), stream_.iterator().ptr()},
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

std::optional<Token> Lexer::lex_name() {
    if (!stream_.match(is_name_start)) {
        return std::nullopt;
    }
    while (stream_.match(is_name_continue)) {
    }
    return make_token(TokenKind::name);
}

void Lexer::emit_unexpected_char_diagnostic() {
    auto message = std::format("unexpected character: U+{:04X}",
                               stream_.current().value());
    diagnostic_emitter_->emit(Diagnostic{.message = message});
}

}  // namespace lovebird
