#ifndef LOVEBIRD_PARSER_TOKEN_STREAM_HPP
#define LOVEBIRD_PARSER_TOKEN_STREAM_HPP

#include <cstddef>
#include <span>

#include "lovebird/syntax/token.hpp"

namespace lovebird {

class TokenStream {
public:
    TokenStream(std::span<const Token> tokens)
        : tokens_{tokens},
          index_{0} {
    }

    bool done() const {
        return index_ >= tokens_.size();
    }

    // Gets the current token without advancing.
    const Token* current() const {
        if (done()) {
            return nullptr;
        }
        return &tokens_[index_];
    }

    // Gets the current token kind without advancing.
    TokenKind kind() const {
        auto* tok = current();
        return tok ? tok->kind : TokenKind::end;
    }

    // Gets the current token and advances.
    const Token* next() {
        auto* tok = current();
        if (!done()) {
            ++index_;
        }
        return tok;
    }

    // Peeks ahead by n tokens.
    const Token* peek(std::size_t n) {
        if (index_ + n >= tokens_.size()) {
            return nullptr;
        }
        return &tokens_[index_ + n];
    }

    // Peeks ahead by n tokens and returns the kind.
    TokenKind peek_kind(std::size_t n) {
        auto* tok = peek(n);
        return tok ? tok->kind : TokenKind::end;
    }

    // Returns the token and advances the stream if the current token matches.
    const Token* match(TokenKind target) {
        if (kind() == target) {
            const Token* tok = current();
            next();
            return tok;
        }
        return nullptr;
    }

private:
    std::span<const Token> tokens_;
    std::size_t index_;
};

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_PARSER_TOKEN_STREAM_HPP
