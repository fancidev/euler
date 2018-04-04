/**
 * @defgroup Matrix Fixed-Size Matrices
 * Implements fixed-size matrices and related operations.
 * @ingroup Library
 */

#ifndef EULER_MATRIX_HPP
#define EULER_MATRIX_HPP

#include <cassert>
#include <functional>
#include "matrix_traits.hpp"
#include "linear.hpp"

namespace euler {

/**
 * Represents a fixed-size matrix.
 *
 * The type of the elements as well as the dimensions of the matrix is
 * determined at compile time. This allows compile-time checks for matrix
 * operations, and allows optimizations for small matrices.
 *
 * In order to support list initialization, the class is declared as an 
 * aggregate. Therefore, the elements are not initialized upon construction.
 *
 * @ingroup Matrix
 */
template <class T, size_t M, size_t N=M, size_t I0=0, size_t J0=0>
class matrix
{
public:

	typedef T vector_type[N];

	/// @cond DEV
	T x[M][N];
	/// @endcond

	/// Sets all elements to a given value.
	/// @timecomplexity <code>O(MN)</code>.
	/// @spacecomplexity Constant.
	void fill(const T &elem)
	{
		for (size_t i = 0; i < M; i++)
			for (size_t j = 0; j < N; j++)
				x[i][j] = elem;
	}

	/// Returns the element at the given index of a vector.
	/// @complexity Constant.
	T& operator () (size_t k)
	{
		assert(M == 1 || N == 1);
		if (M == 1)
			return operator()(I0,k);
		else
			return operator()(k,J0);
	}

	/// Returns the element at the given index of a vector.
	/// @complexity Constant.
	const T& operator () (size_t k) const
	{
		assert(M == 1 || N == 1);
		if (M == 1)
			return operator()(I0,k);
		else
			return operator()(k,J0);
	}

	/// Returns the element at a given index.
	/// @complexity Constant.
	T& operator () (size_t i, size_t j) 
	{
		assert(i >= I0 && i < I0+M);
		assert(j >= J0 && j < J0+N);
		return x[i-I0][j-J0]; 
	}

	/// Returns the element at a given index.
	/// @complexity Constant.
	const T& operator () (size_t i, size_t j) const 
	{
		assert(i >= I0 && i < I0+M);
		assert(j >= J0 && j < J0+N);
		return x[i-I0][j-J0]; 
	}

	/// Returns the given row in the matrix. 
	/// @param i ZERO-based index of the row.
	vector_type& operator [] (size_t i) 
	{
		assert(i >= 0 && i < M);
		return x[i];
	}

	/// Returns the given row in the matrix. 
	/// @param i ZERO-based index of the row.
	const vector_type& operator [] (size_t i) const
	{
		assert(i >= 0 && i < M);
		return x[i];
	}

public:

	/// Creates a diagonal matrix with the given value on the diagonal.
	/// @param value The value on the diagonal.
	/// @timecomplexity <code>O(N^2)</code>.
	/// @spacecomplexity <code>O(N^2)</code>.
	/// @remarks The matrix must be square.
	static matrix diagonal(const T &value)
	{
		static_assert(M==N, "Diagonal matrix is only allowed for square matrix.");
		matrix A = {0};
		for (size_t k = 0; k < N; k++)
			A(I0+k,J0+k) = value;
		return A;
	}

