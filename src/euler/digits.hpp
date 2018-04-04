/**
 * @defgroup Digits Decimal Expansion
 * Routines to access the digits of an integer expanded in a certain base.
 *
 * @ingroup Library
 *
 * This module provides routines to access the digits of an integer expanded
 * in a certain base. For example, it allows you to iterate the digits of
 * a decimal number both from left to right and from right to left.
 */

#ifndef EULER_DIGITS_H
#define EULER_DIGITS_H

#include <iterator>
#include <bitset>
#include <functional>
#include <algorithm>
#include <limits>
#include <array>
#include "sequence.hpp"

namespace euler {

/// Iterator that returns the digits of a number from left (most significant
/// digit) to right (least significant digit). Leading zeros are ignored.
template <int base, typename T>
class digit_iterator 
	: public std::iterator<std::forward_iterator_tag, int, std::ptrdiff_t, void, int>
{
	T _n;
	T _b;

public:

	/**
	 * Constructs the iterator. The iterator initially points to the most
	 * significant digit of the number and moves to less significant digits
	 * when advanced.
	 *
	 * @param n The number whose digits are iterated. Must be positive.
	 * @timecomplexity <code>log<sub>b</sub>n</code> operations.
	 * @spacecomplexity Constant.
	 */
	digit_iterator(T n) : _n(n)
	{
		for (_b = 1; n / _b >= base; _b *= base);
	}

	/// Constructs an empty iterator that points <i>past-the-end</i>.
	/// @complexity Constant.
	digit_iterator() : _n(0), _b(0) { }

	/// Returns the current digit.
	/// @complexity Constant.
	int operator * () const { return (int)(_n / _b); }

	/// Advances the iterator to point to the next (less significant) digit.
	/// @returns The advanced iterator.
	/// @complexity Constant.
	digit_iterator& operator ++ () { _n %= _b; _b /= base; return *this; }

	/// Tests whether this iterator is equal to another iterator.
	bool operator == (const digit_iterator &it) const { 	return _n == it._n && _b == it._b; }

	/// Tests whether this iterator is not equal to another iterator.
	bool operator != (const digit_iterator &it) const {	return _n != it._n || _b != it._b; }
};

/// Iterator that returns the digits of a number from right (least significant
/// digit) to left (most significant digit). Leading zeros are ignored.
template <int base, typename T>
class digit_reverse_iterator 
	: public std::iterator<std::forward_iterator_tag, int, std::ptrdiff_t, void, int>
{
	T _n;
	int _b;

public:

	/**
	 * Constructs the iterator. The iterator initially points to the least
	 * significant digit of the number and moves to more significant digits
	 * when advanced.
	 *
	 * @param n The number whose digits are iterated. Must be positive.
	 * @complexity Constant.
	 */
	digit_reverse_iterator(T n) : _n(n), _b(base) 	{ }

	/// Constructs an empty iterator that points <i>past-the-end</i>.
	/// @complexity Constant.
	digit_reverse_iterator() : _n(0), _b(0) { }

	/// Returns the current digit.
	/// @complexity Constant.
	int operator * () const 	{ return _n % base; 	}

	/// Advances the iterator to point to the next (more significant) digit.
	/// @returns The advanced iterator.
	/// @complexity Constant.
	digit_reverse_iterator& operator ++ ()
	{
		if ((_n /= base) == 0)
			_b = 0;
		return *this;
	}

	/// Tests whether this iterator is equal to another iterator.
	bool operator == (const digit_reverse_iterator &it) const
	{
		return (_n == it._n) && (_b == it._b);
	}

	/// Tests whether this iterator is not equal to another iterator.
	bool operator != (const digit_reverse_iterator &it) const
	{
		return ! operator == (it);
	}
};

/**
 * Returns the sequence of digits of an integer, enumerated from left (most
 * significant digit) to right (least significant digit).
 *
 * @param n An integer whose digits are iterated.
 * @returns The digit sequence of @c n in base @c base.
 * @complexity Constant.
 *
 * @ingroup Digits
 */
template <int base, typename T>
sequence<digit_iterator<base, T>> digits(T n)
{
	return make_sequence(
		digit_iterator<base, T>(n), 
		digit_iterator<base, T>());
}

/**
 * Returns the (reverse) sequence of digits of an integer, enumerate from 
 * right (least significant digit) to left (most significant digit).
 *
 * @param n An integer whose digits are expanded.
 * @returns The reverse digit sequence of @c n in base @c base.
 * @complexity Constant.
 *
 * @ingroup Digits
 */
template <int base, typename T>
sequence<digit_reverse_iterator<base, T>> rdigits(T n)
{
	return make_sequence(
		digit_reverse_iterator<base, T>(n), 
		digit_reverse_iterator<base, T>());
}

/**
 * Converts a sequence of digits in a given base into an integer.
 * The sequence is given from left (most significant digit) to right (least
 * significant digit).
 *
 * @param begin Begin iterator of the digit sequence.
 * @param end End iterator of the digit sequence.
 * @returns An integer converted from the digit sequence.
 * @timecomplexity <code>(end - begin)</code> operations.
 * @spacecomplexity Constant.
 * @remarks The caller must ensure that @c T has enough bits to hold the 
 *      result in order to avoid overflow.
 *
 * @ingroup Digits
 */
template <int base, typename T, class InIt>
T from_digits(InIt begin, InIt end)
{
	T n = 0;
	for (; begin < end; ++begin)
	{
		n = n * base + (*begin);
	}
	return n;
}

/** 
 * Sorts the digits of an integer.
 *
 * @param n An integer whose digits are sorted when the function returns.
 *      Must be greater than or equal to zero.
 * @param acsending If set to @c true, the digits are sorted in acsending 
 *      order. If set to @c false, the digits are sorted in descending order.
 * @returns Number of digits in <code>n</code>.
 * @timecomplexity <code>O(D*log(D))</code>, where @c D is the number of 
 *      digits in <code>n</code>.
 * @spacecomplexity <code>O(D)</code>, where @c D is the number of digits
 *      in <code>n</code>.
 *
 * @ingroup Digits
 */
template <int base, typename T>
int sort_digits(T &n, bool acsending = true)
{
	const int Bits = std::numeric_limits<T>::digits + 1;
	std::array<int,Bits> digits;
	auto d = euler::rdigits<base>(n);
	auto p1 = digits.begin();
	auto p2 = std::copy(d.begin(), d.end(), p1);

	if (acsending)
		std::sort(p1, p2);
	else
		std::sort(p1, p2, std::greater<int>());

	n = euler::from_digits<base,T>(p1,p2);
	return (int)(p2 - p1);
}

/**
 * Checks whether an integer is palindromic in a given base.
 * A number is palindromic (in some base) if it is the same when written
 * forwards or backwards.
 *
 * @param n An integer to test.
 * @returns @c true if @c n is palindromic, @c false otherwise.
 * @timecomplexity <code>O(log<sub>b</sub>n)</code> operations.
 * @spacecomplexity Constant.
 *
 * @ingroup Digits
 * @todo Preferrably we should find a way to use <code>std::equal</code>
 *      and avoid generating @c C4996 warning from VC++.
 */
template <int base, typename T>
bool is_palindromic(T n)
{
	auto r1 = digits<base>(n);
	auto r2 = rdigits<base>(n);

	// Avoid using std::equal() to avoid VC++ warning C4996 on unchecked iterators.
	auto it2 = r2.begin();
	for (auto it1 = r1.begin(); it1 != r1.end(); ++it1, ++it2)
	{
		if (*it1 != *it2)
			return false;
	}
	return true;
}

/**
 * Checks whether a sequence of digits contain each of the digits @c lowest
 * to @c highest exactly once.
 *
 * @param begin Input iterator that points to the beginning of the sequence.
 * @param end Input iterator that points past the end of the sequence.
 * @param lowest Smallest digit required to be present.
 * @param highest Largest digit required to be present.
 * @returns @c true if the sequence is pandigital, @c false otherwise.
 * @timecomplexity <code>O(b)</code> operations.
 * @spacecomplexity <code>O(b)</code> (on the stack). The function uses
 *      a <code>std::bitset</code> of @c b bits to record the presence
 *      of each digit of base @c base.
 *
 * @ingroup Digits
 */
template <int base, class InIt>
bool is_pandigital(InIt begin, InIt end, int lowest = 1, int highest = base - 1)
{
	std::bitset<base> mask;
	int count = 0;

	for (InIt it = begin; it != end; ++it)
	{
		int k = *it;
		if (!(k >= lowest && k <= highest))
			return false;
		if (mask.test(k))
			return false;
		++count;
		mask.set(k);
	}
	return count == (highest - lowest + 1);
}

} // namespace euler

#endif
