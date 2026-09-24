#ifndef LOVEBIRD_LEXER_SOURCE_ITERATOR_HPP
#define LOVEBIRD_LEXER_SOURCE_ITERATOR_HPP

#include <cstddef>
#include <iterator>
#include <string_view>

#include "lovebird/runtime/builtin.hpp"
#include "lovebird/source_position.hpp"
#include "lovebird/unicode/utf8_iterator.hpp"

namespace lovebird {

struct SourceChar {
    Char ch;
    SourcePosition pos;
};

// Iterator over Lovebird source code, including line/column tracking.
class SourceIterator {
public:
    using value_type = SourceChar;
    using difference_type = std::ptrdiff_t;

    SourceIterator() = default;

    SourceIterator(std::string_view sv)
        : it_{sv} {
    }

    SourceIterator(std::string_view sv, SourcePosition pos)
        : it_{sv},
          pos_{pos} {
    }

    SourceIterator(const char* ptr)
        : it_{ptr} {
    }

    SourceIterator(const SourceIterator&) = default;

    SourceIterator(SourceIterator&&) = default;

    SourceIterator& operator=(const SourceIterator&) = default;

    SourceIterator& operator=(SourceIterator&&) = default;

    const char* ptr() const {
        return it_.ptr();
    }

    SourcePosition pos() const {
        return pos_;
    }

    SourceIterator& operator++() {
        const char* before = ptr();
        bool newline = *it_ == '\n';

        ++it_;

        const char* after = ptr();

        if (newline) {
            ++pos_.line;
            pos_.col = 1;
        } else {
            // col is a byte-based offset
            pos_.col += (after - before);
        }

        return *this;
    }

    SourceIterator operator++(int) {
        auto old = *this;
        ++*this;
        return old;
    }

    SourceChar operator*() const {
        return {
            .ch = *it_,
            .pos = pos_,
        };
    }

    friend bool operator==(const SourceIterator& lhs,
                           const SourceIterator& rhs);

private:
    Utf8Iterator it_;
    SourcePosition pos_;
};

inline bool operator==(const SourceIterator& lhs, const SourceIterator& rhs) {
    return lhs.it_ == rhs.it_;
}

static_assert(std::forward_iterator<SourceIterator>);

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_LEXER_SOURCE_ITERATOR_HPP