	/// Creates an identity matrix.
	/// @timecomplexity <code>O(N^2)</code>.
	/// @spacecomplexity <code>O(N^2)</code>.
	/// @remarks The matrix must be square.
	static matrix identity()
	{
		static_assert(M==N, "Identity matrix is only allowed for square matrix.");
		return diagonal(1);
	}
};

// Define two macros to simplify template function writing.
#define MATRIX_ARGS class T, size_t M, size_t N, size_t I0, size_t J0
#define MATRIX_TEMPLATE template <MATRIX_ARGS>
#define MATRIX_TYPE matrix<T,M,N,I0,J0>

#define SQUARE_MATRIX_ARGS class T, size_t N, size_t I0, size_t J0
#define SQUARE_MATRIX_TEMPLATE template <SQUARE_MATRIX_ARGS>
#define SQUARE_MATRIX_TYPE matrix<T,N,N,I0,J0>

/// Matrix accessor traits.
MATRIX_TEMPLATE struct matrix_traits<MATRIX_TYPE>
{
	typedef T value_type;
	typedef T vector_type[N];
};

MATRIX_TEMPLATE struct matrix_traits<const MATRIX_TYPE>
{
	typedef const T value_type;
	typedef const T vector_type[N];
};

template <size_t Dim, MATRIX_ARGS>
size_t size(const MATRIX_TYPE &)
{
	static_assert(Dim == 1 || Dim == 2, "Dim must be 1 or 2.");
	return (Dim == 1)? M : N;
}

// Matrix operations.

/// Divides each element in a matrix by a scalar.
template <MATRIX_ARGS, class T2>
MATRIX_TYPE& operator /= (MATRIX_TYPE &A, const T2 &x)
{
	for (size_t i = I0; i < I0+M; i++)
		for (size_t j = J0; j < J0+N; j++)
			A(i,j) /= x;
	return A;
}

MATRIX_TEMPLATE MATRIX_TYPE operator + (const MATRIX_TYPE &A, const MATRIX_TYPE &B)
{
	return madd(A, B, MATRIX_TYPE());
}

MATRIX_TEMPLATE MATRIX_TYPE operator - (const MATRIX_TYPE &A, const MATRIX_TYPE &B)
{
	return msub(A, B, MATRIX_TYPE());
}

/// Matrix multiplication. The size of the matrix is checked at compile time
/// to be conformant.
/// @timecomplexity <code>O(MNP)</code>.
/// @spacecomplexity <code>O(MP)</code>.
/// @ingroup Matrix
template <class T, size_t M, size_t N, size_t P, size_t I0, size_t J0>
matrix<T,M,P,I0,J0> operator * (
	const matrix<T,M,N,I0,J0> &A, 
	const matrix<T,N,P,I0,J0> &B)
{
	return mmul(A, B, matrix<T,M,P,I0,J0>());
}

/// Matrix exponentiation.
/// @param A The (square) matrix whose exponentiation is computed.
/// @param exponent The exponent.
/// @returns The identity matrix multiplied by @c A for @c exponent times.
/// @timecomplexity <code>O(N^3*log(E))</code>, where @c E is the exponent.
/// @spacecomplexity <code>O(N^2)</code>.
/// @ingroup Matrix
template <SQUARE_MATRIX_ARGS, typename TExponent>
SQUARE_MATRIX_TYPE operator ^ (const SQUARE_MATRIX_TYPE &A, const TExponent &exponent)
{
	return binexp(A, exponent, SQUARE_MATRIX_TYPE::identity(),
		std::multiplies<SQUARE_MATRIX_TYPE>());
}

/// Inverts a square matrix.
SQUARE_MATRIX_TEMPLATE
SQUARE_MATRIX_TYPE inv(const SQUARE_MATRIX_TYPE &A)
{
	// LUP decompose the matrix.
	size_t perm[N];
	SQUARE_MATRIX_TYPE LU = A;
	lup_decompose(LU, perm);

	// Creates an indentity matrix to store the result.
	return lup_solve(LU, perm, SQUARE_MATRIX_TYPE::identity());
}

} // namespace euler

#if 0
static void test_matrix()
{
	using namespace euler;
	std::cout.precision(4);
	std::cout.setf(std::ios::fixed, std::ios::floatfield);
	const int w = 9;

#if 0
	int A[3][3] = { 1 ,2,3,4,5,6,7,8,0 };
	std::cout << as_matrix(A);
	std::cout << "swapped:\n";
	std::swap(A[1],A[2]);
	std::cout << as_matrix(A);
#endif

#if 0
	matrix<double,4,4> A = { {
		{ 2, 0, 2, 0.6 },
		{ 3, 3, 4, -2 },
		{ 5, 5, 4, 2 },
		{ -1, -2, 3.4, -1 }
	} };
	size_t P[4];

	std::cout.precision(4);
	std::cout << "A = \n" << std::setw(7) << as_matrix(A);

	lup_decompose(as_matrix(A), P);
	std::cout << "LU = \n" << std::setw(7) << as_matrix(A);
	std::cout << "P = " << P[0] << ", " << P[1] << ", " << P[2] << "\n";
#endif

#if 1
	matrix<double,4,4> A = 
#else
	double A[4][4] = 
#endif
	{
		 2,  0,  2  ,   0.6,
		 3,  3,  4  ,  -2,
		 5,  5,  4  ,   2,
		-1, -2,  3.4,  -1,
	};
	matrix<double,4,4> B = { 1,2,3,5,  6,2,7,4,  3,8,8,0, 2,8,6,5 };

	std::vector<int> t(10);
	// std::cout << t;

	std::cout << "A = \n" << std::setw(w) << A;
	std::cout << "B = \n" << std::setw(w) << B;

	// Scalar functions: sum, min, max, norm, etc.
	//std::cout << "Sum(A) = " << msum(A) << std::endl;
	assert(msum(A) == 28);

	// Addition, subtraction.
	std::cout << "A + B = \n" << std::setw(w) << (A+B);
	std::cout << "A - B = \n" << std::setw(w) << (A-B);

	// Left- and right-multiplication.
	std::cout << "A * B = \n" << std::setw(w) << (A*B);
	std::cout << "B * A = \n" << std::setw(w) << (B*A);

	// Matrix inversion.
	std::cout << "inv(A) = \n" << std::setw(w) << inv(A);
	std::cout << "inv(B) = \n" << std::setw(w) << inv(B);
	std::cout << "inv(A)*A = \n" << std::setw(w) << inv(A)*A;
	std::cout << "inv(B)*B = \n" << std::setw(w) << inv(B)*B;
}
#endif

#endif // EULER_MATRIX_HPP
