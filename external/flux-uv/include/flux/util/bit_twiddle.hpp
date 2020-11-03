/*-******************************************************* -- HEADER -{{{1- */
/*-	Bit Twiddling
 *
 * Bit operations using either intrinsics or via software fallbacks. See
 * especially "Bit Twiddling Hacks":
 *  https://graphics.stanford.edu/~seander/bithacks.html
 *
 * Unfortunately these cannot be made `constexpr` as (a) the achitecture
 * intrinsics (e.g. _mm_popcnt_u32()) and (b) the MSVC intrinsics (e.g.
 * _BitScanReverse()) are not `constexpr` yet. Apparently GCC/Clang builtins
 * are `constexpr` to some degree.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_UTIL_BIT_TWIDDLE_HPP_E1299D3D_AF5F_4298_9B48_BD5FC2B45104
#define FLUX_UTIL_BIT_TWIDDLE_HPP_E1299D3D_AF5F_4298_9B48_BD5FC2B45104

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/inttypes.hpp>
//
//--    >>> namespace = flux::util >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(util)
//--    functions                           ///{{{2///////////////////////////

/** \name Population Count
 *
 * Count number of set (one) bits.
 *
 * \note Implementation notes:
 *   - x86: `popcont` via `__popcnt()` intrinsic (if supported)
 *   - GCC/clang: `__builtin_popcount*()`
 *   - fallback: logarithmic reduction with sideways addition
 */
//@{
inline uint16_t bit_count_set( uint16_t ) noexcept;
inline uint32_t bit_count_set( uint32_t ) noexcept;
inline uint64_t bit_count_set( uint64_t ) noexcept;
//@}

/** \name Count Leading Zeros
 *
 * Count leading zeros, i.e., the number of zero bits before the first one-bit,
 * starting with the MSB and going towards the LSB.
 *
 * \note Implementation notes:
 *   - x86: `lzcnt` via `__lzcnt()` intrinsic (if supported)
 *   - GCC/clang: `__builtin_clz*()` + check for zero
 *   - MSVC: `_BitScanReverse*()` + check for zero
 *   - fallback: binary search
 */
//@{
inline uint16_t bit_count_leading_zeros( uint16_t ) noexcept;
inline uint32_t bit_count_leading_zeros( uint32_t ) noexcept;
inline uint64_t bit_count_leading_zeros( uint64_t ) noexcept;
//@}

/** \name Count Trailing Zeros
 *
 * Count trailing zeros, i.e., the number of zero bits before the first one-bit
 * starting with the LSB and going towards the MSB.
 *
 * \note Implementation notes:
 *   - x86: `tzcnt` via `__tzcnt()` intrinsic (if supported)
 *   - GCC/clang: `__builtin_ctz*()` + check for zero
 *   - MSVC: `_BitScanForward*()` + check for zero
 *   - fallback: binary search
 */
//@{
inline uint16_t bit_count_trailing_zeros( uint16_t ) noexcept;
inline uint32_t bit_count_trailing_zeros( uint32_t ) noexcept;
inline uint64_t bit_count_trailing_zeros( uint64_t ) noexcept;
//@}

//--    <<< ~ flux::util namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(util)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/bit_twiddle.inl"
#endif // FLUX_UTIL_BIT_TWIDDLE_HPP_E1299D3D_AF5F_4298_9B48_BD5FC2B45104
