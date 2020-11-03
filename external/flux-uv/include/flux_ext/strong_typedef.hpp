/*-******************************************************* -- HEADER -{{{1- */
/*-	Include strong_typedef from type_safe with appropriate settings
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_EXT_STRONG_TYPEDEF_HPP_B411BD34_F14E_44A6_9DA3_E55E56845D53
#define FLUX_EXT_STRONG_TYPEDEF_HPP_B411BD34_F14E_44A6_9DA3_E55E56845D53

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg> 

//--    <span.hpp>                  ///{{{1///////////////////////////////////


#include "impl/strong_typedef.hpp"

/* Also place span and related stuff in the namespace `flux_ext::nonstd`.
 */
namespace flux_ext
{
	namespace type_safe = ::type_safe;
}

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_EXT_STRONG_TYPEDEF_HPP_B411BD34_F14E_44A6_9DA3_E55E56845D53
