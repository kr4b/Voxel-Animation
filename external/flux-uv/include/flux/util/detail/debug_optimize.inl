/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- Compiler optimization
 */
/*-***************************************************************** -}}}1- */

#include "../defaults.hpp"

//--//////////////////////////////////////////////////////////////////////////
//--    config                      ///{{{1///////////////////////////////////

#if FLUX_DEBUG
#	if defined(FLUXCFG_UTIL_NO_DEBUG_OPTIMIZE)
#		define FLUX_UTIL_DEBUG_OPTIMIZE (!FLUXCFG_UTIL_NO_DEBUG_OPTIMIZE)
#	else
#		define FLUX_UTIL_DEBUG_OPTIMIZE 1
#	endif // ~ FLUXCFG
#else // !FLUX_DEBUG
#	define FLUX_UTIL_DEBUG_OPTIMIZE 0
#endif // ~ FLUX_DEBUG

//--    GCC                         ///{{{1///////////////////////////////////
#if FLUX_UTIL_DEBUG_OPTIMIZE
#	if FLUX_COMPILER_FAMILY == FLUX_COMPILER_GCC
#		undef FLUX_DEBUG_OPTIMIZE_BEGIN
#		undef FLUX_DEBUG_OPTIMIZE_END

#		define FLUX_DEBUG_OPTIMIZE_BEGIN()        \
			_Pragma( "GCC push_options" )         \
			_Pragma( "GCC optimize \"-O2\"" )     \
		/*ENDM*/
#		define FLUX_DEBUG_OPTIMIZE_END()          \
			_Pragma( "GCC pop_options" )          \
		/*ENDM*/

//--    Clang                       ///{{{1///////////////////////////////////
#	elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_CLANG

/* Apparently only supports turning off optimization, but not turning them on */

//--    MSVC                        ///{{{1///////////////////////////////////
#	elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_MSVC
#		undef FLUX_DEBUG_OPTIMIZE_BEGIN
#		undef FLUX_DEBUG_OPTIMIZE_END

		/* In some quick tests, this doesn't seem to make any (or at least no
		 * major) difference. :-/
		 */

#		define FLUX_DEBUG_OPTIMIZE_BEGIN()        \
			__pragma( optimize( "gt", on ) )      \
		/*ENDM*/
#		define FLUX_DEBUG_OPTIMIZE_END()          \
			__pragma( optimize( "", on ) )        \
		/*ENDM*/

//--    unknown/unhandled           ///{{{1///////////////////////////////////
#	else // unknown / unhandled

/* Leave undefined*/

#	endif // ~ COMPILER_FAMILY
#endif // ~ FLUX_DEBUG

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
