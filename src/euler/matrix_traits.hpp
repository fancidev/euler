/**
 * @defgroup MatrixTraits Matrix Accessor Traits
 *
 * Defines type traits and functions for matrix accessors.
 *
 * A <i>matrix accessor</i> is a proxy object which encapsulate an 
 * underlying storage to be accessed like a matrix. It can be roughly
 * thought of as the 2-D version of a traditional iterator. Just as
 * iterators enable 1-D algorithm implementations to be separated from 
 * the underlying container, matrix accessors allow the same source code
 * to be applied to different storage structures, even mixing heterogeous 
 * matrix types in the same operation.
 *
 * Despite the anology between a matrix accessor and an array iterator, 
 * there is a key differences between them: a matrix accessor does not
 * have a <i>direction</i>. This is because there is no natural way to 
 * define the traversing order a matrix, just like there is no natural
 * way to compare the magnitude of two complex numbers. This has a few
 * implications, which we put as the following requirements:
 *
 *   - a matrix accessor must provide random access to any element in
 *     the matrix by a pair of indices <code>(i, j)</code>;
 *   - a matrix accessor must not be incremented or otherwise altered;
 *   - a matrix accessor should be passed by reference into a function.
 *
 * The interface for a matrix accessor is designed in a way such that
 * the traditional static 2-D C array can be accessed naturally. 
 * Specifically, the requirements for a matrix accessor @c A are:
 *
 *   - <code>A[i][j]</code> returns a reference to the element at 
 *     row <code>i</code> column <code>j</code>, where both indices
 *     are zero based. This effectively means @c A is being accessed
 *     in a row-major manner.
 *   - The free-standing function <code>size<Dim>(A)</code> be defined,
 *     such that <code>size<1>(A)</code> returns the number of rows 
 *     in <code>A</code>, and <code>size<2>(A)</code> returns the number
 *     of columns in <code>A</code>.
 * 
 * The second requirement is not automatically provided for a static C
 * array, and therefore needs to be implemented explicitly. The matrix
 * adaptor module provides a stock matrix accessor implementation for
 * static arrays, as well as for a few other common storage types.
 *
 * Finally, note the difference between a const matrix accessor and an 
 * accessor to const elements. For example, a const accessor cannot swap 
 * two elements in the matrix, while an accessor to const elements could
 * swap two elements but cannot alter the internal value of any element.
 * 
 * @ingroup LinearAlgebra
 */

#ifndef EULER_MATRIX_TRAITS_HPP
#define EULER_MATRIX_TRAITS_HPP

#include <type_traits>

namespace euler {

/**
 * Defines the type traits for a matrix accessor. A custom matrix accessor
 * must specialize these traits.
 * @ingroup MatrixTraits
 */
template <class M>
struct matrix_traits 
{
	// Type of the elements stored in the matrix.
	// typedef void value_type;
	
	// Type of the row (if the matrix is stored in row-major) or 
	// column (if the matrix is stored in column major).
	// typedef void vector_type;
};

/**
 * Checks whether a type implements the matrix accessor interface.
 * This is done by checking whether <code>T::value_type</code> is defined.
 * @ingroup MatrixTraits
 */
template <
	class M, 
	class Test = typename euler::matrix_traits<M>::value_type>
struct is_matrix : public std::true_type { };

/**
 * Returns the number of rows or columns in a matrix.
 * This function must be specialized for a custom matrix accessor.
 *
 * The dimension along which the extent is returned is specified by
 * the template parameter <code>Dim</code>. If <code>Dim = 1</code>,  
 * the number of rows is returned. If <code>Dim = 2</code>, the number
 * of columns is returned. Other values of @c Dim are not allowed.
 * 
 * @param m The matrix whose extent is returned.
 * @returns The extent of the matrix along the dimension.
 * @ingroup MatrixTraits
 */
template <size_t Dim, class M>
size_t size(const M& m) 
{
	static_assert(false, "size<Dim,M> must be specialized.");
}

} // namespace euler

#endif // EULER_MATRIX_TRAITS_HPP
