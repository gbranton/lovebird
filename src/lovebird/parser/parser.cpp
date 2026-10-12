#include "lovebird/parser/parser.hpp"

#include <format>
#include <optional>
#include <utility>

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

std::optional<OperatorKind> to_binary_operator_kind(TokenKind kind) {
    switch (kind) {
        case TokenKind::plus:
            return OperatorKind::add;
        case TokenKind::minus:
            return OperatorKind::subtract;
        case TokenKind::star:
            return OperatorKind::multiply;
        case TokenKind::slash:
            return OperatorKind::divide;
        case TokenKind::percent:
            return OperatorKind::remainder;
        default:
            return std::nullopt;
    }
}

int precedence(OperatorKind kind) {
    switch (kind) {
        case OperatorKind::add:
        case OperatorKind::subtract:
            return 1;
        case OperatorKind::multiply:
        case OperatorKind::divide:
        case OperatorKind::remainder:
            return 2;
        case OperatorKind::negate:
            return 3;
    }
}

std::optional<LiteralKind> to_literal_kind(TokenKind kind) {
    switch (kind) {
        case TokenKind::literal_int:
            return LiteralKind::literal_int;
        case TokenKind::literal_char:
            return LiteralKind::literal_char;
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
    return parse_binary_operator(0);
}

const Token* Parser::expect(TokenKind kind) {
    auto* tok = stream_.match(kind);
    if (!tok) {
        diagnostic_emitter_.emit(Diagnostic{
            .message = std::format("expected {}", kind),
        });
    }
    return tok;
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

Expr* Parser::parse_binary_operator(int min_prec) {
    auto* left = parse_unary_operator();
    if (!left) {
        return nullptr;
    }

    while (true) {
        auto [kind, next_prec] = parse_binary_operator_kind(min_prec);
        if (!kind) {
            break;
        }

        auto* right = parse_binary_operator(next_prec);
        if (!right) {
            diagnostic_emitter_.emit(Diagnostic{
                .message = "missing operand for binary operator",
            });
            break;
        }

        left = tree_.make<Expr>(Operator{
            .kind = *kind,
            .left = left,
            .right = right,
        });
    }

    return left;
}

std::optional<OperatorKind> Parser::parse_unary_operator_kind() {
    if (auto kind = to_unary_operator_kind(stream_.kind())) {
        stream_.next();
        return kind;
    }
    return std::nullopt;
}

std::pair<std::optional<OperatorKind>, int> Parser::parse_binary_operator_kind(
    int min_prec) {
    if (auto kind = to_binary_operator_kind(stream_.kind())) {
        int prec = precedence(*kind);
        if (prec > min_prec) {
            stream_.next();
            return {kind, prec};
        }
    }
    return {std::nullopt, 0};
}

Expr* Parser::parse_primary_expr() {
    switch (stream_.kind()) {
        case TokenKind::name:
            return parse_name();
        case TokenKind::literal_int:
        case TokenKind::literal_char:
            return parse_literal();
        case TokenKind::paren_left:
            return parse_block();
        default:
            return nullptr;
    }
}

Expr* Parser::parse_name() {
    if (auto* tok = expect(TokenKind::name)) {
        return tree_.make<Expr>(Name{
            .value = tok->view,
        });
    }
    return nullptr;
}

Expr* Parser::parse_literal() {
    auto lit_kind = to_literal_kind(stream_.kind());
    if (!lit_kind) {
        diagnostic_emitter_.emit(Diagnostic{
            .message = "expected literal",
        });
        return nullptr;
    }

    auto* tok = stream_.current();
    stream_.next();

    return tree_.make<Expr>(Literal{
        .kind = *lit_kind,
        .value = tok->view,
    });
}

Expr* Parser::parse_block() {
    if (!expect(TokenKind::paren_left)) {
        return nullptr;
    }
    auto* inner = parse_expr();
    expect(TokenKind::paren_right);
    return inner;
}

}  // namespace lovebird
