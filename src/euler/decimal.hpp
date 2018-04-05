/**
 * @defgroup Decimal Decimal Arithmetic
 * @ingroup Library
 *
 * This module provides support for fixed-precision decimal arithmetic, 
 * such as addition, subtraction, and multiplication. In the current 
 * release, only integers are supported. In some future release, floating 
 * point arithmetic may be supported.
 *
 * This module aims to demonstrate how decimal arithmetic could be 
 * implemented. It is not supposed to be a high performance big integer
 * library and should not be used for that purpose.
 *
 * There are three main reasons why decimal (instead of binary) arithmetic
 * is implemented. First, implementing the arithmetic in decimal (or other
 * bases other than binary) demonstrates the algorithm in general, without
 * relying on specific simplifications coming from a binary representation.
 * Second, most problems in Project Euler where a big integer is actually 
 * needed to compute the result requires to access the decimal digit of 
 * the result, which makes a direct decimal implementation natural. Third,
 * by implementing interval arithmetics on top of suitably rounded floating
 * point decimals, this could be used to illustrate the basics in error 
 * analysis which are typically carried out in decimal for visual simplicity.
 */

#ifndef EULER_DECIMAL_H
#define EULER_DECIMAL_H

#include <iostream>
#include <array>
#include "sequence.hpp"

namespace euler {

/**
 * Represents a fixed-precision decimal number.
 *
 * @ingroup Decimal
 * @todo We could implement also a binary fixed-width big integer class 
 *      which could then be used in say VC++ AMP (which only supports up
 *      to 32 bits).
 * @todo We could generalize the base and data type a little bit to easily
 *      support say base 256 or 2^32, so that efficient binary arithmetic
 *      could be done.
 * @todo Consult <i>The Art of Computer Programming</i> to find out the
 *      proper way to implement software arithmetic.
 */
template <int NDigit, int NExponent = 0, unsigned char base = 10>
class decimal
{
	typedef unsigned char TDigit;
	typedef int TCalc;

	bool of;  // overflow flag
	bool neg; // true if negative
	int nd;   // number of significant digits
	std::array<TDigit,NDigit> d; // digits, least significant bit first

public:

	/// Constructs a decimal of value zero.
	/// @timecomplexity <code>O(D)</code>.
	/// @spacecomplexity <code>O(D)</code>.
	decimal() : of(false), neg(false), nd(0), d() { }

	/// Constructs a decimal of a given integral value.
	/// @timecomplexity <code>O(D)</code>.
	/// @spacecomplexity <code>O(D)</code>.
	decimal(unsigned int n) : of(false), neg(false), nd(0)
	{
		for (int i = 0; i < NDigit; i++)
		{
			if ((d[i] = n % base) != 0)
				nd = i + 1;
			n /= base;
		}
		of = (n > 0);
	}

	/**
	 * Constructs a decimal from a string.
	 *
	 * Supported characters in the string are <code>0-9</code>, which 
	 * represent the digits as is, and <code>A-Z</code> or <code>a-z</code>,
	 * which represent digits greater than 9. Hence,  The largest base 
	 * supported by this function is 36.
	 *
	 * If there are more digits in the input than <code>NDigit</code>, 
	 * the <code>overflow()</code> flag is set and the lowest @c NDigit
	 * digits are kept.
	 *
	 * If the input string is empty, the number is treated as zero. If 
	 * there are invalid digits in the input string, it is treated as 
	 * end-of-string.
	 *
	 * @param s The string containing the expansion of the number in base
	 *      <code>base</code>.
	 * @timecomplexity <code>O(max(D,L))</code> where @c L is the length
	 *      of the input string.
	 * @spacecomplexity <code>O(D)</code>.
	 */
	decimal(const char *s) : of(false), neg(false), nd(0), d()
	{
		// Ignore leading zeros.
		while (*s == '0')
			s++;

		// Input the digits.
		for (; *s; s++, nd++)
		{
			char c = *s;
			int x = (c >= '0' && c <= '9')? c - '0' :
				(c >= 'A' && c <= 'Z')? c - 'A' + 10 :
				(c >= 'a' && c <= 'z')? c - 'a' + 10 : -1;
			if (x < 0 || x >= base)
				break;
			d[nd % NDigit] = static_cast<TDigit>(x);
		}

		// If the input is too long, truncate it and set the overflow flag.
		if (nd > NDigit)
		{
			std::array<TDigit,NDigit> tmp(d);
			for (int i = 0; i < NDigit; i++)
			{
				d[i] = tmp[(nd+i)%NDigit];
			}
			nd = NDigit;
			of = true;
		}

		// Reverse the digits into proper order.
		std::reverse(d.begin(), d.begin() + nd);
	}

	/// Tests if the decimal is overflowed.
	/// @returns @c true if the number if overflowed, @c false otherwise.
	/// @complexity Constant.
	bool overflow() const { return of; }

