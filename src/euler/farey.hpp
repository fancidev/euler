/**
* @defgroup farey Farey Sequence
*
* The <i>Farey sequence</i> of order @c n is the sequence of reduced fractions
* between 0 and 1 with denominator less than or equal to @c n, arranged in
* increasing order of value.
*
* @see https://en.wikipedia.org/wiki/Farey_sequence
*
* @ingroup Library
*/

#ifndef EULER_FAREY_HPP
#define EULER_FAREY_HPP

#include <cassert>
//#include <iterator>
#include <numeric>
#include <utility>
//#include "fraction.hpp"
#include "gcd.hpp"
#include "modular.hpp"
#include "totient.hpp"

namespace euler {

/**
 * Gets the term in the farey sequence of order @c n preceding <c>a/b</c>.
 *
 * @tparam T Type of denominator; must be integral.
 *
 * @param n Order of sequence; must be positive.
 * @param a Numerator; must be within <c>[0, b]</c> and coprime to @c b.
 * @param b Denominator; must be within <c>[1, n]</c>.
 *
 * @returns If <c>a/b == 0/1<c> (the first term of the sequence), returns
 *    <c>0/0</c>. Otherwise, returns the largest reduced fraction <c>p/q</c>
 *    such that <c>q <= n</c> and <c>p/q < a/b</c>.
 *
 * @ingroup farey
 */
template <class T>
std::pair<T,T> farey_prev(T n, T a, T b)
{
  static_assert(std::is_integral<T>::value, "T must be integral");
  assert(n >= 1);
  assert(b >= 1 && b <= n);
  assert(a >= 0 && a <= b);
  assert(gcd(a, b) == 1);

  if (a == 0 && b == 1)
  {
    return std::pair<T,T>(0, 0);
  }
  if (a == 1 && b == 1)
  {
    return std::pair<T,T>(n-1, n);
  }

  T y0 = modinv(a, b);
  T k = (n-y0) / b;
  T y = y0 + k*b;
  T x = (a*y-1) / b;
  return std::pair<T,T>(x, y);
}

/**
 * Gets the term in the farey sequence of order @c n following <c>a/b</c>.
 *
 * @tparam T Type of denominator; must be integral.
 *
 * @param n Order of sequence; must be positive.
 * @param a Numerator; must be within <c>[0, b]</c> and coprime to @c b.
 * @param b Denominator; must be within <c>[1, n]</c>.
 *
 * @returns If <c>a/b == 1/1<c> (the last term of the sequence), returns
 *    <c>0/0</c>. Otherwise, returns the smallest reduced fraction <c>p/q</c>
 *    such that <c>q <= n</c> and <c>p/q > a/b</c>.
 *
 * @ingroup farey
 */
template <class T>
std::pair<T,T> farey_next(T n, T a, T b)
{
  assert(n >= 1);
  assert(b >= 1 && b <= n);
  assert(a >= 0 && a <= b);
  assert(gcd(a, b) == 1);

  if (a == 0 && b == 1)
  {
    return std::pair<T,T>(1, n);
  }
  if (a == 1 && b == 1)
  {
    return std::pair<T,T>(0, 0);
  }

  T x0 = modinv(b, a);
  T y0 = (b*x0-1) / a;
  T k = (n-y0) / b;
  T y = y0 + k*b;
  T x = (a*y+1) / b;
  return std::pair<T,T>(x, y);
}

/**
 * Gets the term in the farey sequence of order @c n preceding the consecutive
 * terms <c>a/b, c/d</c>.
 *
 * @tparam T Type of denominator; must be integral.
 *
 * @param n Order of sequence; must be positive.
 * @param a Numerator; must be within <c>[0, b]</c> and coprime to @c b.
 * @param b Denominator; must be within <c>[1, n]</c>.
 * @param c Numerator; must be within <c>[0, d]</c> and coprime to @c d.
 * @param d Denominator; must be within <c>[1, n]</c>.
 *
 * @returns If <c>a/b == 0/1<c> (the first term of the sequence), returns
 *    <c>0/0</c>. Otherwise, returns the largest reduced fraction <c>p/q</c>
 *    such that <c>q <= n</c> and <c>p/q < a/b</c>.
 *
 * @remarks If <c>a/b</c> and <c>c/d</c> are not consecutive terms, the
 *    behavior is undefined.
 *
 * @ingroup farey
 */
template <class T>
std::pair<T,T> farey_prev(T n, T a, T b, T c, T d)
{
  if (a == 0 && b == 1)
  {
    return std::pair<T,T>(0, 0);
  }
  else
  {
    T k = (n+d)/b;
    T y = k*b-d;
    T x = k*a-c;
    return std::pair<T,T>(x, y);
  }
}

/**
 * Gets the term in the farey sequence of order @c n following the consecutive
 * terms <c>a/b, c/d</c>.
 *
 * @tparam T Type of denominator; must be integral.
 *
 * @param n Order of sequence; must be positive.
 * @param a Numerator; must be within <c>[0, b]</c> and coprime to @c b.
 * @param b Denominator; must be within <c>[1, n]</c>.
 * @param c Numerator; must be within <c>[0, d]</c> and coprime to @c d.
 * @param d Denominator; must be within <c>[1, n]</c>.
 *
 * @returns If <c>c/d == 1/1<c> (the last term of the sequence), returns
 *    <c>0/0</c>. Otherwise, returns the smallest reduced fraction <c>p/q</c>
 *    such that <c>q <= n</c> and <c>p/q > c/d</c>.
 *
 * @remarks If <c>a/b</c> and <c>c/d</c> are not consecutive terms, the
 *    behavior is undefined.
 *
 * @ingroup farey
 */
template <class T>
std::pair<T,T> farey_next(T n, T a, T b, T c, T d)
{
  if (c == 1 && d == 1)
  {
    return std::pair<T,T>(0, 0);
  }
  else
  {
    T k = (n+b)/d;
    T y = k*d-b;
    T x = k*c-a;
    return std::pair<T,T>(x, y);
  }
}

/**
 * Gets the number of terms in the farey sequence of order @c n.
 *
 * @tparam TRet Return type; must be integral.
 * @tparam T Denominator type; must be integral.
 *
 * @param n Order of sequence; must be positive.
 *
 * @returns Number of terms in the farey sequence of order @c n, including
 *    the first term (<c>0/1</c>) and the last term (<c>1/1</c>). This is
 *    equal to the number of coprime integers @c p, @c q such that
 *    <c>p < q <= n</c> plus two.
 *
 * @remarks The return value is on the order of <c>n^2</c>. Therefore the
 *    return type should be large enough to avoid overflow.
 *
 * @ingroup farey
 */
template <class TRet, class T>
TRet farey_size(T n)
{
  assert(n >= 1);
  std::vector<T> phi(n + 1);
  euler::totient_table(phi);
  return std::accumulate(phi.begin(), phi.end(), TRet(1));
}

#if 0
template <class T> class farey_sequence;

template <class T>
class farey_sequence_iterator :
  public std::iterator<
    std::bidirectional_iterator_tag,
    fraction<T>,
    ptrdiff_t,
    const fraction<T> *,
    const fraction<T> &>
{
private:
  fraction<T> _frac, _prev, _next;
  T _order;

  static fraction<T> get_prev_term(T order, const fraction<T> &f)
  {
    return get_prev_term(order, f.p, f.q);
  }

  static fraction<T> get_next_term(T order, const fraction<T> &f)
  {
    return get_next_term(order, f.p, f.q);
  }

  static fraction<T> get_prev_term(T order, const fraction<T> &f1, const fraction<T> &f2)
  {
    return get_prev_term(order, f1.p, f1.q, f2.p, f2.q);
  }

  static fraction<T> get_next_term(T order, const fraction<T> &f1, const fraction<T> &f2)
  {
    return get_next_term(order, f1.p, f1.q, f2.p, f2.q);
  }

  static const fraction<T> first;
  static const fraction<T> last;
  static const fraction<T> null;

  farey_sequence_iterator(T n, T p, T q) : _order(n), _frac(p, q)
  {
    if (_frac == null)
    {
      _prev = null;
      _next = null;
    }
    else if (_frac == first)
    {
      _prev = null;
      _next = get_next_term(n, _frac);
    }
    else if (_frac == last)
    {
      _prev = get_prev_term(n, _frac);
      _next = null;
    }
    else
    {
      _prev = get_prev_term(n, _frac);
      _next = get_next_term(n, _prev, _frac);
    }
  }

  friend class farey_sequence<T>;

public:
  farey_sequence_iterator(const farey_sequence_iterator &it)
    : _order(it._order), _frac(it._frac), _prev(it._prev), _next(it._next)
  {
  }

  farey_sequence_iterator& operator ++ ()
  {
    assert(_frac != null);
    _prev = _frac;
    _frac = _next;
    _next = (_frac == null)? null : get_next_term(_order, _prev, _frac);
    return *this;
  }

  farey_sequence_iterator& operator -- ()
  {
    assert(_frac != null);
    _next = _frac;
    _frac = _prev;
    _prev = (_frac == null)? null : get_prev_term(_order, _frac, _next);
    return *this;
  }

  fraction<T> operator * () const
  {
    assert(_frac != null);
    return _frac;
  }

  const fraction<T>* operator -> () const
  {
    assert(_frac != null);
    return &_frac;
  }

  bool operator == (const farey_sequence_iterator &i) const
  {
    return (_order == i._order && _frac == i._frac);
  }

  bool operator != (const farey_sequence_iterator &i) const
  {
    return !operator == (i);
  }
};

template <typename T> const fraction<T> farey_sequence_iterator<T>::first = fraction<T>(0, 1);
template <typename T> const fraction<T> farey_sequence_iterator<T>::last = fraction<T>(1, 1);
template <typename T> const fraction<T> farey_sequence_iterator<T>::null = fraction<T>(0, 0);

template <typename T>
class farey_sequence
{
private:
  T _order;

public:
  farey_sequence(T order) : _order(order)
  {
    assert(order >= 1);
  }

  T order() const { return _order; }

public:
  typedef typename unsigned long long size_type;
  typedef typename farey_sequence_iterator<T> iterator;

  iterator begin() const
  {
    return iterator(_order, 0, 1);
  }

  iterator end() const
  {
    return iterator(_order, 0, 0);
  }

  iterator find(T p, T q) const
  {
    if (p > q || greatest_common_divisor(p, q) != 1)
      return end();
    else
      return iterator(_order, p, q);
  }

  size_type size() const
  {
    std::vector<int> phi(_order+1);
    totient(phi);
    return std::accumulate(phi.cbegin()+1, phi.cend(), 0ULL) + 1;
  }
};
#endif

} // namespace euler

#endif // EULER_FAREY_HPP
