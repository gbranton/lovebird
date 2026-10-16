#ifndef LOVEBIRD_ARENA_BLOCK_HPP
#define LOVEBIRD_ARENA_BLOCK_HPP

#include <cstddef>
#include <memory>

namespace lovebird {

class ArenaBlock {
public:
    explicit ArenaBlock(std::size_t capacity)
        : buf_{std::make_unique_for_overwrite<std::byte[]>(capacity)},
          ptr_{buf_.get()},
          end_{buf_.get() + capacity} {
    }

    std::size_t capacity() const {
        return end_ - buf_.get();
    }

    void* allocate(std::size_t size, std::size_t alignment) {
        void* p = ptr_;
        std::size_t space = end_ - ptr_;
        p = std::align(alignment, size, p, space);
        if (p) {
            ptr_ = static_cast<std::byte*>(p) + size;
        }
        return p;
    }

private:
    std::unique_ptr<std::byte[]> buf_;
    std::byte* ptr_;
    std::byte* end_;
};

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_ARENA_BLOCK_HPP
