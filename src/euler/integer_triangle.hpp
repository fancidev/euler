#ifndef EULER_INTEGER_TRIANGLE_H
#define EULER_INTEGER_TRIANGLE_H

#include "gcd.hpp"

namespace euler {

/**
 * Generates Pythagorean triangles whose side lengths are coprime integers.
 *
 * @tparam T An integral type.
 *
 * @tparam Func Callable type; see @c f for details.
 *
 * @param max_perimeter Maximum perimeter to generate.
 *
 * @param f Callback function that takes three arguments of type @c T. For
 *    each triangle generated, @c f is called with the side-lengths of the
 *    triangle. The return value of @c f is ignored.
 */
template <typename T, typename Func>
void generate_right_triangles(T max_perimeter, Func f)
{
  /**
   * Generating function:
   *   a = m^2 - n^2
   *   b = 2*m*n
   *   c = m^2 + n^2
   * where m and n are coprime integers and one of them is even with m > n.
   *   L = 2*m*(m+n)
   */
  for (T m = 2; ; m++)
  {
    T n = (m & 1)? 2 : 1;
    for (; n < m; n += 2)
    {
      // make sure m and n are co-prime.
      if (euler::gcd(m, n) != 1)
        continue;

      T a = m*m - n*n;
      T b = 2*m*n;
      T c = m*m + n*n;
#if 0
      if (!f(a, b, c))
        break;
#else
      if (a + b + c > max_perimeter)
        break;
      f(a, b, c);
#endif
    }
    if (n <= 2)
      break;
  }
}

/**
 * Generates triangles with exactly one 60-degree angle and whose side-lengths
 * are coprime integers.
 *
 * @tparam integer_t An integral type that represents the side-length.
 *
 * @tparam Func Callable type; see @c f for details.
 *
 * @param max_perimeter Not used.
 *
 * @param f Callback function that takes three arguments of type @c integer_t.
 *    For each triangle generated, @c f is called with the side-lengths of the
 *    triangle. The return value of @c f is treated as follows.
 *
 * @remarks The algorithm runs a nested loop. In the inner loop, for each
 *   triangle generated, it calls function f(a,b,c)->bool. If f returns false,
 *   the inner loop is terminated. If f returns false for the first iteration
 *   of the inner loop, the outer loop is terminated.
 *
 * @todo Clarity callback behavior.
 */
template <typename integer_t, typename Func>
void generate_60_degree_triangles(integer_t max_perimeter, Func f)
{
  (void)max_perimeter;

  // Generating function:
  //   a = m^2 + m*n + n^2
  //   b = m^2 + 2*m*n
  //   c = n^2 + 2*m*n
  // or
  //   c = m^2 - n^2
  // with coprime integers m, n with 0 < n < m, m not congruent to n (mod 3).
  //   L = (2m+n)(m+2n)
  // or
  //   L = 3m(m+n)
  for (integer_t m = 2; ; m++)
  {
    integer_t n = (m % 3 == 1)? 2 : 1;
    bool gen_1 = true, gen_2 = true;
    for (; n < m; n++)
    {
      // make sure m is not congruent to n (mod 3)
      if (m % 3 == n % 3)
        continue;

      // make sure m and n are co-prime
      if (greatest_common_divisor(m, n) != 1)
        continue;

      integer_t a = m*m + m*n + n*n;
      integer_t b = m*m + 2*m*n;
      if (gen_1)
      {
        integer_t c = n*n + 2*m*n;
        gen_1 = f(a, b, c);
      }
      if (gen_2)
      {
        integer_t c = m*m - n*n;
        gen_2 = f(a, b, c);
      }
      
      if (!gen_1 && !gen_2)
        break;
    }
    if (n == 1)
      break;
  }
}

/**
 * Generates triangles with one 120-degree angle and whose side-lengths are
 * coprime integers.
 *
 * @tparam integer_t An integral type that represents the side-length.
 *
 * @tparam Func Callable type; see @c f for details.
 *
 * @param max_perimeter Not used.
 *
 * @param f Callback function that takes three arguments of type @c integer_t.
 *    For each triangle generated, @c f is called with the side-lengths of the
 *    triangle. The first side-length is guaranteed to be the longest side.
 *    The return value of @c f is treated as follows.
 *
 * @remarks The algorithm runs a nested loop. In the inner loop, for each
 *   triangle generated, it calls function f(a,b,c)->bool. If f returns false,
 *   the inner loop is terminated. If f returns false for the first iteration
 *   of the inner loop, the outer loop is terminated.
 *
 * @todo Clarity callback behavior.
 */
template <typename integer_t, typename Func>
void generate_120_degree_triangles(integer_t max_perimeter, Func f)
{
  (void)max_perimeter;

  // Generating function:
  //   a = m^2 + m*n + n^2
  //   b = 2*m*n + n^2
  //   c = m^2 - n^2
  // with coprime integers m, n with 0 < n < m, m not congruent to n (mod 3). 
  //   L = (2m+n)(m+n)
  for (integer_t m = 2; ; m++)
  {
    integer_t min_n = (m % 3 == 1)? 2 : 1;
    integer_t n;
    for (n = min_n; n < m; n++)
    {
      // make sure m is not congruent to n (mod 3)
      if (m % 3 == n % 3)
        continue;

      // make sure m and n are co-prime
      if (greatest_common_divisor(m, n) != 1)
        continue;

      integer_t a = m*m + m*n + n*n;
      integer_t b = 2*m*n + n*n;
      integer_t c = m*m - n*n;
      if (!f(a, b, c))
        break;
    }
    if (n == min_n)
      break;
  }
}

} // namespace euler

#endif
