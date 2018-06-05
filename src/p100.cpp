#include <cstdint>
#include <iostream>
#include "euler.h"

BEGIN_PROBLEM(100, solve_problem_100)
  PROBLEM_TITLE("Exactly 50% chance of taking two blue discs")
  PROBLEM_ANSWER("756872327473")
  PROBLEM_DIFFICULTY(2)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("log(N)")
  PROBLEM_SPACE_COMPLEXITY("1")
END_PROBLEM()

static void solve_problem_100()
{
  const int64_t N = 1000000000000LL;
  int64_t x = 1, y = 1;
  while (true)
  {
    int64_t xx = 3*x + 4*y;
    int64_t yy = 2*x + 3*y;
    int64_t n = (1+xx)/2;
    int64_t b = (1+yy)/2;
    if (n > N)
    {
      std::cout << b << std::endl;
      break;
    }
    x = xx;
    y = yy;
  }
}
