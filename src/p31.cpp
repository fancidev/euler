/**
 * In England the currency is made up of pound, £, and pence, p, and there
 * are eight coins in general circulation:
 *
 *   1p, 2p, 5p, 10p, 20p, 50p, £1 (100p) and £2 (200p).
 *
 * It is possible to make £2 in the following way:
 *
 *   1*£1 + 1*50p + 2*20p + 1*5p + 1*2p + 3*1p
 * 
 * How many different ways can £2 be made using any number of coins?
 *
 * ANSWER: 73682.
 */

#include <iostream>
#include "euler.h"

BEGIN_PROBLEM(31, solve_problem_31)
  PROBLEM_TITLE("Coin sums")
  PROBLEM_ANSWER("73682")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(2)
  PROBLEM_TIME_COMPLEXITY("?")
  PROBLEM_SPACE_COMPLEXITY("?")
END_PROBLEM()

static const int units[] = { 1, 2, 5, 10, 20, 50, 100, 200 };

static int number_of_ways(int sum, int max_unit)
{
  if (max_unit == 0)
  {
    return 1;
  }

  int ways = 0;
  for (int partial_sum = 0; partial_sum <= sum; partial_sum += units[max_unit])
  {
    ways += number_of_ways(sum - partial_sum, max_unit - 1);
  }
  return ways;
}

static void solve_problem_31()
{
  int ways = number_of_ways(200, sizeof(units)/sizeof(units[0])-1);
  std::cout << ways << std::endl;
}
