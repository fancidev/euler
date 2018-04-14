/**
 * @defgroup PrimeFactor Prime Factorization
 * Routines to find the prime factors of an integer.
 *
 * @ingroup Library
 *
 * Prime factorization is the process of writing a positive integer, @c n
 * as the unique product of a set of primes, i.e.
 * @f[ n = p_1^{k_1} \cdots p_m^{k_m} . @f]
 *
 * This module contains routines to find this factorization through trial
 * division. It also provides several useful routines to handle specific
 * scenarios.
 */

#ifndef EULER_PRIME_FACTOR_HPP
#define EULER_PRIME_FACTOR_HPP

#include <iterator>
#include <algorithm>
#include "int_traits.hpp"
#include "sequence.hpp"

namespace euler {

/**
 * Finds the prime factors of an integer.
 *
 * Given an integer @c n to factorize, this function trial-divides @c n with
 * every integer @c p starting from @c 2 until <code>√n</code>. If @c p divides
 * @c n, then @c p is a prime factor of @c n and it calls <code>f(p)</code>
 * and removes @c p once from @c n.
 *
 * In particular, if @c n is prime, then only <code>f(n)</code> is called.
 *
 * Note that @c 1 is <i>not</i> considered prime. If <code>n = 1</code>, @c f
 * is never called.
 *
 * @param n The integer to factorize. Must be positive.
 * @param f The callback function of signature <code>void f(T)</code>.
 *      Its return value, if any, is ignored.
 * @timecomplexity No more than <code>√n</code> trial divisions.
 * @spacecomplexity Constant.
 * @remarks The factors are guaranteed to be returned from smallest to largest.
 *
 * @ingroup PrimeFactor
 */
template <class T, class Func>
void prime_factorize(T n, Func f)
{
  if (n <= 1)
  {
    return;
  }

  // divide by 2
  while (n % 2 == 0)
  {
    n >>= 1;
    f(2);
  }

  // divide by odd number until sqrt(n)
  T p = 3;
  while (n > 1)
  {
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
      p += 2;
    }
  }
}

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
 * @ingroup PrimeFactor
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
 * Finds the distinct prime factors of an integer.
 *
 * This function uses <code>prime_factorize(n,∙)</code> to factorize an
 * integer @c n. For each distinct prime factor <code>p<sup>k</sup></code>,
 * it calls <code>f(p, k)</code>.
 *
 * If <code>n ≤ 1</code>, no factorization will be performed and @c f is never
 * called.
 *
 * @param n The integer to factorize. Must be positive.
 * @param f The callback function of signature <code>void f(T, int)</code>.
 *     Its return value, if any, is ignored.
 * @timecomplexity No more than <code>√n</code> trial divisions.
 * @spacecomplexity Constant.
 * @remarks The factors are guaranteed to be returned from smallest to largest.
 *
 * @ingroup PrimeFactor
 */
template <class T, class Func>
void prime_factorize_distinct(T n, Func f)
{
  T last_p = 0;
  int last_k = 0;
  prime_factorize(n, [&](T p) {
    if (p != last_p)
    {
      if (last_p > 0)
      {
        f(last_p, last_k);
      }
      last_p = p;
      last_k = 1;
    }
    else
    {
      ++last_k;
    }
  });
  if (last_p > 0)
  {
    f(last_p, last_k);
  }
}

/**
 * Forward iterator that enumerates the prime factors of an integer in order.
 *
 * This iterator enumerates the prime factors of an integer through trial
 * division. The prime factors returned are guaranteed to be in increasing
 * order. If a prime factor occurs multiple times in the number, the factor
 * is iterated that many times.
 *
 * The time complexity of enumerating all the factors is <code>O(√n)</code>.
 * The space complexity is constant.
 *
 * @ingroup PrimeFactor
 */
template <typename T>
class factor_iterator
  : public std::iterator<std::forward_iterator_tag, T, std::ptrdiff_t, const T*, const T&>
{
  T n;
  T p;

  void move_to_next_factor()
  {
    if (n <= 1)
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
   * @param number An integer whose prime factors are to be iterated.
   *      Must be positive.
   * @timecomplexity <code>O(p)</code> where @c p if the smallest prime
   *      factor of @c n if @c is composite; <code>O(√n)</code> if @c n
   *      is prime.
   * @spacecomplexity Constant.
   */
  explicit factor_iterator(T number) : n(number), p(2)
  {
    move_to_next_factor();
  }

  /// Constructs an empty factor iterator that points <i>past-the-end</i>.
  /// @complexity Constant.
  factor_iterator() : n(1), p(0) { }

  /// Returns the current prime factor.
  /// @complexity Constant.
  const T& operator * () const { return p; }

  /**
   * Advances the iterator to point to the next prime factor of the number.
   * @returns The advanced iterator.
   * @timecomplexity <code>O(√n)</code> for the entire iteration.
   * @spacecomplexity Constant.
   */
  factor_iterator& operator ++ ()
  {
    move_to_next_factor();
    return *this;
  }

  /// Tests whether this iterator is equal to another iterator.
  /// @complexity Constant.
  bool operator == (const factor_iterator &it) const
  {
    return n == it.n && p == it.p;
  }

  /// Tests whether this iterator is not equal to another iterator.
  /// @complexity Constant.
  bool operator != (const factor_iterator &it) const
  {
    return ! operator == (it);
  }
};

/**
 * Returns a sequence of the prime factors of an integer.
 * @param n The number to factorize.
 * @complexity See <code>factor_iterator<T>(n)</code>.
 @ @ingroup PrimeFactor
 */
template <typename T>
sequence<factor_iterator<T>> factors(T n)
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

#if 0
static void test()
{
  int a = 12345678, b = 87654321;
  namespace p = euler::prime;

  std::cout << a << " =";
  for (auto it = p::factors(a).begin(); it != p::factors(a).end(); ++it)
  {
    std::cout << ' ' << *it;
  }
  std::cout << std::endl;

  std::cout << b << " =";
  for (auto it = p::factors(b).begin(); it != p::factors(b).end(); ++it)
  {
    std::cout << ' ' << *it;
  }
  std::cout << std::endl;

  std::cout << a << " * " << b << " =";
  for (auto it = p::factors(p::product(a, b)).begin();
    it != p::factors(p::product(a, b)).end(); ++it)
  {
    std::cout << ' ' << *it;
  }
  std::cout << std::endl;

  std::cout << a << " * " << b << " =";
  for (auto it = p::distinct_factors(p::product(a, b)).begin();
    it != p::distinct_factors(p::product(a, b)).end(); ++it)
  {
    std::cout << ' ' << it->first << "^" << it->second;
  }
  std::cout << std::endl;
}
#endif

#endif // EULER_PRIME_FACTOR_HPP
