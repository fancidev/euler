/**
 * A set of disks numbered 1 through 100 are placed in a line in random order.
 *
 * What is the probability that we have a partial derangement such that
 * exactly 22 prime number discs are found away from their natural positions?
 *
 * SOLUTION:
 *
 * Let p(n, m) be the probability of placing n balls numbered 1 to n into n
 * slots numbered 1 to n such that none of the balls numbered 1 to m are
 * placed in a slot with the same number. Balls numbered (m + 1) to n may be
 * placed freely.
 *
 * We find a recurrence relation for p(n, m). For m = 0, no restrictions
 * apply, and therefore
 *
 *   p(n, 0) = 1
 *
 * For 1 <= m <= n, we may place ball #1 either in one of the slots in 2 to m,
 * or in one of the slots in (m + 1) to n. Therefore
 *
 *              m-1                 n-m
 *   p(n, m) = ----- p(n-1, m-2) + ----- p(n-1, m-1)
 *               n                   n
 *
 * For the boundary cases m = 1 and m = n, the coefficient of one of the terms
 * becomes zero, and that term vanishes.
 *
 * Back to the problem. From 1 to 100 there are 25 primes. To arrange exactly
 * 3 primes in position and 22 primes out of position, we first pick and fix
 * 3 primes, and then arrange the rest items.
 *
 * Let C(n, m) be the number of ways to choose m items out of n items, and let
 * P(n, m) be the number of ways to permute m items out of n items. There are
 * C(25, 3) ways to pick the primes to fix, and for each triple picked there
 * are P(100, 3) ways to arrange them without restriction, out of which only
 * 1 way is desired. The remaining 97 items (22 primes + 75 composites) are
 * arranged as described above. Therefore the overall probability is
 *
 *    C(25, 3)                23
 *   ---------- p(97, 22) = ------ p(97, 22)
 *    P(100,3)               9702
 */

#include <iomanip>
#include <iostream>
#include "euler.h"

BEGIN_PROBLEM(239, solve_problem_239)
  PROBLEM_TITLE("Twenty-two Foolish Primes")
  PROBLEM_ANSWER("0.001887854841")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
  PROBLEM_KEYWORDS("combinatorics,enumeration")
END_PROBLEM()

static void solve_problem_239()
{
  const int N = 97, M = 22;
  double p[N+1][M+1];
  for (int n = 1; n <= N; n++)
  {
    p[n][0] = 1.0;
    for (int m = 1; m <= M && m <= n; m++)
    {
      p[n][m] = ((m > 1)? p[n-1][m-2]*(m-1)/n : 0.0)
              + ((m < n)? p[n-1][m-1]*(n-m)/n : 0.0);
    }
  }
  std::cout << std::fixed << std::setprecision(12)
            << p[N][M]*23/9702 << std::endl;
}
