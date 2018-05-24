/**
 * A riffle shuffle is executed as follows: a deck of cards is split into two
 * equal halves, with the top half taken in the left hand and the bottom half
 * taken in the right hand. Next, the cards are interleaved exactly, with the
 * top card in the right half inserted just after the top card in the left
 * half, the 2nd card in the right half just after the 2nd card in the left
 * half, etc. (Note that this process preserves the location of the top and
 * bottom card of the deck.)
 *
 * Let S(n) be the minimum number of consecutive riffle shuffles needed to
 * restore a deck of size n to its original configuration, where n is a
 * positive even number.
 *
 * Amazingly, a standard deck of 52 cards will first return to its original
 * configuration after only 8 perfect shuffles, so S(52) = 8. It can be
 * verified that a deck of 86 cards will also return to its original
 * configuration after exactly 8 shuffles, and the sum of all values of n that
 * satisfy S(n) = 8 is 412.
 *
 * Find the sum of all values of n that satisfy S(n) = 60.
 *
 * SOLUTION:
 *
 * Given a deck of n cards (n being positive and even), number the cards from
 * top to bottom by 0 to (n - 1). Hence the upper half contains cards numbered
 * from 0 to (n / 2 - 1), and the lower half contains cards numbered from
 * (n / 2) to (n - 1).
 *
 * The perfect shuffle procedure inserts each card in the lower half exactly
 * beneath each card in the upper half. Let P(k) denote the position after
 * shuffle of the card that was in position k before shuffle. It is easy to
 * verify that for cards in the upper half, i.e. 0 <= k <= n / 2 - 1,
 *
 *   P(k) = 2 * k
 *
 * And for cards in the lower half, i.e. n / 2 <= k <= n - 1,
 *
 *   P(k) = (k - n / 2) * 2 + 1 = 2 * k - n + 1
 *
 * Note that P(0) = 0 and P(n - 1) = n - 1, which confirms that the top card
 * and the bottom card do not change position after shuffle. Now ignore the
 * bottom card and only consider k in the range [0, n - 2]. It is easy to
 * verify that for 0 <= k <= n - 2, the above two equations can be combined
 * into
 *
 *   P(k) = (2 * k) mod (n - 1)
 *
 * After m perfect shuffles, the final position of the card that was initially
 * in position k becomes
 *
 *   P^m(k) = (2^m * k) mod (n - 1)
 *
 * For the final position to be equal to the initial position, we must have
 * P^m(k) = k, which gives
 *
 *   2^m * k == k   (mod n - 1)
 *
 * or
 *
 *   (2^m - 1) * k == 0   (mod n - 1)
 *
 * For the entire deck to restore to its initial configuration after m perfect
 * shuffles, the above equation must hold for all k in [0, n - 2]; in
 * particular, it must hold for k = n - 2, which is coprime to (n - 1). This
 * requires
 *
 *   2^m - 1 == 0   (mod n - 1)
 *
 * That is, (2^m - 1) is a multiple of (n - 1). This can be verified for a
 * deck of 52 cards, as
 *
 *   2^8 - 1 = 255 = 5 * (52 - 1)
 *
 * Conversely, it is obvious that (2^m - 1) being a multiple of (n - 1) is a
 * sufficient condition for the deck to return to its original configuration
 * after m perfect shuffles.
 *
 * Hence to compute S(n), we need to find the smallest m such that (2^m - 1)
 * is a multiple of (n - 1). And to find all n such that S(n) = m, we need to
 * find all divisors d of (2^m - 1) such that d does not divide any (2^l - 1)
 * for l < m.
 */

#include <iostream>
#include <cstdint>
#include "euler/divisor.hpp"
#include "euler.h"

BEGIN_PROBLEM(622, solve_problem_622)
  PROBLEM_TITLE("Find the unique Pythagorean triplet that sums to 1000")
  PROBLEM_ANSWER("3010983666182123972")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(3)
  PROBLEM_TIME_COMPLEXITY("2^m")
  PROBLEM_SPACE_COMPLEXITY("m")
  PROBLEM_KEYWORDS("divisor,modular")
END_PROBLEM()

static void solve_problem_622()
{
#if 0
  const int m = 8;
#else
  const int m = 60;
#endif

  const int64_t p = (1ULL << m) - 1;
  int64_t sum = 0;
  for (int64_t d: euler::divisors(p))
  {
    // Check that d does not divide any smaller (2^l - 1)
    bool ok = true;
    for (int64_t q = p >> 1; q > 0; q >>= 1)
    {
      ok = ok && (q % d != 0);
    }
    if (ok)
    {
      if (verbose())
      {
        std::cout << d << std::endl;
      }
      sum += (d + 1);
    }
  }
  std::cout << sum << std::endl;
}
