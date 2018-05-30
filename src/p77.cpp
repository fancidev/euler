/**
 * It is possible to write ten as the sum of primes in exactly five different
 * ways:
 *
 *   7 + 3
 *   5 + 5
 *   5 + 3 + 2
 *   3 + 3 + 2 + 2
 *   2 + 2 + 2 + 2 + 2
 *
 * What is the first value which can be written as the sum of primes in over
 * five thousand different ways?
 */

#include <iostream>
#include <vector>
#include "euler/prime_table.hpp"
#include "euler.h"

BEGIN_PROBLEM(77, solve_problem_77)
  PROBLEM_TITLE("Prime summation")
  PROBLEM_ANSWER("71")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
  PROBLEM_KEYWORDS("combinatorics,partition")
END_PROBLEM()

// f(n, k) = number of ways to write n as sum of primes at or after the k-th
// index in table
static int f(int n, int k, const std::vector<int> &primes)
{
  if (n == 0)
  {
    return 1;
  }

  int p = primes[k];
  if (n < p)
  {
    return 0;
  }

  int count = 0;
  for (int s = 0; s <= n; s += p)
  {
    count += f(n - s, k + 1, primes);
  }
  return count;
}

static void solve_problem_77()
{
  euler::prime_table<int> prime_table(100); // arbitrary limit
  std::vector<int> primes(prime_table.begin(), prime_table.end());

  for (int n = 2; n < 100; n++)
  {
    int count = f(n, 0, primes);
    if (verbose())
    {
      std::cout << "f(" << n << ") = " << count << std::endl;
    }
    if (count > 5000)
    {
      std::cout << n << std::endl;
      break;
    }
  }
}
