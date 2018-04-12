/**
 * The arithmetic sequence, 1487, 4817, 8147, in which each of the terms
 * increases by 3330, is unusual in two ways: (i) each of the three terms are
 * prime, and, (ii) each of the 4-digit numbers are permutations of one
 * another.
 *
 * There are no arithmetic sequences made up of three 1-, 2-, or 3-digit
 * primes, exhibiting this property, but there is one other 4-digit increasing
 * sequence.
 *
 * What 12-digit number do you form by concatenating the three terms in this
 * sequence?
 *
 * ANSWER: 296962999629
 */

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include "euler/digits.hpp"
#include "euler/prime_table.hpp"
#include "euler.h"

BEGIN_PROBLEM(49, solve_problem_49)
  PROBLEM_TITLE("Prime permutations")
  PROBLEM_ANSWER("296962999629")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("?")
  PROBLEM_SPACE_COMPLEXITY("?")
END_PROBLEM()

static void search_triple(
  std::vector<std::pair<int, int>>::const_iterator begin,
  std::vector<std::pair<int, int>>::const_iterator end)
{
  for (auto it1 = begin; it1 != end; ++it1)
  {
    for (auto it2 = it1; ++it2 != end; )
    {
      for (auto it3 = it2; ++it3 != end; )
      {
        if (it2->second * 2 == it1->second+it3->second)
        {
          if (!(it1->second == 1487 && it2->second == 4817 && it3->second == 8147))
            std::cout << it1->second << it2->second << it3->second << std::endl;
        }
      }
    }
  }
}

static void solve_problem_49()
{
  // Find all four-digit primes.
  euler::prime_table<int> primes(10000);

  // Map each four-digit prime to it's canonical presentation.
  std::vector<std::pair<int, int>> map; // canonical -> number
  for (int p: primes)
  {
    if (p >= 1000)
    {
      map.push_back(std::pair<int, int>(euler::sort_digits<10>(p), p));
    }
  }

  // Sort the map
  std::sort(map.begin(), map.end());

  // Find increasing sequence
  for (auto it = map.cbegin(); it != map.cend(); )
  {
    auto it2 = it + 1;
    while (it2 != map.cend() && it2->first == it->first)
      ++it2;

    search_triple(it, it2);
    it = it2;
  }
}
