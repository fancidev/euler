/**
 * A 2-by-3 rectangular grid of unit squares contains 18 rectangles.
 *
 * Although there exists no rectangular grid that contains exactly 2,000,000
 * rectangles, find the area of the grid with the nearest solution.
 *
 * SOLUTION:
 *
 * For an m-by-n rectangular grid, there are (m + m-1 + ... + 1) ways to
 * choose the height of a rectangle, and (n + n-1 + ... + 1) ways to choose
 * the width of a rectangle. Therefore the total number of rectangles is
 *
 *             m*(m+1)     n*(n+1)
 *   f(m,n) = --------- * ---------
 *                2           2
 *
 * Without loss of generality, we require m <= n. To find m and n such that
 * the value of f(m,n) is closest to A = 2,000,000, enumerate m starting from
 * 1. For each m, solve the equation
 *
 *    n*(n+1)           2*A
 *   --------- = B = ---------
 *       2            m*(m+1)
 *
 * for which the solution is
 *
 *        √(1+8*B) - 1
 *   n ≈ --------------
 *             2
 *
 * Let n1 <= n <= n2 be two integers nearest to n. Compute f(m,n1) and f(m,n2)
 * and update the known best solution if they are closer to A. Continue until
 * n2 < m.
 */

#include <cmath>
#include <iostream>
#include "euler.h"

BEGIN_PROBLEM(85, solve_problem_85)
  PROBLEM_TITLE("Counting rectangles")
  PROBLEM_ANSWER("2772")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
  PROBLEM_KEYWORDS("combinatorics")
END_PROBLEM()

static int f(int m, int n)
{
  return (m * (m + 1) / 2) * (n * (n + 1) / 2);
}

static void solve_problem_85()
{
  const int A = 2000000;

  int nearest_distance = A;
  int nearest_area = 0;

  for (int m = 1; ; m++)
  {
    const double B = 2.0 * A / (m * (m + 1));
    const double n = (sqrt(1 + 8 * B) - 1) / 2;
    const int n1 = static_cast<int>(n);
    const int n2 = n1 + 1;
    const int f1 = f(m, n1);
    if (abs(f1 - A) < nearest_distance)
    {
      nearest_distance = abs(f1 - A);
      nearest_area = m * n1;
    }
    const int f2 = f(m, n2);
    if (abs(f2 - A) < nearest_distance)
    {
      nearest_distance = abs(f2 - A);
      nearest_area = m * n2;
    }
    if (n2 < m)
    {
      break;
    }
  }
  std::cout << nearest_area << std::endl;
}
