#ifndef ELIXIR_LOOPS_TRANSFORM_VIEW
#define ELIXIR_LOOPS_TRANSFORM_VIEW

#include "elixir/base/config.h"
#include "elixir/base/std/functional.h"
#include "elixir/loops/detail/operations.h"

namespace elixir {
namespace loops {

template <typename Loop, typename Map>
class transform_view;

template <typename Loop, typename Map>
struct is_loop<transform_view<Loop, Map>> : public std::true_type {};

template <typename Loop, typename Map>
class transform_view {
 public:
  using base_loop_type = Loop;
  using transform_type = Map;
  using traits = loop_traits<base_loop_type>;
  using reference = compat::invoke_result_t<Map, typename traits::reference>;
  using value_type = typename std::remove_reference<reference>::type;
  using pointer = typename std::add_pointer<value_type>::type;
  using index_type = typename traits::index_type;

  constexpr transform_view(Loop&& l, Map&& m) noexcept
      : _base(std::forward<Loop>(l)), _map(std::forward<Map>(m)) {
    static_assert(is_loop_v<Loop>,
                  "Base loop for transform is not a elixir loop");
  }

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
