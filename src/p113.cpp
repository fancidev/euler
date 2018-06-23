/**
 * Working from left-to-right if no digit is exceeded by the digit to its left
 * it is called an increasing number; for example, 134468.
 *
 * Similarly if no digit is exceeded by the digit to its right it is called a
 * decreasing number; for example, 66420.
 *
 * We shall call a positive integer that is neither increasing nor decreasing
 * a "bouncy" number; for example, 155349.
 *
 * As n increases, the proportion of bouncy numbers below n increases such
 * that there are only 12951 numbers below one-million that are not bouncy and
 * only 277032 non-bouncy numbers below 10^10.
 *
 * How many numbers below a googol (10^100) are not bouncy?
 *
 * SOLUTION:
 *
 * Let S(n,d) denote the set of n-element non-increasing sequences where each
 * element is taken from [0..d] and the first element is d (d >= 0). Let
 *
 *   f(n,d) = |S(n,d)|
 *
 * denote the number of such sequences. It can be shown that
 *
 *   f(1,d) = 1
 *
 *   f(n,d) = f(n-1,d) + f(n-1,d-1) + ... + f(n-1,0)
 *          = f(n-1,d) + f(n,d-1)
 *
 * where we define f(n,-1) = 0.
 *
 * Increasing numbers with n or fewer digits may be mapped to S(n+1,9) by
 * padding their digits with leading zeros and then reversing them, except
 * that 9000..000 should be excluded. The number of increasing numbers is
 * therefore
 *
 *   f(n+1,9) - 1
 *
 * Decreasing numbers with n or fewer digits may be mapped to S(n+1,10) by
 * padding their digits with leading 'T's where T is a fictious symbol that is
 * equal to 10, except that T000..000, TT00..000, TTT0..000, ... TTTT...TTT
 * should be excluded. The number of decreasing numbers is therefore
 *
 *   f(n+1,10) - (n+1)
 *
 * Putting these together, the number of non-bouncy numbers with n or fewer
 * digits is equal to
 *
 *   (f(n+1,9) - 1) + (f(n+1,10) - (n+1)) - (9 * n)
 *
 * where the last term (9 * n) accounts for 111..111 to 999..999 which are
 * counted twice.
 */

#include <cstdint>
#include <iostream>
#include "euler.h"

BEGIN_PROBLEM(113, solve_problem_113)
  PROBLEM_TITLE("Non-bouncy numbers")
  PROBLEM_ANSWER("51161058134250")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
  PROBLEM_KEYWORDS("digits")
END_PROBLEM()

static void solve_problem_113()
{
#if 0
  const int N = 6;
#else
  const int N = 100;
#endif

  int64_t f[11] = {1,1,1,1,1,1,1,1,1,1,1};
  for (int n = 2; n <= N + 1; n++)
  {
    for (int d = 1; d <= 10; d++)
    {
      f[d] += f[d-1];
    }
  }

  std::cout << (f[9] + f[10] - 2 - 10*N) << std::endl;
}
