#if 0
/**
 * @defgroup matrix_traits Matrix Traits
 *
 * Defines type traits and functions for matrix-like objects.
 *
 * A <i>matrix-like object</i> is a container object that provides access to a
 * 2D grid of elements through row and column indices. It is modelled after 2D
 * C array (if statically-sized) and vector of vectors (if dynamically-sized).
 *
 * Formally, an object @c a of type @c M is a matrix-like object if @c M
 * satisfies the @c Matrix concept. A @c Matrix is either a @c StaticMatrix,
 * whose dimensions are known at compile-time, or a @c DynamicMatrix, whose
 * dimensions are known at run-time. Both @c StaticMatrix and @c DynamicMatrix
 * derive from @c BasicMatrix, which provides access to its elements.
 *
 * The following diagram illustrates the relationship between the concepts and
 * summarizes their requirements. Details on the members are documented later.
 *
 *   +-----------------------------------------------------+
 *   | BasicMatrix                                         |
 *   +-----------------------------------------------------+
 *   | constant: size_t rank                               |
 *   | typedef:  value_type, reference, const_reference    |
 *   | function: at([const] M&, size_t, size_t)            |
 *   +-----------------------------------------------------+
 *            *                             *
 *   +------------------+     +----------------------------+
 *   | StaticExtent     |     | DynamicExtent              |
 *   +------------------+     +----------------------------+
 *   | constant:        |     | function:                  |
 *   | size_t extent<0> |     | size_t extent<0>(const M&) |
 *   | size_t extent<1> |     | size_t extent<1>(const M&) |
 *   +------------------+     +----------------------------+
 *            |                             |
 *            v                             v
 *   +------------------+     +----------------------------+
 *   | StaticMatrix     |  +  | DynamicMatrix              |
 *   +------------------+  |  +----------------------------+
 *                         |
 *                         v
 *   +-----------------------------------------------------+
 *   | Matrix                                              |
 *   +-----------------------------------------------------+
 *
 * For both @c StaticMatrix and @c DynamicMatrix,
 * <code>struct matrix_traits<M></code> must be specialized and provide
 *
 * (a) all of the following members:
 *
 *   size_t rank
 *       Must be 2.
 *
 *   value_type
 *       Type of elements stored in the matrix.
 *
 *   reference
 *       Type of reference to the elements. Must be the return type of member
 *       function <code>at(M&, size_t, size_t)</code>.
 *
 *   const_reference
 *       Type of const reference to the elements. Must be the return type of
 *       member function <code>at(const M&, size_t, size_t)</code>.
 *
 * and (b) at least one of the following members:
 *
 *   reference at(M &a, size_t i, size_t j)
 *       Returns the element at row @c i and column @c j of @c a. Subscripts
 *       @c i and @c j are zero-based. If any subscript is out of range, the
 *       behavior is undefined.
 *
 *   const_reference at(const M &a, size_t i, size_t j)
 *       Same as the above but for const object.
 *
 * A @c StaticMatrix must additionally provide the following members:
 *
 *   size_t extent<0>
 *       Equal to the number of rows in matrices of type @c M; may be 0.
 *
 *   size_t extent<1>
 *       Equal to the number of columns in matrices of type @c M; may be 0.
 *
 * A @c DynamicMatrix must additionally provide the following members:
 *
 *   size_t extent<0>(const M &a)
 *       Returns the number of rows in @c a; may be 0.
 *
 *   size_t extent<1>(const M &a)
 *       Returns the number of columns in @c a; may be 0.
 *
 * By designing algorithms based on generic Matrix types, one decouples code
 * that processes elements from code that handles the storage of elements.
 * This separation of responsibility is similar to the way iterators decouple
 * 1D algorithms from 1D containers.
 *
 * The @c matrix_traits.hpp header defines the @c matrix_traits class which
 * matrix storage implementations can specialize to expose their elements.
 * A stock implementation is provided for 2D C arrays to demonstrate the API.
 *
 * @ingroup Library
 */

