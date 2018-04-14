/**
 * A perfect number is a number for which the sum of its proper divisors is
 * exactly equal to the number. For example, the sum of the proper divisors
 * of 28 would be 1 + 2 + 4 + 7 + 14 = 28, which means that 28 is a perfect
 * number.
 *
 * A number n is called deficient if the sum of its proper divisors is less
 * than n and it is called abundant if this sum exceeds n.
 *
 * As 12 is the smallest abundant number, 1 + 2 + 3 + 4 + 6 = 16, the smallest
 * number that can be written as the sum of two abundant numbers is 24. By
 * mathematical analysis, it can be shown that all integers greater than 28123
 * can be written as the sum of two abundant numbers. However, this upper limit
 * cannot be reduced any further by analysis even though it is known that the
 * greatest number that cannot be expressed as the sum of two abundant numbers
 * is less than this limit.
 *
 * Find the sum of all the positive integers which cannot be written as the sum
 * of two abundant numbers.
 */

#include <iostream>
#include <vector>
#include "euler/divisor.hpp"
#include "euler.h"

BEGIN_PROBLEM(23, solve_problem_23)
  PROBLEM_TITLE("Integers that cannot be written as the sum of two abundant numbers")
  PROBLEM_ANSWER("4179871")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("N^2")
  PROBLEM_SPACE_COMPLEXITY("N")
END_PROBLEM()

static void solve_problem_23()
{
  const int N = 28123;

  // Find all abundant numbers less than or equal to N.
  std::vector<int> abundant_numbers;
  for (int n = 1; n <= N; n++)
  {
    if (euler::sum_divisors(n) > 2*n)
    {
      abundant_numbers.push_back(n);
    }
  }

  // Find all possible sums of abundant numbers.
  std::vector<bool> is_sum(N+1);
  for (auto it1 = abundant_numbers.cbegin(); it1 != abundant_numbers.cend(); ++it1)
  {
    for (auto it2 = it1; it2 != abundant_numbers.cend(); ++it2)
    {
      int s = *it1 + *it2;
      if (s <= N)
      {
        is_sum[s] = true;
      }
    }
  }

  // Add up all numbers which are not the sum of two abundant numbers.
  int total = 0;
  for (int n = 1; n <= N; n++)
  {
    if (!is_sum[n])
    {
      total += n;
    }
  }
  std::cout << total << std::endl;
}
