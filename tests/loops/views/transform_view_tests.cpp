#include "elixir/loops/loops.h"

#include "gtest/gtest.h"

using namespace elixir;

namespace elixir {
namespace tests {

TEST(view_tests, transform_view_can_map_loops) {
  int test[5] = {10, 11, 12, 13, 14};
  int expected[5] = {100, 121, 144, 169, 196};

  loops::enumerate(
      loops::transform(loops::just(test),
                       [](auto value) { return value * value; }),
      [&](auto idx, auto value) { ASSERT_EQ(expected[idx], value); });
}

TEST(view_tests, transform_can_pipe_loops) {
  int test[5] = {10, 11, 12, 13, 14};
  int expected[5] = {100, 121, 144, 169, 196};

  loops::enumerate(
      test | loops::just |
          loops::transform([](auto value) { return value * value; }),
      [&](auto idx, auto value) { ASSERT_EQ(expected[idx], value); });
}

}  // namespace tests
}  // namespace elixir