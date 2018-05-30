/**
 * Find the minimal path sum of a 80 by 80 matrix, from the top left
 * to the bottom right by only moving right and down.
 */

#include <algorithm>
#include <iostream>
#include "euler.h"

BEGIN_PROBLEM(81, solve_problem_81)
  PROBLEM_TITLE("Path sum: two ways")
  PROBLEM_ANSWER("427337")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("N^2")
  PROBLEM_SPACE_COMPLEXITY("N^2")
  PROBLEM_KEYWORDS("combinatorics,dynamic programming")
END_PROBLEM()

static void solve_problem_81()
{
#if 0
  const int N = 5;
  int m[N][N] = {
    { 131, 673, 234, 103,  18 },
    { 201,  96, 342, 965, 150 },
    { 630, 803, 746, 422, 111 },
    { 537, 699, 497, 121, 956 },
    { 805, 732, 524,  37, 331 },
  };
#else
  const int N = 80;
  int m[N][N] = {
#include "p81-matrix.txt"
  };
#endif

  // Compute min path.
  for (int j = N - 2; j >= 0; j--)
  {
    m[N-1][j] += m[N-1][j+1];
  }
  for (int i = N - 2; i >= 0; i--)
  {
    m[i][N-1] += m[i+1][N-1];
  }
  for (int i = N - 2; i >= 0; i--)
  {
    for (int j = N - 2; j >= 0; j--)
    {
      m[i][j] += std::min(m[i+1][j], m[i][j+1]);
    }
  }
  std::cout << m[0][0] << std::endl;
}
