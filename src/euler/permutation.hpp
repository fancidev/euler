/**
 * @defgroup Permutation Permutation Function
 * @ingroup Library
 *
 * A permutation function is a bijection (one-to-one mapping) of a set onto
 * itself. For example, consider a set containing five elements. For
 * convenience, label each element with an index starting from one. A possible
 * permutation is the following:
 * \f[
 * \begin{pmatrix}
 * 1 & 2 & 3 & 4 & 5 \\
 * 2 & 5 & 4 & 3 & 1
 * \end{pmatrix}
 * \f]
 * where the first row displays the sequence of elements before permutation,
 * and the second row displays the sequence of elements after premutation.
 *
 * A permutation can be inversed. Finding the inverse of a permutation is
 * equivalent to finding the new location of each element after permutation.
 * In the above example, the inverse permutation is
 * \f[
 * \begin{pmatrix}
 * 1 & 2 & 3 & 4 & 5 \\
 * 5 & 1 & 4 & 3 & 2
 * \end{pmatrix}
 * \f]
 * which can be interpreted as element 1 moved to position 5, element 2
 * moved to position 1, etc.
 *
 * Two permutations of the same size can be compounded to form a composite
 * permutation. Let @c P be the composite of permutation
 * <code>P<sub>1</sub></code> and <code>P<sub>2</sub></code>.
 * The effect of applying @c P is equivalent to first applying
 * <code>P<sub>1</sub></code> followed by applying <code>P<sub>2</sub></code>.
 * The notation to write a composite permutation can be confusing,
 * so we omit it here.
 *
 * More details related to permutations can be found at
 *   - http://en.wikipedia.org/wiki/Permutation
 *
 */

#ifndef EULER_PERMUTATION_H
#define EULER_PERMUTATION_H

#include <vector>
#include <cassert>
#include "sequence.hpp"

namespace euler {

template <class IndexIt, class ValueIt>
class indexed_iterator
{
	IndexIt index_it;
	IndexIt index_end;
	ValueIt value_it;

public:

	typedef std::forward_iterator_tag iterator_category;
	typedef typename ValueIt::value_type value_type;
	typedef typename IndexIt::difference_type difference_type;
	typedef typename ValueIt::pointer pointer;
	typedef typename ValueIt::reference reference;

	indexed_iterator(IndexIt current, IndexIt end, ValueIt values)
		: index_it(current), index_end(end), value_it(values) { }

	reference operator * ()	{ return value_it[*index_it]; }

	indexed_iterator& operator ++ () { ++index_it; return *this; }

	bool operator == (const indexed_iterator &it) const
	{
		return index_it == it.index_it && index_end == it.index_end
			&& value_it == it.value_it;
	}

	bool operator != (const indexed_iterator &it) const
	{
		return !operator == (it);
	}
};

/**
 * Represents a permutation function.
 *
 * In the implementation, each element in the original sequence is labeled
 * with an index starting from zero. Then the permuted index sequence is
 * stored in an array to represent the permutation.
 *
 * @ingroup Permutation
 */
template <typename T>
class permutation
{
	// Stores the reordered sequence.
	std::vector<T> perm;

public:

	/// Constructs an empty permutation.
	/// @complexity Constant.
	permutation() { }

	/// Constructs an identity permutation of a given size.
	/// @param size Number of elements in the permuted sequence.
	/// @complexity <code>O(size)</code>.
	permutation(size_t size) : perm(size)
	{
		for (size_t i = 0; i < size; i++)
			perm[i] = (T)i;
	}

	/// Copy-constructs a permutation.
	/// @complexity <code>O(n)</code>.
	permutation(const permutation &p) : perm(p.perm) { }

	/// Move-constructs a permutation.
	/// @complexity Constant.
	permutation(permutation &&p) : perm(std::move(p.perm)) { }

	/**
	 * Constructs the composite of two permutations of the same size.
	 *
	 * The effect of applying the composite permutation is equivalent to
	 * applying the first permutation followed by applying the second
	 * permutation.
	 *
	 * @param p1 The first permutation to apply.
	 * @param p2 The second permutation to apply.
	 * @complexity <code>O(n)</code>.
	 */
	permutation(const permutation &p1, const permutation &p2)
		: perm(p2.perm.size())
	{
		for (size_t i = 0; i < perm.size(); i++)
		{
			perm[i] = p1.perm[p2.perm[i]];
		}
	}

	/// Returns the size of the permutation.
	/// @complexity Constant.
	size_t size() const { return perm.size(); }

	/// Tests whether the permutation is empty.
	/// @complexity Constant.
	bool empty() const { return perm.empty(); }

	/// Returns the element at a given position after permutation.
	size_t operator [] (size_t pos) const { return (size_t)perm[pos]; }

	/// Returns the inverse of the permutation.
	/// @complexity <code>O(size)</code>.
	permutation<T> inverse() const
	{
		permutation<T> p;
		p.perm.resize(perm.size());
		for (size_t i = 0; i < perm.size(); i++)
		{
			p.perm[perm[i]] = (T)i;
		}
		return p;
	}

#if 0
	/// Applies the permutation to a sequence of elements.
	template <class RanIt>
	void apply(RanIt first, RanIt last)
	{
		assert((size_t)(last - first) >= size());
	}
#endif

	typedef typename std::vector<T>::iterator iterator;
	typedef typename std::vector<T>::const_iterator const_iterator;

	iterator begin() { return perm.begin(); }

	iterator end() { return perm.end(); }

	const_iterator begin() const { return perm.begin(); }