#ifndef EULER_MATRIX_TRAITS_HPP
#define EULER_MATRIX_TRAITS_HPP

#include <array>
#include <cstddef>
#include <type_traits>
#include <vector>

namespace euler {

namespace details {

template <typename...>
using void_t = void;

}

/**
 * Defines type traits for a matrix-like object.
 *
 * @ingroup matrix_traits
 */
template <class M>
struct matrix_traits;

// static extent: define size_t value
// dynamic extent: define size_t value(const M&)
template <class M, size_t Dim>
struct matrix_extent;

namespace details {

template <class T, class = void>
struct is_basic_matrix : public std::false_type { };

// TODO: check member function at()
template <class T>
struct is_basic_matrix<T,
  void_t<std::enable_if_t<matrix_traits<T>::rank == 2>,
         typename matrix_traits<T>::value_type,
         typename matrix_traits<T>::reference,
         typename matrix_traits<T>::const_reference>>
  : public std::true_type { };

template <class T, size_t Dim, class = void>
struct has_static_extent : public std::false_type { };

template <class T, size_t Dim>
struct has_static_extent<T, Dim, void_t<
    std::integral_constant<size_t, matrix_extent<T, Dim>::value>>>
  : public std::true_type { };

template <class T, size_t Dim, class = void>
struct has_dynamic_extent : public std::false_type { };

template <class T, size_t Dim>
struct has_dynamic_extent<T, Dim, void_t<
    std::result_of_t<decltype(&matrix_extent<T, Dim>::value)(const T&)>>>
  : public std::true_type { };

#if 0
template <class T>
using is_static_matrix = std::conditional_t<
    is_basic_matrix<T>::value && has_static_extent<T>::value,
    std::true_type, std::false_type>;

template <class T>
using is_dynamic_matrix = std::conditional_t<
    is_basic_matrix<T>::value && has_dynamic_extent<T>::value,
    std::true_type, std::false_type>;
#endif

} // namespace details

/**
 * Checks whether the extent of type @c T along dimension @c Dim is known at
 * compile-time.
 *
 * @ingroup matrix_traits
 */
template <class T, size_t Dim>
using has_static_extent = details::has_static_extent<T, Dim>;

template <class T, size_t Dim>
constexpr bool has_static_extent_v = has_static_extent<T, Dim>::value;

/**
 * Checks whether the extent of type @c T along dimension @c Dim is not known
 * until run-time.
 *
 * @ingroup matrix_traits
 */
template <class T, size_t Dim>
using has_dynamic_extent = details::has_dynamic_extent<T, Dim>;

template <class T, size_t Dim>
constexpr bool has_dynamic_extent_v = has_dynamic_extent<T, Dim>::value;

/**
 * Checks whether a type satisfies the @c Matrix concept.
 * @ingroup matrix_traits
 */
template <class T>
using is_matrix = std::conditional_t<
    details::is_basic_matrix<T>::value &&
    (has_static_extent_v<T, 0> || has_dynamic_extent_v<T, 0>) &&
    (has_static_extent_v<T, 1> || has_dynamic_extent_v<T, 1>),
    std::true_type, std::false_type>;

template <class T>
constexpr bool is_matrix_v = is_matrix<T>::value;

/**
 * Gets the number of rows or columns in a matrix.
 *
 * @tparam Dim Dimension along which to retrieve the extent: @c 0 indicates
 *    number of rows and @c 1 indicates number of columns.
 *
 * @tparam T Type that satisfies the @c Matrix concept.
 *
 * @param a Matrix object.
 *
 * @returns <code>matrix_traits<T>::template extent<Dim>(a)</code> for dynamic
 *    matrix; <code>matrix_traits<T>::template extent<Dim></code> for static
 *    matrix.
 *
 * @ingroup matrix_traits
 */
template <size_t Dim, class M>
std::enable_if_t<has_static_extent_v<M, Dim>, size_t>
extent(const M & /* unused */)
{
  return matrix_extent<M, Dim>::value;
}

template <size_t Dim, class M>
std::enable_if_t<has_dynamic_extent_v<M, Dim>, size_t>
extent(const M &a)
{
  return matrix_extent<M, Dim>::value(a);
}

#if 0
template <bool IgnoreScalar, size_t Dim, class T, class... Ts>
struct is_equal_extent_possible;

template <bool IgnoreScalar, size_t Dim>
struct is_equal_extent_possible<IgnoreScalar>
{
  static_assert(Dim < 0, "At least one type argument must be supplied");
}

//template <size_t Dim, class T>
//struct is_equal_extent_possible<false, Dim, T>
//  : public std::enable_if_t<is_matrix_v<T>, std::true_type> { };

// If only one type is supplied, it must be matrix or otherwise the expression
// is ill-formed.
template <bool IgnoreScalar, size_t Dim, class T>
struct is_equal_extent_possible<IgnoreScalar, Dim, T>
  : public std::enable_if_t<is_matrix_v<T>, std::true_type> { };

template <size_t Dim, class T1, class T2>
struct is_equal_extent_possible<false, T1, T2> : public
  std::conditional_t<
    is_static_matrix_v<T1>,
    std::conditional_t<
      is_static_matrix_v<T2>,
      std::conditional_t<
        (matrix_traits<T1>::template extent<Dim>) ==
        (matrix_traits<T2>::template extent<Dim>),
        std::true_type,
        std::false_type>,
      ;

template <size_t Dim, class T1, class T2>
struct static_common_extent<T1, T2> : public

template <size_t Dim, class T1, class T2>
struct is_equal_extent_possible<true, T1, T2> : public

template <size_t Dim, class T, class T2>
struct static_common_extent_ignore_scalar<T1, T2> : public

//template <class T1, class T2>
//struct has_compatible_extent<false, T1, T2> : public is_matrix<T> { };
#endif

#if 0
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
#endif

#if 0
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
#endif

/**
 * Defines matrix traits for 2D C arrays. This demonstrates a static matrix.
 *
 * @ingroup matrix_traits
 */
template <class T, size_t M, size_t N>
struct matrix_traits<T[M][N]>
{
  static const size_t rank = 2;

