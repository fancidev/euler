/**
 * Consider the consecutive primes p = 19 and q = 23. It can be verified that
 * 1219 is the smallest positive integer divisible by q whose last digits are
 * formed by p.
 *
 * In fact, with the exception of (p, q) = (3, 5), for every pair of
 * consecutive primes (p, q), p < q, there exist the smallest positive integer
 * n divisible by q whose last digits are formed by p. Denote this value of n
 * by S(p, q).
 *
 * Find Σ S(p, q) for all pairs of consecutive primes (p, q) where
 * 5 ≤ p ≤ 1000000.
 *
 * SOLUTION:
 *
 * For n's digits to end with p, n can be written in the form n = m * a + p
 * where m is the least power of 10 that is greater than p and a is a positive
 * integer. The requirement that n is divisible by q may then be written as
 *
 *   m * a + p ≡ 0  (mod q)
 *
 * This linear congruence has a unique solution 0 < a < q if m is coprime to
 * q. Since m = 10^k, it has only prime factors 2 and 5; and since q ≥ 3, the
 * only q to which m is not coprime is 5. For all other (p, q) pairs, the
 * congruence can be solved readily.
 */

#include <cstdint>
#include <iostream>
#include "euler/prime_test.hpp"
#include "euler/modular.hpp"
#include "euler.h"

BEGIN_PROBLEM(134, solve_problem_134)
  PROBLEM_TITLE("Prime pair connection")
  PROBLEM_ANSWER("18613426663617118")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
  PROBLEM_KEYWORDS("modular")
END_PROBLEM()

static void solve_problem_134()
{
  int p = 5;
  int m = 10;
  int64_t S = 0;
  while (p <= 1000000)
  {
    const int q = euler::next_prime(p);
    const int a = euler::modmul(
        euler::mod(-p, q), euler::modinv(euler::mod(m, q), q), q);
    const int64_t n = static_cast<int64_t>(m) * a + p;
    S += n;
    p = q;
    if (p > m)
    {
      m *= 10;
    }
  }
  std::cout << S << std::endl;
}
