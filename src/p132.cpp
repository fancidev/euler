// See p132.tex for solution

#include <iostream>
#include "euler/gcd.hpp"
#include "euler/prime_test.hpp"
#include "euler/modular.hpp"
#include "euler.h"

BEGIN_PROBLEM(132, solve_problem_132)
  PROBLEM_TITLE("Prime factors of a large repunit")
  PROBLEM_ANSWER("843296")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(3)
  PROBLEM_TIME_COMPLEXITY("P*(log(P))^2")
  PROBLEM_SPACE_COMPLEXITY("1")
  PROBLEM_KEYWORDS("repunit,factorization")
END_PROBLEM()

// Returns the sum of the first m prime factors of a repunit of n digits.
static unsigned int sum_repunit_prime_factors(unsigned int n, unsigned int m)
{
  if (m == 0)
  {
    return 0;
  }

  unsigned int sum = 0;
  if (n % 3 == 0)
  {
    --m;
    sum += 3;
    if (verbose())
    {
      std::cout << 3 << " ";
    }
  }

  for (unsigned int p = 7; m > 0; p = euler::next_prime(p))
  {
    if (euler::modpow(euler::mod(10, p), euler::gcd(n,p-1), p) == 1)
    {
      sum += p;
      --m;
      if (verbose())
      {
        std::cout << p << " ";
      }
    }
  }

  if (verbose())
  {
    std::cout << std::endl;
  }

  return sum;
}

static void solve_problem_132()
{
#if 0
  std::cout << sum_repunit_prime_factors(10, 4) << std::endl;
#else
  std::cout << sum_repunit_prime_factors(1000000000, 40) << std::endl;
#endif
}
