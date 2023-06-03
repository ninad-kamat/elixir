#ifndef ELIXIR_LOOPS_OPERATIONS
#define ELIXIR_LOOPS_OPERATIONS

#include "elixir/base/config.h"
#include "elixir/loops/detail/traits.h"

namespace elixir {
namespace loops {

namespace __impl {

template <typename Loop>
constexpr auto iter(Loop&& l) noexcept(noexcept(std::forward<Loop>(l).iter()))
    -> decltype(std::forward<Loop>(l).iter()) {
  return std::forward<Loop>(l).iter();
}

struct iter_fn {
  template <typename Loop>
  constexpr auto operator()(Loop&& l) const
      noexcept(noexcept(iter(std::forward<Loop>(l))))
          -> decltype(iter(std::forward<Loop>(l))) {
    return iter(std::forward<Loop>(l));
  }
};

template <typename Loop, typename Index>
constexpr auto valid(Loop&& l, Index&& i) noexcept(
    noexcept(std::forward<Loop>(l).valid(std::forward<Index>(i))))
    -> decltype(std::forward<Loop>(l).valid(std::forward<Index>(i))) {
  return std::forward<Loop>(l).valid(std::forward<Index>(i));
}

struct valid_fn {
  template <typename Loop, typename Index>
  constexpr auto operator()(Loop&& l, Index&& i) const
      noexcept(noexcept(valid(std::forward<Loop>(l), std::forward<Index>(i))))
          -> decltype(valid(std::forward<Loop>(l), std::forward<Index>(i))) {
    return valid(std::forward<Loop>(l), std::forward<Index>(i));
  }
};

template <typename Loop, typename Index>
constexpr auto deref(Loop&& l, Index&& i) noexcept(
    noexcept(std::forward<Loop>(l).deref(std::forward<Index>(i))))
    -> decltype(std::forward<Loop>(l).deref(std::forward<Index>(i))) {
  return std::forward<Loop>(l).deref(std::forward<Index>(i));
}

struct deref_fn {
  template <typename Loop, typename Index>
  constexpr auto operator()(Loop&& l, Index&& i) const
      noexcept(noexcept(deref(std::forward<Loop>(l), std::forward<Index>(i))))
          -> decltype(deref(std::forward<Loop>(l), std::forward<Index>(i))) {
    return deref(std::forward<Loop>(l), std::forward<Index>(i));
  }
};

template <typename Loop, typename Index>
constexpr auto next(Loop&& l, Index&& i) noexcept(
    noexcept(std::forward<Loop>(l).next(std::forward<Index>(i))))
    -> decltype(std::forward<Loop>(l).next(std::forward<Index>(i))) {
  return std::forward<Loop>(l).next(std::forward<Index>(i));
}

struct next_fn {
  template <typename Loop, typename Index>
  constexpr auto operator()(Loop&& l, Index&& i) const
      noexcept(noexcept(next(std::forward<Loop>(l), std::forward<Index>(i))))
          -> decltype(next(std::forward<Loop>(l), std::forward<Index>(i))) {
    return next(std::forward<Loop>(l), std::forward<Index>(i));
  }
};

}  // namespace __impl

constexpr auto iter = __impl::iter_fn{};
constexpr auto valid = __impl::valid_fn{};
constexpr auto deref = __impl::deref_fn{};
constexpr auto next = __impl::next_fn{};

}  // namespace loops
}  // namespace elixir
#endif
