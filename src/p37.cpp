/**
 * The number 3797 has an interesting property. Being prime itself, it is 
 * possible to continuously remove digits from left to right, and remain 
 * prime at each stage: 3797, 797, 97, and 7. Similarly we can work from 
 * right to left: 3797, 379, 37, and 3.
 *
 * Find the sum of the only eleven primes that are both truncatable from 
 * left to right and right to left.
 * 
 * NOTE: 2, 3, 5, and 7 are not considered to be truncatable primes.
 *
 * ANSWER: 748317.
 */

#include <iostream>
#include <numeric>
#include <vector>
#include "euler/prime_test.hpp"
#include "euler.h"

BEGIN_PROBLEM(37, solve_problem_37)
  PROBLEM_TITLE("Truncatable primes")
  PROBLEM_ANSWER("748317")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(2)
  PROBLEM_TIME_COMPLEXITY("?")
  PROBLEM_SPACE_COMPLEXITY("?")
END_PROBLEM()

static std::vector<int> truncatable_primes;

// Check whether a prime number n is still a prime when removing
// one digit from the left iteratively.
static bool is_truncatable_from_left(int n)
{
  int m = 1000000000;
  for (int prev = n; (n %= m) > 0; m /= 10, prev = n)
  {
    if (n != prev)
    {
      if (!euler::is_prime(n))
      {
        return false;
      }
    }
  }
  return true;
}

// Try append digits to a prime number n to make it still a prime
// when truncated iteratively both ways.
static void try_append_digit(int n)
{
  static const int digits[] = { 1, 3, 7, 9 };
  for (int d: digits)
  {
    int k = n * 10 + d;
    if (euler::is_prime(k))
    {
      if (is_truncatable_from_left(k))
      {
        truncatable_primes.push_back(k);
      }
      try_append_digit(k);
    }
  }
}

static void solve_problem_37()
{
  try_append_digit(2);
  try_append_digit(3);
  try_append_digit(5);
  try_append_digit(7);

  int sum = std::accumulate(truncatable_primes.cbegin(), truncatable_primes.cend(), 0);
  std::cout << sum << std::endl;
}
