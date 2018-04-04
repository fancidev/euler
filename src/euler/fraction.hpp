/**
 * @defgroup Fraction Rational Number Arithmetics
 * @ingroup Library
 */

#ifndef EULER_FRACTION_H
#define EULER_FRACTION_H

#include <iostream>
#include "gcd.hpp"
#include "int_traits.hpp"

namespace euler {

/**
 * Represents a rational number in reduced fraction form.
 * @ingroup Fraction
 */
template <typename T>
class fraction
{
	T p; // numerator
	T q; // denominator, must be > 0.

public:

	/// Constructs the fraction <code>0/1</code>.
	/// @complexity Constant.
	fraction() : p(0), q(1) {}

	/// Constructs the fraction <code>n/1</code>.
	/// @complexity Constant.
	fraction(T n) : p(n), q(1) { }

	/// Constructs the fraction <code>p/q</code>.
	/// @param numerator The numerator.
	/// @param denominator The denominator. Must be positive.
	/// @complexity Constant.
	/// @remarks @c p and @c q must be coprime.
	fraction(T numerator, T denominator) : p(numerator), q(denominator) { }

	/// Returns the numerator of the fraction.
	/// @complexity Constant.
	T numerator() const { return p; }

	/// Returns the denominator of the fraction.
	/// @complexity Constant.
	T denominator() const { return q; }

	/// Returns the inverse of the fraction.
	/// @complexity Constant.
	/// @todo If the fraction is zero, an exception should be thrown.
	fraction inverse() const 
	{
		if (p < 0)
			return fraction(-q,-p);
		else
			return fraction(q,p); 
	}
};

/// Outputs a fraction to a stream in the form <code>p/q</code>.
/// @complexity Constant.
/// @ingroup Fraction
template <typename T>
std::ostream& operator << (std::ostream &os, const fraction<T> &f)
{
	return os << f.numerator() << '/' << f.denominator();
}

/// Tests whether two fractions are equal.
/// @complexity Constant.
/// @ingroup Fraction
/// @remarks The fractions must be stored in reduced form.
template <typename T>
bool operator == (const fraction<T> &a, const fraction<T> & b)
{
	return a.numerator() == b.numerator() && a.denominator() == b.denominator();
}

/// Adds two fractions.
/// @complexity Constant.
/// @ingroup Fraction
template <typename T>
fraction<T> operator + (const fraction<T> &a, const fraction<T> &b)
{
	typedef int_traits<T>::wide_type calc_t;
	calc_t p1 = a.numerator(), q1 = a.denominator();
	calc_t p2 = b.numerator(), q2 = b.denominator();
	calc_t p = p1*q2+p2*q1, q=q1*q2;
	calc_t d = gcd(p,q);
	return fraction<T>((T)(p/d), (T)(q/d));
}

#if 0
template <typename T>
bool operator == (const fraction<T> &a, T b)
{
	typedef int_traits<T>::wide_type calc_t;
	calc_t p = a.numerator(), q = a.denominator();
	return q*b == p;
}
#endif

} // namespace euler

#endif // EULER_FRACTION_H
