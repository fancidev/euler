/**
 * @defgroup combination Combination Enumeration
 *
 * Routines to enumerate combinations.
 *
 * @ingroup Library
 */

#ifndef EULER_COMBINATION_HPP
#define EULER_COMBINATION_HPP

#include <algorithm>
#include <cassert>
#include <iterator>
#include <numeric>
#include <vector>
#include "sequence.hpp"

namespace euler {

/**
 * Iterator that enumerates in lexical order the ways of choosing r items out
 * of n items without repetition and without ordering.
 *
 * @ingroup combination
 */
template <class TIndex = size_t>
class combination_iterator
{
  const TIndex _n; // number of items to choose from
  const TIndex _r; // number of items to choose
  std::vector<TIndex> _choice; // current choice
  bool _dereferenceable;

public:

  using iterator_category = std::forward_iterator_tag;
  using value_type = std::vector<TIndex>;
  using difference_type = std::ptrdiff_t;
  using reference = const value_type &;
  using pointer = const value_type *;

public:

  combination_iterator() : _n(0), _r(0), _choice(), _dereferenceable(false)
  {
  }

  combination_iterator(TIndex n, TIndex r)
    : _n(n), _r(r), _choice(r), _dereferenceable(true)
  {
    assert(n >= r && r >= 0);
    std::iota(_choice.begin(), _choice.end(), TIndex(0));
  }

  /**
   * Gets a reference to the current combination.
   *
   * The returned reference is invalidated if the iterator is advanced or
   * destroyed.
   */
  reference operator*() const
  {
    assert(_dereferenceable);
    return _choice;
  }

  /**
   * Gets a pointer to the current combination.
   *
   * The returned pointer is invalidated if the iterator is advanced or
   * destroyed.
   */
  pointer operator->() const
  {
    assert(_dereferenceable);
    return &_choice;
  }

  combination_iterator & operator++()
  {
    assert(_dereferenceable);

    TIndex k = _r;
    while (k > 0 && _choice[k - 1] == (_n - 1) - (_r - k))
    {
      --k;
    }

    if (k == 0)
    {
      _dereferenceable = false;
    }
    else
    {
      std::iota(_choice.begin() + (k - 1), _choice.end(), _choice[k - 1] + 1);
    }
    return *this;
  }

  bool operator==(const combination_iterator &other) const
  {
    if (_dereferenceable && other._dereferenceable)
    {
      return _n == other._n && _r == other._r && _choice == other._choice;
    }
    else
    {
      return _dereferenceable == other._dereferenceable;
    }
  }

  bool operator!=(const combination_iterator &other) const
  {
    return !operator==(other);
  }
};

inline sequence<combination_iterator<size_t>> choose(size_t n, size_t r)
{
  return make_sequence(
      combination_iterator<size_t>(n, r),
      combination_iterator<size_t>());
}

/**
 * Iterator that enumerates in lexical order the cartesian product of sets.
 *
 * @ingroup combination
 */
template <class TIndex = size_t>
class product_iterator
{
  std::vector<TIndex> _length; // _length[i] = number of elements in set i
  std::vector<TIndex> _choice; // current cartesian product
  bool _dereferenceable;

public:

  using iterator_category = std::forward_iterator_tag;
  using value_type = std::vector<TIndex>;
  using difference_type = std::ptrdiff_t;
  using reference = const value_type &;
  using pointer = const value_type *;

public:

  /**
   * Creates an iterator that points past-the-end.
   */
  product_iterator() : _dereferenceable(false)
  {
  }

  /**
   * Creates an iterator that enumerates the Cartesian product of sets of the
   * given cardinalities.
   *
   * @param card_first Iterator pointing to the cardinality of the first set
   * @param card_last  Iterator pointing past the cardinality of the last set
   */
  template <class InputIt>
  product_iterator(InputIt card_first, InputIt card_last)
    : _length(card_first, card_last), _choice(_length.size(), 0),
      _dereferenceable(true)
  {
    for (TIndex card: _length)
    {
      if (card < 0)
      {
        throw std::invalid_argument("Cardinality must be non-negative");
      }
      if (card == 0)
      {
        _dereferenceable = false;
      }
    }
  }

  /**
   * Creates an iterator that enumerates the Cartesian product of @c n sets
   * each of cardinality @c card.
   *
   * @param n Number of sets.
   * @param card Cardinality of each set; must be non-negative.
   */
  product_iterator(size_t n, TIndex card)
    : _length(n, card), _choice(n, 0), _dereferenceable(card > 0)
  {
    if (card < 0)
    {
      throw std::invalid_argument("Cardinality must be non-negative");
    }
  }

  /**
   * Gets a reference to the current Cartesian product.
   *
   * The returned reference is invalidated if the iterator is advanced or
   * destroyed.
   */
  reference operator*() const
  {
    assert(_dereferenceable);
    return _choice;
  }

  /**
   * Gets a pointer to the current Cartesian product.
   *
   * The returned reference is invalidated if the iterator is advanced or
   * destroyed.
   */
  pointer operator->() const
  {
    assert(_dereferenceable);
    return &_choice;
  }

  product_iterator & operator++()
  {
    assert(_dereferenceable);
    for (size_t j = _length.size(); j-- > 0; )
    {
      if (_choice[j] == _length[j] - 1)
      {
        _choice[j] = 0;
      }
      else
      {
        ++_choice[j];
        return *this;
      }
    }
    _dereferenceable = false;
    return *this;
  }

  bool operator==(const product_iterator &other) const
  {
    if (_dereferenceable && other._dereferenceable)
    {
      return _choice == other._choice && _length == other._length;
    }
    else
    {
      return _dereferenceable == other._dereferenceable;
    }
  }

  bool operator!=(const product_iterator &other) const
  {
    return !operator==(other);
  }
};

template <class InputIt>
sequence<product_iterator<size_t>> cartesian(
    InputIt card_first, InputIt card_last)
{
  return make_sequence(
      product_iterator<size_t>(card_first, card_last),
      product_iterator<size_t>());
}

} // namespace euler

#endif // EULER_COMBINATION_HPP
