/**
 * k defects are randomly distributed amongst n integrated-circuit chips
 * produced by a factory (any number of defects may be found on a chip and
 * each defect is independent of the other defects).
 *
 * Let p(k, n) represent the probability that there is a chip with at least 3
 * defects. For instance p(3, 7) ≈ 0.0204081633.
 *
 * Find p(20 000, 1 000 000) rounded to 10 decimal places.
 *
 * SOLUTION:
 *
 * It is helpful to illustrate the defect assignment procedure. With 3 defects
 * and 7 chips, an assignment may be the following:
 *
 *   +---+---+---+---+---+---+---+
 *   |   | X |   |   |   |   |   |
 *   +---+---+---+---+---+---+---+
 *   |   |   |   |   |   | X |   |
 *   +---+---+---+---+---+---+---+
 *   |   |   | X |   |   |   |   |
 *   +---+---+---+---+---+---+---+
 *
 * Let f(k, n; r) represent the number of ways to assign k defects to n chips
 * such that exactly r chips have 2 defects each and the rest chips have at
 * most 1 defect each. We compute f(k, n; r).
 *
 * There are C(n, r) ways to pick r chips out of n each of which bear 2
 * defects. For each combination of r chips, there are
 *
 *                                           r-1
 *   C(k,2) * C(k-2,2) * ... * C(k-2r+2,2) =  Π  C(k-2*s,2)
 *                                           s=0
 *
 * ways to assign 2r defects to them. This leaves us with (k - 2r) defects to
 * assign to the remaining (n - r) chips, for which there are P(n - r, k - 2r)
 * ways. Putting these together, we have
 *
 *                       / r-1            \
 *   f(k,n;r) = C(n,r) * |  Π  C(k-2*s,2) | * P(n-r,k-2r)
 *                       \ s=0            /
 *
 *                    n!          r-1
 *            = --------------- *  Π  C(k-2*s,2)
 *               r! * (n-k+r)!    s=0
 *
 * Let f(k, n) denote the number of ways to assign k defects to n chips such
 * that no chip has more than 2 defects. Then
 *
 *            [k/2]
 *   f(k,n) =   Σ   f(k,n;r)
 *             r=0
 *
 * Let q(k, n) denote the probability of assigning k defects to n chips such
 * that no chip has more than 2 defects. Then
 *
 *             f(k,n)    [k/2]  f(k,n;r)    [k/2]
 *   q(k,n) = -------- =   Σ   ---------- =   Σ   q(k,n;r)
 *              n^k       r=0     n^k        r=0
 *
 * where
 *                f(k,n;r)      n!           1          r-1
 *   q(k,n;r) := ---------- = ----- * --------------- *  Π  C(k-2*s,2)
 *                  n^k        n^k     r! * (n-k+r)!    s=0
 *
 * For computation, it is convenient to write q(k,n;r) recursively as
 *
 *                    n!         k-1  n-j
 *   q(k,n;0) = -------------- =  Π  -----
 *               n^k * (n-k)!    j=0   n
 *
 *                            C(k-2*r+2,2)
 *   q(k,n;r) = q(k,n;r-1) * --------------
 *                            r * (n-k+r)
 *
 *                            (k-2*r+2) * (k-2*r+1)
 *            = q(k,n;r-1) * -----------------------
 *                               2 * r * (n-k+r)
 *
 * Finally, p(k, n) = 1 - q(k, n).
 */

#include <cmath>
#include <iomanip>
#include <iostream>
#include "euler.h"

BEGIN_PROBLEM(307, solve_problem_307)
  PROBLEM_TITLE("Chip Defects")
  PROBLEM_ANSWER("0.7311720251")
  PROBLEM_DIFFICULTY(2)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
  PROBLEM_KEYWORDS("combinatorics,enumeration")
END_PROBLEM()

static void solve_problem_307()
{
#if 0
  const int N = 7, K = 3;
#else
  const int N = 1000000, K = 20000;
#endif

  // Compute q(k,n; 0)
  double q = 1;
  for (int j = 0; j < K; j++)
  {
    q *= static_cast<double>(N - j) / N;
  }
  if (verbose())
  {
    std::cout << "q(" << K << "," << N << ";0) = " << std::scientific
              << std::setprecision(10) << q << std::endl;
  }

  // Compute q(k,n; r) recursively and sum up
  double sum_q = q;
  for (int r = 1; r <= K/2; r++)
  {
    double c = (static_cast<double>(K)-2*r+2) *
               (static_cast<double>(K)-2*r+1) /
               (2.0 * static_cast<double>(r) *
                static_cast<double>(N-K+r));
    q *= c;
    sum_q += q;
    if (verbose())
    {
      std::cout << "q(" << K << "," << N << ";" << r << ") = "
                << std::scientific << std::setprecision(10) << q << std::endl;
    }
    if (q == 0.0)
    {
      break;
    }
  }

  std::cout << std::fixed << std::setprecision(10) << (1-sum_q) << std::endl;
}
