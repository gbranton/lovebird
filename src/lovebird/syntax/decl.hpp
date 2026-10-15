#ifndef LOVEBIRD_SYNTAX_DECL_HPP
#define LOVEBIRD_SYNTAX_DECL_HPP

#include <string_view>
#include <variant>

#include "lovebird/source/source_position.hpp"

namespace lovebird {

struct Expr;

struct LetDecl {
    std::string_view name;
    Expr* value;
};

using DeclVariant = std::variant<LetDecl>;

struct Decl : DeclVariant {
    SourceSpan span;
};

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_SYNTAX_DECL_HPP
