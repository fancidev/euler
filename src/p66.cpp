/**
 * Consider quadratic Diophantine equations of the form:
 *
 *   x^2 - D*y^2 = 1
 *
 * For example, when D=13, the minimal solution in x is 649^2 - 13*180^2 = 1.
 *
 * Find the value of D <= 1000 in minimal solutions of x for which the largest
 * value of x is obtained.
 */

#include <iostream>
#include "euler/pell.hpp"
#include "euler.h"

BEGIN_PROBLEM(66, solve_problem_66)
  PROBLEM_TITLE("Investigate the Diophantine equation x^2 - D*y^2 = 1")
  PROBLEM_ANSWER("661")
  PROBLEM_DIFFICULTY(2)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("N^2")
  PROBLEM_SPACE_COMPLEXITY("1")
  PROBLEM_KEYWORDS("diophantine,pell")
END_PROBLEM()

static void solve_problem_66()
{
  typedef double number_t;
  number_t max_x = 0;
  int max_n = 0;
  for (int n = 1; n <= 1000; n++)
  {
    std::pair<number_t, number_t> xy;
    if (euler::solve_pell_equation(n, xy))
    {
      if (xy.first > max_x)
      {
        max_x = xy.first;
        max_n = n;
      }
    }
  }
  std::cout << max_n << std::endl;
}
