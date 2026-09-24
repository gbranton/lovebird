#ifndef LOVEBIRD_DIAGNOSTIC_DIAGNOSTIC_EMITTER_HPP
#define LOVEBIRD_DIAGNOSTIC_DIAGNOSTIC_EMITTER_HPP

#include <iosfwd>

namespace lovebird {

struct Diagnostic;

class DiagnosticEmitter {
public:
    DiagnosticEmitter(std::ostream& out);

    void emit(const Diagnostic&);

private:
    std::ostream& out_;
};

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_DIAGNOSTIC_DIAGNOSTIC_EMITTER_HPP
