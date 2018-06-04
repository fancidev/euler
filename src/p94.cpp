/**
 * An almost equilateral triangle is a triangle for which two sides are equal
 * and the third differs by no more than one unit.
 *
 * Find the sum of the perimeters of all almost equilateral triangles with
 * integral side lengths and area and whose perimeters do not exceed 10^9.
 *
 * See notes/p94.tex for solution.
 */

#include <cstdint>
#include <iostream>
#include "euler/pell.hpp"
#include "euler.h"

BEGIN_PROBLEM(94, solve_problem_94)
  PROBLEM_TITLE("Almost equilateral Heronian triangles")
  PROBLEM_ANSWER("518408346")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("log(L)")
  PROBLEM_SPACE_COMPLEXITY("1")
  PROBLEM_KEYWORDS("triangle,pell equation")
END_PROBLEM()

static void solve_problem_94()
{
  const int64_t L = 1000000000LL;
  euler::pell_solution_iterator<int64_t, int> xy(3, 2, 1, 0, 1);
  long long total = 0;
  for (; xy->second <= L/2+1; ++xy)
  {
    const int64_t x = xy->first, y = xy->second;
    if (y <= 2)
    {
      continue;
    }

    // h = x / 3, a = (y -/+ 2) / 3.
    int64_t a = 0, b = 0;
    if (y % 3 == 2)
    {
      a = (y - 2) / 3;
      b = 2 * a + 1;
    }
    if (y % 3 == 1 && y <= L/2 - 1)
    {
      a = (y + 2) / 3;
      b = 2 * a - 1;
    }

    if (a > 0)
    {
      total += (2 * a + b + b);
      if (verbose())
      {
        std::cout << "(x,y) = (" << x << ", " << y << "), Sides = ("
                  << (2*a) << ", " << b << ", " << b << ")" << std::endl;
      }
    }
  }
  std::cout << total << std::endl;
}
