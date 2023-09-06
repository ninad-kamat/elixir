#ifndef ELIXIR_LOOPS_SEQUENCE_TRAITS
#define ELIXIR_LOOPS_SEQUENCE_TRAITS

#include "elixir/base/config.h"
#include "elixir/loops/detail/concepts.h"

#include <ranges>

namespace elixir {
namespace loops {

template <typename S>
  requires sequence<S>
struct sequence_traits {
  using sequence_iterator_t = std::ranges::iterator_t<S>;
  using sequence_index_t = std::size_t;
  using reference = std::ranges::range_reference_t<S>;

  static sequence_iterator_t iter(S& s) { return std::ranges::begin(s); }
  static sequence_index_t iter(S& s)
    requires contiguous_sequence<S>
  {
    return 0;
  }

  static bool valid(S& s, sequence_iterator_t const& i) {
    return i != std::ranges::end(s);
  }

  static bool valid(S& s, sequence_index_t const& i)
    requires contiguous_sequence<S>
  {
    return i != std::ranges::size(s);
  }

  static sequence_iterator_t next(S& s, sequence_iterator_t const& i) {
    return std::next(i);
  }
  static sequence_index_t next(S& s, sequence_index_t i)
    requires contiguous_sequence<S>
  {
    return ++i;
  }

  static reference deref(S& s, sequence_iterator_t i) {
    // Taking index_type by value here as sometimes operator* on iterators may
    // not be const :(
    return *i;
  }

  static reference deref(S& s, sequence_index_t i)
    requires contiguous_sequence<S>
  {
    // Taking index_type by value here as sometimes operator* on iterators may
    // not be const :(
    return s[i];
  }

  template <typename Processor>
  static std::size_t loop_until(S& s, Processor&& p) {
    auto i = iter(s);
    for (; valid(s, i); i = next(s, i)) {
      auto& ref = deref(s, i);
      if (!std::forward<Processor>(p)(ref)) break;
    }
    return i;
  }
};

}  // namespace loops
}  // namespace elixir

#endif
