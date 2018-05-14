/**
 * Euler's Totient function, phi(n), is used to determine the number of
 * positive numbers less than or equal to n which are relatively prime to n.
 *
 * For example, as 1, 2, 4, 5, 7, and 8, are all less than nine and relatively
 * prime to nine, phi(9) = 6.
 *
 * The number 1 is considered to be relatively prime to every positive number,
 * so phi(1) = 1.
 *
 * Interestingly, phi(87109) = 79180, and it can be seen that 87109 is a
 * permutation of 79180.
 *
 * Find the value of n, 1 <= n <= 10^7, for which phi(n) is a permutation of n
 * and the ratio n/phi(n) produces a minimum.
 *
 * ANSWER: 8319823.
 */

#include <iostream>
#include "euler/digits.hpp"
#include "euler/totient.hpp"
#include "euler.h"

BEGIN_PROBLEM(70, solve_problem_70)
  PROBLEM_TITLE("Totient permutation")
  PROBLEM_ANSWER("8319823")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
  PROBLEM_KEYWORDS("totient")
END_PROBLEM()

static void solve_problem_70()
{
  const int N = 10000000;
  std::vector<int> phi(N+1);
  euler::totient_table(phi);

  double min_r = 1000;
  int min_n = 0;
  for (int n = 2; n < N; n++)
  {
    if (euler::sort_digits(n) == euler::sort_digits(phi[n]))
    {
      const double r = static_cast<double>(n) / static_cast<double>(phi[n]);
      if (r < min_r)
      {
        min_r = r;
        min_n = n;
      }
    }
  }
  std::cout << min_n << std::endl;
}
