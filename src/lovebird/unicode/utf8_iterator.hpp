#ifndef LOVEBIRD_UNICODE_UTF8_ITERATOR_HPP
#define LOVEBIRD_UNICODE_UTF8_ITERATOR_HPP

#include <cassert>
#include <cstddef>
#include <iterator>
#include <string_view>

#include "lovebird/runtime/builtin.hpp"
#include "lovebird/unicode/utf8.hpp"

namespace lovebird {

// Forward iterator over UTF-8-encoded text.
class Utf8Iterator {
public:
    using value_type = Char;
    using difference_type = std::ptrdiff_t;

    Utf8Iterator() = default;

    Utf8Iterator(const char* ptr, const char* end)
        : ptr_{ptr},
          end_{end} {
    }

    Utf8Iterator(const char* ptr)
        : Utf8Iterator{ptr, ptr} {
    }

    Utf8Iterator(std::string_view sv)
        : Utf8Iterator{sv.data(), sv.data() + sv.size()} {
    }

    Utf8Iterator(const Utf8Iterator&) = default;

    Utf8Iterator(Utf8Iterator&&) = default;

    Utf8Iterator& operator=(const Utf8Iterator&) = default;

    Utf8Iterator& operator=(Utf8Iterator&&) = default;

    const char* ptr() const {
        return ptr_;
    }

    Utf8Iterator& operator++() {
        assert(ptr_ < end_);
        ptr_ += utf8_sequence_length(*ptr_);
        assert(ptr_ <= end_);
        return *this;
    }

    Utf8Iterator operator++(int) {
        auto old = *this;
        ++*this;
        return old;
    }

    Char operator*() const {
        assert(ptr_ < end_);
        return decode_utf8_sequence(std::string_view{ptr_, end_});
    }

    friend bool operator==(const Utf8Iterator& lhs, const Utf8Iterator& rhs);

private:
    const char* ptr_ = nullptr;
    const char* end_ = nullptr;
};

inline bool operator==(const Utf8Iterator& lhs, const Utf8Iterator& rhs) {
    return lhs.ptr_ == rhs.ptr_ && lhs.end_ == rhs.end_;
}

static_assert(std::forward_iterator<Utf8Iterator>);

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_UNICODE_UTF8_ITERATOR_HPP
