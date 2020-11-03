/*-******************************************************* -- HEADER -{{{1- */
/*-	flux.util configuration
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_UTIL_DEFAULTS_HPP_5111A9E4_F158_468C_8524_8AFD224D7136
#define FLUX_UTIL_DEFAULTS_HPP_5111A9E4_F158_468C_8524_8AFD224D7136

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

//--    defaults                    ///{{{1///////////////////////////////////

/** Config: disable `FLUX_HINT_LIKELY` and `FLUX_HINT_UNLIKELY`
 *
 * Specifies whether the macros `FLUX_HINT_LIKELY()`, `FLUX_HINT_UNLIKELY()`
 * and `FLUX_HINT_UNPREDICTABLE` should be ignored.
 * `FLUXCFG_UTIL_IGNORE_LIKELY` is disabled by default, meaning that the macros
 * are active.
 */
#if !defined(FLUXCFG_UTIL_IGNORE_LIKELY)
#	define FLUXCFG_UTIL_IGNORE_LIKELY 0
#endif // ~ !defined(FLUXCFG_UTIL_IGNORE_LIKELY)

/** Config: disable `FLUX_UTIL_DEBUG_OPTIMIZE_BEGIN`/`FLUX_UTIL_DEBUG_OPTIMIZE_END`
 *
 * Specifies whether the macros `FLUX_UTIL_DEBUG_OPTIMIZE_BEGIN()` and
 * `FLUX_UTIL_DEBUG_OPTIMIZE_END()` should be ignored.
 * `FLUXCFG_UTIL_NO_DEBUG_OPTIMIZE` is diabled by default, meaning that debug
 * optimization is enabled in debug builds.
 *
 * \note `FLUXCFG_UTIL_NO_DEBUG_OPTIMIZE` only affects debug builds. The above
 * macros are always ignored in non-debug builds (determined by `FLUX_DEBUG`).
 */
#if !defined(FLUXCFG_UTIL_NO_DEBUG_OPTIMIZE)
#	define FLUXCFG_UTIL_NO_DEBUG_OPTIMIZE 0
#endif // ~ !defined(FLUXCFG_UTIL_NO_DEBUG_OPTIMIZE)

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_UTIL_DEFAULTS_HPP_5111A9E4_F158_468C_8524_8AFD224D7136
