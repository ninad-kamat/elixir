#include "elixir/loops/loops.h"

#include "gtest/gtest.h"

namespace elixir {
namespace tests {

TEST(loops_algorithm_tests, find_find_correct_value) {
  int test[] = {10, 11, 12, 13, 14};
  auto i = loops::find(test | loops::just, 12);
  ASSERT_EQ(2, i);
}

TEST(loops_algorithm_tests, find_if_find_correct_value) {
  int test[] = {10, 11, 12, 13, 14};
  auto i = loops::find_if(test | loops::just,
                          [](auto value) { return value % 3 == 0; });
  ASSERT_EQ(2, i);
}

}  // namespace tests
}  // namespace elixir