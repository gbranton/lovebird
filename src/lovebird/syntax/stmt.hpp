#ifndef LOVEBIRD_SYNTAX_STMT_HPP
#define LOVEBIRD_SYNTAX_STMT_HPP

#include <variant>

namespace lovebird {

struct Decl;
struct Expr;

struct DeclStmt {
    Decl* decl;
};

struct ExprStmt {
    Expr* expr;
};

using StmtVariant = std::variant<DeclStmt, ExprStmt>;

struct Stmt : StmtVariant {};

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_SYNTAX_STMT_HPP
