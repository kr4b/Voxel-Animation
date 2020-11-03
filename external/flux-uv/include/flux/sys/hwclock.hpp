/*-******************************************************* -- HEADER -{{{1- */
/*-	"Hardware" clock (where available)
 *
 * E.g., RDTSC(P) for x86.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SYS_HWCLOCK_HPP_D3DBC020_C049_41F3_B7AD_D2555D8D33D8
#define FLUX_SYS_HWCLOCK_HPP_D3DBC020_C049_41F3_B7AD_D2555D8D33D8

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>


#include <flux/util/attrib.hpp>
#include <flux/std/inttypes.hpp>

//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    macros                      ///{{{1///////////////////////////////////

#define FLUX_SYS_HWCLOCK_SUPPORTED  0 /*OVERRIDDEN_ELSEWHERE*/

//--    types                       ///{{{1///////////////////////////////////

using HwClockTicks = uint64_t;

enum EHwClockState
{
	error,
	success,
	unreliable
};

//--    functions                   ///{{{1///////////////////////////////////

/** Initialize "hardware" clock
 *
 * Checks if the hardware clock is supported and (if so) performs necessary
 * initialization. If the hardware clock is unusable (e.g., unsupported),
 * `hwclock_init()` returns false. If initialization succeeded and the
 * platform's hardware clock is usable, it returns true. Initialization data
 * may be cached internally, and is normally reused. This can be disabled by
 * setting \a aForce to true.
 *
 * The main purpose of the hardware clock is to aid with profiling/measuring
 * very short time spans, where other methods become inaccurate (e.g. QPC) and
 * where minimizing even minimal overheads is important (e.g. QPC and
 * `clock_gettime()`). <b>It should not be used for general-purpose timings.
 * Prefer the standard `std::chrono` clocks or the `flux::Clock` for this!</b>
 *
 * The hardware clock is architecture dependent, and not implemented for all
 * architectures. Currently, x86 and x86-64 use the `rdtscp` instruction (which
 * should be available on most recent CPUs).
 */
EHwClockState hwclock_init( bool aForce = false ) noexcept;

/** Read "hardware" clock
 *
 * Returns the current time of the "hardware" clock. See notes in
 * `hwclock_init()` for additional information. Values returned from this
 * function (and more importantly, differences between two values) are in
 * unspecified units; use `hwclock_to_ns()` to convert to nanoseconds.
 *
 * \see hwclock_init()
 */
FLUX_ATTR_ALWAYS_INLINE
HwClockTicks hwclock_now() noexcept;

uint64_t hwclock_to_ns( HwClockTicks ) noexcept;
//HwClockTicks hwclock_from_ns( uint64_t ) noexcept; //TODO

uint64_t hwclock_frequency_in_khz() noexcept;

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/hwclock.inl"
#endif // FLUX_SYS_HWCLOCK_HPP_D3DBC020_C049_41F3_B7AD_D2555D8D33D8
