#ifndef LOVEBIRD_DIAGNOSTIC_DIAGNOSTIC_HPP
#define LOVEBIRD_DIAGNOSTIC_DIAGNOSTIC_HPP

#include <filesystem>
#include <format>
#include <optional>
#include <string>
#include <vector>

#include "lovebird/source_position.hpp"

namespace lovebird {

struct Note {
    std::string message;
    std::optional<SourceSpan> span;
};

struct Diagnostic {
    std::string message;
    std::filesystem::path path;
    std::optional<SourceSpan> span;
    std::vector<Note> notes;
};

}  // namespace lovebird

template <>
struct std::formatter<lovebird::Note> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const lovebird::Note& note, std::format_context& ctx) const {
        std::string span_str =
            note.span ? std::format("{}", *note.span) : "null";
        return std::format_to(
            ctx.out(), "{{message=\"{}\", span={}}}", note.message, span_str);
    }
};

template <>
struct std::formatter<lovebird::Diagnostic> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const lovebird::Diagnostic& diagnostic,
                std::format_context& ctx) const {
        std::string span_str =
            diagnostic.span ? std::format("{}", *diagnostic.span) : "null";
        return std::format_to(ctx.out(),
                              "{{message=\"{}\", path={}, span={}}}",
                              diagnostic.message,
                              diagnostic.path.string(),
                              span_str);
    }
};

#endif  // ifndef LOVEBIRD_DIAGNOSTIC_DIAGNOSTIC_HPP
