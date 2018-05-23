/**
 * Find the sum of the digits in the number factorial(100).
 */

#include <iostream>
#include <numeric>
#include "euler/decimal.hpp"
#include "euler.h"

BEGIN_PROBLEM(20, solve_problem_20)
  PROBLEM_TITLE("Factorial digit sum")
  PROBLEM_ANSWER("648")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
  PROBLEM_KEYWORDS("digit")
END_PROBLEM()

static void solve_problem_20()
{
  euler::decimal x = 1;
  for (int i = 2; i <= 100; i++)
  {
    x = x * i;
  }
  if (verbose())
  {
    std::cout << x << std::endl;
  }
  auto digits = euler::digits(x);
  int sum = std::accumulate(digits.begin(), digits.end(), 0);
  std::cout << sum << std::endl;
}
