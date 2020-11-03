/*-******************************************************* -- HEADER -{{{1- */
/*-	std::barrier fallback
 *
 * Implementation notes:

 * (1) The default/fallback implementation uses std::condition_variable with
 *     multiple threads waiting on a single CV. This only relies on C++
 *     standard library objects. The disadvantage is that thread wakeup is
 *     serialized as each thread needs to reaquire the std::mutex passed to
 *     condition_variable::wait(). In some quick tests, and on a specific
 *     machine, this seems to be a bit more than 1µs per thread.
 *
 * (2) Windows: Optionally (but by default) uses WaitOnAddress() and a pile of
 *     atomics. This eliminates the issue described above, and reduces the
 *     overall spread of the wakeup time from n*1µs to about 0.1µs constant for
 *     all thread on the same machine. The currrent implementation spins in
 *     userspace for a short while before calling WaitOnAddress().
 *
 *     WARNING: WaitOnAddress() is Windows8+. Currently doesn't attempt to
 *     fallback if unavailable (and will just ASSERT()).
 *
 *     TODO-maybe: should the mCycle be on its own cache line, so that 
 *     stuff can spin peacefully on it without mucking up other loads?
 *
 *     TODO: dlapi_os/win32 should handle loading Win32api when not all
 *     features are there. 
 *     TODO: Missing WaitOnAddress() should then handle this somehow. 
 *     TODO: Throw an exception instead of ASSERT()ing. (see WARNING above)
 *
 * (3) Linux: Uses futexes (SYS_futex) and a pile of atomics as above.
 *     Benchmarks as above, but on a different machine; results are similar
 *     though: using the condition_variable takes around .5µs per thread.  With
 *     the futex, threads start within 0.05µs of each other, often even within
 *     0.01µs (with the thread triggering the barrier being ~0.2µs slower,
 *     likely due to having to perform a syscall after releasing spinning
 *     threads; this seems difficult to avoid).
 * 
 * WARNING: CURRENTLY SOMEWHAT SKETCHY (I.E. NEW AND UNTESTED). THE DIFFERENT
 * VARIANTS ALLOW FOR MANY DIFFERENT WEIRD AND WONDERFUL BUGS.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_NIH_BARRIER_HPP_64D66C3B_4535_441C_8A00_B9B524B4850B
#define FLUX_NIH_BARRIER_HPP_64D66C3B_4535_441C_8A00_B9B524B4850B

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/platform.hpp>

#include <flux/std/inttypes.hpp>
#include <flux/std/static_assert.hpp>

#include <flux/compat/constexpr.hpp>

#include <flux/util/attrib.hpp>

#include <atomic>

#include <cstddef>

//--    config                      ///{{{1///////////////////////////////////

#define FLUX_NIH_BARRIER_STDCONDVAR 0
#define FLUX_NIH_BARRIER_WAITONADDR 1 // Win32 / Windows 8 +
#define FLUX_NIH_BARRIER_FUTEX 2 // Linux futex

#if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
#	define FLUX_NIH_BARRIER FLUX_NIH_BARRIER_WAITONADDR
#	include <flux_dlapi/os/win32.hpp>

#elif FLUX_PLATFORM == FLUX_PLATFORM_LINUX
#	define FLUX_NIH_BARRIER FLUX_NIH_BARRIER_FUTEX
#	include <errno.h>
#	include <unistd.h>
#	include <linux/futex.h>
#	include <sys/syscall.h>

#else // fallback
#	define FLUX_NIH_BARRIER FLUX_NIH_BARRIER_STDCONDVAR

#	include <mutex>
#	include <condition_variable>
#endif // ~ PLATFORM

//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    constants                   ///{{{1///////////////////////////////////

constexpr std::size_t kBarrierSpinCount = 1337;

//--    barrier                     ///{{{1///////////////////////////////////
namespace detail
{
	struct EmptyBarrierCompletion {
		inline FLUX_CONSTEXPR_EX
		void operator() () const noexcept
		{}
	};

#	if FLUX_NIH_BARRIER == FLUX_NIH_BARRIER_FUTEX
	FLUX_ATTR_ALWAYS_INLINE
	int bfutex( int*, int, int, void*, int*, int ) noexcept;
#	endif // ~ BARRIER_FUTEX
}

template< class tFunction >
class basic_barrier
	: private tFunction
{
	public:
#		if FLUX_NIH_BARRIER == FLUX_NIH_BARRIER_STDCONDVAR
		struct arrival_token { std::ptrdiff_t val; };
#		elif FLUX_NIH_BARRIER == FLUX_NIH_BARRIER_WAITONADDR
		struct arrival_token { uint32_t val; };
#		elif FLUX_NIH_BARRIER == FLUX_NIH_BARRIER_FUTEX
		struct arrival_token { uint32_t val; };
#		endif // ~ BARRIER

	public:
		explicit basic_barrier( std::ptrdiff_t, tFunction = tFunction() );
		~basic_barrier();

		basic_barrier( basic_barrier const& ) = delete;
		basic_barrier& operator= (basic_barrier const&) = delete;

	public:
		arrival_token arrive( ptrdiff_t = 1 );
		void wait( arrival_token&& ) const;

		void arrive_and_wait();
		void arrive_and_drop();

		static constexpr
		std::ptrdiff_t max() noexcept;

	private:
#		if FLUX_NIH_BARRIER == FLUX_NIH_BARRIER_STDCONDVAR
		void complete_( std::unique_lock<std::mutex> );
#		elif FLUX_NIH_BARRIER == FLUX_NIH_BARRIER_WAITONADDR
		void complete_();
#		elif FLUX_NIH_BARRIER == FLUX_NIH_BARRIER_FUTEX
		void complete_();
#		endif // ~ BARRIER

	private:
#		if FLUX_NIH_BARRIER == FLUX_NIH_BARRIER_STDCONDVAR
		mutable std::mutex mMutex;
		mutable std::condition_variable mCV;
		std::ptrdiff_t mPhase, mCount;
		uint32_t mCycle;

#		elif FLUX_NIH_BARRIER == FLUX_NIH_BARRIER_WAITONADDR
		dlapi::os::Win32api const* const mWin32;

		std::atomic<std::ptrdiff_t> mPhase, mCount;
		mutable std::atomic<uint32_t> mCycle;

		FLUX_STATIC_ASSERT( sizeof(std::atomic<uint32_t>) == sizeof(uint32_t) );

#		elif FLUX_NIH_BARRIER == FLUX_NIH_BARRIER_FUTEX
		std::atomic<std::ptrdiff_t> mPhase, mCount;
		mutable std::atomic<uint32_t> mCycle;

		FLUX_STATIC_ASSERT( sizeof(std::atomic<uint32_t>) == sizeof(uint32_t) );
#		endif // ~ BARRIER
};

using barrier = basic_barrier< detail::EmptyBarrierCompletion >;

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/barrier.inl"
#endif // FLUX_NIH_BARRIER_HPP_64D66C3B_4535_441C_8A00_B9B524B4850B
