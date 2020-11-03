/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Detect required language features
 *
 * (deprecated)
 *
 * Detect and setup the now-required `LANGFEAT_*` constants.
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_EXT_LANGFEAT_REQUIRED_HXX_13EA8B5F_2832_48A7_9404_8E4BB965F5F9
#define DETAIL_EXT_LANGFEAT_REQUIRED_HXX_13EA8B5F_2832_48A7_9404_8E4BB965F5F9

//--//////////////////////////////////////////////////////////////////////////
//--    Feature tests               ///{{{1///////////////////////////////////

/* Attempt to detect via the feature testing macros
 *
 * See
 *  - http://en.cppreference.com/w/cpp/experimental/feature_test
 *  - http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0096r3.html
 *  - http://en.cppreference.com/w/User:D41D8CD98F/feature_testing_macros
 */
#include "ext-langfeat_req_conf-feature.hxx"

//--    Language level              ///{{{1///////////////////////////////////

#if defined(FLUXCFG_LANGFEAT_FROM_LANGLEVEL) && FLUXCFG_LANGFEAT_FROM_LANGLEVEL
#	include "ext-langfeat_req_conf-langlevel.hxx"

#else
#	if FLUX_COMPILER_FAMILY == FLUX_COMPILER_GCC
#		include "ext-langfeat_req_conf-gcc.hxx"

#	elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_CLANG
#		include "ext-langfeat_req_conf-clang.hxx"

#	elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_MSVC
#		include "ext-langfeat_req_conf-msvc.hxx"

#	elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_PGI
#		include "ext-langfeat_req_conf-pgi.hxx"

#	else // Unknown or unlisted compiler
#		include "ext-langfeat_req_conf-langlevel.hxx"
#	endif // ~ FLUX_COMPILER_FAMILY

#	if FLUX_COMPILER_COMPAT == FLUX_COMPILER_NVCC
#		include "ext-langfeat_req_conf-nvcc.hxx"
#	endif // ~ FLUX_COMPILER_COMPAT

#endif // ~ FLUXCFG_LANGFEAT_FROM_LANGLEVEL

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_EXT_LANGFEAT_REQUIRED_HXX_13EA8B5F_2832_48A7_9404_8E4BB965F5F9
