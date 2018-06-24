// See p141.tex for solution

#include <cstdint>
#include <iostream>
#include "euler/imath.hpp"
#include "euler/prime_factor.hpp"
#include "euler/sequence.hpp"
#include "euler.h"

BEGIN_PROBLEM(141, solve_problem_141)
  PROBLEM_TITLE("Investigating progressive numbers which are also square")
  PROBLEM_ANSWER("878454337159")
  PROBLEM_DIFFICULTY(2)
  PROBLEM_FUN_LEVEL(2)
  PROBLEM_TIME_COMPLEXITY("N^0.58")
  PROBLEM_SPACE_COMPLEXITY("1")
END_PROBLEM()

// Find all progressive numbers.
template <class Func>
static void find_progressive_numbers(int64_t N, Func f)
{
  // Special case: r = 1
  for (int64_t q = 2; ; q++)
  {
    int64_t n = q*q*q+1;
    if (n > N)
    {
      break;
    }
    f(n);
  }

  // For each r >= 2, r must divide q^2, and q must be greater than r.
  for (int r = 2; static_cast<int64_t>(r) * r < N; r++)
  {
    int base = 1;
    for (const auto &pk: euler::distinct(euler::factorize(r)))
    {
      const int p = pk.first;
      const int k = static_cast<int>(pk.second);
      for (int i = (k-1)/2; i >= 0; --i)
      {
        base *= p;
      }
    }
    for (int q = (r / base + 1) * base; ; q += base)
    {
      int64_t p = static_cast<int64_t>(q) * q / r;
      int64_t n = p * q + r;
      if (n > N)
      {
        break;
      }
      f(n);
    }
  }
}

static int64_t sum_progressive_squares(int64_t N)
{
  int64_t sum = 0;
  find_progressive_numbers(N, [&sum](int64_t n)
  {
    int64_t root = euler::isqrt(n);
    if (root * root == n)
    {
      sum += n;
    }
  });
  return sum;
}

static void solve_problem_141()
{
#if 0
  const int64_t N = 99999;
#else
  const int64_t N = 999999999999LL;
#endif
  std::cout << sum_progressive_squares(N) << std::endl;
}
