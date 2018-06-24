// See p137.tex for solution

#include <cstdint>
#include <iostream>
#include "euler/pell.hpp"
#include "euler.h"

BEGIN_PROBLEM(137, solve_problem_137)
  PROBLEM_TITLE("Polynomial series with Fibonacci coefficients")
  PROBLEM_ANSWER("1120149658760")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(2)
  PROBLEM_TIME_COMPLEXITY("n")
  PROBLEM_SPACE_COMPLEXITY("1")
  PROBLEM_KEYWORDS("Fibonacci,Pell")
END_PROBLEM()

static void solve_problem_137()
{
  int N = 15;

  // x^2 - 5*y^2 = -4, (x,y) = (1,1), (4,2), (11,5)
  // x^2 - 5*y^2 = 1,  (x,y) = (9,4)
  typedef euler::pell_solution_iterator<int64_t, int> Iter;
  Iter xy1(5, 9, 4, 1, 1);
  Iter xy2(5, 9, 4, 4, 2);
  Iter xy3(5, 9, 4, 11, 5);
  Iter xy[3] = { xy1, xy2, xy3 };
  int i = 0;
  for (int n = 0; ; ++xy[i], i=(i+1)%3)
  {
    int64_t x = xy[i]->first;
    if (x % 5 == 1 && x > 1)
    {
      if (verbose())
      {
        std::cout << (n+1) << "th golden nugget (from " << (i+1) << ") is "
            << (x-1)/5 << std::endl;
      }
      if (++n >= N)
      {
        break;
      }
    }
  }
  std::cout << (xy[i]->first-1)/5 << std::endl;
}
