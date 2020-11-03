/*-******************************************************* -- HEADER -{{{1- */
/*-	std::latch fallback
 *
 * See barrier.hpp / nih_::barrier for implementation notes. The implementation
 * herein is essentially simplified version of the barrier.
 *
 * WARNING: CURRENTLY SOMEWHAT SKETCHY (I.E. NEW AND UNTESTED). THE DIFFERENT
 * VARIANTS ALLOW FOR MANY DIFFERENT WEIRD AND WONDERFUL BUGS.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_NIH_LATCH_HPP_73064247_ED3A_46B2_A2C3_6F49B00E3D06
#define FLUX_NIH_LATCH_HPP_73064247_ED3A_46B2_A2C3_6F49B00E3D06

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/platform.hpp>

#include <flux/std/inttypes.hpp>
#include <flux/std/static_assert.hpp>


#include <flux/util/attrib.hpp>

#include <atomic>

#include <cstddef>

//--    config                      ///{{{1///////////////////////////////////

#define FLUX_NIH_LATCH_STDCONDVAR 0
#define FLUX_NIH_LATCH_WAITONADDR 1 // Win32 / Windows 8 +
#define FLUX_NIH_LATCH_FUTEX 2 // Linux futex

#if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
#	define FLUX_NIH_LATCH FLUX_NIH_LATCH_WAITONADDR
#	include <flux_dlapi/os/win32.hpp>

#elif FLUX_PLATFORM == FLUX_PLATFORM_LINUX
#	define FLUX_NIH_LATCH FLUX_NIH_LATCH_FUTEX
#	include <errno.h>
#	include <unistd.h>
#	include <linux/futex.h>
#	include <sys/syscall.h>

#else // fallback
#	define FLUX_NIH_LATCH FLUX_NIH_LATCH_STDCONDVAR

#	include <mutex>
#	include <condition_variable>
#endif // ~ PLATFORM

//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    constants                   ///{{{1///////////////////////////////////

constexpr std::size_t kLatchSpinCount = 1337;

//--    latch                       ///{{{1///////////////////////////////////
namespace detail
{
#	if FLUX_NIH_LATCH == FLUX_NIH_LATCH_FUTEX
	FLUX_ATTR_ALWAYS_INLINE
	int lfutex( int*, int, int, void*, int*, int ) noexcept;
#	endif // ~ LATCH_FUTEX
}

class latch
{
	public:
		explicit latch( std::ptrdiff_t );

#		if FLUX_NIH_LATCH == FLUX_NIH_LATCH_WAITONADDR
		~latch();
#		endif // ~ LATCH_WAITONADDR

		latch( latch const& ) = delete;
		latch& operator= (latch const&) = delete;

	public:
		void count_down( ptrdiff_t = 1 );
		bool try_wait() const noexcept;
		void wait() const;

		void arrive_and_wait( ptrdiff_t = 1 );

		static constexpr
		std::ptrdiff_t max() noexcept;

	private:
#		if FLUX_NIH_LATCH == FLUX_NIH_LATCH_STDCONDVAR
		void complete_( std::unique_lock<std::mutex> );
#		elif FLUX_NIH_LATCH == FLUX_NIH_LATCH_WAITONADDR
		void complete_();
#		elif FLUX_NIH_LATCH == FLUX_NIH_LATCH_FUTEX
		void complete_();
#		endif // ~ LATCH

	private:
#		if FLUX_NIH_LATCH == FLUX_NIH_LATCH_STDCONDVAR
		mutable std::mutex mMutex;
		mutable std::condition_variable mCV;
		std::ptrdiff_t mCount;

#		elif FLUX_NIH_LATCH == FLUX_NIH_LATCH_WAITONADDR
		dlapi::os::Win32api const* const mWin32;

		mutable std::atomic<std::ptrdiff_t> mCount;

		FLUX_STATIC_ASSERT( sizeof(std::atomic<std::ptrdiff_t>) == sizeof(std::ptrdiff_t) );

#		elif FLUX_NIH_LATCH == FLUX_NIH_LATCH_FUTEX
		mutable std::atomic<uint32_t> mCount;

		FLUX_STATIC_ASSERT( sizeof(std::atomic<uint32_t>) == sizeof(uint32_t) );
#		endif // ~ LATCH
};

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/latch.inl"
#endif // FLUX_NIH_LATCH_HPP_73064247_ED3A_46B2_A2C3_6F49B00E3D06
