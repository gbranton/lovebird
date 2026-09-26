#ifndef LOVEBIRD_DIAGNOSTIC_DIAGNOSTIC_EMITTER_HPP
#define LOVEBIRD_DIAGNOSTIC_DIAGNOSTIC_EMITTER_HPP

#include <iosfwd>
#include <vector>

#include "lovebird/diagnostic/diagnostic.hpp"

namespace lovebird {

class DiagnosticEmitter {
public:
    DiagnosticEmitter(std::ostream& out);

    void emit(const Diagnostic&);

    const std::vector<Diagnostic>& diagnostics() const {
        return diagnostics_;
    }

private:
    std::ostream& out_;
    std::vector<Diagnostic> diagnostics_;
};

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_DIAGNOSTIC_DIAGNOSTIC_EMITTER_HPP
