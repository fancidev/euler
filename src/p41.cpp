/**
 * We shall say that an n-digit number is pandigital if it makes use of 
 * all the digits 1 to n exactly once. For example, 2143 is a 4-digit 
 * pandigital and is also prime.
 *
 * What is the largest n-digit pandigital prime that exists?
 */

#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include "euler/digits.hpp"
#include "euler/prime_table.hpp"
#include "euler/prime_test.hpp"
#include "euler.h"

BEGIN_PROBLEM(41, solve_problem_41)
  PROBLEM_TITLE("Find the largest pandigital prime")
  PROBLEM_ANSWER("7652413")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("b^(2b)/ln(b)")
  PROBLEM_SPACE_COMPLEXITY("b^(b-1)/ln(b)")
  PROBLEM_KEYWORDS("pandigital,prime")
END_PROBLEM()

// Complexity analysis:
// Base: b
// Largest pandigital number: b^(b-1). Includes permutation complexity.
// Prime factor candidate range: sqrt(b^(b-1)) ~ b^(b/2)
// Prime factor candidate space: b^(b/2) / ln(b^(b/2)) ~ b^(b-1)/ln(b)
// Prime factor candidate generation: should be less than the iteration
// For each permutation,
//   Compute number from digits: b
//   Prime testing: b^(b-1)/ln(b)
//
// Overall,
// Time complexity = prime generation + #perm * each perm
//                 ~ x (ignored) + b^(b-1)*b^b/ln(b)
//                 ~ b^(2b)/ln(b)
// Space complexity = prime candidate storage ~ b^(b-1)/ln(b).

static void solve_problem_41()
{
  // Generate prime table to speed up prime testing.
  euler::prime_table<int> pp(3163); // 3163=sqrt(10^7)
  std::vector<int> primes(pp.begin(), pp.end());
  int result = 0;

  // Test each 1-n pandigital number, starting from the largest one.
  // Note that the sum of 1-9 is divisible by three, so there can't exist
  // 9-digit pandigital primes. In fact, only 7,4,1-digit pandigital primes
  // are possible
  for (int n = 7; n >= 1 && result == 0; n -= 3)
  {
    std::vector<int> digits(n);
    for (int i = 0; i < n; i++)
    {
      digits[i] = n - i;
    }
    do 
    {
      int number = euler::from_digits<int>(digits.cbegin(), digits.cend());
      if (euler::is_prime(number, primes.begin(), primes.end()))
      {
        result = number;
        break;
      }
    }
    while (std::next_permutation(digits.begin(), digits.end(), std::greater<int>()));
  }
  std::cout << result << std::endl;
}
