/**
 * @defgroup Cartesian Analytic Geometry
 * Routines for analytic geometry modeling and computation.
 *
 * @ingroup Library
 *
 * This module provides classes and functions that supports modeling and
 * computation in analytic geometry.
 */

#ifndef EULER_CARTESIAN_H
#define EULER_CARTESIAN_H

#include <cmath>

namespace euler
{

/**
 * Namespace for 2-dimentional analytic geometry analysis.
 */
namespace cartesian2d
{

/// Represents a point <code>(x, y)</code> in a 2D Cartesian coordinate plane.
/// @ingroup Cartesian
template <class T>
struct point
{
	/// x-coordinate of the point.
	T x;
	
	/// y-coordinate of the point.
	T y;

	/// Constructs a point of the given coordinates.
	/// @param _x x-coordinate of the point.
	/// @param _y y-coordinate of the point.
	point(T _x, T _y) : x(_x), y(_y) { }

	/// Constructs a point at origin.
	point() : x(0), y(0) { }
};

/**
 * Represents a line in a 2D Cartesian coordinate plane.
 *
 * A line is determined by passing through two distinct points 
 * <code>P<sub>1</sub>(x<sub>1</sub>,y<sub>1</sub>)</code> and 
 * <code>P<sub>2</sub>(x<sub>2</sub>,y<sub>2</sub>)</code>.
 * It can then be described by the following parametric equation
 * \f[
 *  \begin{align}
 *   x &= x_1 + (x_2 - x_1) t \\
 *   y &= y_1 + (y_2 - y_1) t 
 *  \end{align}
 * \f] 
 * where @c t is an arbitrary real number. 
 *
 * Although not required, in some applications it is useful to define a 
 * <i>direction</i> for the line. A natural choice is to define it as the 
 * direction that @c t increases. If we do so, it is also natural to call 
 * <code>P<sub>1</sub></code>
 * the starting point and <code>P<sub>2</sub></code> the ending point.
 * Note that this is just a naming convention; it doesn't require the line
 * to be constrained between these points.
 *
 * @ingroup Cartesian
 */
template <class T>
struct line
{
	/// Starting point of the line.
	point<T> p1;

	/// Ending point of the line.
	point<T> p2;

	/**
	 * Constructs a line that passes through a pair of given points.
	 * The line can be considered directional (though it doesn't have
	 * to be) by treating the first point as the starting point and 
	 * the second point as the ending point.
	 *
	 * @param start_point Starting point of the line.
	 * @param end_point Ending point of the line.
	 */
	line(const point<T> &start_point, const point<T> &end_point)
		: p1(start_point), p2(end_point) { }
};

/**
 * Represents a stardard ellipse in a 2D Cartesian coordinate plane.
 * The ellipse is determined by the equation
 * \f[ \frac{x^2}{a^2} + \frac{y^2}{b^2} = 1 \f] 
 * where @c a and @c b are its diameters along the x- and y-axis, respectively.
 * Both @c a and @c b must be greater than zero. If <code>a = b</code>,
 * then it degenerates into a circle, but is still allowed.
 *
 * @ingroup Cartesian
 */
template <class T>
struct ellipse
{
	/// Diameter of the ellipse along the x-axis. Must be greater than zero.
	T a;

	/// Diameter of the ellipse along the y-axis. Must be greater than zero.
	T b;

	/// Constructs an ellipse of the given diameters.
	/// @param _a Diameter along the x-axis. Must be greater than zero.
	/// @param _b Diameter along the y-axis. Must be greater than zero.
	ellipse(T _a, T _b) : a(_a), b(_b) { }

