/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	(Thread) execution control and stuff
 */
/*-***************************************************************** -}}}1- */

#include <flux/sys/execctrl.hpp>
#include <flux/sys/info.hpp>
#include <flux/sys/errno_category.hpp>
#include <flux/sys/win32_category.hpp>

#include <flux/util/switch.hpp>
#include <flux/util/bit_twiddle.hpp>

#include <flux/std/log.hpp>
#include <flux/setup/platform.hpp>
#include <flux/compat/exchange.hpp>

#include <flux_ext/phmap.hpp>

#include <mutex>
#include <thread>
#include <algorithm>

#include <cstring>

#if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
#	include <sched.h>
#	include <pthread.h>

#	if FLUX_PLATFORM == FLUX_PLATFORM_LINUX
#		define FLUXTMP_SYS_PERTHREAD_NICE_ 1
#		include <unistd.h>
#		include <sys/syscall.h>
#		include <sys/resource.h>
#	else // !PLATFORM_LINUX
#		define FLUXTMP_SYS_PERTHREAD_NICE_ 0
#	endif // ~ PLATFORM_LINUX
#else // PLATFORM_WIN32
#	include <flux_dlapi/os/win32.hpp>
#endif // ~ PLATFORM

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	using NativeThread_ = ::pthread_t;
	constexpr int kFifoPriority_ = 1;
#	else // PLATFORM_WIN32
	using NativeThread_ = dlapi::os::Win32::HANDLE;
#	endif // ~ PLATFORM

#	if FLUXTMP_SYS_PERTHREAD_NICE_
	// Nice values for EThreadPriority::low & EThreadPriority::lower
	// (Lower priority -> higher niceness)
	constexpr int kLowNice_ = 6;
	constexpr int kLowerNice_ = 12;
#	endif // ~ PERTHREAD_NICE

	void* make_handle_( NativeThread_ const& ) noexcept;
	void clean_handle_( void* ) noexcept;

	NativeThread_ get_handle_( void* ) noexcept;
}

//--    ThreadHandle                        ///{{{2///////////////////////////
ThreadHandle::ThreadHandle()
	: mOpaqueHandle( nullptr )
{}
ThreadHandle::~ThreadHandle()
{
	clean_handle_( mOpaqueHandle );
}

ThreadHandle::ThreadHandle( ThreadHandle&& aOther ) noexcept
	: mOpaqueHandle( compat::exchange( aOther.mOpaqueHandle, nullptr ) )
{}
ThreadHandle& ThreadHandle::operator= (ThreadHandle&& aOther) noexcept
{
	std::swap( mOpaqueHandle, aOther.mOpaqueHandle );
	return *this;
}

ThreadHandle::ThreadHandle( std::nullptr_t ) noexcept
	: mOpaqueHandle( nullptr )
{}
ThreadHandle& ThreadHandle::operator= (std::nullptr_t) noexcept
{
	clean_handle_( mOpaqueHandle );
	mOpaqueHandle = nullptr;
	return *this;
}

ThreadHandle::ThreadHandle( void* aOpaqueHandle ) noexcept
	: mOpaqueHandle( aOpaqueHandle )
{}
ThreadHandle& ThreadHandle::operator= (void* aOpaqueHandle) noexcept
{
	clean_handle_( mOpaqueHandle );
	mOpaqueHandle = aOpaqueHandle;
	return *this;
}

void* ThreadHandle::opaque() const
{
	return mOpaqueHandle;
}

//--    thread_self()                       ///{{{2///////////////////////////
ThreadHandle thread_self() noexcept
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	return ThreadHandle( make_handle_(::pthread_self()) );
#	else // PLATFORM_WIN32
	auto const* w32 = dlapi::os::win32();
	return ThreadHandle( make_handle_(w32->getCurrentThread()) );
#	endif // ~ PLATFORM
}

