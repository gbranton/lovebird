#ifndef LOVEBIRD_PARSER_PARSER_HPP
#define LOVEBIRD_PARSER_PARSER_HPP

#include <optional>

#include "lovebird/diagnostic/diagnostic_emitter.hpp"
#include "lovebird/parser/token_stream.hpp"
#include "lovebird/syntax/expr.hpp"
#include "lovebird/syntax/syntax_tree.hpp"

namespace lovebird {

class Parser {
public:
    Parser(TokenStream stream,
           SyntaxTree& tree,
           DiagnosticEmitter& diagnostic_emitter);

    Expr* parse_expr();

private:
    TokenStream stream_;
    SyntaxTree& tree_;
    DiagnosticEmitter& diagnostic_emitter_;

    Expr* parse_unary_operator();

    std::optional<OperatorKind> parse_unary_operator_kind();

    Expr* parse_primary_expr();
    Expr* parse_name();
};

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_PARSER_PARSER_HPP
