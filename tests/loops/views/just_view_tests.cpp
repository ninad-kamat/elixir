#include "elixir/loops/loops.h"
#include "elixir/loops/std.h"

#include "gtest/gtest.h"

namespace elixir {
namespace tests {

TEST(view_tests, just_view_can_iterate_c_arrays) {
  int test[5] = {10, 11, 12, 13, 14};
  loops::enumerate(loops::just(test),
                   [&](auto idx, auto value) { ASSERT_EQ(test[idx], value); });
}

TEST(view_tests, just_can_pipe_c_arrays) {
  int test[5] = {10, 11, 12, 13, 14};
  loops::enumerate(test | loops::just,
                   [&](auto idx, auto value) { ASSERT_EQ(test[idx], value); });
}

TEST(view_tests, just_view_can_iterate_const_c_arrays) {
  const int test[5] = {10, 11, 12, 13, 14};
  loops::enumerate(loops::just(test),
                   [&](auto idx, auto value) { ASSERT_EQ(test[idx], value); });
}

TEST(view_tests, just_can_pipe_const_c_arrays) {
  const int test[5] = {10, 11, 12, 13, 14};
  loops::enumerate(test | loops::just,
                   [&](auto idx, auto value) { ASSERT_EQ(test[idx], value); });
}

TEST(view_tests, just_view_can_iterate_std_vector) {
  std::vector<int> test = {10, 11, 12, 13, 14};
  loops::enumerate(loops::just(test),
                   [&](auto idx, auto value) { ASSERT_EQ(test[idx], value); });
}

TEST(view_tests, just_can_pipe_std_vector) {
  std::vector<int> test = {10, 11, 12, 13, 14};
  loops::enumerate(test | loops::just,
                   [&](auto idx, auto value) { ASSERT_EQ(test[idx], value); });
}

TEST(view_tests, just_view_can_iterate_const_std_vector) {
  const std::vector<int> test = {10, 11, 12, 13, 14};
  loops::enumerate(loops::just(test),
                   [&](auto idx, auto value) { ASSERT_EQ(test[idx], value); });
}

TEST(view_tests, just_can_pipe_const_std_vector) {
  const std::vector<int> test = {10, 11, 12, 13, 14};
  loops::enumerate(test | loops::just,
                   [&](auto idx, auto value) { ASSERT_EQ(test[idx], value); });
}

}  // namespace tests
}  // namespace elixir