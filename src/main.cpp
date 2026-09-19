#include <print>

#include "lovebird/source_position.hpp"

using namespace lovebird;

int main() {
    std::println("Hello, lovebird!");

    SourcePosition pos{10, 1};
    std::println("{}", pos);

    SourceSpan span{
        .begin = {2, 27},
        .end = {5, 28},
    };
    std::println("{}", span);

    return 0;
}