	const_iterator end() const { return perm.end(); }

	/// Permutes a given sequence according to this permutation.
	template <class RanIt>
	sequence<indexed_iterator<const_iterator,RanIt>>
	operator() (RanIt value_begin, RanIt /* value_end */) const
	{
		return make_sequence(
			indexed_iterator<const_iterator,RanIt>(begin(), end(), value_begin),
			indexed_iterator<const_iterator,RanIt>(end(), end(), value_begin));
	}

public:

	/**
	 * Computes the permutation that stable-sorts a sequence of elements.
	 *
	 * @param first Begin of the sequence.
	 * @param last End of the sequence.
	 * @param pred Predicate for comparison.
	 * @returns Permutation @c P such that applying @c P to the original
	 *      sequence yields the sorted sequence.
	 *
	 * @timecomplexity <code>O(N*log(N))</code>.
	 * @spacecomplexity <code>O(N)</code>.
	 */
	template <class RanIt, class Func>
	static permutation reorder(RanIt first, RanIt last, Func pred)
	{
		permutation<T> p(last - first);
		std::sort(p.perm.begin(), p.perm.end(), [&](T a, T b) -> bool {
			if (pred(first[a], first[b]))
				return true;
			if (pred(first[b], first[a]))
				return false;
			return a < b;
		});
		return p;
	}

	/**
	 * Computes the permutation that stable-sorts a sequence of elements.
	 *
	 * @param first Begin of the sequence.
	 * @param last End of the sequence.
	 * @returns Permutation @c P such that applying @c P to the original
	 *      sequence yields the sorted sequence.
	 *
	 * @timecomplexity <code>O(N*log(N))</code>.
	 * @spacecomplexity <code>O(N)</code>.
	 */
	template <class RanIt>
	static permutation reorder(RanIt first, RanIt last)
	{
		return reorder(first, last, std::less<typename RanIt::value_type>());
	}
};

/**
 * Returns the lexicographical ordering of two permutations.
 * @param a The first permutation to compare.
 * @param b The second permutation to compare.
 * @returns @c true if @c a is lexicographically less than <code>b</code>;
 *      @c false otherwise.
 * @timecomplexity <code>O(min(size<sub>1</sub>,size<sub>2</sub>))</code>.
 * @spacecomplexity Constant.
 * @ingroup Permutation
 */
template <typename T>
bool operator < (const permutation<T> &a, const permutation<T> &b)
{
	return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
}

/**
 * Outputs a permutation to a stream.
 *
 * This function prints the permuted sequence of elements, which are labeled
 * starting from one before permuted. If there are no more than nine elements,
 * the labels are printed without any separator. If there are more than nine
 * elements, the labels are separated by a comma.
 *
 * @param os The output stream.
 * @param p The permutation to output.
 * @returns The output stream.
 * @timecomplexity <code>O(size)</code>.
 * @spacecomplexity Constant.
 *
 * @ingroup Permutation
 */
template <typename T>
std::ostream& operator << (std::ostream &os, const permutation<T> &p)
{
	size_t n = p.size();
	for (size_t i = 0; i < n; i++)
	{
		if (n > 9 && i > 0)
			os << ',';
		os << (p[i]+1);
	}
	return os;
}

/**
 * Reorders a sequence in-place according to a permutation.
 *
 * @param values The sequence to be reordered.
 * @param perm   Permutation that stores the zero-based index of the elements
 *      after reordering. This permutation is not altered.
 * @param n Number of elements in the sequence.
 * @remarks Both the value iterator and the index iterator must support
 *      random-access.
 * @timecomplexity <code>O(n^2)</code> operations. At most <code>(n-1)</code> 
 *      swaps.
 * @spacecomplexity Constant.
 * @ingroup Permutation
 */
template <class RanItValue, class RanItIndex>
void reorder(RanItValue &values, const RanItIndex &perm, size_t n)
{
	for (size_t i = 0; i < n - 1; ++i) // no need to place the last element
	{
		// Find the new index of the desired element.
		size_t j = perm[i];
		while (j < i)
			j = (size_t)perm[j];

		// Swap the element if not in place.
		if (j > i)
			std::swap(values[i], values[j]);
	}
}

/**
 * Reorders a sequence in-place according to a permutation.
 *
 * @param first Begin of the sequence to be reordered.
 * @param last  End of the sequence to be reordered.
 * @param perm  Permutation that stores the zero-based index of the elements
 *      after reordering. This permutation is not altered.
 * @remarks Both the value iterator and the index iterator must support
 *      random-access.
 * @timecomplexity <code>O(n^2)</code> operations. At most <code>(n-1)</code> 
 *      swaps.
 * @spacecomplexity Constant.
 * @ingroup Permutation
 */
template <class RanItValue, class RanItIndex>
void reorder(RanItValue first, RanItValue last, RanItIndex perm)
{
	size_t n = last - first;
	for (size_t i = 0; i < n - 1; ++i) // no need to place the last element
	{
		// Find the new index of the desired element.
		size_t j = perm[i];
		while (j < i)
			j = (size_t)perm[j];

		// Swap the element if not in place.
		if (j > i)
			std::swap(first[i], first[j]);
	}
}

#if 0
// permutation test
static void test_permute()
{
	char s[] = "abcdefg";
	int index[7]={ 0, 5, 3, 6, 4, 2, 1 };
	std::cout << s << std::endl;
	permute(&s[0], &s[7], index);
	std::cout << s << std::endl;
}
#endif

} // namespace euler

#endif
