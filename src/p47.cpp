/**
 * The first two consecutive numbers to have two distinct prime factors are:
 *
 *   14 = 2 * 7
 *   15 = 3 * 5
 *
 * The first three consecutive numbers to have three distinct prime factors
 * are:
 *
 *   644 = 2^2 * 7 * 23
 *   645 = 3 * 5 * 43
 *   646 = 2 * 17 * 19
 *
 * Find the first four consecutive integers to have four distinct prime
 * factors. What is the first of these numbers?
 *
 * ANSWER: 134043.
 */

#include <iostream>
#include <algorithm>
#include "euler/prime_factor.hpp"
#include "euler.h"

BEGIN_PROBLEM(47, solve_problem_47)
  PROBLEM_TITLE("Distinct primes factors")
  PROBLEM_ANSWER("134043")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("?")
  PROBLEM_SPACE_COMPLEXITY("?")
END_PROBLEM()

static void solve_problem_47()
{
  const int find_consecutive = 4;

  int n = 3;
  int num_consecutive = 0;
  for (; num_consecutive < find_consecutive; ++n)
  {
    auto distinct_factors = euler::distinct(euler::factorize(n));
    auto num_distinct_factors = std::distance(
        distinct_factors.begin(), distinct_factors.end());
    if (num_distinct_factors == find_consecutive)
    {
      ++num_consecutive;
    }
    else
    {
      num_consecutive = 0;
    }
  }
  std::cout << (n - find_consecutive) << std::endl;
}
