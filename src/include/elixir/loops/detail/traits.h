#ifndef ELIXIR_LOOPS_TRAITS
#define ELIXIR_LOOPS_TRAITS

#include "elixir/base/config.h"

#include <type_traits>

namespace elixir {
namespace loops {

template <typename Loop>
struct is_loop : public std::false_type {};

template <typename Loop>
constexpr bool is_loop_v = is_loop<Loop>::value;

template <typename Loop>
struct loop_traits {
  static_assert(is_loop_v<Loop>,
                "Traits requested for type that is not an elixir loop");
  using index_type = typename Loop::index_type;
  using reference = typename Loop::reference;
  using value_type = typename Loop::value_type;
  using pointer = typename Loop::pointer;

  static constexpr bool reversible_v = Loop::reversible_v;
};

template <typename Loop>
constexpr bool is_loop_reversible_v = loop_traits<Loop>::reversible_v;

template <typename Sequence>
struct is_contiguous_sequence : public std::false_type {};

template <typename Sequence>
constexpr bool is_contiguous_sequence_v =
    is_contiguous_sequence<Sequence>::value;

template <typename T, std::size_t N>
struct is_contiguous_sequence<T[N]> : public std::true_type {};

template <typename T, std::size_t N>
struct is_contiguous_sequence<const T[N]> : public std::true_type {};

template <class Range, class Enable = void>
struct range_traits;

template <typename T, std::size_t N>
struct range_traits<T[N], void> {
  using index_type = std::size_t;
  using value_type = T;
  using reference = T&;
  using pointer = T*;
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;

  static size_type size(T (&x)[N]) noexcept { return N; }
  static bool valid(T (&x)[N], index_type i) noexcept { return i < N; }
  static reference deref(T (&x)[N], index_type idx) { return x[idx]; }
  static index_type next(index_type i, difference_type offset = 1) noexcept {
    return i + offset;
  }
  static index_type iter(T (&x)[N]) { return 0; }
};

template <typename T, std::size_t N>
struct range_traits<const T[N], void> {
  using index_type = std::size_t;
  using value_type = T;
  using reference = T const&;
  using pointer = T const*;
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;

  static size_type size(const T (&x)[N]) noexcept { return N; }
  static bool valid(const T (&x)[N], index_type i) noexcept { return i < N; }
  static reference deref(const T (&x)[N], index_type idx) { return x[idx]; }
  static index_type next(index_type i, difference_type offset = 1) noexcept {
    return i + offset;
  }
  static index_type iter(const T (&x)[N]) { return 0; }
};

}  // namespace loops
}  // namespace elixir
#endif
