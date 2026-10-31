#ifndef LOVEBIRD_PARSER_TEST_PARSER_TEST_HPP
#define LOVEBIRD_PARSER_TEST_PARSER_TEST_HPP

#include <span>
#include <sstream>
#include <string_view>
#include <utility>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "lovebird/diagnostic/diagnostic.hpp"
#include "lovebird/diagnostic/diagnostic_emitter.hpp"
#include "lovebird/parser/lexer.hpp"
#include "lovebird/parser/parser.hpp"
#include "lovebird/parser/token_stream.hpp"
#include "lovebird/syntax/expr.hpp"
#include "lovebird/syntax/item.hpp"
#include "lovebird/syntax/syntax_tree.hpp"
#include "lovebird/syntax/test/equal.hpp"
#include "lovebird/syntax/token.hpp"

namespace lovebird {

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
        expect_equal(*actual, *expected);
    }

    void parse_type_expr_and_expect_tree(std::string_view source,
                                         TypeExpr* expected) {
        auto parser = make_parser(source);
        auto* actual = parser.parse_type_expr();
        ASSERT_NE(actual, nullptr);
        expect_equal(*actual, *expected);
    }

    void parse_item_and_expect_null(std::string_view source) {
        auto parser = make_parser(source);
        auto* actual = parser.parse_item();
        EXPECT_EQ(actual, nullptr);
    }

    void parse_item_and_expect_tree(std::string_view source, Item* expected) {
        auto parser = make_parser(source);
        auto* actual = parser.parse_item();
        ASSERT_NE(actual, nullptr);
        expect_equal(*actual, *expected);
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
        return tree_.arena.make<Expr>(std::forward<T>(expr));
    }

    template <class T>
    TypeExpr* make_type_expr(T&& type_expr) {
        return tree_.arena.make<TypeExpr>(std::forward<T>(type_expr));
    }

    template <class T>
    Item* make_item(T&& item) {
        return tree_.arena.make<Item>(std::forward<T>(item));
    }

    template <class T>
    Stmt* make_stmt(T&& stmt) {
        return tree_.arena.make<Stmt>(std::forward<T>(stmt));
    }

    Expr* name(std::string_view value) {
        return make_expr(Name{.value = value});
    }

    Expr* int_lit(std::string_view value) {
        return make_expr(Literal{
            .kind = LiteralKind::literal_int,
            .value = value,
        });
    }

    Expr* char_lit(std::string_view value) {
        return make_expr(Literal{
            .kind = LiteralKind::literal_char,
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

    Expr* block(std::span<Stmt* const> stmts) {
        return make_expr(Block{
            .statements = tree_.arena.copy<Stmt*>(stmts),
        });
    }

    TypeExpr* type_name(std::string_view value) {
        return make_type_expr(Name{.value = value});
    }

    Field field(std::string_view name, TypeExpr* type) {
        return {
            .name = name,
            .type = type,
            .pub = false,
        };
    }

    Field pub_field(std::string_view name, TypeExpr* type) {
        return {
            .name = name,
            .type = type,
            .pub = true,
        };
    }

    Item* unit_struct(std::string_view name) {
        return make_item(Struct{
            .name = name,
            .kind = StructKind::unit,
        });
    }

    Item* brace_struct(std::string_view name, std::span<const Field> fields) {
        return make_item(Struct{
            .name = name,
            .kind = StructKind::brace,
            .fields = tree_.arena.copy<Field>(fields),
        });
    }

    Stmt* expr_stmt(Expr* expr) {
        return make_stmt(ExprStmt{.expr = expr});
    }
};

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_PARSER_TEST_PARSER_TEST_HPP
