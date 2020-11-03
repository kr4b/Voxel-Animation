/*-******************************************************* -- HEADER -{{{1- */
/*-	Normalize compiler and platform settings
 *
 * Normalize compiler and platform enviromnent somewhat by defining common
 * macros (e.g. NDEBUG) across platforms, and by enabling/disabling certain
 * platform specific features automatically (e.g., NOMINMAX).
 *
 * Also, ¤!"#$€€¡@£€ MSVC crt secure deprecation warnings with a rusty $¥¡@]¥.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SETUP_NORMALIZE_HPP_6DBABC68_E8E3_4818_948A_9B14D754A880
#define FLUX_SETUP_NORMALIZE_HPP_6DBABC68_E8E3_4818_948A_9B14D754A880

//--//////////////////////////////////////////////////////////////////////////
//--    Include                     ///{{{1///////////////////////////////////

#include "defaults.hpp"
#include "buildcfg.hpp"
#include "compiler.hpp"
#include "platform.hpp"

//--    --- exclude from doxygen ---            ///{{{1///////////////////////
#if !defined(__DOXYGEN__) // There's nothing here for Doxygen.
//--    NDEBUG                      ///{{{2///////////////////////////////////

#if !FLUX_DEBUG && !defined(NDEBUG)
#	define NDEBUG 1
#endif

//--    _DEBUG                      ///{{{2///////////////////////////////////

#if FLUX_DEBUG && !defined(_DEBUG)
#	define _DEBUG 1
#endif

//--    MSVC Annoyances             ///{{{2///////////////////////////////////
#if FLUX_COMPILER == FLUX_COMPILER_MSVC

#	if !defined(_CRT_SECURE_NO_WARNINGS)
	// Suppress annoying warnings that MSVC emits whenever certain standard
	// functions are used.
#		define _CRT_SECURE_NO_WARNINGS 1
#	endif // ~ _CRT_SECURE_NO_WARNINGS

#	if !defined(_SCL_SECURE_NO_WARNINGS)
	// Suppress annoying warnings that MSVC emits whenever certain standard
	// C++ functions are used.
#		define _SCL_SECURE_NO_WARNINGS 1
#	endif // ~ _SCL_SECURE_NO_WARNINGS:

#endif // ~ FLUX_COMPILER_MSVC

//--    Windows Annoyances          ///{{{2///////////////////////////////////
#if FLUX_PLATFORM == FLUX_PLATFORM_WIN32

// Get rid of the terrible min() and max() macros in <windows.h>.
#	if !defined(NOMINMAX)
#		define NOMINMAX 1
#	endif // ~ defined(NOMINMAX)

#endif // ~ FLUX_PLATFORM_WIN32

//--    Cygwin Annoyance s          ///{{{2///////////////////////////////////
#if FLUX_PLATCAP_CYGWIN
// Not sure about why this isn't defined when building under Cygwin. But its
// absence creates problems in a few places (like <fts.h>) because of missing
// typedefs.
#	define _DEFAULT_SOURCE 1
#endif // ~ Cygwin

//--    NVCC Fixups                 ///{{{2///////////////////////////////////
#if FLUX_COMPILER_ACTUAL == FLUX_COMPILER_NVCC

/* NVCC is going to encounter unknown #pragma:s (from GCC & co), so let's 
 * tell NVCC to ignore those.
 */

#	if FLUX_COMPILER_FAMILY == FLUX_COMPILER_GCC
#		pragma diag_suppress = unrecognized_gcc_pragma

#	elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_CLANG
#		pragma diag_suppress = unrecognized_pragma

#	elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_MSVC
#		error TODO
#	endif // ~ compiler family

#endif // ~ NVCC

//--    --- end exclude from doxygen ---        ///}}}1///////////////////////
#endif // ~ !defined(__DOXYGEN__)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_SETUP_NORMALIZE_HPP_6DBABC68_E8E3_4818_948A_9B14D754A880
