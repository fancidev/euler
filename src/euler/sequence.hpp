/**
 * @defgroup sequence Sequence
 *
 * Iterators and Adaptors
 *
 * @ingroup Library
 */

#ifndef EULER_SEQUENCE_H
#define EULER_SEQUENCE_H

#include <algorithm>
#include <cassert>
#include <iterator>
#include <type_traits>
#include <utility>

namespace euler {

/**
 * Represents a sequence defined by a pair of iterators.
 * @ingroup sequence
 */
template <class Iter>
struct sequence : private std::pair<Iter, Iter>
{
  /**
   * Indicates the type of value in the sequence.
   */
  using value_type = typename std::iterator_traits<Iter>::value_type;

  /**
   * Constructs a sequence defined by a pair of iterators.
   *
   * @param begin Begin iterator.
   *
   * @param end End iterator.
   */
  sequence(const Iter &begin, const Iter &end)
    : std::pair<Iter, Iter>(begin, end) { }

#if 0
  /// Returns the begin iterator.
  Iter& begin() { return std::pair<Iter,Iter>::first; }
#endif

  /// Returns the begin iterator.
  Iter begin() const { return std::pair<Iter,Iter>::first; }

#if 0
  /// Returns the end iterator.
  Iter& end() { return std::pair<Iter,Iter>::second; }
#endif

  /// Returns the end iterator.
  Iter end() const { return std::pair<Iter,Iter>::second; }
};

/**
 * Constructs a sequence from a pair of iterators.
 *
 * @param begin Begin iterator.
 *
 * @param end End iterator.
 *
 * @returns A sequence defined by <code>[begin, end)</code>.
 *
 * @complexity Constant.
 *
 * @ingroup sequence
 */
template <class Iter>
sequence<Iter> make_sequence(const Iter &begin, const Iter &end)
{
  return sequence<Iter>(begin, end);
}

/**
 * Iterator adaptor that returns elements from two underlying sequences in
 * order.
 *
 * @tparam Iter Type of iterator that represents the underlying sequences.
 *    Must satisfy InputIterator requirements. The value type of @c Iter must
 *    be LessThanComparable.
 *
 * @remarks The underlying sequences must not be mutated during iteration.
 *    Otherwise the behavior is undefined.
 *
 * @see https://en.cppreference.com/w/cpp/concept/InputIterator for
 *    requirements of an input iterator.
 *
 * @ingroup sequence
 */
template <class Iter>
class merged_iterator
{
public:

  static_assert(std::is_base_of<std::input_iterator_tag,
      typename std::iterator_traits<Iter>::iterator_category>::value,
      "Iter must be an input iterator");

  /// Marks this iterator as an input iterator.
  using iterator_category = std::input_iterator_tag;

  /// Type of elements iterated.
  using value_type = typename std::iterator_traits<Iter>::value_type;

  /// Type of iterator difference.
  using difference_type =
      typename std::iterator_traits<Iter>::difference_type;

  /// Type of pointer to elements iterated.
  using pointer = typename std::iterator_traits<Iter>::pointer;

  /// Type of reference to elements iterated.
  using reference = typename std::iterator_traits<Iter>::reference;

private:
  Iter _it1, _end1;
  Iter _it2, _end2;

public:

  /**
   * Constructs the iterator from two underlying sequences whose elements are
   * merged.
   *
   * @param begin1 Begin iterator of first sequence.
   * @param end1   End iterator of first sequence.
   * @param begin2 Begin iterator of second sequence.
   * @param end2   End iterator of second sequence.
   *
   * @complexity Constant.
   */
  merged_iterator(
    const Iter &begin1, const Iter &end1,
    const Iter &begin2, const Iter &end2)
    : _it1(begin1), _end1(end1), _it2(begin2), _end2(end2) { }

  /**
   * Gets the current element.
   *
   * @returns Reference to the current element.
   *
   * @complexity Constant.
   */
  reference operator*()
  {
    assert(_it1 != _end1 || _it2 != _end2);
    if (_it2 == _end2)
    {
      return *_it1;
    }
    else if (_it1 == _end1)
    {
      return *_it2;
    }
    else
    {
      return (*_it1 < *_it2)? *_it1 : *_it2;
    }
  }

  /**
   * Advances the iterator.
   *
   * @complexity Constant.
   */
  merged_iterator& operator++()
  {
    assert(_it1 != _end1 || _it2 != _end2);
    if (_it2 == _end2)
    {
      ++_it1;
    }
    else if (_it1 == _end1)
    {
      ++_it2;
    }
    else
    {
      if (*_it1 <= *_it2)
      {
        ++_it1;
      }
      else
      {
        ++_it2;
      }
    }
    return *this;
  }

