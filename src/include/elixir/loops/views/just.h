#ifndef ELIXIR_LOOPS_JUST_VIEW
#define ELIXIR_LOOPS_JUST_VIEW

#include "elixir/base/config.h"
#include "elixir/loops/detail/operations.h"
#include "elixir/loops/detail/sequence_traits.h"

namespace elixir {
namespace loops {

template <typename S, typename Traits = sequence_traits<S>>
  requires sequence<S>
class just_view {
 public:
  using base = S;
  using traits = Traits;
  using index_type = decltype(traits::iter(std::declval<base&>()));
  using reference = decltype(traits::deref(std::declval<base&>(),
                                           std::declval<index_type>()));

  constexpr just_view(base& b) noexcept : _base(b) {}

  constexpr auto iter() noexcept(noexcept(traits::iter(_base))) {
    return traits::iter(_base);
  }

  constexpr bool valid(index_type i) noexcept(noexcept(traits::valid(_base,
                                                                     i))) {
    return traits::valid(_base, i);
  }

  constexpr auto deref(index_type i) noexcept(noexcept(traits::deref(_base,
                                                                     i))) {
    return traits::deref(_base, i);
  }

  constexpr auto next(index_type i) noexcept(noexcept(traits::next(_base, i))) {
    return traits::next(_base, i);
  }

  template <typename Processor>
  constexpr auto loop_until(Processor&& p) noexcept(
      noexcept(traits::loop_until(_base, std::forward<Processor>(p)))) {
    return traits::loop_until(_base, std::forward<Processor>(p));
  }

 private:
  base& _base;
};

namespace __impl {
struct just_fn {
  template <typename Range>
  constexpr just_view<typename std::remove_reference<Range>::type> operator()(
      Range&& r) const noexcept {
    return just_view<typename std::remove_reference<Range>::type>(
        std::forward<Range>(r));
  }
};

template <typename Range>
decltype(auto) operator|(Range&& r, just_fn const& j) noexcept {
  return j(std::forward<Range>(r));
}
}  // namespace __impl

constexpr auto just = __impl::just_fn{};

}  // namespace loops
}  // namespace elixir
#endif
