#include "lovebird/parser/parser.hpp"

#include <span>
#include <sstream>
#include <string_view>
#include <utility>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "lovebird/diagnostic/diagnostic.hpp"
#include "lovebird/diagnostic/diagnostic_emitter.hpp"
#include "lovebird/lexer/lexer.hpp"
#include "lovebird/parser/token_stream.hpp"
#include "lovebird/syntax/expr.hpp"
#include "lovebird/syntax/syntax_tree.hpp"
#include "lovebird/syntax/test/equal.hpp"
#include "lovebird/syntax/token.hpp"

namespace lovebird {

namespace {

using ::testing::IsEmpty;
using ::testing::Pointwise;
using ::testing::Test;

MATCHER(DiagnosticMessageEqual, "") {
    const auto& [actual, expected] = arg;
    return actual.message == expected.message;
}

class ParserTest : public Test {
protected:
    std::ostringstream oss_;
    DiagnosticEmitter diagnostic_emitter_;
    std::vector<Token> tokens_;
    SyntaxTree tree_;

    ParserTest()
        : diagnostic_emitter_{oss_} {
    }

    Parser make_parser(std::string_view source) {
        tokens_ = lex_all(source, diagnostic_emitter_);
        expect_no_diagnostics();
        TokenStream stream{tokens_};
        return Parser{stream, tree_, diagnostic_emitter_};
    }

    void parse_expr_and_expect_null(std::string_view source) {
        auto parser = make_parser(source);
        auto* actual = parser.parse_expr();
        EXPECT_EQ(actual, nullptr);
    }

    void parse_expr_and_expect_tree(std::string_view source, Expr* expected) {
        auto parser = make_parser(source);
        auto* actual = parser.parse_expr();
        ASSERT_NE(actual, nullptr);
        test_equal(*actual, *expected);
    }

    void expect_diagnostics(std::span<const Diagnostic> expected) const {
        EXPECT_THAT(diagnostic_emitter_.diagnostics(),
                    Pointwise(DiagnosticMessageEqual(), expected));
    }

    void expect_no_diagnostics() const {
        EXPECT_THAT(diagnostic_emitter_.diagnostics(), IsEmpty());
    }

    template <class T>
    Expr* make_expr(T&& expr) {
        return tree_.make<Expr>(std::forward<T>(expr));
    }
};

TEST_F(ParserTest, ParsePrimaryExpression) {
    std::string_view source = "cat";
    auto* expr = make_expr(Name{.value = "cat"});
    parse_expr_and_expect_tree(source, expr);
    expect_no_diagnostics();
}

TEST_F(ParserTest, ParseUnaryOperator) {
    std::string_view source = "-negated";
    auto* expr = make_expr(Operator{
        .kind = OperatorKind::negate,
        .right = make_expr(Name{.value = "negated"}),
    });
    parse_expr_and_expect_tree(source, expr);
    expect_no_diagnostics();
}

TEST_F(ParserTest, ParseMultipleUnaryOperators) {
    std::string_view source = "---negated";
    auto* expr = make_expr(Operator{
        .kind = OperatorKind::negate,
        .right = make_expr(Operator{
            .kind = OperatorKind::negate,
            .right = make_expr(Operator{
                .kind = OperatorKind::negate,
                .right = make_expr(Name{.value = "negated"}),
            }),
        }),
    });
    parse_expr_and_expect_tree(source, expr);
    expect_no_diagnostics();
}

TEST_F(ParserTest, EmitMissingOperandForUnaryOperator) {
    std::string_view source = "-";
    parse_expr_and_expect_null(source);
    expect_diagnostics({
        Diagnostic{.message = "missing operand for unary operator"},
    });
}

}  // namespace

}  // namespace lovebird
