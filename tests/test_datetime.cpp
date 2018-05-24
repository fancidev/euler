#include "euler/datetime.hpp"
#include "gtest/gtest.h"

TEST(datetime, datenum)
{
  EXPECT_EQ(1, euler::datenum(0, 1, 1));
  EXPECT_EQ(367, euler::datenum(1, 1, 1));
  EXPECT_EQ(729159, euler::datenum(1996, 5, 14));
  EXPECT_EQ(730990, euler::datenum(2001, 5, 19));
  EXPECT_EQ(731204, euler::datenum(2001, 12, 19));
  EXPECT_EQ(733301, euler::datenum(2007, 9, 16));
  EXPECT_EQ(734471, euler::datenum(2010, 11, 29));
  EXPECT_EQ(736858, euler::datenum(2017, 6, 12));
  EXPECT_EQ(3652425, euler::datenum(9999, 12, 31));
}
