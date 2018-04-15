#include <iostream>
#include <iomanip>
#include "euler/prime_test.hpp"
#include "euler.h"

BEGIN_PROBLEM(58, solve_problem_58)
  PROBLEM_TITLE("Number of primes on the diagonals of a spiral grid")
  PROBLEM_ANSWER("26241")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("N*log(N)")
  PROBLEM_SPACE_COMPLEXITY("1")
  PROBLEM_KEYWORDS("prime")
END_PROBLEM()

static void solve_problem_58()
{
  if (verbose())
  {
    std::cout << "Side        N       P        %" << std::endl;
  }

  int total_prime = 0;
  for (int i = 1; ; i++)
  {
    int side = (2*i-1);
    int d1 = side*side - (side-1);
    int d2 = d1 - (side-1);
    int d3 = d2 - (side-1);
    int n = 2*side-1;
    if (euler::is_prime(d1)) { ++total_prime; }
    if (euler::is_prime(d2)) { ++total_prime; }
    if (euler::is_prime(d3)) { ++total_prime; }
    float prop = static_cast<float>(total_prime) / n;
    if (i > 10 && prop < 0.1)
    {
      if (verbose())
      {
        std::cout << std::setw(4) << side
          << std::setw(8) << n
          << std::setw(8) << total_prime
          << std::setw(8) << (prop*100) << "%" << std::endl;
      }
      else
      {
        std::cout << side << std::endl;
      }
      break;
    }
  }
}
