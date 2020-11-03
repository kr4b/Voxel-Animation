/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Apply fixups and workarounds for NVCC when under TR1
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_EXT_STDFEAT_TR1_FIXUPS_NVCC_HXX_8FC939B9_899F_490D_BD36_F06FE595F9F5
#define DETAIL_EXT_STDFEAT_TR1_FIXUPS_NVCC_HXX_8FC939B9_899F_490D_BD36_F06FE595F9F5

//--//////////////////////////////////////////////////////////////////////////
//--    NVCC Version                ///{{{1///////////////////////////////////

/* Get CUDART_VERSION. This is the closest to a compiler version that we can
 * get. (It should be equal to CUDA_VERSION, though.)
 */
#	include <cuda_runtime_api.h> 

//--    Fixups: GNU libstdc++       ///{{{1///////////////////////////////////
#if FLUX_STDLIB == FLUX_STDLIB_STDCPP

/* NOTE - the headers below must work when compiled as CUDA sources. I.e.,
 * either when included from a file with .cu extension, or with nvcc invoked
 * as `nvcc -x cu`!
 */

/* <tr1/cmath> seems permanently broken (various mathy functions are already
 * defined. <tr1/random> implicitily includes <tr1/cmath>, so it breaks too.
 *
 * *Update:* This is true even under NVCC 8.0. Will probably never be fixed.
 */
#	undef FLUX_STDFEAT_TR1_EXTRAMATH
#	define FLUX_STDFEAT_TR1_EXTRAMATH 0

#	undef FLUX_STDFEAT_TR1_RANDOM
#	define FLUX_STDFEAT_TR1_RANDOM 0


/* Consider all of TR1 broken before CUDA 5.0.
 */
#	if CUDART_VERSION < 5000
#		undef FLUX_STDFEAT_TR1_SHAREDPTR
#		define FLUX_STDFEAT_TR1_SHAREDPTR 0

#		undef FLUX_STDFEAT_TR1_FUNCTIONAL
#		define FLUX_STDFEAT_TR1_FUNCTIONAL 0

#		undef FLUX_STDFEAT_TR1_TYPE_TRAITS 
#		define FLUX_STDFEAT_TR1_TYPE_TRAITS 0

#		undef FLUX_STDFEAT_TR1_TUPLE
#		define FLUX_STDFEAT_TR1_TUPLE 0

#		undef FLUX_STDFEAT_TR1_UNORDERED
#		define FLUX_STDFEAT_TR1_UNORDERED 0

#	elif CUDART_VERSION < 6050

/* TODO: verify these ! */
/* <tr1/memory> includes <ext/concurrence.h> which was updated between
 * versions 20120702 and 20130411.
 *  - Works with 20120702 (GCC 4.5.4) and NVCC 5.5
 *  - Broken with 20120920 (GCC 4.7.2) and NVCC 5.5
 *  - Broken with 20130411 (GCC 4.7.3) and NVCC 5.5
 */
#		if __GLIBCXX__ > 20120702
#			undef FLUX_STDFEAT_TR1_SHAREDPTR
#			define FLUX_STDFEAT_TR1_SHAREDPTR 0
#		endif // ~ __GLIBCXX__ > 20120301

/* <tr1/functional> causes errors in the host compiler with NVCC 5.5 and 
 * GCC's since at least 4.4.5. (FWIW, the tr1/functional header uses template
 * var-args even outside of C++0x/C++11 mode.)
 *
 *  - Broken with 20120920 (GCC 4.7.2) and NVCC 5.5
 */
#		undef FLUX_STDFEAT_TR1_FUNCTIONAL
#		define FLUX_STDFEAT_TR1_FUNCTIONAL 0

#	endif // ~ CUDART_VERSION
#endif //  ~ STDLIB == STDCPP

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_EXT_STDFEAT_TR1_FIXUPS_NVCC_HXX_8FC939B9_899F_490D_BD36_F06FE595F9F5
