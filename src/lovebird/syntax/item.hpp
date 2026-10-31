#ifndef LOVEBIRD_SYNTAX_ITEM_HPP
#define LOVEBIRD_SYNTAX_ITEM_HPP

#include <optional>
#include <span>
#include <string_view>
#include <variant>

#include "lovebird/source/source_position.hpp"

namespace lovebird {

struct Expr;
struct TypeExpr;

struct Field {
    std::optional<std::string_view> name;
    TypeExpr* type = nullptr;
    bool pub = false;
};

enum class StructKind {
    unit,
    bracket,
    brace,
};

struct Struct {
    std::string_view name;
    std::span<Field> fields;
    StructKind kind = StructKind::unit;
};

struct EnumCase {
    std::string_view name;
    std::span<Field> fields;
};

struct Enum {
    std::string_view name;
    std::span<EnumCase> cases;
};

struct Function {
    std::string_view name;
    std::span<Field> params;
    Expr* body = nullptr;
};

using ItemVariant = std::variant<Struct, Enum, Function>;

struct Item : ItemVariant {
    SourceSpan span;
};

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_SYNTAX_ITEM_HPP