	/// Tests whether the ellipse is a circle, i.e. whether its diameters 
	/// along two axis are equal.
	/// @return @c true if the ellipse is a circle, @c false otherwise.
	bool is_circle() const { return a == b; }
};

/**
 * Mirrors a point (@c shape) against a point (@c mirror).
 *
 * Let the fixed point be \f$P_0(x_0,y_0)\f$ and the point to
 * be mirrored be \f$P(x, y)\f$. Then the mirrored point \f$P'(x',y')\f$ 
 * can be found by solving
 * \f[ x_0 = \frac{x+x'}{2}, y_0 = \frac{y+y'}{2} \f]
 * which yields
 * \f[ x' = 2x_0 - x, y' = 2 y_0 - y . \f]
 *
 * @param shape The point to be mirrored.
 * @param mirror The point that acts as a mirror.
 * @return The mirrored point.
 * @timecomplexity Constant.
 * @spacecomplexity Constant.
 *
 * @ingroup Cartesian
 */
template <class T>
point<T> mirror(const point<T> &shape, const point<T>& mirror)
{
	return point<T>(mirror.x*2-shape.x, mirror.y*2-shape.y);
}

/**
 * Mirrors a point (@c shape) against a line (@c mirror).
 *
 * Let the line connecting \f$P_1(x_1,y_1)\f$ and \f$P_2(x_2, y_2)\f$ be
 * \f$(x_1+(x_2-x_1) t, y_1+(y_2-y_1) t)\f$, and let the source point be 
 * \f$P_0(x_0, y_0)\f$. To find the mirror of \f$P_0\f$ against the
 * line, first find its projected point @c P(x,y) on the line, such that
 * \f$P_0 P\f$ is perpendicular to \f$P_1 P_2\f$.
 * This can be found by setting the vector inner product of 
 * \f$P_0 P\f$ and \f$P_1 P_2\f$ to zero, which yields
 * @f[ 
 *  [x_1+(x_2-x_1)t-x_0](x_2-x_1) + [y_1+(y_2-y_1)t-y_0](y_2-y_1) = 0 .
 * @f]
 * Solving this equation, we get
 * @f[
 *  t = \frac{(x_0-x_1)(x_2-x_1)+(y_0-y_1)(y_2-y_1)}{(x_2-x_1)^2+(y_2-y_1)^2} .
 * @f]
 * Once we find the point @c P, we can mirror <code>P<sub>0</sub></code>
 * against @c P to find its mirror against the line.
 *
 * @param shape The point to be mirrored.
 * @param mirror The line that acts as a mirror.
 * @return The mirrored point.
 * @timecomplexity Constant.
 * @spacecomplexity Constant.
 *
 * @ingroup Cartesian
 */
template <class T>
point<T> mirror(const point<T> &shape, const line<T>& mirror)
{
	// Find the mid point.
	T x0 = shape.x, y0 = shape.y;
	T x1 = mirror.p1.x, y1 = mirror.p1.y;
	T x2 = mirror.p2.x, y2 = mirror.p2.y;
	T t = ((x0-x1)*(x2-x1)+(y0-y1)*(y2-y1))/((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
	point<T> mid(x1+t*(x2-x1), y1+t*(y2-y1));
	return euler::cartesian2d::mirror(shape, mid);
}

/**
 * Returns the normal line at a given point on a standard ellipse.
 *
 * To find the normal line at point \f$P(x_0,y_0)\f$ on a standard ellipse
 * with diameters @c a and @c b, write the parametric form of the ellipse as
 * \f[ x = a \cos \theta, y = b \sin \theta , \f]
 * and note that 
 * \f[ dx = -a \sin \theta d\theta, dy = b \cos \theta d\theta . \f]
 * Thus the slope of the normal line is 
 * \f[ -\frac{dx}{dy} = \frac{a^2 y_0}{b^2 x_0} , \f]
 * and the line function is
 * \f[ x = x_0 + b^2 x_0 t, y = y_0 + a^2 y_0 t . \f]
 *
 * @param shape A standard ellipse.
 * @param pt A point on the ellipse.
 * @return The normal line at the given point on the ellipse.
 * @timecomplexity Constant.
 * @spacecomplexity Constant.
 *
 * @ingroup Cartesian
 */
template <class T>
line<T> normal_line(const ellipse<T> &shape, const point<T> &pt)
{
	T x0 = pt.x, y0 = pt.y;
	T a = shape.a, b = shape.b;
	return line<T>(pt, point<T>(x0+b*b*x0, y0+a*a*y0));
}

#if 0
template <class T>
bool solve_2nd_order_equation(T a, T b, T c, T &x1, T &x2)
{
	T delta = b*b-4*a*c;
	if (delta >= 0)
	{
		delta = std::sqrt(delta);
		x1 = (-b - delta) / (a+a);
		x2 = (-b + delta) / (a+a);
		return true;
	}
	return false;
}
#endif

/**
 * Computes the intersection of a line and a standard ellipse. 
 *
 * If there are two intersection points, both are stored and the function
 * returns @c true. If there is only one intersection point, (i.e. the 
 * line is tangent to the ellipse,) the tangent point is stored twice and 
 * the function returns @c true. If there is no intersection point, no
 * point is stored and the function returns @c false.
 * 
 * To find the intersection of a line with an ellipse, write the the line 
 * in its parametric form
 * \f[ x = x_0 + p t, y = y_0 + q t \f]
 * and the ellipse in its standard form
 * \f[ \frac{x^2}{a^2} + \frac{y^2}{b^2} = 1 . \f]
 * Substituting the line equation into the ellipse equation and rearranging 
 * terms, we get
 * \f[
 *  (b^2 p^2 + a^2 q^2) t^2 + 2(b^2 p x_0 + a^2 q y_0) t 
 *  + (b^2 x_0^2 + a^2 y_0^2 - a^2 b^2) = 0 .
 * \f]
 * This quadratic equation can be solved easily with standard techniques. 
 *
 * In some applications, it is useful to define the <i>direction</i> of the
 * line to be the direction where @c t increases. Thus, we can label the two
 * intersection points (where they exist) by calling the point corresponding
 * to the smaller solution of @c t the <i>near point</i>, and the point 
 * corresponding to the greater solution of @c t the <i>far point</i>.
 *
 * In the special case where the starting point of the line, 
 * <code>(x<sub>0</sub>,y<sub>0</sub>)</code> is on the ellipse, the constant
 * term in the above equation would vanish, making it a linear equation which
 * can be solved without effort.
 *
 * @param l A line.
 * @param e An ellipse.
 * @param near_point On return, stores the first intersection point if any;
 *      otherwise unchanged.
 * @param far_point On return, stores the second intersection point if any;
 *      otherwise unchanged.
 * @return @c true if the line and the ellipse intersects, @c false otherwise.
 * @timecomplexity Constant.
 * @spacecomplexity Constant.
 *
 * @ingroup Cartesian
 */
template <class T>
bool intersect(const line<T> &l, const ellipse<T> &e, point<T> &near_point, point<T> &far_point)
{
	T x0 = l.p1.x, y0 = l.p1.y;
	T p = l.p2.x - x0, q = l.p2.y - y0;
	T a = e.a, b = e.b;
	T t1, t2;

	auto solve_2nd_order_equation = [](T a, T b, T c, T &x1, T &x2) -> bool
	{
		T delta = b*b-4*a*c;
		if (delta >= 0)
		{
			delta = std::sqrt(delta);
			x1 = (-b - delta) / (a+a);
			x2 = (-b + delta) / (a+a);
			return true;
		}
		return false;
	};

	if (solve_2nd_order_equation(b*b*p*p+a*a*q*q, 2*(x0*b*b*p+y0*a*a*q),
		b*b*x0*x0+a*a*y0*y0-a*a*b*b, t1, t2))
	{
		near_point.x = x0 + p*t1;
		near_point.y = y0 + q*t1;
		far_point.x = x0 + p*t2;
		far_point.y = y0 + q*t2;
		return true;
	}
	return false;
}

} // namespace cartesian

} // namespace math

#endif
