#include "lovebird/syntax/test/equal.hpp"

#include <cstddef>
#include <type_traits>
#include <variant>

#include <gtest/gtest.h>

#include "lovebird/syntax/decl.hpp"
#include "lovebird/syntax/expr.hpp"
#include "lovebird/syntax/stmt.hpp"

namespace lovebird {

namespace {

template <class T>
void test_not_null_and_equal(const T* actual, const T* expected) {
    ASSERT_NE(actual, nullptr);
    ASSERT_NE(expected, nullptr);
    test_equal(*actual, *expected);
}

template <class T>
void test_node_variants_equal(const T& actual, const T& expected) {
    std::visit(
        [](const auto& actual, const auto& expected) {
            if constexpr (std::is_same_v<decltype(actual),
                                         decltype(expected)>) {
                test_equal(actual, expected);
            } else {
                FAIL() << "node type mismatch: " << typeid(actual).name()
                       << ", " << typeid(expected).name();
            }
        },
        actual,
        expected);
}

}  // namespace

void test_equal(const Decl& actual, const Decl& expected) {
    test_node_variants_equal(actual, expected);
}

void test_equal(const LetDecl& actual, const LetDecl& expected) {
    EXPECT_EQ(actual.name, expected.name);
    test_not_null_and_equal(actual.value, expected.value);
}

void test_equal(const Expr& actual, const Expr& expected) {
    test_node_variants_equal(actual, expected);
}

void test_equal(const Name& actual, const Name& expected) {
    EXPECT_EQ(actual.value, expected.value);
}

void test_equal(const Literal& actual, const Literal& expected) {
    EXPECT_EQ(actual.kind, expected.kind);
    EXPECT_EQ(actual.value, expected.value);
}

void test_equal(const Operator& actual, const Operator& expected) {
    ASSERT_EQ(actual.kind, expected.kind);

    // Corresponding sides must both be null, or neither.
    ASSERT_EQ(actual.left == nullptr, expected.left == nullptr);
    ASSERT_EQ(actual.right == nullptr, expected.right == nullptr);

    if (actual.left != nullptr) {
        test_equal(*actual.left, *expected.left);
    }
    if (actual.right != nullptr) {
        test_equal(*actual.right, *expected.right);
    }
}

void test_equal(const Block& actual, const Block& expected) {
    ASSERT_EQ(actual.statements.size(), expected.statements.size());

    for (std::size_t i = 0; i < actual.statements.size(); ++i) {
        test_not_null_and_equal(actual.statements[i], expected.statements[i]);
    }
}

void test_equal(const Stmt& actual, const Stmt& expected) {
    test_node_variants_equal(actual, expected);
}

void test_equal(const DeclStmt& actual, const DeclStmt& expected) {
    test_not_null_and_equal(actual.decl, expected.decl);
}

void test_equal(const ExprStmt& actual, const ExprStmt& expected) {
    test_not_null_and_equal(actual.expr, expected.expr);
}

}  // namespace lovebird
