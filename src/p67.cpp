/**
 * Find the maximum total from the top of the triangle to the bottom by
 * moving to adjacent numbers on the row below.
 */

#include <algorithm>
#include <iostream>
#include "euler.h"

BEGIN_PROBLEM(67, solve_problem_67)
  PROBLEM_TITLE("Efficient algorithm to find the maximal sum in a triangle")
  PROBLEM_ANSWER("7273")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("n^2")
  PROBLEM_SPACE_COMPLEXITY("n^2")
END_PROBLEM()

static void solve_problem_67()
{
  const int N = 100;

  // Read the numbers from a file.
  const int numbers[N][N] = {
#include "p67-triangle.txt"
  };

  // Find the maximum sum using dynamic programming.
  int best[N];
  for (int j = 0; j < N; j++)
  {
    best[j] = numbers[N - 1][j];
  }

  for (int i = N - 2; i >= 0; i--)
  {
    for (int j = 0; j <= i; j++)
    {
      best[j] = numbers[i][j] + std::max(best[j], best[j + 1]);
    }
  }
  std::cout << best[0] << std::endl;
}
