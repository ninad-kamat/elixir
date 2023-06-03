#include "elixir/loops/loops.h"

#include "gtest/gtest.h"

namespace elixir {
namespace tests {

TEST(view_tests, range_adaptor_can_process_loops) {
  int test[5] = {10, 11, 12, 13, 14};
  loops::enumerate(loops::range(loops::just(test)),
                   [&](auto idx, auto value) { ASSERT_EQ(test[idx], value); });
}

TEST(view_tests, range_adaptor_can_process_loops_pipe) {
  int test[5] = {10, 11, 12, 13, 14};
  loops::enumerate(test | loops::just | loops::range,
                   [&](auto idx, auto value) { ASSERT_EQ(test[idx], value); });
}

TEST(view_tests, range_adaptor_supports_for_loop) {
  int test[5] = {10, 11, 12, 13, 14};
  auto view = test | loops::just;
  int idx = 0;
  for (auto value : loops::range(view)) {
    ASSERT_EQ(test[idx], value);
    ++idx;
  }
}

TEST(view_tests, range_adaptor_supports_for_loop_with_pipe) {
  int test[5] = {10, 11, 12, 13, 14};
  int idx = 0;
  for (auto value : test | loops::just | loops::range) {
    ASSERT_EQ(test[idx], value);
    ++idx;
  }
}

}  // namespace tests
}  // namespace elixir