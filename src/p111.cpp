/**
 * There are nine 4-digit primes containing three ones:
 *
 *   1117, 1151, 1171, 1181, 1511, 1811, 2111, 4111, 8111
 *
 * We shall say that M(n, d) represents the maximum number of repeated digits
 * for an n-digit prime where d is the repeated digit, N(n, d) represents the
 * number of such primes, and S(n, d) represents the sum of these primes.
 *
 * So M(4, 1) = 3 is the maximum number of repeated digits for a 4-digit prime
 * where one is the repeated digit, there are N(4, 1) = 9 such primes, and the
 * sum of these primes is S(4, 1) = 22275.
 *
 * We obtain the following results for 4-digit primes.
 *
 *   -------------------------------
 *   Digit,d  M(4,d)  N(4,d)  S(4,d)
 *   -------------------------------
 *   0        2       13      67061
 *   1        3       9       22275
 *   2        3       1       2221
 *   3        3       12      46214
 *   4        3       2       8888
 *   5        3       1       5557
 *   6        3       1       6661
 *   7        3       9       57863
 *   8        3       1       8887
 *   9        3       7       48073
 *   -------------------------------
 *
 * For d = 0 to 9, the sum of all S(4, d) is 273700.
 *
 * Find the sum of all S(10, d).
 *
 * SOLUTION:
 *
 * Generating all 10-digit primes is too heavy in terms of time and memory.
 * Expecting that M(10, d) should be large, we enumerate all 10-digit numbers
 * with 10 repeats, 9 repeats, 8 repeats, etc of d, and test for primality.
 */

#include <cstdint>
#include <iostream>
#include <vector>
#include "euler/combination.hpp"
#include "euler/digits.hpp"
#include "euler/prime_test.hpp"
#include "euler.h"

BEGIN_PROBLEM(111, solve_problem_111)
  PROBLEM_TITLE("Primes with runs")
  PROBLEM_ANSWER("612407567715")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
  PROBLEM_KEYWORDS("digits,prime")
END_PROBLEM()

// Returns the sum of n-digit primes containing the maximum number of digit d.
static int64_t S(int n, int d)
{
  // Build a list of the possible digits in each free place.
  // - d cannot appear in any free place;
  // - the first digit cannot be zero;
  // - the last digit cannot be even.
  std::vector<std::vector<int>> allowed(n);
  for (int i = 0; i < n; i++)
  {
    for (int k = 0; k <= 9; k++)
    {
      if (k == d)
        continue;
      if (i == 0 && k == 0)
        continue;
      if (i == n - 1 && k % 2 == 0)
        continue;
      allowed[i].push_back(k);
    }
  }

  // Allocate a sequence of n digits.
  std::vector<int> digits(n);

  // Let f be the number of free positions to fill in digits <> d. Start with
  // f = 0, i.e. n repeating d. Then try f = 1, i.e. (n - 1) repeating d.
  // Continue until we find a prime.
  for (int f = 0; f < n; f++)
  {
    // Let s be the sum of primes with (n - f) repeating d.
    int64_t s = 0;

    // Iterate each possible choice of f free places out of n places.
    for (const std::vector<size_t> &free: euler::choose(n, f))
    {
      // The most significant position must be free if d == 0.
      if (d == 0 && (f == 0 || free[0] != 0))
      {
        // ?
        continue;
      }

      // Set all digits to d.
      std::fill(digits.begin(), digits.end(), d);

      // Generate each possible digit tuple in the free places.
      std::vector<size_t> free_count(f);
      for (int i = 0; i < f; i++)
      {
        free_count[i] = allowed[free[i]].size();
      }

      for (const std::vector<size_t> &free_choice:
           euler::cartesian(free_count.begin(), free_count.end()))
      {
        // Construct the number.
        for (int i = 0; i < f; ++i)
        {
          digits[free[i]] = allowed[free[i]][free_choice[i]];
        }

        // Test whether the number is prime.
        int64_t v = euler::from_digits<int64_t>(digits.begin(), digits.end());
        if (euler::is_prime(v))
        {
          if (verbose())
          {
            std::cout << "Prime: " << v << std::endl;
          }
          s += v;
        }

      } // next free

    } // next f

    // Return if at least one such prime is found.
    if (s > 0)
    {
      return s;
    }
  }

  // Not found.
  return 0;
}

static void solve_problem_111()
{
#if 0
  const int Digits = 4;
#else
  const int Digits = 10;
#endif

  int64_t sum = 0;
  for (int d = 0; d <= 9; d++)
  {
    sum += S(Digits, d);
  }
  std::cout << sum << std::endl;
}