  using value_type = T;
  using reference = T&;
  using const_reference = const T&;

  static reference at(T (&a)[M][N], size_t i, size_t j)
  {
    return a[i][j];
  }

  static const_reference at(const T (&a)[M][N], size_t i, size_t j)
  {
    return a[i][j];
  }
};

template <class T, size_t M, size_t N>
struct matrix_extent<T[M][N], 0>
{
  static const size_t value = M;
};

template <class T, size_t M, size_t N>
struct matrix_extent<T[M][N], 1>
{
  static const size_t value = N;
};

/**
 * Defines matrix traits for a vector of arrays. This demonstrates a dynamic
 * matrix.
 *
 * @ingroup matrix_traits
 */
template <class T, size_t N>
struct matrix_traits<std::vector<std::array<T, N>>>
{
  static const size_t rank = 2;

  using value_type = T;
  using reference = T&;
  using const_reference = const T&;

  static reference at(std::vector<std::array<T, N>> &a, size_t i, size_t j)
  {
    return a[i][j];
  }

  static const_reference at(
      const std::vector<std::array<T, N>> &a, size_t i, size_t j)
  {
    return a[i][j];
  }
};

template <class T, size_t N>
struct matrix_extent<std::vector<std::array<T, N>>, 0>
{
  static size_t value(const std::vector<std::array<T, N>> &a)
  {
    return a.size();
  }
};

template <class T, size_t N>
struct matrix_extent<std::vector<std::array<T, N>>, 1>
{
  static const size_t value = N;
};

} // namespace euler

#endif // EULER_MATRIX_TRAITS_HPP
#endif