  /**
   * Equality test.
   *
   * @param a Merged iterator.
   * @param b Merged iterator.
   *
   * @returns @c true if @c a and @c b are constructed based on the same
   *    sequences and currently point to the same element.
   *
   * @complexity Constant.
   */
  friend bool operator==(const merged_iterator &a, const merged_iterator &b)
  {
    return (a._it1 == b._it1) && (a._end1 == b._end1)
        && (a._it2 == b._it2) && (a._end2 == b._end2);
  }

  /**
   * Inequality test.
   *
   * @complexity Constant.
   */
  friend bool operator!=(const merged_iterator &a, const merged_iterator &b)
  {
    return !(a == b);
  }
};

/// Merges two ordered sequences.
template <class Iter>
sequence<merged_iterator<Iter>>
merge(const sequence<Iter> &a, const sequence<Iter> &b)
{
  return make_sequence(
      merged_iterator<Iter>(a.begin(), a.end(), b.begin(), b.end()),
      merged_iterator<Iter>(a.end(), a.end(), b.end(), b.end()));
}

/**
 * Iterator adaptor that packs consecutive equal elements from an underlying
 * iterator.
 *
 * @tparam Iter Type of the underlying iterator; must satisfy InputIterator
 *    requirements. The value type of @c Iter must be EqualityComparable,
 *    DefaultConstructible and CopyAssignable, as values from the underlying
 *    sequence are copied during iteration.
 *
 * @see https://en.cppreference.com/w/cpp/concept/InputIterator for
 *    requirements of an input iterator.
 *
 * @ingroup sequence
 */
template <class Iter>
class distinct_iterator
{
public:

  static_assert(std::is_base_of<std::input_iterator_tag,
      typename std::iterator_traits<Iter>::iterator_category>::value,
      "Iter must be an input iterator");

  /// Marks this iterator as an input iterator.
  using iterator_category = std::input_iterator_tag;

  /// Type of the values enumerated.
  using value_type =
      std::pair<typename std::iterator_traits<Iter>::value_type, size_t>;

  /// Type of iterator differences.
  using difference_type =
      typename std::iterator_traits<Iter>::difference_type;

  /// No pointer to value is supported.
  using pointer = const value_type *;

  /// Type of reference to the values enumerated.
  using reference = const value_type &;

private:

  Iter _it, _end;
  value_type _current;

  void move_next()
  {
    if (_it == _end)
    {
      _current.first = typename std::iterator_traits<Iter>::value_type();
      _current.second = 0;
    }
    else
    {
      typename std::iterator_traits<Iter>::value_type value = *_it;
      size_t count = 1;
      while (++_it != _end && *_it == value)
      {
        ++count;
      }
      _current.first = value;
      _current.second = count;
    }
  }

public:

  /**
   * Creates the iterator based on an underlying sequence.
   *
   * @param begin Begin iterator of the underlying sequence.
   * @param end   End iterator of the underlying sequence.
   *
   * @complexity <code>O(k)</code> where @c k is the number of occurrences of
   *   the next value in the underlying sequence.
   */
  distinct_iterator(const Iter &begin, const Iter &end)
    : _it(begin), _end(end), _current()
  {
    move_next();
  }

  /**
   * Gets the current value along with its frequency.
   */
  reference operator*() const { return _current; }

  /**
   * Gets the current value along with its frequency.
   */
  pointer operator->() const { return &_current; }

  /**
   * Advances the iterator.
   *
   * @complexity <code>O(k)</code> where @c k is the number of occurrences of
   *    of the next value in the underlying sequence.
   */
  distinct_iterator& operator++()
  {
    move_next();
    return *this;
  }

  /**
   * Equality testing.
   *
   * @complexity Constant.
   */
  friend bool
  operator==(const distinct_iterator &a, const distinct_iterator &b)
  {
    return (a._current == b._current) && (a._it == b._it && a._end == b._end);
  }

  /**
   * Inequality testing.
   *
   * @complexity Constant.
   */
  friend bool
  operator!=(const distinct_iterator &a, const distinct_iterator &b)
  {
    return !(a == b);
  }
};

template <class Iter>
sequence<distinct_iterator<Iter>> distinct(const sequence<Iter> &a)
{
  return sequence<distinct_iterator<Iter>>(
    distinct_iterator<Iter>(a.begin(), a.end()),
    distinct_iterator<Iter>(a.end(), a.end()));
}

} // namespace euler

#endif // EULER_SEQUENCE_H
