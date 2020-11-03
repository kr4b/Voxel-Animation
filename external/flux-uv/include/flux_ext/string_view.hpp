/*-******************************************************* -- HEADER -{{{1- */
/*-	Include string-view-lite with appropriate settings
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_EXT_STRING_VIEW_HPP_5FB27202_FD12_4643_B59B_7B3D50EA3F9B
#define FLUX_EXT_STRING_VIEW_HPP_5FB27202_FD12_4643_B59B_7B3D50EA3F9B

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg> 
#include <flux/setup/stdfeat.hpp> 

//--    <string_view.hpp>           ///{{{1///////////////////////////////////

#if FLUX_STDFEAT_CXX17_STRING_VIEW
#	define nssv_CONFIG_SELECT_STD_STRING_VIEW 1
#	define nssv_CONFIG_CONVERSTION_STD_STRING_FREE_FUNCTIONS 1
#endif // ~ CXX17_STRING_VIEW

#include "impl/string_view.hpp"

/* Also place string_view and related stuff in the namespace `flux_ext::nonstd`.
 */
namespace flux_ext
{
	namespace nonstd = ::nonstd;
}

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_EXT_STRING_VIEW_HPP_5FB27202_FD12_4643_B59B_7B3D50EA3F9B
