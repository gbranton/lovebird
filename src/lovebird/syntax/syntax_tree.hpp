#ifndef LOVEBIRD_SYNTAX_SYNTAX_TREE_HPP
#define LOVEBIRD_SYNTAX_SYNTAX_TREE_HPP

#include <utility>

#include "lovebird/arena/arena.hpp"

namespace lovebird {

class SyntaxTree {
public:
    template <class T, class... Args>
    T* make(Args&&... args) {
        return arena_.make<T>(std::forward<Args>(args)...);
    }

private:
    Arena arena_;
};

}  // namespace lovebird

#endif  // ifndef LOVEBIRD_SYNTAX_SYNTAX_TREE_HPP
