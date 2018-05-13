/**
 * @defgroup imath Integer Math Functions
 *
 * Integer arithmetic functions.
 *
 * @ingroup Library
 */

#ifndef EULER_IMATH_H
#define EULER_IMATH_H

#include <cmath>
#include <stdexcept>
#include <type_traits>
#include <functional>

namespace euler {

/**
 * Raises an element to an integral power.
 *
 * @param a Base.
 *
 * @param k Exponent.
 *
 * @param op An associative binary operator mapping <code>(T, T) -> T</code>.
 *    Associativity means <code>(a op b) op c == a op (b op c)</code> for all
 *    @c a, @c b and @c c of type @c T.
 *
 * @param e Identity element for operator @c op, i.e. the element such that
 *    <code>(e op a) == (a op e) == a</code> for all @c a of type @c T.
 *
 * @returns The result of <code>e op a op a ...</code> where @c op is applied
 *    @c k times.
 *
 * @exception std::domain_error if <code>k < 0</code>.
 *
 * @remarks If arithmetic overflow occurs during the calculation, the result
 *    is undefined.
 *
 * @algorithm The <i>binary exponentiation</i> algorithm, which is explained
 *    at http://en.wikipedia.org/wiki/Exponentiation_by_squaring.
 *
 * @timecomplexity No more than <code>2*log<sub>2</sub>k</code> applications
 *    of @c op.
 *
 * @spacecomplexity Constant.
 *
 * @ingroup imath
 */
template <typename T, typename TExponent, typename Op = std::multiplies<T>>
T ipow(T a, TExponent k, Op op = Op(), const T &e = T(1))
{
  static_assert(
      std::is_integral<TExponent>::value,
      "Exponent must be of integral type.");

  if (k < TExponent(0))
  {
    throw std::domain_error("Exponent must be nonnegative.");
  }

  T result = e;
  for (; k > 0; k >>= 1)
  {
    if ((k & 1) != 0)
    {
      result = op(result, a);
    }
    a = op(a, a);
  }
  return result;
}

/**
 * Computes the integral part of the square root an integer.
 *
 * @tparam T An integral type.
 *
 * @param n A non-negative integer whose square root is computed.
 *
 * @returns If @c n is non-negative, returns the largest integer that is less
 *    than or equal to the square root of @c n. If @c n is negative, returns
 *    @c -1.
 *
 * @complexity Constant.
 *
 * @ingroup imath
 */
template <typename T>
T isqrt(T n)
{
  if (n < 0)
  {
    return -1;
  }
  return static_cast<T>(std::sqrt(n));
}

/**
 * Computes the integral part of the logarithm of an integer.
 *
 * @tparam T An integral type.
 *
 * @param n A positive integer whose logarithm to take.
 *
 * @param base Base of the logarithm.
 *
 * @returns <code>⌊ log<sub>base</sub> x ⌋</code>.
 *
 * @timecomplexity <code>O(ln x)</code>.
 *
 * @spacecomplexity Constant.
 *
 * @ingroup imath
 */
template <typename T>
T ilog(T n, T base)
{
  T ub = n / base;
  T e = 0;
  for (T v = 1; v <= ub; v *= base)
  {
    e++;
  }
  return e;
}

#if 0
// TODO: revive the following implementation.
template <class T>
T isqrt_safe(T n)
{
  if (n < 0)
    return -1;
  if (n == 0)
    return 0;

  typedef typename std::make_unsigned<T>::type UnsignedT;
  const int bits = std::numeric_limits<UnsignedT>::digits;

  UnsignedT a = n;
  UnsignedT rem = 0;
  UnsignedT root = 0;
  for (int i = 0; i < bits/2; i++)
  {
    root <<= 1;
    rem = (rem << 2) + (a >> (bits - 2));
    a <<= 2;
    root++;
    if (root <= rem)
    {
      rem -= root;
      root++;
    }
    else
    {
      root--;
    }
  }
  return (root >> 1);
}

static void test_sqrt()
{
  for (int n = 1; n < 100000000; n++)
  {
    int x = (int)sqrt((double)n);
    int y = isqrt(n);
    if (x != y)
      std::cout << "Failed for n = " << n << std::endl;
  }
}
#endif

} // namespace euler

#endif // EULER_IMATH_H
