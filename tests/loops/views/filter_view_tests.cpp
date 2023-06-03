#include "elixir/loops/loops.h"

#include "gtest/gtest.h"

using namespace elixir;

namespace elixir {
namespace tests {

TEST(view_tests, filter_view_can_filter_loops) {
  int test[5] = {10, 11, 12, 13, 14};
  int expected[1] = {12};
  loops::enumerate(
      loops::filter(loops::just(test), [](auto num) { return num % 3 == 0; }),
      [&](auto idx, auto value) { ASSERT_EQ(expected[idx], value); });
}

TEST(view_tests, filter_can_pipe_loops) {
  int test[5] = {10, 11, 12, 13, 14};
  int expected[1] = {12};
  loops::enumerate(
      test | loops::just | loops::filter([](auto num) { return num % 3 == 0; }),
      [&](auto idx, auto value) { ASSERT_EQ(expected[idx], value); });
}

}  // namespace tests
}  // namespace elixir