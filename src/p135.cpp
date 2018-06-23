/**
 * Given the positive integers, x, y, and z, are consecutive terms of an
 * arithmetic progression, the least value of the positive integer, n, for
 * which the equation, x^2 - y^2 - z^2 = n, has exactly two solutions is
 * n = 27:
 *
 *   34^2 - 27^2 - 20^2 = 12^2 - 9^2 - 6^2 = 27
 *
 * It turns out that n = 1155 is the least value which has exactly ten
 * solutions.
 *
 * How many values of n less than one million have exactly ten distinct
 * solutions?
 *
 * SOLUTION:
 *
 * Let x = m+d, y = m, z = m-d where m >= 1 and 1-m <= d <= m-1. Substituting
 * these into the equation yields
 *
 *   (m+d)^2 - m^2 - (m-d)^2 = n
 *
 * Rearranging terms, we get
 *
 *   m * (4*d - m) = n
 *
 * For this equation to hold, we must have 1 <= m <= n.
 *
 * For this problem we enumerate solutions 1 <= n <= N for a given N. Setting
 * 0 < m * (4*d - m) <= N yields
 *
 *    m          1    /  N      \
 *   --- < d <= --- * | --- + m |
 *    4          4    \  m      /
 *
 * We use a sieve-style algorithm to count the total number of solutions.
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include "euler.h"

BEGIN_PROBLEM(135, solve_problem_135)
  PROBLEM_TITLE("Same differences")
  PROBLEM_ANSWER("4989")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
END_PROBLEM()

static void solve_problem_135()
{
  const int N = 999999;
  std::vector<size_t> count(N+1, 0);

  for (int m = 1; m <= N; m++)
  {
    const int min_d = m/4+1;
    const int max_d = std::min(m-1, (N/m+m)/4);
    for (int d = min_d; d <= max_d; d++)
    {
      const int n = m*(4*d-m);
      ++count[n];
    }
  }

  std::cout << std::count(count.begin(), count.end(), 10u) << std::endl;
}
