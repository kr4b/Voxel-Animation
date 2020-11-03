/*-******************************************************* -- HEADER -{{{1- */
/*-	std::exchange()
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_EXCHANGE_HPP_B636784F_36D2_4E7D_B973_C69DD3DD0A77
#define FLUX_COMPAT_EXCHANGE_HPP_B636784F_36D2_4E7D_B973_C69DD3DD0A77

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/stdfeat.hpp>
#include <flux/setup/namespace.hpp>

//--    clamp()                     ///{{{1///////////////////////////////////

#if FLUX_STDFEAT_CXX14_EXCHANGE
#	include <utility>

FLUX_NAMESPACE_ENTER1(compat)
	using std::exchange;
FLUX_NAMESPACE_LEAVE1(compat)

#else // flux-nih_ fallback
#	include <flux/nih_/exchange.hpp>

FLUX_NAMESPACE_ENTER1(compat)
	using nih_::exchange;
FLUX_NAMESPACE_LEAVE1(compat)

#endif // ~ implementations

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_EXCHANGE_HPP_B636784F_36D2_4E7D_B973_C69DD3DD0A77
