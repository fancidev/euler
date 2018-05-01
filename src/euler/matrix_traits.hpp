/**
 * @defgroup matrix_traits Matrix Traits
 *
 * Defines type traits and functions for matrix-like objects.
 *
 * A <i>matrix-like object</i>, or <i>matrix accessor</i> hereafter, is an
 * object that provides access to a 2D grid of elements using syntax identical
 * to that of accessing a 2D C array.
 *
 * Specifically, let @c a be a matrix accessor and @c i @c j be integers, then
 * <code>a[i][j]</code> refers to the element in row @c i and column @c j of
 * @c a. The subscripts are zero-based.
 *
 * A matrix accessor must contain at least one row and at least one column.
 *
 * By designing algorithms based on matrix accessors, one decouples code that
 * processes elements from code that handles the storage of elements. This is
 * similar to how standard iterators decouple 1D algorithms from 1D containers.
 *
 * This module defines the @c matrix_traits class to provide compile-time type
 * information about a matrix accessor. The module also defines several helper
 * functions to query information about a matrix accessor at runtime. A stock
 * implementation is provided for 2D C arrays.
 *
 * @ingroup Library
 */

#ifndef EULER_MATRIX_TRAITS_HPP
#define EULER_MATRIX_TRAITS_HPP

#include <cstddef>
#include <type_traits>

