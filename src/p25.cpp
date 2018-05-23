/**
 * The Fibonacci sequence is defined by the recurrence relation:
 *
 *   F(n) = F(n−1) + F(n−2)
 *
 * with F(1) = 1 and F(2) = 1.
 *
 * What is the index of the first term in the Fibonacci sequence to contain
 * 1000 decimal digits?
 */

#include <iostream>
#include <numeric>
#include "euler/decimal.hpp"
#include "euler.h"

BEGIN_PROBLEM(25, solve_problem_25)
  PROBLEM_TITLE("1000-digit Fibonacci number")
  PROBLEM_ANSWER("4782")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
  PROBLEM_KEYWORDS("digit")
END_PROBLEM()

static void solve_problem_25()
{
  euler::decimal a = 1, b = 1;
  for (int n = 3; ; n++)
  {
    euler::decimal c = a + b;
    if (c.size() >= 1000)
    {
      std::cout << n << std::endl;
      break;
    }
    a = b;
    b = c;
  }
}
