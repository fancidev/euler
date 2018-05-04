/**
 * @defgroup matrix_algorithm Matrix Algorithms
 *
 * Algorithms that operate on matrices.
 *
 * This module provides algorithms for
 *
 *   - basic matrix operations such as addition and multiplication;
 *   - element-wise matrix operations including map and reduce;
 *   - solving system of linear equations.
 *
 * The functions in this module operate on <i>matrix accessors</i>, which is
 * the 2D analogy to 1D iterators. This decouples the matrix algorithms from
 * the underlying matrix storage.
 *
 * The module is designed for tiny scale problems.
 *
 * @ingroup Library
 */

#ifndef EULER_MATRIX_ALGORITHM_HPP
#define EULER_MATRIX_ALGORITHM_HPP

#include <ostream>
#include <cassert>
#include <functional>
#include <stdexcept>
#include <sstream>
#include "permutation.hpp"
#include "matrix_traits.hpp"

namespace euler {

/**
 * Outputs a matrix to a stream.
 * The rows are separated by a new line.
 * The columns are separated by a space.
 * @ingroup matrix_algorithm
 */
template <class T>
std::enable_if_t<is_matrix_v<T>, std::ostream&>
operator << (std::ostream &os, const T &x)
{
  const size_t M = euler::extent<0>(x);
  const size_t N = euler::extent<1>(x);
  const std::streamsize w = os.width();
  for (size_t i = 0; i < M; i++)
  {
    for (size_t j = 0; j < N; j++)
    {
      if (j > 0)
      {
        os << ' ';
      }
      os.width(w);
      os << x[i][j];
    }
    os << std::endl;
  }
  os.width(0);
  return os;
}

namespace details {

template <class T>
decltype(auto) mat(const T &a, size_t row, size_t column,
    std::false_type /* is_matrix */, std::false_type /* allow_scalar */)
{
  static_assert(sizeof(T) < 0, "Matrix expected");
}

template <class T>
decltype(auto) mat(const T &a, size_t row, size_t column,
    std::false_type /* is_matrix */, std::true_type /* allow_scalar */)
{
  return a;
}

template <class T>
decltype(auto) mat(const T &a, size_t row, size_t column,
    std::true_type /* is_matrix */, std::false_type /* allow_scalar */)
{
  return a[row][column];
}

template <class T>
decltype(auto) mat(const T &a, size_t row, size_t column,
    std::true_type /* is_matrix */, std::true_type /* allow_scalar */)
{
  return a[row][column];
}

template <bool AllowScalar, class T>
decltype(auto) mat(const T &a, size_t row, size_t column)
{
  return mat(a, row, column, is_matrix<T>(),
      std::conditional_t<AllowScalar, std::true_type, std::false_type>());
}

} // namespace details

template <bool AllowScalar = false, class T>
decltype(auto) mat(const T &a, size_t row, size_t column)
{
  return details::mat<AllowScalar>(a, row, column);
}

namespace details {

/**
 * Encapsulates the functor and arguments of a binary function call where at
 * least one of the arguments is a matrix.
 */
template <class Func, class T1, class T2>
struct result_matrix_info
{
  static_assert(
      is_matrix_v<T1> || is_matrix_v<T2>,
      "At least one of T1 and T2 must be a matrix type.");

  const size_t m;
  const size_t n;
  const Func &f;
  const T1 &a;
  const T2 &b;

  result_matrix_info(size_t m, size_t n, const Func &f, const T1 &a,
      const T2 &b) : m(m), n(n), f(f), a(a), b(b) { }
};

/**
 * Represents a row of @c result_matrix_accessor.
 */
template <class Func, class T1, class T2>
class result_row_accessor
{
  const result_matrix_info<Func, T1, T2> &matrix;
  const size_t row;

public:

  result_row_accessor(const result_matrix_info<Func, T1, T2> &matrix,
      size_t row) : matrix(matrix), row(row) { }

  decltype(auto) operator[](size_t column) const
  {
    assert(column >= 0 && column < matrix.n);
    return matrix.f(mat<true>(matrix.a, row, column),
                    mat<true>(matrix.b, row, column));
  }
};

/**
 * Matrix accessor whose elements are computed on-demand upon access by
 * evaluating a binary function of matrix arguments.
 *
 * @ingroup matrix_algorithm
 */
template <class Func, class T1, class T2>
class result_matrix_accessor
{
  const result_matrix_info<Func, T1, T2> op;

public:

