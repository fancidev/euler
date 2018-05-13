/**
 * @defgroup Matrix Matrix Operations
 *
 * Fixed-size matrices and related operations.
 */

#ifndef EULER_MATRIX_HPP
#define EULER_MATRIX_HPP

#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#include "imath.hpp"

namespace euler {

/**
 * Represents a fixed-size matrix.
 *
 * The dimension of the matrix is fixed at compile-time, allowing compile-time
 * type checks for matrix operations and optimization for small matrices.
 *
 * @tparam T Element type. See the Remarks section for its requirements.
 *
 * @tparam M Number of rows in the matrix; must be positive.
 *
 * @tparam N Number of columns in the matrix; must be positive.
 *
 * @remarks To ensure that common mathematical operations are well-behaved,
 *    the element type @c T should satisfy the Field requirement.
 *    See https://en.wikipedia.org/wiki/Field_(mathematics)#Classic_definition
 *    for details on the requirement. In addition, @c T must be default
 *    constructible where the default value is 0. And the multiplicative
 *    identity must be <code>T(1)</code>.
 *
 * @ingroup Matrix
 */
template <class T, size_t M, size_t N>
class matrix
{
  std::array<std::array<T, N>, M> x;

public:

  static_assert(M > 0, "Matrix must have at least one row.");
  static_assert(N > 0, "Matrix must have at least one column.");

  /**
   * Constructs a matrix with all elements initialized to their default value.
   */
  matrix() : x()
  {
  }

  /**
   * Constructs a matrix initialized with provided elements.
   */
  matrix(std::initializer_list<std::initializer_list<T>> rows) : x()
  {
    if (rows.size() > M)
    {
      throw std::invalid_argument("Too many rows in initializer list.");
    }
    size_t i = 0;
    for (const std::initializer_list<T> &row: rows)
    {
      if (row.size() > N)
      {
        throw std::invalid_argument("Too many columns in initializer list.");
      }
      std::copy(row.begin(), row.end(), x[i].begin());
      i++;
    }
  }

  /**
   * Constructs a vector with the provided elements.
   */
  matrix(std::initializer_list<T> elements) : x()
  {
    static_assert(M == 1 || N == 1, "");
    if (elements.size() > M * N)
    {
      throw std::invalid_argument("Too many elements in initializer list.");
    }
    size_t i = 0, j = 0;
    for (const T &element: elements)
    {
      x[i][j] = element;
      if (M == 1)
      {
        j++;
      }
      else
      {
        i++;
      }
    }
  }

  //
  // Element access
  //

  /**
   * Gets the element at the given index of a vector.
   * @remarks This function is only defined for vector.
   */
  T& operator()(size_t k)
  {
    static_assert(
        M == 1 || N == 1,
        "This method requires matrix with a single row or column.");
    return (M == 1)? operator()(0, k) : operator()(k, 0);
  }

  /**
   * Gets the element at the given index of a vector.
   * @remarks This function is only defined for vector.
   */
  const T& operator()(size_t k) const
  {
    static_assert(
        M == 1 || N == 1,
        "This method requires matrix with a single row or column.");
    return (M == 1)? operator()(0, k) : operator()(k, 0);
  }

  /**
   * Gets the element at the given row and column.
   */
  T& operator()(size_t i, size_t j)
  {
    assert(i >= 0 && i < M);
    assert(j >= 0 && j < N);
    return x[i][j];
  }

  /**
   * Gets the element at the given row and column.
   */
  const T& operator()(size_t i, size_t j) const
  {
    assert(i >= 0 && i < M);
    assert(j >= 0 && j < N);
    return x[i][j];
  }

public: // assignment

