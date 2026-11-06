#ifndef LOVEBIRD_SYNTAX_TEST_EQUAL_HPP
#define LOVEBIRD_SYNTAX_TEST_EQUAL_HPP

#include "lovebird/syntax/ast.hpp"

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

void expect_equal(const FieldDecl& actual, const FieldDecl& expected);

void expect_equal(const StructDecl& actual, const StructDecl& expected);

void expect_equal(const EnumCaseDecl& actual, const EnumCaseDecl& expected);

void expect_equal(const EnumDecl& actual, const EnumDecl& expected);

void expect_equal(const FunctionDecl& actual, const FunctionDecl& expected);

// ------------------------------------------------------------
// Statements

void expect_equal(const Stmt& actual, const Stmt& expected);

void expect_equal(const ExprStmt& actual, const ExprStmt& expected);

void expect_equal(const ItemStmt& actual, const ItemStmt& expected);

void expect_equal(const LetStmt& actual, const LetStmt& expected);

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_SYNTAX_TEST_EQUAL_HPP
