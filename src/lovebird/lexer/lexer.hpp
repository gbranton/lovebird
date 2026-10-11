#ifndef LOVEBIRD_LEXER_LEXER_HPP
#define LOVEBIRD_LEXER_LEXER_HPP

#include <optional>
#include <string_view>
#include <vector>

#include "lovebird/diagnostic/diagnostic_emitter.hpp"
#include "lovebird/source/source_iterator.hpp"
#include "lovebird/source/source_stream.hpp"
#include "lovebird/syntax/token.hpp"

namespace lovebird {

class Lexer {
public:
    Lexer(std::string_view source, DiagnosticEmitter& diagnostic_emitter);

    // Advances the lexer and returns the next token.
    std::optional<Token> lex();

private:
    SourceStream stream_;
    DiagnosticEmitter& diagnostic_emitter_;
    SourceIterator token_start_;

    std::string_view token_view() const;
    Token make_token(TokenKind);

    void skip_trivia();
    bool skip_whitespace();
    bool skip_comment();

    std::optional<Token> lex_name_or_keyword();
    std::optional<Token> lex_punctuation();
    std::optional<Token> lex_numeric_literal();

    enum class Base { bin, oct, dec, hex };

    Base match_base_prefix();
    bool match_digits(Base);
    static std::string_view to_string(Base);

    void emit_unexpected_char_diagnostic();
};

std::vector<Token> lex_all(std::string_view source,
                           DiagnosticEmitter& diagnostic_emitter);

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_LEXER_LEXER_HPP
