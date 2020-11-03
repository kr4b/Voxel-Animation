/*-******************************************************* -- HEADER -{{{1- */
/*-	Include shared_mutex by Howard Hinnant with appropriate settings
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_EXT_SHARED_MUTEX_HPP_E8FF07D2_89E5_4AD3_9E6F_6832F6870607
#define FLUX_EXT_SHARED_MUTEX_HPP_E8FF07D2_89E5_4AD3_9E6F_6832F6870607

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg> 

//--    <shared_mutex>              ///{{{1///////////////////////////////////


#include "impl/shared_mutex"

/* Also place span and related stuff in the namespace `flux_ext::nonstd`.
 */
namespace flux_ext
{
	namespace ting = ::ting;
}

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_EXT_SHARED_MUTEX_HPP_E8FF07D2_89E5_4AD3_9E6F_6832F6870607
