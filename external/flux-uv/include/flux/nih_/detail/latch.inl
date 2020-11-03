/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	latch implementation
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
#	if FLUX_NIH_LATCH == FLUX_NIH_LATCH_FUTEX
	FLUX_ATTR_ALWAYS_INLINE
	int lfutex( void* aAddr, int aOp, int aVal, void* aTimeOut, int* aUaddr2, int aVal3 ) noexcept
	{
		return ::syscall( SYS_futex, aAddr, aOp, aVal, aTimeOut, aUaddr2, aVal3 );
	}
#	endif // ~ LATCH_FUTEX
}

//--   latch -- STDCONDVAR                  ///{{{2///////////////////////////
#if FLUX_NIH_LATCH == FLUX_NIH_LATCH_STDCONDVAR
inline
latch::latch( std::ptrdiff_t aPhase )
	: mCount( aPhase )
{}

inline
void latch::count_down( ptrdiff_t aUpdate )
{
	std::unique_lock<std::mutex> lock{mMutex};

	FLUX_ASSERT( mCount >= aUpdate );
	if( 0 == (mCount -= aUpdate) )
		complete_( std::move(lock) );
}

inline
bool latch::try_wait() const noexcept
{
	std::unique_lock<std::mutex> lock{mMutex};
	return 0 == mCount;
}

inline
void latch::wait() const
{
	std::unique_lock<std::mutex> lock{mMutex};

	if( 0 == mCount )
		return;
	
	mCV.wait( lock, [&] {
		return 0 == mCount;
	} );
}

inline
void latch::arrive_and_wait( ptrdiff_t aUpdate )
{
	std::unique_lock<std::mutex> lock{mMutex};

	FLUX_ASSERT( mCount >= aUpdate );
	if( 0 == (mCount -= aUpdate) )
	{
		complete_( std::move(lock) );
		return;
	}

	// Wait for everybody else to arrive
	mCV.wait( lock, [&] {
		return 0 == mCount;
	} );
}

inline constexpr
std::ptrdiff_t latch::max() noexcept
{
	return std::numeric_limits<std::ptrdiff_t>::max();
}

inline
void latch::complete_( std::unique_lock<std::mutex> aLock )
{
	FLUX_ASSERT( aLock.owns_lock() );

	// Completion only triggered when nobody is waiting, i.e., when the count 
	// has reached zero.
	FLUX_ASSERT( mCount == 0 );

	// Wake everybody
	aLock.unlock();
	mCV.notify_all();
}
#endif // ~ LATCH_STDCONDVAR

//--   latch -- WAITONADDR          ///{{{2///////////////////////////
#if FLUX_NIH_LATCH == FLUX_NIH_LATCH_WAITONADDR
inline
latch::latch( std::ptrdiff_t aPhase )
	: mWin32( (dlapi::os::acquire_win32(), dlapi::os::win32()) )
	, mCount( aPhase )
{
	FLUX_ASSERT( mWin32 ); // XXX-FIXME Or even throw() something?
}

inline
latch::~latch()
{
	dlapi::os::release_win32();
}

inline
void latch::count_down( ptrdiff_t aUpdate )
{
	auto const old = mCount.fetch_sub( aUpdate );
	FLUX_ASSERT( old >= aUpdate );

	if( aUpdate == old )
		complete_();
}

inline
bool latch::try_wait() const noexcept
{
	return 0 == mCount.load();
}

inline
void latch::wait() const
{
	for( std::size_t i = kLatchSpinCount; i; --i )
	{
		if( mCount.load() == 0 )
			return;

		sys::hwpause();
	}

	FLUX_ASSERT( mWin32 );
	while( auto val = mCount.load() )
	{
		// WaitOnAddress() returns immediately if mCount is not equal to val;
		// This means that we might wake up a few times here if there is a lot
		// of contention (i.e., somebody else decrements mCount before
		// WaitOnAddress "properly" blocks).
		mWin32->waitOnAddress( &mCount, &val, sizeof(std::ptrdiff_t), dlapi::os::Win32::infinite );
	}
}

