/**
 * The positive integers, x, y, and z, are consecutive terms of an arithmetic
 * progression. Given that n is a positive integer, the equation,
 * x^2 - y^2 - z^2 = n, has exactly one solution when n = 20:
 *
 *   13^2 - 10^2 - 7^2 = 20
 *
 * In fact there are twenty-five values of n below one hundred for which the
 * equation has a unique solution.
 *
 * How many values of n less than fifty million have exactly one solution?
 *
 * SOLUTION:
 *
 * Similar to Problem 135, substituting x = m + d, y = m, z = m - d into the
 * equation and rearranging terms yields
 *
 *   m * (4 * d - m) = n
 *
 * where 1 <= m <= n and
 *
 *    m                      1      N
 *   --- < d <= min( m - 1, --- * (--- + m) )
 *    4                      4      m
 *
 * We enumerate all (m, d) pairs that count unique n's.
 */

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>
#include "euler.h"

BEGIN_PROBLEM(136, solve_problem_136)
  PROBLEM_TITLE("Singleton difference")
  PROBLEM_ANSWER("2544559")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
END_PROBLEM()

static void solve_problem_136()
{
  const int N = 50000000 - 1;
  std::vector<int8_t> count(N + 1, 0);

  for (int m = 1; m <= N; m++)
  {
    const int min_d = m/4+1;
    const int max_d = std::min(m-1, (N/m+m)/4);
    for (int d = min_d; d <= max_d; d++)
    {
      const int n = m*(4*d-m);
      if (count[n] <= 1)
      {
        ++count[n];
      }
    }
  }

  std::cout << std::count(count.begin(), count.end(), 1) << std::endl;
}
