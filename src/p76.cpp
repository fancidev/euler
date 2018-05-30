/**
 * It is possible to write five as a sum in exactly six different ways:
 *
 *   4 + 1
 *   3 + 2
 *   3 + 1 + 1
 *   2 + 2 + 1
 *   2 + 1 + 1 + 1
 *   1 + 1 + 1 + 1 + 1
 *
 * How many different ways can one hundred be written as a sum of at least
 * two positive integers?
 */

#include <iostream>
#include "euler/partition.hpp"
#include "euler.h"

BEGIN_PROBLEM(76, solve_problem_76)
  PROBLEM_TITLE("Counting summations")
  PROBLEM_ANSWER("190569291")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("N^2")
  PROBLEM_SPACE_COMPLEXITY("N")
  PROBLEM_KEYWORDS("combinatorics,partition")
END_PROBLEM()

#if 1
static void solve_problem_76()
{
  euler::partition_function<int> p;
  std::cout << p(100) - 1 << std::endl;
}
#else
// Below is an implementation based on dynamic programming. It has higher
// complexity than the proper solution, and is not suitable for large n.

// f(n, m) = number of ways to decompose n using integers no larger than m
static int f(int n, int m)
{
  if (m == 1)
  {
    return 1;
  }

  int count = 0;
  for (int s = 0; s <= n; s += m)
  {
    count += f(n - s, m - 1);
  }
  return count;
}

static void solve_problem_76()
{
  std::cout << f(100, 99) << std::endl;
}
#endif
