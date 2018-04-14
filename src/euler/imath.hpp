/**
 * @defgroup IMath Integer Math Functions
 * @ingroup Library
 */

#ifndef EULER_IMATH_H
#define EULER_IMATH_H

#include <cmath>
#include <type_traits>
#include <functional>

namespace euler {

/**
 * Computes the multiplicative power of an element by binary exponentation.
 *
 * This function uses the <i>binary exponentiation</i> algorithm to apply
 * a given operator (multiplication) for a given number of times (exponent).
 * The algorithm squares @c base in each iteration and multiplies it to the
 * result if the corresponding bit in the exponent is set. 
 *
 * The multiplication operation must be defined for the element and it 
 * must obey the commutative law.
 *
 * A detailed explanation of the binary exponentiation algorithm can be 
 * found at 
 *   - http://en.wikipedia.org/wiki/Exponentiation_by_squaring
 *
 * @param base The base.
 * @param exponent The exponent. Must be greater than or equal to zero.
 * @param identity The identity element.
 * @param multiplies The multiplication operator.
 * @returns @c base multiplied @c exponent times.
 *
 * @timecomplexity No more than <code>2*log<sub>2</sub>E</code> 
 *      multiplications, where @c E is the exponent.
 * @spacecomplexity Constant.
 * @ingroup IMath
 */
template <typename T, typename TExponent, typename Func>
T binexp(T base, TExponent exponent, const T &identity, Func multiplies)
{
  T result = identity;
  for (; exponent > 0; exponent >>= 1)
  {
    if (exponent & 1)
    {
      result = multiplies(result, base);
    }
    base = multiplies(base, base);
  }
  return result;
}

/**
 * Computes the integral power of an element by binary exponentation.
 *
 * This function uses the <i>binary exponentiation</i> algorithm which 
 * squares @c base in each iteration and multiplies it to the result if
 * the corresponding bit in the exponent is set. The multiplication 
 * operation must be defined for the element and it must obey the 
 * commutative law.
 *
 * A detailed explanation of the binary exponentiation algorithm can be 
 * found at 
 *   - http://en.wikipedia.org/wiki/Exponentiation_by_squaring
 *
 * @param base The base.
 * @param exponent The exponent. Must be greater than or equal to zero.
 * @returns One multiplied by @c base for @c exponent times.
 *
 * @timecomplexity No more than <code>2*log<sub>2</sub>E</code> 
 *      multiplications, where @c E is the exponent.
 * @spacecomplexity Constant.
 * @ingroup IMath
 */
template <typename T, typename TExponent>
T ipow(const T &base, TExponent exponent)
{
  return binexp(base, exponent, T(1), std::multiplies<T>());
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
 * @ingroup IMath
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
 * @ingroup IMath
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
