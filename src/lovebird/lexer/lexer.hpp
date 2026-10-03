#ifndef LOVEBIRD_LEXER_LEXER_HPP
#define LOVEBIRD_LEXER_LEXER_HPP

#include <memory>
#include <optional>
#include <string_view>

#include "lovebird/diagnostic/diagnostic_emitter.hpp"
#include "lovebird/source/source_iterator.hpp"
#include "lovebird/source/source_stream.hpp"
#include "lovebird/syntax/token.hpp"

namespace lovebird {

class Lexer {
public:
    Lexer(std::string_view source,
          std::shared_ptr<DiagnosticEmitter> diagnostic_emitter);

    // Advances the lexer and returns the next token.
    std::optional<Token> lex();

private:
    SourceStream stream_;
    std::shared_ptr<DiagnosticEmitter> diagnostic_emitter_;
    SourceIterator token_start_;

    std::string_view token_view() const;
    Token make_token(TokenKind);

    void skip_trivia();
    bool skip_whitespace();
    bool skip_comment();

    std::optional<Token> lex_name_or_keyword();
    std::optional<Token> lex_punctuation();

    void emit_unexpected_char_diagnostic();
};

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_LEXER_LEXER_HPP
