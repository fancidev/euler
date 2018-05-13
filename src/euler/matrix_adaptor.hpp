#if 0
/**
 * @defgroup MatrixAdaptor Matrix Adaptors
 *
 * Implements the matrix accessor interface for common storage types.
 *
 * A <i>matrix adaptor</i> is a set of type traits and free functions
 * that implements the matrix accessor interface, which allows a generic
 * storage object be accessed like a standard 2-D C array. The following
 * adaptors are implemented by this module:
 *   - static array
 *   - dynamic array (with continuous storage)
 *   - zagged array
 *
 * @ingroup LinearAlgebra
 */

#ifndef EULER_MATRIX_ADAPTOR_HPP
#define EULER_MATRIX_ADAPTOR_HPP

#include "matrix_traits.hpp"

namespace euler {

/// Matrix traits for a static 2-D array.
template <class T, size_t M, size_t N>
struct matrix_traits<T[M][N]>
{
	typedef T value_type;
	typedef T vector_type[N];
};

template <size_t Dim, class T, size_t M, size_t N>
size_t size(const T (&A)[M][N])
{
	static_assert(Dim == 1 || Dim == 2, "Dim must be 1 or 2.");
	return (Dim == 1)? M : N;
}

} // namespace euler

#endif // EULER_MATRIX_ADAPTOR_HPP
#endif
