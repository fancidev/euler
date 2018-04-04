/**
 * @defgroup LinearAlgebra Linear Algebra
 * 
 * Linear algebra module.
 *
 * This module provides several routines to solve basic problems in linear
 * algebra, such as solving linear system of equations. The library is 
 * designed to be simple and flexible for small (or tiny) scale problems.
 * If large scale problems is handled or best performance and robustness 
 * is desired, a professional linear algebra library should be used instead.
 *
 * In order to work with a wide range of data structures with minimum change
 * in client code, the routines in this module operate on matrix accessors,
 * which are proxy objects that encapsulate an underlying storage to be 
 * accessed like a matrix.
 * 
 * @ingroup Library
 */

#ifndef EULER_LINEAR_HPP
#define EULER_LINEAR_HPP

#include <ostream>
#include <cassert>
#include <functional>
#include "permutation.hpp"
#include "matrix_traits.hpp"

namespace euler {

/**
 * Outputs a matrix to a stream. 
 * The rows are separated by a new line. 
 * The columns are separated by a space.
 * @ingroup LinearAlgebra
 */
template <class M1>
typename std::enable_if<is_matrix<M1>::value, std::ostream&>::type
operator << (std::ostream &os, const M1 &x)
{
	const size_t M = euler::size<1>(x), N = euler::size<2>(x);
	std::streamsize w = os.width();
	for (size_t i = 0; i < M; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			if (j > 0)
				os << ' ';
			os.width(w);
			os << x[i][j];
		}
		os << std::endl;
	}
	os.width(0);
	return os;
}

/**
 * Returns the sum of elements in a matrix.
 * @timecomplexity <code>O(MN)</code>.
 * @spacecomplexity Constant.
 * @ingroup Linear
 */
template <class M1>
typename std::remove_cv<typename matrix_traits<M1>::value_type>::type
msum(const M1 &A)
{
	// todo: check how auto determines the type
	// todo: we could make a reduce function to adapt to sum, max, min, etc.
	const size_t M = size<1>(A), N = size<2>(A);
	typename std::remove_cv<typename matrix_traits<M1>::value_type>::type s(0);
	for (size_t i = 0; i < M; ++i)
		for (size_t j = 0; j < N; ++j)
			s += A[i][j];
	return s;
}

/**
 * Returns the inner product of two matrices of the same size.
 * The inner product is defined as the sum of the element-wise
 * product of the two matrices.
 * @timecomplexity <code>O(MN)</code>.
 * @spacecomplexity Constant.
 * @ingroup Linear
 */
template <class T, class M1, class M2>
typename std::enable_if<is_matrix<M1>::value && is_matrix<M2>::value, T>::type
inner_product(const M1 &A, const M2 &B, T y = 0)
{
	const size_t M = size<1>(A), N = size<2>(A);
	assert(size<1>(B) == M && size<2>(B) == N);

	for (size_t i = 0; i < M; ++i)
		for (size_t j = 0; j < N; ++j)
			y += A[i][j] * B[i][j];

	return y;
}

/**
 * Applies a binary operation elementwise to two matrices of the same size,
 * and stores the result in another matrix.
 * Any pair of matrices must not overlap unless they are the same.
 * @timecomplexity <code>O(MN)</code>.
 * @spacecomplexity Constant.
 * @ingroup Linear
 */
template <class M1, class M2, class M3, class Func>
M3& mapply2(const M1 &A, const M2 &B, M3 &C, Func f)
{
	const size_t M = size<1>(C), N = size<2>(C);
	assert(size<1>(A) == M && size<2>(A) == N);
	assert(size<1>(B) == M && size<2>(B) == N);

	for (size_t i = 0; i < M; i++)
		for (size_t j = 0; j < N; j++)
			C[i][j] = f(A[i][j], B[i][j]);
	return C;
}

/**
 * Adds two matrices and stores the result in another matrix.
 * Any pair of matrices must not overlap unless they are the same.
 * @timecomplexity <code>O(MN)</code>.
 * @spacecomplexity Constant.
 * @ingroup Linear
 */
template <class M1, class M2, class M3>
M3& madd(const M1 &A, const M2 &B, M3 &C)
{
	return mapply2(A, B, C, std::plus<matrix_traits<M3>::value_type>());
}

/**
 * Subtracts two matrices and stores the result in another matrix.
 * Any pair of matrices must not overlap unless they are the same.
 * @timecomplexity <code>O(MN)</code>.
 * @spacecomplexity Constant.
 * @ingroup Linear
 */
template <class M1, class M2, class M3>
M3& msub(const M1 &A, const M2 &B, M3 &C)
{
	return mapply2(A, B, C, std::minus<matrix_traits<M3>::value_type>());
}

/**
 * Multiplies two matrices and stores the result in another matrix.
 * The matrices must not overlap.
 * @timecomplexity <code>O(MNP)</code>.
 * @spacecomplexity Constant.
 * @ingroup Linear
 */
