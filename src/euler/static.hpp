/**
 * @defgroup Static Compile-Time Arithmetics
 * @ingroup Library
 */

#ifndef EULER_STATIC_H
#define EULER_STATIC_H

namespace euler {

/// Compile-time Fibonacci number.
/// @ingroup Static
template <unsigned int n>
struct static_fibonacci
{
	/// Value of the <code>n</code>-th Fibonacci number.
	static const unsigned long long value = 
		static_fibonacci<n-1>::value + static_fibonacci<n-2>::value;
};

/// @cond IMPLEMENTATION
template <> 
struct static_fibonacci<0> { static const unsigned long long value = 0; };
template <> 
struct static_fibonacci<1> { static const unsigned long long value = 1; };
/// @endcond

/// Compile-time power.
/// @ingroup Static
template <unsigned int base, unsigned int exponent> 
struct static_pow
{
	/// Value of <code>base</code> raised to the power of <code>exponent</code>.
	static const unsigned long long value = base * static_pow<base, exponent-1>::value;
};

/// @cond IMPLEMENTATION
template <unsigned int base> 
struct static_pow<base, 0> { static const unsigned long long value = 1; };
/// @endcond

/// Compile-time factorial.
/// @ingroup Static
template <unsigned int n>
struct static_factorial
{
	/// Value of the factorial of <code>n</code>.
	static const unsigned long long value = 
		static_factorial<n-1>::value * n;
};

/// @cond IMPLEMENTATION
template <> 
struct static_factorial<0> { static const unsigned long long value = 1; };
/// @endcond

/// Compile-time logarithm.
/// @ingroup Static
template <unsigned long long n, unsigned long long base>
struct static_log
{
	static const int value = (n < base)? 0 : static_log<n/base,base>::value + 1;
};

/// @cond IMPLEMENTATION
template <unsigned long long base>
struct static_log<0,base>
{
	static const int value = 0;
};
/// @endcond

} // namespace euler

#endif // EULER_STATIC_H
