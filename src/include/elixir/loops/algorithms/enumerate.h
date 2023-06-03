#ifndef ELIXIR_LOOPS_ENUMERATE
#define ELIXIR_LOOPS_ENUMERATE

#include "elixir/base/config.h"
#include "elixir/loops/detail/operations.h"

namespace elixir {
namespace loops {

template <typename Loop, typename Callable>
Callable enumerate(Loop&& l, Callable&& c) {
  std::size_t idx = 0;
  for (auto i = ::elixir::loops::iter(std::forward<Loop>(l));
       ::elixir::loops::valid(std::forward<Loop>(l), i);
       i = ::elixir::loops::next(std::forward<Loop>(l), i)) {
    std::forward<Callable>(c)(idx++,
                              ::elixir::loops::deref(std::forward<Loop>(l), i));
  }
  return std::move(c);
}

}  // namespace loops
}  // namespace elixir
#endif
