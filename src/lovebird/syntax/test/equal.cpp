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
void expect_not_null_and_equal(const T* actual, const T* expected) {
    ASSERT_NE(actual, nullptr);
    ASSERT_NE(expected, nullptr);
    expect_equal(*actual, *expected);
}

template <class T>
void expect_node_variants_equal(const T& actual, const T& expected) {
    std::visit(
        [](const auto& actual, const auto& expected) {
            if constexpr (std::is_same_v<decltype(actual),
                                         decltype(expected)>) {
                expect_equal(actual, expected);
            } else {
                FAIL() << "node type mismatch: " << typeid(actual).name()
                       << ", " << typeid(expected).name();
            }
        },
        actual,
        expected);
}

}  // namespace

void expect_equal(const Decl& actual, const Decl& expected) {
    expect_node_variants_equal(actual, expected);
}

void expect_equal(const LetDecl& actual, const LetDecl& expected) {
    EXPECT_EQ(actual.name, expected.name);
    expect_not_null_and_equal(actual.value, expected.value);
}

void expect_equal(const Expr& actual, const Expr& expected) {
    expect_node_variants_equal(actual, expected);
}

void expect_equal(const Name& actual, const Name& expected) {
    EXPECT_EQ(actual.value, expected.value);
}

void expect_equal(const Literal& actual, const Literal& expected) {
    EXPECT_EQ(actual.kind, expected.kind);
    EXPECT_EQ(actual.value, expected.value);
}

void expect_equal(const Operator& actual, const Operator& expected) {
    ASSERT_EQ(actual.kind, expected.kind);

    // Corresponding sides must both be null, or neither.
    ASSERT_EQ(actual.left == nullptr, expected.left == nullptr);
    ASSERT_EQ(actual.right == nullptr, expected.right == nullptr);

    if (actual.left != nullptr) {
        expect_equal(*actual.left, *expected.left);
    }
    if (actual.right != nullptr) {
        expect_equal(*actual.right, *expected.right);
    }
}

void expect_equal(const Block& actual, const Block& expected) {
    ASSERT_EQ(actual.statements.size(), expected.statements.size());

    for (std::size_t i = 0; i < actual.statements.size(); ++i) {
        expect_not_null_and_equal(actual.statements[i], expected.statements[i]);
    }
}

void expect_equal(const Stmt& actual, const Stmt& expected) {
    expect_node_variants_equal(actual, expected);
}

void expect_equal(const DeclStmt& actual, const DeclStmt& expected) {
    expect_not_null_and_equal(actual.decl, expected.decl);
}

void expect_equal(const ExprStmt& actual, const ExprStmt& expected) {
    expect_not_null_and_equal(actual.expr, expected.expr);
}

}  // namespace lovebird
