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
#include <type_traits>

namespace euler {
	
/** Integral type of at least twice the number bits in @c T. */
template <class T> struct make_wide    { typedef void type; };
template <> struct make_wide<int32_t>  { typedef int64_t type; };
template <> struct make_wide<int16_t>  { typedef int32_t type; };
template <> struct make_wide<int8_t>   { typedef int16_t type; };
template <> struct make_wide<uint32_t> { typedef uint64_t type; };
template <> struct make_wide<uint16_t> { typedef uint32_t type; };
template <> struct make_wide<uint8_t>  { typedef uint16_t type; };

/** Integral type of at least half the number bits in @c T. */
template <typename T> struct make_narrow { typedef void type; };
template <> struct make_narrow<int64_t>  { typedef int32_t type; };
template <> struct make_narrow<int32_t>  { typedef int16_t type; };
template <> struct make_narrow<int16_t>  { typedef int8_t type; };
template <> struct make_narrow<uint64_t> { typedef uint32_t type; };
template <> struct make_narrow<uint32_t> { typedef uint16_t type; };
template <> struct make_narrow<uint16_t> { typedef uint8_t type; };

template <typename T> using make_unsigned = std::make_unsigned<T>;

template <typename T> using make_signed = std::make_signed<T>;

} // namespace euler

#endif
