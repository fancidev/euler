/**
 * @defgroup int_traits Integer Traits
 *
 * Compile-time tweaks to get properties of an integral type.
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
 *
 * @ingroup Library
 */

#ifndef EULER_INT_TRAITS_HPP
#define EULER_INT_TRAITS_HPP

#include <cstdint>
#include <type_traits>

namespace euler {

/**
 * Integral type with at least twice the number bits in @c T.
 * @ingroup int_traits
 */
template <class T> struct make_wide
{
  /** Result type */
  using type = void;
};

/**
 * Specializes @c make_wide for @c int32_t.
 * @ingroup int_traits
 */
template <> struct make_wide<int32_t>
{
  /** Result type */
  using type = int64_t;
};

/**
 * Specializes @c make_wide for @c int16_t.
 * @ingroup int_traits
 */
template <> struct make_wide<int16_t>
{
  /** Result type */
  using type = int32_t;
};

/**
 * Specializes @c make_wide for @c int8_t.
 * @ingroup int_traits
 */
template <> struct make_wide<int8_t>
{
  /** Result type */
  using type = int16_t;
};

/**
 * Specializes @c make_wide for @c uint32_t.
 * @ingroup int_traits
 */
template <> struct make_wide<uint32_t>
{
  /** Result type */
  using type = uint64_t;
};

/**
 * Specializes @c make_wide for @c uint16_t.
 * @ingroup int_traits
 */
template <> struct make_wide<uint16_t>
{
  /** Result type */
  using type = uint32_t;
};

/**
 * Specializes @c make_wide for @c uint8_t.
 * @ingroup int_traits
 */
template <> struct make_wide<uint8_t>
{
  /** Result type */
  using type = uint16_t;
};

/** Integral type of at least half the number bits in @c T. */
template <typename T> struct make_narrow
{
  /** Result type */
  typedef void type;
};

/**
 * Specializes @c make_narrow for @c int64_t.
 * @ingroup int_traits
 */
template <> struct make_narrow<int64_t>
{
  /** Result type */
  using type = int32_t;
};

/**
 * Specializes @c make_narrow for @c int32_t.
 * @ingroup int_traits
 */
template <> struct make_narrow<int32_t>
{
  /** Result type */
  using type = int16_t;
};

/**
 * Specializes @c make_narrow for @c int16_t.
 * @ingroup int_traits
 */
template <> struct make_narrow<int16_t>
{
  /** Result type */
  using type = int8_t;
};

/**
 * Specializes @c make_narrow for @c uint64_t.
 * @ingroup int_traits
 */
template <> struct make_narrow<uint64_t>
{
  /** Result type */
  using type = uint32_t;
};

/**
 * Specializes @c make_narrow for @c uint32_t.
 * @ingroup int_traits
 */
template <> struct make_narrow<uint32_t>
{
  /** Result type */
  using type = uint16_t;
};

/**
 * Specializes @c make_narrow for @c uint16_t.
 * @ingroup int_traits
 */
template <> struct make_narrow<uint16_t>
{
  /** Result type */
  using type = uint8_t;
};

template <class T>
using make_unsigned = std::make_unsigned<T>;

template <class T>
using make_signed = std::make_signed<T>;

} // namespace euler

#endif // EULER_INT_TRAITS_HPP
