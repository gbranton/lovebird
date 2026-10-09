#include "lovebird/lexer/lexer.hpp"

#include <span>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "lovebird/diagnostic/diagnostic.hpp"
#include "lovebird/diagnostic/diagnostic_emitter.hpp"
#include "lovebird/lexer/keyword_table.hpp"
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
    DiagnosticEmitter diagnostic_emitter_;

    LexerTest()
        : diagnostic_emitter_{oss_} {
    }

    void lex_and_expect_tokens(std::string_view source,
                               std::span<const Token> expected) {
        auto tokens = lex_all(source, diagnostic_emitter_);
        EXPECT_THAT(tokens, Pointwise(TokenKindAndViewEqual(), expected));
    }

    void lex_and_expect_no_tokens(std::string_view source) {
        auto tokens = lex_all(source, diagnostic_emitter_);
        EXPECT_THAT(tokens, IsEmpty());
    }

    void expect_diagnostics(std::span<const Diagnostic> expected) const {
        EXPECT_THAT(diagnostic_emitter_.diagnostics(),
                    Pointwise(DiagnosticMessageEqual(), expected));
    }

    void expect_no_diagnostics() const {
        EXPECT_THAT(diagnostic_emitter_.diagnostics(), IsEmpty());
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
    std::string_view source = "  $ Hello";
    std::vector<Diagnostic> diagnostics = {
        {{.message = "unexpected character: U+0024"}}};
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

TEST_F(LexerTest, LexKeywords) {
    std::string source;
    std::vector<Token> tokens;

    for (const auto& [key, val] : keyword_table) {
        source += key;
        source += " ";
        tokens.push_back({
            .kind = val,
            .view = key,
        });
    }

    EXPECT_EQ(tokens.size(), 26);
    lex_and_expect_tokens(source, tokens);
    expect_no_diagnostics();
}

TEST_F(LexerTest, LexPunctuation) {
    std::string_view source =
        "() [] {} "
        ". , ; : ! ? @ # -> "
        "+ - * / % "
        "= += -= *= /= %= "
        "~ & | ^ << >> "
        "&= |= ^= <<= >>= "
        "&& || "
        "== != < <= > >= ";
    std::vector<Token> tokens = {
        {.kind = TokenKind::paren_left, .view = "("},
        {.kind = TokenKind::paren_right, .view = ")"},
        {.kind = TokenKind::bracket_left, .view = "["},
        {.kind = TokenKind::bracket_right, .view = "]"},
        {.kind = TokenKind::brace_left, .view = "{"},
        {.kind = TokenKind::brace_right, .view = "}"},
        {.kind = TokenKind::period, .view = "."},
        {.kind = TokenKind::comma, .view = ","},
        {.kind = TokenKind::semicolon, .view = ";"},
        {.kind = TokenKind::colon, .view = ":"},
        {.kind = TokenKind::bang, .view = "!"},
        {.kind = TokenKind::question, .view = "?"},
        {.kind = TokenKind::at, .view = "@"},
        {.kind = TokenKind::hash, .view = "#"},
        {.kind = TokenKind::arrow, .view = "->"},
        {.kind = TokenKind::plus, .view = "+"},
        {.kind = TokenKind::minus, .view = "-"},
        {.kind = TokenKind::star, .view = "*"},
        {.kind = TokenKind::slash, .view = "/"},
        {.kind = TokenKind::percent, .view = "%"},
        {.kind = TokenKind::equal, .view = "="},
        {.kind = TokenKind::plus_equal, .view = "+="},
        {.kind = TokenKind::minus_equal, .view = "-="},
        {.kind = TokenKind::star_equal, .view = "*="},
        {.kind = TokenKind::slash_equal, .view = "/="},
        {.kind = TokenKind::percent_equal, .view = "%="},
        {.kind = TokenKind::tilde, .view = "~"},
        {.kind = TokenKind::ampersand, .view = "&"},
        {.kind = TokenKind::pipe, .view = "|"},
        {.kind = TokenKind::caret, .view = "^"},
        {.kind = TokenKind::less_less, .view = "<<"},
        {.kind = TokenKind::greater_greater, .view = ">>"},
        {.kind = TokenKind::ampersand_equal, .view = "&="},
        {.kind = TokenKind::pipe_equal, .view = "|="},
        {.kind = TokenKind::caret_equal, .view = "^="},
        {.kind = TokenKind::less_less_equal, .view = "<<="},
        {.kind = TokenKind::greater_greater_equal, .view = ">>="},
        {.kind = TokenKind::ampersand_ampersand, .view = "&&"},
        {.kind = TokenKind::pipe_pipe, .view = "||"},
        {.kind = TokenKind::equal_equal, .view = "=="},
        {.kind = TokenKind::bang_equal, .view = "!="},
        {.kind = TokenKind::less, .view = "<"},
        {.kind = TokenKind::less_equal, .view = "<="},
        {.kind = TokenKind::greater, .view = ">"},
        {.kind = TokenKind::greater_equal, .view = ">="},
    };
    lex_and_expect_tokens(source, tokens);
    expect_no_diagnostics();
}

}  // namespace

}  // namespace lovebird
