/**
 * In the following equation x, y, and n are positive integers.
 *
 *   1/x + 1/y = 1/n
 *
 * For n = 4 there are exactly three distinct solutions:
 *
 *   1/5 + 1/20 = 1/4
 *   1/6 + 1/12 = 1/4
 *   1/8 + 1/8 = 1/4
 *
 * What is the least value of n for which the number of distinct solutions
 * exceeds one-thousand?
 *
 * NOTE: This problem is an easier version of Problem 110.
 *
 * SOLUTION:
 *
 * We observe the following relation:
 *
 *   (x-n)*(y-n) = n^2
 *
 * Hence the solutions (x,y) map one-to-one to the divisors of n^2.
 *
 * Let the prime factors of n be (p1,p2,...,pm), then (x,y) is a partition of
 *
 *   (p1^2k1)*(p2^2k2)*...*(pm^2km) = n^2.
 *
 * The detailed solution is presented for problem 110.
 */

#include <iostream>
#include "euler/prime_factor.hpp"
#include "euler.h"

BEGIN_PROBLEM(108, solve_problem_108)
  PROBLEM_TITLE("Diophantine reciprocals I")
  PROBLEM_ANSWER("180180")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
END_PROBLEM()

// Counts number of distinct solutions 1/x + 1/y = 1/n.
static size_t count_solutions(int n)
{
  size_t count = 1;
  for (const auto &pk: euler::distinct(euler::factorize(n)))
  {
    const size_t k = pk.second;
    count *= (2 * k + 1);
  }
  return (count + 1) / 2;
}

static void solve_problem_108()
{
  for (int n = 2; ; n++)
  {
    const size_t count = count_solutions(n);
    if (count >= 1000)
    {
      std::cout << n << std::endl;
      break;
    }
  }
}
