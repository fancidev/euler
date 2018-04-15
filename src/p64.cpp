/**
 * All square roots are periodic when written as continued fractions.
 *
 * The first ten continued fraction representations of (irrational) square 
 * roots are:
 *
 * sqrt(2)=[1;(2)], period=1
 * sqrt(3)=[1;(1,2)], period=2
 * sqrt(5)=[2;(4)], period=1
 * sqrt(6)=[2;(2,4)], period=2
 * sqrt(7)=[2;(1,1,1,4)], period=4
 * sqrt(8)=[2;(1,4)], period=2
 * sqrt(10)=[3;(6)], period=1
 * sqrt(11)=[3;(3,6)], period=2
 * sqrt(12)=[3;(2,6)], period=2
 * sqrt(13)=[3;(1,1,1,1,6)], period=5
 *
 * Exactly four continued fractions, for N <= 13, have an odd period.
 *
 * How many continued fractions for N <= 10000 have an odd period?
 */

#include <iostream>
#include "euler/imath.hpp"
#include "euler/continued_fraction.hpp"
#include "euler.h"

BEGIN_PROBLEM(64, solve_problem_64)
  PROBLEM_TITLE("How many continued fractions have an odd period")
  PROBLEM_ANSWER("1322")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("N^2")
  PROBLEM_SPACE_COMPLEXITY("1")
  PROBLEM_KEYWORDS("continued fraction")
END_PROBLEM()

static void solve_problem_64()
{
  const int N = 10000;

  int count = 0;
  for (int n = 1; n <= N; n++)
  {
    if (euler::continued_fraction_sqrt(n).period() % 2 == 1)
    {
      ++count;
    }
  }
  std::cout << count << std::endl;
}
