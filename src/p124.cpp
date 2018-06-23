/**
 * The radical of n, rad(n), is the product of the distinct prime factors of
 * n. For example, 504 = 23 × 32 × 7, so rad(504) = 2 × 3 × 7 = 42.
 *
 * If we calculate rad(n) for 1 <= n <= 10, then sort them on rad(n), and
 * sorting on n if the radical values are equal, we get:
 *
 *   Unsorted:
 *   ======================================
 *   n        1  2  3  4  5  6  7  8  9  10
 *   --------------------------------------
 *   rad(n)   1  2  3  2  5  6  7  2  3  10
 *   ======================================
 *
 *   Sorted:
 *   ======================================
 *   n        1  2  4  8  3  9  5  6  7  10
 *   --------------------------------------
 *   rad(n)   1  2  2  2  3  3  5  6  7  10
 *   --------------------------------------
 *   k        1  2  3  4  5  6  7  8  9  10
 *   ======================================
 *
 * Let E(k) be the kth element in the sorted n row; for example, E(4) = 8 and
 * E(6) = 9.
 *
 * If rad(n) is sorted for 1 <= n <= 100000, find E(10000).
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include <utility>
#include "euler.h"

BEGIN_PROBLEM(124, solve_problem_124)
  PROBLEM_TITLE("Ordered radicals")
  PROBLEM_ANSWER("21417")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
END_PROBLEM()

static void solve_problem_124()
{
  const int N = 100000;
  std::vector<int> rad(N+1, 1);

  // sieve method
  for (int p = 2; p <= N; p += 2)
  {
    rad[p] = 2;
  }
  for (int p = 3; p <= N; p += 2)
  {
    if (rad[p] == 1) // p is prime
    {
      for (int k = p; k <= N; k += p)
      {
        rad[k] *= p;
      }
    }
  }

  // sort
  std::vector<std::pair<int,int>> list(N);
  for (int i = 0; i < N; i++)
  {
    list[i] = std::pair<int,int>(rad[i+1], i+1);
  }
  std::sort(list.begin(), list.end());
  std::cout << list[9999].second << std::endl;
}
