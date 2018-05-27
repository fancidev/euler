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
 * It can be seen that there are 3 fractions between 1/3 and 1/2.
 *
 * How many fractions lie between 1/3 and 1/2 in the sorted set of reduced
 * proper fractions for q <= 12,000?
 */

#include <iostream>
#include "euler/farey.hpp"
#include "euler.h"

BEGIN_PROBLEM(73, solve_problem_73)
  PROBLEM_TITLE("Counting fractions in a range")
  PROBLEM_ANSWER("7295372")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
  PROBLEM_KEYWORDS("farey")
END_PROBLEM()

static void solve_problem_73()
{
#if 0
  const int n = 8;
#else
  const int n = 12000;
#endif
  auto it1 = euler::farey_iterator<int>(n, 1, 3);
  auto it2 = euler::farey_iterator<int>(n, 1, 2);
  auto count = std::distance(it1, it2) - 1;
  std::cout << count << std::endl;
}
