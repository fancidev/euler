/**
 * A permutation is an ordered arrangement of objects. For example, 3124 is one
 * possible permutation of the digits 1, 2, 3 and 4. If all of the permutations
 * are listed numerically or alphabetically, we call it lexicographic order.
 * The lexicographic permutations of 0, 1 and 2 are:
 *
 *   012   021   102   120   201   210
 *
 * What is the millionth lexicographic permutation of the digits 
 * 0, 1, 2, 3, 4, 5, 6, 7, 8 and 9?
 */

#include <iostream>
#include <utility>
#include "euler.h"

BEGIN_PROBLEM(24, solve_problem_24)
  PROBLEM_TITLE("Lexicographic permutations")
  PROBLEM_ANSWER("2783915460")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("lg(N)")
  PROBLEM_SPACE_COMPLEXITY("1")
END_PROBLEM()

template <typename Iterator>
bool nth_permutation(Iterator first, Iterator last, int n)
{
  const int max_fact = 12;
  static const int fact[max_fact+1] = { 1, 1, 2, 6, 24, 120, 720, 5040, 40320, 
    362880, 3628800, 39916800, 479001600 };
  if (n >= fact[max_fact])
  {
    return false;
  }

  int len = static_cast<int>(last - first);
  if (len > max_fact)
  {
    first += (max_fact - len);
    len = max_fact;
  }

  for (int k = len; k >= 1; k--)
  {
    int i = n / fact[k-1];
    for (int j = i; j > 0; j--)
    {
      std::swap(first[j], first[j-1]);
    }
    n %= fact[k-1];
    ++first;
  }

  return true;
}

static void solve_problem_24()
{
  char digits[] = "0123456789";
  
  nth_permutation(digits+0, digits+sizeof(digits)-1, 999999);
  std::cout << digits << std::endl;
}
