/**
 * An unbiased coin is tossed repeatedly until two consecutive heads are
 * obtained. Suppose these occur on the (M−1)th and Mth toss. Let P(n) be the
 * probability that M is divisible by n. For example, the outcomes HH, HTHH,
 * and THTTHH all count towards P(2), but THH and HTTHH do not.
 *
 * You are given that P(2) = 3/5 and P(3) = 9/31. Indeed, it can be shown that
 * P(n) is always a rational number.
 *
 * For a prime p and a fully reduced fraction a/b, define Q(a/b, p) to be the
 * smallest positive q for which a ≡ b * q (mod p).
 *
 * For example Q(P(2), 109) = Q(35, 109) = 66, because 5 * 66 = 330 ≡ 3 (mod
 * 109) and 66 is the smallest positive such number. Similarly
 * Q(P(3), 109) = 46.
 *
 * Find Q(P(10^18), 1000000009).
 *
 * Solution:
 *
 * Let
 *
 *   H(n) = Prob(game not over after n tosses and the n-th toss is HEAD)
 *   T(n) = Prob(game not over after n tosses and the n-th toss is TAIL)
 *   L(n) = Prob(game over right after n tosses)
 *
 * It is easy to show that
 *
 *   H(n) = T(n-1) * (1/2)
 *   T(n) = [H(n-1) + T(n-1)] * (1/2)
 *   L(n) = H(n-1) * (1/2)
 *
 * Rearranging terms, we find a recurrence relation for L(n)
 *
 *   L(n) = (1/2)*L(n-1) + (1/4)*L(n-2)
 *
 * with initial values
 *
 *   L(1) = 0, L(2) = 1/4.
 *
 * We can write the recurrence relation in the usual matrix form
 *
 *   /        \   /          \   /        \
 *   |  L(n)  |   | 1/2  1/4 |   | L(n-1) |
 *   |        | = |          | * |        |
 *   | L(n-1) |   |  1    0  |   | L(n-2) |
 *   \        /   \          /   \        /
 *
 * Denote the coefficient matrix by
 *
 *       /          \         /      \
 *       | 1/2  1/4 |    1    | 2  1 |    1
 *   A = |          | = --- * |      | = --- * A'
 *       |  1    0  |    4    | 4  0 |    4
 *       \          /         \      /
 *
 * By continuing the recurrence relation, we find
 *
 *   /        \            /      \            /     \
 *   |  L(n)  |    (n-2)   | L(2) |    (n-2)   | 1/4 |
 *   |        | = A      * |      | = A      * |     |
 *   | L(n-1) |            | L(1) |            |  0  |
 *   \        /            \      /            \     /
 *
 * Next, observe that
 *
 *                                          ∞
 *   P(n) = L(n) + L(2*n) + L(3*n) + ... =  Σ  L(k*n)
 *                                         k=1
 *
 * It is convenient to write the above equation in matrix form as
 *
 *   /      \   /            \                  /     \
 *   | P(n) |   |  Σ L(k*n)  |    ∞   (k*n-2)   | 1/4 |
 *   |      | = |            | =  Σ  A        * |     |
 *   |  .   |   | Σ L(k*n-1) |   k=1            |  0  |
 *   \      /   \            /   ------------   \     /
 *
 * It can be computed that A's eigenvalues are (1 ± √5) / 4. Therefore the
 * above underlined series converge and the sum is equal to
 *
 *    ∞   (k*n-2)    ∞       k  (-2)    n            (-1)    (-2)
 *    Σ  A        =  Σ  (A^n)  A     = A  * (I - A^n)     * A
 *   k=1            k=1
 *
 * where I is the 2-by-2 identity matrix. Substituting this back into the
 * previous equation and reducing constant terms yields
 *
 *   /      \                         /    \
 *   | P(n) |     n            (-1)   |  1 |
 *   |      | =  A  * (I - A^n)     * |    |
 *   |  .   |                         | -2 |
 *   \      /                         \    /
 *
 *                  n    /   n           n  \(-1)   /    \
 *                A'     |  4  * I     A'   |       |  1 |
 *            =  ----- * | -------- - ----- |     * |    |
 *                 n     |     n        n   |       | -2 |
 *                4      \    4        4    /       \    /
 *
 *                    /              \(-1)   /    \
 *                n   |  n         n |       |  1 |
 *            = A'  * | 4  * I - A'  |     * |    |
 *                    |              |       | -2 |
 *                    \              /       \    /
 *
 * Computing the right-hand side modulo p gives the answer to this problem.
 */

#include <cstdint>
#include <iostream>
#include "euler/matrix.hpp"
#include "euler/residue.hpp"
#include "euler.h"

BEGIN_PROBLEM(624, solve_problem_624)
  PROBLEM_TITLE("Two heads are better than one")
  PROBLEM_ANSWER("984524441")
  PROBLEM_DIFFICULTY(2)
  PROBLEM_FUN_LEVEL(2)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
  PROBLEM_KEYWORDS("probability,recurrence,modular")
END_PROBLEM()

static void solve_problem_624()
{
  const int64_t n = 1000000000LL * 1000000000LL;
  const int p = 1000000009;

  using R = euler::residue<int, p>;
  using M = euler::matrix<R, 2, 2>;
  using V = euler::matrix<R, 2, 1>;

  M A = { { 2, 1 }, { 4, 0 } };
  M I = M::identity();
  M An = A ^ n;
  V P = An * euler::inv((R(4) ^ n) * I - An) * V{1, -2};
  std::cout << P(0, 0) << std::endl;
}
