#ifndef ELIXIR_LOOPS_RANGE_ADAPTOR
#define ELIXIR_LOOPS_RANGE_ADAPTOR

#include "elixir/base/config.h"
#include "elixir/loops/detail/concepts.h"
#include "elixir/loops/detail/operations.h"

#include <iterator>

namespace elixir {
namespace loops {

struct default_sentinel {};

/**
 * @brief Iterator adaptor for C++17 range-based for loop
 *
 */
template <typename Loop>
  requires loop<Loop>
class range_iterator {
 public:
  using loop_type = Loop;
  using index_type = typename loops::index_t<loop_type>;
  using reference = typename loops::reference_t<loop_type>;
  using difference_type = std::ptrdiff_t;
  using iterator_category =
      std::input_iterator_tag;  // TODO: find a better way of getting iterator
                                // category using concepts

  constexpr range_iterator(Loop& l) noexcept
      : _base(std::addressof(l)), _idx(::elixir::loops::iter(l)) {}

  constexpr range_iterator& operator++() noexcept {
    increment();
    return *this;
  }

  constexpr range_iterator operator++(int) noexcept {
    auto copy = *this;
    increment();
    return copy;
  }

  constexpr reference operator*() noexcept { return dereference(); }

  friend constexpr bool operator==(range_iterator const& lhs,
                                   range_iterator const& rhs) noexcept {
    return (lhs._base == rhs._base) && (lhs._idx == rhs._idx);
  }

  friend constexpr bool operator!=(range_iterator const& lhs,
                                   range_iterator const& rhs) noexcept {
    return !(lhs == rhs);
  }

  friend constexpr bool operator==(range_iterator const& lhs,
                                   default_sentinel const&) noexcept {
    return lhs.valid();
  }

  friend constexpr bool operator!=(range_iterator const& lhs,
                                   default_sentinel const& rhs) noexcept {
    return !(lhs == rhs);
  }

 private:
  constexpr reference dereference() {
    return ::elixir::loops::deref(*_base, _idx);
  }

  constexpr bool valid() const { return ::elixir::loops::valid(*_base, _idx); }

  constexpr void increment() { _idx = ::elixir::loops::next(*_base, _idx); }

  loop_type* _base = nullptr;
  index_type _idx;
};

template <typename Loop>
  requires loop<Loop>
class range_adaptor {
 public:
  using base_loop_type = Loop;
  using index_type = typename loops::index_t<base_loop_type>;
  using reference = typename loops::reference_t<base_loop_type>;
  using iterator = range_iterator<base_loop_type>;
  using sentinel = default_sentinel;

  template <typename Base>
  constexpr range_adaptor(Base&& l) noexcept : _base(std::forward<Base>(l)) {}

  constexpr iterator begin() noexcept { return iterator(_base); }
  constexpr sentinel end() noexcept { return default_sentinel{}; }

  constexpr index_type iter() noexcept { return ::elixir::loops::iter(_base); }

  constexpr bool valid(index_type i) noexcept {
    return ::elixir::loops::valid(_base, i);
  }

  constexpr index_type next(index_type i) noexcept {
    return ::elixir::loops::next(_base, i);
  }

  constexpr reference deref(index_type i) noexcept {
    return ::elixir::loops::deref(_base, i);
  }

  template <typename Processor>
  constexpr auto loop_until(Processor&& p) {
    return ::elixir::loops::loop_until(
        _base, [this, _p = std::forward<Processor>(p)](auto&& ref) {
          return _p(ref);
        });
  }

 private:
  base_loop_type _base;
};

namespace __impl {

struct range_fn {
  template <typename Loop>
  constexpr range_adaptor<typename std::remove_reference<Loop>::type>
  operator()(Loop&& l) const noexcept {
    return range_adaptor<typename std::remove_reference<Loop>::type>(
        std::forward<Loop>(l));
  }
};

template <typename Loop>
constexpr decltype(auto) operator|(Loop&& l, range_fn const& r) noexcept {
  return r(std::forward<Loop>(l));
}

}  // namespace __impl

constexpr auto range = __impl::range_fn{};

}  // namespace loops
}  // namespace elixir

#endif
