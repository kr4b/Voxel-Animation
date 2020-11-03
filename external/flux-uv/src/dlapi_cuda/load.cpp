/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	flux-dlapi_cuda loader
 */
/*-***************************************************************** -}}}1- */

#include <flux_dlapi/cuda/cuda.hpp>
#include <flux_dlapi/cuda/load.hpp>

#include <flux/setup.pkg>
#include <flux/setup/platform.hpp>

#include <flux/sys/dylib.hpp>

#include <cstdio>
#include <cassert>
#include <cstring>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = dlapi::cuda >>>             ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(dlapi,cuda)
//--    $ local                         ///{{{2///////////////////////////////
namespace
{
#	if FLUX_PLATFORM == FLUX_PLATFORM_WIN32 || FLUX_PLATCAP_CYGWIN
	constexpr char const* kDefaultCUDALibName = "nvcuda.dll";
#	else // !WIN32
	constexpr char const* kDefaultCUDALibName = "libcuda.so";
#	endif // ~ PLATFORM

	CUapi* load_api_( void* (*)(char const*,void*), void* );

	// Warning: moderate evil ahead (but we have to cast things either way).
	struct FnPtrProxy_
	{
		inline FnPtrProxy_( void* aPtr )
			: mPtr(aPtr)
		{}

		template< typename tType > inline
		operator tType*() const {
			return reinterpret_cast<tType*>(mPtr);
		}

		void* mPtr;
	};
}

//--    unload_cuda()                   ///{{{2///////////////////////////////
void unload_cuda( CUapi const* aCU )
{
	CUapi* mptr = const_cast<CUapi*>(aCU);
	assert( mptr );

	mptr->valid = false;
	
	release_cuda( aCU );
}

//--    acquire_cuda()                  ///{{{2///////////////////////////////
void acquire_cuda( CUapi const* aCU )
{
	assert( aCU );
	++const_cast<CUapi*>(aCU)->referenceCount_;
}

//--    release_cuda()                  ///{{{2///////////////////////////////
void release_cuda( CUapi const* aCU )
{
	assert( aCU );
	if( 0 == --const_cast<CUapi*>(aCU)->referenceCount_ )
	{
		if( aCU->libHandle_ ) 
			sys::dylib_close( aCU->libHandle_ );

		delete aCU;
	}
}

//--    load_cuda()                     ///{{{2///////////////////////////////
CUapi const* load_cuda( void* (*aGetFn)(char const*,void*), void* aUser )
{
	return load_api_( aGetFn, aUser );
}

CUapi const* load_cuda()
{
	if( auto const handle = sys::dylib_open( kDefaultCUDALibName ) )
	{
		using FnType_ = void* (*)(char const*, void*);
		FnType_ const get = [] (char const* aName, void* aHandle) {
			auto const handle = static_cast<sys::DylibHandle>(aHandle);
			return (void*)sys::dylib_get_proc( handle, aName );
		};

		if( auto const ret = load_api_( get, handle ) )
		{
			ret->libHandle_ = handle;
			return ret;
		}

		sys::dylib_close( handle );
	}

	return nullptr;
}

//--    $ load_api_()                   ///{{{2///////////////////////////////
namespace
{
	CUapi* load_api_( void* (*aGetFn)(char const*,void*), void* aUser )
	{
		assert( aGetFn );
		auto load = [&] (char const* aName) {
			return FnPtrProxy_(aGetFn( aName, aUser ));
		};

		CUapi* cu = new CUapi();
		cu->libHandle_ = nullptr;

		bool ok = true;

		//TODO: check API version of CUDA context thing? -> see cuCtxGetApiVersion()

#		define LOADFN_( fld, name ) ok = !!(cu->fld = load( name )) && ok
		// generic methods
		LOADFN_( init, "cuInit" );
		LOADFN_( driverGetVersion, "cuDriverGetVersion" );

		// context management
		LOADFN_( devicePrimaryCtxRetain, "cuDevicePrimaryCtxRetain" ); // __CUDA_API_VERSION >= 7000
		LOADFN_( devicePrimaryCtxRelease, "cuDevicePrimaryCtxRelease" ); // __CUDA_API_VERSION >= 7000

		LOADFN_( ctxGetCurrent, "cuCtxGetCurrent" );
		LOADFN_( ctxSetCurrent, "cuCtxSetCurrent" );
		LOADFN_( ctxPopCurrent, "cuCtxPopCurrent_v2" ); // __CUDA_API_VERSION >= 4000
		LOADFN_( ctxPushCurrent, "cuCtxPushCurrent_v2" ); // __CUDA_API_VERSION >= 4000
		LOADFN_( ctxGetDevice, "cuCtxGetDevice" );
		LOADFN_( ctxSynchronize, "cuCtxSynchronize" );
		LOADFN_( ctxGetApiVersion, "cuCtxGetApiVersion" );
		LOADFN_( ctxGetStreamPriorityRange, "cuCtxGetStreamPriorityRange" );

		// device management 
		LOADFN_( deviceGetCount, "cuDeviceGetCount" );
		LOADFN_( deviceGet, "cuDeviceGet" );
		LOADFN_( deviceGetName, "cuDeviceGetName" );
		LOADFN_( deviceTotalMem, "cuDeviceTotalMem_v2" ); // __CUDA_API_VERSION >= 3200
		LOADFN_( deviceGetAttribute, "cuDeviceGetAttribute" );
		LOADFN_( deviceComputeCapability, "cuDeviceComputeCapability" ); // DEPRECATED!

		// stream management
		LOADFN_( streamCreate, "cuStreamCreate" );
		LOADFN_( streamCreateWithPriority, "cuStreamCreateWithPriority" ); // compute cap 3.5+
		LOADFN_( streamGetPriority, "cuStreamGetPriority" );
		LOADFN_( streamGetFlags, "cuStreamGetFlags" );
		LOADFN_( streamWaitEvent, "cuStreamWaitEvent" );
		LOADFN_( streamQuery, "cuStreamQuery" );
		LOADFN_( streamSynchronize, "cuStreamSynchronize" );
		LOADFN_( streamDestroy, "cuStreamDestroy_v2" ); // __CUDA_API_VERSION >= 4000

		// event management
		LOADFN_( eventCreate, "cuEventCreate" );
		LOADFN_( eventRecord, "cuEventRecord" );
		LOADFN_( eventQuery, "cuEventQuery" );
		LOADFN_( eventSynchronize, "cuEventSynchronize" );
		LOADFN_( eventDestroy, "cuEventDestroy_v2" ); // __CUDA_API_VERSION >= 4000
		LOADFN_( eventElapsedTime, "cuEventElapsedTime" );

		// functions & modules
		LOADFN_( moduleUnload, "cuModuleUnload" );
		LOADFN_( moduleLoadData, "cuModuleLoadData" );
		LOADFN_( moduleGetFunction, "cuModuleGetFunction" );

		LOADFN_( funcSetBlockShape, "cuFuncSetBlockShape" );

		LOADFN_( paramSetSize, "cuParamSetSize" );
		LOADFN_( paramSetv, "cuParamSetv" );

		LOADFN_( launch, "cuLaunch" );
		LOADFN_( launchGrid, "cuLaunchGrid" );
		LOADFN_( launchGridAsync, "cuLaunchGridAsync" );
#		undef LOADFN_

		// done.
		if( ok )
		{
			cu->valid = true;
			cu->referenceCount_ = 1;
			return cu;
		}

		delete cu;
		return nullptr;
	}
}

//--    <<< ~ dlapi::cuda namespace <<<             ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(dlapi,cuda)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
