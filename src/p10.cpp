/**
 * The sum of the primes below 10 is 2 + 3 + 5 + 7 = 17.
 *
 * Find the sum of all the primes below two million.
 */

#include <cstdint>
#include <iostream>
#include <numeric>
#include "euler/prime_table.hpp"
#include "euler.h"

BEGIN_PROBLEM(10, solve_problem_10)
  PROBLEM_TITLE("Calculate the sum of all the primes below 2,000,000")
  PROBLEM_ANSWER("142913828922")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("N*ln(ln(N))")
  PROBLEM_SPACE_COMPLEXITY("N")
END_PROBLEM()

static void solve_problem_10()
{
#if 0
  const int n = 10;
#else
  const int n = 2000000;
#endif
  euler::prime_table<int> primes(n);
  int64_t sum = std::accumulate(primes.begin(), primes.end(), 0LL);
  std::cout << sum << std::endl;
}
