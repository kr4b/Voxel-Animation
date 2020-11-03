/*-******************************************************* -- HEADER -{{{1- */
/*-	FLUX_LOG() et al.
 *
 * This is essentially a thin wrapper around loguru. See ext_misc_ and 
 * externals.md for additional information. See log_control.hpp for 
 * additional API.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_LOG_HPP_D6BCB6DD_CAB5_43BC_B639_E1629BC39B7D
#define FLUX_STD_LOG_HPP_D6BCB6DD_CAB5_43BC_B639_E1629BC39B7D

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux_ext/loguru.hpp>

//--    sketch v2                   ///{{{1///////////////////////////////////

#if 0

/*
 -  have both facility and level. Or together

	 FLUX_LOG( INFO, "foo {:d}", 55 ); // facility = default (app?)
	 FLUX_LOG( FLUX|INFO, "foo {:d}", 55 );  // facility = flux
	 FLUX_LOG( FLUX|WARN, "flab {}", "hej" );

 - FLUX_LOG(), FLUX_LOG_SCOPE() ;; FLUX_VLOG(), FLUX_VLOG_SCOPE() 
 - FLUX_DEBUG_LOG() + variations above

 - FLUX_LOG_IF() ???

 - Can set level per facility
   gLogFacility[facility] <= level ==>> early out
 - Can set log per consumer?
   Each consumer gets a struct with meta data, so that should be possible.

 - Common format functions call out to precompiled versions
    - (no formatting)
	- integers := int32_t, uint32_t, uint64_t, int64_t
	- strings :=  char const*, std::string const&, compat::string_view const&
	- single argument: integers, strings
	- double argument: integers, strings

*/

#endif


//--    include                     ///{{{1///////////////////////////////////

#define FLUX_LOG(level, ...) LOG_F( level, __VA_ARGS__ )
#define FLUX_LOG_IF(level, cond, ...) LOG_IF_F( level, cond, __VA_ARGS__ )

#define FLUX_DEBUG_LOG(level, ...) DLOG_F( level, __VA_ARGS__ )
#define FLUX_DEBUG_LOG_IF(level, cond, ...) DLOG_IF_F( level, cond, __VA_ARGS__ )

#define FLUX_LOG_SCOPE(level, ...) LOG_SCOPE_F( level, __VA_ARGS__ )
#define FLUX_LOG_SCOPE_FUNCTION(level) LOG_SCOPE_FUNCTION( level )

#define FLUX_VLOG(verb, ... ) VLOG_F( verb, __VA_ARGS__ )
#define FLUX_VLOG_IF(verb, cond, ... ) VLOG_IF_F( verb, cond, __VA_ARGS__ )

#define FLUX_DEBUG_VLOG(verb, ... ) DVLOG_F( verb, __VA_ARGS__ )
#define FLUX_DEBUG_VLOG_IF(verb, cond, ... ) DVLOG_IF_F( verb, cond, __VA_ARGS__ )

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_STD_LOG_HPP_D6BCB6DD_CAB5_43BC_B639_E1629BC39B7D
