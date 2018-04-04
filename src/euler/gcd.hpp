/**
 * @defgroup CommonDivisor Common Divisor
 * @ingroup Library
 */

#ifndef EULER_GCD_H
#define EULER_GCD_H

#include <cassert>
#include <type_traits>

namespace euler {

/**
 * Computes the greatest common divisor of two integers using the
 * Euclidean algorithm.
 * 
 * @param a The first integer. Must be non-negative.
 * @param b The second integer. Must be non-negative.
 * @returns <code>gcd(a,b)</code>. If one of @c a and @c b is zero, returns
 *      the other. If both are zero, returns @c 0.
 * @timecomplexity <code>O(log(min(a,b)))</code> divisions.
 * @spacecomplexity Constant.
 *
 * @ingroup CommonDivisor
 */
template <typename T>
T gcd(T a, T b)
{
	assert(a >= 0 && b >= 0);
	if (a == 0)
		return b;
	if (b == 0)
		return a;
	while (1)
	{
		T rem = a % b;
		if (rem == 0)
			return b;
		a = b;
		b = rem;
	}
}

/**
 * Computes the least common multiple of two integers using the
 * Euclidean algorithm.
 * 
 * @param a The first integer. Must be non-negative.
 * @param b The second integer. Must be non-negative.
 * @returns <code>lcm(a,b)</code>.
 * @timecomplexity <code>O(log(min(a,b)))</code> divisions.
 * @spacecomplexity Constant.
 *
 * @ingroup CommonDivisor
 */
template <typename T>
T lcm(T a, T b)
{
	T d = gcd(a, b);
	return a * (b / d);
}

/**
 * Solves the Diophantine equation <code>ax + by = gcd(a,b)</code> using 
 * the extended Euclidean algorithm.
 *
 * @param a First parameter of the equation.
 * @param b Second parameter of the equation.
 * @param x Outputs the solution of @c x.
 * @param y Outputs the solution of @c y.
 * @returns <code>gcd(a,b)</code>.
 *
 * @timecomplexity <code>O(log(min(a,b)))</code>.
 * @spacecomplexity Constant.
 *
 * @ingroup CommonDivisor
 */
template <typename T>
T egcd(T a, T b, 
	   typename std::make_signed<T>::type &x, 
	   typename std::make_signed<T>::type &y)
{
	assert(a >= 0 && b >= 0);

	// Special cases.
	if (a == 0 && b == 0)
	{
		x = y = 0;
		return 0;
	}
	if (a == 0)
	{
		x = 0;
		y = 1;
		return b;
	}
	if (b == 0)
	{
		x = 1;
		y = 0;
		return a;
	}

	// Extended Eucilean algorithm.
	typedef typename std::make_signed<T>::type TCalc;
	T r_2 = a, r_1 = b;
	TCalc x_2 = 1, x_1 = 0;
	while (1)
	{
		T q = r_2 / r_1;
		T r = r_2 % r_1;
		if (r == 0)
		{
			x = x_1;
			y = (r_1-a*x_1)/b;
			return r_1;
		}
		TCalc x_0 = x_2 - q*x_1;
		r_2 = r_1;
		r_1 = r;
		x_2 = x_1;
		x_1 = x_0;
	}
}

/**
 * Generates a family of quadruples that satisfy Bézout's identity 
 * <code>ax - by = 1</code> from a given quadruple.
 *
 * This function generates a family of quadruples <code>(a, b, x, y)</code> 
 * that satisfy Bézout's identity <code>ax - by = 1</code>. It starts from
 * the supplied solution \f$(a, b, x, y)\f$ and recursively applies the 
 * following relation
 * \f[
 * (x+kb)a  - (y+ka)b = 1
 * \f]
 * to generate a family of solutions \f$a' = x+kb, b' = y+ka, x' = a, y' = b\f$
 * for \f$k \ge 1\f$.
 *
 * For each quadruple generated, excluding the supplied one, this function
 * calls <code>f(a, b, x, y)</code> which must return a boolean value to 
 * indicate whether to continue generation on that path. 
 * If <code>f(a, b, x, y)</code> returns <code>false</code>,
 * any quadruple <code>f(a', b', x', y')</code> such that 
 * <code>a' ≥ a, b' ≥ b, x' ≥ x, y' ≥ y</code> will not be generated.
 *
 * Note that while solutions to Bézout's identity are integral by definition,
 * the data type passed to this function need not be integral because a 
 * recurrence relation is used to generate the solutions and the identity
 * itself is never checked explicitly.
 *
 * @param a First coefficient.
 * @param b Second coefficient.
 * @param x First Bézout number.
 * @param y Second Bézout number.
 * @param f Callback function.
 * @returns Number of quadruples generated, not including the supplied one.
 *      This is equal to the number of times <code>f(a, b, x, y)</code> 
 *      returns <code>true</code>.
 *      
 * @timecomplexity <code>O(N*log(N))</code>, where @c N is the largest 
 *      element generated.
 * @spacecomplexity <code>O(log(N))</code> on the stack, where @c N is the 
 *      largest element generated.
 *
 * @ingroup CommonDivisor
 */
template <class T, class Func>
size_t generate_bezout_quadruples(T a, T b, T x, T y, Func f)
{
	size_t count = 0;
	T aa=x+b, bb=y+a, xx=a, yy=b;
	while (f(aa, bb, xx, yy))
	{
		++count;
		count += generate_bezout_quadruples(aa,bb,xx,yy,f);
		aa += b;
		bb += a;
	}
	return count;
}

/**
 * Generates quadruples that satisfy Bézout's identity <code>ax - by = 1</code>.
 *
 * This function generates all quadruples <code>(a, b, x, y)</code> where
 * <code>a, b, x, y ≥ 1</code> that satisfy Bézout's identity 
 * <code>ax - by = 1</code>. It also guarantees that <code>|x| ≤ |b|</code>
 * and <code>|y| ≤ |a|</code>, where the equal sign holds only when 
 * <code>b = 1</code> or <code>a = 1</code>.
 *
 * @param f Callback function.
 * @returns Number of quadruples generated. This is equal to the number
 *      of times <code>f(a, b, x, y)</code> returns <code>true</code>.
 *
 * @timecomplexity <code>O(N*log(N))</code>, where @c N is the largest 
 *      element generated.
 * @spacecomplexity <code>O(log(N))</code> on the stack, where @c N is the 
 *      largest element generated.
 *
 * @ingroup CommonDivisor
 */
template <class T, class Func>
size_t generate_bezout_quadruples(Func f)
{
	size_t count = 0;
	for (T n = 1; ; n++)
	{
		size_t c = generate_bezout_quadruples(1,n,1,0,f);
		if (c == 0)
			break;
		count += c;
	}
	return count;
}

} // namespace euler

#endif // EULER_GCD_HPP
