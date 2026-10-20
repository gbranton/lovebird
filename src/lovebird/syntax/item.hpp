#ifndef LOVEBIRD_SYNTAX_ITEM_HPP
#define LOVEBIRD_SYNTAX_ITEM_HPP

#include <span>
#include <string_view>
#include <variant>

#include "lovebird/source/source_position.hpp"

namespace lovebird {

struct Expr;

struct Parameter {
    std::string_view name;
};

struct FunctionItem {
    std::string_view name;
    std::span<Parameter> parameters;
    Expr* body;
};

using ItemVariant = std::variant<FunctionItem>;

struct Item : ItemVariant {
    SourceSpan span;
};

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_SYNTAX_ITEM_HPP
