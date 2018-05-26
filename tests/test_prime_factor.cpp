#include <algorithm>
#include "euler/prime_factor.hpp"
#include "gtest/gtest.h"

TEST(prime_factor, factorize)
{
  const int a = 12345678;
  const int ap[] = {2, 3, 3, 47, 14593};
  auto af = euler::factorize(a);
  EXPECT_TRUE(std::equal(std::begin(ap), std::end(ap), af.begin(), af.end()));

  const int b = 87654321;
  const int bp[] = {3, 3, 1997, 4877};
  auto bf = euler::factorize(b);
  EXPECT_TRUE(std::equal(std::begin(bp), std::end(bp), bf.begin(), bf.end()));

  // TODO: test distinct
}
