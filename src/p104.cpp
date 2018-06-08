/**
 * The Fibonacci sequence is defined by the recurrence relation:
 *
 *   F(n) = F(n-1) + F(n-2), where F(1) = 1 and F(2) = 1.
 *
 * It turns out that F(541), which contains 113 digits, is the first Fibonacci
 * number for which the last nine digits are 1-9 pandigital (contain all the
 * digits 1 to 9, but not necessarily in order). And F(2749), which contains
 * 575 digits, is the first Fibonacci number for which the first nine digits
 * are 1-9 pandigital.
 *
 * Given that F(k) is the first Fibonacci number for which the first nine
 * digits AND the last nine digits are 1-9 pandigital, find k.
 *
 * SOLUTION:
 *
 * Compute F(n) in order. The last nine digits are kept track of exactly by
 * storing the result modulo 10^9. The first nine digits are kept track of
 * approximately by storing the most significant ~15 digits as a double and
 * reducing the exponent regularly to avoid overflow.
 */

#include <iostream>
#include "euler/digits.hpp"
#include "euler.h"

BEGIN_PROBLEM(104, solve_problem_104)
  PROBLEM_TITLE("Pandigital Fibonacci ends")
  PROBLEM_ANSWER("329468")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
  PROBLEM_KEYWORDS("digit,fibonacci")
END_PROBLEM()

static void solve_problem_104()
{
  int F_2 = 1, F_1 = 1;
  double G_2 = 1.0, G_1 = 1.0;
  for (int k = 3; ; k++)
  {
    // Keep track of the last 9 digits by "mod" the result by 10^9
    int F = (F_2 + F_1) % 1000000000;

    // Keep trace of the first 9 digits using floating point approximation
    double G = G_2 + G_1;
    if (G >= 1e9)
    {
      G /= 10.0;
      G_1 /= 10.0;
    }

    // Check digits
    if (euler::is_pandigital(F) && euler::is_pandigital(static_cast<int>(G)))
    {
      std::cout << k << std::endl;
      break;
    }

    // Update sequence
    F_2 = F_1;
    F_1 = F;
    G_2 = G_1;
    G_1 = G;
  }
}
