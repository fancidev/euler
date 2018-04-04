/**
 * @defgroup Fibonacci Fibonacci Sequence
 * Routines for retrieving elements from a Fibonacci sequence.
 *
 * The Fibonacci sequence is defined by the recurrence relation
 * \f[
 * F_{n} = F_{n-1}+F_{n-2} ,
 * \f]
 * starting from \f$F_0 = 0, F_1 = 1\f$. This recurrence relation can be
 * written in the more tractable matrix form:
 * \f[
 * \begin{pmatrix} F_{n+1} \\ F_{n} \end{pmatrix} =
 * \begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix} 
 * \begin{pmatrix} F_{n} \\ F_{n-1} \end{pmatrix} .
 * \f]
 * If we apply this transformation repeatedly, we finally get
 * \f[
 * \begin{pmatrix} F_{n+1} \\ F_{n} \end{pmatrix} =
 * \begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix}^n 
 * \begin{pmatrix} F_{1} \\ F_{0} \end{pmatrix} .
 * \f]
 * This formula allows us to compute exact values of 
 * <code>F<sub>n</sub></code> quickly because we can use the binary
 * exponentiation method to compute the matrix exponentiation.
 *
 * Note also that the formula is not restricted to particular values
 * of <code>F<sub>0</sub></code> and <code>F<sub>1</sub></code>.
 * For example, if we let \f$F_0 = 2, F_1 = 1\f$, then the sequence
 * becomes the Lucas numbers.
 *
 * @ingroup Library
 */

#ifndef EULER_FIBONACCI_H
#define EULER_FIBONACCI_H

#include "imath.hpp"
#include "matrix.hpp"

namespace euler {

#if 0
// Table of Fibonacci numbers.
//
// Note: this class doesn't really work well enough. If the indices of 
// Fibonacci numbers to retrieve are close, we can use an iterative 
// formula that is very fast. If the indices to retrieve are far apart,
// then the cache won't work. So this class only works for scenario
// where the indices to retrieve are not too close nor too far, which
// is hard to determine and justify.
template <typename T, typename TArg>
class fibonacci_table
{
	std::map<TArg,T> cache;

public:

	fibonacci_table(const T &one)
	{
		cache.insert(std::pair<TArg,T>(0, one - one));
		cache.insert(std::pair<TArg,T>(1, one));
		cache.insert(std::pair<TArg,T>(2, one));
	}

	T operator () (TArg n) 
	{
		auto it = cache.find(n);
		if (it == cache.end())
		{
			if (n % 2 == 1)
			{
				T a = (*this)((n+1)/2);
				T b = (*this)((n-1)/2);
				it = cache.insert(std::pair<TArg,T>(n, a*a + b*b)).first;
			}
			else
			{
				T a = (*this)(n/2+1);
				T b = (*this)(n/2-1);
				it = cache.insert(std::pair<TArg,T>(n, a*a - b*b)).first;
			}
		}
		return it->second;
	}

	size_t size() const { return cache.size(); }
};
#endif

/**
 * Returns a pair of adjacent Fibonacci numbers at a given index.
 *
 * Given an index <code>n</code>, this function computes and returns the 
 * <code>n</code>-th and <code>(n+1)</code>-th Fibonacci number, using
 * a matrix multiplication algorithm.
 *
 * @timecomplexity <code>O(log n)</code>.
 * @spacecomplexity Constant.
 * @ingroup Fibonacci
 */
template <typename T, typename TArg>
std::pair<T,T> fibonacci2(TArg n, const T &F0, const T &F1)
{
	if (n == 0)
		return std::pair<T,T>(F0, F1);

	matrix<T,2,2> M = { 1,1, 1,0 };
	matrix<T,2,1> F = { F1, F0 };
	F = (M ^ n) * F;
	return std::pair<T,T>(F(1,0), F(0,0));
}

template <typename T, typename TArg>
std::pair<T,T> fibonacci2(TArg n)
{
	return fibonacci2(n, algebra<T>::zero(), algebra<T>::one());
}

/**
 * Returns the Fibonacci number at a given index.
 *
 * @timecomplexity <code>O(log n)</code>.
 * @spacecomplexity Constant.
 * @ingroup Fibonacci
 */
template <typename T, typename TArg>
T fibonacci(TArg n, const T &F0, const T &F1)
{
	return fibonacci2<T,TArg>(n, F0, F1).first;
}

/**
 * Returns the Fibonacci number at a given index.
 *
 * @timecomplexity <code>O(log n)</code>.
 * @spacecomplexity Constant.
 * @ingroup Fibonacci
 */
template <typename T, typename TArg>
T fibonacci(TArg n)
{
	return fibonacci2<T,TArg>(n).first;
}

/** 
 * Iterator for the Fibonacci sequence.
 *
 * @ingroup Fibonacci
 */
template <typename T>
class fibonacci_iterator
{
	T F0; // current term
	T F1; // next term

public:

	/// Iterator category.
	typedef std::forward_iterator_tag iterator_category;

	/// Type of the elements in the Fibonacci sequence.
	typedef T value_type;

	/// Difference type is not supported.
	typedef void difference_type;

	/// Reference type for the element.
	typedef const T &reference;

	/// Pointer type for the element.
	typedef const T *pointer;

	/// Constructs an iterator that points to the first element
	/// of the Fibonacci sequence.
	/// @complexity Constant.
	fibonacci_iterator() : F0(0), F1(1) { }

	/// Constructs an iterator that points to the first element
	/// of a Fibonacci sequence seeded with two given initial
	/// terms.
	/// @complexity Constant.
	fibonacci_iterator(const T &term0, const T &term1) 
		: F0(term0), F1(term1) { }

	/// Returns the current element.
	/// @complexity Constant.
	const T& operator * () const { return F0; }

	/// Advances the iterator to the next term in the sequence.
	/// @complexity Constant.
	fibonacci_iterator& operator ++ ()
	{
		std::swap(F0, F1);
		F1 += F0;
		return *this;
	}

	/// Advances the iterator by a number of terms.
	/// @timecomplexity <code>O(log(n))</code>.
	/// @spacecomplexity Constant.
	template <typename TArg>
	fibonacci_iterator& operator += (const TArg &n)
	{
		std::pair<T,T> p = fibonacci2<T,TArg>(n, F0, F1);
		F0 = p.first;
		F1 = p.second;
		return *this;
	}
	
};

#if 0
void test_fibonacci()
{
	const long long M = 1234567891011LL;
	typedef euler::residue<long long> value_t;

	value_t one(1, M);
	euler::fibonacci_table<value_t, long long> F(one);
	for (int n = 1; n < 100000000; n += 1000000)
	{
		value_t f = F(n);
		value_t f2 = euler::fibonacci(n, one);
		std::cout << "F(" << n << ") = " << f << " (" << f2 << ")" << std::endl;
	}
}
#endif

} // namespace euler

#endif // EULER_FIBONACCI_H
