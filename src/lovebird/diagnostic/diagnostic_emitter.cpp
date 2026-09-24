#include "lovebird/diagnostic/diagnostic_emitter.hpp"

#include <format>
#include <iterator>
#include <ostream>

#include "lovebird/diagnostic/diagnostic.hpp"

namespace lovebird {

DiagnosticEmitter::DiagnosticEmitter(std::ostream& out)
    : out_{out} {
}

void DiagnosticEmitter::emit(const Diagnostic& diagnostic) {
    auto it = std::ostreambuf_iterator<char>(out_);

    std::format_to(it, "{}:", diagnostic.path.string());

    if (diagnostic.span) {
        std::format_to(it, "{}:", diagnostic.span.value());
    }

    std::format_to(it, " {}\n", diagnostic.message);
}

}  // namespace lovebird
