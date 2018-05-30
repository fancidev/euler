/**
 * Let p(n) represent the number of different ways in which n coins can be
 * separated into piles. For example, five coins can be separated into piles
 * in exactly seven different ways, so p(5) = 7.
 *
 *   5 = 5
 *   5 = 4 + 1
 *   5 = 3 + 2
 *   5 = 3 + 1 + 1
 *   5 = 2 + 2 + 1
 *   5 = 2 + 1 + 1 + 1
 *   5 = 1 + 1 + 1 + 1 + 1
 *
 * Find the least value of n for which p(n) is divisible by one million.
 */

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include "euler/partition.hpp"
#include "euler/residue.hpp"
#include "euler.h"

BEGIN_PROBLEM(78, solve_problem_78)
  PROBLEM_TITLE("Coin partitions")
  PROBLEM_ANSWER("55374")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("N^1.5")
  PROBLEM_SPACE_COMPLEXITY("N")
  PROBLEM_KEYWORDS("combinatorics,partition")
END_PROBLEM()

#if 1
static void solve_problem_78()
{
  using R = euler::residue<int, 1000000>;
  euler::partition_function<R> p;
  for (size_t n = 0; n <= 100000 /* arbitrary bound */; n++)
  {
    if (p(n) == 0)
    {
      std::cout << n << std::endl;
      break;
    }
  }
}
#else
// The following does not work because its complexity is O(n^3) and too high.
static void solve_problem_78()
{
  using T = euler::residue<int, 1000000>;

  std::vector<T> p;

  // p(0) = 0, p(1) = 1
  p.push_back(0);
  p.push_back(1);

  // For n >= m >= 0, p[n][m] = number of ways of writing n as sum of integers
  // no greater than m.
  std::vector<std::vector<T>> p;

  // p[0][0] = 1
  p.push_back(std::vector<T>(1, 1));

  for (int n = 1; n < 10000; n++)
  {
    p.push_back(std::vector<T>(n + 1));
    p[n][0] = 0;
    p[n][1] = 1;
    for (int m = 2; m <= n; m++)
    {
      T count = 0;
      for (int s = 0; s <= n; s += m)
      {
        count += p[n - s][std::min(n - s, m - 1)];
      }
      p[n][m] = count;
    }
    if (verbose())
    {
      std::cout << "p(" << n << ") == " << p[n][n] << std::endl;
    }
    if (p[n][n] == 0)
    {
      std::cout << n << std::endl;
      break;
    }
  }
}
#endif
