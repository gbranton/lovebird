#ifndef LOVEBIRD_SOURCE_POSITION_HPP
#define LOVEBIRD_SOURCE_POSITION_HPP

#include <format>

namespace lovebird {

struct SourcePosition {
    int line = 1;
    int col = 1;
};

struct SourceSpan {
    SourcePosition begin;
    SourcePosition end;
};

}  // namespace lovebird

template <>
struct std::formatter<lovebird::SourcePosition> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const lovebird::SourcePosition& pos,
                std::format_context& ctx) const {
        return std::format_to(ctx.out(), "{}:{}", pos.line, pos.col);
    }
};

template <>
struct std::formatter<lovebird::SourceSpan> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const lovebird::SourceSpan& span,
                std::format_context& ctx) const {
        return std::format_to(ctx.out(), "{}-{}", span.begin, span.end);
    }
};

#endif  // ifndef LOVEBIRD_SOURCE_POSITION_HPP
