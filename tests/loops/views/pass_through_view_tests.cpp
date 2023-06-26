#include "elixir/loops/loops.h"

#include "gtest/gtest.h"

namespace elixir {
namespace tests {

TEST(loops_view_tests, pass_through_can_process_loops) {
  int test[5] = {10, 11, 12, 13, 14};
  int count = 0;
  loops::enumerate(loops::pass_through(loops::just(test),
                                       [&](auto value) mutable {
                                         ASSERT_EQ(test[count++], value);
                                       }),
                   [&](auto idx, auto value) { ASSERT_EQ(test[idx], value); });
}

TEST(loops_view_tests, pass_through_can_be_piped) {
  int test[5] = {10, 11, 12, 13, 14};
  int count = 0;
  loops::enumerate(
      test | loops::just | loops::pass_through([&](auto value) mutable {
        ASSERT_EQ(test[count++], value);
      }),
      [&](auto idx, auto value) { ASSERT_EQ(test[idx], value); });
}

}  // namespace tests
}  // namespace elixir
