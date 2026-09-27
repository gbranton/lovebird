#include "lovebird/diagnostic/diagnostic.hpp"

#include <format>
#include <iterator>
#include <ostream>

namespace lovebird {

std::ostream& operator<<(std::ostream& out, const Diagnostic& diagnostic) {
    std::format_to(std::ostreambuf_iterator<char>(out), "{}", diagnostic);
    return out;
}

}  // namespace lovebird
