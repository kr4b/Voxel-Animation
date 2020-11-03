/*-******************************************************* -- HEADER -{{{1- */
/*-	Include invoke-lite with appropriate settings
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_EXT_INVOKE_HPP_3ABB7D89_4C06_428F_8FE0_2EAE8E1A6F87
#define FLUX_EXT_INVOKE_HPP_3ABB7D89_4C06_428F_8FE0_2EAE8E1A6F87

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg> 

//--    <invoke.hpp>                ///{{{1///////////////////////////////////

#include "impl/invoke.hpp"

/* Also place span and related stuff in the namespace `flux_ext::nonstd`.
 */
namespace flux_ext
{
	namespace nonstd = ::nonstd;
}

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_EXT_INVOKE_HPP_3ABB7D89_4C06_428F_8FE0_2EAE8E1A6F87
