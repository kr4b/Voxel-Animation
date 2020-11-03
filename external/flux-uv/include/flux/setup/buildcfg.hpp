/*-******************************************************* -- HEADER -{{{1- */
/*-	Build configuration
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SETUP_BUILDCFG_HPP_BB9819C3_D88E_4488_9305_80795B7607FF
#define FLUX_SETUP_BUILDCFG_HPP_BB9819C3_D88E_4488_9305_80795B7607FF

//--//////////////////////////////////////////////////////////////////////////
//--    Include                     ///{{{1///////////////////////////////////

#include "defaults.hpp"
#include "compiler.hpp"

//--    Build settings              ///{{{1///////////////////////////////////

// --{{{2-- FLUX_DEBUG
/** Enable debugging checks?
 *
 * `FLUX_DEBUG` determines wheter debug checks (e.g., `FLUX_ASSERT()`) should
 * be enabled or disabled. The value of `FLUX_DEBUG` is determined by
 *
 *   - `FLUXCFG_DEBUG` (if defined)
 *   - the presence of `NDEBUG` (non-MSVC compiler)
 *   - the presence of `_DEBUG` (MSVC)
 *
 * \warning `FLUX_DEBUG` is always defined - it's value determines whether or
 *  not to perform debug checks. I.e., the following is the correct way to
 *  use `FLUX_DEBUG`:
 *  \code
 *  #if FLUX_DEBUG
 *  // debug only code
 *  #endif // ~ FLUX_DEBUG
 *  \endcode
 *  The following is *incorrect*:
 *  \code
 *  #if defined(FLUX_DEBUG)
 *  // FLUX_DEBUG is always defined!
 *  #endif
 *  \endcode
 *  (it differs in this from both `NDEBUG` and `_DEBUG`)
 *
 * \see FLUXCFG_DEBUG
 */
#if defined(FLUXCFG_DEBUG)
/* If FLUXCFG_DEBUG is defined, then we use its value.
 */
#	define FLUX_DEBUG FLUXCFG_DEBUG
#else
/* MSVC defines _DEBUG in debug builds, but apparently nothing in release
 * builds. Note: normalize.hpp makes sure that NDEBUG is always defined in
 * release builds nevertheless.
 */
#	if FLUX_COMPILER_FAMILY == FLUX_COMPILER_MSVC
#		if defined(_DEBUG)
#			define FLUX_DEBUG 1
#		else
#			define FLUX_DEBUG 0
#		endif
#	else // !FLUX_COMPILER_MSVC
/* Non-MSVC environments use the portable NDEBUG to mark "release" builds.
 */
#		if defined(NDEBUG)
#			define FLUX_DEBUG 0
#		else
#			define FLUX_DEBUG 1
#		endif 
#	endif // ~ !FLUX_COMPILER_MSVC

#endif // ~ !FLUXCFG_DEBUG

/* Dummy check: is FLUX_DEBUG still undefined? If so, define it (enabled by
 * default).
 */
#if !defined(FLUX_DEBUG)
#	define FLUX_DEBUG 1
#endif // ~ !defined(FLUX_DEBUG)

//--    Enabled components          ///{{{1///////////////////////////////////

#if defined(FLUXCFG_WITH_FLUXGL)
#	define FLUX_WITH_FLUXGL FLUXCFG_WITH_FLUXGL
#else // !defined(FLUXCFG_WITH_FLUXGL)
#	define FLUX_WITH_FLUXGL 1
#endif // ~ defined(FLUXCFG_WITH_FLUXGL)

#if defined(FLUXCFG_WITH_FLUXIO)
#	define FLUX_WITH_FLUXIO FLUXCFG_WITH_FLUXIO
#else // !defined(FLUXCFG_WITH_FLUXIO)
#	define FLUX_WITH_FLUXIO 1
#endif // ~ defined(FLUXCFG_WITH_FLUXIO)

#if defined(FLUXCFG_WITH_FLUXVK)
#	define FLUX_WITH_FLUXVK FLUXCFG_WITH_FLUXVK
#else // !defined(FLUXCFG_WITH_FLUXVK)
#	define FLUX_WITH_FLUXVK 1
#endif // ~ defined(FLUXCFG_WITH_FLUXVK)

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_SETUP_BUILDCFG_HPP_BB9819C3_D88E_4488_9305_80795B7607FF
