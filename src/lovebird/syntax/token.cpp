#include "lovebird/syntax/token.hpp"

#include <format>
#include <iterator>
#include <ostream>
#include <string_view>

namespace lovebird {

std::string_view to_string(TokenKind kind) {
    switch (kind) {
        case TokenKind::end:
            return "end of input";
        case TokenKind::name:
            return "name";
        case TokenKind::keyword_break:
            return "break";
        case TokenKind::keyword_case:
            return "case";
        case TokenKind::keyword_const:
            return "const";
        case TokenKind::keyword_continue:
            return "continue";
        case TokenKind::keyword_default:
            return "default";
        case TokenKind::keyword_else:
            return "else";
        case TokenKind::keyword_embed:
            return "embed";
        case TokenKind::keyword_enum:
            return "enum";
        case TokenKind::keyword_fn:
            return "fn";
        case TokenKind::keyword_for:
            return "for";
        case TokenKind::keyword_if:
            return "if";
        case TokenKind::keyword_impl:
            return "impl";
        case TokenKind::keyword_import:
            return "import";
        case TokenKind::keyword_in:
            return "in";
        case TokenKind::keyword_let:
            return "let";
        case TokenKind::keyword_module:
            return "module";
        case TokenKind::keyword_mut:
            return "mut";
        case TokenKind::keyword_package:
            return "package";
        case TokenKind::keyword_pub:
            return "pub";
        case TokenKind::keyword_return:
            return "return";
        case TokenKind::keyword_self:
            return "self";
        case TokenKind::keyword_struct:
            return "struct";
        case TokenKind::keyword_switch:
            return "switch";
        case TokenKind::keyword_trait:
            return "trait";
        case TokenKind::keyword_type:
            return "type";
        case TokenKind::keyword_unsafe:
            return "unsafe";
    }
}

std::ostream& operator<<(std::ostream& out, const Token& tok) {
    std::format_to(std::ostreambuf_iterator<char>(out), "{}", tok);
    return out;
}

}  // namespace lovebird
