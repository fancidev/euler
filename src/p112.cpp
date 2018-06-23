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
 * Clearly there cannot be any bouncy numbers below one-hundred, but just over
 * half of the numbers below one-thousand (525) are bouncy. In fact, the least
 * number for which the proportion of bouncy numbers first reaches 50% is 538.
 *
 * Surprisingly, bouncy numbers become more and more common and by the time we
 * reach 21780 the proportion of bouncy numbers is equal to 90%.
 *
 * Find the least number for which the proportion of bouncy numbers is exactly
 * 99%.
 */

#include <iostream>
#include "euler.h"

BEGIN_PROBLEM(112, solve_problem_112)
  PROBLEM_TITLE("Bouncy numbers")
  PROBLEM_ANSWER("1587000")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
  PROBLEM_KEYWORDS("digits")
END_PROBLEM()

static bool is_nonbouncy_number(int n)
{
  int dir = 0;
  int prev = n % 10;
  n /= 10;
  while (n > 0)
  {
    int d = n % 10;
    n /= 10;
    int new_dir = (d > prev)? 1 : (d < prev)? -1 : 0;
    if ((dir < 0 && new_dir > 0) || (dir > 0 && new_dir < 0))
      return false;
    if (new_dir != 0)
      dir = new_dir;
    prev = d;
  }
  return true;
}

static int find_bouncy_proportion(int percent)
{
  int n = 100;
  int count = 0;
  do
  {
    count += is_nonbouncy_number(++n)? 0 : 1;
  }
  while (percent*n != count*100);
  return n;
}

static void solve_problem_112()
{
#if 0
  const int percent = 90;
#else
  const int percent = 99;
#endif
  std::cout << find_bouncy_proportion(percent) << std::endl;
}
