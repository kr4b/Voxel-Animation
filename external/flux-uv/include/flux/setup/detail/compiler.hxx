/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Detect and setup compiler
 *
 * Detect and setup FLUX_COMPILER* and related constants.
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_COMPILER_HXX_BFB80F9F_13F9_4A18_979B_74BE1A729FA6
#define DETAIL_COMPILER_HXX_BFB80F9F_13F9_4A18_979B_74BE1A729FA6

//--//////////////////////////////////////////////////////////////////////////
//--    Detect compiler             ///{{{1///////////////////////////////////

/* Detect main compiler. 
 *
 * Note: order matters. For instance, clang defines both __clang__ and
 * __GNUC__.
 */

#if defined(__PGI)
#	include "compiler_conf-pgi.hxx"
#elif defined(__INTEL_COMPILER)
#	include "compiler_conf-intel.hxx"
#elif defined(__clang__)
#	include "compiler_conf-clang.hxx"
#elif defined(__GNUC__)
#	include "compiler_conf-gcc.hxx"
#elif defined(_MSC_VER)
#	include "compiler_conf-msvc.hxx"
#endif // ~ <compiler>

/* Detect NVCC
 *
 * If we're using NVCC, we'll have to change around some of the constants.
 */
#if defined(__CUDACC__)
#	include "compiler_conf-nvcc.hxx"
#endif // ~ defined(__CUDACC__)

//--    Detect user defaults        ///{{{1///////////////////////////////////

/* If FLUXCFG_COMPILER_COMPAT is defined, ensure that FLUX_COMPILER_COMPAT is
 * equal to this value.
 */
#if defined(FLUXCFG_COMPILER_COMPAT)
#	undef FLUX_COMPILER_COMPAT
#	define FLUX_COMPILER_COMPAT FLUXCFG_COMPILER_COMPAT
#endif // ~ defined(FLUXCFG_COMPILER_COMPAT)

/* If FLUXCFG_LANGLEVEL is defined, use that instead of whatever we've 
 * detected automatically.
 */
#if defined(FLUXCFG_LANGLEVEL)
#	undef FLUX_LANGLEVEL
#	define FLUX_LANGLEVEL FLUXCFG_LANGLEVEL
#endif // ~ defined(FLUXCFG_LANGLEVEL)

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_COMPILER_HXX_BFB80F9F_13F9_4A18_979B_74BE1A729FA6