namespace euler {

/**
 * Indicates that elements in the same row are stored consecutively in a
 * matrix.
 *
 * This tag is used by @c matrix_traits to specify the underlying storage
 * layout of elements in a matrix. A matrix accessor marked with this tag
 * indicates to an algorithm implementation that traversing by row is more
 * efficient than traversing by column.
 *
 * Arrays in C are stored in row-major order.
 *
 * @ingroup matrix_traits
 */
struct row_major_storage_tag { };

/**
 * Indicates that elements in the same column are stored consecutively in a
 * matrix.
 *
 * This tag is used by @c matrix_traits to specify the underlying storage
 * layout of elements in a matrix. A matrix accessor marked with this tag
 * indicates to an algorithm implementation that traversing by column is
 * more efficient than traversing by row.
 *
 * Arrays in Fortran and Matlab are stored in column-major order.
 *
 * @ingroup matrix_traits
 */
struct column_major_storage_tag { };

/**
 * Indicates that elements in the matrix are stored in no particular order.
 *
 * This tag is used by @c matrix_traits to specify the underlying storage
 * layout of elements in a matrix. A matrix accessor marked with this tag
 * indicates to an algorithm implementation that there is no performance
 * difference between traversing the matrix by row or by column.
 *
 * For example, this storage tag is suitable for a matrix accessor which
 * computes elements on-demand upon access by a function that takes matrix
 * arguments of mixed storage layouts.
 *
 * @ingroup matrix_traits
 */
struct no_major_storage_tag { };

namespace details {

template <typename...>
using void_t = void;

} // namespace details

namespace details {

//template <class M, typename = void>
//struct matrix_traits_helper : std::false_type { };
template <class M, typename = void>
struct matrix_traits { };

template <class M>
struct matrix_traits<M, void_t<
  typename M::value_type, typename M::matrix_storage_layout>>
{
  /**
   * Underlying storage layout of elements exposed by the matrix accessor.
   *
   * This <code>typedef</code> must be one of @c row_major_storage_tag, @c
   * column_major_storage_tag, or @c no_major_storage_tag. A matrix algorithm
   * implementation may consult this tag to determine the most efficient way
   * to traverse elements in the matrix.
   */
  using matrix_storage_layout = typename M::matrix_storage_layout;

  /**
   * Type of elements exposed by the matrix accessor.
   */
  using value_type = typename M::value_type;

  // ???
  // Type of the row (if the matrix is stored in row-major) or
  // column (if the matrix is stored in column major).
  // typedef void vector_type;

  /**
   * Gets the number of rows or columns in a matrix.
   *
   * @tparam Dim Dimension along which to retrieve the extent: @c 0 indicates
   *    number of rows and @c 1 indicates number of columns.
   *
   * @param a Matrix accessor which these traits prescribe.
   *
   * @returns <code>a.template extent<Dim>()</code>, which must be defined
   *    and must return the number of rows in @c a for <code>Dim == 0</code>,
   *    the number of columns in @c a for <code>Dim == 1</code>, or @c 0 for
   *    any other value of @c Dim.
   */
  template <size_t Dim>
  static auto extent(const M &a) -> decltype(a.template extent<Dim>())
  {
    return a.template extent<Dim>();
  }
};

} // namespace details

/**
 * Defines type traits for a matrix accessor.
 *
 * The default implementation retrieves traits from the type's member
 * <code>typedef</code>s. The trait object is defined only if all the required
 * type definitions are present.
 *
 * @ingroup matrix_traits
 */
template <class M>
struct matrix_traits : public details::matrix_traits<M> { };

/**
 * Checks whether a type satisfies matrix accessor requirements.
 *
 * The default implementation checks whether
 * <code>matrix_traits<T>::value_type</code> is defined.
 *
 * @ingroup matrix_traits
 */
template <class T, class = void>
struct is_matrix : public std::false_type { };

template <class T>
struct is_matrix<T, details::void_t<typename matrix_traits<T>::value_type>>
  : public std::true_type { };

template <class T>
constexpr bool is_matrix_v = is_matrix<T>::value;

/**
 * Gets the number of rows or columns in a matrix.
 *
 * @tparam Dim Dimension along which to retrieve the extent: @c 0 indicates
 *    number of rows and @c 1 indicates number of columns.
 *
 * @tparam T Matrix accessor type for which <code>matrix_traits<T></code> is
 *    suitably defined.
 *
 * @param a Matrix accessor object.
 *
 * @returns <code>matrix_traits<T>::template extent<Dim>(a)</code>, which
 *    returns the number of rows in @c a for <code>Dim == 0</code>, the number
 *    of columns in @c a for <code>Dim == 1</code>, or @c 0 for any other value
 *    of @c Dim.
 *
 * @ingroup matrix_traits
 */
template <size_t Dim, class T>
size_t extent(const T &a)
{
  return matrix_traits<T>::template extent<Dim>(a);
}

namespace details {

template <size_t Dim>
size_t common_extent_tr(size_t m)
{
  return m;
}

template <size_t Dim, class T, class... Ts>
size_t common_extent_tr(size_t m, const T &first, const Ts&... rest)
{
  size_t n = extent<Dim>(first);
  if (m != n && m != 0 && n != 0)
  {
    return 0;
  }
  else
  {
    return common_extent_tr<Dim>(m | n, rest...);
  }
}

} // namespace details

/**
 * Gets the common extent of a list of matrices along a given dimension.
 *
 * @tparam Dim Dimension along which to retrieve the extent: @c 0 indicates
 *    number of rows and @c 1 indicates number of columns.
 *
 * @tparam Ts Matrix or non-matrix (scalar) types.
 *
 * @param ms List of matrices and/or scalars.
 *
 * @returns If all matrices in the argument list have the same extent along
 *    @c Dim, returns that extent. Otherwise, returns @c 0.
 *
 * @remarks Scalars are ignored in extent calculation. In particular, if there
 *    is no matrix in the argument list, the return value is @c 0.
 *
 * @ingroup matrix_traits
 */
template <size_t Dim, class... Ts>
size_t common_extent(const Ts&... ms)
{
  return details::common_extent_tr<Dim>(0, ms...);
}

/**
 * Defines matrix traits for 2D C arrays.
 *
 * @ingroup matrix_traits
 */
template <class T, size_t M, size_t N>
struct matrix_traits<T[M][N]>
{
  using matrix_storage_layout = row_major_storage_tag;

  using value_type = T;

  template <size_t Dim>
  static size_t extent(const T (&/*unused*/)[M][N])
  {
    return (Dim == 0)? M : (Dim == 1)? N : 0;
  }
};

} // namespace euler

#endif // EULER_MATRIX_TRAITS_HPP
