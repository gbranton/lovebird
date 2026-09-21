#include "lovebird/syntax/token.hpp"

#include <string_view>

namespace lovebird {

std::string_view to_string(TokenKind kind) {
    switch (kind) {
        case TokenKind::name:
            return "name";
    }
}

}  // namespace lovebird
