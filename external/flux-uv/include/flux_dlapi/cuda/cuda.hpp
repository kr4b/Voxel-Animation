/*-******************************************************* -- HEADER -{{{1- */
/*- flux-dlapi_cuda forward declarations
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUXDLAPI_CUDA_CUDA_HPP_114E3E3F_9AB5_40DE_A126_21B880BD8B56
#define FLUXDLAPI_CUDA_CUDA_HPP_114E3E3F_9AB5_40DE_A126_21B880BD8B56

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/sys/dylib.hpp>

#include <cstddef>

//--    >>> namespace = dlapi::cuda >>>         ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(dlapi,cuda)
//--    macros                          ///{{{2///////////////////////////////

#if !defined(FLUXCU_API)
#	if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
#		define FLUXCU_API __stdcall
#	else
#		define FLUXCU_API
#	endif
#endif // ~ defined(FLUXCU_API)

//--    types                           ///{{{2///////////////////////////////
namespace CU
{
	typedef int Device;

	typedef struct EventOpaque_* Event;
	typedef struct StreamOpaque_* Stream;
	typedef struct ModuleOpaque_* Module;
	typedef struct ContextOpaque_* Context;
	typedef struct FunctionOpaque_* Function;
}

//--    constants                       ///{{{2///////////////////////////////
namespace CU
{
	enum Result
	{
		//XXX- very much incomplete
		SUCCESS = 0,

		ERROR_INVALID_VALUE = 1,
		ERROR_OUT_OF_MEMORY = 2,
		ERROR_NOT_INITIALIZED = 3,
		ERROR_DEINITIALIZED = 4,
		ERROR_INVALID_CONTEXT = 201,
		//ERROR_INVALID_HANDLE = 400, // Shit, this is a #define on windows.
		ERROR_INVALID_CUHANDLE = 400,
		//ERROR_NOT_READY = 600, // So is this. $@#% Windows
		ERROR_NOT_CUREADY = 600,

		ERROR_UNKNOWN = 999
	};

	enum DevAttrib
	{
		DEVATTR_MAX_THREADS_PER_BLOCK = 1,
		//XXX-omitted: MAX_BLOCK_DIM_*, MXA_GRID_DIM_*
		DEVATTR_MAX_SHARED_MEMORY_PER_BLOCK = 8,
		DEVATTR_TOTAL_CONSTANT_MEMORY = 9,
		//XXX-omitted: MAX_PITCH
		DEVATTR_MAX_REGISTERS_PER_BLOCK = 12,
		DEVATTR_CLOCK_RATE = 13,
		//XXX-omitted: TEXTURE_ALIGNMENT...GPU_OVERLAP
		DEVATTR_MULTIPROCESSOR_COUNT = 16,
		DEVATTR_KERNEL_EXEC_TIMEOUT = 17,
		DEVATTR_INTEGRATED = 18,
		DEVATTR_CAN_MAP_HOST_MEMORY = 19,
		DEVATTR_COMPUTE_MODE = 20,
		//XXX-omitted: MAXIMUM_TEXTURE* ... SURFACED_ALIGNMENT
		DEVATTR_CONCURRENT_KERNELS = 31,
		DEVATTR_ECC_ENABLED = 32,
		DEVATTR_PCI_BUS_ID = 33,
		DEVATTR_PCI_DEVICE_ID = 34,
		//XXX-omitted: TCC_DRIVER
		DEVATTR_MEMORY_CLOCK_RATE = 36,
		DEVATTR_GLOBAL_MEMORY_BUS_WIDTH = 37,
		DEVATTR_L2_CACHE_SIZE = 38,
		DEVATTR_MAX_THREADS_PER_MULTIPROCESSOR = 39,
		DEVATTR_ASYNC_ENGINE_COUNT = 40,
		DEVATTR_UNIFIED_ADDRESSING = 41,
		//XXX-omitted: MAXIMUM_TEXTURE1D* ... MAXIMUM_TEXTURE2D_MIPMAPPED_HEIGHT
		DEVATTR_COMPUTE_CAPABILITY_MAJOR = 75,
		DEVATTR_COMPUTE_CAPABILITY_MINOR = 76,
		//XXX-omitted: MAXIMUM_TEXTURE1D_...
		DEVATTR_STREAM_PRIORITIES_SUPPORTED = 78,
		DEVATTR_GLOBAL_L1_CACHE_SUPPORTED = 79,
		DEVATTR_LOCAL_L1_CACHE_SUPPORTED = 80,
		DEVATTR_MAX_SHARED_MEMORY_PER_MULTIPROCESSOR=82,
		DEVATTR_MANAGED_MEMORY = 83,
		//XXX-omitted: MULTI_GPU_BOARD*
	};

	enum /*StreamFlags*/
	{
		STREAM_DEFAULT = 0x0,
		STREAM_NON_BLOCKING = 0x1
	};
	enum /*EventFlags*/
	{
		EVENT_DEFAULT = 0x0,
		EVENT_BLOCKING_SYNC = 0x1,
		EVENT_DISABLE_TIMING = 0x2,
		EVENT_INTERPROCESS = 0x4
	};
}