inline
void latch::arrive_and_wait( std::ptrdiff_t aUpdate )
{
	auto const old = mCount.fetch_sub( aUpdate );
	FLUX_ASSERT( old >= aUpdate );

	if( aUpdate == old )
	{
		complete_();
		return;
	}

	for( std::size_t i = kLatchSpinCount; i; --i )
	{
		if( mCount.load() == 0 )
			return;

		sys::hwpause();
	}

	FLUX_ASSERT( mWin32 );
	while( auto val = mCount.load() )
	{
		// See comments in ::wait() above.
		mWin32->waitOnAddress( &mCount, &val, sizeof(std::ptrdiff_t), dlapi::os::Win32::infinite );
	}
}

inline constexpr
std::ptrdiff_t latch::max() noexcept
{
	return std::numeric_limits<std::ptrdiff_t>::max();
}

inline
void latch::complete_()
{
	// Completion only triggered when nobody is waiting, i.e., when the count 
	// has reached zero.
	FLUX_ASSERT( mCount.load() == 0 );

	// Make sure all the sleepies wake up.
	FLUX_ASSERT( mWin32 );
	mWin32->wakeByAddressAll( &mCount );
}
#endif // ~ LATCH_WAITONADDR

//--   latch -- FUTEX               ///{{{2///////////////////////////
#if FLUX_NIH_LATCH == FLUX_NIH_LATCH_FUTEX
inline
latch::latch( std::ptrdiff_t aPhase )
	: mCount( (FLUX_ASSERT(std::ptrdiff_t(uint32_t(aPhase)) == aPhase), uint32_t(aPhase)) )
{}

inline
void latch::count_down( ptrdiff_t aUpdate )
{
	auto const old = mCount.fetch_sub( aUpdate );
	FLUX_ASSERT( old >= std::uint32_t(aUpdate) );

	if( std::uint32_t(aUpdate) == old )
		complete_();
}

inline
bool latch::try_wait() const noexcept
{
	return 0 == mCount.load();
}

inline
void latch::wait() const
{
	// Spin.
	for( std::size_t i = kLatchSpinCount; i; --i )
	{
		if( mCount.load() == 0 )
			return;

		sys::hwpause();
	}

	// Wait. Futexes maybe have spurious wakeups; the man page suggests using
	// a defensive approach and checking for it. 
	while( auto val = mCount.load() )
	{
		auto const ret = detail::lfutex( &mCount, FUTEX_WAIT_PRIVATE, val, nullptr, nullptr, 0 );

		FLUX_ASSERT( ret >= 0 || (errno == EAGAIN || errno == EINTR) );
		(void)ret;
	}
}

inline
void latch::arrive_and_wait( std::ptrdiff_t aUpdate )
{
	auto const old = mCount.fetch_sub( aUpdate );
	FLUX_ASSERT( old >= uint32_t(aUpdate) );

	if( uint32_t(aUpdate) == old )
	{
		complete_();
		return;
	}

	// Spin (see above)
	for( std::size_t i = kLatchSpinCount; i; --i )
	{
		if( mCount.load() == 0 )
			return;

		sys::hwpause();
	}

	// Wait. 
	while( auto val = mCount.load() )
	{
		auto const ret = detail::lfutex( &mCount, FUTEX_WAIT_PRIVATE, val, nullptr, nullptr, 0 );

		FLUX_ASSERT( ret >= 0 || (errno == EAGAIN || errno == EINTR) );
		(void)ret;
	}
}

inline constexpr
std::ptrdiff_t latch::max() noexcept
{
	// Limited to a 32-bit value thanks to futexes being 32 bits always.
	// Note: on 32-bit machines std::ptrdiff_t is a 32-bit signed integer,
	// and therefore the limiting factor.
	return sizeof(std::uint32_t) == sizeof(std::ptrdiff_t)
		? std::numeric_limits<std::ptrdiff_t>::max()
		: std::numeric_limits<std::uint32_t>::max()
	;
}

inline
void latch::complete_()
{
	// Completion only triggered when nobody is waiting, i.e., when the count 
	// has reached zero.
	FLUX_ASSERT( mCount.load() == 0 );


	// Make sure all the sleepies wake up.
	int ret = detail::lfutex( &mCount, FUTEX_WAKE_PRIVATE, std::numeric_limits<int>::max(), nullptr, nullptr, 0 );

	FLUX_ASSERT( ret >= 0 );
	(void)ret;
}
#endif // ~ LATCH_FUTEX

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
