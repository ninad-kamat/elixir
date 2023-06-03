#ifndef ELIXIR_LOOPS_COMPATIBILITY_STD_VECTOR
#define ELIXIR_LOOPS_COMPATIBILITY_STD_VECTOR

#include "elixir/loops/detail/traits.h"

namespace elixir {
namespace loops {

template <typename T, typename Alloc>
struct is_contiguous_sequence<std::vector<T, Alloc>> : public std::true_type {};

template <typename T, typename Alloc>
struct is_contiguous_sequence<const std::vector<T, Alloc>>
    : public std::true_type {};

template <typename T, typename Alloc>
struct range_traits<std::vector<T, Alloc>, void> {
  using base_type = std::vector<T, Alloc>;
  using index_type = std::size_t;
  using value_type = typename base_type::value_type;
  using reference = typename base_type::reference;
  using pointer = typename base_type::pointer;
  using difference_type = std::ptrdiff_t;
  using size_type = typename base_type::size_type;

  static size_type size(std::vector<T, Alloc>& vec) noexcept {
    return vec.size();
  }
  static bool valid(std::vector<T, Alloc>& vec, index_type i) noexcept {
    return i < vec.size();
  }
  static reference deref(std::vector<T, Alloc>& vec, index_type idx) {
    return vec[idx];
  }
  static index_type next(index_type i, difference_type offset = 1) noexcept {
    return i + offset;
  }
  static index_type iter(std::vector<T, Alloc>& vec) { return 0; }
};

template <typename T, typename Alloc>
struct range_traits<std::vector<T, Alloc> const, void> {
  using base_type = std::vector<T, Alloc>;
  using index_type = std::size_t;
  using value_type = typename base_type::value_type;
  using reference = typename base_type::const_reference;
  using pointer = typename base_type::const_pointer;
  using difference_type = std::ptrdiff_t;
  using size_type = typename base_type::size_type;

  static size_type size(std::vector<T, Alloc> const& vec) noexcept {
    return vec.size();
  }
  static bool valid(std::vector<T, Alloc> const& vec, index_type i) noexcept {
    return i < vec.size();
  }
  static reference deref(std::vector<T, Alloc> const& vec, index_type idx) {
    return vec[idx];
  }
  static index_type next(index_type i, difference_type offset = 1) noexcept {
    return i + offset;
  }
  static index_type iter(std::vector<T, Alloc> const& vec) { return 0; }
};

}  // namespace loops
}  // namespace elixir
#endif
