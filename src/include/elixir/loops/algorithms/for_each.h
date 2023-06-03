#ifndef ELIXIR_LOOPS_FOR_EACH
#define ELIXIR_LOOPS_FOR_EACH

#include "elixir/base/config.h"
#include "elixir/loops/detail/operations.h"

namespace elixir {
namespace loops {

template <typename Loop, typename UnaryFunction>
UnaryFunction for_each(Loop&& l, UnaryFunction&& f) {
  for (auto i = ::elixir::loops::iter(std::forward<Loop>(l));
       ::elixir::loops::valid(std::forward<Loop>(l), i);
       i = ::elixir::loops::next(std::forward<Loop>(l), i)) {
    std::forward<UnaryFunction>(f)(
        ::elixir::loops::deref(std::forward<Loop>(l), i));
  }
  return std::move(f);
}

}  // namespace loops
}  // namespace elixir
#endif