  //using matrix_type = result_matrix;
  using vector_type = result_row_accessor<Func, T1, T2>;
  using scalar_type = std::result_of_t<
    decltype(&vector_type::operator[])(vector_type, size_t)>;

  using value_type = scalar_type;
  using matrix_storage_layout = no_major_storage_tag;

  result_matrix_accessor(size_t m, size_t n, const Func &f, const T1 &a,
      const T2 &b) : op(m, n, f, a, b) { }

  const vector_type operator[](size_t row) const
  {
    assert(row >= 0 && row < op.m);
    return vector_type(op, row);
  }

  template <size_t Dim>
  size_t extent() const
  {
    return (Dim == 0)? op.m : (Dim == 1)? op.n : 0;
  }
};

} // namespace details

#if 0
template <size_t Dim, class Func, class T1, class T2>
size_t extent(const result_matrix_accessor<Func, T1, T2> &a)
{
  return a.template extent<Dim>();
}
#endif

template <class Func, class T1, class T2>
details::result_matrix_accessor<Func, T1, T2>
mapply(const Func &f, const T1 &a, const T2 &b)
{
  const size_t m = common_extent<0>(a, b);
  const size_t n = common_extent<1>(a, b);
  if (m == 0 || n == 0)
  {
    std::ostringstream ss;
    ss << "mapply: expect scalar or matrix arguments of the same size, but "
        << "got arguments of size " << extent<0>(a) << "-by-" << extent<1>(a)
        << " and " << extent<0>(b) << "-by-" << extent<1>(b);
    throw std::invalid_argument(ss.str());
  }
  return details::result_matrix_accessor<Func, T1, T2>(m, n, f, a, b);
}

template <class T1, class T2>
decltype(auto) meq(const T1 &a, const T2 &b)
{
  return mapply(std::equal_to<void>(), a, b);
}

/**
 * Applies a binary function to each element of a matrix and folds the result.
 * (In row-major order)
 * calls accumulator = f(accumulator, x) for each x in matrix
 */
template <class Func, class T, class M>
T mreduce(T accumulator, Func f, const M &matrix)
{
  static_assert(is_matrix_v<M>, "Expects matrix argument.");

  const size_t m = extent<0>(matrix);
  const size_t n = extent<1>(matrix);
  for (size_t i = 0; i < m; i++)
  {
    for (size_t j = 0; j < n; j++)
    {
      accumulator = f(accumulator, matrix[i][j]);
    }
  }
  return accumulator;
}

/**
 * Applies a function to each element of a matrix and folds the result.
 * (In row-major order)
 * calls accumulator = f(accumulator, x) for each x in matrix
 */
template <class Func, class M>
decltype(auto) mreduce(Func f, const M &matrix)
{
  static_assert(is_matrix_v<M>, "Expects matrix argument.");

  const size_t m = extent<0>(matrix);
  const size_t n = extent<1>(matrix);
  if (m == 0 || n == 0)
  {
    throw std::invalid_argument("Matrix must not be empty.");
  }

  using T = typename matrix_traits<M>::value_type;
  T result = matrix[0][0];
  for (size_t i = 0; i < m; i++)
  {
    for (size_t j = 0; j < n; j++)
    {
      if (!(i == 0 && j == 0))
      {
        result = f(result, matrix[i][j]);
      }
    }
  }
  return result;
}

/**
 * Returns @c true if all elements in a matrix evaluate to true.
 *
 * @timecomplexity <code>O(M*N)</code>.
 *
 * @spacecomplexity Constant.
 *
 * @ingroup matrix_algorithm
 */
template <class M>
decltype(auto) mall(const M &matrix)
{
  return mreduce(true, std::logical_and<void>(), matrix);
}

/**
 * Returns the sum of elements in a matrix.
 * @timecomplexity <code>O(MN)</code>.
 * @spacecomplexity Constant.
 * @ingroup Linear
 */
template <class M>
decltype(auto) msum(const M &matrix)
{
  return mreduce(std::plus<void>(), matrix);
}

/**
 * Returns the inner product of two matrices of the same size.
 * The inner product is defined as the sum of the element-wise
 * product of the two matrices.
 * @timecomplexity <code>O(MN)</code>.
 * @spacecomplexity Constant.
 * @ingroup Linear
 */
#if 0
template <class T, class M1, class M2>
std::enable_if_t<is_matrix_v<M1> && is_matrix_v<M2>, T>
inner_product(const M1 &A, const M2 &B, T y = 0)
{
  const size_t M = size<1>(A), N = size<2>(A);
  assert(size<1>(B) == M && size<2>(B) == N);

  for (size_t i = 0; i < M; ++i)
  {
    for (size_t j = 0; j < N; ++j)
    {
      y += A[i][j] * B[i][j];
    }
  }
  return y;
}
#endif

/**
 * Matrix-matrix, matrix-scalar, and scalar-matrix addition.
 *
 * @remarks If matrix @c C overlaps with @c A or @c B, it must overlap exactly
 *    with it, or otherwise the result is undefined.
 *
 * @timecomplexity <code>O(M*N)</code>.
 *
 * @spacecomplexity Constant.
 *
 * @ingroup Linear
 */
template <class T1, class T2>
decltype(auto) madd(const T1 &a, const T2 &b)
{
  static_assert(is_matrix_v<T1> || is_matrix_v<T2>, "Matrix expected");
  return mapply(std::plus<void>(), a, b);
}

/**
 * Matrix-matrix, matrix-scalar, and scalar-matrix subtraction.
 *
 * Subtracts two matrices and stores the result in another matrix.
 * Any pair of matrices must not overlap unless they are the same.
 * @timecomplexity <code>O(MN)</code>.
 * @spacecomplexity Constant.
 * @ingroup Linear
 */
template <class T1, class T2>
decltype(auto) msub(const T1 &a, const T2 &b)
{
  static_assert(is_matrix_v<T1> || is_matrix_v<T2>, "Matrix expected");
  return mapply(std::minus<void>(), a, b);
}

/**
 * Matrix-scalar and scalar-matrix multiplication.
 *
 * The matrices must not overlap.
 * @timecomplexity <code>O(M*N)</code>.
 * @spacecomplexity Constant.
 * @ingroup Linear
 */
template <class T1, class T2,
          class = std::enable_if_t<is_matrix_v<T1> != is_matrix_v<T2>>>
decltype(auto) mmul(const T1 &a, const T2 &b)
{
  static_assert(is_matrix_v<T1> || is_matrix_v<T2>, "Matrix expected");
  return mapply(std::multiplies<void>(), a, b);
}

/**
 * Matrix-matrix multiplication.
 *
 * Multiplies two matrices and stores the result in another matrix.
 * The matrices must not overlap.
 * @timecomplexity <code>O(MNP)</code>.
 * @spacecomplexity Constant.
 * @ingroup Linear
 */
template <class T0, class T1, class T2>
std::enable_if_t<is_matrix_v<T0> && is_matrix_v<T1> && is_matrix_v<T2>, T0&>
mmul(T0 &result, const T1 &a, const T2 &b)
{
  using T = typename matrix_traits<T0>::value_type;

  const size_t M = extent<0>(a), N = extent<1>(a), P = extent<1>(b);
  if (!(extent<0>(b) == N && extent<0>(result) == M && extent<1>(result) == P))
  {
    throw std::invalid_argument("Inconsistent matrix size.");
  }
  for (size_t i = 0; i < M; i++)
  {
    for (size_t j = 0; j < P; j++)
    {
      T s(0);
      for (size_t k = 0; k < N; k++)
      {
        s += a[i][k] * b[k][j];
      }
      result[i][j] = s;
    }
  }
  return result;
}

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
 * @ingroup Linear
 */
template <class MA, class TPerm>
int lup_decompose(MA &A, TPerm perm[])
{
  typedef typename matrix_traits<MA>::value_type value_type;

  const size_t M = extent<0>(A), N = extent<1>(A);
  assert(M == N);

  int d = 1; // determinant of the permutation matrix

  // Initialize the permutation to identity.
  for (size_t i = 0; i < N; i++)
  {
    perm[i] = TPerm(i);
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
 * @ingroup Linear
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

} // namespace euler

#endif // EULER_MATRIX_ALGORITHM_HPP
