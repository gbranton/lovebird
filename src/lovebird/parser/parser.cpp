#include "lovebird/parser/parser.hpp"

#include <optional>

#include "lovebird/diagnostic/diagnostic.hpp"
#include "lovebird/diagnostic/diagnostic_emitter.hpp"
#include "lovebird/parser/token_stream.hpp"
#include "lovebird/syntax/expr.hpp"
#include "lovebird/syntax/syntax_tree.hpp"
#include "lovebird/syntax/token.hpp"

namespace lovebird {

namespace {

std::optional<OperatorKind> to_unary_operator_kind(TokenKind kind) {
    switch (kind) {
        case TokenKind::minus:
            return OperatorKind::negate;
        default:
            return std::nullopt;
    }
}

}  // namespace

Parser::Parser(TokenStream stream,
               SyntaxTree& tree,
               DiagnosticEmitter& diagnostic_emitter)
    : stream_{stream},
      tree_{tree},
      diagnostic_emitter_{diagnostic_emitter} {
}

Expr* Parser::parse_expr() {
    return parse_unary_operator();
}

Expr* Parser::parse_unary_operator() {
    auto kind = parse_unary_operator_kind();
    if (!kind) {
        return parse_primary_expr();
    }

    auto* right = parse_unary_operator();
    if (!right) {
        diagnostic_emitter_.emit(Diagnostic{
            .message = "missing operand for unary operator",
        });
        return nullptr;
    }

    return tree_.make<Expr>(Operator{
        .kind = *kind,
        .right = right,
    });
}

std::optional<OperatorKind> Parser::parse_unary_operator_kind() {
    auto kind = to_unary_operator_kind(stream_.kind());
    if (kind) {
        stream_.next();
    }
    return kind;
}

Expr* Parser::parse_primary_expr() {
    switch (stream_.kind()) {
        case TokenKind::name:
            return parse_name();
        default:
            return nullptr;
    }
}

Expr* Parser::parse_name() {
    auto* tok = stream_.match(TokenKind::name);
    if (!tok) {
        diagnostic_emitter_.emit(Diagnostic{
            .message = "expected name",
        });
        return nullptr;
    }
    return tree_.make<Expr>(Name{
        .value = tok->view,
    });
}

}  // namespace lovebird
