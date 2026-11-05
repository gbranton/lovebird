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

struct FieldDecl {
    std::optional<std::string_view> name;
    TypeExpr* type = nullptr;
    bool pub = false;
};

enum class StructKind {
    unit,
    bracket,
    brace,
};

struct StructDecl {
    std::string_view name;
    StructKind kind = StructKind::unit;
    std::span<FieldDecl> fields;
};

struct EnumCaseDecl {
    std::string_view name;
    std::span<FieldDecl> fields;
};

struct EnumDecl {
    std::string_view name;
    std::span<EnumCaseDecl> cases;
};

struct FunctionDecl {
    std::string_view name;
    std::span<FieldDecl> params;
    Expr* body = nullptr;
};

using ItemVariant = std::variant<StructDecl, EnumDecl, FunctionDecl>;

struct Item : ItemVariant {
    SourceSpan span;
};

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_SYNTAX_ITEM_HPP
