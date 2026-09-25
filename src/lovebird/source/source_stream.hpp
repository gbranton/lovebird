#ifndef LOVEBIRD_SOURCE_SOURCE_STREAM_HPP
#define LOVEBIRD_SOURCE_SOURCE_STREAM_HPP

#include <concepts>
#include <cstddef>
#include <iterator>
#include <optional>
#include <string_view>

#include "lovebird/source/source_iterator.hpp"

namespace lovebird {

class SourceStream {
public:
    SourceStream(std::string_view source)
        : it_{source},
          end_{source.data() + source.size()} {
    }

    SourceIterator iterator() const {
        return it_;
    }

    bool done() const {
        return it_ == end_;
    }

    // Gets the current character without advancing.
    std::optional<Char> current() const {
        if (done()) {
            return std::nullopt;
        }
        auto ch = *it_;
        return ch.ch;
    }

    // Looks ahead by n characters.
    std::optional<Char> peek(std::size_t n) const {
        auto it_copy = it_;
        std::ranges::advance(it_copy, n, end_);
        if (it_copy == end_) {
            return std::nullopt;
        }
        auto ch = *it_copy;
        return ch.ch;
    }

    // Gets the current character and advances the stream.
    std::optional<Char> next() {
        if (done()) {
            return std::nullopt;
        }
        auto ch = *it_;
        ++it_;
        return ch.ch;
    }

    // Advances the stream if the current character matches.
    bool match(Char ch) {
        if (current() == ch) {
            ++it_;
            return true;
        }
        return false;
    }

    // Advances the stream if the next two characters match.
    bool match(Char ch1, Char ch2) {
        if (current() == ch1 && peek(1) == ch2) {
            ++it_;
            ++it_;
            return true;
        }
        return false;
    }

    // Advances the stream if the current character matches a predicate.
    template <std::predicate<Char> Predicate>
    bool match(Predicate&& p) {
        auto ch = current();
        if (ch && p(*ch)) {
            ++it_;
            return true;
        }
        return false;
    }

private:
    SourceIterator it_;
    SourceIterator end_;
};

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_SOURCE_SOURCE_STREAM_HPP
