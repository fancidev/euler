#include <cstdint>
#include "euler/partition.hpp"
#include "gtest/gtest.h"

TEST(partition, function)
{
  euler::partition_function<int64_t> p;
  EXPECT_EQ(1, p(0));
  EXPECT_EQ(1, p(1));
  EXPECT_EQ(2, p(2));
  EXPECT_EQ(7, p(5));
  EXPECT_EQ(190569292, p(100));
}
