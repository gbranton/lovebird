#include "lovebird/parser/test/parser_test.hpp"

#include <string_view>

#include <gtest/gtest.h>

#include "lovebird/diagnostic/diagnostic.hpp"
#include "lovebird/syntax/expr.hpp"

namespace lovebird {

namespace {

TEST_F(ParserTest, ParseName) {
    std::string_view source = "cat";
    auto* expr = name("cat");
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
    auto* expr = negate_op(name("negated"));
    parse_expr_and_expect_tree(source, expr);
    expect_no_diagnostics();
}

TEST_F(ParserTest, ParseMultipleUnaryOperators) {
    std::string_view source = "---negated";
    auto* expr = negate_op(negate_op(negate_op(name("negated"))));
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

TEST_F(ParserTest, ParseParenthesizedExpression) {
    auto test_case = [&](std::string_view source, Expr* expected) {
        parse_expr_and_expect_tree(source, expected);
        expect_no_diagnostics();
    };
    test_case("(1)", int_lit("1"));
    test_case("(((1)))", int_lit("1"));
    test_case("-(1)", negate_op(int_lit("1")));
    test_case("(1 + 2) * 3",
              multiply_op(add_op(int_lit("1"), int_lit("2")), int_lit("3")));
    test_case("1 + (2 * 3)",
              add_op(int_lit("1"), multiply_op(int_lit("2"), int_lit("3"))));
}

TEST_F(ParserTest, ParseBlock) {
    auto test_case = [&](std::string_view source, Expr* expected) {
        parse_expr_and_expect_tree(source, expected);
        expect_no_diagnostics();
    };
    test_case("()", block({}));
    test_case("(10;)", block({expr_stmt(int_lit("10"))}));
    test_case("(1; 2; 3 + 4)",
              block({
                  expr_stmt(int_lit("1")),
                  expr_stmt(int_lit("2")),
                  expr_stmt(add_op(int_lit("3"), int_lit("4"))),
              }));
}

TEST_F(ParserTest, EmitUnclosedBlock) {
    std::string_view source = "(1 + 2";
    Expr* expr = add_op(int_lit("1"), int_lit("2"));
    parse_expr_and_expect_tree(source, expr);
    expect_diagnostics({Diagnostic{.message = "expected )"}});
}

}  // namespace

}  // namespace lovebird
