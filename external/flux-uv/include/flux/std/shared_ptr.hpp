/*-******************************************************* -- HEADER -{{{1- */
/*-	shared_ptr<>
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_SHARED_PTR_HPP_9C80C89A_48C6_4D57_A348_3176AB134C5D
#define FLUX_STD_SHARED_PTR_HPP_9C80C89A_48C6_4D57_A348_3176AB134C5D

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/namespace.hpp>

#include <memory>

//--    shared_ptr<>                ///{{{1///////////////////////////////////

FLUX_NAMESPACE_ENTER()
	using std::weak_ptr;
	using std::shared_ptr;

	using std::make_shared;
FLUX_NAMESPACE_LEAVE()

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_STD_SHARED_PTR_HPP_9C80C89A_48C6_4D57_A348_3176AB134C5D
