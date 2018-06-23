// See p121.tex for solution.

#include <algorithm>
#include <iostream>
#include "euler.h"

BEGIN_PROBLEM(121, solve_problem_121)
  PROBLEM_TITLE("Investigate the game of chance involving colored discs")
  PROBLEM_ANSWER("2269")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("N^2")
  PROBLEM_SPACE_COMPLEXITY("N")
END_PROBLEM()

static void solve_problem_121()
{
#if 0
  const int N = 4;
#else
  const int N = 15;
#endif

  const int M = N/2+1;
  double P[M+1] = {0};
  P[0] = 1;
  for (int n = 1; n <= N; n++)
  {
    for (int m = std::min(n, M); m >= 1; m--)
    {
      P[m] = P[m-1]/(n+1) + P[m]*n/(n+1);
    }
  }
  std::cout << static_cast<int>(1/P[M]) << std::endl;
}
