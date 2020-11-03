/*-******************************************************* -- HEADER -{{{1- */
/*-	observer_ptr
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_OBSERVER_PTR_HPP_7C4E044C_405D_4C93_8F5A_CCBE343EC73A
#define FLUX_COMPAT_OBSERVER_PTR_HPP_7C4E044C_405D_4C93_8F5A_CCBE343EC73A

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/stdfeat.hpp>
#include <flux/setup/namespace.hpp>

//--    make_unique()               ///{{{1///////////////////////////////////

#if FLUX_STDFEAT_XP_OBSERVER_PTR
#	include <experimental/memory>

FLUX_NAMESPACE_ENTER1(compat)
	using std::experimental::observer_ptr;
	using std::experimental::make_observer;
FLUX_NAMESPACE_LEAVE1(compat)

#else // fallback to nih_
#	include <flux/nih_/observer_ptr.hpp>

FLUX_NAMESPACE_ENTER1(compat)
	using nih_::observer_ptr;
	using nih_::make_observer;
FLUX_NAMESPACE_LEAVE1(compat)

#endif // ~ implementations

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_OBSERVER_PTR_HPP_7C4E044C_405D_4C93_8F5A_CCBE343EC73A
