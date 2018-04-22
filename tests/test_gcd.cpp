#include <cstdint>
#include "euler/gcd.hpp"
#include "gtest/gtest.h"

TEST(gcd, gcd)
{
  // Common usage
  EXPECT_EQ(22, euler::gcd(88, 66));
  EXPECT_EQ(22, euler::gcd(66, 88));
  EXPECT_EQ(10000, euler::gcd(10000, 10000));
  EXPECT_EQ(1, euler::gcd(123, 1));
  EXPECT_EQ(456, euler::gcd(0, 456));

  // Both positve, coprime
  EXPECT_EQ(1, euler::gcd<int32_t>(17, 23));
  EXPECT_EQ(1, euler::gcd<uint8_t>(20, 33));
  EXPECT_EQ(1, euler::gcd<int32_t>(0x7FFFFFFF, 65536));
  EXPECT_EQ(1, euler::gcd<uint64_t>(0xFFFFFFFFFFFFFFFFULL, 14593));

  // Both positive, not coprime
  EXPECT_EQ(30, euler::gcd<int8_t>(120, 90));
  EXPECT_EQ(1024, euler::gcd<uint16_t>(32768, 3072));

  // Both positive, one is multiple of other
  EXPECT_EQ(1, euler::gcd(1, 1));
  EXPECT_EQ(3, euler::gcd(3, 15));
  EXPECT_EQ(10, euler::gcd(20, 10));

  // At least one is zero
  EXPECT_EQ(0, euler::gcd(0, 0));
  EXPECT_EQ(10, euler::gcd(10, 0));
  EXPECT_EQ(255, euler::gcd<uint8_t>(0, 255));

  // Exception test: negative input
  EXPECT_THROW(euler::gcd(-1, -5), std::invalid_argument);
  EXPECT_THROW(euler::gcd(-123, 0), std::invalid_argument);
  EXPECT_THROW(euler::gcd(0, -456), std::invalid_argument);
}

TEST(gcd, egcd)
{
  // Common usage
  EXPECT_EQ(std::make_pair(22, std::make_pair(1, 1)), euler::egcd(88, 66));
  EXPECT_EQ(std::make_pair(22, std::make_pair(3, 2)), euler::egcd(66, 88));

  // Should not overflow
  EXPECT_EQ(
      std::make_pair(1, std::make_pair(347879503, 605123096)),
      euler::egcd(0x7FFFFFFF, 1234567890));
}
