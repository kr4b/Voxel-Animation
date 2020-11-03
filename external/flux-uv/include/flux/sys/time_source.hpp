/*-******************************************************* -- HEADER -{{{1- */
/*- Native system timers
 *
 * Gets the "best" available time source. See the `SysClock` (clock.hpp) 
 * wrapper for more friendly interface.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SYS_TIME_SOURCE_HPP_941C97F2_3555_4F04_B33C_429FCCBB3212
#define FLUX_SYS_TIME_SOURCE_HPP_941C97F2_3555_4F04_B33C_429FCCBB3212

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/platform.hpp>

#include <flux/std/inttypes.hpp>


#include <chrono>

//--    defines                     ///{{{1///////////////////////////////////

#define FLUX_SYS_TIMER_UNKNOWN       0
#define FLUX_SYS_TIMER_CLOCKGETTIME  1 //!< use `clock_gettime()`
#define FLUX_SYS_TIMER_QUERYPC       2 //!< use `QueryPerformanceCounter()`
#define FLUX_SYS_TIMER_MACHTIME      3 //1< use "mach_absolute_time()`

/** Native Timer API
 *
 * Identifies the native timer API used to implement `sys::TimeSource`. 
 * `FLUX_SYS_TIMER` is equal to one of the `FLUX_SYS_TIMER_*` constants.
 */
#define FLUX_SYS_TIMER  FLUX_SYS_TIMER_UNKNOWN /*OVERRIDDEN_ELSEWHERE*/

//--    select timer                ///{{{1///////////////////////////////////
#if FLUX_PLATCAP_WIN32LIB
#	undef FLUX_SYS_TIMER
#	define FLUX_SYS_TIMER FLUX_SYS_TIMER_QUERYPC 

#elif FLUX_PLATFORM == FLUX_PLATFORM_LINUX
#	undef FLUX_SYS_TIMER
#	define FLUX_SYS_TIMER FLUX_SYS_TIMER_CLOCKGETTIME 

#elif FLUX_PLATFORM == FLUX_PLATFORM_ANDROID
#	undef FLUX_SYS_TIMER
#	define FLUX_SYS_TIMER FLUX_SYS_TIMER_CLOCKGETTIME 

#elif FLUX_PLATFORM == FLUX_PLATFORM_MACOS
#	undef FLUX_SYS_TIMER
#	define FLUX_SYS_TIMER FLUX_SYS_TIMER_MACHTIME

#endif // ~ timer API

//--    headers                     ///{{{1///////////////////////////////////
#if FLUX_SYS_TIMER == FLUX_SYS_TIMER_QUERYPC
#	include <flux/sys/win32/windows.h>

#elif FLUX_SYS_TIMER == FLUX_SYS_TIMER_CLOCKGETTIME
#	include <unistd.h>

#	if defined(_POSIX_TIMERS)
#		include <time.h>
#	else
#		undef FLUX_SYS_TIMER
#		define FLUX_SYS_TIMER FLUX_SYS_TIMER_UNKNOWN
#	endif // ~ defined(_POSIX_TIMERS)

#elif FLUX_SYS_TIMER == FLUX_SYS_TIMER_MACHTIME
extern "C" {
#	include <mach/mach_time.h>
}
#endif // ~ FLUX_SYS_TIMER

#if FLUX_SYS_TIMER == FLUX_SYS_TIMER_UNKNOWN
#	error "Fallback timer API not yet available"
#endif // ~ FLUX_SYS_TIMER

//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    constants                   ///{{{1///////////////////////////////////
namespace detail
{
	constexpr int64_t kNSecPerSec = 1000000000ll;
}

//--    types                       ///{{{1///////////////////////////////////

#if FLUX_SYS_TIMER == FLUX_SYS_TIMER_QUERYPC
/** Native representation of a point in time
 */
typedef LARGE_INTEGER TimePoint;
/** Native data about a timer
 */
typedef LARGE_INTEGER TimerFreqData;

#elif FLUX_SYS_TIMER == FLUX_SYS_TIMER_CLOCKGETTIME
typedef timespec TimePoint;
typedef timespec TimerFreqData;

#elif FLUX_SYS_TIMER == FLUX_SYS_TIMER_MACHTIME
typedef uint64_t TimePoint;
typedef mach_timebase_info_data_t TimerFreqData;
#endif // ~ FLUX_SYS_TIMER

//--    classes                     ///{{{1///////////////////////////////////
/** Native time source
 *
 * Native time source using an OS-dependent high performance time source. The
 * OS-dependent time source employed is identified by `FLUX_SYS_TIMER`.
 *
 * Example:
   \code
   TimeSource timeSource;
   std::chrono::nanoseconds t0 = timeSource.elapsed<std::chrono::nanoseconds>();
   \endcode
 *
 * \note The constructor doesn't throw exceptions, enabling the use of static
 * `TimeSource` instances. Rarely, if ever, the native API may not be supported
 * or available, in which case the `TimeSource` is invalid, and queries to it
 * return undefined values. Validity can be checked with the `valid()` member
 * method.
 *
 * \see flux::sys::Clock, flux::Clock
 */
class TimeSource
{
	public:
		/* Note: never throws, to enable static initialization. If there's no
		 * usable time source available, valid() will return false.
		 */
		inline TimeSource() noexcept;

	public:
		/** Is TimeSource valid?
		 *
		 * Returns true if the TimeSource is valid and usable, false otherwise.
		 */
		inline bool valid() const noexcept;
		/** Is TimeSource steady?
		 *
		 * Returns true if the TimeSource is guaranteed to increase 
		 * monotonically.
		 */
		inline bool steady() const noexcept;

		/** Read time
		 *
		 * Read time into the native representation of time.
		 */
		inline bool read_time( TimePoint& ) const noexcept;
		/** Read frequency information
		 */
		inline bool read_freq( TimerFreqData& ) const noexcept;

		/** Read elapsed time
		 *
		 * Read time elapsed since some point in the past.
		 */
		template< class tDuration >
		inline tDuration elapsed() const noexcept;
		/** Compute time elapsed between two native time points
		 *
		 * Computes the time elapsed between the starting time \a aA and the
		 * ending time \a aB.
		 */
		template< class tDuration >
		inline tDuration delta( TimePoint const& aA, TimePoint const& aB ) const noexcept;

		/** Compute approximate resolution
		 *
		 * Computes the approximate resolution, and returns the smallest time
		 * step that the native time source can take. 
		 *
		 * \note This is largely informational, and in some cases the actual
		 * measurements have a much lower resolution, i.e., the actual minimal
		 * time step that the clock can resolve might be larger than the value
		 * returned by `resolution()`.
		 */
		template< class tDuration >
		inline tDuration resolution() const noexcept;

	private:
		TimePoint mStartTime;

#		if FLUX_SYS_TIMER == FLUX_SYS_TIMER_QUERYPC || FLUX_SYS_TIMER == FLUX_SYS_TIMER_MACHTIME 
		TimerFreqData mFreqData;
#		endif // ~ FLUX_SYS_TIMER

#		if FLUX_SYS_TIMER == FLUX_SYS_TIMER_CLOCKGETTIME
		clockid_t mClockId;
#		endif // ~ FLUX_SYS_TIMER_CLOCKGETTIME
};

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/time_source.inl"
#endif // FLUX_SYS_TIME_SOURCE_HPP_941C97F2_3555_4F04_B33C_429FCCBB3212
