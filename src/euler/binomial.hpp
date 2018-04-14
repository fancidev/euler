/**
 * @defgroup Binomial Binomial Coefficient
 * @ingroup Library
 */

#ifndef EULER_BINOMIAL_H
#define EULER_BINOMIAL_H

#include "modular.hpp"
#include <algorithm>
#include <limits>
#include <vector>
#include <cassert>

namespace euler {

/**
 * Stores the binomial coefficients <code>C(n,k)</code> for all @c n below
 * a given threshold.
 *
 * @ingroup Binomial
 */
template <typename T, typename TArg=int>
class binom_table
{
  TArg max_n;
  std::vector<T> _values;

public:

  /**
   * Constructs a table that stores the binomial coefficients
   * <code>C(n,k)</code> for all <code>0 ≤ k ≤ n ≤ N</code>,
   * where @c N is the given threshold of the table.
   *
   * @param N Threshold of the binomial coefficient table. In practice,
   *      useful values of @c N is restricted by the maximum integer
   *      that can be represented by @c T. Specifically, @c N should not
   *      be greater than the number of bits in @c T.
   *
   * @timecomplexity <code>O(N^2)</code>.
   * @spacecomplexity <code>O(N^2)</code>.
   */
  explicit binom_table(TArg N) :
    max_n(N), _values(static_cast<size_t>(N+2)*static_cast<size_t>(N+1)/2U)
  {
    size_t i = 1, j = 0;
    _values[0] = 1;
    for (TArg n = 1; n <= N; ++n)
    {
      _values[i++] = 1;
      for (TArg k = 1; k < n; ++k, ++j)
      {
        _values[i++] = _values[j] + _values[j+1];
      }
      ++j;
      _values[i++] = 1;
    }
  }

  /**
   * Returns the threshold of the table.
   * @complexity Constant.
   */
  TArg threshold() const { return max_n; }

  /**
   * Returns the binomial coefficient <code>C(n,k)</code>.
   * @param n First argument. Must be greater than or equal to zero and
   *      less than or equal to <code>threshold()</code>.
   * @param k Second argument.
   * @returns <code>C(n,k)</code> if <code>0 ≤ k ≤ n</code>; zero
   *      otherwise.
   * @complexity Constant.
   */
  T operator () (TArg n, TArg k) const
  {
    assert(n >= 0 && n <= max_n);
    if (k >= 0 && k <= n && n <= max_n)
    {
      T i = (n+1)*n/2 + k;
      return _values[i];
    }
    else
    {
      return 0;
    }
  }
};

#if 0
template <typename T>
class binom_residue
{
  T p;
  binom_table<T> C;

public:
  binom_residue(T modulus) : p(modulus), C(modulus - 1) { }

