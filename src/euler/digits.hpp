/**
 * @defgroup Digits Decimal Expansion
 * Routines to expand the digits of an integer in a certain base.
 *
 * @ingroup Library
 *
 * This header-only module provides routines to expand the digits of an
 * integer in a certain base.
 */

#ifndef EULER_DIGITS_H
#define EULER_DIGITS_H

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <functional>
#include <iterator>
#include <limits>
#include <type_traits>
#include "sequence.hpp"

namespace euler {

/**
 * Represents the value of a digit of an integer.
 */
typedef int digit_t;

#define EULER_DIGITS_VALIDATE_BASE(base) \
  static_assert((base) >= 2, "base must be greater than or equal to 2.")

#define EULER_DIGITS_VALIDATE_TYPE(type) \
  static_assert(std::is_integral<type>::value, "expects integral type.")

#define EULER_DIGITS_VALIDATE_NUMBER(n) \
  do { \
    if (!((n) >= T(0))) \
    { \
      throw std::invalid_argument(#n " must be non-negative"); \
    } \
  } while (false)

/// Iterator that returns the digits of a non-negative integer from left to
/// right (most significant to least significant). Leading zeros are not
/// returned, unless the number is zero, in which case a single digit of 0 is
/// returned.
template <int base, typename T>
class digit_iterator
  : public std::iterator<std::forward_iterator_tag, digit_t,
                         std::ptrdiff_t, void, void>
{
  EULER_DIGITS_VALIDATE_BASE(base);
  EULER_DIGITS_VALIDATE_TYPE(T);

  T _n;
  T _b;

public:

  /**
   * Constructs an iterator that initially points to the most significant
   * digit of a non-negative integer.
   *
   * @param n A non-negative integer whose digits to expand.
   * @timecomplexity <code>log<sub>b</sub>n</code> operations.
   * @spacecomplexity Constant.
   */
  explicit digit_iterator(T n) : _n(n)
  {
    EULER_DIGITS_VALIDATE_NUMBER(n);
    for (_b = 1; n / _b >= base; _b *= base) { }
  }

  /// Constructs an empty iterator that points <i>past-the-end</i>.
  /// @complexity Constant.
  digit_iterator() : _n(0), _b(0) { }

  /// Returns the current digit.
  /// @complexity Constant.
  digit_t operator * () const
  {
    return static_cast<digit_t>(_n / _b);
  }

  /// Advances the iterator to point to the next (less significant) digit.
  /// @returns The advanced iterator.
  /// @complexity Constant.
  digit_iterator& operator ++ ()
  {
    _n %= _b;
    _b /= base;
    return *this;
  }

  /// Tests whether this iterator is equal to another iterator.
  bool operator == (const digit_iterator &other) const
  {
    return (_n == other._n) && (_b == other._b);
  }

  /// Tests whether this iterator is not equal to another iterator.
  bool operator != (const digit_iterator &other) const
  {
    return ! operator == (other);
  }
};

/// Iterator that returns the digits of a non-negative integer from right to
/// left (least significant to most significant). Leading zeros are not
/// returned, unless the number is zero, in which case a single digit of 0 is
/// returned.
template <int base, typename T>
class digit_reverse_iterator
  : public std::iterator<std::forward_iterator_tag, digit_t,
                         std::ptrdiff_t, void, void>
{
  EULER_DIGITS_VALIDATE_BASE(base);
  EULER_DIGITS_VALIDATE_TYPE(T);

  T _n;
  int _b;

public:

  /**
   * Constructs an iterator that initially points to the least significant
   * digit of a non-negative integer.
   *
   * @param n A non-negative integer whose digits to expand.
   *
   * @complexity Constant.
   */
  explicit digit_reverse_iterator(T n) : _n(n), _b(base)
  {
    EULER_DIGITS_VALIDATE_NUMBER(n);
  }

  /// Constructs an empty iterator that points <i>past-the-end</i>.
  /// @complexity Constant.
  digit_reverse_iterator() : _n(0), _b(0) { }

  /// Returns the current digit.
  /// @complexity Constant.
  digit_t operator * () const
  {
    return static_cast<digit_t>(_n % base);
  }

  /// Advances the iterator to point to the next (more significant) digit.
  /// @returns The advanced iterator.
  /// @complexity Constant.
  digit_reverse_iterator& operator ++ ()
  {
    if ((_n /= base) == 0)
    {
      _b = 0;
    }
    return *this;
  }

  /// Tests whether this iterator is equal to another iterator.
  bool operator == (const digit_reverse_iterator &other) const
  {
    return (_n == other._n) && (_b == other._b);
  }

  /// Tests whether this iterator is not equal to another iterator.
  bool operator != (const digit_reverse_iterator &other) const
  {
    return ! operator == (other);
  }
};

/**
 * Expands the digits of an integer from left to right (most significant to
 * least significant).
 *
 * @tparam base The base to expand into. Must be greater than or equal to 2.
 *
 * @tparam T An integral type.
 *
 * @param n A non-negative integer whose digits to expand.
 *
 * @returns A sequence containing the digits of @c n expanded in base @c base
 *    from left to right. The sequence does not contain any leading zero if
 *    @c n is positive; if @c n is zero, the sequence comprises a single digit
 *    of zero.
 *
 * @complexity Constant.
 *
 * @see from_digits(), rdigits()
 *
 * @ingroup Digits
 */
template <int base = 10, typename T>
sequence<digit_iterator<base, T>> digits(T n)
{
  EULER_DIGITS_VALIDATE_BASE(base);
  EULER_DIGITS_VALIDATE_TYPE(T);
  EULER_DIGITS_VALIDATE_NUMBER(n);
  return make_sequence(
    digit_iterator<base, T>(n),
    digit_iterator<base, T>());
}

/**
 * Expands the digits of an integer from right to left (least significant to
 * most significant).
 *
 * @tparam base The base to expand into. Must be greater than or equal to 2.
 *
 * @tparam T An integral type.
 *
 * @param n A non-negative integer whose digits to expand.
 *
 * @returns A sequence containing the digits of @c n expanded in base @c base
 *    from right to left. The sequence does not contain any leading zero if
 *    @c n is positive; if @c n is zero, the sequence comprises a single digit
 *    of zero.
 *
 * @complexity Constant.
 *
 * @see digits()
 *
 * @ingroup Digits
 */
template <int base = 10, typename T>
sequence<digit_reverse_iterator<base, T>> rdigits(T n)
{
  EULER_DIGITS_VALIDATE_BASE(base);
  EULER_DIGITS_VALIDATE_TYPE(T);
  EULER_DIGITS_VALIDATE_NUMBER(n);
  return make_sequence(
    digit_reverse_iterator<base, T>(n),
    digit_reverse_iterator<base, T>());
}

/**
 * Converts a sequence of digits expanded in a given base into an integer.
 * The sequence is given from left to right (most significant to least
 * significant). Leading zeros are allowed.
 *
 * @param begin Begin iterator of the digit sequence.
 *
 * @param end End iterator of the digit sequence.
 *
 * @returns An (non-negative) integer converted from the digit sequence.
 *
 * @timecomplexity <code>(end - begin)</code> operations.
 *
 * @spacecomplexity Constant.
 *
 * @remarks The caller must ensure that @c T has enough bits to hold the
 *    result to avoid an overflow condition. If @c T does not have enough
 *    bits to hold the result, the behavior is undefined.
 *
 * @ingroup Digits
 */
template <typename T, int base = 10, class InIt>
T from_digits(InIt begin, InIt end)
{
  EULER_DIGITS_VALIDATE_BASE(base);
  EULER_DIGITS_VALIDATE_TYPE(T);

  T n = 0;
  for (; begin != end; ++begin)
  {
    n = n * base + (*begin);
  }
  return n;
}

/**
 * Sorts the digits of an integer in descending order.
 *
 * @param n A non-negative integer whose digits to sort.
 *
 * @returns An integer with the same digits as @c n but sorted in descending
 *    order, i.e. larger digit in more significant place. In particular,
 *    embedded zeros in @c n are sorted to trailing position of the return
 *    value, which guarantees that the return value has the same number of
 *    digits as @c n.
 *
 * @timecomplexity <code>O(D*log(D))</code>, where @c D is the number of
 *    digits in <code>n</code>.
 *
 * @spacecomplexity <code>O(D)</code>, where @c D is the number of digits in
 *    <code>n</code>.
 *
 * @ingroup Digits
 */
template <int base = 10, typename T>
T sort_digits(T n)
{
  EULER_DIGITS_VALIDATE_BASE(base);
  EULER_DIGITS_VALIDATE_TYPE(T);
  EULER_DIGITS_VALIDATE_NUMBER(n);

  const int Bits = std::numeric_limits<T>::digits + 1;
  std::array<digit_t, Bits> digits;
  auto d = rdigits<base>(n);
  auto p1 = digits.begin();
  auto p2 = std::copy(d.begin(), d.end(), p1);

  std::sort(p1, p2, std::greater<digit_t>());

  return from_digits<T, base>(p1, p2);
}

/**
 * Counts the number of digits in an integer.
 *
 * @param n A non-negative integer whose digits to count.
 *
 * @returns Number of digits in @c n. If @c n is equal to zero, returns 1.
 *
 * @timecomplexity <code>O(D)</code>, where @c D is the number of digits in
 *    <code>n</code>.
 *
 * @spacecomplexity <code>O(1)</code>.
 *
 * @ingroup Digits
 */
template <int base = 10, typename T>
size_t count_digits(T n)
{
  EULER_DIGITS_VALIDATE_BASE(base);
  EULER_DIGITS_VALIDATE_TYPE(T);
  EULER_DIGITS_VALIDATE_NUMBER(n);

  size_t num_digits = 1;
  while ((n /= base) != 0)
  {
    num_digits++;
  }
  return num_digits;
}

/**
 * Checks whether an integer is palindromic in a given base.
 *
 * An integer is palindromic if its digit expansion reads the same from left
 * to right and from right to left.
 *
 * @param n A non-negative integer to test.
 *
 * @returns @c true if @c n is palindromic, @c false otherwise.
 *
 * @timecomplexity <code>O(log<sub>b</sub>n)</code> operations.
 *
 * @spacecomplexity Constant.
 *
 * @ingroup Digits
 *
 * @todo Preferrably we should find a way to use <code>std::equal</code>
 *      and avoid generating @c C4996 warning from VC++.
 */
template <int base = 10, typename T>
bool is_palindromic(T n)
{
  EULER_DIGITS_VALIDATE_BASE(base);
  EULER_DIGITS_VALIDATE_TYPE(T);
  EULER_DIGITS_VALIDATE_NUMBER(n);

  auto r1 = digits<base>(n);
  auto r2 = rdigits<base>(n);

  // Avoid using std::equal() to avoid VC++ warning C4996 on unchecked iterators.
  // Requires C++14.
  auto it2 = r2.begin();
  for (auto it1 = r1.begin(); it1 != r1.end(); ++it1, ++it2)
  {
    if (*it1 != *it2)
    {
      return false;
    }
  }
  return true;
}

/**
 * Checks whether a digit sequence is pandigital.
 *
 * A digit sequence is pandigital if it contains all of the digits @c lowest
 * to @c highest and each digit appears exactly once.
 *
 * @param begin Input iterator that points to the beginning of the sequence.
 *
 * @param end Input iterator that points past the end of the sequence.
 *
 * @param lowest Smallest digit (inclusive) required to be present.
 *
 * @param highest Largest digit (inclusive) required to be present.
 *
 * @returns @c true if the sequence is pandigital, @c false otherwise.
 *
 * @timecomplexity <code>O(b)</code> operations.
 *
 * @spacecomplexity <code>O(b)</code> (on the stack).
 *
 * @ingroup Digits
 */
template <int base = 10, class InIt>
bool is_pandigital(InIt begin, InIt end, int lowest = 1, int highest = base - 1)
{
  EULER_DIGITS_VALIDATE_BASE(base);

  std::bitset<base> mask;
  int count = 0;

  for (InIt it = begin; it != end; ++it)
  {
    int k = *it;
    if (!(k >= lowest && k <= highest))
    {
      return false;
    }
    if (mask.test(k))
    {
      return false;
    }
    ++count;
    mask.set(k);
  }
  return count == (highest - lowest + 1);
}

/**
 * Checks whether the digit expansion sequence of an integer is pandigital.
 *
 * @tparam base Base of digit expansion.
 * @tparam T Integral type.
 *
 * @param n Integer whose digits are expanded.
 * @param from Smallest digit (inclusive) required to be present.
 * @param to Largest digit (inclusive) required to be present.
 *
 * @returns @c true if the digit expansion of @c n in base @c base contains
 *    each of the digits between @c from and @c to exactly once; @c false
 *    otherwise.
 *
 * @ingroup Digits
 */
template <int base = 10, class T>
bool is_pandigital(T n, int from = 1, int to = base - 1)
{
  auto d = digits(n);
  return is_pandigital(d.begin(), d.end(), from, to);
}

} // namespace euler

#endif