  /**
   * Sets all elements to the given value.
   */
  matrix& operator=(const T &value)
  {
    for (size_t i = 0; i < M; i++)
    {
      for (size_t j = 0; j < N; j++)
      {
        x[i][j] = value;
      }
    }
    return *this;
  }

public: // static methods

#if 0
  /// <summary>
  ///   Creates a diagonal matrix.
  /// </summary>
  /// <param name="value">
  ///   Value on the diagonal.
  /// </param>
  /// <returns>
  ///   Matrix <c>A</c> such that <c>A(i, j) == value</c> for <c>i == j</c>
  ///   and <c>A(i, j) == T(0)</c> for <c>i != j</c>.
  /// </returns>
  /// <timecomplexity>
  ///   <c>O(N^2)</c>.
  /// </timecomplexity>
  /// <remarks>
  ///   This function is only supported for square matrix.
  /// </remarks>
#endif
  /**
   * Creates a diagonal matrix.
   *
   * @param value Value on the diagonal.
   *
   * @returns Matrix <c>A</c> such that <c>A(i, j) == value</c> for
   *    <c>i == j</c> and <c>A(i, j) == T(0)</c> for <c>i != j</c>.
   *
   * @complexity <c>O(N<sup>2</sup>)</c>.
   *
   * @remarks This function is only supported for square matrix.
   */
  static matrix diagonal(const T &value)
  {
    static_assert(M == N, "Square matrix required.");
    matrix A;
    for (size_t i = 0; i < N; i++)
    {
      A(i, i) = value;
    }
    return A;
  }

