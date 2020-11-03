/*-******************************************************* -- HEADER -{{{1- */
/*-	Default configuration
 *
 * Specifies default values for flux-wide configurable items. Custom values
 * can be specified by
 *
 *   - defining `FLUXCFG_USER_HEADER` to an #includable header file
 *   - specifying `FLUXCFG_*` definitions via the compiler command line
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SETUP_DEFAULTS_HPP_BFA75051_8C93_4F7D_92CE_768419CEC46F
#define FLUX_SETUP_DEFAULTS_HPP_BFA75051_8C93_4F7D_92CE_768419CEC46F

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include "detail/defaults-deploy.hxx"

#if defined(FLUXCFG_USER_HEADER)
#	include FLUXCFG_USER_HEADER
#endif // ~ defined(FLUXCFG_USER_HEADER)

//--    default config              ///{{{1///////////////////////////////////

	// --{{{3-- Namespacing
/** Config: custom version-namespace
 * 
 * The outer `flux` namespace encompasses all of flux. If inline namespaces are
 * supported, this namespace mainly contains a second inline namespace named
 * after the current version (`FLUX_NAMESPACE_CURRENT`) by default. This option
 * overrides the default name of this second namespace.
 *
 * The purpose of this option is to support the somewhat odd situation where
 * two different versions of flux are used concurrently.
 */
//#define FLUXCFG_NAMESPACE_CURRENT FLUX_CURRENT_VERSION
#if defined(__DOXYGEN__)
#	define FLUXCFG_NAMESPACE_CURRENT   UNDEFINED
#endif

	// --{{{3-- Debugging
/** Config: Debug build
 *
 * Specify if this a debug build, i.e., extra checks (such as `asserts()`)
 * should be compiled in.
 *
 * By default, this is determined from the environment (via `NDEBUG` and/or
 * `_DEBUG`). Setting `FLUXCFG_DEBUG` will override the default value.
 *
 * - enable debug mode ("debug build"): define to 1
 * - enable debug mode ("release build"): define to 0
 * - determine based on environment: leave undefined
 *
 * \see FLUX_DEBUG
 */
//#define FLUXCFG_DEBUG 0
//#define FLUXCFG_DEBUG 1

#if defined(__DOXYGEN__)
#	define FLUXCFG_DEBUG    UNDEFINED
#endif

	// --{{{3-- Compiler
/** Config: Specify target "compatible" compiler
 *
 * See `FLUX_COMPILER_COMPAT` for further information. The value of 
 * `FLUX_COMPILER_COMPAT` is set equal to `FLUXCFG_COMPILER_COMPAT` if the
 * latter has been defined. Otherwise it is derived from `FLUX_COMPILER_ACTUAL`.
 *
 * - specify target compiler: set to one of the `FLUX_COMPILER_*` constants
 * - use default value: leave undefined
 *
 * \see FLUX_COMPILER_COMPAT
 */
//#define FLUXCFG_COMPILER_COMPAT FLUX_COMPILER_UNKNOWN

#if defined(__DOXYGEN__)
#	define FLUXCFG_COMPILER_COMPAT   UNDEFINED
#endif

/** Config: Specify the compiler's language level
 *
 * If defined, sets the value of `FLUX_LANGLEVEL`. If not defined, the value is
 * derived from the compiler environment.
 *
 *  - specify level: define to `FLUX_LANGLEVEL_*`
 *  - determine automatically: leave undefined
 *
 * \see FLUX_LANGLEVEL
 */
//#define FLUXCFG_LANGLEVEL FLUX_LANGLEVEL_CXX11

#if defined(__DOXYGEN__)
#	define FLUXCFG_LANGLEVEL   UNDEFINED
#endif

	// --{{{3-- Architecture
/** Config: Target architecture
 *
 * Choose target `FLUX_ARCH`. By default (when `FLUXCFG_ARCH` is left
 * undefined), the platform is guessed from compiler pre-defined macros.
 *
 * - specify target arch: set to one of the `FLUX_ARCH_*` constants
 * - use default value: leave undefined
 */
//#define FLUXCFG_ARCH FLUXCFG_ARCH_X86_64

#if defined(__DOXYGEN__)
#	define FLUXCFG_ARCH   UNDEFINED
#endif

	// --{{{3-- Platform
/** Config: Target platform
 *
 * Choose target `FLUX_PLATFORM`. By default (when `FLUXCFG_PLATFORM` is left
 * undefined), the platform is guessed from compiler pre-defined macros.
 *
 * - specify target platform: set to one of the `FLUX_PLATFORM_*` constants
 * - use default value: leave undefined
 */
//#define FLUXCFG_PLATFORM FLUX_PLATFORM_UNKNOWN