//--    thread_get_affinity()               ///{{{2///////////////////////////
LogicalCoreSet thread_get_affinity( ThreadHandle aThreadHandle, compat::error_code* aErrorOut ) noexcept
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	::pthread_t const handle = (aThreadHandle.opaque())
		? get_handle_( aThreadHandle.opaque() )
		: ::pthread_self()
	;

	cpu_set_t cpuset;
	CPU_ZERO( &cpuset );
	auto res = ::pthread_getaffinity_np( handle, sizeof(cpu_set_t), &cpuset );
	if( 0 != res )
	{
		if( aErrorOut ) *aErrorOut = sys::make_errno_error_code( res );
		return LogicalCoreSet{};
	}

	LogicalCoreSet ret{};
	for( std::size_t i = 0; i < CPU_SETSIZE; ++i )
	{
		if( CPU_ISSET( i, &cpuset ) )
			ret.emplace( i );
	}
	return ret;		
#	else // PLATFORM_WIN32
	auto const* w32 = dlapi::os::win32();
	auto const handle = (aThreadHandle.opaque())
		? get_handle_( aThreadHandle.opaque() )
		: w32->getCurrentThread()
	;

	DWORD_PTR proc, sys;
	if( !w32->getProcessAffinityMask( w32->getCurrentProcess(), &proc, &sys ) )
	{
		if( aErrorOut ) *aErrorOut = sys::make_win32_error_code();
		return LogicalCoreSet{};
	}

	auto res = w32->setThreadAffinityMask( handle, proc );
	if( 0 == res )
	{
		if( aErrorOut ) *aErrorOut = sys::make_win32_error_code();
		return LogicalCoreSet{};
	}

	if( 0 == w32->setThreadAffinityMask( handle, res ) )
		FLUX_LOG( WARNING, "Failed to restore thread affinity mask" );

	LogicalCoreSet ret;
	for( ; res; res &= res -1 )
	{
		ret.emplace( LogicalCoreId(util::bit_count_trailing_zeros( uint64_t(res) )) ); //XXX-fixme int_cast<>
	}
	return ret;
#	endif // ~ PLATFORM
}

//--    thread_set_affinity()               ///{{{2///////////////////////////
bool thread_set_affinity( LogicalCoreSet const& aSet, ThreadHandle aThreadHandle, compat::error_code* aErrorOut ) noexcept
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	::pthread_t const handle = (aThreadHandle.opaque())
		? get_handle_( aThreadHandle.opaque() )
		: ::pthread_self()
	;

	cpu_set_t cpuset;
	CPU_ZERO( &cpuset );
	for( auto const id : aSet )
		CPU_SET( id, &cpuset );
	
	auto res = ::pthread_setaffinity_np( handle, sizeof(cpu_set_t), &cpuset );
	if( 0 != res )
	{
		if( aErrorOut ) *aErrorOut = sys::make_errno_error_code( res );
		return false;
	}

	return true;
#	else // PLATFORM_WIN32
	auto const* w32 = dlapi::os::win32();
	auto const handle = (aThreadHandle.opaque())
		? get_handle_( aThreadHandle.opaque() )
		: w32->getCurrentThread()
	;

	DWORD_PTR mask = 0;
	for( auto const idx : aSet )
		mask |= (DWORD_PTR(1)<<idx);

	if( 0 == w32->setThreadAffinityMask( handle, mask ) )
	{
		if( aErrorOut ) *aErrorOut = sys::make_win32_error_code();
		return false;
	}

	return true;
#	endif // ~ PLATFORM
}
bool thread_set_affinity( LogicalCoreId aIndex, ThreadHandle aThreadHandle, compat::error_code* aErrorOut ) noexcept
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	::pthread_t const handle = (aThreadHandle.opaque())
		? get_handle_( aThreadHandle.opaque() )
		: ::pthread_self()
	;

	cpu_set_t cpuset;
	CPU_ZERO( &cpuset );
	CPU_SET( aIndex, &cpuset );
	
	auto res = ::pthread_setaffinity_np( handle, sizeof(cpu_set_t), &cpuset );
	if( 0 != res )
	{
		if( aErrorOut ) *aErrorOut = sys::make_errno_error_code( res );
		return false;
	}

	return true;
