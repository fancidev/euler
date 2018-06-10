/**
 * In the following equation x, y, and n are positive integers.
 *
 *   1/x + 1/y = 1/n
 *
 * It can be verified that when n = 1260 there are 113 distinct solutions
 * and this is the least value of n for which the total number of distinct
 * solutions exceeds one hundred.
 *
 * What is the least value of n for which the number of distinct solutions
 * exceeds four million?
 */

#include <cmath>
#include <iostream>
#include <limits>
#include <vector>
#include "euler/prime_table.hpp"
#include "euler.h"

BEGIN_PROBLEM(110, solve_problem_110)
  PROBLEM_TITLE("Diophantine reciprocals II")
  PROBLEM_ANSWER("9350130049860600")
  PROBLEM_DIFFICULTY(2)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("(ln(N)ln(ln(N)))^2")
  PROBLEM_SPACE_COMPLEXITY("ln(N)")
  PROBLEM_KEYWORDS("divisor")
END_PROBLEM()

// j - search the first j primes k_j
// k - exponents must be >= k
// a - constraint >= a
// b - optimal solution < b
// p - smallest primes
// S - running sum of log(p[i])
// solution - exponents of the optimal solution
static double search(int j, int k, double a, double b,
  const std::vector<int> &p, const std::vector<double> &S,
  std::vector<int> &solution )
{
  double best = std::numeric_limits<double>::infinity();
  if (j-- == 0)
  {
    return (a <= 0)? 0 : best;
  }

  for (int k_j = (int)(b/S[j]); k_j >= k; k_j--)
  {
    double cost = k_j*log((double)p[j]) + search(j, k_j,
      a-log((double)(2*k_j+1)), b-k_j*log((double)p[j]), p, S, solution);
    if (cost < best)
    {
      b = best = cost;
      solution[j] = k_j;
    }
  }
  return best;
}

static void solve_problem_110()
{
#if 0
  const int A = 1001;
#else
  const int A = 4000001;
#endif

  int m = (int)ceil(log(2.0*A-1.0)/log(3.0));

  // Find the first m primes.
  euler::prime_table<int> primes(1000);
  std::vector<int> p(primes.begin(), primes.end());
  p.resize(m);

  // Compute the running sum of log(p[i]).
  std::vector<double> S(m);
  double s = 0;
  for (int i = 0; i < m; i++)
  {
    s += log((double)p[i]);
    S[i] = s;
  }

  // Now a feasible solution is the product of the first m primes.
  // We can use this as an upper bound to the optimal solution.
  std::vector<int> solution(m);
  search(m-1, 0, log(2.0*A-1.0), s, p, S, solution);
  long long n = 1;
  for (int i = 0; i < m; i++)
  {
    for (int k = solution[i]; k > 0; k--)
    {
      n *= p[i];
    }
  }
  std::cout << n << std::endl;
}
