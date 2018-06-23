/**
 * The number 145 is well known for the property that the sum of the factorial
 * of its digits is equal to 145:
 *
 *   1! + 4! + 5! = 1 + 24 + 120 = 145
 *
 * Perhaps less well known is 169, in that it produces the longest chain of
 * numbers that link back to 169; it turns out that there are only three such
 * loops that exist:
 *
 *   169 -> 363601 -> 1454 -> 169
 *   871 -> 45361 -> 871
 *   872 -> 45362 -> 872
 *
 * It is not difficult to prove that EVERY starting number will eventually
 * get stuck in a loop. For example,
 *
 *   69 -> 363600 -> 1454 -> 169 -> 363601 (-> 1454)
 *   78 -> 45360 -> 871 -> 45361 (-> 871)
 *   540 -> 145 (-> 145)
 *
 * Starting with 69 produces a chain of five non-repeating terms, but the
 * longest non-repeating chain with a starting number below one million is
 * sixty terms.
 *
 * How many chains, with a starting number below one million, contain exactly
 * sixty non-repeating terms?
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include "euler/digits.hpp"
#include "euler.h"

BEGIN_PROBLEM(74, solve_problem_74)
  PROBLEM_TITLE("Factorial chains with exactly sixty non-repeating terms")
  PROBLEM_ANSWER("402")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("N")
  PROBLEM_SPACE_COMPLEXITY("N")
  PROBLEM_KEYWORDS("digits")
END_PROBLEM()

// Returns the factorial of n.
static int factorial(int n)
{
  static const int fact[10] = {
    1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880
  };
  return fact[n];
}

// Returns the sum of the factorial of the digits in n.
static int sum_digit_factorial(int n)
{
  int sum = 0;
  for (int d: euler::rdigits<10>(n))
  {
    sum += factorial(d);
  }
  return sum;
}

// Prints the factorial chain of a starting number.
static void print_factorial_chain(int start)
{
  std::vector<int> chain;
  int n = start;
  do
  {
    chain.push_back(n);
    std::cout << n << " -> ";
    n = sum_digit_factorial(n);
  } while (std::find(chain.cbegin(), chain.cend(), n) == chain.cend());
  std::cout << "(" << n << ")" << std::endl;
}

// Cache of factorial chain length for a range of numbers.
class factorial_chain
{
  std::vector<int> length; // Cache of known chain length of starting numbers
  std::vector<int> chain;  // Current chain being computed. This variable is
                           // declared here to reduce memory reallocations.

public:

  factorial_chain(size_t cache_size) : length(cache_size+1) { }

  int get_chain_length(int start)
  {
    if (length[start] != 0)
    {
      return length[start];
    }

    chain.resize(0);

    // Compute the chain and mark the length of each term as -1 to detect cycle.
    int n = start;
    while (length[n] == 0)
    {
      chain.push_back(n);
      length[n] = -1;
      n = sum_digit_factorial(n);
    }

    // If a loop is found, mark each element in the loop with the loop size,
    // and then truncate the chain before the start of the loop.
    if (length[n] == -1)
    {
      auto loop_begin = std::find(chain.cbegin(), chain.cend(), n);
      int loop_length = static_cast<int>(chain.cend() - loop_begin);
      std::for_each(loop_begin, chain.cend(), [&](int k) {
        length[k] = loop_length;
      });
      chain.resize(loop_begin - chain.cbegin());
    }

    // Update the length cache of all elements before the loop.
    int L = length[n];
    std::for_each(chain.crbegin(), chain.crend(), [&](int k) {
      length[k] = ++L;
    });
    return L;
  }
};

static void solve_problem_74()
{
  factorial_chain chain(7*factorial(9));

  // Run some tests.
  if (verbose())
  {
    int tests[] = {169, 78, 540, 145, 69, 871, 872};
    for (int start: tests)
    {
      print_factorial_chain(start);
      std::cout << "Length: " << chain.get_chain_length(start) << std::endl;
    }
  }

  // Compute the chain length of all numbers below 10^6.
  int count = 0;
  for (int n = 1; n < 1000000; n++)
  {
    if (chain.get_chain_length(n) == 60)
    {
      ++count;
    }
  }
  std::cout << count << std::endl;
}