#	else // PLATFORM_WIN32
	auto const* w32 = dlapi::os::win32();
	FLUX_ASSERT( w32 );

	auto const handle = (aThreadHandle.opaque())
		? get_handle_( aThreadHandle.opaque() )
		: w32->getCurrentThread()
	;

	DWORD_PTR mask = DWORD_PTR(1) << aIndex;
	if( 0 == w32->setThreadAffinityMask( handle, mask ) )
	{
		if( aErrorOut ) *aErrorOut = sys::make_win32_error_code();
		return false;
	}

	return true;
#	endif // ~ PLATFORM
}

//--    thread_get_priority()               ///{{{2///////////////////////////
EThreadPriority thread_get_priority( ThreadHandle aThreadHandle ) noexcept
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	::pthread_t const handle = (aThreadHandle.opaque())
		? get_handle_( aThreadHandle.opaque() )
		: ::pthread_self()
	;

	int policy = 0;
	sched_param param;
	if( 0 != ::pthread_getschedparam( handle, &policy, &param ) )
		return EThreadPriority::unidentified;

	switch( policy )
	{
		case SCHED_OTHER: {
#			if FLUXTMP_SYS_PERTHREAD_NICE_
			auto const tid = ::syscall( __NR_gettid );

			errno = 0;
			auto const nice  = ::getpriority( PRIO_PROCESS, tid );
			if( -1 == nice && errno != 0 )
				return EThreadPriority::unidentified;

			if( nice == kLowerNice_ ) return EThreadPriority::lower;
			if( nice == kLowNice_ ) return EThreadPriority::low;
			if( nice == 0 ) return EThreadPriority::normal;

			::rlimit rnice;
			if( -1 == ::getrlimit( RLIMIT_NICE, &rnice ) )
				return EThreadPriority::unidentified;

			int niceMin = 20-int(rnice.rlim_cur);
			if( niceMin > 0 )
				niceMin = 0;

			if( nice == niceMin ) return EThreadPriority::higher;
			if( nice == niceMin/2 ) return EThreadPriority::high;

			return EThreadPriority::unidentified;
#			endif // ~ PERTHREAD_NICE

			return EThreadPriority::normal;
		} break;
		case SCHED_FIFO: {
			if( kFifoPriority_ == param.sched_priority ) 
				return EThreadPriority::critical;
			return EThreadPriority::unidentified;
		} break;
		case SCHED_IDLE: {
			return EThreadPriority::idle;
		} break;
	}

	return EThreadPriority::unidentified;	
	
#	else // PLATFORM_WIN32
	namespace Win32_ = dlapi::os::Win32;
	auto const* w32 = dlapi::os::win32();
	FLUX_ASSERT( w32 );

	auto const pclass = w32->getPriorityClass( w32->getCurrentProcess() );
	if( Win32_::normal_priority_class != pclass )
		return EThreadPriority::unidentified;

	auto const handle = (aThreadHandle.opaque())
		? get_handle_( aThreadHandle.opaque() )
		: w32->getCurrentThread()
	;

	switch( w32->getThreadPriority( handle ) )
	{
		case Win32_::thread_priority_highest: return EThreadPriority::higher;
		case Win32_::thread_priority_above_normal: return EThreadPriority::high;
		case Win32_::thread_priority_normal: return EThreadPriority::normal;
		case Win32_::thread_priority_below_normal: return EThreadPriority::low;
		case Win32_::thread_priority_lowest: return EThreadPriority::lower;

		case Win32_::thread_priority_idle: return EThreadPriority::idle;
		case Win32_::thread_priority_time_critical: return EThreadPriority::critical;
	}
