#ifndef ELIXIR_LOOPS_TRANSFORM_VIEW
#define ELIXIR_LOOPS_TRANSFORM_VIEW

#include "elixir/base/config.h"
#include "elixir/base/std/functional.h"
#include "elixir/loops/detail/concepts.h"
#include "elixir/loops/detail/operations.h"

namespace elixir {
namespace loops {

template <typename Loop, typename Map>
  requires loop<Loop>
class transform_view {
 public:
  using base_loop_type = Loop;
  using transform_type = Map;
  using index_type = typename loops::index_t<base_loop_type>;
  using reference = typename loops::reference_t<base_loop_type>;

  constexpr transform_view(Loop&& l, Map&& m) noexcept
      : _base(std::forward<Loop>(l)), _map(std::forward<Map>(m)) {}

  constexpr index_type iter() noexcept { return ::elixir::loops::iter(_base); }

  constexpr index_type next(index_type i) noexcept {
    return ::elixir::loops::next(_base, i);
  }

  constexpr bool valid(index_type i) noexcept {
    return ::elixir::loops::valid(_base, i);
  }

  constexpr reference deref(index_type i) noexcept {
    return _map(::elixir::loops::deref(_base, i));
  }

  template <typename Processor>
  constexpr auto loop_until(Processor&& p) {
    return ::elixir::loops::loop_until(
        _base, [this, _p = std::forward<Processor>(p)](auto&& ref) {
          return _p(map(ref));
        });
  }

 private:
  base_loop_type _base;
  transform_type _map;
};

namespace __impl {
template <typename Map>
struct transform_closure {
  constexpr transform_closure(Map& m) noexcept : _map(m) {}
  constexpr transform_closure(Map&& m) noexcept : _map(std::move(m)) {}

  template <typename Loop>
  constexpr transform_view<typename std::remove_reference<Loop>::type, Map>
  operator()(Loop&& l) && noexcept {
    return transform_view<typename std::remove_reference<Loop>::type, Map>(
        std::forward<Loop>(l), std::move(_map));
  }

  Map _map;
};

template <typename Loop, typename Map>
constexpr transform_view<typename std::remove_reference<Loop>::type, Map>
operator|(Loop&& l, transform_closure<Map>&& c) noexcept {
  return std::move(c)(std::forward<Loop>(l));
}

struct transform_fn {
  template <typename Map>
  constexpr transform_closure<Map> operator()(Map&& m) const noexcept {
    return transform_closure<Map>(std::forward<Map>(m));
  }

  template <typename Loop, typename Map>
  constexpr transform_view<typename std::remove_reference<Loop>::type, Map>
  operator()(Loop&& l, Map&& m) const noexcept {
    return transform_view<typename std::remove_reference<Loop>::type, Map>(
        std::forward<Loop>(l), std::forward<Map>(m));
  }
};
}  // namespace __impl

constexpr auto transform = __impl::transform_fn{};

}  // namespace loops
}  // namespace elixir

#endif
