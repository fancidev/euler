// See p133.tex for solution

#include <iostream>
#include "euler/prime_test.hpp"
#include "euler/modular.hpp"
#include "euler.h"

BEGIN_PROBLEM(133, solve_problem_133)
  PROBLEM_TITLE("Repunit nonfactors")
  PROBLEM_ANSWER("453647705")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(3)
  PROBLEM_TIME_COMPLEXITY("M*ln(M)")
  PROBLEM_SPACE_COMPLEXITY("1")
  PROBLEM_KEYWORDS("repunit,prime factor")
END_PROBLEM()

// Returns the sum of prime factors p <= M such that p does not divide any
// repunit of 10^k digits.
static unsigned int sum_repunit_prime_nonfactors(unsigned int M)
{
  if (verbose())
  {
    std::cout << "Prime divisors of R(10^k):";
  }

  unsigned int sum = 2+3+5;
  for (unsigned int p = 7; p <= M; p = euler::next_prime(p))
  {
    // Factorize p-1 = (2^r * 5^s) * d = x * d
    unsigned int n = p - 1, x = 1;
    while (n % 2 == 0)
    {
      n /= 2;
      x *= 2;
    }
    while (n % 5 == 0)
    {
      n /= 5;
      x *= 5;
    }

    // Check whether 10^x == 1 (mod p).
    if (!(euler::modpow(euler::mod(10, p), x, p) == 1))
    {
      sum += p;
    }
    else if (verbose())
    {
      std::cout << " " << p;
    }
  }

  if (verbose())
  {
    std::cout << std::endl;
  }

  return sum;
}

static void solve_problem_133()
{
#if 0
  std::cout << sum_repunit_prime_nonfactors(100) << std::endl;
#else
  std::cout << sum_repunit_prime_nonfactors(100000) << std::endl;
#endif
}
