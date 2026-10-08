#ifndef LOVEBIRD_SYNTAX_SYNTAX_TREE_HPP
#define LOVEBIRD_SYNTAX_SYNTAX_TREE_HPP

#include <memory>
#include <memory_resource>
#include <type_traits>
#include <utility>

namespace lovebird {

class SyntaxTree {
public:
    template <class T, class... Args>
    T* make(Args&&... args) {
        // Destructors are never called, so T must be trivially destructible.
        static_assert(std::is_trivially_destructible_v<T>);

        auto* p = static_cast<T*>(resource_.allocate(sizeof(T), alignof(T)));
        return std::construct_at(p, std::forward<Args>(args)...);
    }

private:
    std::pmr::monotonic_buffer_resource resource_;
};

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_SYNTAX_SYNTAX_TREE_HPP
