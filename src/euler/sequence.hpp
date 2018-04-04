/**
 * @defgroup Sequence Sequences Iterators and Adaptors
 * @ingroup Library
 */

#ifndef EULER_SEQUENCE_H
#define EULER_SEQUENCE_H

#include <utility>
#include <algorithm>
#include <cassert>
#include <iterator>

namespace euler {

/**
 * Represents a sequence defined by a pair of iterators.
 * @ingroup Sequence
 */
template <class Iter>
struct sequence : private std::pair<Iter,Iter>
{
	/// Constructs a sequence defined by a pair of iterators.
	/// @param begin The begin iterator.
	/// @param end The end iterator.
	/// @complexity Constant.
	sequence(const Iter &begin, const Iter &end)
		: std::pair<Iter,Iter>(begin, end) { }

	/// Returns the begin iterator.
	Iter& begin() { return std::pair<Iter,Iter>::first; }

	/// Returns the begin iterator.
	const Iter& begin() const { return std::pair<Iter,Iter>::first; }

	/// Returns the end iterator.
	Iter& end() { return std::pair<Iter,Iter>::second; }

	/// Returns the end iterator.
	const Iter& end() const { return std::pair<Iter,Iter>::second; }
};

/**
 * Constructs a sequence from a pair of iterators.
 * @param begin The begin iterator.
 * @param end The end iterator.
 * @returns A sequence defined by <code>[begin, end)</code>.
 * @complexity Constant.
 * @ingroup Sequence
 */
template <class Iter>
sequence<Iter> make_sequence(const Iter &begin, const Iter &end)
{
	return sequence<Iter>(begin, end);
}

/// Iterator that returns elements from two underlying iterators in order.
/// @ingroup Sequence
template <class FwdIt>
class merged_iterator
{
public:

	/// Marks this iterator as a forward iterator.
	typedef std::forward_iterator_tag iterator_category;

	/// Type of elements iterated.
	typedef typename FwdIt::value_type value_type;

	/// Type of iterator difference.
	typedef typename FwdIt::difference_type difference_type;

	/// Type of pointer to elements iterated.
	typedef typename FwdIt::pointer pointer;

	/// Type of reference to elements iterated.
	typedef typename FwdIt::reference reference;

private:
	FwdIt it1, end1;
	FwdIt it2, end2;

public:

	/// Constructs an iterator adapator that merges elements from two 
	/// underlying iterators.
	/// @complexity Constant.
	merged_iterator(
		const FwdIt &_begin1, const FwdIt &_end1, 
		const FwdIt &_begin2, const FwdIt &_end2)
		: it1(_begin1), end1(_end1), it2(_begin2), end2(_end2) { }

	/// Returns the current element.
	/// @complexity Constant.
	reference operator * ()
	{
		assert(it1 != end1 || it2 != end2);
		if (it2 == end2)
			return *it1;
		else if (it1 == end1)
			return *it2;
		else
			return std::min(*it1, *it2);
	}

	/// Advances the iterator.
	/// @complexity Constant.
	merged_iterator& operator ++ ()
	{
		assert(it1 != end1 || it2 != end2);
		if (it2 == end2)
			++it1;
		else if (it1 == end1)
			++it2;
		else
		{
			if (*it1 <= *it2)
				++it1;
			else
				++it2;
		}
		return *this;
	}

	/// Tests whether this iterator is equal to another iterator.
	/// @complexity Constant.
	bool operator == (const merged_iterator &it) const
	{
		return it1 == it.it1 && it2 == it.it2 && end1 == it.end1 && end2 == it.end2;
	}

	/// Tests whether this iterator is not equal to another iterator.
	/// @complexity Constant.
	bool operator != (const merged_iterator &it) const
	{
		return ! operator == (it);
	}
};

/// Merges two ordered sequences.
template <class Iter>
sequence<merged_iterator<Iter>> merge(const sequence<Iter> &a, const sequence<Iter> &b)
{
	return sequence<merged_iterator<Iter>>(
		merged_iterator<Iter>(a.begin(), a.end(), b.begin(), b.end()),
		merged_iterator<Iter>(a.end(), a.end(), b.end(), b.end()));
}

/// Iterator adaptor that returns unique values along with their frequencies
/// from an underlying iterator.
/// The underlying iterator can be a (single pass) input iterator. The unique
/// values are copied.
/// @ingroup Sequence
template <class InIt>
class distinct_iterator
{
public:

	/// Marks this iterator as a forward iterator.
	typedef std::forward_iterator_tag iterator_category;

	/// Type of the values enumerated.
	typedef typename InIt::value_type value_type;
	
	/// Type of iterator differences.
	typedef std::ptrdiff_t difference_type;

	/// No pointer to value is supported.
	typedef void pointer;

	/// Type of reference to the values enumerated.
	typedef const value_type& reference;

	/// Type of the frequency of a value.
	typedef int count_type;

private:

	InIt it, end;
	value_type value;
	count_type count;

	void move_next()
	{
		if (it == end)
		{
			value = value_type();
			count = 0;
		}
		else
		{
			value = *it;
			for (count = 1; ++it != end && *it == value; ++count);
		}
	}

public:

	/// Creates the iterator.
	/// @complexity <code>O(n)</code> where @c n is the number of occurrence
	///      of the next value in the underlying iterator.
	distinct_iterator(const InIt &_begin, const InIt &_end)
		: it(_begin), end(_end)
	{
		move_next();
	}

	/// Returns the current value.
	reference operator * () { return value; }

	/// Returns the number of occurrence of the current value in the underlying
	/// iterator.
	count_type frequency() const { return count; }

	/// Advances the iterator.
	/// @complexity <code>O(n)</code> where @c n is the number of occurrence
	///      of the next value in the underlying iterator.
	distinct_iterator& operator ++ ()
	{
		move_next();
		return *this;
	}

	/// Tests whether this iterator is equal to another iterator.
	/// @complexity Constant.
	bool operator == (const distinct_iterator &i) const
	{
		return value == i.value && count == i.count && it == i.it && end == i.end;
	}

	/// Tests whether this iterator is not equal to another iterator.
	/// @complexity Constant.
	bool operator != (const distinct_iterator &i) const
	{
		return ! operator == (i);
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
