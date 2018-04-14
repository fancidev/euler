/**
 * @defgroup Divisor Divisor Functions
 * Routines to compute the divisors of an integer and its related functions.
 *
 * @ingroup Library
 *
 * This module provides routines to enumerate the divisors of an integer.
 * It also contains several routines to compute the divisor function of
 * an integer.
 */

#ifndef EULER_DIVISOR_H
#define EULER_DIVISOR_H

#include <limits>
#include <array>
#include "imath.hpp"
#include "prime_factor.hpp"
#include "sequence.hpp"

namespace euler {

#if 0
template <typename T, typename Func>
void get_divisors(T n, Func f)
{
  for (T i = 1; i <= n; i++)
  {
    auto d = div(n, i);
    if (d.quot < i)
      break;
    if (d.rem == 0)
    {
      f(i);
      if (i != d.quot)
        f(d.quot);
    }
  }
}
#endif

/**
 * Returns the number of divisors of an integer.
 *
 * To compute it, first factorize the integer, @c n as
 * @f[ n = p_1^{k_1} \cdots p_m^{k_m} @f]
 * and then compute the result by
 * @f[ \sigma_0(n) = (k_1+1) \cdots (k_m+1) . @f]
 *
 * @param n A positive integer.
 * @return The number of divisors of @c n.
 * @timecomplexity No more than <code>√n</code> operations.
 * @spacecomplexity Constant.
 * @remarks Since the number of divisors of @c n will not exceed @c n,
 *      it is safe to set the return type same as the type of @c n.
 *
 * @ingroup Divisor
 */
template <class T>
T count_divisors(T n)
{
  T count = 1;
  prime_factorize_distinct(n, [&count](T /* p */, int k) {
    count *= (k + 1);
  });
  return count;
}

/**
 * Returns the sum of divisors of an integer.
 *
 * To compute it, first factorize the integer, @c n as
 * @f[ n = p_1^{k_1} \cdots p_m^{k_m} , @f]
 * and then compute the result by
 * @f[ \sigma_1(n) = \prod_{i=1}^m (1+p_i+\cdots+p_i^{k_i}) . @f]
 *
 * @param n A positive integer.
 * @return The sum of divisors of @c n.
 * @timecomplexity <code>O(√n + (log<sub>2</sub>n)<sup>2</sup>)</code>.
 *      It takes no more than <code>√n</code> operations to factorize @c n,
 *      and no more than <code>(log<sub>2</sub>n)<sup>2</sup></code>
 *      operations to compute the result.
 * @spacecomplexity Constant.
 * @remarks The sum of divisors of @c n may exceed @c n,
 *      therefore the result may be subject to integer overflow.
 *
 * @ingroup Divisor
 */
template <class T>
T sum_divisors(T n)
{
  T sum = 1;
  prime_factorize_distinct(n, [&sum](T p, int k) {
    T pp = p, term = 1 + p;
    for (int i = 1; i < k; i++)
    {
      pp *= p;
      term += pp;
    }
    sum *= term;
  });
  return sum;
}

/// Define this to use the "loopless reflected mixed-radix Gray generation"
/// algorithm to iterate the divisors. If not defined, a simplistic algorithm
/// is used.
#ifndef EULER_LOOPLESS_DIVISOR_GENERATION
#define EULER_LOOPLESS_DIVISOR_GENERATION 0
#endif

/**
 * Iterator that enumerates the divisors of an integer in unspecified order.
 *
 * This iterator takes a sequence of (prime) factors as input, and generates
 * all possible products of the factors, in no specific order.
 *
 * Two algorithms are implemented to generate the products: the <i>simple</i>
 * algorithm and the <i>loopless</i> algorithm. The loopless algorithm has
 * superior theoretical time complexity, but in practice the difference is
 * marginal (and may even be reversed) for small number of factors. Therefore,
 * the simple algorithm is used by default. To select the loopless algorithm,
 * define the following before including any header from the Euler library:
 * \code
 * #define EULER_LOOPLESS_DIVISOR_GENERATION 1
 * \endcode
 * More details on the loopless algorithm can be found from
 * <a href="http://www.cs.utsa.edu/~wagner/knuth/fasc2a.pdf">this excerpt</a>
 * from <i>The Art of Computer Programming</i>, Vol 4A, Section 7.2.1.1.
 *
 * @todo Find out a way to return only half of the divisors (i.e. iterate
 *      pair of divisors. If done properly, this could improve the performance
 *      of divisor enumeration by 50%.
 * @todo Consider progressive/adaptive integer factorization, i.e. pre-sieve
 *      the smallest prime divisor for each number first, so that finding the
 *      divisors over this range of numbers is fast.
 * @ingroup Divisor
 */
template <class TDivisor, class Iter=factor_iterator<TDivisor>>
class divisor_iterator :
  public std::iterator<std::forward_iterator_tag,
  TDivisor, std::ptrdiff_t, const TDivisor *, const TDivisor &>
{
  // Type of prime factos. This can be narrower than the type of divisor.
  typedef typename Iter::value_type TFactor;

  // Type of exponent. Unsigned char suffices.
  typedef unsigned char TExponent;

  // Maximum number of binary bits in the divisor.
  static const int BinaryBits = 
    std::numeric_limits<typename std::make_unsigned<TDivisor>::type>::digits;

  // Maximum number of distinct prime factors.
  // By examining the product of the first few primes, 2*3*5*7*...,
  // we note that the product grows quite fast. In particular:
  // - For a 32-bit unsigned integer, 10 factors at most.
  // - For a 64-bit unsigned integer, 16 factors at most.
  static const int Bits = BinaryBits <= 32? 10 : BinaryBits <= 64? 16 : -1;

#if EULER_LOOPLESS_DIVISOR_GENERATION
  std::array<int, Bits+1> _flags;
#endif

  /// @cond NoDoc
  struct element_t
  {
    TFactor p;
    TExponent max;
    TExponent exp;
#if EULER_LOOPLESS_DIVISOR_GENERATION
    bool dec; // whether to decrease it
#else
    TDivisor p_k;  // p^max
#endif
  };
  /// @endcond

  std::array<element_t, Bits> _elems;
  int _nf;        // number of factors
  TDivisor _d;  // current divisor

  void initialize(const sequence<Iter> &factors)
  {
    _d = 1;
    _nf = 0;

    auto df = distinct(factors);
    for (auto it = df.begin(); it != df.end(); ++it, ++_nf)
    {
      // Stop if we have too many factors.
      assert(_nf < Bits);
      if (_nf >= Bits)
      {
        break;
      }

      _elems[_nf].p = *it;
      _elems[_nf].max = static_cast<TExponent>(it.frequency());
      _elems[_nf].exp = 0;
#if EULER_LOOPLESS_DIVISOR_GENERATION
      _elems[_nf].dec = false;
      _flags[_nf] = _nf;
#else
      _elems[_nf].p_k = ipow(static_cast<TDivisor>(*it), it.frequency());
#endif
    }
#if EULER_LOOPLESS_DIVISOR_GENERATION
    _flags[_nf] = _nf;
#endif
  }

public:

  /**
   * Constructs a divisor iterator from a sequence of prime factors.
   * The factors need not come in order, but equal factors must be adjacent.
   *
   * @param begin The begin iterator of prime factors.
   * @param end The end iterator of prime factors.
   * @timecomplexity <code>O(m)</code> where @c m is the number of supplied
   *      factors.
   * @spacecomplexity Object size is <code>O(1) + O(b)</code>
   *      where @c b is the number of bits in @c TDivisor.
   */
  divisor_iterator(const Iter &begin, const Iter &end)
  {
    initialize(make_sequence(begin, end));
  }

  /**
   * Constructs a divisor iterator from a sequence of prime factors.
   * The factors need not come in order, but equal factors must be adjacent.
   *
   * @param factors A sequence of prime factors.
   * @timecomplexity <code>O(m)</code> where @c m is the number of supplied
   *      factors.
   * @spacecomplexity Object size is <code>O(1) + O(b)</code>
   *      where @c b is the number of bits in @c TDivisor.
   */
  explicit divisor_iterator(const sequence<Iter> &factors)
  {
    initialize(factors);
  }

  /**
   * Constructs a divisor iterator for an integer.
   *
   * @param n A positive integer whose divisors are to be iterated.
   * @timecomplexity <code>O(√n + log<sub>2</sub>n)</code>.
   *      It takes no more than <code>√n</code> operations to factorize @c n,
     *      and the number of prime factors of @c n is no more than
   *      <code>log<sub>2</sub>n</code>.
   * @spacecomplexity Object size is <code>O(b)</code> where @c b is the
   *      number of bits in @c TDivisor.
   */
  explicit divisor_iterator(TDivisor n)
  {
    initialize(make_sequence(Iter(n), Iter()));
  }

  /// Constructs an empty divisor iterator that points <i>past-the-end</i>.
  /// @timecomplexity Constant.
  /// @spacecomplexity Object size is <code>O(b)</code> where @c b is the
  ///      number of bits in @c TDivisor.
  divisor_iterator() : _d(0) { }

  /// Returns the current divisor.
  /// @returns The current divisor.
  /// @complexity Constant.
  const TDivisor& operator * () const
  {
    return _d;
  }

  /**
   * Advances the iterator to point to the next divisor.
   * @returns The advanced iterator.
    * @timecomplexity If the loopless algorithm is selected, constant.
   *      If the simple algorithm is selected, <code>O(d)</code> where
   *      @c d is the number of distinct factors.
   * @spacecomplexity Constant.
   * @remarks The divisors are not iterated in any specific order,
   *      which means the next divisor may be larger or smaller than
   *      the current divisor.
   */
  divisor_iterator& operator ++ ()
  {
#if EULER_LOOPLESS_DIVISOR_GENERATION
    int j = _flags[0];
    _flags[0] = 0;
    if (j == _nf) // finish
    {
      _d = 0;
    }
    else
    {
      element_t &e = _elems[j];
      if (e.dec)
      {
        _d /= e.p;
        if (--e.exp == 0)
        {
          e.dec = false;
          _flags[j] = _flags[j+1];
          _flags[j+1] = j+1;
        }
      }
      else
      {
        _d *= e.p;
        if (++e.exp == e.max)
        {
          e.dec = true;
          _flags[j] = _flags[j+1];
          _flags[j+1] = j+1;
        }
      }
    }
#else
    int j = 0;
    for (; j < _nf && _elems[j].exp == _elems[j].max; j++)
    {
      _elems[j].exp = 0;
      _d /= _elems[j].p_k;
    }
    if (j >= _nf)
    {
      _d = 0;
    }
    else
    {
      ++_elems[j].exp;
      _d *= _elems[j].p;
    }
#endif
    return *this;
  }

  /**
   * Tests whether this iterator is equal to another iterator.
   * For performance reason, two iterators are equal if and only if
   * they are both <i>past-the-end</i>.
   * @param it Another divisor iterator.
   * @returns @c true if the two iterators are equal, @c false otherwise.
   * @complexity Constant.
   */
  bool operator == (const divisor_iterator &it) const
  {
    return (_d == 0) && (it._d == 0);
  }

  /**
   * Tests whether this iterator is not equal to another iterator.
   * For performance reason, two iterators are equal if and only if
   * they are both <i>past-the-end</i>.
   * @param it Another divisor iterator.
   * @returns @c true if the two iterators are not equal, @c false otherwise.
   * @complexity Constant.
   */
  bool operator != (const divisor_iterator &it) const
  {
    return ! operator == (it);
  }
};

/**
 * Enumerates the divisors of a pre-factorized integer.
 *
 * @param factors The prime factors of the integer whose divisors are
 *      to be returned.
 * @returns A sequence containing all possible products of the supplied
 *      factors. The products are enumerated in no specific order.
 * @timecomplexity <code>O(m)</code> where @c m is the number of supplied
 *      factors.
 * @spacecomplexity Object size is <code>O(b)</code> where @c b is the
 *      number of bits in @c T.
 *
 * @todo Use rvalue reference to eliminate the copying of return value.
 * @ingroup Divisor
 */
template <typename T, class Iter>
sequence<divisor_iterator<T,Iter>>
divisors(const sequence<Iter> &factors)
{
  return make_sequence(
    divisor_iterator<T,Iter>(factors.begin(), factors.end()),
    divisor_iterator<T,Iter>());
}

/**
 * Enumerates the divisors of an integer.
 *
 * @param n An integer whose divisors are to be returned.
 * @returns A sequence containing the divisors of @c n. The divisors are
 *      enumerated in no specific order.
 * @timecomplexity <code>O(√n + log<sub>2</sub>n)</code>.
 * @spacecomplexity Object size is <code>O(b)</code> where @c b is the
 *      number of bits in @c T.
 *
 * @todo Use rvalue reference to eliminate the copying of return value.
 * @ingroup Divisor
 */
template <typename T>
sequence<divisor_iterator<T>> divisors(T n)
{
  return make_sequence(divisor_iterator<T>(n), divisor_iterator<T>());
}

} // namespace euler

#endif
