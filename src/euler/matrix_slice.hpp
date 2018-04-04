#ifndef EULER_MATRIX_SLICE_HPP
#define EULER_MATRIX_SLICE_HPP

#if 0 /* Not used */

namespace euler {

// Todo: rename matrix_row to matrix_slice (dx,dy). So that it can work with both
// row, column, and diagonal.
template <class Matrix>
class matrix_row
{
	Matrix &_mat;
	size_t _i;
public:
	typedef typename Matrix::value_type value_type;
	matrix_row(Matrix &mat, size_t i) : _mat(mat), _i(i) { }
	size_t size() const { return _mat.size2(); }
	value_type& operator[](size_t k) { return _mat(_i,k); }
};

/// Implements a vector adaptor that encapsulates a row or column in a matrix.
template <class Matrix>
class matrix_row_iterator
{
	Matrix &_mat;
	size_t _i;
public:
	matrix_row_iterator(Matrix &mat, size_t i) : _mat(mat), _i(i) { }
	
	bool operator == (const matrix_row_iterator &r) const 
	{
		return &_mat == &r._mat && _i == i;
	}

	size_t index() const { return _i; }

	matrix_row<Matrix> operator[](size_t k) const
	{
		return matrix_row<Matrix>(_mat, _i+k);
	}
};

template <class Matrix>
std::ptrdiff_t operator - (
	const matrix_row_iterator<Matrix> &r1, 
	const matrix_row_iterator<Matrix> &r2)
{
	return (ptrdiff_t)r1.index() - (ptrdiff_t)r2.index();
}

} // namespace euler

namespace std {

template <class M1, class M2>
void swap(euler::matrix_row<M1> r1, euler::matrix_row<M2> r2)
{
	const size_t N = r1.size();
	assert(N == r2.size());

	for (size_t i = 0; i < N; i++)
		std::swap(r1[i], r2[i]);
}

} // namespace std

#endif

#endif EULER_MATRIX_SLICE_HPP
