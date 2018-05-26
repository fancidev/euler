/**
 * @defgroup prime_factor Integer Factorization
 *
 * Routines to factorize an integer into product of prime factors.
 *
 * Given a positive integer @c n, prime factorization writes @n as the unique
 * product of prime powers. That is,
 * @f[
 *   n = p_1^{k_1} \cdots p_m^{k_m}
 * @f]
 * where @f$ p_i @f$ are primes and @f$ k_i \ge 1 @f$. The existence and
 * uniqueness of such factorization is guaranteed by the fundamental theorem
 * of arithmetic.
 *
 * This module provides functions to factorize an integer through trial
 * division. Additional routines are provided to handle special cases.
 *
 * @see https://en.wikipedia.org/wiki/Fundamental_theorem_of_arithmetic
 * @see https://en.wikipedia.org/wiki/Integer_factorization
 *
 * @ingroup Library
 */

#ifndef EULER_PRIME_FACTOR_HPP
#define EULER_PRIME_FACTOR_HPP

#include <algorithm>
#include <cassert>
#include <iterator>
#include <type_traits>
#include "int_traits.hpp"
#include "sequence.hpp"

namespace euler {

/**
 * Factorizes an integer on a given sequence of factors.
 *
 * This function factorizes an integer, @c n, on a given sequence of potential
 * factors given by a pair of iterators <code>[primes_begin, primes_end)</code>.
 * It iterates through each element @c p in the sequence
 * and trial-divide @c n with @c p. If @c p divides @c n, then it calls
 * <code>f(p)</code> and removes @c p once from @c n.
 *
 * If the entire factor sequence is iterated and @c n cannot be fully
 * factorized (i.e. <code>n > 1</code>), it calls <code>f(n)</code>.
 *
 * If <code>n ≤ 1</code>, no factorization will be performed and the function
 * returns immediately.
 *
 * @param n The number to factorize. Must be a positive integer.
 * @param f The callback function of signature <code>void f(T)</code>.
 *      Its return value, if any, is ignored.
 * @param primes_begin The begin iterator of the factor sequence.
 * @param primes_end The end iterator of the factor sequence.
 *
 * @ingroup prime_factor
 */
template <class T, class Func, class FwdIt>
void prime_factorize(T n, Func f, FwdIt primes_begin, FwdIt primes_end)
{
  // divide by each prime known prime number until sqrt(n) or until there are
  // no more known prime number.
  for (FwdIt it = primes_begin; n > 1 && it != primes_end; )
  {
    T p = *it;
    T q = n / p;
    if (n % p == 0)
    {
      n = q;
      f(p);
    }
    else
    {
      if (q < p)
      {
        f(n);
        break;
      }
      ++it;
    }
  }
  if (n > 1)
  {
    f(n);
  }
}

/**
 * Iterator that enumerates the prime factors of a positive integer in order.
 *
 * @tparam T Type of integer to factorize; must be integral.
 *
 * @remarks Prime factors of the given integer are guaranteed to be returned
 *    in non-decreasing order. A prime factor of power @c k is returned @c k
 *    times. The integer one (1) has no prime factor.
 *
 * @algorithm Trial division.
 *
 * @timecomplexity <code>O(√n)</code> for enumerating all factors.
 *
 * @spacecomplexity Constant.
 *
 * @ingroup prime_factor
 */
template <class T>
class factor_iterator : public std::iterator<
    std::input_iterator_tag, T, std::ptrdiff_t, const T*, const T&>
{
  static_assert(std::is_integral<T>::value, "T must be integral");

  T n; // remaining number to factorize; always >= 1
  T p; // current prime factor; 0 if finished

  void move_to_next_factor()
  {
    assert(n > 0);
    assert(p > 0);
    if (n == 1)
    {
      p = 0;
      n = 1;
    }
    else if (n % p == 0)
    {
      n /= p;
    }
    else
    {
      p = (p == 2)? 3 : p + 2;
      while (n > 1)
      {
        T q = n / p;
        if (n % p == 0)
        {
          n = q;
          break;
        }
        else
        {
          if (q <= p)
          {
            p = n;
            n = 1;
            break;
          }
          p += 2;
        }
      }
    }
  }

public:

  /**
   * Constructs an iterator to enumerate the prime factors of an integer.
   *
   * @param n Positive integer to factorize.
   *
   * @timecomplexity <code>O(p)</code> where @c p is the smallest prime factor
   *    of @c n.
   *
   * @spacecomplexity Constant.
   */
  explicit factor_iterator(T n) : n(n), p(2)
  {
    assert(n > 0);
    move_to_next_factor();
  }

  /**
   * Constructs a sentinel iterator that indicates factor exhaustion.
   *
   * @complexity Constant.
   */
  factor_iterator() : n(1), p(0) { }

  /**
   * Returns the current factor.
   *
   * @complexity Constant.
   */
  const T& operator*() const
  {
    assert(p != 0);
    return p;
  }

  /**
   * Advances the iterator to the next prime factor of the integer.
   *
   * @returns The advanced iterator.
   *
   * @timecomplexity <code>O(√n)</code> for the entire iteration.
   *
   * @spacecomplexity Constant.
   */
  factor_iterator& operator++()
  {
    move_to_next_factor();
    return *this;
  }

  /**
   * Equality testing.
   *
   * @complexity Constant.
   */
  friend bool operator==(const factor_iterator &a, const factor_iterator &b)
  {
    return (a.n == b.n) && (a.p == b.p);
  }

  /**
   * Inequality testing.
   *
   * @complexity Constant.
   */
  friend bool operator!=(const factor_iterator &a, const factor_iterator &b)
  {
    return !(a == b);
  }
};

/**
 * Factorizes a positive integer.
 *
 * @tparam T Integral type.
 *
 * @param n Positive integer to factorize.
 *
 * @returns A sequence which, when iterated, returns the prime factors of @c n
 *    in non-decreasing order, possibly with repetition.
 *
 * @timecomplexity <code>O(p)</code> where @c p is the smallest prime factor
 *    of @c n.
 *
 * @spacecomplexity Constant.
 *
 * @ingroup prime_factor
 */
template <class T>
sequence<factor_iterator<T>> factorize(T n)
{
  return make_sequence(factor_iterator<T>(n), factor_iterator<T>());
}

#if 0
// Prime factor sieve.
// Store the smallest prime factor of all numbers below N.
template <typename T>
class factor_table
{
  typedef typename make_half<T>::type TFactor;

  T max; // maximum number whose prime factor is built
  std::vector<TFactor> factor; // smallest prime factor of odd number

public:
  factor_table(T N) : max(N), factor((N+1)/2)
  {
    // Stores the smallest prime divisor of a number.
    // Only the divisor of odd numbers are stored.
    // 1 -> 0, 3 -> 1, 5 -> 2, ..., n -> (int)(n/2)
    factor[0] = 1;
    for (T p = 3; p*p <= N; p += 2)
    {
      if (factor[p/2] == 0) // p is prime
      {
        for (T k = p*p; k <= N; k += 2*p)
        {
          if (factor[k/2] == 0)
            factor[k/2] = (TFactor)p;
        }
      }
    }
  }

  T operator [](T n) const
  {
    assert(n > 0);
    assert(n <= max);
    if (n % 2 == 0)
      return 2;
    else if (factor[n/2] == 0) // is prime
      return n;
    else
      return factor[n/2];
  }

  bool is_prime(T n) const
  {
    assert(n > 0);
    assert(n <= max);
    if (n == 2)
      return true;
    if (n % 2 == 0)
      return false;
    return factor[n/2] == 0;
  }

};
#endif

} // namespace euler

#endif // EULER_PRIME_FACTOR_HPP
