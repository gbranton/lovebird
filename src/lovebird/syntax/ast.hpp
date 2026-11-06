#ifndef LOVEBIRD_SYNTAX_AST_HPP
#define LOVEBIRD_SYNTAX_AST_HPP

#include <optional>
#include <span>
#include <string_view>
#include <variant>
#include <vector>

#include "lovebird/arena/arena.hpp"
#include "lovebird/runtime/builtin.hpp"
#include "lovebird/source/source_position.hpp"

namespace lovebird {

struct Expr;
struct Stmt;
struct Item;

struct SyntaxTree {
    Arena arena;
    std::vector<Item*> items;
};

// ------------------------------------------------------------
// Expressions

struct Name {
    std::string_view value;
};

enum class LiteralKind {
    literal_int,
    literal_char,
};

struct Literal {
    LiteralKind kind;
    std::string_view value;
};

enum class OperatorKind {
    negate,

    add,
    subtract,
    multiply,
    divide,
    remainder,
};

struct Operator {
    OperatorKind kind;
    Expr* left = nullptr;
    Expr* right = nullptr;
};

struct Block {
    std::span<Stmt*> statements;
};

using ExprVariant = std::variant<Name, Literal, Operator, Block>;

struct Expr : ExprVariant {
    SourceSpan span;
};

// ------------------------------------------------------------
// Type Expressions

using TypeExprVariant = std::variant<Name>;

struct TypeExpr : TypeExprVariant {
    SourceSpan span;
};

// ------------------------------------------------------------
// Items

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
    TypeExpr* return_type = nullptr;
    Expr* body = nullptr;
};

using ItemVariant = std::variant<StructDecl, EnumDecl, FunctionDecl>;

struct Item : ItemVariant {
    SourceSpan span;
};

// ------------------------------------------------------------
// Statements

struct ExprStmt {
    Expr* expr;
};

struct ItemStmt {
    Item* item;
};

struct LetStmt {
    std::string_view name;
    Expr* value;
};

using StmtVariant = std::variant<ExprStmt, ItemStmt, LetStmt>;

struct Stmt : StmtVariant {};

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_SYNTAX_AST_HPP
