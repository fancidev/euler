#include <cstdint>
#include <iostream>
#include "euler/prime_factor.hpp"
#include "euler.h"

BEGIN_PROBLEM(3, solve_problem_3)
  PROBLEM_TITLE("Find the largest prime factor of 600851475143")
  PROBLEM_ANSWER("6857")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("sqrt(N)")
  PROBLEM_SPACE_COMPLEXITY("1")
END_PROBLEM()

static int64_t get_largest_prime_factor(int64_t n)
{
  int64_t ret = 1;
  for (int64_t p: euler::factorize(n))
  {
    ret = p;
  }
  return ret;
}

static void solve_problem_3()
{
  const int64_t N = 600851475143LL;
  std::cout << get_largest_prime_factor(N) << std::endl;
}
