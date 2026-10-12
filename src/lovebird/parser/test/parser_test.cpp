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

    Expr* int_lit(std::string_view value) {
        return make_expr(Literal{
            .kind = LiteralKind::literal_int,
            .value = value,
        });
    }

    Expr* unary_op(OperatorKind kind, Expr* right) {
        return make_expr(Operator{
            .kind = kind,
            .right = right,
        });
    }

    Expr* binary_op(OperatorKind kind, Expr* left, Expr* right) {
        return make_expr(Operator{
            .kind = kind,
            .left = left,
            .right = right,
        });
    }

    Expr* negate_op(Expr* right) {
        return unary_op(OperatorKind::negate, right);
    }

    Expr* add_op(Expr* left, Expr* right) {
        return binary_op(OperatorKind::add, left, right);
    }

    Expr* subtract_op(Expr* left, Expr* right) {
        return binary_op(OperatorKind::subtract, left, right);
    }

    Expr* multiply_op(Expr* left, Expr* right) {
        return binary_op(OperatorKind::multiply, left, right);
    }
};

TEST_F(ParserTest, ParseName) {
    std::string_view source = "cat";
    auto* expr = make_expr(Name{.value = "cat"});
    parse_expr_and_expect_tree(source, expr);
    expect_no_diagnostics();
}

TEST_F(ParserTest, ParseLiterals) {
    auto test_case =
        [&](std::string_view source, LiteralKind kind, std::string_view value) {
            auto* expr = make_expr(Literal{
                .kind = kind,
                .value = value,
            });
            parse_expr_and_expect_tree(source, expr);
            expect_no_diagnostics();
        };
    test_case("64", LiteralKind::literal_int, "64");
    test_case("'*'", LiteralKind::literal_char, "'*'");
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

TEST_F(ParserTest, ParseBinaryOperator) {
    auto test_case = [&](std::string_view source, Expr* expected) {
        parse_expr_and_expect_tree(source, expected);
        expect_no_diagnostics();
    };
    test_case("2 + 2", add_op(int_lit("2"), int_lit("2")));
    test_case("1 + 2 + 3",
              add_op(add_op(int_lit("1"), int_lit("2")), int_lit("3")));
    test_case("1 * 2 + 3",
              add_op(multiply_op(int_lit("1"), int_lit("2")), int_lit("3")));
    test_case("1 + 2 * 3",
              add_op(int_lit("1"), multiply_op(int_lit("2"), int_lit("3"))));
}

TEST_F(ParserTest, ParseUnaryAndBinaryOperator) {
    std::string_view source = "-1 - -2 * -3";
    Expr* expr = subtract_op(
        negate_op(int_lit("1")),
        multiply_op(negate_op(int_lit("2")), negate_op(int_lit("3"))));
    parse_expr_and_expect_tree(source, expr);
    expect_no_diagnostics();
}

TEST_F(ParserTest, EmitMissingOperandForBinaryOperator) {
    std::string_view source = "10 * ";
    Expr* expr = int_lit("10");
    parse_expr_and_expect_tree(source, expr);
    expect_diagnostics({
        Diagnostic{.message = "missing operand for binary operator"},
    });
}

}  // namespace

}  // namespace lovebird
