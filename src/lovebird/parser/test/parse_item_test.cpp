#include "lovebird/parser/test/parser_test.hpp"

#include <array>
#include <string_view>

#include <gtest/gtest.h>

#include "lovebird/syntax/item.hpp"

namespace lovebird {

namespace {

TEST_F(ParserTest, ParseStructMissingName) {
    std::string_view source = R"(
        struct;
    )";
    parse_item_and_expect_null(source);
    expect_diagnostics({Diagnostic{.message = "expected name"}});
}

TEST_F(ParserTest, ParseUnitStruct) {
    std::string_view source = R"(
        struct Frog;
    )";
    auto* expected = unit_struct("Frog");
    parse_item_and_expect_tree(source, expected);
    expect_no_diagnostics();
}

TEST_F(ParserTest, ParseUnitStructMissingSemicolon) {
    std::string_view source = R"(
        struct Frog
    )";
    auto* expected = unit_struct("Frog");
    parse_item_and_expect_tree(source, expected);
    expect_diagnostics({Diagnostic{.message = "expected ;"}});
}

TEST_F(ParserTest, ParseBraceStruct) {
    std::string_view source = R"(
        struct Puzzle {
            pub clue: String,
            solution: Solution,
        }
    )";
    auto fields = std::array{
        pub_field("clue", type_name("String")),
        field("solution", type_name("Solution")),
    };
    auto* expected = brace_struct("Puzzle", fields);
    parse_item_and_expect_tree(source, expected);
    expect_no_diagnostics();
}

TEST_F(ParserTest, ParseBraceStructMissingCommaAfterField) {
    std::string_view source = R"(
        struct Puzzle {
            pub clue: String,
            solution: Solution
        }
    )";
    auto fields = std::array{
        pub_field("clue", type_name("String")),
        field("solution", type_name("Solution")),
    };
    auto* expected = brace_struct("Puzzle", fields);
    parse_item_and_expect_tree(source, expected);
    expect_diagnostics({Diagnostic{.message = "expected ,"}});
}

TEST_F(ParserTest, ParseBraceStructMissingFieldName) {
    std::string_view source = R"(
        struct Puzzle {
            struct
        }
    )";
    auto* expected = brace_struct("Puzzle", {});
    parse_item_and_expect_tree(source, expected);
    expect_diagnostics({
        Diagnostic{.message = "expected name"},
        Diagnostic{.message = "expected }"},
    });
}

TEST_F(ParserTest, ParseBraceStructMissingColonBeforeFieldType) {
    std::string_view source = R"(
        struct Puzzle {
            pub clue String,
        }
    )";
    auto fields = std::array{
        pub_field("clue", type_name("String")),
    };
    auto* expected = brace_struct("Puzzle", fields);
    parse_item_and_expect_tree(source, expected);
    expect_diagnostics({Diagnostic{.message = "expected :"}});
}

TEST_F(ParserTest, ParseBraceStructMissingFieldType) {
    std::string_view source = R"(
        struct Puzzle {
            pub clue,
        }
    )";
    auto* expected = brace_struct("Puzzle", {});
    parse_item_and_expect_tree(source, expected);
    expect_diagnostics({
        Diagnostic{.message = "expected :"},
        Diagnostic{.message = "expected name"},
        Diagnostic{.message = "expected field type"},
        Diagnostic{.message = "expected }"},
    });
}

}  // namespace

}  // namespace lovebird
