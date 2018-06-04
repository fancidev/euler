/**
 * A number chain is created by continuously adding the square of the digits
 * in a number to form a new number until it has been seen before.
 *
 * For example,
 *   44 -> 32 -> 13 -> 10 -> 1 -> 1
 *   85 -> 89 -> 145 -> 42 -> 20 -> 4 -> 16 -> 37 -> 58 -> 89
 *
 * Therefore any chain that arrives at 1 or 89 will become stuck in an endless
 * loop. What is most amazing is that EVERY starting number will eventually
 * arrive at 1 or 89.
 *
 * How many starting numbers below ten million will arrive at 89?
 */

#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include "euler/digits.hpp"
#include "euler.h"

BEGIN_PROBLEM(92, solve_problem_92)
  PROBLEM_TITLE("Square digit chains")
  PROBLEM_ANSWER("8581146")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("N*log(N)")
  PROBLEM_SPACE_COMPLEXITY("log(N)")
END_PROBLEM()

static int sum_square_digits(int n)
{
  int sum = 0;
  for (int d: euler::rdigits(n))
  {
    sum += d * d;
  }
  return sum;
}

static void solve_problem_92()
{
  std::vector<int> cache(81*7+1);
  cache[1] = 1;
  cache[89] = 89;

  int count = 0;
  std::vector<int> path;
  for (int n = 1; n < 10000000; n++)
  {
    path.clear();
    int k;
    for (k = sum_square_digits(n); cache[k] == 0; k = sum_square_digits(k))
    {
      path.push_back(k);
    }
    k = cache[k];
    if (n < static_cast<int>(cache.size()))
    {
      std::for_each(path.begin(), path.end(), [&](int i) { cache[i] = k; });
      cache[n] = k;
    }
    if (k == 89)
    {
      ++count;
    }
  }
  std::cout << count << std::endl;
}
