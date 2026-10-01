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
    }
}

std::ostream& operator<<(std::ostream& out, const Token& tok) {
    std::format_to(std::ostreambuf_iterator<char>(out), "{}", tok);
    return out;
}

}  // namespace lovebird