//--    prot                            ///{{{2///////////////////////////////
namespace proto
{
	typedef CU::Result (FLUXCU_API *InitFn)( unsigned );
	typedef CU::Result (FLUXCU_API *DriverGetVersionFn)( int* );

	// context management
	typedef CU::Result (FLUXCU_API *DevicePrimaryCtxRetain)( CU::Context*, CU::Device );
	typedef CU::Result (FLUXCU_API *DevicePrimaryCtxRelease)( CU::Device );
	
	//XXX-omitted: create contexts, push/pop, destroy, context flags and so on
	typedef CU::Result (FLUXCU_API *CtxGetCurrentFn)( CU::Context* );
	typedef CU::Result (FLUXCU_API *CtxSetCurrentFn)( CU::Context );
	typedef CU::Result (FLUXCU_API *CtxPopCurrentFn)( CU::Context* );
	typedef CU::Result (FLUXCU_API *CtxPushCurrentFn)( CU::Context );
	typedef CU::Result (FLUXCU_API *CtxGetDeviceFn)( CU::Device* );
	//XXX-omitted: cuCtxGetFlags()
	typedef CU::Result (FLUXCU_API *CtxSynchronizeFn)();
	//XXX-omitted: cuCtxSetLimit() (FIXME)
	//XXX-omitted: cuCtxGetLimit() (FIXME)
	//XXX-omitted: cuCtxGetCacheConfig() (FIXME)
	//XXX-omitted: cuCtxSetCacheConfig() (FIXME)
	//XXX-omitted: cuCtxGetSharedMemConfig() (FIXME)
	//XXX-omitted: cuCtxSetSharedMemConfig() (FIXME)
	typedef CU::Result (FLUXCU_API *CtxGetApiVersionFn)( CU::Context, unsigned* );
	typedef CU::Result (FLUXCU_API *CtxGetStreamPriorityRange)( int*, int* );

	// device management
	typedef CU::Result (FLUXCU_API *DeviceGetCountFn)( int* );

	typedef CU::Result (FLUXCU_API *DeviceGetFn)( CU::Device*, int );

	typedef CU::Result (FLUXCU_API *DeviceGetNameFn)( char*, int, CU::Device );
	typedef CU::Result (FLUXCU_API *DeviceTotalMemFn)( std::size_t*, CU::Device );
	typedef CU::Result (FLUXCU_API *DeviceGetAttributeFn)( int*, CU::DevAttrib, CU::Device );
	typedef CU::Result (FLUXCU_API *DeviceComputeCapabilityFn)( int*, int*, CU::Device );

	// stream management
	typedef CU::Result (FLUXCU_API *StreamCreateFn)( CU::Stream*, unsigned );
	typedef CU::Result (FLUXCU_API *StreamCreateWithPriorityFn)( CU::Stream*, unsigned, int );
	typedef CU::Result (FLUXCU_API *StreamGetPriorityFn)( CU::Stream, int* );
	typedef CU::Result (FLUXCU_API *StreamGetFlagsFn)( CU::Stream, unsigned* );
	typedef CU::Result (FLUXCU_API *StreamWaitEventFn)( CU::Stream, CU::Event, unsigned );
	//XXX-omitted: cuStreamAddCallback (FIXME)
	//XXX-omitted: cuStreamAttachMemAsync [__CUDA_API_VERSION >= 6000]
	typedef CU::Result (FLUXCU_API *StreamQueryFn)( CU::Stream );
	typedef CU::Result (FLUXCU_API *StreamSynchronizeFn)( CU::Stream );
	typedef CU::Result (FLUXCU_API *StreamDestroyFn)( CU::Stream );

