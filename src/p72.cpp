/**
 * Consider the fraction, p/q, where p and q are positive integers. If p < q
 * and gcd(p, q) = 1, it is called a reduced proper fraction.
 *
 * If we list the set of reduced proper fractions for q <= 8 in ascending
 * order of value, we get:
 *
 *   1/8, 1/7, 1/6, 1/5, 1/4, 2/7, 1/3, 3/8, 2/5, 3/7, 1/2, 4/7, 3/5, 5/8,
 *   2/3, 5/7, 3/4, 4/5, 5/6, 6/7, 7/8
 *
 * It can be seen that there are 21 elements in this set.
 *
 * How many elements would be contained in the set of reduced proper fractions
 * for q <= 1,000,000?
 */

#include <iostream>
#include "euler/farey.hpp"
#include "euler.h"

BEGIN_PROBLEM(72, solve_problem_72)
  PROBLEM_TITLE("Counting fractions")
  PROBLEM_ANSWER("303963552391")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
  PROBLEM_KEYWORDS("farey,totient")
END_PROBLEM()

static void solve_problem_72()
{
#if 0
  const int M = 8;
#else
  const int M = 1000000;
#endif
  std::cout << euler::farey_size<int64_t>(M) - 2 << std::endl;
}
