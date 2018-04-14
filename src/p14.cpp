/**
 * The following iterative sequence is defined for the set of positive
 * integers:
 *
 *   n -> n / 2     (n is even)
 *   n -> 3 * n + 1 (n is odd)
 *
 * Using the rule above and starting with 13, we generate the following
 * sequence:
 *
 *   13  40  20  10  5  16  8  4  2  1
 *
 * It can be seen that this sequence (starting at 13 and finishing at 1)
 * contains 10 terms. Although it has not been proved yet (Collatz Problem),
 * it is thought that all starting numbers finish at 1.
 *
 * Which starting number, under one million, produces the longest chain?
 *
 * NOTE: Once the chain starts the terms are allowed to go above one million.
 */

#include <cstdint>
#include <iostream>
#include "euler.h"

BEGIN_PROBLEM(14, solve_problem_14)
  PROBLEM_TITLE("Longest sequence under 1,000,000 for Collatz Problem")
  PROBLEM_ANSWER("837799")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(2)
  PROBLEM_TIME_COMPLEXITY("N*L")
  PROBLEM_SPACE_COMPLEXITY("1")
  PROBLEM_KEYWORDS("collatz problem")
END_PROBLEM()

static int get_chain_length(int start, int64_t &max_number)
{
  int length = 1;
  for (int64_t n = start; n > 1; length++)
  {
    if ((n & 1) != 0)
    {
      n = 3 * n + 1;
    }
    else
    {
      n >>= 1;
    }
    if (n > max_number)
    {
      max_number = n;
    }
  }
  return length;
}

static void solve_problem_14()
{
  bool verbose = false;
  int64_t max_number = 0;
  int x = 1, xlen = 1;
  for (int i = 2; i < 1000000; i++)
  {
    int len = get_chain_length(i, max_number);
    if (len > xlen)
    {
      x = i;
      xlen = len;
    }
  }
  std::cout << x << std::endl;
  if (verbose)
  {
    std::cout << "Max length: " << xlen << std::endl;
    std::cout << "Max number: " << max_number << std::endl;
  }
}
