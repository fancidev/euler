/**
 * @defgroup ContFrac Continued Fraction
 * Routines to compute the continued fraction representation of certain
 * classes of numbers.
 *
 * @ingroup Library
 *
 * This module contains routines to compute the continued fraction 
 * refresentation of certain classes of numbers. At present, it only 
 * implements the square root of a square free integer.
 */

#ifndef EULER_CONTINUED_FRACTION_H
#define EULER_CONTINUED_FRACTION_H

#include <cassert>
#include <iterator>
#include "imath.hpp"

namespace euler {

/**
 * Iterator used to enumerate the continued fraction representation of the
 * square root of a squarefree integer.
 *
 * The square root of a squarefree integer, @c D has a periodic continued
 * fraction of the form 
 * \f[ \left[ a_0; \overline{ a_1, \ldots, a_r, 2a_0 } \right] \f]
 * where \f$a_0 = \lfloor \sqrt{D} \rfloor\f$. If @c D is a perfect square,
 * then its continued fraction representation doesn't exist.
 *
 * The following recurrence relation is used to compute the terms of the
 * continued fraction expansion of a squarefree integer:
 * \f[
 * \begin{align}
 *  P_0 &= 0 \\
 *  Q_0 &= 1 \\
 *  P_n &= a_{n-1} Q_{n-1} - P_{n-1} \\
 *  Q_n &= (D - P_n^2) / Q_{n-1} \\
 *  a_n &= \lfloor (a_0 + P_n) / Q_n \rfloor .
 * \end{align}
 * \f]
 *
 * In addition, two facts are useful about the continued fraction expansion
 * of a squarefree integer. First, all the terms in the expansion satisfy 
 * \f$0 < a_n < 2 \sqrt{D}\f$. This means it is safe to return the terms
 * in the same data type as the type of @c D. Second, the length of the 
 * period of the continued fraction is bounded by <code>2D</code>, and 
 * is asymptotically bounded by \f$\mathcal{O}(\sqrt{D} \ln{D})\f$. 
 *
 * More information can be found in the pages below.
 *   - http://mathworld.wolfram.com/ContinuedFraction.html
 *   - http://mathworld.wolfram.com/PellEquation.html
 *   - http://en.wikipedia.org/wiki/Periodic_continued_fraction
 * 
 * @todo Consider if it's more appropriate to start iteration from 
 *      <code>a<sub>1</sub></code> rather than <code>a<sub>0</sub></code>
 *      to get rid of numerous issues regarding whether @c D is squarefree
 *      and how to compare the end iterator and how to compute the period
 *      length.
 * @ingroup ContFrac
 */
template <class T>
class continued_fraction_sqrt_iterator 
	: public std::iterator<std::input_iterator_tag, T>
{
	T D, a0, a, P, Q;

public:

	/**
	 * Constructs an iterator to enumerate the terms in the continued fraction
	 * representation of a squarefree integer.
	 *
	 * @param n A squarefree integer. If @c n is not squarefree, the result is
	 *      undefined.
	 * @timecomplexity Constant.
	 * @spacecomplexity Constant.
	 */
	continued_fraction_sqrt_iterator(T n) 
		: D(n), a0(isqrt(D)), a(a0), P(0), Q(1) 
	{
		// Skip the a0 term.
		if (!(a0*a0 == D))
			++(*this);
	}

	/// Returns the current term in the continued fraction expansion.
	/// @timecomplexity Constant.
	/// @spacecomplexity Constant.
	T operator * () const { 	return a; }

	/// Advances the iterator to point to the next term in the continued 
	/// fraction expansion.
	/// @return The advanced iterator.
	/// @timecomplexity Constant.
	/// @spacecomplexity Constant.
	continued_fraction_sqrt_iterator<T>& operator ++ ()
	{
		P = a*Q - P;
		Q = (D - P*P) / Q;
		a = (a0 + P) / Q;
		return *this;
	}

	/// Tests whether this iterator is equal to another iterator.
	/// @return @c true if the two iterators are equal, @c false otherwise.
	/// @timecomplexity Constant.
	/// @spacecomplexity Constant.
	bool operator == (const continued_fraction_sqrt_iterator<T> &it) const
	{
		return (D == it.D) && (a == it.a) && (P == it.P) && (Q == it.Q);
	}

	/// Tests whether this iterator is not equal to another iterator.
	/// @return @c true if the two iterators are not equal, @c false otherwise.
	/// @timecomplexity Constant.
	/// @spacecomplexity Constant.
	bool operator != (const continued_fraction_sqrt_iterator<T> &it) const
	{
		return ! operator == (it);
	}

	/**
	 * Checks whether the fractional part of the continued fraction expansion
	 * is empty. This is the case if and only if @c n is a perfect square.
	 * @returns @c true if the fractional part of the continued fraction 
	 *      is empty; @c false otherwise.
	 * @timecomplexity Constant.
	 * @spacecomplexity Constant.
	 */
	bool empty() const { return a0*a0 == D; }

	/// Returns the integer part <code>a<sub>0</sub></code> of the continued
	/// fraction expansion.
	/// @timecomplexity Constant.
	/// @spacecomplexity Constant.
	T integer_part() const { return a0; }

	/// Returns the period length of the continued fraction.
	/// @returns The period length of the fractional part of the continued 
	///      fraction. If the continued fraction has no fractional part
	///      (i.e. if @c D is a perfect square), returns zero.
	/// @timecomplexity <code>O(D)</code>.
	/// @spacecomplexity Constant.
	T period() const 
	{
		if (empty())
			return 0;

		continued_fraction_sqrt_iterator it(D);
		T period = 1;
		while (*it != 2*a0)
		{
			++period;
			++it;
		}
		return period;
	}
};

/**
 * Returns an iterator to enumerate the terms in the continued fraction
 * representation of a squarefree integer. This is a helper function;
 * it is equivalent to construct the iterator directly.
 *
 * @param n A squarefree integer.
 * @return An iterator that enumerates the terms in the continued fraction
 *      expansion of <code>√n</code>.
 * @timecomplexity Constant.
 * @spacecomplexity Constant.
 *
 * @ingroup ContFrac
 */
template <class T>
continued_fraction_sqrt_iterator<T> continued_fraction_sqrt(T n)
{
	return continued_fraction_sqrt_iterator<T>(n);
}

#if 0
template <class T>
continued_fraction_sqrt_iterator<T> continued_fraction_sqrt(T n)
{
	int a0 = (int)sqrt((double)n);
	frac.resize(1);
	frac[0] = a0;
	if (a0*a0 == n)
		return;

	int P = 0, Q = 1;
	int a = a0;
	do
	{
		P = a*Q - P;
		Q = (n - P*P)/Q;
		a = (a0 + P) / Q;
		frac.push_back(a);
	}
	while (a != 2*a0);
}
#endif

} // namespace euler

#if 0
static void test_continued_fraction()
{
	for (int n = 1; n <= 100; n++)
	{
		auto it = euler::continued_fraction_sqrt(n);
		int a0 = *it;
		if (a0 * a0 != n)
		{
			std::cout << n << " = [" << a0 << ";";
			int a;
			do
			{
				a = *(++it);
				std::cout << " " << a;
			} while (a != 2*a0);
			std::cout << "]" << std::endl;
		}
	}
}
#endif

#endif // EULER_CONTINUED_FRACTION_H
