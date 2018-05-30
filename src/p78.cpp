/**
 * Let p(n) represent the number of different ways in which n coins can be
 * separated into piles. For example, five coins can be separated into piles
 * in exactly seven different ways, so p(5) = 7.
 *
 *   5 = 5
 *   5 = 4 + 1
 *   5 = 3 + 2
 *   5 = 3 + 1 + 1
 *   5 = 2 + 2 + 1
 *   5 = 2 + 1 + 1 + 1
 *   5 = 1 + 1 + 1 + 1 + 1
 *
 * Find the least value of n for which p(n) is divisible by one million.
 */

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>
#include "euler/residue.hpp"
#include "euler.h"

BEGIN_PROBLEM(78, solve_problem_78)
  PROBLEM_TITLE("Coin partitions")
  PROBLEM_ANSWER("55374")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("N^1.5")
  PROBLEM_SPACE_COMPLEXITY("N")
  PROBLEM_KEYWORDS("combinatorics,partition")
END_PROBLEM()

/**
 * Computes the number of ways of writing the integer n as a sum of positive
 * integers, ignoring order.
 *
 * @see http://mathworld.wolfram.com/PartitionFunctionP.html
 */
template <class T>
class partition_function
{
  std::vector<T> _cache;

  T _compute(size_t n)
  {
    assert(n == _cache.size());
    T p = 0, s = 1;
    for (size_t k = 1; k <= n; k++)
    {
      size_t n1 = k*(3*k-1)/2;
      size_t n2 = k*(3*k+1)/2;
      if (n1 <= n)
      {
        p += s * _cache[n - n1];
      }
      if (n2 <= n)
      {
        p += s * _cache[n - n2];
      }
      if (n1 > n /* && n2 > n */)
      {
        break;
      }
      s *= -1;
    }
    return p;
  }

public:

  partition_function() : _cache(2)
  {
    _cache[0] = 1;
    _cache[1] = 1;
  }

  /**
   * Computes the partition function for argument @c n.
   *
   * @param n Nonnegative integer.
   *
   * @returns If <c>n > 0</c>, the number of ways of writing @c n as a sum of
   *    positive integers, ignoring order. If <c>n == 0</c>, returns @c 1.
   */
  T operator()(size_t n)
  {
    if (n >= _cache.size())
    {
      for (size_t m = _cache.size(); m <= n; m++)
      {
        _cache.push_back(_compute(m));
      }
    }
    return _cache[n];
  }
};

#if 1
static void solve_problem_78()
{
#if 0
  partition_function<int64_t> p;
  for (size_t n = 0; n <= 100; n++)
  {
    std::cout << "p(" << n << ") = " << p(n) << std::endl;
  }
#endif
  using R = euler::residue<int, 1000000>;
  partition_function<R> p;
  for (size_t n = 0; n <= 100000; n++)
  {
    if (p(n) == 0)
    {
      std::cout << n << std::endl;
      break;
    }
  }
}
#else
// The following does not work because its complexity is O(n^3) and too high.
static void solve_problem_78()
{
  using T = euler::residue<int, 1000000>;

  std::vector<T> p;

  // p(0) = 0, p(1) = 1
  p.push_back(0);
  p.push_back(1);

  // For n >= m >= 0, p[n][m] = number of ways of writing n as sum of integers
  // no greater than m.
  std::vector<std::vector<T>> p;

  // p[0][0] = 1
  p.push_back(std::vector<T>(1, 1));

  for (int n = 1; n < 10000; n++)
  {
    p.push_back(std::vector<T>(n + 1));
    p[n][0] = 0;
    p[n][1] = 1;
    for (int m = 2; m <= n; m++)
    {
      T count = 0;
      for (int s = 0; s <= n; s += m)
      {
        count += p[n - s][std::min(n - s, m - 1)];
      }
      p[n][m] = count;
    }
    if (verbose())
    {
      std::cout << "p(" << n << ") == " << p[n][n] << std::endl;
    }
    if (p[n][n] == 0)
    {
      std::cout << n << std::endl;
      break;
    }
  }
}
#endif
