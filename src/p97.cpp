/**
 * The first known prime found to exceed one million digits was discovered in
 * 1999, and is a Mersenne prime of the form 2^6972593 - 1; it contains
 * exactly 2,098,960 digits. Subsequently other Mersenne primes, of the form
 * 2^p - 1, have been found which contain more digits.
 *
 * However, in 2004 there was found a massive non-Mersenne prime which
 * contains 2,357,207 digits: 28433*2^7830457+1.
 *
 * Find the last ten digits of this prime number.
 */

#include <cstdint>
#include <iostream>
#include "euler/residue.hpp"
#include "euler.h"

BEGIN_PROBLEM(97, solve_problem_97)
  PROBLEM_TITLE("Find the last ten digits of a non-Mersenne prime")
  PROBLEM_ANSWER("8739992577")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("log(d)")
  PROBLEM_SPACE_COMPLEXITY("1")
END_PROBLEM()

static void solve_problem_97()
{
  const int64_t M = 10000000000LL;
  euler::residue<int64_t, M> a = 2;
  std::cout << 28433 * (a ^ 7830457) + 1 << std::endl;
}