#	endif // ~ PLATFORM

	return EThreadPriority::unidentified;
}

//--    thread_set_priority()               ///{{{2///////////////////////////
bool thread_set_priority( EThreadPriority aPrio, ThreadHandle aThreadHandle, compat::error_code* aErrorOut ) noexcept
{
	if( EThreadPriority::unidentified == aPrio )
	{
		if( aErrorOut ) *aErrorOut = compat::make_error_code( compat::errc::invalid_argument );
		return false;
	}

#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	::pthread_t const handle = (aThreadHandle.opaque())
		? get_handle_( aThreadHandle.opaque() )
		: ::pthread_self()
	;

#	if FLUXTMP_SYS_PERTHREAD_NICE_
	::rlimit rnice;
	if( -1 == ::getrlimit( RLIMIT_NICE, &rnice ) )
	{
		if( aErrorOut ) *aErrorOut = sys::make_errno_error_code();
		return false;
	}

	int niceMin = 20-int(rnice.rlim_cur);
	if( niceMin > 0 )
		niceMin = 0;

	int nice = 0;
	bool increased = false;
	switch( aPrio )
	{
		case EThreadPriority::lower: nice = kLowerNice_; break;
		case EThreadPriority::low: nice = kLowNice_; break;
		case EThreadPriority::normal: nice = 0; break;
		case EThreadPriority::high: nice = niceMin/2; increased = true; break;
		case EThreadPriority::higher: nice = niceMin; increased = true; break;
		default: break;
	}

	if( increased && 0 == nice )
	{
		if( aErrorOut ) *aErrorOut = compat::make_error_code( compat::errc::operation_not_permitted );
		return false;
	}
#	endif // ~ PERTHREAD_NICE_

	int policy = 0;
	sched_param param{};
	switch( aPrio )
	{
		case EThreadPriority::lower: FLUX_SWITCH_FALLTHROUGH();
		case EThreadPriority::low: FLUX_SWITCH_FALLTHROUGH();
		case EThreadPriority::normal: FLUX_SWITCH_FALLTHROUGH();
		case EThreadPriority::high: FLUX_SWITCH_FALLTHROUGH();
		case EThreadPriority::higher: {
#			if FLUXTMP_SYS_PERTHREAD_NICE_
			auto const tid = ::syscall( __NR_gettid );
			if( -1 ==  ::setpriority( PRIO_PROCESS, tid, nice ) )
			{
				int errno_ = errno;
				if( EACCES == errno_ ) errno_ = EPERM;
				if( aErrorOut ) *aErrorOut = sys::make_errno_error_code(errno_);
				return false;
			}
#			endif // ~ PERTHREAD_NICE

			policy = SCHED_OTHER;
		} break;

		case EThreadPriority::idle: {
			policy = SCHED_IDLE;
		} break;
		case EThreadPriority::critical: {
			policy = SCHED_FIFO;
			param.sched_priority = kFifoPriority_;
		} break;

		case EThreadPriority::unidentified: FLUX_ASSERT(false); //XXX unreachable
	}

	auto const res = ::pthread_setschedparam( handle, policy, &param );
	if( 0 != res )
	{
		if( aErrorOut ) *aErrorOut = sys::make_errno_error_code( res );
		return false;

	}
	
	return true;
#	else // PLATFORM_WIN32
	namespace Win32_ = dlapi::os::Win32;
	auto const* w32 = dlapi::os::win32();
	FLUX_ASSERT( w32 );

	int const prio = [&] {
		switch( aPrio )
		{
			case EThreadPriority::higher: return Win32_::thread_priority_highest;
			case EThreadPriority::high: return Win32_::thread_priority_above_normal;
			case EThreadPriority::normal: return Win32_::thread_priority_normal;
			case EThreadPriority::low: return Win32_::thread_priority_below_normal;
			case EThreadPriority::lower: return Win32_::thread_priority_lowest;

			case EThreadPriority::idle: return Win32_::thread_priority_idle;
			case EThreadPriority::critical: return Win32_::thread_priority_time_critical;

			case EThreadPriority::unidentified: FLUX_ASSERT(false); //XXX switch unreachable
		}

		//XXX unreachable.
		FLUX_ASSERT( false );
	}();

	// Probably don't do this, since it affects the whole process.
	/*if( !w32->setPriorityClass( w32->getCurrentProcess(), Win32_::normal_priority_class ) )
	{
		if( aErrorOut ) *aErrorOut = sys::make_win32_error_code();
		return false;
	}*/

	auto const handle = (aThreadHandle.opaque())
		? get_handle_( aThreadHandle.opaque() )
		: w32->getCurrentThread()
	;

	if( !w32->setThreadPriority( handle, prio ) )
	{
		if( aErrorOut ) *aErrorOut = sys::make_win32_error_code();
		return false;
	}

	return true;
#	endif // ~ PLATFORM
}

