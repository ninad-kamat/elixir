#ifndef ELIXIR_LOOPS_FIND_IF
#define ELIXIR_LOOPS_FIND_IF

#include "elixir/base/config.h"
#include "elixir/loops/common/functors.h"
#include "elixir/loops/detail/operations.h"

namespace elixir {
namespace loops {

template <typename Loop, typename Predicate>
constexpr auto find_if(Loop&& l, Predicate&& p) {
  using loop_type = typename std::decay<Loop>::type;
  static_assert(is_loop_v<loop_type>,
                "Loops find_if can only operate on valid loops");
  return ::elixir::loops::loop_until(
      std::forward<Loop>(l), ::elixir::make_not(std::forward<Predicate>(p)));
}

template <typename Loop, typename T>
constexpr auto find(Loop&& l, T&& value) {
  using loop_type = typename std::decay<Loop>::type;
  static_assert(is_loop_v<loop_type>,
                "Loops find_if can only operate on valid loops");
  return ::elixir::loops::loop_until(std::forward<Loop>(l),
                                     [&](auto&& ref) { return ref != value; });
}

}  // namespace loops
}  // namespace elixir

#endif
