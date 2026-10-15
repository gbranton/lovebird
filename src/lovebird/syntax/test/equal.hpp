#ifndef LOVEBIRD_SYNTAX_TEST_EQUAL_HPP
#define LOVEBIRD_SYNTAX_TEST_EQUAL_HPP

#include "lovebird/syntax/decl.hpp"
#include "lovebird/syntax/expr.hpp"
#include "lovebird/syntax/stmt.hpp"

namespace lovebird {

// ------------------------------------------------------------
// Declarations

void test_equal(const Decl& actual, const Decl& expected);

void test_equal(const LetDecl& actual, const LetDecl& expected);

// ------------------------------------------------------------
// Expressions

void test_equal(const Expr& actual, const Expr& expected);

void test_equal(const Name& actual, const Name& expected);

void test_equal(const Literal& actual, const Literal& expected);

void test_equal(const Operator& actual, const Operator& expected);

void test_equal(const Block& actual, const Block& expected);

// ------------------------------------------------------------
// Statements

void test_equal(const Stmt& actual, const Stmt& expected);

void test_equal(const DeclStmt& actual, const DeclStmt& expected);

void test_equal(const ExprStmt& actual, const ExprStmt& expected);

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_SYNTAX_TEST_EQUAL_HPP
