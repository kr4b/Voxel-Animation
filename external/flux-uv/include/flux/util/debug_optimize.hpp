/*-******************************************************* -- HEADER -{{{1- */
/*-	Selectively enable optimization in debug builds.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_UTIL_DEBUG_OPTIMIZE_HPP_4B9DD18F_4E17_4380_89EA_E248DB953802
#define FLUX_UTIL_DEBUG_OPTIMIZE_HPP_4B9DD18F_4E17_4380_89EA_E248DB953802

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/compiler.hpp>
#include <flux/setup/normalize.hpp>
#include <flux/setup/buildcfg.hpp>

//--    macros                      ///{{{1///////////////////////////////////

/** Debug: enable compiler optimizations
 *
 * Enable compiler optimizations in debug mode. Use `FLUX_DEBUG_OPTIMIZE_END()`
 * to restore normal compiler settings. Ignored outside of debug (`FLUX_DEBUG`)
 * mode. Can only be used in a top-level context.
 *
 * \note The main purpose of this is to enable some basic optimizations in
 * well-known third-party code that is included in the flux build. One such
 * example is the `miniz.h` library, where this improves up performance
 * quite significantly.
 *
 * This feature is not supported by all compilers.
 *
 * `FLUX_DEBUG_OPTIMIZE_BEGIN()` and `FLUX_DEBUG_OPTIMIZE_END()` can be
 * disabled by defining `FLUXCFG_UTIL_NO_DEBUG_OPTIMIZE` to `1` prior to
 * inclusion of this header (e.g., via the command line).
 */
#define FLUX_DEBUG_OPTIMIZE_BEGIN()      /*IMPLEMENTED_ELSEWHERE*/
/** Debug: restore normal compiler settings
 *
 * Restore normal compiler settings after these have been changed via
 * `FLUX_DEBUG_OPTIMIZE_BEGIN()`.
 */
#define FLUX_DEBUG_OPTIMIZE_END()        /*IMPLEMENTED_ELSEWHERE*/

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/debug_optimize.inl"
#endif // FLUX_UTIL_DEBUG_OPTIMIZE_HPP_4B9DD18F_4E17_4380_89EA_E248DB953802
