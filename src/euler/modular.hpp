/**
 * @defgroup ModularArithmetic Modular Arithmetic
 * @ingroup Library
 */

#ifndef EULER_MODULAR_H
#define EULER_MODULAR_H

#include <cassert>
#include <type_traits>
#include "gcd.hpp"
#include "imath.hpp"
#include "int_traits.hpp"

namespace euler {

#define EULER_MODULAR_CHECK_TYPE(T) \
  static_assert(std::is_integral<T>::value, #T " must be an integral type")

#define EULER_MODULAR_CHECK_MODULUS(m) \
  do { \
    if (!((m) > 0)) { \
      throw std::invalid_argument("Modulus " #m " must be positive"); \
    } \
  } while (false)

#define EULER_MODULAR_CHECK_RESIDUE(a, m) \
  do { \
    if (!((a) >= 0 && (a) < (m))) { \
      throw std::invalid_argument("Residue " #a " must be between [0, M)"); \
    } \
  } while (false)

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
  T r = static_cast<T>(n % m);
#if 1
  if (r < 0)
  {
    r += m;
  }
#else
  normalize(std::is_signed<T>());
#endif
  return r;
}

/**
 * Proper modular addition.
 *
 * @tparam T An integral type.
 *
 * @param a An integer within <code>[0, m)</code>.
 *
 * @param b An integer within <code>[0, m)</code>.
 *
 * @param m Modulus; must be a positive integer.
 *
 * @returns The unique integer @c c within <code>[0, m)</code> such that
 *    <code>a + b ≡ c (mod m)</code>.
 *
 * @exception std::invalid_argument if any argument is invalid.
 *
 * @remarks This function is guaranteed not to incur integer overflow.
 *
 * @complexity Constant.
 *
 * @ingroup ModularArithmetic
 */
template <typename T>
T modadd(T a, T b, T m)
{
  EULER_MODULAR_CHECK_TYPE(T);
  EULER_MODULAR_CHECK_MODULUS(m);
  EULER_MODULAR_CHECK_RESIDUE(a, m);
  EULER_MODULAR_CHECK_RESIDUE(b, m);
  return (a >= m - b)? a - (m - b) : a + b;
}

/**
 * Proper modular subtraction.
 *
 * @tparam T An integral type.
 *
 * @param a An integer within <code>[0, m)</code>.
 *
 * @param b An integer within <code>[0, m)</code>.
 *
 * @param m Modulus; must be a positive integer.
 *
 * @returns The unique integer @c c within <code>[0, m)</code> such that
 *    <code>a - b ≡ c (mod m)</code>.
 *
 * @exception std::invalid_argument if any argument is invalid.
 *
 * @remarks This function is guaranteed not to incur integer overflow.
 *
 * @complexity Constant.
 *
 * @ingroup ModularArithmetic
 */
template <typename T>
T modsub(T a, T b, T m)
{
  EULER_MODULAR_CHECK_TYPE(T);
  EULER_MODULAR_CHECK_MODULUS(m);
  EULER_MODULAR_CHECK_RESIDUE(a, m);
  EULER_MODULAR_CHECK_RESIDUE(b, m);
  return (a >= b)? a - b : a + (m - b);
}

namespace details {

template <typename T>
T modmul(T a, T b, T m, std::false_type /* no_wide_type */)
{
  typedef typename euler::make_wide<T>::type calc_type;
  return (static_cast<calc_type>(a) * static_cast<calc_type>(b)) % m;
}

/// @todo Add optimization if the product of ab will not overflow.
template <typename T>
T modmul(T a, T b, T m, std::true_type /* no_wide_type */)
{
  if (a < b)
  {
    std::swap(a, b);
  }
  return ipow(a, b, [m](T x, T y) -> T { return modadd(x, y, m); }, T(0));
}

} // namespace details

/**
 * Proper modular multiplication.
 *
 * @tparam T An integral type.
 *
 * @param a An integer within <code>[0, m)</code>.
 *
 * @param b An integer within <code>[0, m)</code>.
 *
 * @param m Modulus; must be a positive integer.
 *
 * @returns The unique integer @c c within <code>[0, m)</code> such that
 *    <code>a * b ≡ c (mod m)</code>.
 *
 * @exception std::invalid_argument if any argument is invalid.
 *
 * @remarks This function is guaranteed not to incur integer overflow.
 *
 * @algorithm If an integral type with twice the number of bits of @c T is
 *    available, a simple multiplication is performed and the result taken
 *    modulus of. Otherwise, the following procedure is performed to avoid
 *    potential integer overflow. Assume without loss of generality that
 *    <code>a ≥ b</code>. Write the binary expansion of @c b as
 *    @f[
 *      b = b_0 + b_1 \times 2 + \cdots + b_{n-1} \times 2^{n-1}
 *    @f]
 *    where <code>b<sub>i</sub> ∈ { 0, 1 }</code>. With this expansion, write
 *    the modular multiplication as
 *    @f[
 *      a \times b \equiv a \times b_0 + (a \times 2) \times b_1  + \cdots
 *                        + (a \times 2^{n-1}) \times b_{n-1} .
 *    @f]
 *    Evaluate the right-hand side of the equation by doubling @c a after each
 *    iteration and adding it to the result if the corresponding bit in @c b
 *    is @c 1.
 *
 * @timecomplexity If an integral type with twice the number of bits of @c T
 *    is available, constant. Otherwise, no more than
 *    <code>2*log<sub>2</sub>b</code> modular additions where @c b is the
 *    smaller operand.
 *
 * @spacecomplexity Constant.
 *
 * @ingroup ModularArithmetic
 */
template <typename T>
T modmul(T a, T b, T m)
{
  EULER_MODULAR_CHECK_TYPE(T);
  EULER_MODULAR_CHECK_MODULUS(m);
  EULER_MODULAR_CHECK_RESIDUE(a, m);
  EULER_MODULAR_CHECK_RESIDUE(b, m);
  using tag = std::conditional_t<
      std::is_void<typename euler::make_wide<T>::type>::value,
      std::true_type, std::false_type>;
  return details::modmul(a, b, m, tag());
}

/**
 * Modular multiplicative inverse.
 *
 * @tparam T An integral type.
 *
 * @param a An integer within <code>[1, m)</code> that is coprime to @c m.
 *
 * @param m Modulus; must be an integer that is greater than or equal to @c 2.
 *
 * @returns The unique integer @c x within <code>[1, m)</code> such that
 *   <code>a * x ≡ 1 (mod m)</code>.
 *
 * @exception std::invalid_argument if any argument is invalid.
 *
 * @remarks The requirement that @c a and @c m are coprime ensures that the
 *   modular multiplicative inverse @c x exists and is unique.
 *
 * @algorithm <a href="https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm">Extended Euclidean algorithm</a>.
 *
 * @timecomplexity <code>O(log m)</code>.
 *
 * @spacecomplexity Constant.
 *
 * @ingroup ModularArithmetic
 */
template <typename T>
T modinv(T a, T m)
{
  const auto &s = egcd(a, m);
  if (s.first != 1)
  {
    throw std::invalid_argument("Not coprime.");
  }
  return s.second.first;
}
#if 0
template <typename T>
T modinv(T a, T m)
{
  T r_2 = a, r_1 = m;
  T x_2 = 1, x_1 = 0;
  while (true)
  {
    T q = r_2 / r_1;
    T r = r_2 % r_1;
    T x = x_2 - q*x_1;
    if (r == 1)
    {
      if (x < 0)
      {
        x += m;
      }
      return x;
    }
    r_2 = r_1;
    r_1 = r;
    x_2 = x_1;
    x_1 = x;
  }
  return 1;
}
#endif

/**
 * Solves linear congruence equation.
 *
 * @tparam T An integral type.
 *
 * @param a An integer within <code>[0, m)</code> that is coprime to @c m.
 *
 * @param b An integer within <code>[0, m)</code>.
 *
 * @param m Modulus; must be a positive integer.
 *
 * @returns An integer @c x within <code>[0, m)</code> such that
 *   <code>a * x ≡ b (mod m)</code>.
 *
 * @exception std::invalid_argument if any argument is invalid.
 *
 * @remarks The requirement that @c a and @c m are coprime ensures that there
 *   exists a unique solution to the linear congruence equation. Two integers
 *   are coprime if @c 1 is the largest integer that divides both of them. In
 *   particular, if <code>m ≥ 2</code>, then @c a must be positive (to start
 *   with). If <code>m = 1</code>, then @c a and @c b must both be zero, in
 *   which case the (unique) solution is zero.
 *
 * @algorithm Multiply @c b by the multiplicative inverse of @c a.
 *
 * @timecomplexity <code>O(log m)</code>.
 *
 * @spacecomplexity Constant.
 *
 * @ingroup ModularArithmetic
 */
template <typename T>
T modsolve(T a, T b, T m)
{
  EULER_MODULAR_CHECK_TYPE(T);
  EULER_MODULAR_CHECK_MODULUS(m);
  EULER_MODULAR_CHECK_RESIDUE(a, m);
  EULER_MODULAR_CHECK_RESIDUE(b, m);
  if (m == 1)
  {
    return 0;
  }
  else
  {
    return modmul(b, modinv(a, m), m);
  }
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
  return ipow(base, exponent, [modulus](T x, T y) -> T
  {
    return modmul(x, y, modulus);
  }, T(1));
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
    mov ebx, 1;      // ebx = remainder
    mov ecx, exponent;  // ecx = exponent
    mov esi, m;      // esi = modulus
    mov edi, power;    // edi = power

begin_of_loop:
    shr ecx, 1;      // exponent >>= 1
    jnc update_power;  // if the last bit of exponent is zero, do nothing

    mov eax, edi;    // remainder = remainder * power mod m
    mul ebx;
    div esi;
    mov ebx, edx;

update_power:
    jecxz end_of_loop;
    mov eax, edi;    // power = power^2 mod m
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
