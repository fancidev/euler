/**
 * Find the minimal path sum of a 80 by 80 matrix, from the top left cell to
 * the bottom right cell, by going up, down, left or right.
 */

#include <algorithm>
#include <iostream>
#include <limits>
#include <set>
#include "euler.h"

BEGIN_PROBLEM(83, solve_problem_83)
  PROBLEM_TITLE("Path sum: four ways")
  PROBLEM_ANSWER("425185")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
  PROBLEM_KEYWORDS("graph,shortest path")
END_PROBLEM()

static void solve_problem_83()
{
#if 0
  const int N = 5;
  const int m[N][N] = {
    { 131, 673, 234, 103,  18 },
    { 201,  96, 342, 965, 150 },
    { 630, 803, 746, 422, 111 },
    { 537, 699, 497, 121, 956 },
    { 805, 732, 524,  37, 331 },
  };
#else
  const int N = 80;
  const int m[N][N] = {
#include "p83-matrix.txt"
  };
#endif

  // Simple implementation of Dijkstra's algorithm
  int cost[N][N];
  for (int i = 0; i < N; i++)
  {
    std::fill(&cost[i][0], &cost[i][N], std::numeric_limits<int>::max());
  }

  std::set<std::pair<int,int>> frontier;
  frontier.insert(std::make_pair(0, 0));
  cost[0][0] = m[0][0];

  while (true)
  {
    // Find the lowest-cost node in frontier
    auto it = std::min_element(frontier.begin(), frontier.end(),
        [&cost](const std::pair<int,int> &a, const std::pair<int,int> &b)
        {
          return cost[a.first][a.second] < cost[b.first][b.second];
        });
    const std::pair<int,int> ij = *it;
    const int i = ij.first, j = ij.second;
    if (i == N - 1 && j == N - 1) // target cell
    {
      std::cout << cost[i][j] << std::endl;
      break;
    }
    frontier.erase(it);

    // Try move forward from (i, j)
    if (j > 0 && cost[i][j] + m[i][j-1] < cost[i][j-1]) // move left
    {
      frontier.insert(std::make_pair(i, j-1));
      cost[i][j-1] = cost[i][j] + m[i][j-1];
    }
    if (j < N - 1 && cost[i][j] + m[i][j+1] < cost[i][j+1]) // move right
    {
      frontier.insert(std::make_pair(i, j+1));
      cost[i][j+1] = cost[i][j] + m[i][j+1];
    }
    if (i > 0 && cost[i][j] + m[i-1][j] < cost[i-1][j]) // move up
    {
      frontier.insert(std::make_pair(i-1, j));
      cost[i-1][j] = cost[i][j] + m[i-1][j];
    }
    if (i < N - 1 && cost[i][j] + m[i+1][j] < cost[i+1][j]) // move down
    {
      frontier.insert(std::make_pair(i+1, j));
      cost[i+1][j] = cost[i][j] + m[i+1][j];
    }
  }
}
