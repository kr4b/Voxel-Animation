/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	basic_barrier implementation
 *
 * TODO: memory order for atomics? 
 */
/*-***************************************************************** -}}}1- */

#include <flux/setup/buildcfg.hpp>

#include <flux/std/assert.hpp>
#include <flux/sys/intrin.hpp>

#include <limits>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--   $ detail                             ///{{{2///////////////////////////
namespace detail
{
#	if FLUX_NIH_BARRIER == FLUX_NIH_BARRIER_FUTEX
	FLUX_ATTR_ALWAYS_INLINE
	int bfutex( void* aAddr, int aOp, int aVal, void* aTimeOut, int* aUaddr2, int aVal3 ) noexcept
	{
		return ::syscall( SYS_futex, aAddr, aOp, aVal, aTimeOut, aUaddr2, aVal3 );
	}
#	endif // ~ BARRIER_FUTEX
}

//--   basic_barrier -- STDCONDVAR          ///{{{2///////////////////////////
#if FLUX_NIH_BARRIER == FLUX_NIH_BARRIER_STDCONDVAR
template< class tFunction > inline
basic_barrier<tFunction>::basic_barrier( std::ptrdiff_t aPhase, tFunction aFunction )
	: tFunction( std::move(aFunction) )
	, mPhase( aPhase )
	, mCount( aPhase )
	, mCycle( 0 )
{}

template< class tFunction > inline
basic_barrier<tFunction>::~basic_barrier()
{
#	if FLUX_DEBUG
	std::unique_lock<std::mutex> lock( mMutex );
	FLUX_ASSERT( mCount == mPhase );
#	endif // ~ DEBUG
}

template< class tFunction > inline
auto basic_barrier<tFunction>::arrive( ptrdiff_t aUpdate ) -> arrival_token
{
	std::unique_lock<std::mutex> lock{mMutex};

	auto const cycle = mCycle;

	FLUX_ASSERT( mCount >= aUpdate );
	if( 0 == (mCount -= aUpdate) )
	{
		complete_( std::move(lock) );
		return { cycle-1 };
	}

	return { cycle };
}

template< class tFunction > inline
void basic_barrier<tFunction>::wait( arrival_token&& aArrival ) const
{
	FLUX_ASSERT( mPhase > 0 );

	// Wait for everybody else to arrive
	std::unique_lock<std::mutex> lock{mMutex};

	if( aArrival.val != mCycle )
		return;
	
	mCV.wait( lock, [&] {
		return aArrival.val != mCycle;
	} );
}

template< class tFunction > inline
void basic_barrier<tFunction>::arrive_and_wait()
{
	std::unique_lock<std::mutex> lock{mMutex};

	FLUX_ASSERT( mCount > 0 );
	if( --mCount == 0 )
	{
		complete_( std::move(lock) );
		return;
	}

	// Wait for everybody else to arrive
	auto const cycle = mCycle;
	mCV.wait( lock, [&] {
		return cycle != mCycle;
	} );
}
template< class tFunction > inline
void basic_barrier<tFunction>::arrive_and_drop()
{
	std::unique_lock<std::mutex> lock( mMutex );

	FLUX_ASSERT( mCount > 0 && mPhase > 0 );

	// Thread no longer participates in this barrier. Decrement phase count to
	// reflect this.
	--mPhase;

	if( --mCount == 0 )
	{
		complete_( std::move(lock) );
		return;
	}

	// Don't wait.
}

template< class tFunction > inline constexpr
std::ptrdiff_t basic_barrier<tFunction>::max() noexcept
{
	return std::numeric_limits<std::ptrdiff_t>::max();
}

template< class tFunction > inline
void basic_barrier<tFunction>::complete_( std::unique_lock<std::mutex> aLock )
{
	FLUX_ASSERT( aLock.owns_lock() );

	// Completion only triggered when nobody is waiting, i.e., when the count 
	// has reached zero.
	FLUX_ASSERT( mCount == 0 );

	// Run the completion function
	static_cast<tFunction&>(*this)();

	// Transition state to next phase
	mCount = mPhase;
	++mCycle;

	// Wake everybody
	aLock.unlock();
	mCV.notify_all();
}
#endif // ~ BARRIER_STDCONDVAR