  template <typename TArg>
  T operator () (TArg n, TArg k) const
  {
    euler::residue<T> result((T)1, p);
    do
    {
      T a = n % p;
      T b = k % p;
      if (a < b)
        return 0;
      n /= p;
      k /= p;
      result *= C(a, b);
    } while (k);
    return result.value();
  }
};
#endif

/**
 * Computes the binomial coefficient <code>C(n,k)</code>.
 *
 * This function computes the exact value of the binomial coefficient
 * <code>C(n,k)</code> using the recurrence formula
 * \f[ C(n,k) = C(n-1,k) + C(n-1,k-1) . \f]
 *
 * Alternatively, one <i>could</i> compute the binomial coefficient using
 * the factorial formula <code>C(n,k) = n! / [(n-k)! k!]</code>. However,
 * if we apply the factorial formula directly, the intermediate result
 * will easily overflow. If we instead factorize and reduce the numerator
 * and denominator first, the computation would be too complex. Therefore,
 * using the recurrence formula ensures accuracy and balances performance.
 *
 * To ensure the result does not overflow, <code>C(n,k)</code> should not
 * exceed the capacity of @c T. While this is hard to tell before
 * the result is actually computed, a rule of thumb is to make sure the
 * value of @c n is not greater than the number of bits in @c T.
 * For example, if @c T is <code>unsigned long long</code>, which contains
 * 64 bits, then @c n should not be greater than 64.
 *
 * @timecomplexity <code>O(nk)</code>.
 * @spacecomplexity <code>O(k)</code>.
 *
 * @ingroup Binomial
 */
template <typename T, typename TArg>
T binom(TArg n, TArg k)
{
  if (!(0 <= k && k <= n))
  {
    return 0;
  }
  if (k > n / 2)
  {
    k = n - k;
  }
  if (k == 0)
  {
    return T(1);
  }
  if (k == 1)
  {
    return T(n);
  }

  std::vector<T> C(k + 1);
  C[0] = T(1);
  for (TArg i = 1; i < n; i++)
  {
    for (TArg j = (std::min)(i, k); j >= 1; j--)
    {
      C[j] += C[j-1];
    }
  }
  return C[k] + C[k-1];
}

/**
 * Computes the binomial coefficient <code>C(n,k)</code> modulo a prime,
 * @c p.
 *
 * To compute <code>C(n,k)</code> mod @c p where @c p is prime, expand @c n
 * and @c k in base @c p as
 * \f[
 * \begin{align}
 * n &= a_r p^r + a_{r-1} p^{r-1} + \cdots + a_0 \\
 * k &= b_r p^r + b_{r-1} p^{r-1} + \cdots + b_0
 * \end{align}
 * \f]
 * and it follows from the properties of binomial coefficients that
 * \f[
 * C(n, k) \equiv C(a_r, b_r) \times \cdots \times C(a_0, b_0) \mod p
 * \f]
 * where <code>C(a,b)</code> is the usual binomial coefficient if \f$a \ge b\f$,
 * or 0 if \f$a < b\f$. Note that since <code>C(a,0) = 1</code>, we only
 * need to expand @c k to the maximum power of @c p; the remaining
 * higher-order terms in @c n can be ignored.
 *
 * To compute the binomial coefficients <code>C(n,k)</code> mod @c p
 * where \f$0 \le k \le n \le p - 1\f$, use the formula
 * \f[
 * C(n, k) = \frac{n (n-1) \cdots (n-k+1)}{k (k-1) \cdots 1} .
 * \f]
 * Since @c p is prime and @c n and @c k < @c p, all the terms in the
 * above equation are coprime to @c p. Therefore,
 * \f[
 * C(n, k) \equiv [n (n-1) \cdots (n-k+1)] [k (k-1) \cdots 1]_p^{-1}
 * \f]
 * where \f$[a]_p^{-1}\f$ denotes the modular multiplicative inverse
 * of @c a mod @c p.
 *
 * The time complexity of the algorithm is <code>(r+1)</code> times the
 * the cost to compute each <code>C(a,b)</code>. In a straightforward
 * implementation, computing each <code>C(a,b)</code> takes <code>O(p)</code>
 * modular multiplications plus one modular multiplicative inverse.
 * Thus the overall time complexity is <code>O(log(k)*p)</code> operations.
 *
 * A few optimizations are possible. First, the arguments \f$(a_i, b_i)\f$
 * can be stored to detect any \f$a_i < b_i\f$ in which case @c 0 can be
 * returned immediately. Second, duplicate multiplications could be avoided
 * by caching previous results. The current implementation does not contain
 * all these optimizations.
 *
 * For more information regarding the calculation and properties of binomial
 * coefficients, see
 *   - http://en.wikipedia.org/wiki/Binomial_coefficient
 *
 * @param n First argument.
 * @param k Second argument.
 * @param p The modulus of the result. Must be prime.
 * @returns <code>C(n,k)</code> mod @c p.
 *
 * @timecomplexity <code>O(log(k)*p)</code>.
 * @spacecomplexity <code>O(log(k))</code>.
 *
 * @ingroup Binomial
 */
template <typename T, typename TArg>
T modbinom(TArg n, TArg k, T p)
{
  // Special cases.
  if (n < k)
  {
    return 0;
  }
  if (k == 0 || k == n)
  {
    return 1;
  }

  // Store the digit expansion of n and k in base p in a vector.
  const int Bits = std::numeric_limits<TArg>::digits + 1;
  std::pair<T,T> digits[Bits];
  int terms = 0;
  T max_b = 0; // Find the maximum b.
  for (; terms < Bits && k != 0; ++terms)
  {
    T a = n % p;
    T b = k % p;
    if (a < b)
    {
      return 0;
    }
    n /= p;
    k /= p;
    digits[terms] = std::pair<T,T>(a, std::min(b, a - b));
    max_b = std::max(max_b, b);
  }

  // Compute each C(a,b) and append to the result.
  //euler::residue<T> result(1, p);
  T result = 1;
  for (int i = 0; i < terms; i++)
  {
    T a = digits[i].first, b = digits[i].second;

    // Special case: b = 0.
    if (b == 0)
    {
      continue;
    }

    // u = a*(a-1)*...*(a-b+1) mod p.
    // v = b*(b-1)*...*1 mod p.
#if 0
    euler::residue<T> u(a,p), v(b,p);
    for (T j = 1; j < b; j++)
    {
      u *= (a - j);
      v *= (b - j);
    }
    result *= (u / v);
#else
    T u = a, v = b;
    for (T j = 1; j < b; j++)
    {
      u = modmul(u, a - j, p);
      v = modmul(v, b - j, p);
    }
    result = modmul(result, moddiv(u, v, p), p);
#endif
  }
  return result;
}

#if 0
template <typename T, typename TArg>
T modbinom(TArg n, TArg k, T p)
{
  // Special case.
  if (n < k)
    return 0;
  if (k == 0 || k == n)
    return 1;

  // Store the digit expansion of n and k in base p in a vector.
  const int Bits = std::numeric_limits<TArg>::digits + 1;
  std::pair<T,T> digits[Bits];
  int terms = 0;
  T max_b = 0; // Find the maximum b.
  for (; terms < Bits && k != 0; ++terms)
  {
    T a = n % p;
    T b = k % p;
    if (a < b)
      return 0;
    n /= p;
    k /= p;
    digits[terms++] = std::pair<T,T>(a, std::min(b, a - b));
    max_b = std::max(max_b, b);
  }

  // Sort the binomial coefficients by (a, b).
  std::sort(&digits[0], &digits[terms],
    [](const std::pair<T,T> &t1, const std::pair<T,T> &t2) -> bool {
      return (t1.first < t2.first) || (t1.first == t2.first && t1.second < t2.second);
  });

  // Compute and multiply the binomial numbers in order.
  euler::residue<T> result(1, p);
  std::vector<euler::residue<T>> C(max_b + 1);
  C[0] = result;
  int current_n = 0;
  for (int i = 0; i < terms; i++)
  {
    T a = digits[i].first, b = digits[i].second;
    for (; aa < a; ++aa)
      // to be completed
  }
  return result.value();
}
#endif

} // namespace euler

#if 0
static void test_binom()
{
  const int N = 64;
  euler::binom_table<unsigned long long> C(N);
  euler::prime_table<int> ptable(10000);
  std::vector<int> primes(ptable.begin(), ptable.end());

  for (int n = 0; n <= N; n++)
  {
    for (int k = 0; k <= n; k++)
    {
      unsigned long long b = C(n, k);
      for (int i = 0; i < primes.size(); i++)
      {
        int p = primes[i];
        if (b % p != euler::modbinom(n, k, p))
          std::cout << "Error: C(" << n << "," << k
            << ") mod " << p << std::endl;
      }
    }
  }
}
#endif

#endif // EULER_BINOMIAL_H
