/*-******************************************************* -- HEADER -{{{1- */
/*-	bit_cast()
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_BIT_CAST_HPP_822C65C7_BD51_4D81_99D0_2B138BF63E40
#define FLUX_COMPAT_BIT_CAST_HPP_822C65C7_BD51_4D81_99D0_2B138BF63E40

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/stdfeat.hpp>
#include <flux/setup/namespace.hpp>

//--    bit_cast()                  ///{{{1///////////////////////////////////

#if FLUX_STDFEAT_CXX20_BIT_CAST
#	include <bit>

FLUX_NAMESPACE_ENTER1(compat)
	using std::bit_cast;
FLUX_NAMESPACE_LEAVE1(compat)

#else // flux-nih_ fallback
#	include <flux/nih_/bit_cast.hpp>

FLUX_NAMESPACE_ENTER1(compat)
	using nih_::bit_cast;
FLUX_NAMESPACE_LEAVE1(compat)

#endif // ~ implementations

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_BIT_CAST_HPP_822C65C7_BD51_4D81_99D0_2B138BF63E40
