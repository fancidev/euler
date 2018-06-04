/**
 * @defgroup Roman Roman Numeral Input/Output
 * Routines to convert integers to and from Roman numeral form.
 *
 * @ingroup Library
 *
 * Roman numerals is a number system that writes an integer as a sequence
 * of Latin alphabets. The following seven symbols are defined: <code>
 * I = 1, V = 5, X = 10, L = 50, C = 100, D = 500, M = 1000</code>.
 *
 * There are certain rules to convert an integer to and from Roman numeral
 * form. These rules are implemented in the <code>roman_numeral</code> class
 * of this module which supports reading/writing an integer from/to a 
 * standard stream in Roman numeral form.
 *
 * For more information regarding the definition and use of Roman numerals,
 * see
 *    - http://en.wikipedia.org/wiki/Roman_numerals
 */

#ifndef EULER_ROMAN_NUMERAL_HPP
#define EULER_ROMAN_NUMERAL_HPP

#if 0
// I tried to implement a facet, but it doesn't work unfortunately.
struct roman_num_put : std::num_put<char>
{
  static int idx()
  {
    static int i = std::ios_base::xalloc();
    return i;
  }

    virtual iter_type do_put(iter_type out, std::ios_base& str, char_type fill, long v) const
  {
    *out = 'H';
    return ++out;
        //return num_put<char>::do_put(out, str, fill, v);
    }

    virtual iter_type do_put(iter_type out, std::ios_base& str, char_type fill, unsigned long v) const
  {
    *out = 'H';
    return ++out;
        //return num_put<char>::do_put(out, str, fill, v + str.iword(idx()));
    }

  friend std::ostream& roman(std::ostream& os)
  {
    os.iword(idx()) = 1;
    return os;
  }

  friend std::ostream& noroman(std::ostream& os)
  {
    os.iword(idx()) = 0;
    return os;
  }
};
#endif

namespace euler {

/**
 * Wraps an integer that should be serialized in Roman numeral form.
 * @ingroup Roman
 */
template <typename T>
class roman_numeral
{
  T& _value;

public:

  /// Constructs the wrapper object.
  /// @param value Reference to an integer variable to wrap.
  /// @complexity Constant.
  roman_numeral(T& value) : _value(value) { }

  /// Returns a reference to the underlying variable.
  /// @complexity Constant.
  T& value() { return _value; }

  /// Returns the value of the underlying variable.
  /// @complexity Constant.
  T value() const { return _value; }
};

/**
 * Wraps an integer so that it can be serialized in Roman numeral form.
 * @param vref Reference to an integer variable to wrap.
 * @returns A @c roman_numeral object that wraps the variable.
 * @complexity Constant.
 * @ingroup Roman
 */
template <typename T>
roman_numeral<T> roman(T &vref)
{
  return roman_numeral<T>(vref);
}

/**
 * Reads an integer in Roman numeral form from an input stream.
 *
 * @param is The input stream.
 * @param x A @c roman_numeral object that wraps an integer where the input
 *      value is stored.
 * @returns The input stream @c is.
 * @timecomplexity <code>O(L)</code> where @c L is the number of symbols in
 *      the input Roman numeral.
 * @spacecomplexity Constant.
 *
 * @ingroup Roman
 */
template <typename T>
std::istream& operator >> (std::istream &is, roman_numeral<T> x)
{
  // I = 1, V = 5, X = 10, L = 50, C = 100, D = 500, M = 1000
  T& n = x.value();
  n = 0;
  char c;
  for (int last = 0; is >> c; )
  {
    int v = 0;
    switch (c)
    {
    case 'M': v = 1000; break;
    case 'D': v = 500; break;
    case 'C': v = 100; break;
    case 'L': v = 50; break;
    case 'X': v = 10; break;
    case 'V': v = 5; break;
    case 'I': v = 1; break;
    }
    if (v == 0)
      break;
    n += v;
    if (last < v)
      n -= 2*last;
    last = v;
  }
  return is;
}

/**
 * Writes an integer in standard Roman numeral form to an output stream.
 *
 * @param os The output stream.
 * @param x The integer to output (wrapped in a @c roman_numeral object).
 * @returns The output stream @c os.
 * @timecomplexity <code>O(n)</code> where @c n is the value to output.
 * @spacecomplexity Constant.
 *
 * @ingroup Roman
 */
template <typename T>
std::ostream& operator << (std::ostream &os, const roman_numeral<T> &x)
{
  // I = 1, V = 5, X = 10, L = 50, C = 100, D = 500, M = 1000
  T n = x.value();

  struct presentation_t
  {
    int threshold;
    const char *text;
  };
  const static presentation_t p[] = {
    { 1000, "M" },
    { 900, "CM" },
    { 500, "D" },
    { 400, "CD" },
    { 100, "C" },
    { 90, "XC" },
    { 50, "L" },
    { 40, "XL" },
    { 10, "X" },
    { 9, "IX" },
    { 5, "V" },
    { 4, "IV" },
    { 1, "I" },
  };
  for (int i = 0; n > 0; )
  {
    if (n >= p[i].threshold)
    {
      os << p[i].text;
      n -= p[i].threshold;
    }
    else
    {
      i++;
    }
  }

  return os;
}

} // namespace euler

#endif // EULER_ROMAN_NUMERAL_HPP
