/**
 * @defgroup residue Modular Arithmetic (Classes)
 *
 * Class-based interface to modular arithmetic operations.
 *
 * @ingroup Library
 */

#ifndef EULER_RESIDUE_H
#define EULER_RESIDUE_H

#include "modular.hpp"

namespace euler {

/**
 * Represents a residue class in a residue system modulo a given modulus.
 *
 * This class supports basic modular arithmetic operations including addition,
 * subtraction, multiplication, multiplicative inverse, and exponentiation.
 * As long as the modulus is of a built-in integral type, the operations are
 * guaranteed to be free of integer overflow problem.
 *
 * The <modular.hpp> header provides a function-based interface to modular
 * arithmetic operations.
 *
 * For more information, see the following pages.
 *    - http://en.wikipedia.org/wiki/Modular_arithmetic
 *
 * @ingroup residue
 */
template <class T, T M>
class residue
{
  static_assert(std::is_integral<T>::value, "T must be an integral type.");
  static_assert(M >= 2, "Modulus M must be greater than or equal to 2.");

  T x;

public:

  /**
   * Constructs the residue class @c value modulo <code>M</code>.
   *
   * @param value An integer whose residue class is to be created.
   *
   * @complexity Constant.
   */
  residue(T value) : x(mod(value, M)) { }

  /**
   * Constructs the residue class @c 0 modulo <code>M</code>.
   *
   * @complexity Constant.
   */
  residue() : x(0) { }

  /**
   * Gets the modulus of the residue system.
   *
   * @returns Template parameter @c M.
   *
   * @complexity Constant.
   * */
  T modulus() const { return M; }

  /**
   * Gets the representative of the residue class.
   *
   * @returns The unique integer between <code>[0, M)</code> to which all
   * integers in the residue class are congruent.
   *
   * @complexity Constant.
   */
  T value() const { return x; }

  /**
   * In-place residue addition.
   *
   * @complexity Constant.
   */
  residue& operator += (const residue &other)
  {
    x = modadd(x, other.x, modulus());
    return *this;
  }

  /**
   * In-place residue subtraction.
   *
   * @complexity Constant.
   */
  residue& operator -= (const residue &other)
  {
    x = modsub(x, other.x, modulus());
    return *this;
  }

