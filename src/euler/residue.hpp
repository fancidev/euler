#ifndef EULER_RESIDUE_H
#define EULER_RESIDUE_H

#include "modular.hpp"

namespace euler {

/**
 * Represents a residue class in a residue system modulo a given modulus.
 *
 * This class supports the basic modular operations: addition, subtraction,
 * multiplication, and exponentiation. As long as the modulus is of a
 * built-in integral type, the operations are guaranteed to be free of integer
 * overflow problem.
 *
 * For more information, see the following pages.
 *    - http://en.wikipedia.org/wiki/Modular_arithmetic
 *
 * @ingroup ModularArithmetic
 */
template <class T, T Modulus>
class residue
{
  T x;

public:

  /**
   * Constructs the residue class @c value mod <code>Modulus</code>.
   *
   * @param value An integer whose residue class is to be created.
   *
   * @complexity Constant.
   */
  template <typename TArg>
  explicit residue(TArg value) : x(mod(value, Modulus)) { }

#if 0
  /**
   * Constructs the residue zero.
   *
   * @complexity Constant.
   */
  residue() : x(0) { }
#endif

  /**
   * Gets the modulus of the residue system.
   *
   * @returns Modulus of the residue system.
   *
   * @complexity Constant.
   * */
  T modulus() const { return Modulus; }

  /**
   * Gets the representative of the residue class.
   *
   * @returns An integer between @c 0 and <code>(Modulus - 1)</code>
   *   (inclusive) to which all integers in the residue class are congruent.
   *
   * @complexity Constant.
   */
  T value() const { return x; }

#if 0
  /// Tests whether the residue class is empty.
  /// @returns @c true if the residue class is empty, @c false otherwise.
  /// @remarks An empty residue class may be returned by certain functions
  ///      (such as @c modinv) to indicate failure. It can also be explicitly
  ///      constructed by calling the the constructor without parameters.
  /// @complexity Constant.
  bool empty() const { return m == 0; }
#endif

  /// Modular addition.
  /// @complexity Constant.
  residue& operator += (const residue &a)
  {
    x = modadd(x, a.x, modulus());
    return *this;
  }

  /// Modular subtraction.
  /// @complexity Constant.
  residue& operator -= (const residue &a)
  {
    x = modsub(x, a.x, modulus());
    return *this;
  }

#if 0
  residue& operator *= (const residue &a)
  {
    assert(a.m == m);
    x = multiply(x, a.x, m,
      typename std::is_void<typename euler::int_traits<T>::wide_type>());
    return *this;
  }
#endif

#if 0
  /// Modular multiplication with an integer.
  residue& operator *= (T a)
  {
    return operator *= (residue(a, m));
  }
#endif
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
 * @ingroup ModularArithmetic
 */
template <typename T, T Modulus>
std::ostream& operator << (std::ostream &os, const residue<T, Modulus> &x)
{
  return os << x.value();
}

/**
 * Tests whether two residue classes are equivalent.
 *
 * @complexity Constant.
 *
 * @ingroup ModularArithmetic
 */
template <typename T, T Modulus>
bool operator == (const residue<T, Modulus> &a, const residue<T, Modulus> &b)
{
  return (a.value() == b.value());
}

/// Tests whether an integer belongs to a residue class.
/// @complexity Constant.
/// @ingroup ModularArithmetic
template <typename T, T Modulus>
bool operator == (const residue<T,Modulus> &a, const T &b)
{
  return (a.value() == b) || (a.value() == mod(b, Modulus));
}

/// Tests whether an integer belongs to a residue class.
/// @complexity Constant.
/// @ingroup ModularArithmetic
template <typename T1, typename T2, T2 Modulus>
bool operator == (const T1 &a, const residue<T2,Modulus> &b)
{
  return b == a;
}

/**
 * Adds two residue classes.
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
 * @ingroup ModularArithmetic
 */
template <typename T, T M>
residue<T, M> operator + (const residue<T, M> &a, const residue<T, M> &b)
{
  return residue<T, M>(modadd(a.value(), b.value(), M));
}

#if 0
/// @complexity Constant.
/// @ingroup ModularArithmetic
template <typename T, T Modulus, class TArg>
residue<T,Modulus> operator + (const residue<T,Modulus> &a, const TArg &b)
{
  return operator + (a, residue<T,Modulus>(b));
}
#endif

#if 0
/// Adds two residues.
/// @complexity Constant.
/// @ingroup ModularArithmetic
template <class T1, class T2>
residue<T2> operator + (const T1 &a, const residue<T2> &b)
{
  return residue<T2>(b) += a;
}
#endif

/**
 * Subtracts two residue classes.
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
 * @ingroup ModularArithmetic
 */
template <class T, T M>
residue<T, M> operator - (const residue<T, M> &a, const residue<T, M> &b)
{
  return residue<T, M>(modsub(a.value(), b.value(), M));
}

/**
 * Multiplies two residue classes.
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
 * @ingroup ModularArithmetic
 */
/// Multiplies two residues.
/// @ingroup ModularArithmetic
template <class T, T M>
residue<T, M> operator * (const residue<T, M> &a, const residue<T, M> &b)
{
  return residue<T, M>(modmul(a.value(), b.value(), M));
}

#if 0
/// Multiplies two residues.
/// @ingroup ModularArithmetic
template <class T, T Modulus, class TArg>
residue<T,Modulus> operator * (const residue<T,Modulus> &a, const TArg &b)
{
  return operator * (a, residue<T,Modulus>(b));
}

/// Multiplies two residues.
/// @ingroup ModularArithmetic
template <class T, T Modulus, class TArg>
residue<T,Modulus> operator * (const TArg &a, const residue<T,Modulus> &b)
{
  return operator * (residue<T,Modulus>(a), b);
}
#endif

/**
 * Divides two residues. This is equivalent to multiplying the first residue
 * by the modular multiplicative inverse of the second residue. If the second
 * residue is not coprime to the modulus, a division-by-zero exception is
 * thrown.
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
 * @ingroup ModularArithmetic
 */
template <typename T, T M>
residue<T, M> operator / (const residue<T, M> &a, const residue<T, M> &b)
{
  return modmul(a, modinv(b.value(), M), M);
}

/**
 * Modular exponentiation.
 *
 * @param a A residue class to raise.
 *
 * @param b An integer exponent.
 *
 * @returns Residue class @c c modulo @c M such that for any @c x in @c a and
 *    @c z in @c c, <code>a ** b == c (mod M)</code>.
 */
template <typename T, T M, typename TArg>
residue<T, M> operator ^ (const residue<T, M> &a, const TArg &b)
{
  return residue<T, M>(modpow(a.value(), b, M));
}

} // namespace euler

#endif // EULER_RESIDUE_H
