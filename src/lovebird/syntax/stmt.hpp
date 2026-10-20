#ifndef LOVEBIRD_SYNTAX_STMT_HPP
#define LOVEBIRD_SYNTAX_STMT_HPP

#include <string_view>
#include <variant>

namespace lovebird {

struct Expr;
struct Item;

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

#endif  // ifndef LOVEBIRD_SYNTAX_STMT_HPP
