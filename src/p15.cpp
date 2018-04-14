/*
 * Starting in the top left corner of a 2-by-2 grid, there are 6 routes 
 * (without backtracking) to the bottom right corner.
 *
 * How many routes are there through a 20-by-20 grid?
 */

#include <iostream>
#include "euler/binomial.hpp"
#include "euler.h"

BEGIN_PROBLEM(15, solve_problem_15)
  PROBLEM_TITLE("Number of routes in a grid")
  PROBLEM_ANSWER("137846528820")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("N^2")
  PROBLEM_SPACE_COMPLEXITY("N")
END_PROBLEM()

static void solve_problem_15()
{
  const int N = 20;
  std::cout << euler::binom<int64_t>(2*N,N) << std::endl;
}
