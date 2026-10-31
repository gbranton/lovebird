#ifndef LOVEBIRD_SYNTAX_TEST_EQUAL_HPP
#define LOVEBIRD_SYNTAX_TEST_EQUAL_HPP

#include "lovebird/syntax/expr.hpp"
#include "lovebird/syntax/item.hpp"
#include "lovebird/syntax/stmt.hpp"

namespace lovebird {

// ------------------------------------------------------------
// Expressions

void expect_equal(const Expr& actual, const Expr& expected);

void expect_equal(const Name& actual, const Name& expected);

void expect_equal(const Literal& actual, const Literal& expected);

void expect_equal(const Operator& actual, const Operator& expected);

void expect_equal(const Block& actual, const Block& expected);

// ------------------------------------------------------------
// Type Expressions

void expect_equal(const TypeExpr& actual, const TypeExpr& expected);

// ------------------------------------------------------------
// Items

void expect_equal(const Item& actual, const Item& expected);

void expect_equal(const Field& actual, const Field& expected);

void expect_equal(const Struct& actual, const Struct& expected);

void expect_equal(const EnumCase& actual, const EnumCase& expected);

void expect_equal(const Enum& actual, const Enum& expected);

void expect_equal(const Function& actual, const Function& expected);

// ------------------------------------------------------------
// Statements

void expect_equal(const Stmt& actual, const Stmt& expected);

void expect_equal(const ExprStmt& actual, const ExprStmt& expected);

void expect_equal(const ItemStmt& actual, const ItemStmt& expected);

void expect_equal(const LetStmt& actual, const LetStmt& expected);

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_SYNTAX_TEST_EQUAL_HPP
