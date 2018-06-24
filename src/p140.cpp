// See p140.tex for solution

#include <cstdint>
#include <iostream>
#include "euler/pell.hpp"
#include "euler.h"

BEGIN_PROBLEM(140, solve_problem_140)
  PROBLEM_TITLE("Polynomial series with recursive coefficients")
  PROBLEM_ANSWER("5673835352990")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("n")
  PROBLEM_SPACE_COMPLEXITY("1")
  PROBLEM_KEYWORDS("recurrence,Pell")
END_PROBLEM()

static void solve_problem_140()
{
  int N = 30;

  // x^2 - 5*y^2 = 44, (x,y) = (7,1), (8,2), (13,5), (17,7), (32,14), (43,19)
  // x^2 - 5*y^2 = 1,  (x,y) = (9,4)
  typedef euler::pell_solution_iterator<int64_t,int> Iter;
  Iter xy1(5, 9, 4, 7, 1);
  Iter xy2(5, 9, 4, 8, 2);
  Iter xy3(5, 9, 4, 13, 5);
  Iter xy4(5, 9, 4, 17, 7);
  Iter xy5(5, 9, 4, 32, 14);
  Iter xy6(5, 9, 4, 43, 19);
  Iter xy[] = { xy1, xy2, xy3, xy4, xy5, xy6 };
  int64_t sum = 0;
  int i = 0;
  for (int n = 0; n < N; ++xy[i], i=(i+1)%(sizeof(xy)/sizeof(xy[0])))
  {
    int64_t x = xy[i]->first;
    if (x % 5 == 2 && x > 7)
    {
      if (verbose())
      {
        std::cout << (n+1) << "th golden nugget (from " << (i+1) << ") is "
          << (x-7)/5 << std::endl;
      }
      sum += (x-7)/5;
      ++n;
    }
  }
  std::cout << sum << std::endl;
}