#if defined(__DOXYGEN__)
#	define FLUXCFG_PLATFORM   UNDEFINED
#endif

/** Config: C++ Standard Library
 *
 * Define the C++ Standard library available for this target. By default, this
 * is guessed from a combination of the compiler that we're using and 
 * pre-defined macros (possibly via included<> headers).
 *
 *  - specify C++ Standard library: set to one of the `FLUX_STDLIB_*`constants
 *  - use default value: leave undefined
 */
//#define FLUX_STDLIB FLUX_STDLIB_UNKNOWN

#if defined(__DOXYGEN__)
#	define FLUXCFG_STDLIB   UNDEFINED
#endif

/** Config: Win32 API available?
 *
 * Allow flux to use stuff from the Win32. By default, the Win32 API is
 * assumed to be present under Windows targets, and not otherwise.
 *
 * - enable Win32 stuff: define to 1
 * - disable Win32 stuff: define to 0
 * - detect automatically: leave undefined
 *
 * \todo Currently not used very consistently.
 */
//#define FLUXCFG_PLATCAP_WIN32LIB 0

#if defined(__DOXYGEN__)
#	define FLUXCFG_PLATCAP_WIN32LIB   UNDEFINED
#endif // ~ __DOXYGEN__

/** Config: Build for/under Cygwin?
 *
 * Controls whether or not Cygwin-specific workarounds are enabled.
 *
 * - enable Cygwin workarounds: define to 1
 * - disable Cygwin workarounds: define to 0
 * - detect automatically: leave undefined

 * \todo Currently not used very consistently.
 */
//#define FLUXCFG_PLATCAP_ 0

#if defined(__DOXYGEN__)
#	define FLUXCFG_PLATCAP_CYGWIN   UNDEFINED
#endif // ~ __DOXYGEN__

	// --{{{3-- Endian
/** Config: Target platform endian
 *
 * Choose target `FLUX_ENDIAN`. By default (when `FLUXCFG_ENDIAN` is 
 * left undefined), the platform's endianness is guessed from compiler 
 * pre-defined macros and platform headers.
 *
 * - specify target platform endian: set to one of the `FLUX_ENDIAN_*` 
 *   	constants
 * - guess value: leave undefined
 */
//#define FLUXCFG_ENDIAN FLUX_ENDIAN_UNKNOWN

#if defined(__DOXYGEN__)
#	define FLUXCFG_ENDIAN   UNDEFINED
#endif

	// --{{{3-- Language & Standard Library features
/** Config: detect language features from feature testing macros only
 *
 * Normally, language features (`FLUX_LANGFEAT_*`) are determined by first
 * checking feature testing macros, and then inspecting either the language
 * level (if `FLUXCFG_LANGFEAT_FROM_LANGLEVEL` is set) or depending on the
 * value of `FLUX_COMPILER`. This flag disables the second part and forces
 * checks to only use the feature testing macros.
 *
 * \warning Some `FLUX_LANGFEAT_*` cannot be detected via feature testing
 * macros at the moment.
 */
//#define FLUXCFG_LANGFEAT_FROM_FEATURE_ONLY   1

#if defined(__DOXYGEN__)
#	define FLUXCFG_LANGFEAT_FROM_FEATURE_ONLY   UNDEFINED
#endif

/** Config: Force language features to match langlevel
 *
 * Normally, language features (`FLUX_LANGFEAT_*`) are determined from the 
 * value of `FLUX_COMPILER`. If `FLUXCFG_LANGFEAT_FROM_LANGLEVEL` is defined
 * and enabled, the values of `FLUX_LANGFEAT_*` are instead determined by
 * `FLUX_LANGLEVEL`.
 *
 * \see FLUX_LANGLEVEL
 */
//#define FLUXCFG_LANGFEAT_FROM_LANGLEVEL 0

#if defined(__DOXYGEN__)
#	define FLUXCFG_LANGFEAT_FROM_LANGLEVEL   UNDEFINED
#endif

/** Config: Force standard library features to match langlevel
 *
 * Normally, standard library features (`FLUX_STDFEAT_*`) are determined from
 * the value of `FLUX_STDLIB` and the likes. If
 * `FLUXCFG_STDFEAT_FROM_LANGLEVEL` is defined and enabled, the values of
 * `FLUX_STDFEAT_*` are instead determined by `FLUX_LANGLEVEL`.
 *
 * \see FLUX_LANGLEVEL
 */
//#define FLUXCFG_STDFEAT_FROM_LANGLEVEL 0

#if defined(__DOXYGEN__)
#	define FLUXCFG_STDFEAT_FROM_LANGLEVEL   UNDEFINED
#endif

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_SETUP_DEFAULTS_HPP_BFA75051_8C93_4F7D_92CE_768419CEC46F
