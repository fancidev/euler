#include <iostream>
#include <algorithm>
#include "euler/digits.hpp"
#include "euler.h"

BEGIN_PROBLEM(40, solve_problem_40)
  PROBLEM_TITLE("Finding the nth digit of a fractional number")
  PROBLEM_ANSWER("210")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("ln(N)")
  PROBLEM_SPACE_COMPLEXITY("1")
  PROBLEM_KEYWORDS("digit")
END_PROBLEM()

static int get_nth_digit(int m, int p)
{
  auto digits = euler::digits<10>(m);
  auto it = digits.begin();
  std::advance(it, (p - 1));
  return *it;
}

static void solve_problem_40()
{
  const int N = 1000000;
  const int b = 10; // base

  int result = 1;
  int n = 1;
  int k = 1; // k-digit group
  int s = 0; // total number of digits till the end of k-digit group
  int bexp = 1; // b^(k-1)
  for (; ; )
  {
    int s_next = s + (b-1)*k*bexp;
    for (; n <= s_next && n <= N; n *= b)
    {
      int i = (n - s - 1) / k;
      int m = bexp + i;
      int p = n - s - i*k;
      if (verbose())
      {
        std::cout << p << "-th digit in " << m << " is "
                  << get_nth_digit(m, p) << std::endl;
      }
      result *= get_nth_digit(m, p);
    }
    if (n > N)
    {
      break;
    }

    s = s_next;
    bexp *= b;
    k++;
  }
  std::cout << result << std::endl;
}
