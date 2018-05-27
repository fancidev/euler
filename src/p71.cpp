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
 * It can be seen that 2/5 is the fraction immediately to the left of 3/7.
 *
 * By listing the set of reduced proper fractions for q <= 1,000,000 in
 * ascending order of size, find the numerator of the fraction immediately
 * to the left of 3/7.
 */

#include <iostream>
#include "euler/farey.hpp"
#include "euler.h"

BEGIN_PROBLEM(71, solve_problem_71)
  PROBLEM_TITLE("Ordered fractions")
  PROBLEM_ANSWER("428570")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("1")
  PROBLEM_SPACE_COMPLEXITY("1")
  PROBLEM_KEYWORDS("farey")
END_PROBLEM()

static void solve_problem_71()
{
  std::cout << euler::farey_prev(1000000, 3, 7).first << std::endl;
}