//--    thread_current_logical_cpu()        ///{{{2///////////////////////////
LogicalCoreId thread_current_logical_cpu( compat::error_code* aErrorOut ) noexcept
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	auto ret = ::sched_getcpu();
	if( -1 == ret )
	{
		if( aErrorOut ) *aErrorOut = sys::make_errno_error_code();
		return ~LogicalCoreId(0);
	}

	return std::size_t(ret); // FIXME check::int_cast<>
#	else // PLATFORM_WIN32
	auto const* w32 = dlapi::os::win32();
	FLUX_ASSERT( w32 );

	return w32->getCurrentProcessorNumber(); //FIXME check::int_cast<>
#	endif // ~ PLATFORM
}

//--    exec_for_each_enabled_cpu()         ///{{{2///////////////////////////
std::size_t exec_for_each_enabled_cpu( unique_function<void(LogicalCoreId)> aFunc )
{
	if( !aFunc )
		return 0;

	// Store initial affinity, and reset it later. Use the platform APIs 
	// directly, to avoid creating and destroying the LogicalCoreSet objects.
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	auto const self = ::pthread_self();
	
	::cpu_set_t initial;
	if( 0 != ::pthread_getaffinity_np( self, sizeof(cpu_set_t), &initial ) )
		return 0;

	::cpu_set_t cpuset;
	CPU_ZERO( &cpuset );

	auto const set_affinity_ = [&] (LogicalCoreId aNew, LogicalCoreId aOld) {
		CPU_CLR( aOld, &cpuset );
		CPU_SET( aNew, &cpuset );

		return 0 == ::pthread_setaffinity_np( self, sizeof(cpu_set_t), &cpuset );
	};
#	else // PLATFORM_WIN32
	auto const* w32 = dlapi::os::win32();
	auto const self = w32->getCurrentThread();

	DWORD_PTR proc, sys;
	if( !w32->getProcessAffinityMask( w32->getCurrentProcess(), &proc, &sys ) )
		return 0;

	auto const initial = w32->setThreadAffinityMask( self, proc );
	if( 0 == initial )
		return 0;

	auto const set_affinity_ = [&] (LogicalCoreId aNew, LogicalCoreId) {
		return 0 != w32->setThreadAffinityMask( self, DWORD_PTR(1) << aNew );
	};
#	endif // ~ PLATFORM

	auto const numCPUs = LogicalCoreId(get_logical_cpu_count()); //FIXME check::int_cast

	std::size_t ret = 0;
	for( LogicalCoreId i = 0; i < numCPUs; ++i )
	{
		if( set_affinity_( i, i ? i-1 : 0 ) )
		{
			aFunc( i );
			++ret;
		}
	}

	// Reset original affinity
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	if( 0 != ::pthread_setaffinity_np( self, sizeof(cpu_set_t), &initial ) )
		FLUX_LOG( WARNING, "Failed to reset thread affinity" );
#	else // PLATFORM_WIN32
	if( 0 == w32->setThreadAffinityMask( self, initial ) )
		FLUX_LOG( WARNING, "Failed to restore thread affinity mask" );
#	endif // ~ PLATFORM

	return ret;
}

