#ifndef LOVEBIRD_SYNTAX_TEST_EQUAL_HPP
#define LOVEBIRD_SYNTAX_TEST_EQUAL_HPP

#include "lovebird/syntax/expr.hpp"

namespace lovebird {

void test_equal(const Expr& actual, const Expr& expected);

void test_equal(const Name& actual, const Name& expected);

void test_equal(const IntLiteral& actual, const IntLiteral& expected);

void test_equal(const CharLiteral& actual, const CharLiteral& expected);

void test_equal(const Operator& actual, const Operator& expected);

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_SYNTAX_TEST_EQUAL_HPP
