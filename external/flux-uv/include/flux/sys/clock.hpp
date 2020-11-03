/*-******************************************************* -- HEADER -{{{1- */
/*-	Native system clock
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SYS_CLOCK_HPP_242CEDCC_C4D5_4D72_9F19_58686A31EDFA
#define FLUX_SYS_CLOCK_HPP_242CEDCC_C4D5_4D72_9F19_58686A31EDFA

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>


#include <chrono>

//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    SysClock                    ///{{{1///////////////////////////////////

/** Native system clock
 *
 * `SysClock` wraps the native high-performance OS-dependent `TimeSource` 
 * into a class that is compatible with the `std::chrono` APIs. 
 *
 * \see sys::TimeSource
 */
class SysClock
{
	public:
		typedef int64_t rep;
		typedef std::nano period;
		typedef std::chrono::duration<rep,period> duration;
		typedef std::chrono::time_point<SysClock> time_point;

	public:
		static time_point now() noexcept;

	public:
		/** Monotonically increasing?
		 *
		 * The native `SysSource` is typically "steady" (or, at least 
		 * monotonically increasing):
		 *   - `QueryPerformanceCounter()`:
		 *      https://msdn.microsoft.com/en-us/library/dn553408(v=vs.85).aspx
		 *   - `clock_gettime()` uses `CLOCK_MONOTONIC`, the monontonically
		 *      increasing clock (it might not be "steady", in the sense that
		 *      some time adjustments are still made).
		 *   - `mach_absolute_time()` which *presumably* is steady too.
		 */
		static constexpr bool is_steady = true;
};

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_SYS_CLOCK_HPP_242CEDCC_C4D5_4D72_9F19_58686A31EDFA
