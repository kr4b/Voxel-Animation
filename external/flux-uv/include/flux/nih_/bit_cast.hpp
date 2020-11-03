/*-******************************************************* -- HEADER -{{{1- */
/*-	std::bit_cast() fallback
 *
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_NIH_BIT_CAST_HPP_2CE7589D_6327_4938_AFDD_F05C38EC0D12
#define FLUX_NIH_BIT_CAST_HPP_2CE7589D_6327_4938_AFDD_F05C38EC0D12

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/enable_if.hpp>

#include <flux/compat/type_traits.hpp>
#include <flux/compat/is_trivially.hpp>

#include <cstring>
#include <type_traits>

//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    bit_cast()                  ///{{{1///////////////////////////////////

// We can't make this constexpr without compiler magic. :-(
// See https://en.cppreference.com/w/cpp/numeric/bit_cast

template< typename tTo, typename tFrom >
FLUX_ENABLE_IF_C(
	sizeof(tTo) == sizeof(tFrom) &&
	std::is_trivial<tTo>::value &&
	compat::is_trivially_copyable<tFrom>::value
)(tTo) bit_cast( tFrom const& aSrc) noexcept
{
	compat::remove_const_t<tTo> dst;
	std::memcpy( &dst, &aSrc, sizeof(dst) );
	return dst;
}

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_NIH_BIT_CAST_HPP_2CE7589D_6327_4938_AFDD_F05C38EC0D12
