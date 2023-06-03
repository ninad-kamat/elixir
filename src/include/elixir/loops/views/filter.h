#ifndef ELIXIR_LOOPS_FILTER_VIEW
#define ELIXIR_LOOPS_FILTER_VIEW

#include "elixir/base/config.h"
#include "elixir/loops/detail/operations.h"

namespace elixir {
namespace loops {

template <typename Loop, typename Predicate>
class filter_view;

template <typename Loop, typename Predicate>
struct is_loop<filter_view<Loop, Predicate>> : public std::true_type {};

template <typename Loop, typename Predicate>
class filter_view {
 public:
  using base_loop_type = Loop;
  using predicate_type = Predicate;
  using traits = loop_traits<base_loop_type>;
  using reference = typename traits::reference;
  using pointer = typename traits::pointer;
  using value_type = typename traits::value_type;
  using index_type = typename traits::index_type;

  constexpr filter_view(Loop&& base, Predicate&& p) noexcept
      : _base(std::forward<Loop>(base)), _pred(std::forward<Predicate>(p)) {
    static_assert(is_loop_v<Loop>, "Base loop for filter is not a elixir loop");
  }

  constexpr index_type iter() noexcept {
    auto i = ::elixir::loops::iter(_base);
    do {
      auto& ref = ::elixir::loops::deref(_base, i);
      if (_pred(ref)) {
        break;
      }
      i = ::elixir::loops::next(_base, i);
    } while (::elixir::loops::valid(_base, i));
    return i;
  }

  constexpr bool valid(index_type i) noexcept {
    return ::elixir::loops::valid(_base, i);
  }

  constexpr reference deref(index_type i) noexcept {
    return ::elixir::loops::deref(_base, i);
  }

  constexpr index_type next(index_type i) noexcept {
    i = ::elixir::loops::next(_base, i);
    do {
      auto& ref = ::elixir::loops::deref(_base, i);
      if (_pred(ref)) {
        break;
      }
      i = ::elixir::loops::next(_base, i);
    } while (::elixir::loops::valid(_base, i));
    return i;
  }

 private:
  base_loop_type _base;
  predicate_type _pred;
};

namespace __impl {
template <typename Predicate>
struct filter_closure {
  constexpr filter_closure(Predicate& p) noexcept : _pred(p) {}
  constexpr filter_closure(Predicate&& p) noexcept : _pred(std::move(p)) {}

  template <typename Loop>
  constexpr filter_view<typename std::remove_reference<Loop>::type, Predicate>
  operator()(Loop&& l) && noexcept {
    return filter_view<typename std::remove_reference<Loop>::type, Predicate>(
        std::forward<Loop>(l), std::move(_pred));
  }

  Predicate _pred;
};

template <typename Loop, typename Predicate>
decltype(auto) operator|(Loop&& l, filter_closure<Predicate>&& c) noexcept {
  return std::move(c)(std::forward<Loop>(l));
}

struct filter_fn {
  template <typename Predicate>
  constexpr filter_closure<Predicate> operator()(Predicate&& p) const noexcept {
    return filter_closure<Predicate>(std::forward<Predicate>(p));
  }

  template <typename Loop, typename Predicate>
  constexpr filter_view<typename std::remove_reference<Loop>::type, Predicate>
  operator()(Loop&& l, Predicate&& p) const noexcept {
    return filter_view<typename std::remove_reference<Loop>::type, Predicate>(
        std::forward<Loop>(l), std::forward<Predicate>(p));
  }
};
}  // namespace __impl

constexpr auto filter = __impl::filter_fn{};

}  // namespace loops
}  // namespace elixir
#endif
