/**
 * @defgroup IntTraits Integer Traits
 * Compile-time tweaks to get properties of an integral type.
 *
 * @ingroup Library
 *
 * Integer arithmetic constitutes a large part of number theory calculations.
 * However, since integers are of fixed bits in C++, it is often subject to
 * potential overflow issues, if not taken care of properly. In addition, 
 * the choice of signed or unsigned integer sometimes imposes subtle impact 
 * on the correctness of computations.
 *
 * One approach to avoid integer overflow is to restrict the size of the
 * input and use appropriate types during the calculation. This type-aware
 * approach can also be used to deal with signed or unsigned mismatch.
 *
 * This module provides several useful type traits for integral types. 
 * For a given integral type @c T, it provides the related integral type
 * of twice the bits or half the bits of @c T, as well as signed and 
 * unsigned version of @c T. These traits are used extensively in modular 
 * arithmetic routines.
 */

#ifndef EULER_INT_TRAITS_HPP
#define EULER_INT_TRAITS_HPP

#include <cstdint>

namespace euler {
	
template <class T> struct make_wide    { typedef void type; };
template <> struct make_wide<int32_t>  { typedef int64_t type; };
template <> struct make_wide<int16_t>  { typedef int32_t type; };
template <> struct make_wide<int8_t>   { typedef int16_t type; };
template <> struct make_wide<uint32_t> { typedef uint64_t type; };
template <> struct make_wide<uint16_t> { typedef uint32_t type; };
template <> struct make_wide<uint8_t>  { typedef uint16_t type; };

template <typename T> struct make_narrow { typedef void type; };
template <> struct make_narrow<int64_t>  { typedef int32_t type; };
template <> struct make_narrow<int32_t>  { typedef int16_t type; };
template <> struct make_narrow<int16_t>  { typedef int8_t type; };
template <> struct make_narrow<uint64_t> { typedef uint32_t type; };
template <> struct make_narrow<uint32_t> { typedef uint16_t type; };
template <> struct make_narrow<uint16_t> { typedef uint8_t type; };

/**
 * Provides related types to an integral type.
 * @ingroup IntTraits
 */
template <class T> struct int_traits 
{
	/** Integral type of at least twice the number bits in @c T. */
	typedef void wide_type;

	/** Integral type of at least half the number of bits in @c T. */
	typedef void narrow_type;

	/** Signed version of @c T. */
	typedef void signed_type;

	/** Unsigned version of @c T. */
	typedef void unsigned_type;
};

/// Specialized integer traits for @c char.
/// @ingroup IntTraits
template <> struct int_traits<char> 
{
	/// Integral type of twice the number of bits in this type.
	typedef short wide_type;

	/// Integral type of at least half the number of bits in this type.
	/// Defined to <code>char</code> because there is no integral type of
	/// fewer bits.
	typedef char narrow_type;

	/// Signed version of the type.
	typedef char signed_type;

	/// Unsigned version of the type.
	typedef unsigned char unsigned_type;
};

/// Specialized integer traits for <code>unsigned char</code>.
/// @ingroup IntTraits
template <> struct int_traits<unsigned char> 
{ 
	/// Integral type of twice the number of bits in this type.
	typedef unsigned short wide_type;

	/// Integral type of at least half the number of bits in this type.
	/// Defined to <code>unsigned char</code> because there is no 
	/// integral type of fewer bits.
	typedef unsigned char narrow_type;

	/// Signed version of the type.
	typedef char signed_type;

	/// Unsigned version of the type.
	typedef unsigned char unsigned_type;
};

/// Specialized integer traits for <code>short</code>.
/// @ingroup IntTraits
template <> struct int_traits<short> 
{ 
	/// Integral type of twice the number of bits in this type.
	typedef long wide_type;

	/// Integral type of half the number of bits in this type.
	typedef char narrow_type;

	/// Signed version of the type.
	typedef short signed_type;

	/// Unsigned version of the type.
	typedef unsigned short unsigned_type;
};

/// Specialized integer traits for <code>unsigned short</code>.
/// @ingroup IntTraits
template <> struct int_traits<unsigned short> 
{ 
	/// Integral type of twice the number of bits in this type.
	typedef unsigned long wide_type;

	/// Integral type of half the number of bits in this type.
	typedef unsigned char narrow_type;

	/// Signed version of the type.
	typedef short signed_type;

	/// Unsigned version of the type.
	typedef unsigned short unsigned_type;
};

/// Specialized integer traits for <code>long</code>.
/// @ingroup IntTraits
template <> struct int_traits<long>
{ 
	/// Integral type of twice the number of bits in this type.
	typedef long long wide_type;

	/// Integral type of half the number of bits in this type.
	typedef short narrow_type;

	/// Signed version of the type.
	typedef long signed_type;

	/// Unsigned version of the type.
	typedef unsigned long unsigned_type;
};

/// Specialized integer traits for <code>unsigned long</code>.
/// @ingroup IntTraits
template <> struct int_traits<unsigned long>
{ 
	/// Integral type of twice the number of bits in this type.
	typedef unsigned long long wide_type;

	/// Integral type of half the number of bits in this type.
	typedef unsigned short narrow_type;

	/// Signed version of the type.
	typedef long signed_type;

	/// Unsigned version of the type.
	typedef unsigned long unsigned_type;
};

/// Specialized integer traits for <code>int</code>.
/// @ingroup IntTraits
template <> struct int_traits<int>
{ 
	/// Integral type of twice the number of bits in this type.
	typedef long long wide_type;

	/// Integral type of half the number of bits in this type.
	typedef short narrow_type;

	/// Signed version of the type.
	typedef int signed_type;

	/// Unsigned version of the type.
	typedef unsigned int unsigned_type;
};

/// Specialized integer traits for <code>unsigned int</code>.
/// @ingroup IntTraits
template <> struct int_traits<unsigned int>
{ 
	/// Integral type of twice the number of bits in this type.
	typedef unsigned long long wide_type;

	/// Integral type of half the number of bits in this type.
	typedef unsigned short narrow_type;

	/// Signed version of the type.
	typedef int signed_type;

	/// Unsigned version of the type.
	typedef unsigned int unsigned_type;
};

/// Specialized integer traits for <code>long long</code>.
/// @ingroup IntTraits
template <> struct int_traits<long long>
{ 
	/// Defined to <code>void</code> because there is no built-in 
	/// integral type of more bits than <code>long long</code>.
	typedef void wide_type;

	/// Integral type of half the number of bits in this type.
	typedef long narrow_type;

	/// Signed version of the type.
	typedef long long signed_type;

	/// Unsigned version of the type.
	typedef unsigned long long unsigned_type;
};

/// Specialized integer traits for <code>unsigned long long</code>.
/// @ingroup IntTraits
template <> struct int_traits<unsigned long long> 
{ 
	/// Defined to <code>void</code> because there is no built-in 
	/// integral type of more bits than <code>unsigned long long</code>.
	typedef void wide_type;

	/// Integral type of half the number of bits in this type.
	typedef unsigned long narrow_type;

	/// Signed version of the type.
	typedef long long signed_type;

	/// Unsigned version of the type.
	typedef unsigned long long unsigned_type;
};

#if 0
template <typename T, typename NeedInt=std::enable_if<std::is_integral<T>::value>>
struct product : public std::pair<T,T> 
{ 
	product(T a, T b) : std::pair<T,T>(a, b) { }
};

template <typename T>
product<T> make_product(T a, T b)
{
	return product<T>(a, b);
}

template <typename T> 
struct value_of { typedef T type; };

template <typename T>
struct value_of<product<T>> { typedef typename int_traits<T>::wide_type type; };
#endif

} // namespace euler

#endif
