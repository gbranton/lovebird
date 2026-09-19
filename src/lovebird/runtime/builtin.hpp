#ifndef LOVEBIRD_RUNTIME_BUILTIN_HPP
#define LOVEBIRD_RUNTIME_BUILTIN_HPP

#include <cstdint>

namespace lovebird {

// A Unicode scalar value (any code point that isn't a surrogate).
using Char = std::uint32_t;

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_RUNTIME_BUILTIN_HPP