	// event management
	typedef CU::Result (FLUXCU_API *EventCreateFn)( CU::Event*, unsigned );
	typedef CU::Result (FLUXCU_API *EventRecordFn)( CU::Event, CU::Stream );
	typedef CU::Result (FLUXCU_API *EventQueryFn)( CU::Event );
	typedef CU::Result (FLUXCU_API *EventSynchronizeFn)( CU::Event );
	typedef CU::Result (FLUXCU_API *EventDestroyFn)( CU::Event );
	typedef CU::Result (FLUXCU_API *EventElapsedTimeFn)( float*, CU::Event, CU::Event );

	// modules & functions
	// This is primarily for testing without requiring NVCC compilation; many
	// methods are missing.
	typedef CU::Result (FLUXCU_API *ModuleUnloadFn)( CU::Module );
	typedef CU::Result (FLUXCU_API *ModuleLoadDataFn)( CU::Module*, void const* );
	typedef CU::Result (FLUXCU_API *ModuleGetFunctionFn)( CU::Function*, CU::Module, char const* );

	typedef CU::Result (FLUXCU_API *FuncSetBlockShapeFn)( CU::Function, int, int, int );

	typedef CU::Result (FLUXCU_API *ParamSetSizeFn)( CU::Function, unsigned );
	typedef CU::Result (FLUXCU_API *ParamSetvFn)( CU::Function, int, void*, unsigned );

	typedef CU::Result (FLUXCU_API *LaunchFn)( CU::Function );
	typedef CU::Result (FLUXCU_API *LaunchGridFn)( CU::Function, int, int );
	typedef CU::Result (FLUXCU_API *LaunchGridAsyncFn)( CU::Function, int, int, CU::Stream );
}

//--    CUapi                           ///{{{2///////////////////////////////
struct CUapi
{
	bool valid;
	
	proto::InitFn init;
	proto::DriverGetVersionFn driverGetVersion;


	proto::DevicePrimaryCtxRetain devicePrimaryCtxRetain;
	proto::DevicePrimaryCtxRelease devicePrimaryCtxRelease;

	proto::CtxGetCurrentFn ctxGetCurrent;
	proto::CtxSetCurrentFn ctxSetCurrent;
	proto::CtxPopCurrentFn ctxPopCurrent;
	proto::CtxPushCurrentFn ctxPushCurrent;
	proto::CtxGetDeviceFn ctxGetDevice;
	proto::CtxSynchronizeFn ctxSynchronize;
	proto::CtxGetApiVersionFn ctxGetApiVersion;
	proto::CtxGetStreamPriorityRange ctxGetStreamPriorityRange;

	proto::DeviceGetCountFn deviceGetCount;
	proto::DeviceGetFn deviceGet;
	proto::DeviceGetNameFn deviceGetName;
	proto::DeviceTotalMemFn deviceTotalMem;
	proto::DeviceGetAttributeFn deviceGetAttribute;
	proto::DeviceComputeCapabilityFn deviceComputeCapability; // Apparently deprecated.


	proto::StreamCreateFn streamCreate;
	proto::StreamCreateWithPriorityFn streamCreateWithPriority;
	proto::StreamGetPriorityFn streamGetPriority;
	proto::StreamGetFlagsFn streamGetFlags;
	proto::StreamWaitEventFn streamWaitEvent;
	proto::StreamQueryFn streamQuery;
	proto::StreamSynchronizeFn streamSynchronize;
	proto::StreamDestroyFn streamDestroy;


	proto::EventCreateFn eventCreate;
	proto::EventRecordFn eventRecord;
	proto::EventQueryFn eventQuery;
	proto::EventSynchronizeFn eventSynchronize;
	proto::EventDestroyFn eventDestroy;
	proto::EventElapsedTimeFn eventElapsedTime;


	proto::ModuleUnloadFn moduleUnload;
	proto::ModuleLoadDataFn moduleLoadData;
	proto::ModuleGetFunctionFn moduleGetFunction;

	proto::FuncSetBlockShapeFn funcSetBlockShape;

	proto::ParamSetSizeFn paramSetSize;
	proto::ParamSetvFn paramSetv;

	proto::LaunchFn launch;
	proto::LaunchGridFn launchGrid;
	proto::LaunchGridAsyncFn launchGridAsync;

	size_t referenceCount_;
	sys::DylibHandle libHandle_; //TODO: Hide?
};

//--    <<< ~ dlapi::cuda namespace <<<        ///{{{1////////////////////////
FLUX_NAMESPACE_LEAVE2(dlapi,cuda)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUXDLAPI_CUDA_CUDA_HPP_114E3E3F_9AB5_40DE_A126_21B880BD8B56
