/// @defgroup Library Library

/**
 * @defgroup Pell Pell Equation
 * Routines to solve Pell equations.
 *
 * A standard Pell equation is a Diophatine equation of the form
 * \f[
 * x^2 - D y^2 = 1
 * \f]
 * where @c D is a positive integer. Obviously the equation has the
 * trivial solution <code>x = 1, y = 0</code>. Therefore we are interested
 * in finding non-trivial solutions.
 *
 * A (non-trivial) solution to the standard Pell equation exists if
 * and only if @c D is not a perfect square. In this case, the solution
 * can be found from the convergent sequence of the continued fraction
 * for <code>√D</code>. @c D is called the <i>determinant</i>.
 *
 * Once a solution \f$(x_0, y_0)\f$ to the standard Pell equation is
 * found, a family of solutions can be generated from it. Let
 * \f$(x_1, y_1)\f$ be a solution to the slightly generalized Pell equation
 * \f[
 * x^2 - D y^2 = \pm c
 * \f]
 * where @c c is a positive integer. Putting together this equation with
 * the standard equation and factorizing the terms, we get
 * \f[
 * \begin{align}
 * (x_1+\sqrt{D} y_1)(x_1-\sqrt{D} y_1) &= \pm c \\
 * (x_0+\sqrt{D} y_0)(x_0-\sqrt{D} y_0) &= 1 .
 * \end{align}
 * \f]
 * Multiplying vertically matching terms in the above equations, we get
 * \f[
 * [(x_0 x_1+Dy_0 y_1) + \sqrt{D}(x_0 y_1 + x_1 y_0)]
 * [(x_0 x_1+Dy_0 y_1) - \sqrt{D}(x_0 y_1 + x_1 y_0)] = \pm c .
 * \f]
 * Hence we find a new solution to the generalized equation:
 * \f[
 * \begin{align}
 * x_2 &= x_0 x_1+Dy_0 y_1 \\
 * y_2 &= x_0 y_1 + x_1 y_0 .
 * \end{align}
 * \f]
 *
 * If we repeat this step, a family of solutions can be found whose
 * magnitude grows exponentially with @c D. Note, however, that this
 * recurrence relation may not generate <i>all</i> solutions to the
 * generalized Pell equation. A solution that is not generated from
 * any other solution and from which a family of solutions can be
 * generated is called a <i>fundamental solution</i>. There is no
 * easy way to find fundamental solutions for the generalized Pell
 * equation except when <code>c = 1</code>.
 *
 * More information on Pell equations can be found from
 *   - http://mathworld.wolfram.com/PellEquation.html
 *
 * @ingroup Library
 */

#ifndef EULER_PELL_H
#define EULER_PELL_H

#include <utility>
#include <iterator>
#include "continued_fraction.hpp"

