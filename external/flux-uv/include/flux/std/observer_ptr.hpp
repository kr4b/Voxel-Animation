/*-******************************************************* -- HEADER -{{{1- */
/*-	observer_ptr<>
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_OBSERVER_PTR_HPP_4F7A0F71_9D65_47B2_8E9A_0A49CFA1BA50
#define FLUX_STD_OBSERVER_PTR_HPP_4F7A0F71_9D65_47B2_8E9A_0A49CFA1BA50

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/namespace.hpp>
#include <flux/compat/observer_ptr.hpp>

//--    observer_ptr<>              ///{{{1///////////////////////////////////

FLUX_NAMESPACE_ENTER()
	using compat::observer_ptr;
	using compat::make_observer;
FLUX_NAMESPACE_LEAVE()

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_STD_OBSERVER_PTR_HPP_4F7A0F71_9D65_47B2_8E9A_0A49CFA1BA50
