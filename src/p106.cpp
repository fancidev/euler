/**
 * Let S(A) represent the sum of elements in set A of size n. We shall call it
 * a special sum set if for any two non-empty disjoint subsets, B and C, the
 * following properties are true:
 *
 *   (1) S(B) != S(C); that is, sums of subsets cannot be equal.
 *
 *   (2) If B contains more elements than C then S(B) > S(C).
 *
 * For this problem we shall assume that a given set contains n strictly
 * increasing elements and it already satisfies the second rule.
 *
 * Surprisingly, out of the 25 possible subset pairs that can be obtained from
 * a set for which n = 4, only 1 of these pairs need to be tested for equality
 * (first rule). Similarly, when n = 7, only 70 out of the 966 subset pairs
 * need to be tested.
 *
 * For n = 12, how many of the 261625 subset pairs that can be obtained need
 * to be tested for equality?
 *
 * NOTE: This problem is related to problems 103 and 105.
 *
 * SOLUTION:
 *
 * Rule (2) guarantees that subsets of different size have different sums.
 * Therefore to test rule (1), we only need to compare pairs of (disjoint)
 * subsets of equal size.
 *
 * Let (U, V) be an unordered pair of non-empty, equal-sized, disjoint subsets
 * of A. That is,
 *
 *   U ⊆ A, V ⊆ A, U ∩ V = ∅, |U| = |V| = k ≥ 1.
 *
 * To count the number of such (U, V) pairs, we classify them by k: we first
 * count pairs of subsets with one element, then pairs of subsets with two
 * elements, and so on.
 *
 * Let g(n,k) denote the number of such (U, V) pairs where |U| = |V| = k ≥ 1.
 * There are C(n,2*k) ways to pick (U ∪ V) which contains 2*k elements. For
 * each pick, there are C(2*k,k) ways to pick U; since the pair is unordered,
 * half of these picks are duplicate. Putting these together yields
 *
 *             1
 *   g(n,k) = --- * C(n,2*k) * C(2*k,k)
 *             2
 *
 *             1             n!             (2*k)!
 *          = --- * ------------------- * ---------
 *             2     (2*k)! * (n-2*k)!     k! * k!
 *
 *             1             n!
 *          = --- * -------------------
 *             2     (n-2*k)! * (k!)^2
 *
 * For computation it is convenient to write g(n,k) recursively as
 *
 *             n*(n-1)
 *   g(n,1) = ---------
 *                2
 *
 *                        (n-2*k+1)*(n-2*k+2)
 *   g(n,k) = g(n,k-1) * ---------------------
 *                                k^2
 *
 * The total number of such (U, V) pairs is equal to
 *
 *          ⌊n/2⌋
 *   g(n) =   Σ   g(n,k)
 *           k=1
 *
 * Next, observe that among non-empty, equal-sized, disjoint subset pairs
 * (U, V), there is one class that we need not compare, which are pairs that
 * are "elementwise ordered". Given
 *
 *   U = { u[1], ..., u[l] }
 *
 *   V = { v[1], ..., v[l] }
 *
 * U and V are called "elementwise ordered" if u[1] < v[1], u[2] < v[2], ...,
 * u[l] < v[l]. If U and V are elementwise ordered, then S(U) < S(V) and we
 * need not compare their sums explicitly.
 *
 * To count the number of (U, V) pairs that are elementwise ordered, suppose
 * without loss of generality that A = { 1, 2, ..., N }. Let f(m,n), m ≤ n
 * denote the number of ways to pick (U, V) such that
 *
 *   U ⊆ { 1, ..., m }
 *
 *   V ⊆ { 1, ..., m } ∪ { m+1, ..., n }
 *
 *   U ∩ V = ∅, |U| = |V| ≥ 0, U and V are elementwise ordered
 *
 * Note that the empty set pair (∅, ∅) is allowed in the definition of f(m,n).
 * The answer of interest is f(N,N).
 *
 * To compute f(m,n), we construct U and V backwards from their last element,
 * u' and v', of which u' must be smaller than v' to ensure elementwise order.
 * Either u' = m or u' < m. If u' < m, we continue constructing the earlier
 * elements of U by exluding m from its candidates; no additional restriction
 * is applied to V. The number of ways in this case is
 *
 *   f(m-1,n)
 *
 * If u' = m < v', then v' ∈ { m+1, ..., n }. For each choice of v', we
 * continue constructing the earlier elements by restricting candidates for U
 * before m and candidates for V before v' and excluding m. The number of ways
 * in this case is
 *
 *     n
 *     Σ   f(m-1,k-2)
 *   k=m+1
 *
 * Putting both cases together, we find
 *
 *                         n
 *   f(m,n) = f(m-1,n) +   Σ   f(m-1,k-2)
 *                       k=m+1
 *
 * The boundary conditions are f(0,*) = 1, which corresponds to the empty set
 * pair (∅, ∅).
 *
 * Finally, the number of pairs that need to be compared is equal to
 *
 *   g(N) - f(N,N) + 1
 *
 * where the last term corrects for the empty set pair.
 */

#include <iostream>
#include <iomanip>
#include "euler.h"

BEGIN_PROBLEM(106, solve_problem_106)
  PROBLEM_TITLE("Special subset sums: meta-testing")
  PROBLEM_ANSWER("21384")
  PROBLEM_DIFFICULTY(2)
  PROBLEM_FUN_LEVEL(2)
  PROBLEM_TIME_COMPLEXITY("N^3")
  PROBLEM_SPACE_COMPLEXITY("N^2")
END_PROBLEM()

// Compute g(n), the number of unordered pairs of non-empty, equal-sized,
// disjoint subsets (U, V) of set A with n elements.
static int g(int n)
{
  int g_n_k = n*(n-1)/2;
  int g_n = g_n_k;
  for (int k = 2; k <= n / 2; k++)
  {
    g_n_k = g_n_k*(n-2*k+1)*(n-2*k+2)/(k*k);
    g_n += g_n_k;
  }
  return g_n;
}

static void solve_problem_106()
{
#if 0
  const int N = 7;
#else
  const int N = 12;
#endif

  // Compute f(m,n)
  int f[N+1][N+1];
  for (int n = 0; n <= N; n++)
  {
    f[0][n] = 1;
  }
  for (int m = 1; m <= N; m++)
  {
    for (int n = m; n <= N; n++)
    {
      f[m][n] = f[m-1][n];
      for (int k = m + 1; k <= n; k++)
      {
        f[m][n] += f[m-1][k-2];
      }
    }
  }

  std::cout << g(N) - f[N][N] + 1 << std::endl;
}
