#ifndef LOVEBIRD_PARSER_PARSER_HPP
#define LOVEBIRD_PARSER_PARSER_HPP

#include <optional>
#include <string_view>
#include <utility>

#include "lovebird/diagnostic/diagnostic_emitter.hpp"
#include "lovebird/parser/token_stream.hpp"
#include "lovebird/syntax/expr.hpp"
#include "lovebird/syntax/item.hpp"
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
    TypeExpr* parse_type_expr();
    Item* parse_item();
    Stmt* parse_stmt();

private:
    TokenStream stream_;
    SyntaxTree& tree_;
    DiagnosticEmitter& diagnostic_emitter_;

    template <class T, class... Args>
    T* make(Args&&... args) {
        return tree_.arena.make<T>(std::forward<Args>(args)...);
    }

    const Token* expect(TokenKind);

    std::optional<std::string_view> parse_name();

    Expr* parse_unary_operator();
    Expr* parse_binary_operator(int min_prec);

    std::optional<OperatorKind> parse_unary_operator_kind();
    std::pair<std::optional<OperatorKind>, int> parse_binary_operator_kind(
        int min_prec);

    Expr* parse_primary_expr();
    Expr* parse_name_expr();
    Expr* parse_literal();
    Expr* parse_parenthesized_expr();
    Expr* parse_block();

    TypeExpr* parse_type_name();

    Item* parse_struct();
};

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_PARSER_PARSER_HPP
