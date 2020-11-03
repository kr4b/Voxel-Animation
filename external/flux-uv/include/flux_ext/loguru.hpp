/*-******************************************************* -- HEADER -{{{1- */
/*-	Include loguru 
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_EXT_LOGURU_HPP_16611C4E_9661_4674_8B18_02349D770CE9
#define FLUX_EXT_LOGURU_HPP_16611C4E_9661_4674_8B18_02349D770CE9

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg> 
#include <flux/setup/buildcfg.hpp>

//--    <loguru.hpp>                ///{{{1///////////////////////////////////

#define LOGURU_DEBUG_CHECKS   FLUX_DEBUG
#define LOGURU_DEBUG_LOGGING  FLUX_DEBUG

// Don't mess with assert() for now.
#define LOGURU_REDEFINE_ASSERT    0

// Don't include the C++ streams, they have a hideous syntax anyway
#define LOGURU_WITH_STREAMS       0 

// Make stuff slightly narrower.
#define LOGURU_FILENAME_WIDTH     12
#define LOGURU_THREADNAME_WIDTH   8

// Use fmtlib. Loguru modified to use the one included in flux.
#define LOGURU_USE_FMTLIB         1

#include "impl/loguru.hpp"

namespace flux_ext
{
	namespace loguru = ::loguru;
}

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_EXT_LOGURU_HPP_16611C4E_9661_4674_8B18_02349D770CE9
