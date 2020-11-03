/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	CUDA Method
 */
/*-***************************************************************** -}}}1- */

#include <flux/report/instr/method_cuda.hpp>

#include <flux/std/assert.hpp>

#include <flux_dlapi/cuda/cuda.hpp>
#include <flux_dlapi/cuda/load.hpp>
namespace CU = flux::dlapi::cuda::CU;

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::report::instr >>>     ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(report,instr)
//--    $ local                         ///{{{2///////////////////////////////
namespace
{
	// MethodInstance
	class CudaMethodInstance_ : public MethodInstance
	{
		public:
			explicit CudaMethodInstance_( dlapi::cuda::CUapi const* ) noexcept;
			~CudaMethodInstance_();

		public: /* virtuals: MethodInstance */
			void begin( Value&, std::size_t ) noexcept override;
			void end( Value&, std::size_t, Value&, std::size_t ) noexcept override;
		
			void set_params( ftl::not_null<MethodParams const*> ) override;

		private:
			void try_acquire_context_() noexcept;
		
		private:
			dlapi::cuda::CUapi const* mApi;
			CU::Context mContext;
	};

	// Method
	class CudaMethod_ : public Method
	{
		public:
			explicit CudaMethod_( dlapi::cuda::CUapi const* );
			~CudaMethod_();

		public: /* virtuals: Method */
			char const* name() noexcept override;
			char const* unit() noexcept override;

			unique_ptr<MethodInstance> create_instance() override;
			
		public:


		private:
			dlapi::cuda::CUapi const* mApi;
	};

	int const kMethodCudaIdSentinel_ = 0;
}

//--    kMethodCudaContext              ///{{{2///////////////////////////////

void const* const kMethodCudaContext = &kMethodCudaIdSentinel_;

//--    method_cuda_create()            ///{{{2///////////////////////////////
unique_ptr<Method> method_cuda_create( dlapi::cuda::CUapi const* aApi )
{
	return make_unique<CudaMethod_>( aApi );
}

//--   $ CudaMethod_                    ///{{{2///////////////////////////////
namespace
{
	CudaMethod_::CudaMethod_( dlapi::cuda::CUapi const* aApi )
		: mApi( aApi )
	{
		if( !mApi )
			mApi = dlapi::cuda::load_cuda();
	}
	CudaMethod_::~CudaMethod_()
	{
		if( mApi )
			dlapi::cuda::release_cuda( mApi );
	}
	
	
	char const* CudaMethod_::name() noexcept
	{
		return "CUDA";
	}
	char const* CudaMethod_::unit() noexcept
	{
		return "ms";
	}

	unique_ptr<MethodInstance> CudaMethod_::create_instance()
	{
		//TODO: if CUDA api couldn't be loaded, return a fake instance that
		//just sets stuff to -0.0 or nan or something.
		return make_unique<CudaMethodInstance_>( mApi );
	}
}

//--   $ CudaMethodInstance_            ///{{{2///////////////////////////////
namespace
{
	CudaMethodInstance_::CudaMethodInstance_( dlapi::cuda::CUapi const* aApi ) noexcept
		: mApi( (acquire_cuda(aApi), aApi) )
		, mContext( nullptr )
	{
		FLUX_ASSERT( mApi );
	}
	CudaMethodInstance_::~CudaMethodInstance_()
	{
		FLUX_ASSERT( mApi );
		release_cuda( mApi );
	}


	void CudaMethodInstance_::begin( Value& aValue, std::size_t aIndex ) noexcept
	{
		if( FLUX_HINT_UNLIKELY(!mContext) )
			try_acquire_context_();
	}
	void CudaMethodInstance_::end( Value& aValue, std::size_t, Value& aOldValue, std::size_t aOldInstance ) noexcept
	{
		if( FLUX_HINT_UNLIKELY(!mContext) )
			return;


	}


	void CudaMethodInstance_::set_params( ftl::not_null<MethodParams const*> aParams )
	{
		//TODO: verify that aContext->methodId is kMethodCudaContext

		auto const& params = reinterpret_cast<MethodParamsCuda const&>(*aParams);
		mContext = params.context;
	}


	void CudaMethodInstance_::try_acquire_context_() noexcept
	{
		//TODO
		//TODO  Check how flux-gianttoe deals with the CUDA contexts
		//TODO
		//TODO
	}
}

//--    <<< ~ flux::report::instr namespace <<<     ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(report,instr)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
