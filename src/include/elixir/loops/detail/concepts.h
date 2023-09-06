#ifndef ELIXIR_LOOPS_CONCEPTS
#define ELIXIR_LOOPS_CONCEPTS

#if !ELIXIR_CXX20_ENABLED
#error "Loops library uses concepts and needs C++20 support."
#endif

#include "elixir/base/config.h"
#include "elixir/loops/detail/operations.h"

#include <concepts>
#include <ranges>

namespace elixir {
namespace loops {

template <class B>
concept boolean_testable_impl = std::convertible_to<B, bool>;

template <class B>
concept boolean_testable = boolean_testable_impl<B> && requires(B&& b) {
  { !std::forward<B>(b) } -> boolean_testable_impl;
};

template <typename T>
concept index = std::movable<T>;

template <typename T>
concept regular_index = index<T> && std::regular<T>;

template <class Range>
concept sequence = std::ranges::range<Range>;

template <class Range>
concept input_sequence = sequence<Range> && std::ranges::input_range<Range>;

template <class Range, typename T>
concept output_sequence =
    sequence<Range> && std::ranges::output_range<Range, T>;

template <class Range>
concept forward_sequence =
    input_sequence<Range> && std::ranges::forward_range<Range>;

template <class Range>
concept bidirectional_sequence =
    forward_sequence<Range> && std::ranges::bidirectional_range<Range>;

template <class Range>
concept random_access_sequence =
    bidirectional_sequence<Range> && std::ranges::random_access_range<Range>;

template <class Range>
concept sized_sequence = sequence<Range> && std::ranges::sized_range<Range>;

template <class Range>
concept contiguous_sequence =
    random_access_sequence<Range> && sized_sequence<Range> &&
    std::ranges::contiguous_range<Range> &&
    requires(Range& r, std::size_t idx) {
      { r[idx] } -> std::same_as<std::ranges::range_reference_t<Range>>;
    };

// template <class Range>
// concept constant_sequence =
//     input_sequence<Range> && std::ranges::constant_range<Range>;
// Usually sequences have legacy iterators that we can use...

template <typename Loop>
using index_t = decltype(loops::iter(std::declval<Loop>()));

template <typename Loop>
using reference_t =
    decltype(loops::deref(std::declval<Loop>(), std::declval<index_t<Loop>>()));

template <typename Loop>
concept loop = requires(Loop& l) {
  { loops::iter(l) } -> index;
} && requires(Loop& l, loops::index_t<Loop> const& i) {
  { loops::valid(l, i) } -> boolean_testable;
  { loops::next(l, i) } -> index;
  { loops::deref(l, i) };
};

template <typename Loop>
concept sized_loop = loop<Loop> && requires(Loop& l) {
  { loops::size(l) } -> std::integral;
};

template <typename Loop>
using loop_size_t = decltype(loops::size(std::declval<Loop>()));

// template <class Loop>
// concept has_sequence_category = requires {
//   typename Loop::sequence_category;
// } && std::derived_from<typename Loop::sequence_category, sequence_tag>;

}  // namespace loops
}  // namespace elixir

#endif
