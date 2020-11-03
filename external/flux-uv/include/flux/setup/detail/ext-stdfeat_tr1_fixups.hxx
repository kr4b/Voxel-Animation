/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Apply fixups and workarounds for TR1
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_EXT_STDFEAT_TR1_FIXUPS_HXX_0F280BE1_2497_4A6D_B3B2_CF0B933BD919
#define DETAIL_EXT_STDFEAT_TR1_FIXUPS_HXX_0F280BE1_2497_4A6D_B3B2_CF0B933BD919

#include "../arch.hpp"

//--//////////////////////////////////////////////////////////////////////////
//--    libstdc++                   ///{{{1///////////////////////////////////
#if FLUX_STDLIB == FLUX_STDLIB_STDCPP
/* <tr1/random> likely contains broken implementations of
 *   - normal_distribution
 *   - poisson_distribution
 *   - binomial_distribution
 *
 * Observed in __GLIBCXX__
 * 	- from at least 20101001
 * 	- to at least 20141030
 * (don't know outside of these values)
 */
#	undef FLUX_STDQUIRK_TR1_RANDOM_BROKEN_DISTS
#	define FLUX_STDQUIRK_TR1_RANDOM_BROKEN_DISTS 1
#endif // ~ libstdc++

//--    GCC                         ///{{{1///////////////////////////////////
#if FLUX_COMPILER_FAMILY == FLUX_COMPILER_GCC
/* GCC 8.x (and later, presumably) do no longer support the TR1 cmath
 * header in C++17 mode.
 *
 * <tr1/random> under GCC includes <tr1/cmath>, so that too blows up.
 *
 * TODO: verify GCC 8.0. Tested for GCC 8.1.
 */
#	if __GNUC__ > 8 || (__GNUC__ == 8 && __GNUC_MINOR__ >= 0)
#		if __cplusplus >= 201703
#			undef FLUX_STDFEAT_TR1_RANDOM
#			undef FLUX_STDFEAT_TR1_EXTRAMATH

#			define FLUX_STDFEAT_TR1_RANDOM 0
#			define FLUX_STDFEAT_TR1_EXTRAMATH 0
#		endif // ~ C++17 / C++1z
#	endif // GCC 8.x
#endif // ~ COMPILER_FAMILY == GCC

//--    Clang                       ///{{{1///////////////////////////////////
#if FLUX_COMPILER_FAMILY == FLUX_COMPILER_CLANG
/* Some clang before 3.5 would have trouble with parts of the GNU libstd++; 
 * disable the parts that were found to be problematic.
 */
#	if __clang_major__ < 3 || (__clang_major__ == 3 && __clang_minor__ < 5)
#		if FLUX_STDLIB == FLUX_STDLIB_STDCPP
/* <tr1/cmath> from the GNU libstdc++ causes compiler errors.
 */
#			undef FLUX_STDFEAT_TR1_EXTRAMATH
#			define FLUX_STDFEAT_TR1_EXTRAMATH 0
/* <tr1/random> implicitly includes <tr1/cmath>, so disable that too.
 */
#			undef FLUX_STDFEAT_TR1_RANDOM
#			define FLUX_STDFEAT_TR1_RANDOM 0
#		endif // ~ STDLIB == STDCPP
#	endif // __clang__ version

#endif // ~ COMPILER_FAMILY == Clang

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_EXT_STDFEAT_TR1_FIXUPS_HXX_0F280BE1_2497_4A6D_B3B2_CF0B933BD919
