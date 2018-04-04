/**
 * @defgroup ModularArithmetic Modular Arithmetic
 * @ingroup Library
 */

#ifndef EULER_MODULAR_H
#define EULER_MODULAR_H

#include <cassert>
#include <type_traits>
#include "imath.hpp"
#include "int_traits.hpp"

namespace euler {

	/*
void normalize(std::true_type)
{
	if (x < 0)
		x += modulus();
}

void normalize(std::false_type) { }
*/

/**
 * Returns the residue of @c n mod @c m.
 *
 * The value returned is guaranteed to be greater than or equal to zero.
 *
 * @param n An integer whose residue is computed.
 * @param m The modulus. Must be a positive integer.
 * @return The residue of @c n mod @c m.
 * @complexity Constant.
 *
 * @ingroup ModularArithmetic
 */
template <class TVal, class T>
T mod(TVal n, T m)
{
	T r = (T)(n % m);
#if 1
	if (r < 0)
		r += m;
#else
	normalize(std::is_signed<T>());
#endif
	return r;
}

template <typename T>
T modadd(T a, T b, T m)
{
	assert(a >= 0 && a < m);
	assert(b >= 0 && b < m);
	return (a >= m - b)? a - (m - b) : a + b;
}

template <typename T>
T modsub(T a, T b, T m)
{
	assert(a >= 0 && a < m);
	assert(b >= 0 && b < m);
	return (a >= b)? a - b : a + (m - b);
}

template <typename T>
T modmul(T a, T b, T m, std::false_type manually)
{
	typedef typename euler::int_traits<T>::wide_type calc_type;
	return ((calc_type)a * (calc_type)b) % m;
}

/// @todo Add optimization if the product of ab will not overflow.
template <typename T>
T modmul(T a, T b, T m, std::true_type manually)
{
	if (a < b)
	{
		std::swap(a, b);
	}
	return binexp(a, b, T(0), [m](T x, T y) -> T { return modadd(x, y, m); });
}

/**
 * Modular multiplication with another residue class of the same modulus.
 *
 * Special care is taken to avoid integer overflow in the multiplication.
 * If an integral type of twice the bits in the modulus is available,
 * a simple multiplication followed by division is performed. Otherwise,
 * the following procedure is performed.
 *
 * Let @c a be the larger operand to multiply and @c b be the smaller
 * operand. Write @c b in binary expansion
 * \f[ b = b_0 + b_1 \times 2 + \cdots + b_{n-1} \times 2^{n-1} \f]
 * where <code>b<sub>i</sub></code> is either @c 0 or @c 1.
 * Next, expand the modular multiplication as
 * \f[ a \times b \equiv a \times b_0 + (a \times 2) \times b_1  + \cdots
 *     + (a \times 2^{n-1}) \times b_{n-1} .  \f]
 * Following this equation, we can double @c a in each iteration, and
 * add to the result if the corresponding bit in @c b is @c 1. In this
 * process, only modular addition is used, therefore integer overflow is
 * avoided.
 *
 * @timecomplexity
 *     - In the simple case, one integer multiplication and one integer
 *       division.
 *     - In the complex case, no more than <code>2*log<sub>2</sub>b</code>
 *       modular additions where @c b is the smaller operand.
 *
 * @spacecomplexity Constant.
 */
template <typename T>
T modmul(T a, T b, T m)
{
	return modmul(a, b, m, 
		typename std::is_void<typename euler::int_traits<T>::wide_type>());
}

/**
 * Computes the modular multiplicative inverse of \c a modulo \c m.
 *
 * @param a The number whose modular multiplicative inverse is computed.
 * @param m The modulus.
 * @returns \c x such that <code>ax ≡ 1 (mod m)</code>.
 *
 * @timecomplexity <code>O(log m)</code>.
 * @spacecomplexity Constant.
 *
 * @ingroup ModularArithmetic
 */
template <typename T>
T modinv(T a, T m)
{
	T r_2 = a, r_1 = m;
	T x_2 = 1, x_1 = 0;
	while (1)
	{
		T q = r_2 / r_1;
		T r = r_2 % r_1;
		T x = x_2 - q*x_1;
		if (r == 1)
		{
			if (x < 0)
				x += m;
			return x;
		}
		r_2 = r_1;
		r_1 = r;
		x_2 = x_1;
		x_1 = x;
	}
	return 1;
}

template <typename T>
T moddiv(T a, T b, T m)
{
	return modmul(a, modinv(b, m), m);
}


/**
 * Returns the residue of <code>base<sup>exponent</sup></code> mod @c modulus.
 *
 * This function uses the <i>binary exponentiation</i> algorithm which squares
 * the base in each iteration and multiplies it to the result if the
 * corresponding bit in the exponent is set. A detailed explanation of the
 * algorithm can be found
 * <a href="http://en.wikipedia.org/wiki/Modular_exponentiation">here</a>.
 *
 * @param base The base. Must be greater than or equal to zero.
 * @param exponent The exponent. Must be greater than or equal to zero.
 * @param modulus The modulus. Must be positive.
 * @returns The residue of <code>base<sup>exponent</sup></code> mod @c modulus.
 * @timecomplexity No more than <code>2*log<sub>2</sub>E</code> modular
 *      multiplications, where @c E is the exponent.
 * @spacecomplexity Constant.
 * @remarks This function relies on the modular multiplication operation,
 *      which must be defined for <code>residue<T></code>.
 *
 * @ingroup ModularArithmetic
 * @todo Consider using Montgomery reduction to improve performance.
 */
template <class T, class TExponent>
T modpow(T base, TExponent exponent, T modulus)
{
	return binexp(base, exponent, T(1), 
		[modulus](T x, T y) -> T { return modmul(x, y, modulus); });
}

} // namespace euler

#if 0

// Below are implementations of modular multiplication and modular
// exponentiation in assembly.
//
// It's about 75% faster than a plain C implementation, because
// it takes advantage of the fact that the MUL and DIV instruction
// supports result/operand of double the width of the operand/result.
//
// However, since assembly code is not portable (with regarding to
// CPU, compiler, and the data type), we choose the plain C
// implementation in favor.

unsigned int modmult(unsigned int a, unsigned int b, unsigned int m)
{
	// a, b must be < m.
	unsigned int result;
	__asm
	{
		mov eax, a;
		mul b;
		div m;
		mov result, edx;
	}
	return result;
}

unsigned int modpow3(unsigned int a, unsigned int exponent, unsigned int m)
{
	unsigned int power = a % m;
	unsigned int result;
	__asm
	{
		mov ebx, 1;			// ebx = remainder
		mov ecx, exponent;	// ecx = exponent
		mov esi, m;			// esi = modulus
		mov edi, power;		// edi = power

begin_of_loop:
		shr ecx, 1;			// exponent >>= 1
		jnc update_power;	// if the last bit of exponent is zero, do nothing

		mov eax, edi;		// remainder = remainder * power mod m
		mul ebx;
		div esi;
		mov ebx, edx;

update_power:
		jecxz end_of_loop;
		mov eax, edi;		// power = power^2 mod m
		mul eax;
		div esi;
		mov edi, edx;
		jmp begin_of_loop;

end_of_loop:
		mov result, ebx;
	}
	return result;
}
#endif

#endif // EULER_MODULAR_H
