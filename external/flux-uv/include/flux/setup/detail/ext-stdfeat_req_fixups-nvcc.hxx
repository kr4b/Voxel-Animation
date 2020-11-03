/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Apply fixups and workarounds for NVCC
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_EXT_STDFEAT_REQ_FIXUPS_NVCC_HXX_3429C5CA_7558_4934_B847_E20E52DF63F9
#define DETAIL_EXT_STDFEAT_REQ_FIXUPS_NVCC_HXX_3429C5CA_7558_4934_B847_E20E52DF63F9

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

/* For some reason(tm), NVCC 6.5 picks the wrong constructor somewhere in
 * std::thread under libstdc++ from GCC 4.8.x and ends always attempting to
 * copy-construct the function object that's been passed to std::thread.
 *
 * This breaks with move-only types, such as packaged_task<>.
 *
 * Note: this has not been verified to be fixed under either newer NVCCs nor
 * newer libstdc++ (NVCC 6.5 refuses to compile with libstdc++ from GCC 4.9.x
 * and newer anyway).
 */
#	if CUDART_VERSION <= 6050
#		if __GLIBCXX__ < 20140422 /*GCC 4.9.0*/ || __GLIBCXX__ == 20141219 /*4.8.4*/
#			undef FLUX_STDFEAT_CXX11_THREAD
#			define FLUX_STDFEAT_CXX11_THREAD 0
#		endif // ~ libstdc++ with GCC 4.8.x and earlier
#	endif // ~ NVCC version 6.5 and earlier

#endif //  ~ STDLIB == STDCPP

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_EXT_STDFEAT_REQ_FIXUPS_NVCC_HXX_3429C5CA_7558_4934_B847_E20E52DF63F9
