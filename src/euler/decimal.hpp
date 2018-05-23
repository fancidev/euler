/**
 * @defgroup Decimal Decimal Arithmetics
 *
 * @ingroup Library
 *
 * This module implements unsigned big integer arithmetics with digits stored
 * in user-specified radix. The module illustrates the basic algorithms for
 * software arithmetics; it is not intended to be a high-performance library.
 */

#ifndef EULER_DECIMAL_H
#define EULER_DECIMAL_H

#include <array>
#include <cstdint>
#include <iostream>
#include <vector>
#include "imath.hpp"
#include "sequence.hpp"

namespace euler {

/**
 * Represents an unsigned integer with digits represented in a given radix.
 *
 * @tparam Base Base of digit representation; must be between 2 and 16
 *    (inclusive). Digit expansion and digit shift operations are performed in
 *    this base.
 *
 * @see <i>The Art of Computer Programming</i> for more information on
 *    implementing software arithmetic.
 *
 * @ingroup Decimal
 */
template <size_t Base>
class integer
{
  static_assert(Base >= 2 && Base <= 16, "Base must be between 2 and 16.");

  using digit_t = uint8_t;
  using store_t = std::vector<digit_t>;

  store_t _digits; // least significant digit first; empty if 0

  static const store_t& _zero_digits()
  {
    static const store_t zero(1);
    return zero;
  }

public:

  /**
   * Creates an integer of value zero.
   */
  integer() : _digits() { }

  /**
   * Creates an integer of the given value.
   */
  integer(uint64_t n) : _digits()
  {
    while (n > 0)
    {
      _digits.push_back(static_cast<digit_t>(n % Base));
      n /= Base;
    }
  }

  /**
   * Parses an integer from a string.
   *
   * Supported characters in the string are <c>0-9</c>, which represent the
   * respective digit as is, and <c>A-Z</c> or <c>a-z</c>, which represent
   * digits from 10 to 15.
   *
   * Parsing stops at the first character that is not a digit character. If
   * the input string does not contain any digit character, the returned value
   * is zero.
   *
   * @param s String containing the digit expansion of an integer in base
   *    <c>Base</c>.
   */
  static integer parse(const char *s)
  {
    // Default value is 0
    integer x;

    // Skip leading zeros.
    while (*s == '0')
    {
      s++;
    }

    // Input the digits.
    for (; *s != '\0'; s++)
    {
      char c = *s;
      int d = (c >= '0' && c <= '9')? c - '0' :
        (c >= 'A' && c <= 'Z')? c - 'A' + 10 :
        (c >= 'a' && c <= 'z')? c - 'a' + 10 : -1;
      if (d < 0 || d >= static_cast<int>(Base))
      {
        break;
      }
      x._digits.push_back(d);
    }

    // Reverse the digits into proper order.
    std::reverse(x._digits.begin(), x._digits.end());
    return x;
  }

#if 0
  /**
   * Right-shifts the digits of the integer in-place.
   *
   * @param shift Number of digits to shift to the right.
   *
   * @returns The integer after digit shift.
   */
  integer& operator>>=(size_t shift)
  {
    if (shift >= _digits.size()) // zero
    {
      _digits.erase(_digits.begin(), _digits.end());
    }
    else
    {
      _digits.erase(_digits.begin(), _digits.begin() + shift);
    }
    return *this;
  }
#endif

  /**
   * Integer addition and assignment.
   *
   * @param other Operand.
   */
  integer& operator+=(const integer &other)
  {
    *this = *this + other;
    return *this;
  }

  /**
   * Integer addition.
   */
  friend integer operator+(const integer &a, const integer &b)
  {
    const size_t n1 = a._digits.size();
    const size_t n2 = b._digits.size();
    const size_t n = std::max(n1, n2);

    integer c;
    digit_t carry = 0;
    for (size_t i = 0; i < n; i++)
    {
      int t = (i < n1 ? a._digits[i] : 0) +
              (i < n2 ? b._digits[i] : 0) +
              carry;
      if (t >= static_cast<int>(Base))
      {
        carry = 1;
        t -= Base;
      }
      else
      {
        carry = 0;
      }
      c._digits.push_back(static_cast<digit_t>(t));
    }
    if (carry != 0)
    {
      c._digits.push_back(1);
    }
    return c;
  }

  /**
   * Integer multiplication.
   */
  friend integer operator*(const integer &a, uint64_t b)
  {
    return ipow(a, b, std::plus<integer>(), integer());
  }

  /**
   * Iterator that enumerates digits from right (least significant) to left
   * (most significant).
   */
  using const_iterator = typename store_t::const_reverse_iterator;

  /**
   * Iterator that enumerates digits from right (least significant) to left
   * (most significant).
   */
  using const_reverse_iterator = typename store_t::const_iterator;

  sequence<const_iterator> digits() const
  {
    if (_digits.empty())
    {
      return make_sequence(_zero_digits().rbegin(), _zero_digits().rend());
    }
    else
    {
      return make_sequence(_digits.rbegin(), _digits.rend());
    }
  }

  sequence<const_reverse_iterator> rdigits() const
  {
    if (_digits.empty())
    {
      return make_sequence(_zero_digits().begin(), _zero_digits().end());
    }
    else
    {
      return make_sequence(_digits.begin(), _digits.end());
    }
  }
};

template <size_t Base>
sequence<typename integer<Base>::const_iterator>
digits(const integer<Base> &x)
{
  return x.digits();
}

template <size_t Base>
sequence<typename integer<Base>::const_reverse_iterator>
rdigits(const integer<Base> &x)
{
  return x.rdigits();
}

/**
 * Outputs an integer to a stream.
 *
 * @param os Output stream.
 *
 * @param x Integer to output.
 *
 * @returns <c>os</c>.
 *
 * @complexity <code>O(L)</code> where @c L is the number of digits in @c x.
 *
 * @ingroup Decimal
 */
template <size_t Base>
std::ostream& operator<<(std::ostream &os, const integer<Base> &x)
{
  for (int c: digits(x))
  {
    os << static_cast<char>(c <= 9? '0' + c : c - 10 + 'a');
  }
  return os;
}

using decimal = integer<10>;

} // namespace euler

#endif // EULER_DECIMAL_H
