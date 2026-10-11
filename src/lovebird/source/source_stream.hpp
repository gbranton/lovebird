#ifndef LOVEBIRD_SOURCE_SOURCE_STREAM_HPP
#define LOVEBIRD_SOURCE_SOURCE_STREAM_HPP

#include <concepts>
#include <cstddef>
#include <iterator>
#include <string_view>

#include "lovebird/source/source_iterator.hpp"

namespace lovebird {

constexpr Char source_end_char = 0;

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
    Char current() const {
        if (done()) {
            return source_end_char;
        }
        auto ch = *it_;
        return ch.ch;
    }

    // Looks ahead by n characters.
    Char peek(std::size_t n) const {
        auto it_copy = it_;
        std::ranges::advance(it_copy, n, end_);
        if (it_copy == end_) {
            return source_end_char;
        }
        auto ch = *it_copy;
        return ch.ch;
    }

    // Gets the current character and advances the stream.
    Char next() {
        if (done()) {
            return source_end_char;
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
        if (p(current())) {
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
