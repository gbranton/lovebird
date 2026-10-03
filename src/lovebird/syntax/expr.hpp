#ifndef LOVEBIRD_SYNTAX_EXPR_HPP
#define LOVEBIRD_SYNTAX_EXPR_HPP

#include <cstdint>
#include <string_view>
#include <type_traits>
#include <variant>

#include "lovebird/runtime/builtin.hpp"
#include "lovebird/source/source_position.hpp"

namespace lovebird {

struct Expr;

struct Name {
    std::string_view value;
};

enum class IntBase { bin, oct, dec, hex };

struct IntLiteral {
    std::uint64_t value;
    IntBase base;
};

struct CharLiteral {
    Char value;
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

using ExprVariant = std::variant<Name, IntLiteral, CharLiteral, Operator>;

struct Expr : ExprVariant {
    SourceSpan span;
};

static_assert(std::is_trivially_destructible_v<Expr>);

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_SYNTAX_EXPR_HPP
