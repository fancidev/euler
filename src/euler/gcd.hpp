/**
 * @defgroup gcd Euclidean Algorithm
 * Euclidean algorithms and routines related to Bézout identity.
 * @ingroup Library
 */

#ifndef EULER_GCD_H
#define EULER_GCD_H

#include <stdexcept>
#include <type_traits>

namespace euler {

#define EULER_GCD_CHECK_TYPE(T) \
  static_assert(std::is_integral<T>::value, #T " must be an integral type.")

#define EULER_GCD_CHECK_VALUE(n) \
  do { \
    if (!((n) >= 0)) { \
      throw std::invalid_argument(#n " must be non-negative."); \
    } \
  } while (false)

#define EULER_GCD_CHECK_POSITIVE(n) \
  do { \
    if (!((n) > 0)) { \
      throw std::invalid_argument(#n " must be positive."); \
    } \
  } while (false)

namespace details {

// Tail-recursive implementation of gcd(a, b) where a, b >= 0.
// See the documentation of gcd() for details on the algorithm.
template <typename T>
T gcd_tr(const T &a, const T &b)
{
  if (b == 0)
  {
    return a;
  }
  else
  {
    return gcd_tr(b, a % b);
  }
}

} // namespace details

/**
 * Computes the greatest common divisor of two integers.
 *
 * The semantics of this function is the same as that of
 * <a href="http://en.cppreference.com/w/cpp/numeric/gcd">std::gcd()</a>,
 * setting aside minor differences in argument type and domain.
 *
 * @tparam T An integral type.
 *
 * @param a A non-negative integer.
 *
 * @param b A non-negative integer.
 *
 * @returns If @c a and @c b are both zero, returns zero. Otherwise, returns
 *    the largest (positive) integer @c d such that <code>a % d == 0</code>
 *    and <code>b % d == 0</code>. (For the avoidance of doubt, if one of @c
 *    a and @c b is zero, returns the other argument.)
 *
 * @exception std::invalid_argument if any argument is invalid.
 *
 * @algorithm A tail-recursive implementation of <a href="https://en.wikipedia.org/wiki/Euclidean_algorithm">Euclidean algorithm</a>, as described below.
 *    Let <code>a ≧ b ≧ 1</code>. Write <code>a = b * q + r</code>, where
 *    <code>0 ≤ r < b</code> is the proper remainder. If <code>r = 0</code>,
 *    then <code>gcd(a, b) = b</code>. Otherwise, denote by
 *    <code>d = gcd(a, b) ≧ 1</code>. Since @c d divides both @c a and @c b,
 *    @c d also divides @c r. Conversely, any @c d that divides both @c b and
 *    @c r also divides @c a. Therefore <code>gcd(a, b) = gcd(b, r)</code>.
 *
 * @timecomplexity <code>O(log(min(a, b)))</code> divisions.
 *
 * @spacecomplexity Constant.
 *
 * @see <code>lcm()</code>.
 *
 * @ingroup gcd
 */
template <typename T>
T gcd(T a, T b)
{
  EULER_GCD_CHECK_TYPE(T);
  EULER_GCD_CHECK_VALUE(a);
  EULER_GCD_CHECK_VALUE(b);
  return details::gcd_tr(a, b);
}

/**
 * Computes the least common multiple of two integers.
 *
 * The semantics of this function is the same as that of
 * <a href="http://en.cppreference.com/w/cpp/numeric/lcm">std::lcm()</a>,
 * setting aside minor differences in argument type and domain.
 *
 * @tparam T An integral type.
 *
 * @param a A non-negative integer.
 *
 * @param b A non-negative integer.
 *
 * @returns If @c a and/or @c b is zero, returns zero. Otherwise, returns the
 *    smallest positve integer @c m such that <code>m % a == 0</code> and
 *    <code>m % b == 0</code>. If such @c m is not representable in @c T, the
 *    return value is undefined.
 *
 * @exception std::invalid_argument if any argument is invalid.
 *
 * @algorithm <code>lcm(a, b) = a * (b / gcd(a, b))</code>.
 *
 * @timecomplexity <code>O(log(min(a, b)))</code> divisions.
 *
 * @spacecomplexity Constant.
 *
 * @see <code>gcd()</code>.
 *
 * @ingroup gcd
 */
template <typename T>
T lcm(T a, T b)
{
  EULER_GCD_CHECK_TYPE(T);
  EULER_GCD_CHECK_VALUE(a);
  EULER_GCD_CHECK_VALUE(b);

  if (a == 0 || b == 0)
  {
    return 0;
  }
  else
  {
    T d = gcd(a, b);
    return a * (b / d);
  }
}

namespace details {

template <typename T>
std::pair<T, std::pair<T, T>> egcd_r(const T &a, const T &b)
{
  T q = a / b;
  T r = a % b;
  if (r == 0)
  {
    T d = b, x = 1, y = q - 1;
    return std::make_pair(d, std::make_pair(x, y));
  }
  else
  {
    const std::pair<T, std::pair<T, T>> &s = egcd_r(b, r);
    T d = s.first;
    T u = s.second.first;
    T v = s.second.second;
    T x = b / d - v;
    T y = a / d - u - v * q;
    return std::make_pair(d, std::make_pair(x, y));
  }
}

} // namespace details

/**
 * Solves an alternative form of Bézout's identity with positive arguments.
 *
 * @tparam T An integral type.
 *
 * @param a A positive integer.
 *
 * @param b A positive integer.
 *
 * @returns <code>(d, (x, y))</code> where <code>d = gcd(a, b)</code> and
 *    <code>(x, y)</code> is the unique integer solution to an alternative
 *    form of Bézout's identity
 *    @f[
 *      ax - by = \textrm{gcd}(a, b) = d
 *    @f]
 *    such that
 *    @f[
 *      0 < x \le \frac{b}{d}, \, 0 \le y < \frac{a}{d} .
 *    @f]
 *    The equality bound of @c x is attained iff @f$ \textrm{gcd}(a,b)=b @f$,
 *    in which case @f$ x = 1 @f$ and @f$ y = a / d - 1 @f$. The equality
 *    bound of @c y is attained iff @f$ \textrm{gcd}(a, b) = a @f$, in which
 *    case @f$ x = 1 @f$ and @f$ y = 0 @f$.
 *
 * @exception std::invalid_argument if any argument is invalid.
 *
 * @algorithm <a href="https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm">Extended Euclidean algorithm</a>, as described below.
 *    First observe that from any one solution @f$ (x, y) @f$, the entire
 *    family of solutions can be generated by
 *    @f[
 *      \left( x + k \frac{b}{d}, y + k \frac{a}{d} \right), k \in \mathbb{Z}.
 *    @f]
 *    Now write @f$ a = b q + r @f$ where @f$ 0 \le r < b @f$. If @f$ r=0 @f$,
 *    then @f$ d = \textrm{gcd}(a, b) = b @f$ and the equation reduces to
 *    @f[
 *      a x - b y = b .
 *    @f]
 *    Among the family of solutions
 *    @f[
 *      x = k, \, y = -1 + k \frac{a}{b}, \, k \in \mathbb{Z},
 *    @f]
 *    it can be verified that
 *    @f[
 *      x = 1, \, y = \frac{a}{b} - 1
 *    @f]
 *    is the unique solution that satisfies
 *    @f[
 *      0 < x \le \frac{b}{d}, \, 0 \le y < \frac{a}{d}.
 *    @f]
 *    Now suppose @f$ r \ne 0 @f$. Note that any integer that divides both
 *    @c a and @c b also divides @c r, and that any integer that divides both
 *    @c b and @c r also divides @c a. It follows that @f$ d =
 *    \textrm{gcd}(a, b) = \textrm{gcd}(b, r) @f$. Consider the equation
 *    @f[
 *      b u - r v = \textrm{gcd}(b, r) = d
 *    @f]
 *    and suppose we have found a (unique) solution @f$ (u, v) @f$ such that
 *    @f[
 *      0 < u \le \frac{r}{d}, \, 0 \le v < \frac{b}{d}.
 *    @f]
 *    Chaining the equations and substituting @f$ a = bq + r @f$ yields
 *    @f[
 *      bu - rv = d = ax - by = (bq + r) x - by = b (qx - y) + rx .
 *    @f]
 *    Among the family of solutions
 *    @f[
 *      x = -v+k \frac{b}{d}, \, y = -u-vq+k \frac{a}{d}, \, k \in \mathbb{Z},
 *    @f]
 *    it can be verified that
 *    @f[
 *      x = \frac{b}{d} - v, \, y = \frac{a}{d} - u - vq
 *    @f]
 *    is the unique solution that satisfies
 *    @f[
 *      0 < x \le \frac{b}{d}, \, 0 \le y < \frac{a}{d}.
 *    @f]
 *
 * @timecomplexity <code>O(log(min(a, b)))</code>.
 *
 * @spacecomplexity Constant.
 *
 * @ingroup gcd
 */
template <typename T>
std::pair<T, std::pair<T, T>> egcd(T a, T b)
{
  EULER_GCD_CHECK_TYPE(T);
  EULER_GCD_CHECK_POSITIVE(a);
  EULER_GCD_CHECK_POSITIVE(b);
  return details::egcd_r(a, b);
}

/**
 * Generates a family of quadruples that satisfy Bézout's identity
 * <code>ax - by = 1</code> from a given quadruple.
 *
 * This function generates a family of quadruples <code>(a, b, x, y)</code>
 * that satisfy Bézout's identity <code>ax - by = 1</code>. It starts from
 * the supplied solution \f$(a, b, x, y)\f$ and recursively applies the
 * following relation
 * \f[
 * (x+kb)a  - (y+ka)b = 1
 * \f]
 * to generate a family of solutions \f$a' = x+kb, b' = y+ka, x' = a, y' = b\f$
 * for \f$k \ge 1\f$.
 *
 * For each quadruple generated, excluding the supplied one, this function
 * calls <code>f(a, b, x, y)</code> which must return a boolean value to
 * indicate whether to continue generation on that path.
 * If <code>f(a, b, x, y)</code> returns <code>false</code>,
 * any quadruple <code>f(a', b', x', y')</code> such that
 * <code>a' ≥ a, b' ≥ b, x' ≥ x, y' ≥ y</code> will not be generated.
 *
 * Note that while solutions to Bézout's identity are integral by definition,
 * the data type passed to this function need not be integral because a
 * recurrence relation is used to generate the solutions and the identity
 * itself is never checked explicitly.
 *
 * @param a First coefficient.
 * @param b Second coefficient.
 * @param x First Bézout number.
 * @param y Second Bézout number.
 * @param f Callback function.
 * @returns Number of quadruples generated, not including the supplied one.
 *    This is equal to the number of times <code>f(a, b, x, y)</code>
 *    returns <code>true</code>.
 *
 * @timecomplexity <code>O(N*log(N))</code>, where @c N is the largest element
 *    generated.
 *
 * @spacecomplexity <code>O(log(N))</code> on the stack, where @c N is the
 *    largest element generated.
 *
 * @ingroup gcd
 */
template <class T, class Func>
size_t generate_bezout_quadruples(T a, T b, T x, T y, Func f)
{
  size_t count = 0;
  T aa=x+b, bb=y+a, xx=a, yy=b;
  while (f(aa, bb, xx, yy))
  {
    ++count;
    count += generate_bezout_quadruples(aa,bb,xx,yy,f);
    aa += b;
    bb += a;
  }
  return count;
}

/**
 * Generates quadruples that satisfy the special form of Bézout's identity
 * <code>a * x - b * y = 1</code>.
 *
 * This function generates all integer quadruples <code>(a, b, x, y)</code>
 * such that <code>a, b, x, y ≥ 1</code> and <code>a * x - b * y = 1</code>.
 * It additionally guarantees that <code>|x| ≤ |b|</code> and
 * <code>|y| ≤ |a|</code>, where <code>|x| = |b|</code> iff <code>b = 1</code>
 * and <code>|y| = |a|</code> iff <code>a = 1</code>.
 *
 * @param f Callback function.
 *
 * @returns Number of quadruples generated. This is equal to the number
 *      of times <code>f(a, b, x, y)</code> returns <code>true</code>.
 *
 * @timecomplexity <code>O(N*log(N))</code>, where @c N is the largest
 *      element generated.
 *
 * @spacecomplexity <code>O(log(N))</code> on the stack, where @c N is the
 *      largest element generated.
 *
 * @ingroup gcd
 */
template <class T, class Func>
size_t generate_bezout_quadruples(Func f)
{
  size_t count = 0;
  for (T n = 1; ; n++)
  {
    size_t c = generate_bezout_quadruples(1,n,1,0,f);
    if (c == 0)
    {
      break;
    }
    count += c;
  }
  return count;
}

} // namespace euler

#endif // EULER_GCD_HPP
