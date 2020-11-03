/*-******************************************************* -- HEADER -{{{1- */
/*-	Include span-lite with appropriate settings
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_EXT_SPAN_HPP_3C332C19_8BEC_476A_B1CA_E58940CEA905
#define FLUX_EXT_SPAN_HPP_3C332C19_8BEC_476A_B1CA_E58940CEA905

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg> 

//--    <span.hpp>                  ///{{{1///////////////////////////////////

#define span_CONFIG_PROVIDE_MAKE_SPAN 1
#define span_CONFIG_CONTRACT_VIOLATION_THROWS 1

//TODO span_HAVE based on langfeat / stdfeat ?

#include "impl/span.hpp"

/* Also place span and related stuff in the namespace `flux_ext::nonstd`.
 */
namespace flux_ext
{
	namespace nonstd = ::nonstd;
}

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_EXT_SPAN_HPP_3C332C19_8BEC_476A_B1CA_E58940CEA905
