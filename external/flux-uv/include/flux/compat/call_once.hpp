/*-******************************************************* -- HEADER -{{{1- */
/*-	call_once from <mutex>
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_CALL_ONCE_HPP_2FDA0A48_C797_47CE_A3DD_130A36552D53
#define FLUX_COMPAT_CALL_ONCE_HPP_2FDA0A48_C797_47CE_A3DD_130A36552D53

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/stdfeat.hpp>
#include <flux/setup/namespace.hpp>

//--    mutex                       ///{{{1///////////////////////////////////

#if !FLUX_STDQUIRK_CXX11_CALL_ONCE_EXCEPT
#	include <mutex>

FLUX_NAMESPACE_ENTER1(compat)
	using std::once_flag;
	using std::call_once;
FLUX_NAMESPACE_LEAVE1(compat)

#else
#	include <flux/nih_/call_once.hpp>

FLUX_NAMESPACE_ENTER1(compat)
	using nih_::once_flag;
	using nih_::call_once;
FLUX_NAMESPACE_LEAVE1(compat)

#endif // ~ implementations

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_CALL_ONCE_HPP_2FDA0A48_C797_47CE_A3DD_130A36552D53
