/**
 * @defgroup Figurate Figurate Number
 * Figurate number related routines.
 *
 * @ingroup Library
 *
 * A <i>figurate number</i> is a number that can be represented by a regular
 * geometrical arrangement of equally spaced points. If the arrangement
 * forms a regular polygon, the number is called a <i>polygonal number</i>.
 * At present, this module only provides routines related to polygonal
 * numbers.
 *
 * Let @c r be the number of sides in a polygon, then the <code>n</code>th
 * polygonal number, <code>P<sub>n</sub></code> is defined by
 * \f[ P_n = \frac{1}{2} n [(r-2)n - (r-4)] . \f]
 * The following equivalent forms are sometimes useful. The first form shows
 * that <code>P<sub>n</sub></code> is an integer:
 * \f[ P_n = \frac{n(n-1)(r-2)}{2} + n , \f]
 * and the second form writes it in squares:
 * \f[ P_n = \frac{[2(r-2)n - (r-4)]^2 - (r-4)^2}{8(r-2)} . \f]
 *
 * There are special names for polygonal numbers with 3 to 10 sides (and
 * more): triangular, square, pentagonal, hexagonal, heptagonal, octagonal,
 * nonagonal, and decagonal.
 *
 * For more information on polygonal numbers, see
 *   - http://mathworld.wolfram.com/PolygonalNumber.html
 *   - http://en.wikipedia.org/wiki/Polygonal_number
 */

#ifndef EULER_FIGURATE_H
#define EULER_FIGURATE_H

#include "imath.hpp"

namespace euler {

/**
 * Returns the <code>n</code>th <code>r</code>-gonal number.
 *
 * @param r Rank of the polygonal number.
 *
 * @param n Index of the number to return.
 *
 * @returns The <code>n</code>th <code>r</code>-gonal number.
 *
 * @complexity Constant.
 *
 * @ingroup Figurate
 */
template <typename T>
T polygonal(int r, T n)
{
  return n*(n-1)/2*(r-2)+n;
}

/**
 * Gets the index of a number in the sequence of <code>r</code>-gonal numbers.
 *
 * @param r Rank of polygonal to check.
 *
 * @param n An integer to check.
 *
 * @returns If @c n is an <code>r</code>-gonal number, returns its (one-based)
 *    index in the sequence of all <code>r</code>-gonal numbers. If @c n is
 *    not an <code>r</code>-gonal number, returns zero.
 *
 * @complexity Constant.
 *
 * @ingroup Figurate
 */
template <typename T>
T polygonal_index(int r, T n)
{
  T delta = (r-4)*(r-4)+8*(r-2)*n;
  T s = isqrt(delta);
  if ((s*s == delta) && ((r-4+s) % (2*(r-2)) == 0))
  {
    return (r-4+s) / (2*(r-2));
  }
  else
  {
    return 0;
  }
}

/**
 * Tests whether an integer is an <code>r</code>-gonal number.
 *
 * @param r Rank of polygonal to check.
 *
 * @param n An integer to check.
 *
 * @returns @c true if @c n is an <code>r</code>-gonal number, @c false
 *    otherwise.
 *
 * @complexity Constant.
 *
 * @ingroup Figurate
 */
template <typename T>
bool is_polygonal(int r, T n)
{
  return polygonal_index(r, n) != 0;
}

} // namespace euler

#endif // EULER_FIGURATE_H