template <class M1, class M2, class M3>
M3& mmul(const M1 &A, const M2 &B, M3 &C)
{
	const size_t M = size<1>(A), N = size<2>(A), P = size<2>(B);
	assert(size<1>(B) == N);
	assert(size<1>(C) == M && size<2>(C) == P);
	
	for (size_t i = 0; i < M; i++)
	{
		for (size_t j = 0; j < P; j++)
		{
			C[i][j] = 0;
			for (size_t k = 0; k < N; k++)
				C[i][j] += A[i][k] * B[k][j];
		}
	}
	return C;
}

/**
 * LUP decomposition. Given a square, non-singular matrix <code>A</code>, 
 * this procedure finds a unit lower triangular matrix <code>L</code>, 
 * an upper triangular matrix <code>U</code>, and a permutation matrix
 * <code>P</code>, such that <code>L*U = P*A</code>.
 *
 * @param A On input, contains a non-singular matrix to be decomposed.
 *      On return, if <code>A</code> is not singular, the elements 
 *      below the diagonal store the unit lower triangular matrix 
 *      <code>L</code>; the elements on and above the diagonal store 
 *      the upper triangular matrix <code>U</code>. If @c A is singular,
 *      the value is undefined.
 * @param perm If @c A is not singular, stores the permutation on return.
 *      If @c A is singular, the result is undefined.
 * @returns The determinant of the permutation matrix (which is either
 *      <code>1</code> or <code>-1</code>) if successful; otherwise zero.
 *
 * @timecomplexity <code>O(N^3)</code>.
 * @spacecomplexity Constant.
 * @ingroup Linear
 */
template <class MA, class TPerm>
int lup_decompose(MA &A, TPerm perm[])
{
	typedef typename matrix_traits<MA>::value_type value_type;

	const size_t M = size<1>(A), N = size<2>(A);
	assert(M == N);

	int d = 1; // determinant of the permutation matrix

	// Initialize the permutation to identity.
	for (size_t i = 0; i < N; i++)
		perm[i] = TPerm(i);

	// Perform Gaussian elimination on the k-th row of A.
	for (size_t k = 0; k < N; k++)
	{
		// Search for largest element in the k-th column as pivot.
		size_t pivot = k;
		value_type big = std::abs(A[k][k]);
		for (size_t i = k + 1; i < N; i++)
		{
			value_type t = std::abs(A[i][k]);
			if (t > big)
			{
				big = t;
				pivot = i;
			}
		}
		if (big == value_type(0))
			return 0;

		// Swap rows (of A and L) if the pivot chosen is not in the
		// current row.
		if (pivot != k)
		{
			for (size_t j = 0; j < N; j++)
			{
				std::swap(A[pivot][j], A[k][j]);
			}
			std::swap(perm[pivot], perm[k]);
			d = -d;
		}

		// Eliminate the k-th column from the remaining equations.
		for (size_t i = k + 1; i < N; i++)
		{
			A[i][k] /= A[k][k];
			value_type t = A[i][k];
			for (size_t j = k + 1; j < N; j++)
				A[i][j] -= t*A[k][j];
		}
	}
	return d;
}

/**
 * Solves a linear system given the LUP decomposition of the coefficient
 * matrix.
 *
 * @param LU The elements below the diagonal store the unit lower 
 *      triangular matrix <code>L</code>; the elements on and above
 *      the diagonal store the upper triangular matrix <code>U</code>.
 * @param perm The row permutation.
 * @param B On input, stores the coefficients to solve. On output, stores
 *      the solved variables.
 *
 * @timecomplexity <code>O(KN^2)</code>, where @c K is the number of 
 *      columns in <code>B</code>.
 * @spacecomplexity Constant.
 * @ingroup Linear
 */
template <class MLU, class TPerm, class MB>
MB& lup_solve(const MLU &LU, const TPerm perm[], MB &B)
{
	const size_t M = size<1>(LU), N = size<2>(LU), K = size<2>(B);
	assert(M == N);
	assert(size<1>(B) == N);

	// Reorder the rows in B according to the permutation matrix.
	//euler::reorder(matrix_row_iterator<MB>(B,0), matrix_row_iterator<MB>(B,N), perm);
	reorder(B, perm, N);

	// Solve each column in B in turn.
	for (size_t k = 0; k < K; k++)
	{
		// Use forward substitution to solve Ly=b.
		// Skip leading zeros. i0 is the first non-zero row.
		size_t i0 = 0;
		for (; i0 < N && B[i0][k] == 0; ++i0);
		for (size_t i = i0 + 1; i < N; ++i)
		{
			for (size_t j = i0; j < i; ++j)
				B[i][k] -= LU[i][j]*B[j][k];
		}

		// Use backward substitution to solve Ux=y.
		for (size_t i = N; i > 0; )
		{
			--i;
			for (size_t j = i + 1; j < N; ++j)
				B[i][k] -= LU[i][j]*B[j][k];
			B[i][k] /= LU[i][i];
		}
	}
	return B;
}

} // namespace euler

#endif // EULER_LINEAR_HPP