namespace euler {

/**
 * Solves the standard Pell equation <code>x<sup>2</sup> - Dy<sup>2</sup> = 1</code>.
 *
 * A solution exists if and only if the determinant, @c D is not a perfect
 * square. In this case, the solution is found from the convergent sequence
 * to the continued fraction for <code>√D</code>.
 *
 * @param D Determinant of the equation. Must be a positive integer.
 * @param xy Stores the solution to the equation if one exists; otherwise
 *      not used.
 * @return <code>true</code> if the equation is solvable, <code>false</code>
 *      otherwise.
 * @timecomplexity No more than <code>2L</code> operations where \c L is the
 *      length of the period of the continued fraction for <code>√D</code>.
 *      Note that \c L is bounded by <code>2D</code>. See, for example,
 *      http://en.wikipedia.org/wiki/Periodic_continued_fraction#Length_of_the_repeating_block.
 * @spacecomplexity Constant.
 * @remarks Note that while any solution to the equation is integral by
 *      definition, the data type of the solution @c xy need not be integral.
 *
 * @ingroup Pell
 */
template <typename TD, typename T>
bool solve_pell_equation(TD D, std::pair<T,T> &xy)
{
	// Sqrt(D) = [a0; a1, ..., a_r, 2a0, ...]
	auto it = continued_fraction_sqrt(D);
	if (it.empty())
		return false;

	TD a0 = it.integer_part();
	int r = 0; // period length - 1

	T h_2 = 0, h_1 = 1, h = 0;
	T k_2 = 1, k_1 = 0, k = 0;
	for (T a = a0; (a != 2*a0) || (r % 2 == 1); ++it, ++r)
	{
		h = a * h_1 + h_2;
		k = a * k_1 + k_2;
		h_2 = h_1;
		h_1 = h;
		k_2 = k_1;
		k_1 = k;
		a = *it;
	}

	xy = std::pair<T,T>(h,k);
	return true;
}

/**
 * Generates a family of solutions of a Pell equation from its fundamental
 * solution.
 *
 * This iterator generates the family of solutions to the generalized Pell
 * equation <code>x<sup>2</sup> - Dy<sup>2</sup> = c</code> from a known
 * solution and the basic solution to the corresponding standard equation
 * <code>x<sup>2</sup> - Dy<sup>2</sup> = 1</code>. Note that iterator does
 * <i>not</i> solve the equation; it merely generates more solutions from
 * known ones. Therefore, both solutions must be available in the first
 * place, but the constant @c c in the equation is not used.
 *
 * Note also that while any solution to a Pell equation is integral by
 * definition, the data type of the solution supplied and returned need not
 * be integral because the iterator uses an recurrence formula in the
 * calculation which permits numerical error.
 *
 * @ingroup Pell
 */
template <typename T, typename TD>
class pell_solution_iterator
{
	TD _D;
	std::pair<T,T> _basic;
	std::pair<T,T> _xy;

public:

	/// Iterator category.
	typedef std::forward_iterator_tag iterator_category;

	/// Value type.
	typedef std::pair<T,T> value_type;

	/// Difference type.
	typedef std::ptrdiff_t difference_type;

	/// Pointer type.
	typedef const std::pair<T,T> *pointer;

	/// Reference type.
	typedef const std::pair<T,T> &reference;

	/**
	 * Constructs a family of solutions from a basic solution and a
	 * fundamental solution.
	 * @param D     Determinant of the equation.
	 * @param basic Basic solution to the standard equation.
	 * @param xy    Known solution to the generalized equation.
	 * @complexity Constant.
	 */
	pell_solution_iterator(TD D, const std::pair<T,T> &basic, const std::pair<T,T> &xy)
		: _D(D), _basic(basic), _xy(xy) { }

	/**
	 * Constructs a family of solutions from a basic solution and a
	 * fundamental solution.
	 * @param D Determinant of the equation.
	 * @param x0 Basic solution to the standard equation.
	 * @param y0 Basic solution to the standard equation.
	 * @param x1 Known solution to the generalized equation.
	 * @param y1 Known solution to the generalized equation.
	 * @complexity Constant.
	 */
	pell_solution_iterator(TD D, T x0, T y0, T x1, T y1)
		: _D(D), _basic(x0,y0), _xy(x1,y1) { }

	/// Returns the current solution.
	/// @complexity Constant.
	reference operator * () const { return _xy; }

	/// Returns a pointer to the current solution.
	/// @complexity Constant.
	pointer operator -> () const { return &_xy; }

	/// Advances to the next solution.
	/// @complexity Constant.
	pell_solution_iterator& operator ++ ()
	{
		_xy = std::pair<T,T>(
			_basic.first*_xy.first + _D*_basic.second*_xy.second,
			_basic.first*_xy.second + _basic.second*_xy.first);
		return *this;
	}
};

} // namespace euler

#if 0
static void test_pell()
{
	// test pell equation
	for (int n = 1; n <= 100; n++)
	{
		std::pair<int,int> solution;
		if (euler::solve_pell_equation(n, solution))
		{
			std::cout << "Solution for " << n << ": " << solution.first << ", " << solution.second << std::endl;
		}
	}
}
#endif

#endif
