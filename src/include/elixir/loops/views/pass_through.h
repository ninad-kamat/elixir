#ifndef ELIXIR_LOOPS_PASS_THROUGH
#define ELIXIR_LOOPS_PASS_THROUGH

#include "elixir/base/config.h"
#include "elixir/loops/detail/concepts.h"
#include "elixir/loops/detail/operations.h"

namespace elixir {
namespace loops {

template <typename Loop, typename Callable>
  requires loop<Loop>
class pass_through_view {
 public:
  using base_loop_type = Loop;
  using callable_type = typename std::decay<Callable>::type;
  using index_type = typename loops::index_t<base_loop_type>;
  using reference = typename loops::reference_t<base_loop_type>;

  template <typename Base>
  constexpr pass_through_view(Base&& b, Callable&& c)
      : _base(std::forward<Base>(b)), _call(std::forward<Callable>(c)) {}

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

  template <typename Processor>
  constexpr auto loop_until(Processor&& p) {
    return ::elixir::loops::loop_until(
        _base, [this, _p = std::forward<Processor>(p)](auto&& ref) {
          _call(ref);
          return true;
        });
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
