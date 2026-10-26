#ifndef LOVEBIRD_SYNTAX_EXPR_HPP
#define LOVEBIRD_SYNTAX_EXPR_HPP

#include <cstdint>
#include <span>
#include <string_view>
#include <variant>

#include "lovebird/runtime/builtin.hpp"
#include "lovebird/source/source_position.hpp"

namespace lovebird {

struct Expr;
struct Stmt;

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

using TypeExprVariant = std::variant<Name>;

struct TypeExpr : TypeExprVariant {
    SourceSpan span;
};

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_SYNTAX_EXPR_HPP
