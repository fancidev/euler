/**
 * The fraction 49/98 is a curious fraction, as an inexperienced mathematician
 * in attempting to simplify it may incorrectly believe that 49/98 = 4/8,
 * which is correct, is obtained by cancelling the 9s.
 *
 * We shall consider fractions like, 30/50 = 3/5, to be trivial examples.
 *
 * There are exactly four non-trivial examples of this type of fraction, less
 * than one in value, and containing two digits in the numerator and
 * denominator.
 *
 * If the product of these four fractions is given in its lowest common terms,
 * find the value of the denominator.
 *
 * ANSWER: 100.
 */

#include <iostream>
#include "euler/gcd.hpp"
#include "euler.h"

BEGIN_PROBLEM(33, solve_problem_33)
  PROBLEM_TITLE("Digit cancelling fractions")
  PROBLEM_ANSWER("100")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("?")
  PROBLEM_SPACE_COMPLEXITY("?")
END_PROBLEM()

static void solve_problem_33()
{
  int p_prod = 1, q_prod = 1;

  // Let p = 10*a_1 + b_1, q = 10*a_2 + b_2, with 1 <= a,b <= 9.
  // It is easy to show that it can't be a_1=a_2 or b_1=b_2.
  // So we have a_1=b_2 or b_1=a_2. And we require a_1 < a_2.
  for (int a1 = 1; a1 <= 8; a1++)
  {
    for (int b1 = 1; b1 <= 9; b1++)
    {
      int p = 10*a1 + b1;
      for (int a2 = a1+1; a2 <= 9; a2++)
      {
        for (int b2 = 1; b2 <= 9; b2++)
        {
          int q = 10*a2 + b2;
          if ((a1 == b2 && b1*q == a2*p) // p/q == b1/a2
            || (b1 == a2 && a1*q == b2*p)) // p/q = a1/b2
          {
            //std::cout << p << " / " << q << std::endl;
            int d = euler::gcd(p, q);
            p_prod *= p / d;
            q_prod *= q / d;
          }
        }
      }
    }
  }

  int d = euler::gcd(p_prod, q_prod);
  p_prod /= d;
  q_prod /= d;
  if (verbose())
  {
    std::cout << p_prod << " / " << q_prod << std::endl;
  }
  std::cout << q_prod << std::endl;
}