	/// Tests if the decimal is negative.
	/// @returns @c true if the number is negative, @c false if the number is
	///      zero or positive.
	/// @complexity Constant.
	bool negative() const { return neg; }

	/**
	 * Logical right shifts the number.
	 * @param shift The number of digits to shift.
	 * @returns The number after the shift.
	 * @timecomplexity <code>O(D)</code>.
	 * @spacecomplexity Constant.
	 */
	decimal& operator >>= (int shift)
	{
		if (shift >= nd) // zero
		{
			std::fill(d.begin(), d.begin() + nd, TDigit(0));
			nd = 0;
		}
		else
		{
			std::copy(d.begin() + shift, d.begin() + nd, d.begin());
			std::fill(d.begin() + nd - shift, d.begin() + nd, TDigit(0));
			nd -= shift;
		}
		return *this;
	}

	/**
	 * Adds another number to this number.
	 * @param x The number to add to this number.
	 * @returns The number after addition. If the result is too big to be
	 *      represented with @c D digits, the <code>overflow()</code> flag
	 *      is set.
	 * @timecomplexity <code>O(D)</code>.
	 * @spacecomplexity Constant.
	 */
	decimal& operator += (const decimal &x)
	{
		// Be careful if &x == this.
		TDigit carry = 0;
		nd = std::max(nd, x.nd);
		for (int i = 0; i < nd; i++)
		{
			TCalc t = (TCalc)d[i] + (TCalc)x.d[i] + (TCalc)carry;
			if (t >= base)
			{
				carry = 1;
				t -= base;
			}
			else
			{
				carry = 0;
			}
			d[i] = (TDigit)t;
		}
		if (carry)
		{
			if (nd >= NDigit)
				of = true;
			else
				d[nd++] = 1;
		}
		return *this;
	}


#if 0
	// Multiplication
	// Can we use rvalue reference to improve performance of Multiplication
	// of two decimals?
	decimal& operator *= (const decimal &x)
	{
		return *this;
	}
#endif

#if 0
	/// Returns the <code>k</code>th digit in the number.
	int digit(int k) const { return d[k]; }
#endif

	/// Iterator that enumerates the digits from right (least significant)
	/// to left (most significant).
	typedef typename std::array<TDigit,NDigit>::const_iterator const_reverse_iterator;

	/// Returns an iterator that initially points to the least significant
	/// digit, and moves to more significant digits when advanced.
	/// @complexity Constant.
	const_reverse_iterator digit_rbegin() const 	{ return d.begin(); }

	/// Returns an iterator that points past the most significant digit.
	/// @complexity Constant.
	const_reverse_iterator digit_rend() const { return d.begin() + std::max(1,nd); }

	/// Iterator that enumerates the digits from right (least significant)
	/// to left (most significant).
	typedef typename std::array<TDigit,NDigit>::const_reverse_iterator const_iterator;

	/// Returns an iterator that initially points to the most significant
	/// digit, and moves to less significant digits when advanced.
	/// @complexity Constant.
	const_iterator digit_begin() const 	{ return d.rbegin() + (NDigit - std::max(1,nd)); }

	/// Returns an iterator that points past the least significant digit.
	/// @complexity Constant.
	const_iterator digit_end() const { return d.rend(); }

};

/**
 * Returns the sequence of digits of a decimal number, read forwards.
 * @complexity Constant.
 */
template <int NDigit, int NExponent, unsigned char Base>
sequence<typename decimal<NDigit,NExponent,Base>::const_iterator> 
digits(const decimal<NDigit,NExponent,Base> &x)
{
	return make_sequence(x.digit_begin(), x.digit_end());
}

/**
 * Returns the sequence of digits of a decimal number, read backwards.
 * @complexity Constant.
 */
template <int NDigit, int NExponent, unsigned char Base>
sequence<typename decimal<NDigit,NExponent,Base>::const_reverse_iterator> 
rdigits(const decimal<NDigit,NExponent,Base> &x)
{
	return make_sequence(x.digit_rbegin(), x.digit_rend());
}

/**
 * Writes a decimal number to an output stream.
 * @param os The output stream.
 * @param x The decimal number to output.
 * @returns The output stream.
 * @complexity <code>O(d)</code> where @c d is the number of digits in @c x.
 * @ingroup Decimal
 */
template <int NDigit, int NExponent, unsigned char base>
std::ostream& operator << (std::ostream &os, const decimal<NDigit,NExponent,base> &x)
{
	for (auto it = x.digit_begin(); it != x.digit_end(); ++it)
	{
		int c = *it;
		os << (char)(c <= 9? '0' + c : c <= 35? c - 10 + 'a' : '_');
	}
	return os;
}

} // namespace euler

#endif // EULER_DECIMAL_H
