#ifndef ELIXIR_LOOPS_COMMON_FUNCTORS
#define ELIXIR_LOOPS_COMMON_FUNCTORS

namespace elixir {
namespace __impl {

template <typename Predicate>
struct not_fn_closure {
  constexpr not_fn_closure(Predicate&& p) noexcept
      : _pred(std::forward<Predicate>(p)) {}

  template <typename... Args>
  constexpr bool operator()(Args&&... args) const
      noexcept(noexcept(_pred(std::forward<Args>(args)...))) {
    return !_pred(std::forward<Args>(args)...);
  }
  Predicate _pred;
};

struct not_fn {
  template <typename Predicate>
  constexpr not_fn_closure<Predicate> operator()(Predicate&& p) const noexcept {
    return not_fn_closure<Predicate>(std::forward<Predicate>(p));
  }
};

struct always_true_fn {
  template <typename... Args>
  constexpr bool operator()(Args&&... args) const noexcept {
    return true;
  }
};

struct always_false_fn {
  template <typename... Args>
  constexpr bool operator()(Args&&... args) const noexcept {
    return false;
  }
};

}  // namespace __impl

constexpr auto not_fn = __impl::not_fn{};
constexpr auto always_true = __impl::always_true_fn{};
constexpr auto always_false = __impl::always_false_fn{};

}  // namespace elixir

#endif
