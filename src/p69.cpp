/**
 * Euler's Totient function, phi(n), is used to determine the number of
 * numbers less than n which are relatively prime to n. For example, as 1, 2,
 * 4, 5, 7, and 8, are all less than nine and relatively prime to nine,
 * phi(9) = 6.
 *
 * It can be seen that n = 6 produces a maximum n/phi(n) for n ≤ 10.
 *
 * Find the value of n ≤ 1,000,000 for which n/phi(n) is a maximum.
 *
 * Solution: see notes.
 */

#include <iostream>
#include "euler/prime_test.hpp"
#include "euler.h"

BEGIN_PROBLEM(69, solve_problem_69)
  PROBLEM_TITLE("Find the value of n <= 10^6 that maximizes n/phi(n)")
  PROBLEM_ANSWER("510510")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("ln(n)*ln(ln(n))")
  PROBLEM_SPACE_COMPLEXITY("1")
END_PROBLEM()

static void solve_problem_69()
{
  const int N = 1000000;

  int product = 1;
  for (int p = 2; product * p <= N; p = euler::next_prime(p))
  {
    product *= p;
  }
  std::cout << product << std::endl;
}
