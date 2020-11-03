/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Detect and setup platform
 *
 * Detect and setup FLUX_PLATFORM* and related constants.
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_PLATFORM_HXX_DCF726B0_8F4B_4C86_B535_0D0FB9217914
#define DETAIL_PLATFORM_HXX_DCF726B0_8F4B_4C86_B535_0D0FB9217914

//--//////////////////////////////////////////////////////////////////////////
//--    Detect platform             ///{{{1///////////////////////////////////

#if defined(FLUXCFG_PLATFORM)
/* FLUXCFG_PLATFORM defined. Use its value
 */
#	undef FLUX_PLATFORM
#	define FLUX_PLATFORM FLUXCFG_PLATFORM

#else // !defined(FLUXCFG_PLATFORM)
/* FLUXCFG_PLATFORM undefined. Guess the appropriate settings. See
 * http://sourceforge.net/p/predef/wiki/OperatingSystems/
 */

#	if defined(__ANDROID__)
#		undef FLUX_PLATFORM
#		define FLUX_PLATFORM FLUX_PLATFORM_ANDROID

#	elif defined(__APPLE__) || defined(__MACH__)
#		undef FLUX_PLATFORM
#		define FLUX_PLATFORM FLUX_PLATFORM_MACOS

#	elif defined(_WIN32) || defined(_WIN64)
#		undef FLUX_PLATFORM
#		define FLUX_PLATFORM FLUX_PLATFORM_WIN32

#	elif defined(__linux__) || defined(__CYGWIN__)
#		undef FLUX_PLATFORM
#		define FLUX_PLATFORM FLUX_PLATFORM_LINUX

#	endif // ~ guess
#endif // ~ defined(FLUXCFG_PLATFORM)

//--    Detect C++ Stdlib           ///{{{1///////////////////////////////////

#if defined(FLUXCFG_STDLIB)
/* FLUXCFG_STDLIB defined. Use that
 */
#	undef FLUX_STDLIB
#	define FLUX_STDLIB FLUXCFG_STDLIB

#else // !defined(FLUXCFG_STDLIB)
/* FLUXCFG_STDLIB undefined. Guess the appropriate settings. This is somewhat
 * icky. See following for details:
 *  - http://clang-developers.42468.n3.nabble.com/Best-way-to-detect-libc-at-compile-time-td4025578.html
 *  - <boost/config/select_stdlib_config.hpp>
 */

#	if FLUX_COMPILER_FAMILY == FLUX_COMPILER_MSVC
/* We're on MSVC, so guess that we're using MSVC's stdlib too.
 */
#		undef FLUX_STDLIB
#		define FLUX_STDLIB FLUX_STDLIB_VSCPP

#	else // !MSVC
/* <ciso646> is defined to do nothing (the operators it defines are already in
 * the core C++ language). However, it leaks _LIBCPP_VERSION in the case of
 * libc++, and __GLIBCXX__ for newer versions of libstdc++ (e.g., for GCC 6.4
 * and newer). For older libstdc++, we need to include something like
 * <cstddef>.
 *
 * Hack/fix: don't do this if __cplusplus isn't defined; we can then include 
 * this header from pure C as well.
 */
#		if defined(__cplusplus)
#			include <ciso646>

#			if defined(_LIBCPP_VERSION)
#				undef FLUX_STDLIB
#				define FLUX_STDLIB FLUX_STDLIB_LIBCPP
#			elif defined(__GLIBCXX__)
#				undef FLUX_STDLIB
#				define FLUX_STDLIB FLUX_STDLIB_STDCPP
#			else /* Only include <cstddef> as a final resort. */
#				include <cstddef>
#				if defined(__GLIBCPP__) || defined(__GLIBCXX__)
#					undef FLUX_STDLIB
#					define FLUX_STDLIB FLUX_STDLIB_STDCPP
#				endif
#			endif
#		endif // ~ defined(__cplusplus)
#	endif // ~ COMPILER_FAMILY
#endif // ~ defined(FLUXCFG_STDLIB)

//--    Detect capabilities         ///{{{1///////////////////////////////////

/* Detect Win32Lib
 */
#if defined(FLUXCFG_PLATCAP_WIN32LIB)
#	undef FLUX_PLATCAP_WIN32LIB
#	define FLUX_PLATCAP_WIN32LIB FLUXCFG_PLATCAP_WIN32LIB

#else // !defined(FLUXCFG_PLATCAP_WIN32LIB)

#	if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
#		undef FLUX_PLATCAP_WIN32LIB
#		define FLUX_PLATCAP_WIN32LIB 1
#	endif // ~ PLATFORM == WIN32

#endif // ~ defined(FLUXCFG_PLATCAP_WIN32LIB)

/* Detect Cygiwin
 */
#if defined(FLUXCFG_PLATCAP_CYGWIN)
#	undef FLUX_PLATCAP_CYGWIN
#	define FLUX_PLATCAP_CYGWIN FLUXCFG_PLATCAP_CYGWIN

#else
#	if defined(__CYGWIN__)
#		undef FLUX_PLATCAP_CYGWIN
#		define FLUX_PLATCAP_CYGWIN 1
#	endif // ~ (cygwin build)

#endif // ~ defined(FLUXCFG_PLATCAP_CYGWIN)

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_PLATFORM_HXX_DCF726B0_8F4B_4C86_B535_0D0FB9217914
