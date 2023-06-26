#ifndef ELIXIR_LOOPS_JUST_VIEW
#define ELIXIR_LOOPS_JUST_VIEW

#include "elixir/base/config.h"
#include "elixir/loops/detail/operations.h"

namespace elixir {
namespace loops {

template <typename Range, class Enable = void>
class just_view;

template <typename Range>
struct is_loop<just_view<Range>> : public std::true_type {};

template <typename Range>
class just_view<
    Range, typename std::enable_if<is_contiguous_sequence_v<Range>>::type> {
 public:
  using base = Range;
  using traits = range_traits<base>;
  using value_type = typename traits::value_type;
  using reference = typename traits::reference;
  using pointer = typename traits::pointer;
  using index_type = typename traits::index_type;
  static constexpr bool reversible_v = true;

  constexpr just_view(base& b) noexcept : _base(b) {}

  constexpr index_type iter() noexcept(noexcept(traits::iter(_base))) {
    return traits::iter(_base);
  }

  constexpr bool valid(index_type i) noexcept(noexcept(traits::valid(_base,
                                                                     i))) {
    return traits::valid(_base, i);
  }

  constexpr reference deref(index_type i) noexcept(noexcept(traits::deref(_base,
                                                                          i))) {
    return traits::deref(_base, i);
  }

  constexpr index_type next(index_type i) noexcept(noexcept(traits::next(_base,
                                                                         i))) {
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