//--    exec_for_each_logical_cpu()         ///{{{2///////////////////////////
std::size_t exec_for_each_logical_cpu( unique_function<void(LogicalCoreId)> aFunc, unique_function<void(LogicalCoreId)> aDisabledFunc )
{
	if( !aFunc )
		return 0;

	// Store initial affinity, and reset it later. Use the platform APIs 
	// directly, to avoid creating and destroying the LogicalCoreSet objects.
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	auto const self = ::pthread_self();
	
	::cpu_set_t initial;
	if( 0 != ::pthread_getaffinity_np( self, sizeof(cpu_set_t), &initial ) )
		return 0;

	::cpu_set_t cpuset;
	CPU_ZERO( &cpuset );

	auto const set_affinity_ = [&] (LogicalCoreId aNew, LogicalCoreId aOld) {
		CPU_CLR( aOld, &cpuset );
		CPU_SET( aNew, &cpuset );

		return 0 == ::pthread_setaffinity_np( self, sizeof(cpu_set_t), &cpuset );
	};
#	else // PLATFORM_WIN32
	auto const* w32 = dlapi::os::win32();
	auto const self = w32->getCurrentThread();

	DWORD_PTR proc, sys;
	if( !w32->getProcessAffinityMask( w32->getCurrentProcess(), &proc, &sys ) )
		return 0;

	auto const initial = w32->setThreadAffinityMask( self, proc );
	if( 0 == initial )
		return 0;

	auto const set_affinity_ = [&] (LogicalCoreId aNew, LogicalCoreId) {
		return 0 != w32->setThreadAffinityMask( self, DWORD_PTR(1) << aNew );
	};
#	endif // ~ PLATFORM

	auto const numCPUs = LogicalCoreId(get_logical_cpu_count()); // FIXME check::int_cast<>

	std::size_t ret = 0;
	for( LogicalCoreId i = 0; i < numCPUs; ++i )
	{
		if( set_affinity_( i, i ? i-1 : 0 ) )
		{
			aFunc( i );
			++ret;
		}
		else
		{
			aDisabledFunc( i );
		}
	}

	// Reset original affinity
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	if( 0 != ::pthread_setaffinity_np( self, sizeof(cpu_set_t), &initial ) )
		FLUX_LOG( WARNING, "Failed to reset thread affinity" );
#	else // PLATFORM_WIN32
	if( 0 == w32->setThreadAffinityMask( self, initial ) )
		FLUX_LOG( WARNING, "Failed to restore thread affinity mask" );
#	endif // ~ PLATFORM

	return ret;
}

//--    exec_for_each_physical_cpu()        ///{{{2///////////////////////////
std::size_t exec_for_each_physical_cpu( unique_function<void(PhysicalCoreId,LogicalCoreId)> aFunc, unique_function<void(PhysicalCoreId)> aDisabledFunc )
{
	if( auto const topo = sys::get_cpu_topology() )
	{
		// Store initial affinity, and reset it later. Use the platform APIs 
		// directly, to avoid creating and destroying the LogicalCoreSet objects.
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		auto const self = ::pthread_self();
		
		::cpu_set_t initial;
		if( 0 != ::pthread_getaffinity_np( self, sizeof(cpu_set_t), &initial ) )
			return 0;

		::cpu_set_t cpuset;
		CPU_ZERO( &cpuset );

		auto const set_affinity_ = [&] (LogicalCoreId aId) {
			CPU_ZERO( &cpuset );
			CPU_SET( aId, &cpuset );

			return 0 == ::pthread_setaffinity_np( self, sizeof(cpu_set_t), &cpuset );
		};
#		else // PLATFORM_WIN32
		auto const* w32 = dlapi::os::win32();
		auto const self = w32->getCurrentThread();

		DWORD_PTR proc, sys;
		if( !w32->getProcessAffinityMask( w32->getCurrentProcess(), &proc, &sys ) )
			return 0;

		auto const initial = w32->setThreadAffinityMask( self, proc );
		if( 0 == initial )
			return 0;

		auto const set_affinity_ = [&] (LogicalCoreId aId) {
			return 0 != w32->setThreadAffinityMask( self, DWORD_PTR(1) << aId );
		};
#		endif // ~ PLATFORM

		std::size_t ret = 0, count = topo.physical_count();
		for( PhysicalCoreId phy = 0; phy < count; ++phy )
		{
			auto const logical = topo.physical_to_logical( phy );
			if( ~LogicalCoreId(0) != logical && set_affinity_( logical ) )
			{
				aFunc( phy, logical );
				++ret;
			}
			else
			{
				aDisabledFunc( phy );
			}
		}
		return ret;
	}
	
	return 0;
}

