/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Detect and setup language features
 *
 * Detect and setup FLUX_LANGFEAT_* and related constants.
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_LANGFEAT_HXX_6C01FE42_06DC_461A_9767_D12E016B16ED
#define DETAIL_LANGFEAT_HXX_6C01FE42_06DC_461A_9767_D12E016B16ED

//--//////////////////////////////////////////////////////////////////////////
//--    Feature tests               ///{{{1///////////////////////////////////

/* Attempt to detect via the feature testing macros
 *
 * See
 *  - http://en.cppreference.com/w/cpp/experimental/feature_test
 *  - http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0096r3.html
 *  - http://en.cppreference.com/w/User:D41D8CD98F/feature_testing_macros
 */
#include "langfeat_conf-feature.hxx"

//--    Detection                   ///{{{1///////////////////////////////////
#if !defined(FLUXCFG_LANGFEAT_FROM_FEATURE_ONLY) || !FLUXCFG_LANGFEAT_FROM_FEATURE_ONLY
#	if defined(FLUXCFG_LANGFEAT_FROM_LANGLEVEL) && FLUXCFG_LANGFEAT_FROM_LANGLEVEL
/* If FLUXCFG_LANGFEAT_FROM_LANGLEVEL is defined and enabled, always get the
 * language features from the language level.
 */
#		include "langfeat_conf-langlevel.hxx"

#	else
/* Otherwise, determine these from the "base" compiler.
 */
#		if FLUX_COMPILER_FAMILY == FLUX_COMPILER_GCC
#			include "langfeat_conf-gcc.hxx"
#		elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_CLANG
#			include "langfeat_conf-clang.hxx"
#		elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_MSVC
#			include "langfeat_conf-msvc.hxx"
#		elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_PGI
#			include "langfeat_conf-pgi.hxx"
#		elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_INTEL
			// TODO: langfeat_conf for INTEL
#			include "langfeat_conf-langlevel.hxx"

#		elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_UNKNOWN
/* If the compiler is unknown, always determine supported features from the
 * language level.
 */
#			include "langfeat_conf-langlevel.hxx"

#		else // sanity check
#			error "FLUX_COMPILER_FAMILY: unknown value"
#		endif // ~ FLUX_COMPILER_FAMILY

/* Let NVCC override the values. 
 */
#		if FLUX_COMPILER_COMPAT == FLUX_COMPILER_NVCC
#			include "langfeat_conf-nvcc.hxx"
#		endif // ~ FLUX_COMPILER_COMPAT

#	endif // ~ FLUXCFG_LANGFEAT_FROM_LANGLEVEL
#endif // ~ FLUXCFG_LANGFEAT_FROM_FEATURE_ONLY

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_LANGFEAT_HXX_6C01FE42_06DC_461A_9767_D12E016B16ED
