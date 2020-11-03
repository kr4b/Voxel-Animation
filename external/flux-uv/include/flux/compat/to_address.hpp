/*-******************************************************* -- HEADER -{{{1- */
/*-	to_address & co
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_TO_ADDRESS_HPP_7EC27788_DED0_4C27_9229_713F20822986
#define FLUX_COMPAT_TO_ADDRESS_HPP_7EC27788_DED0_4C27_9229_713F20822986

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/stdfeat.hpp>
#include <flux/setup/namespace.hpp>

//--    to_address()                ///{{{1///////////////////////////////////

#if FLUX_STDFEAT_CXX20_TO_ADDRESS
#	include <memory>

FLUX_NAMESPACE_ENTER1(compat)
	using std::to_address;
FLUX_NAMESPACE_LEAVE1(compat)

#else // flux-nih_ fallback
#	include <flux/nih_/to_address.hpp>

FLUX_NAMESPACE_ENTER1(compat)
	using nih_::to_address;
FLUX_NAMESPACE_LEAVE1(compat)

#endif // ~ implementations

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_TO_ADDRESS_HPP_7EC27788_DED0_4C27_9229_713F20822986
