#include "lovebird/syntax/test/equal.hpp"

#include <type_traits>
#include <variant>

#include <gtest/gtest.h>

#include "lovebird/syntax/expr.hpp"

namespace lovebird {

void test_equal(const Expr& actual, const Expr& expected) {
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

void test_equal(const Name& actual, const Name& expected) {
    EXPECT_EQ(actual.value, expected.value);
}

void test_equal(const IntLiteral& actual, const IntLiteral& expected) {
    EXPECT_EQ(actual.value, expected.value);
    EXPECT_EQ(actual.base, expected.base);
}

void test_equal(const CharLiteral& actual, const CharLiteral& expected) {
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

}  // namespace lovebird
