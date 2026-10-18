#ifndef LOVEBIRD_ARENA_ARENA_HPP
#define LOVEBIRD_ARENA_ARENA_HPP

#include <algorithm>
#include <cassert>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "lovebird/arena/block.hpp"

namespace lovebird {

class Arena {
public:
    Arena() = default;

    Arena(const Arena&) = delete;
    Arena(Arena&&) = delete;
    Arena& operator=(const Arena&) = delete;
    Arena& operator=(Arena&&) = delete;

    template <class T, class... Args>
    T* make(Args&&... args) {
        static_assert(std::is_trivially_destructible_v<T>);

        auto* p = static_cast<T*>(allocate(sizeof(T), alignof(T)));
        return std::construct_at(p, std::forward<Args>(args)...);
    }

private:
    std::vector<ArenaBlock> blocks_;
    std::vector<ArenaBlock> large_blocks_;
    std::size_t next_capacity_ = first_block_capacity;

    static constexpr std::size_t first_block_capacity = 0x1000;  // 4 KiB
    static constexpr std::size_t max_block_capacity = 0x10000;   // 64 KiB

    ArenaBlock* current() {
        if (blocks_.empty()) {
            return nullptr;
        }
        return &blocks_.back();
    }

    ArenaBlock* make_block(std::size_t size, std::size_t alignment) {
        std::size_t min_capacity = size + alignment - 1;

        // Sufficiently large allocations get their own block.
        if (min_capacity > next_capacity_) {
            large_blocks_.emplace_back(min_capacity);
            return &large_blocks_.back();
        }

        std::size_t capacity = next_capacity_;
        next_capacity_ = std::min(next_capacity_ * 2, max_block_capacity);

        blocks_.emplace_back(capacity);
        return &blocks_.back();
    }

    void* allocate(std::size_t size, std::size_t alignment) {
        ArenaBlock* block = current();
        if (!block) {
            block = make_block(size, alignment);
        }

        auto* p = block->allocate(size, alignment);
        if (!p) {
            block = make_block(size, alignment);
            p = block->allocate(size, alignment);
        }

        assert(p);
        return p;
    }
};

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_ARENA_ARENA_HPP
