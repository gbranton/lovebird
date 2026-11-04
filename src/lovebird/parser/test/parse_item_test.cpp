#include "lovebird/parser/test/parser_test.hpp"

#include <array>
#include <optional>
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

TEST_F(ParserTest, ParseBracketStruct) {
    std::string_view source = R"(
        struct Vec2[float, float];
    )";
    auto fields = std::array{
        pub_field(std::nullopt, type_name("float")),
        pub_field(std::nullopt, type_name("float")),
    };
    auto* expected = bracket_struct("Vec2", fields);
    parse_item_and_expect_tree(source, expected);
    expect_no_diagnostics();
}

TEST_F(ParserTest, ParseEmptyBracketStruct) {
    std::string_view source = R"(
        struct Empty[];
    )";
    auto* expected = bracket_struct("Empty", {});
    parse_item_and_expect_tree(source, expected);
    expect_no_diagnostics();
}

TEST_F(ParserTest, ParseBracketStructMissingRightBracket) {
    std::string_view source = R"(
        struct Vec2[float, float;
    )";
    auto fields = std::array{
        pub_field(std::nullopt, type_name("float")),
        pub_field(std::nullopt, type_name("float")),
    };
    auto* expected = bracket_struct("Vec2", fields);
    parse_item_and_expect_tree(source, expected);
    expect_diagnostics({Diagnostic{.message = "expected ]"}});
}

TEST_F(ParserTest, ParseBracketStructMissingSemicolon) {
    std::string_view source = R"(
        struct Vec2[float, float]
    )";
    auto fields = std::array{
        pub_field(std::nullopt, type_name("float")),
        pub_field(std::nullopt, type_name("float")),
    };
    auto* expected = bracket_struct("Vec2", fields);
    parse_item_and_expect_tree(source, expected);
    expect_diagnostics({Diagnostic{.message = "expected ;"}});
}

TEST_F(ParserTest, ParseBracketStructWithFieldNames) {
    std::string_view source = R"(
        struct Vec2[x: float, y: float];
    )";
    auto fields = std::array{
        pub_field("x", type_name("float")),
        pub_field("y", type_name("float")),
    };
    auto* expected = bracket_struct("Vec2", fields);
    parse_item_and_expect_tree(source, expected);
    expect_no_diagnostics();
}

TEST_F(ParserTest, ParseBracketStructMissingComma) {
    std::string_view source = R"(
        struct Vec2[float float];
    )";
    auto fields = std::array{
        pub_field(std::nullopt, type_name("float")),
        pub_field(std::nullopt, type_name("float")),
    };
    auto* expected = bracket_struct("Vec2", fields);
    parse_item_and_expect_tree(source, expected);
    expect_diagnostics({Diagnostic{.message = "expected field separator ,"}});
}

TEST_F(ParserTest, ParseBracketUnexpectedDeclaration) {
    std::string_view source = R"(
        struct Vec2[float,

        struct Unexpected;
    )";
    auto fields = std::array{
        pub_field(std::nullopt, type_name("float")),
    };
    auto* expected = bracket_struct("Vec2", fields);
    parse_item_and_expect_tree(source, expected);
    expect_diagnostics({
        Diagnostic{.message = "expected ]"},
        Diagnostic{.message = "expected ;"},
    });
}

TEST_F(ParserTest, ParseBracketStructWithPubKeyword) {
    std::string_view source = R"(
        struct Cat[pub name: String];
    )";
    auto fields = std::array{
        pub_field("name", type_name("String")),
    };
    auto* expected = bracket_struct("Cat", fields);
    parse_item_and_expect_tree(source, expected);
    expect_diagnostics({
        Diagnostic{.message = "public modifier is not allowed here"},
    });
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

TEST_F(ParserTest, ParseEmptyBraceStruct) {
    std::string_view source = R"(
        struct Empty {}
    )";
    auto* expected = brace_struct("Empty", {});
    parse_item_and_expect_tree(source, expected);
    expect_no_diagnostics();
}

TEST_F(ParserTest, ParseBraceStructMissingRightBrace) {
    std::string_view source = R"(
        struct Puzzle {
            pub clue: String,
            solution: Solution,
    )";
    auto fields = std::array{
        pub_field("clue", type_name("String")),
        field("solution", type_name("Solution")),
    };
    auto* expected = brace_struct("Puzzle", fields);
    parse_item_and_expect_tree(source, expected);
    expect_diagnostics({Diagnostic{.message = "expected }"}});
}

TEST_F(ParserTest, ParseBraceStructMissingComma) {
    std::string_view source = R"(
        struct Puzzle {
            pub clue: String
            solution: Solution,
        }
    )";
    auto fields = std::array{
        pub_field("clue", type_name("String")),
        field("solution", type_name("Solution")),
    };
    auto* expected = brace_struct("Puzzle", fields);
    parse_item_and_expect_tree(source, expected);
    expect_diagnostics({Diagnostic{.message = "expected field separator ,"}});
}

TEST_F(ParserTest, ParseBraceStructUnexpectedDeclaration) {
    std::string_view source = R"(
        struct Puzzle {

        struct Unexpected;
    )";
    auto* expected = brace_struct("Puzzle", {});
    parse_item_and_expect_tree(source, expected);
    expect_diagnostics({Diagnostic{.message = "expected }"}});
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
    expect_diagnostics({Diagnostic{.message = "expected field type"}});
}

}  // namespace

}  // namespace lovebird