//--    $ make_handle_()                    ///{{{2///////////////////////////
namespace
{
	template< bool >
	struct MakeHandle_
	{
		template< typename tType >
		static void* enact( tType const& aNative ) noexcept
		{
			static_assert( sizeof(tType) <= sizeof(void*), "Error: inconsistent MakeHandle_ selection" );
			
			void* ret;
			std::memcpy( &ret, &aNative, sizeof(tType) );
			return ret;
		}
	};
	template<>
	struct MakeHandle_<false>
	{
		template< typename tType >
		static void* enact( tType const& aNative ) noexcept try
		{
			static_assert( sizeof(tType) > sizeof(void*), "Error: inconsistent MakeHandle_ selection" );

			return new tType{ aNative };
		}
		catch( std::bad_alloc const& )
		{
			return nullptr;
		}
	};

	void* make_handle_( NativeThread_ const& aNative ) noexcept
	{
		return MakeHandle_<
			sizeof(NativeThread_) <= sizeof(void*)
		>::enact( aNative );
	}
}

//--    $ clean_handle_()                   ///{{{2///////////////////////////
namespace
{
	template< bool >
	struct CleanHandle_
	{
		template< typename tType >
		static void enact( void* ) noexcept
		{
			static_assert( sizeof(tType) <= sizeof(void*), "Error: inconsistent CleanHandle_ selection" );
		}
	};
	template<>
	struct CleanHandle_<false>
	{
		template< typename tType >
		static void enact( void* aOpaque ) noexcept
		{
			static_assert( sizeof(tType) > sizeof(void*), "Error: inconsistent CleanHandle_ selection" );

			delete static_cast<tType*>(aOpaque);
		}
	};

	void clean_handle_( void* aOpaque ) noexcept
	{
		return CleanHandle_<
			sizeof(NativeThread_) <= sizeof(void*)
		>::template enact<NativeThread_>( aOpaque );
	}
}

//--    $ get_handle_()                     ///{{{2///////////////////////////
namespace
{
	template< bool >
	struct GetHandle_
	{
		template< typename tType >
		static tType enact( void* aOpaque ) noexcept
		{
			static_assert( sizeof(tType) <= sizeof(void*), "Error: inconsistent GetHandle_ selection" );

			tType ret;
			std::memcpy( &ret, &aOpaque, sizeof(tType) );
			return ret;
		}
	};
	template<>
	struct GetHandle_<false>
	{
		template< typename tType >
		static tType enact( void* aOpaque ) noexcept
		{
			static_assert( sizeof(tType) > sizeof(void*), "Error: inconsistent GetHandle_ selection" );

			return *static_cast<tType*>(aOpaque);
		}
	};

	NativeThread_ get_handle_( void* aOpaque ) noexcept
	{
		return GetHandle_<
			sizeof(NativeThread_) <= sizeof(void*)
		>::template enact<NativeThread_>( aOpaque );
	}
}

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
