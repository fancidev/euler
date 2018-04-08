#ifndef EULER_RESIDUE_H
#define EULER_RESIDUE_H

#include "modular.hpp"

namespace euler {

/**
 * Represents the residue of an integer modulo a modulus.
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
	 * Constructs the residue as @c value mod <code>Modulus</code>.
	 * @param value An integer whose residue class is to be created.
	 * @complexity Constant.
	 */
	template <typename TArg>
	residue(TArg value) : x(mod(value, Modulus)) { }

	/**
	 * Constructs the residue zero.
	 * @complexity Constant.
	 */
	residue() : x(0) { }

#if 0
	/**
	 * Assigns a value to the residue.
	 * @complexity Constant.
	 */
	template <typename TArg>
	residue& operator = (const TArg &value) 
	{ 
		x = mod(value, Modulus);
		return *this;
	}
#endif

	/// Returns the modulus of the residue class.
	/// @complexity Constant.
	T modulus() const { return Modulus; }

	/// Returns the residue value.
	/// @returns An integer between @c 0 and <code>(m - 1)</code> (inclusive)
	///      that represents the residue class, where @c m is the modulus.
	/// @complexity Constant.
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

#if 0
	/// Modular addition with an integer.
	/// @complexity Constant.
	residue& operator += (T a)
	{
		return operator += (residue(a));
	}
#endif

	/// Modular subtraction.
	/// @complexity Constant.
	residue& operator -= (const residue &a)
	{
		x = modsub(x, a.x, modulus());
		return *this;
	}

#if 0
	/// Modular subtraction with an integer.
	/// @complexity Constant.
	residue& operator -= (T a)
	{
		return operator -= (residue(a));
	}
#endif

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

/// Outputs a residue.
/// @complexity Constant.
/// @ingroup ModularArithmetic
template <typename T, T Modulus>
std::ostream& operator << (std::ostream &os, const residue<T,Modulus> &x)
{
	return os << x.value();
}

/// Tests whether two residue classes are equivalent.
/// @complexity Constant.
/// @ingroup ModularArithmetic
template <typename T, T Modulus>
bool operator == (const residue<T,Modulus> &a, const residue<T,Modulus> &b)
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

/// Adds two residues.
/// @complexity Constant.
/// @ingroup ModularArithmetic
template <typename T, T Modulus>
residue<T,Modulus> operator + (const residue<T,Modulus> &a, const residue<T,Modulus> &b)
{
	return modadd(a.value(), b.value(), Modulus);
}

/// Adds two residues.
/// @complexity Constant.
/// @ingroup ModularArithmetic
template <typename T, T Modulus, class TArg>
residue<T,Modulus> operator + (const residue<T,Modulus> &a, const TArg &b)
{
	return operator + (a, residue<T,Modulus>(b));
}

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

/// Subtracts two residues.
/// @complexity Constant.
/// @ingroup ModularArithmetic
template <class T, T Modulus>
residue<T,Modulus> operator - (const residue<T,Modulus> &a, const residue<T,Modulus> &b)
{
	return modsub(a.value(), b.value(), Modulus);
}

/// Multiplies two residues.
/// @ingroup ModularArithmetic
template <class T, T Modulus>
residue<T,Modulus> operator * (const residue<T,Modulus> &a, const residue<T,Modulus> &b)
{
	return modmul(a.value(), b.value(), Modulus);
}

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

/**
 * Divides two residues. This is equivalent to multiplying the first residue
 * by the modular multiplicative inverse of the second residue. If the second
 * residue is not coprime to the modulus, a division-by-zero exception is
 * thrown.
 *
 * @timecomplexity <code>O(log m)</code>.
 * @spacecomplexity Constant.
 *
 * @ingroup ModularArithmetic
 */
template <typename T, T Modulus>
residue<T,Modulus> operator / (const residue<T,Modulus> &a, const residue<T,Modulus> &b)
{
	return modmul(a, modinv(b.value(), Modulus), Modulus);
}

template <typename T, T Modulus, typename TExponent>
residue<T,Modulus> operator ^ (const residue<T,Modulus> &base, TExponent exponent)
{
	return modpow(base.value(), exponent, Modulus);
}

#if 0
template <long long Modulus>
class residue_class : public residue<long long, Modulus> { };

template <int Modulus>
class residue_class : public residue<int, Modulus> { };
#endif

} // namespace euler

#endif // EULER_RESIDUE_H
