#include "lovebird/lexer/lexer.hpp"

#include <memory>
#include <span>
#include <sstream>
#include <string_view>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "lovebird/diagnostic/diagnostic.hpp"
#include "lovebird/diagnostic/diagnostic_emitter.hpp"
#include "lovebird/syntax/token.hpp"

namespace lovebird {

namespace {

using ::testing::IsEmpty;
using ::testing::Pointwise;
using ::testing::Test;

MATCHER(TokenKindAndViewEqual, "") {
    const auto& [actual, expected] = arg;
    return actual.kind == expected.kind && actual.view == expected.view;
}

MATCHER(DiagnosticMessageEqual, "") {
    const auto& [actual, expected] = arg;
    return actual.message == expected.message;
}

class LexerTest : public Test {
protected:
    std::ostringstream oss_;
    std::shared_ptr<DiagnosticEmitter> diagnostic_emitter_;

    LexerTest()
        : diagnostic_emitter_{std::make_shared<DiagnosticEmitter>(oss_)} {
    }

    Lexer make_lexer(std::string_view source) const {
        return Lexer{source, diagnostic_emitter_};
    }

    std::vector<Token> lex_all(std::string_view source) {
        auto lexer = make_lexer(source);
        std::vector<Token> tokens;
        while (auto token = lexer.lex()) {
            tokens.push_back(*token);
        }
        return tokens;
    }

    void lex_and_expect_tokens(std::string_view source,
                               std::span<const Token> expected) {
        auto tokens = lex_all(source);
        EXPECT_THAT(tokens, Pointwise(TokenKindAndViewEqual(), expected));
    }

    void lex_and_expect_no_tokens(std::string_view source) {
        auto tokens = lex_all(source);
        EXPECT_THAT(tokens, IsEmpty());
    }

    void expect_diagnostics(std::span<const Diagnostic> expected) const {
        EXPECT_THAT(diagnostic_emitter_->diagnostics(),
                    Pointwise(DiagnosticMessageEqual(), expected));
    }

    void expect_no_diagnostics() const {
        EXPECT_THAT(diagnostic_emitter_->diagnostics(), IsEmpty());
    }
};

TEST_F(LexerTest, SkipTrivia) {
    std::string_view source =
        "// Hello there!"
        " \t\n\r";
    lex_and_expect_no_tokens(source);
    expect_no_diagnostics();
}

TEST_F(LexerTest, EmitUnexpectedChar) {
    std::string_view source = "  ~ Hello";
    std::vector<Diagnostic> diagnostics = {
        {{.message = "unexpected character: U+007E"}}};
    lex_and_expect_no_tokens(source);
    expect_diagnostics(diagnostics);
}

TEST_F(LexerTest, LexNames) {
    std::string_view source =
        "lovebird snake_case lowerCamelCase UpperCamelCase x num123";
    std::vector<Token> tokens = {
        {
            .kind = TokenKind::name,
            .view = "lovebird",
        },
        {
            .kind = TokenKind::name,
            .view = "snake_case",
        },
        {
            .kind = TokenKind::name,
            .view = "lowerCamelCase",
        },
        {
            .kind = TokenKind::name,
            .view = "UpperCamelCase",
        },
        {
            .kind = TokenKind::name,
            .view = "x",
        },
        {
            .kind = TokenKind::name,
            .view = "num123",
        },
    };
    lex_and_expect_tokens(source, tokens);
    expect_no_diagnostics();
}

}  // namespace

}  // namespace lovebird
