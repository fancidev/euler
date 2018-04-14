#include <iostream>
#include <numeric>
#include "euler/decimal.hpp"
#include "euler.h"

BEGIN_PROBLEM(16, solve_problem_16)
  PROBLEM_TITLE("Sum of the digits of 2^1000")
  PROBLEM_ANSWER("1366")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("N^2")
  PROBLEM_SPACE_COMPLEXITY("N")
END_PROBLEM()

static void solve_problem_16()
{
  const int N = 1000;
  const int D = static_cast<int>(N*0.301029995663981) + 1;
  euler::decimal<D> x(2);
  for (int i = 1; i < N; i++)
  {
    x += x;
  }
  auto r = euler::rdigits(x);
  std::cout << std::accumulate(r.begin(), r.end(), 0) << std::endl;
}
