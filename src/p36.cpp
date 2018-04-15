#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include "euler/digits.hpp"
#include "euler.h"

BEGIN_PROBLEM(36, solve_problem_36)
  PROBLEM_TITLE("Find palindromic numbers in both base 10 and base 2")
  PROBLEM_ANSWER("872187")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("sqrt(N) log(N)")
  PROBLEM_SPACE_COMPLEXITY("log(N)")
END_PROBLEM()

static void solve_problem_36()
{
  int64_t sum = 0;
#if 0
  for (int n = 1; n < 1000000; n++)
  {
    if (is_palindromic(n,10) && is_palindromic(n,2))
    {
      if (verbose)
        std::cout << n << std::endl;
      sum += n;
    }
  }
#else
  // Note: the algorithm can be improved by skipping all even numbers.
  // However, this is not implemented below.
  std::array<int,10> digits;
  for (int a = 1; a < 1000; a++)
  {
    auto p0 = digits.begin();
    auto p1 = std::copy(euler::digits(a).begin(), euler::digits(a).end(), p0);

    // Mirror abc => abcba
    {
      auto p2 = std::reverse_copy(p0, p1 - 1, p1);
      int n = euler::from_digits<int>(p0, p2);
      if (euler::is_palindromic<2>(n))
      {
        if (verbose())
        {
          std::cout << n << std::endl;
        }
        sum += n;
      }
    }

    // Mirror abc => abccba
    {
      auto p2 = std::reverse_copy(p0, p1, p1);
      int n = euler::from_digits<int>(p0, p2);
      if (euler::is_palindromic<2>(n))
      {
        if (verbose())
        {
          std::cout << n << std::endl;
        }
        sum += n;
      }
    }
  }
#endif
  std::cout << sum << std::endl;
}
