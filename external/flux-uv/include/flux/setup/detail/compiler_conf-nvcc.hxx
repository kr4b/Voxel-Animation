/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Setup: NVCC
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_COMPILER_CONF_NVCC_HXX_EE584DA4_CAD1_48F7_86B4_055F4EA5464E
#define DETAIL_COMPILER_CONF_NVCC_HXX_EE584DA4_CAD1_48F7_86B4_055F4EA5464E

//--//////////////////////////////////////////////////////////////////////////
//--    FLUX_COMPILER*              ///{{{1///////////////////////////////////

#undef FLUX_COMPILER_ACTUAL
#define FLUX_COMPILER_ACTUAL FLUX_COMPILER_NVCC

//--    FLUX_NVCC_PASS*             ///{{{1///////////////////////////////////

#if defined(__CUDA_ARCH__)
#	undef FLUX_NVCC_ACTIVE_PASS
#	define FLUX_NVCC_ACTIVE_PASS FLUX_NVCC_PASS_DEVICE

#else
#	undef FLUX_NVCC_ACTIVE_PASS
#	define FLUX_NVCC_ACTIVE_PASS FLUX_NVCC_PASS_HOST

#endif // ~ defined(__CUDA_ARCH__)

//--    FLUX_LANGLEVEL              ///{{{1///////////////////////////////////

/* NVCC pre-6.x (tested on 6.5) doesn't support C++11 constructs, regardless of
 * the host compiler. So, disable C++11 constructs there.
 */
#if defined(CUDART_VERSION) && CUDART_VERSION < 6050
#	undef FLUX_LANGLEVEL
#	define FLUX_LANGLEVEL FLUX_LANGLEVEL_CXX03
#endif // ~ CUDART_VERSION

/* Otherwise, use whatever the host compiler specifies. NVCC typically tries to
 * be compatible with that (and succeeds to some degree).
 */

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_COMPILER_CONF_NVCC_HXX_EE584DA4_CAD1_48F7_86B4_055F4EA5464E
