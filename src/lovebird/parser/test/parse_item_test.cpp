#include "lovebird/parser/test/parser_test.hpp"

#include <string_view>

#include <gtest/gtest.h>

#include "lovebird/syntax/item.hpp"

namespace lovebird {

namespace {

TEST_F(ParserTest, ParseUnitStruct) {
    std::string_view source = R"(
        struct Frog;
    )";
    auto* expected = function("Frog", StructKind::unit);
    parse_item_and_expect_tree(source, expected);
    expect_no_diagnostics();
}

}  // namespace

}  // namespace lovebird
