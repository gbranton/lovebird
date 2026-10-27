#include "lovebird/parser/test/parser_test.hpp"

#include <string_view>

#include <gtest/gtest.h>

#include "lovebird/syntax/expr.hpp"

namespace lovebird {

namespace {

TEST_F(ParserTest, ParseTypeName) {
    std::string_view source = "List";
    auto* type_expr = type_name("List");
    parse_type_expr_and_expect_tree(source, type_expr);
    expect_no_diagnostics();
}

}  // namespace

}  // namespace lovebird
