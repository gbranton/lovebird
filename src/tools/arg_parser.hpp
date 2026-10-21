#ifndef LOVEBIRD_TOOLS_ARG_PARSER_HPP
#define LOVEBIRD_TOOLS_ARG_PARSER_HPP

#include <algorithm>
#include <expected>
#include <format>
#include <iterator>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace lovebird::tools {

struct InvalidArgError {
    std::string arg;
};

class ArgParser {
public:
    ArgParser(std::string tool_name, std::string tool_description)
        : tool_name_{tool_name},
          tool_description_{tool_description} {
    }

    void positional(std::string_view name, std::vector<std::string>& out) {
        positional_.emplace(Positional{
            .name = std::string{name},
            .out = out,
        });
    }

    void flag(std::string_view name,
              std::string_view short_name,
              std::string_view description,
              bool& out) {
        flags_.emplace_back(Flag{
            .name = std::string{name},
            .short_name = std::string{short_name},
            .description = std::string{description},
            .out = out,
        });
    }

    std::expected<void, InvalidArgError> parse(int argc, char* argv[]) const {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];

            if (match_flag(arg) || match_positional(arg)) {
                continue;
            }

            return std::unexpected(InvalidArgError{.arg = arg});
        }

        return {};
    }

    std::string help() const {
        std::string str;
        auto it = std::back_inserter(str);

        std::format_to(it, "{} - {}\n\n", tool_name_, tool_description_);

        // Usage line
        std::format_to(it, "Usage: {}", tool_name_);
        if (!flags_.empty()) {
            str += " [options]";
        }
        if (positional_) {
            std::format_to(it, " {}...", positional_->name);
        }
        str += "\n";

        // Options
        if (!flags_.empty()) {
            str += "\nOptions:\n";
        }
        for (const auto& flag : flags_) {
            auto flag_names = std::format("{}, {}", flag.name, flag.short_name);
            std::format_to(it, "  {:<20} {}\n", flag_names, flag.description);
        }

        return str;
    }

private:
    struct Positional {
        std::string name;
        std::vector<std::string>& out;
    };

    struct Flag {
        std::string name;
        std::string short_name;
        std::string description;
        bool& out;
    };

    std::string tool_name_;
    std::string tool_description_;
    std::optional<Positional> positional_;
    std::vector<Flag> flags_;

    bool match_flag(std::string_view arg) const {
        auto it = std::ranges::find_if(flags_, [&](const auto& flag) {
            return arg == flag.name || arg == flag.short_name;
        });
        if (it == flags_.end()) {
            return false;
        }
        it->out = true;
        return true;
    }

    bool match_positional(std::string_view arg) const {
        if (!positional_ || arg.starts_with("-")) {
            return false;
        }
        positional_->out.emplace_back(arg);
        return true;
    }
};

}  // namespace lovebird::tools

#endif  // ifndef LOVEBIRD_TOOLS_ARG_PARSER_HPP