//--   basic_barrier -- WAITONADDR          ///{{{2///////////////////////////
#if FLUX_NIH_BARRIER == FLUX_NIH_BARRIER_WAITONADDR
template< class tFunction > inline
basic_barrier<tFunction>::basic_barrier( std::ptrdiff_t aPhase, tFunction aFunction )
	: tFunction( std::move(aFunction) )
	, mWin32( (dlapi::os::acquire_win32(), dlapi::os::win32()) )
	, mPhase( aPhase )
	, mCount( aPhase )
	, mCycle( 0 )
{
	FLUX_ASSERT( mWin32 ); // XXX-FIXME Or even throw() something?
}

template< class tFunction > inline
basic_barrier<tFunction>::~basic_barrier()
{
#	if FLUX_DEBUG
	//FLUX_ASSERT( mCount.load() == mPhase.load() );
#	endif // ~ DEBUG

	dlapi::os::release_win32();
}

template< class tFunction > inline
auto basic_barrier<tFunction>::arrive( ptrdiff_t aUpdate ) -> arrival_token
{
	auto const cycle = mCycle.load();
	auto const old = mCount.fetch_sub( aUpdate );
	FLUX_ASSERT( old >= aUpdate );

	if( aUpdate == old )
	{
		complete_();
		return { cycle-1 }; // Makes sure that wait() from this arrival_token() never blocks.
	}

	return { cycle };
}

template< class tFunction > inline
void basic_barrier<tFunction>::wait( arrival_token&& aArrival ) const
{
	for( std::size_t i = kBarrierSpinCount; i; --i )
	{
		if( mCycle.load() != aArrival.val )
			return;

		sys::hwpause();
	}

	FLUX_ASSERT( mWin32 );
	while( mCycle.load() == aArrival.val )
		mWin32->waitOnAddress( &mCycle, &aArrival.val, sizeof(uint32_t), dlapi::os::Win32::infinite );
}

template< class tFunction > inline
void basic_barrier<tFunction>::arrive_and_wait()
{
	auto cycle = mCycle.load();
	auto const old = mCount.fetch_sub( 1 );
	FLUX_ASSERT( old > 0 );

	if( 1 == old )
	{
		complete_();
		return;
	}

	for( std::size_t i = kBarrierSpinCount; i; --i )
	{
		if( mCycle.load() != cycle )
			return;

		sys::hwpause();
	}

	FLUX_ASSERT( mWin32 );
	while( mCycle.load() == cycle )
		mWin32->waitOnAddress( &mCycle, &cycle, sizeof(uint32_t), dlapi::os::Win32::infinite );
}

template< class tFunction > inline
void basic_barrier<tFunction>::arrive_and_drop()
{
	mPhase.fetch_sub( 1 );

	auto const old = mCount.fetch_sub( 1 );
	FLUX_ASSERT( old > 0 );

	if( 1 == old )
	{
		complete_();
		return;
	}

	// Don't wait.
}

template< class tFunction > inline constexpr
std::ptrdiff_t basic_barrier<tFunction>::max() noexcept
{
	return std::numeric_limits<std::ptrdiff_t>::max();
}

template< class tFunction > inline
void basic_barrier<tFunction>::complete_()
{
	// Completion only triggered when nobody is waiting, i.e., when the count 
	// has reached zero.
	FLUX_ASSERT( mCount.load() == 0 );

	// Run the completion function
	static_cast<tFunction&>(*this)();

	// Transition state to next phase
	mCount.store( mPhase.load() ); 
	++mCycle;

	// Make sure all the sleepies wake up.
	FLUX_ASSERT( mWin32 );
	mWin32->wakeByAddressAll( &mCycle );
}
#endif // ~ BARRIER_WAITONADDR

