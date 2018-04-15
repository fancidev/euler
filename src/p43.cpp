/**
 * The number, 1406357289, is a 0 to 9 pandigital number because it is made 
 * up of each of the digits 0 to 9 in some order, but it also has a rather 
 * interesting sub-string divisibility property.
 *
 * Let d1 be the 1st digit, d2 be the 2nd digit, and so on. In this way, 
 * we note the following:
 *   d2d3d4=406 is divisible by 2
 *   d3d4d5=063 is divisible by 3
 *   d4d5d6=635 is divisible by 5
 *   d5d6d7=357 is divisible by 7
 *   d6d7d8=572 is divisible by 11
 *   d7d8d9=728 is divisible by 13
 *   d8d9d10=289 is divisible by 17
 *
 * Find the sum of all 0 to 9 pandigital numbers with this property.
 */

#include <algorithm>
#include <bitset>
#include <cstdint>
#include <iostream>
#include "euler/digits.hpp"
#include "euler.h"

BEGIN_PROBLEM(43, solve_problem_43)
  PROBLEM_TITLE("Pandigital numbers with sub-string divisibility property")
  PROBLEM_ANSWER("16695334890")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("n!")
  PROBLEM_SPACE_COMPLEXITY("n")
END_PROBLEM()

// multi - list of multipliers
// k - try divisors[k]
// tail - last two digits required
// number - current partial number
// mask - current digit mask
static int64_t search(const int multi[], int k, int tail, int number[10], std::bitset<10> &mask)
{
  int64_t sum = 0;
  for (int n = multi[k]; n < 1000; n += multi[k])
  {
    if (n % 100 == tail && !mask.test(n / 100))
    {
      number[k + 1] = n / 100;
      mask.set(n / 100);
      if (k > 0)
      {
        sum += search(multi, k - 1, n / 10, number, mask);
      }
      else if (mask.test(0))
      {
        for (int d = 1; d <= 9; d++)
        {
          if (!mask.test(d))
          {
            number[0] = d;
            int64_t x = euler::from_digits<10, int64_t>(
                &number[0], &number[10]);
            sum += x;
            // std::cout << "Found " << x << std::endl;
          }
        }
      }
      mask.reset(n / 100);
    }
  }
  return sum;
}

static void solve_problem_43()
{
  const int multi[] = { 2, 3, 5, 7, 11, 13, 17 };

  int64_t sum = 0;
  const int k = 6;
  for (int n = multi[k]; n < 1000; n += multi[k])
  {
    int d1 = n % 10;
    int d2 = (n / 10) % 10;
    int d3 = (n / 100) % 10;
    if (d1 == d2 || d2 == d3 || d1 == d3)
    {
      continue;
    }

    std::bitset<10> mask;
    mask.set(d1);
    mask.set(d2);
    mask.set(d3);
    int number[10] = {-1};
    number[9] = d1;
    number[8] = d2;
    number[7] = d3;
    int64_t x = search(multi, 5, n / 10, number, mask);
    sum += x;
  }
  std::cout << sum << std::endl;
}
