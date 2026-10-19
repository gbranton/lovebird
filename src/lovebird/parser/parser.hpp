#ifndef LOVEBIRD_PARSER_PARSER_HPP
#define LOVEBIRD_PARSER_PARSER_HPP

#include <optional>
#include <utility>

#include "lovebird/diagnostic/diagnostic_emitter.hpp"
#include "lovebird/parser/token_stream.hpp"
#include "lovebird/syntax/expr.hpp"
#include "lovebird/syntax/stmt.hpp"
#include "lovebird/syntax/syntax_tree.hpp"
#include "lovebird/syntax/token.hpp"

namespace lovebird {

class Parser {
public:
    Parser(TokenStream stream,
           SyntaxTree& tree,
           DiagnosticEmitter& diagnostic_emitter);

    Expr* parse_expr();
    Stmt* parse_stmt();

private:
    TokenStream stream_;
    SyntaxTree& tree_;
    DiagnosticEmitter& diagnostic_emitter_;

    template <class... Args>
    Expr* make_expr(Args&&... args) {
        return tree_.arena.make<Expr>(std::forward<Args>(args)...);
    }

    template <class... Args>
    Stmt* make_stmt(Args&&... args) {
        return tree_.arena.make<Stmt>(std::forward<Args>(args)...);
    }

    const Token* expect(TokenKind);

    Expr* parse_unary_operator();
    Expr* parse_binary_operator(int min_prec);

    std::optional<OperatorKind> parse_unary_operator_kind();
    std::pair<std::optional<OperatorKind>, int> parse_binary_operator_kind(
        int min_prec);

    Expr* parse_primary_expr();
    Expr* parse_name();
    Expr* parse_literal();
    Expr* parse_block();
};

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_PARSER_PARSER_HPP
