#ifndef ELIXIR_LOOPS_PASS_THROUGH
#define ELIXIR_LOOPS_PASS_THROUGH

#include "elixir/base/config.h"
#include "elixir/loops/detail/operations.h"

namespace elixir {
namespace loops {

template <typename Loop, typename Callable>
class pass_through_view;

template <typename Loop, typename Callable>
struct is_loop<pass_through_view<Loop, Callable>> : public std::true_type {};

template <typename Loop, typename Callable>
class pass_through_view {
 public:
  using base_loop_type = Loop;
  using callable_type = typename std::decay<Callable>::type;
  using traits = loop_traits<base_loop_type>;
  using index_type = typename traits::index_type;
  using reference = typename traits::reference;
  using value_type = typename traits::value_type;
  using pointer = typename traits::pointer;

  template <typename Base>
  constexpr pass_through_view(Base&& b, Callable&& c)
      : _base(std::forward<Base>(b)), _call(std::forward<Callable>(c)) {
    static_assert(is_loop_v<base_loop_type>,
                  "Base loop for filter is not a elixir loop");
  }

  index_type iter() noexcept { return ::elixir::loops::iter(_base); }

  bool valid(index_type i) noexcept { return ::elixir::loops::valid(_base, i); }

  index_type next(index_type i) noexcept {
    return ::elixir::loops::next(_base, i);
  }

  reference deref(index_type i) noexcept {
    reference ref = ::elixir::loops::deref(_base, i);
    _call(ref);
    return ref;
  }

 private:
  base_loop_type _base;
  callable_type _call;
};

namespace __impl {

template <typename Callable>
struct pass_through_closure {
  constexpr pass_through_closure(Callable&& c) noexcept
      : _c(std::forward<Callable>(c)) {}

  template <typename Loop>
  constexpr pass_through_view<typename std::remove_reference<Loop>::type,
                              Callable>
  operator()(Loop&& l) && noexcept {
    return pass_through_view<typename std::remove_reference<Loop>::type,
                             Callable>(std::forward<Loop>(l), std::move(_c));
  }
  Callable _c;
};

template <typename Loop, typename Callable>
constexpr decltype(auto) operator|(
    Loop&& l, pass_through_closure<Callable>&& c) noexcept {
  return std::move(c)(std::forward<Loop>(l));
}

struct pass_through_fn {
  template <typename Callable>
  constexpr pass_through_closure<Callable> operator()(
      Callable&& c) const noexcept {
    return pass_through_closure<Callable>(std::forward<Callable>(c));
  }

  template <typename Loop, typename Callable>
  constexpr pass_through_view<typename std::remove_reference<Loop>::type,
                              Callable>
  operator()(Loop&& l, Callable&& c) const noexcept {
    return pass_through_view<typename std::remove_reference<Loop>::type,
                             Callable>(std::forward<Loop>(l),
                                       std::forward<Callable>(c));
  }
};

}  // namespace __impl

constexpr auto pass_through = __impl::pass_through_fn{};

}  // namespace loops
}  // namespace elixir

#endif
