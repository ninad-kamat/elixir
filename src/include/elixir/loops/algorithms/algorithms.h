#ifndef ELIXIR_LOOPS_ALGORITHMS
#define ELIXIR_LOOPS_ALGORITHMS

#include "elixir/base/config.h"
#include "elixir/loops/common/functors.h"
#include "elixir/loops/detail/concepts.h"
#include "elixir/loops/detail/operations.h"

namespace elixir {
namespace loops {

//////////////////////////////////////////////////////////////////////
// BASE OPERATIONS
//////////////////////////////////////////////////////////////////////

/**
 * @brief Apply a given callable on each element of an Elixir.Loop
 *
 * @tparam Loop An elixir loop
 * @tparam UnaryFunction Callable to apply of each element of loop
 * @param l An instance of sequence
 * @param f An instance of callable
 * @return Return the instance of callable passed in
 */
template <typename Loop, typename UnaryFunction>
  requires loop<Loop>
UnaryFunction for_each(Loop&& l, UnaryFunction&& f) {
  for (auto i = ::elixir::loops::iter(std::forward<Loop>(l));
       ::elixir::loops::valid(std::forward<Loop>(l), i);
       i = ::elixir::loops::next(std::forward<Loop>(l), i)) {
    std::forward<UnaryFunction>(f)(
        ::elixir::loops::deref(std::forward<Loop>(l), i));
  }
  return std::move(f);
}

/**
 * @brief Apply a given callable and pass in the index count of iteration on an
 * Elixir.Loop
 *
 * @tparam Loop An Elixir.Loop type
 * @tparam Callable Callable that takes first argument as the index of iteration
 * and the second argument as item in the Loop sequence
 * @param l An instance of sequence
 * @param c An instance of callable
 * @return Return same callable that was passed in
 */
template <typename Loop, typename Callable>
  requires loop<Loop>
Callable enumerate(Loop&& l, Callable&& c) {
  std::size_t idx = 0;
  for (auto i = ::elixir::loops::iter(std::forward<Loop>(l));
       ::elixir::loops::valid(std::forward<Loop>(l), i);
       i = ::elixir::loops::next(std::forward<Loop>(l), i)) {
    std::forward<Callable>(c)(idx++,
                              ::elixir::loops::deref(std::forward<Loop>(l), i));
  }
  return std::move(c);
}

//////////////////////////////////////////////////////////////////////
// NON-MODIFYING SEQUENCE OPERATIONS
//////////////////////////////////////////////////////////////////////

/**
 * @brief Find the first instance of a given value in the loop's sequence.
 *
 * @tparam Loop An Elixir.Loop type
 * @tparam T Value type for search
 * @param l An instance of the sequence
 * @param value Value to search
 * @return If value is found, then return index of found object. If search
 * fails, then return index equivalent to the loop's end index.
 */
template <typename Loop, typename T>
  requires loop<Loop>
constexpr auto find(Loop&& l, T&& value) {
  using loop_type = typename std::decay<Loop>::type;
  return ::elixir::loops::loop_until(std::forward<Loop>(l),
                                     [&](auto&& ref) { return ref != value; });
}

/**
 * @brief Find the first item in loop's sequence that satisfies a given
 * predicate
 *
 * @tparam Loop An Elixir.Loop type
 * @tparam Predicate A functor type that takes in an argument of Loops's
 * value_type and returns a bool.
 * @param l An instance of the sequence
 * @param p An instance of predicate
 * @return If predicate is satisfied, then return index of found object. If
 * search fails, then return index equivalent to the loop's end index.
 */
template <typename Loop, typename Predicate>
  requires loop<Loop>
constexpr auto find_if(Loop&& l, Predicate&& p) {
  using loop_type = typename std::decay<Loop>::type;
  return ::elixir::loops::loop_until(
      std::forward<Loop>(l), ::elixir::not_fn(std::forward<Predicate>(p)));
}

/**
 * @brief Find the first item in the loop's sequence that does not satisfy the
 * given predicate
 *
 * @tparam Loop An Elixir.Loop type
 * @tparam Predicate A functor type that takes in an argument of Loops's
 * value_type and returns a bool.
 * @param l An instance of the sequence
 * @param p An instance of Predicate
 * @return If the predicate returns false for an item, then return the index of
 * that item, else return the index equivalent to loop's end
 */
template <typename Loop, typename Predicate>
  requires loop<Loop>
constexpr auto find_if_not(Loop&& l, Predicate&& p) {
  using loop_type = typename std::decay<Loop>::type;
  return ::elixir::loops::loop_until(std::forward<Loop>(l),
                                     std::forward<Predicate>(p));
}

/**
 * @brief Check if all items in the sequence satisfy a given predicate
 *
 * @tparam Loop An Elixir.Loop type
 * @tparam Predicate A functor type to check against that takes in the loop's
 * value_type as an argument and returns true/false
 * @param l An instance of the sequence
 * @param p An instance of the predicate
 * @return true if all items in the sequence satisfy the predicate
 */
template <typename Loop, typename Predicate>
  requires loop<Loop>
bool all_of(Loop&& l, Predicate&& p) {}

/**
 * @brief Check if any items in the sequence satisfy a given predicate
 *
 * @tparam Loop An Elixir.Loop type
 * @tparam Predicate A functor type to check against that takes in the loop's
 * value_type as an argument and returns true/false
 * @param l An instance of the sequence
 * @param p An instance of the predicate
 * @return true if any item in the sequence satisfy the predicate
 */
template <typename Loop, typename Predicate>
  requires loop<Loop>
bool any_of(Loop&& l, Predicate&& p) {}

/**
 * @brief Check if none of the items in the sequence satisfy a given predicate
 *
 * @tparam Loop An Elixir.Loop type
 * @tparam Predicate A functor type to check against that takes in the loop's
 * value_type as an argument and returns true/false
 * @param l An instance of the sequence
 * @param p An instance of the predicate
 * @return true if none of the items in the sequence satisfy the predicate
 */
template <typename Loop, typename Predicate>
  requires loop<Loop>
bool none_of(Loop&& l, Predicate&& p) {}

/**
 * @brief Count items in the sequence that satisfy a given predicate
 *
 * @tparam Loop An Elixir.Loop type
 * @tparam Predicate A functor type that takes in the loop's value_type as an
 * argument and returns true/false
 * @param l An instance of the sequence
 * @param p An instance of the predicate
 * @return Number of items in the sequence that satisfy the predicate
 */
template <typename Loop, typename Predicate>
  requires loop<Loop>
std::size_t count_if(Loop&& l, Predicate&& p) {
  std::size_t c = 0;
  ::elixir::loops::for_each(std::forward<Loop>(l), [&](auto&& val) {
    if (std::forward<Predicate>(p)(val)) {
      c++;
    }
  });
  return c;
}

/**
 * @brief Count items in a sequence
 *
 * @tparam Loop An Elixir.Loop type
 * @param l An instance of the sequence
 * @return Number of items in the sequence
 */
template <typename Loop>
  requires loop<Loop>
std::size_t count(Loop&& l) {
  return count_if(std::forward<Loop>(l), ::elixir::always_true);
}

//////////////////////////////////////////////////////////////////////
// MODIFYING SEQUENCE OPERATIONS
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// PARTITIONING OPERATIONS
//////////////////////////////////////////////////////////////////////

template <typename Loop, typename Predicate>
  requires loop<Loop>
constexpr auto partition(Loop& l, Predicate&& p) {
  auto i = elixir::loops::iter(l);
  auto j = i;
  while (elixir::loops::valid(l, i)) {
    auto&& current = elixir::loops::deref(l, i);
    std::swap(elixir::loops::deref(l, j), current);
    if (std::forward<Predicate>(p)()) {
      j = elixir::loops::next(l, j);
    }
    i = elixir::loops::next(l, i);
  }
  return p;
}

//////////////////////////////////////////////////////////////////////
// SORTING OPERATIONS
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// SORTED RANGE - BINARY SEARCH OPERATIONS
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// SORTED RANGE - OTHER OPERATIONS
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// SORTED RANGE - SET OPERATIONS
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// HEAP OPERATIONS
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// MIN / MAX OPERATIONS
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// COMPARISON OPERATIONS
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// PERMUTATION OPERATIONS
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// NUMERIC OPERATIONS
//////////////////////////////////////////////////////////////////////

}  // namespace loops
}  // namespace elixir

#endif
