/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Apply fixups and workarounds for NVCC
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_STDFEAT_FIXUPS_NVCC_HXX_721D5680_E6D5_4E62_BFA9_1B2FB3D96E44
#define DETAIL_STDFEAT_FIXUPS_NVCC_HXX_721D5680_E6D5_4E62_BFA9_1B2FB3D96E44

//--//////////////////////////////////////////////////////////////////////////
//--    NVCC Version                ///{{{1///////////////////////////////////

/* Get CUDART_VERSION. This is the closest to a compiler version that we can
 * get. (It should be equal to CUDA_VERSION, though.)
 */
#	include <cuda_runtime_api.h> 

//--    Fixups: GNU libstdc++       ///{{{1///////////////////////////////////
#if FLUX_STDLIB == FLUX_STDLIB_STDCPP
#endif //  ~ STDLIB == STDCPP

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_STDFEAT_FIXUPS_NVCC_HXX_721D5680_E6D5_4E62_BFA9_1B2FB3D96E44