  /**
   * Creates an identity matrix.
   *
   * @returns Matrix <c>A</c> such that <c>A(i, j) == T(1)</c> for
   *    <c>i == j</c> and <c>A(i, j) == T(0)</c> for <c>i != j</c>.
   *
   * @remarks This function is only supported for square matrix.
   */
  static matrix identity()
  {
    static_assert(M == N, "Square matrix required.");
    return diagonal(T(1));
  }
};

/**
 * @addtogroup Matrix
 * @{
 */

/**
 * Returns <c>true</c> if all elements in the matrix are <c>true</c>.
 */
template <size_t M, size_t N>
bool all_of(const matrix<bool,M,N> &a)
{
  for (size_t i = 0; i < M; i++)
  {
    for (size_t j = 0; j < N; j++)
    {
      if (!a(i, j))
      {
        return false;
      }
    }
  }
  return true;
}

namespace details {

// Matrix-matrix binary operation.
template <class Op, class T, size_t M, size_t N>
matrix<std::result_of_t<Op(const T&, const T&)>, M, N>
apply(Op f, const matrix<T,M,N> &a, const matrix<T,M,N> &b)
{
  matrix<std::result_of_t<Op(const T&, const T&)>, M, N> c;
  for (size_t i = 0; i < M; i++)
  {
    for (size_t j = 0; j < N; j++)
    {
      c(i, j) = f(a(i, j), b(i, j));
    }
  }
  return c;
}

// Matrix-scalar binary operation.
template <class Op, class T, size_t M, size_t N>
matrix<std::result_of_t<Op(const T&, const T&)>, M, N>
apply(Op f, const matrix<T,M,N> &a, const T &b)
{
  matrix<std::result_of_t<Op(const T&, const T&)>, M, N> c;
  for (size_t i = 0; i < M; i++)
  {
    for (size_t j = 0; j < N; j++)
    {
      c(i, j) = f(a(i, j), b);
    }
  }
  return c;
}

// Scalar-matrix binary operation.
template <class Op, class T, size_t M, size_t N>
matrix<std::result_of_t<Op(const T&, const T&)>, M, N>
apply(Op f, const T &a, const matrix<T,M,N> &b)
{
  matrix<std::result_of_t<Op(const T&, const T&)>, M, N> c;
  for (size_t i = 0; i < M; i++)
  {
    for (size_t j = 0; j < N; j++)
    {
      c(i, j) = f(a, b(i, j));
    }
  }
  return c;
}

} // namespace details

/**
 * @name Comparison Operators
 * @{
 */

/**
 * Matrix-matrix element-wise equality comparison.
 */
template <class T, size_t M, size_t N>
matrix<bool,M,N> operator==(const matrix<T,M,N> &a, const matrix<T,M,N> &b)
{
  return details::apply(std::equal_to<T>(), a, b);
}

/** @} */

/**
 * @name Arithmetic Operators
 * @{
 */

/**
 * Matrix-matrix addition.
 */
template <class T, size_t M, size_t N>
matrix<T,M,N> operator+(const matrix<T,M,N> &a, const matrix<T,M,N> &b)
{
  return details::apply(std::plus<T>(), a, b);
}

/**
 * Matrix-scalar addition.
 */
template <class T, size_t M, size_t N>
matrix<T,M,N> operator+(const matrix<T,M,N> &a, const T &b)
{
  return details::apply(std::plus<T>(), a, b);
}

/**
 * Scalar-matrix addition.
 */
template <class T, size_t M, size_t N>
matrix<T,M,N> operator+(const T &a, const matrix<T,M,N> &b)
{
  return details::apply(std::plus<T>(), a, b);
}

/**
 * Matrix-matrix subtraction.
 */
template <class T, size_t M, size_t N>
matrix<T,M,N> operator-(const matrix<T,M,N> &a, const matrix<T,M,N> &b)
{
  return details::apply(std::minus<T>(), a, b);
}

/**
 * Matrix-scalar subtraction.
 */
template <class T, size_t M, size_t N>
matrix<T,M,N> operator-(const matrix<T,M,N> &a, const T &b)
{
  return details::apply(std::minus<T>(), a, b);
}

/**
 * Scalar-matrix subtraction.
 */
template <class T, size_t M, size_t N>
matrix<T,M,N> operator-(const T &a, const matrix<T,M,N> &b)
{
  return details::apply(std::minus<T>(), a, b);
}

/**
 * Matrix-matrix multiplication.
 *
 * @timecomplexity <code>O(MNP)</code>.
 *
 * @spacecomplexity <code>O(MP)</code>.
 */
template <class T, size_t M, size_t N, size_t P>
matrix<T,M,P> operator*(const matrix<T,M,N> &a, const matrix<T,N,P> &b)
{
  matrix<T,M,P> c;
  for (size_t i = 0; i < M; i++)
  {
    for (size_t j = 0; j < P; j++)
    {
      T x(0);
      for (size_t k = 0; k < N; k++)
      {
        x += a(i, k) * b(k, j);
      }
      c(i, j) = x;
    }
  }
  return c;
}

/**
 * Matrix-scalar multiplication.
 */
template <class T, size_t M, size_t N>
matrix<T,M,N> operator*(const matrix<T,M,N> &a, const T &b)
{
  return details::apply(std::multiplies<T>(), a, b);
}

/**
 * Scalar-matrix multiplication.
 */
template <class T, size_t M, size_t N>
matrix<T,M,N> operator*(const T &a, const matrix<T,M,N> &b)
{
  return details::apply(std::multiplies<T>(), a, b);
}

/**
 * Matrix-matrix multiplication and assignment.
 */
template <class T, size_t M, size_t N>
matrix<T,M,N>& operator*=(const matrix<T,M,N> &a, const matrix<T,N,N> &b)
{
  a = a * b;
  return a;
}

/**
 * Matrix-scalar multiplication and assignment.
 *
 * @returns @c a after updating each element using <code>a(i, j) *= b</code>.
 */
template <class T, size_t M, size_t N>
matrix<T,M,N>& operator*=(const matrix<T,M,N> &a, const T &b)
{
  for (size_t i = 0; i < M; i++)
  {
    for (size_t j = 0; j < N; j++)
    {
      a(i, j) *= b;
    }
  }
  return a;
}

/**
 * Matrix-scalar division and assignment.
 *
 * @returns @c a after updating each element using <code>a(i, j) /= b</code>.
 */
template <class T, size_t M, size_t N>
matrix<T,M,N>& operator/=(matrix<T,M,N> &a, const T &b)
{
  for (size_t i = 0; i < M; i++)
  {
    for (size_t j = 0; j < N; j++)
    {
      a(i, j) /= b;
    }
  }
  return a;
}

/**
 * Matrix exponentiation.
 *
 * @param a Square matrix.
 *
 * @param k Exponent.
 *
 * @returns The N-by-N identity matrix multiplied by @c a for @c k times.
 *
 * @timecomplexity <code>O(N^3*log(k))</code>.
 *
 * @spacecomplexity <code>O(N^2)</code>.
 */
template <class T, size_t N, typename TExponent>
matrix<T,N,N> operator^(const matrix<T,N,N> &a, TExponent k)
{
  return ipow(a, k, std::multiplies<matrix<T,N,N>>(),
      matrix<T,N,N>::identity());
}

/** @} */

#if 0
/**
 * LUP decomposition.
 *
 * Given a square, non-singular matrix <code>A</code>, this procedure finds
 * a unit lower triangular matrix <code>L</code>, an upper triangular matrix
 * <code>U</code>, and a permutation matrix <code>P</code>, such that
 * <code>L*U = P*A</code>.
 *
 * @param A On input, contains a non-singular matrix to be decomposed.
 *      On return, if <code>A</code> is not singular, the elements
 *      below the diagonal store the unit lower triangular matrix
 *      <code>L</code>; the elements on and above the diagonal store
 *      the upper triangular matrix <code>U</code>. If @c A is singular,
 *      the value is undefined.
 *
 * @param perm If @c A is not singular, stores the permutation on return.
 *      If @c A is singular, the result is undefined.
 *
 * @returns The determinant of the permutation matrix (which is either
 *      <code>1</code> or <code>-1</code>) if successful; otherwise zero.
 *
 * @timecomplexity <code>O(N^3)</code>.
 *
 * @spacecomplexity Constant.
 *
 */
template <class T, size_t N>
int lup_decompose(matrix<T,N,N> &A, size_t perm[N])
{
  using value_type = T;

  int d = 1; // determinant of the permutation matrix

  // Initialize permutation to identity.
  for (size_t i = 0; i < N; i++)
  {
    perm[i] = i;
  }

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
    {
      return 0;
    }

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
      {
        A[i][j] -= t*A[k][j];
      }
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
 */
template <class MLU, class TPerm, class MB>
MB& lup_solve(const MLU &LU, const TPerm perm[], MB &B)
{
  const size_t M = extent<0>(LU), N = extent<1>(LU), K = extent<1>(B);
  assert(M == N);
  assert(extent<0>(B) == N);

  // Reorder the rows in B according to the permutation matrix.
  //euler::reorder(matrix_row_iterator<MB>(B,0), matrix_row_iterator<MB>(B,N), perm);
  reorder(B, perm, N);

  // Solve each column in B in turn.
  for (size_t k = 0; k < K; k++)
  {
    // Use forward substitution to solve Ly=b.
    // Skip leading zeros. i0 is the first non-zero row.
    size_t i0 = 0;
    while (i0 < N && B[i0][k] == 0)
    {
      ++i0;
    }
    for (size_t i = i0 + 1; i < N; ++i)
    {
      for (size_t j = i0; j < i; ++j)
      {
        B[i][k] -= LU[i][j]*B[j][k];
      }
    }

    // Use backward substitution to solve Ux=y.
    for (size_t i = N; i > 0; )
    {
      --i;
      for (size_t j = i + 1; j < N; ++j)
      {
        B[i][k] -= LU[i][j]*B[j][k];
      }
      B[i][k] /= LU[i][i];
    }
  }
  return B;
}
#endif

#if 0
/**
 * Matrix inversion.
 */
template <class T, size_t N>
matrix<T,N,N> inv(const matrix<T,N,N> &a)
{
  // LUP decompose the matrix.
  size_t perm[N];
  matrix<T,N,N> LU = A;
  lup_decompose(LU, perm);

  // Creates an indentity matrix to store the result.
  return lup_solve(LU, perm, SQUARE_MATRIX_TYPE::identity());
}
#endif

/**
 * 1-by-1 matrix inversion.
 *
 * @tparam T Type that satisfies Field requirement.
 *
 * @param a 1-by-1 matrix.
 *
 * @returns 1-by-1 matrix @c b such that <code>a * b == b * a == 1</code> if
 *    @c T supports exact arithmetic.
 *
 * @remarks If the matrix @c a is not invertible, the behavior is undefined.
 */
template <class T>
matrix<T,1,1> inv(const matrix<T,1,1> &a)
{
  const T &one = T(1);
  matrix<T,1,1> b;
  b(0, 0) = one / a(0, 0);
  return b;
}

template <class T>
matrix<T,2,2> inv(const matrix<T,2,2> &a)
{
  const T &z = a(0, 0) * a(1, 1) - a(0, 1) * a(1, 0);
  matrix<T,2,2> b;
  b(0, 0) =  a(1, 1) / z;
  b(0, 1) = -a(0, 1) / z;
  b(1, 0) = -a(1, 0) / z;
  b(1, 1) =  a(0, 0) / z;
  return b;
}

/** @} */

} // namespace euler

#endif // EULER_MATRIX_HPP
