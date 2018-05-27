#include <cstdint>
#include "euler/digits.hpp"
#include "gtest/gtest.h"

TEST(digits, count_digits)
{
  EXPECT_EQ(1u, euler::count_digits(0));
  EXPECT_EQ(1u, euler::count_digits(1));
  EXPECT_EQ(4u, euler::count_digits(1234));

  EXPECT_EQ(2u, euler::count_digits<10>(int8_t(99)));
  EXPECT_EQ(3u, euler::count_digits<10>(uint8_t(255)));
  EXPECT_EQ(6u, euler::count_digits<7>(int16_t(0x7FFF)));
  EXPECT_EQ(6u, euler::count_digits<7>(uint16_t(0xFFFF)));
  EXPECT_EQ(7u, euler::count_digits<16>(int32_t(0x6543210)));
  EXPECT_EQ(8u, euler::count_digits<16>(uint32_t(0xDEADBEEF)));
  EXPECT_EQ(37u, euler::count_digits<2>(int64_t(0x1020304050LL)));
  EXPECT_EQ(64u, euler::count_digits<2>(uint64_t(0xFEDCBA9876543210ULL)));

  EXPECT_EQ(1u, euler::count_digits<2>(0));
  EXPECT_EQ(3u, euler::count_digits<123>(123456));
}