  /**
   * In-place residue multiplication.
   *
   * @see <code>modmul()</code>.
   */
  residue& operator *= (const residue &other)
  {
    x = modmul(x, other.x, modulus());
    return *this;
  }
};

/**
 * Outputs the representative of a residue class to a stream.
 *
 * @param os An output stream.
 *
 * @param x A residue class.
 *
 * @returns Reference to the output stream.
 *
 * @complexity Constant.
 *
 * @ingroup residue
 */
template <typename T, T M>
std::ostream& operator << (std::ostream &os, const residue<T, M> &x)
{
  return os << x.value();
}

/**
 * Tests whether two residue classes are equivalent.
 *
 * @complexity Constant.
 *
 * @ingroup residue
 */
template <typename T, T M>
bool operator == (const residue<T, M> &a, const residue<T, M> &b)
{
  return (a.value() == b.value());
}

/**
 * Tests whether an integer belongs to a residue class.
 *
 * @complexity Constant.
 *
 * @ingroup residue
 */
template <class T, T M>
bool operator==(const residue<T, M> &a, const T &b)
{
  return a.value() == mod(b, M);
}

#if 0
/// Tests whether an integer belongs to a residue class.
/// @complexity Constant.
/// @ingroup residue
template <typename T1, typename T2, T2 Modulus>
bool operator == (const T1 &a, const residue<T2,Modulus> &b)
{
  return b == a;
}
#endif

/**
 * Residue addition.
 *
 * @param a A residue class modulo @c M.
 *
 * @param b A residue class modulo @c M.
 *
 * @returns Residual class @c c modulo @c M such that for any @c x in @c a,
 *    @c y in @c b and @c z in @c c, <code>x + y == z (mod M)</code>.
 *
 * @complexity Constant.
 *
 * @ingroup residue
 */
template <class T, T M>
residue<T, M> operator+(const residue<T, M> &a, const residue<T, M> &b)
{
  return residue<T, M>(modadd(a.value(), b.value(), M));
}

template <class T, T M, class S>
std::enable_if_t<std::is_convertible<S, T>::value, residue<T, M>>
operator+(const residue<T, M> &a, const S &b)
{
  return a + residue<T, M>(b);
}

template <class T, T M, class S>
std::enable_if_t<std::is_convertible<S, T>::value, residue<T, M>>
operator+(const S &a, const residue<T, M> &b)
{
  return residue<T, M>(a) + b;
}

/**
 * Residue subtraction.
 *
 * @param a A residue class modulo @c M.
 *
 * @param b A residue class modulo @c M.
 *
 * @returns Residue class @c c modulo @c M such that for any @c x in @c a,
 *    @c y in @c b and @c z in @c c, <code>x - y == z (mod M)</code>.
 *
 * @complexity Constant.
 *
 * @ingroup residue
 */
template <class T, T M>
residue<T, M> operator-(const residue<T, M> &a, const residue<T, M> &b)
{
  return residue<T, M>(modsub(a.value(), b.value(), M));
}

/**
 * Residue negation.
 *
 * @param a A residue class modulo @c M.
 *
 * @returns Residue class @c b modulo @c M such that for any @c x in @c a and
 *    @c y in @c b, <code>x + y == 0 (mod M)</code>.
 *
 * @complexity Constant.
 *
 * @ingroup residue
 */
template <class T, T M>
residue<T, M> operator-(const residue<T, M> &a)
{
  return residue<T, M>(modsub(T(0), a.value(), M));
}

/**
 * Residue multiplication.
 *
 * @param a A residue class modulo @c M.
 *
 * @param b A residue class modulo @c M.
 *
 * @returns Residue class @c c modulo @c M such that for any @c x in @c a,
 *    @c y in @c b and @c z in @c c, <code>x * y == z (mod M)</code>.
 *
 * @complexity Constant.
 *
 * @ingroup residue
 */
template <class T, T M>
residue<T, M> operator*(const residue<T, M> &a, const residue<T, M> &b)
{
  return residue<T, M>(modmul(a.value(), b.value(), M));
}

template <class T, T M, class S>
std::enable_if_t<std::is_convertible<S, T>::value, residue<T, M>>
operator*(const residue<T, M> &a, const S &b)
{
  return a * residue<T, M>(b);
}

template <class T, T M, class S>
std::enable_if_t<std::is_convertible<S, T>::value, residue<T, M>>
operator*(const S &a, const residue<T, M> &b)
{
  return residue<T, M>(a) * b;
}

/**
 * Residue division.
 *
 * This is equivalent to multiplying the first residue by the modular
 * multiplicative inverse of the second residue. If the second residue is not
 * coprime to the modulus, a division-by-zero exception is thrown.
 *
 * @param a A residue class modulo @c M.
 *
 * @param b A residue class modulo @c M.
 *
 * @returns Residue class @c c modulo @c M such that for any @c x in @c a,
 *    @c y in @c b and @c z in @c c, <code>x == y * z (mod M)</code>, if such
 *    @c c exists and is unique.
 *
 * @exception Exception if no solution or more than one solution exists.
 *
 * @timecomplexity <code>O(log M)</code>.
 *
 * @spacecomplexity Constant.
 *
 * @ingroup residue
 */
template <typename T, T M>
residue<T, M> operator / (const residue<T, M> &a, const residue<T, M> &b)
{
  return modmul(a.value(), modinv(b.value(), M), M);
}

/**
 * Modular exponentiation.
 *
 * @param a A residue class to raise.
 *
 * @param k Exponent.
 *
 * @returns Residue class @c c modulo @c M such that for any @c x in @c a and
 *    @c z in @c c, <code>a ** b == c (mod M)</code>.
 */
template <typename T, T M, typename TExponent>
residue<T, M> operator^(const residue<T, M> &a, TExponent k)
{
  return residue<T, M>(modpow(a.value(), k, M));
}

} // namespace euler

#endif // EULER_RESIDUE_H