//--   basic_barrier -- FUTEX               ///{{{2///////////////////////////
#if FLUX_NIH_BARRIER == FLUX_NIH_BARRIER_FUTEX
template< class tFunction > inline
basic_barrier<tFunction>::basic_barrier( std::ptrdiff_t aPhase, tFunction aFunction )
	: tFunction( std::move(aFunction) )
	, mPhase( aPhase )
	, mCount( aPhase )
	, mCycle( 0 )
{}

template< class tFunction > inline
basic_barrier<tFunction>::~basic_barrier()
{
#	if FLUX_DEBUG
	//FLUX_ASSERT( mCount.load() == mPhase.load() );
#	endif // ~ DEBUG
}

template< class tFunction > inline
auto basic_barrier<tFunction>::arrive( ptrdiff_t aUpdate ) -> arrival_token
{
	auto const cycle = mCycle.load();
	auto const old = mCount.fetch_sub( aUpdate );
	FLUX_ASSERT( old >= aUpdate );

	if( aUpdate == old )
	{
		complete_();
		return { cycle-1 }; // Makes sure that wait() from this arrival_token() never blocks.
	}

	return { cycle };
}

template< class tFunction > inline
void basic_barrier<tFunction>::wait( arrival_token&& aArrival ) const
{
	// Spin.
	for( std::size_t i = kBarrierSpinCount; i; --i )
	{
		if( mCycle.load() != aArrival.val )
			return;

		sys::hwpause();
	}

	// Wait. Futexes maybe have spurious wakeups; the man page suggests using
	// a defensive approach and checking for it. 
	while( mCycle.load() == aArrival.val )
	{
		auto const ret = detail::bfutex( &mCycle, FUTEX_WAIT_PRIVATE, aArrival.val, nullptr, nullptr, 0 );

		FLUX_ASSERT( ret >= 0 || (errno == EAGAIN || errno == EINTR) );
		(void)ret;
	}
}

template< class tFunction > inline
void basic_barrier<tFunction>::arrive_and_wait()
{
	auto cycle = mCycle.load();
	auto const old = mCount.fetch_sub( 1 );
	FLUX_ASSERT( old > 0 );

	if( 1 == old )
	{
		complete_();
		return;
	}

	for( std::size_t i = kBarrierSpinCount; i; --i )
	{
		if( mCycle.load() != cycle )
			return;

		sys::hwpause();
	}

	// See above
	while( mCycle.load() == cycle )
	{
		auto const ret = detail::bfutex( &mCycle, FUTEX_WAIT_PRIVATE, cycle, nullptr, nullptr, 0 );

		FLUX_ASSERT( ret >= 0 || (errno == EAGAIN || errno == EINTR) );
		(void)ret;
	}
}

template< class tFunction > inline
void basic_barrier<tFunction>::arrive_and_drop()
{
	mPhase.fetch_sub( 1 );

	auto const old = mCount.fetch_sub( 1 );
	FLUX_ASSERT( old > 0 );

	if( 1 == old )
	{
		complete_();
		return;
	}

	// Don't wait.
}

template< class tFunction > inline constexpr
std::ptrdiff_t basic_barrier<tFunction>::max() noexcept
{
	return std::numeric_limits<std::ptrdiff_t>::max();
}

template< class tFunction > inline
void basic_barrier<tFunction>::complete_()
{
	// Completion only triggered when nobody is waiting, i.e., when the count 
	// has reached zero.
	FLUX_ASSERT( mCount.load() == 0 );

	// Run the completion function
	static_cast<tFunction&>(*this)();

	// Transition state to next phase
	mCount.store( mPhase.load() ); 
	++mCycle;

	// Make sure all the sleepies wake up.
	int ret = detail::bfutex( &mCycle, FUTEX_WAKE_PRIVATE, std::numeric_limits<int>::max(), nullptr, nullptr, 0 );

	FLUX_ASSERT( ret >= 0 );
	(void)ret;
}
#endif // ~ BARRIER_FUTEX

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
