/*-******************************************************* -- HEADER -{{{1- */
/*-	Platform identification and basic capabilities
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SETUP_PLATFORM_HPP_C3505207_C8A1_420C_9644_FCAA45B2E817
#define FLUX_SETUP_PLATFORM_HPP_C3505207_C8A1_420C_9644_FCAA45B2E817

//--//////////////////////////////////////////////////////////////////////////
//--    Include                     ///{{{1///////////////////////////////////

#include "defaults.hpp"
#include "compiler.hpp"

//--    Platform                    ///{{{1///////////////////////////////////

#define FLUX_PLATFORM_UNKNOWN         0  //!< unknown platform
#define FLUX_PLATFORM_LINUX           1  //!< platform is linux (-like)
#define FLUX_PLATFORM_MACOS           2  //!< platform is macos (X, not classic)
#define FLUX_PLATFORM_WIN32           3  //!< platform is win32
#define FLUX_PLATFORM_ANDROID         4  //!< platform is android

/** Target platform
 *
 * `FLUX_PLATFORM` determines the target platform. This is used to guess the
 * capabilities (see `FLUX_PLATCAP_*`) of that platform, if these are not
 * provided externally.
 *
 * The value of `FLUX_PLATFORM` is derived from
 *   - `FLUXCFG_PLATFORM` (if defined)
 *   - default pre-defined macros (otherwise)
 *
 * \see FLUXCFG_PLATFORM
 */
#define FLUX_PLATFORM          FLUX_PLATFORM_UNKNOWN /*OVERRIDDEN_ELSEWHERE*/

//--    C++ Standard Library        ///{{{1///////////////////////////////////

#define FLUX_STDLIB_UNKNOWN           0  //!< unknown standard library
#define FLUX_STDLIB_STDCPP            1  //!< GNU libstdc++
#define FLUX_STDLIB_LIBCPP            2  //!< libc++ (clang)
#define FLUX_STDLIB_VSCPP             3  //!< whatever we get from VisualStudio

#define FLUX_STDLIB            FLUX_STDLIB_UNKNOWN /*OVERRIDDEN_ELSEWHERE*/

//--    Capabilities                ///{{{1///////////////////////////////////

/** Win32 API present?
 *
 * Determines if the Win32 API is available and may be used. By default, the
 * value of `FLUX_PLATCAP_WIN32LIB` is derived from `FLUX_PLATFORM`, i.e., 
 * it is automatically enabled for Win32 targets. The setting may be overridden
 * with `FLUXCFG_PLATCAP_WIN32LIB`.
 *
 * \see FLUXCFG_PLATCAP_WIN32LIB
 */
#define FLUX_PLATCAP_WIN32LIB         0 /*OVERRIDDEN_ELSEWHERE*/

/** Are we building for/on Cygwin?
 *
 * Cygwin has a few special quirks and incompatibilities when compared to a
 * normal Linux environment. The value of `FLUX_PLATCAP_CYGWIN` is derived
 * from the predefined `__CYGWIN__` macro, unless manually overridden with
 * `FLUXCFG_PLATCAP_CYGWIN`.
 */
#define FLUX_PLATCAP_CYGWIN           0 /*OVERRIDDEN_ELSEWHERE*/

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/platform.hxx"
#endif // FLUX_SETUP_PLATFORM_HPP_C3505207_C8A1_420C_9644_FCAA45B2E817
