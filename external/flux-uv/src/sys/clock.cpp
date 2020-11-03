/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	native clock implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/sys/clock.hpp>
#include <flux/sys/time_source.hpp>

#include <chrono>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    SysClock                            ///{{{2///////////////////////////

	// static -{{{2-
SysClock::time_point SysClock::now() noexcept
{
	static sys::TimeSource timeSource;
	return time_point( timeSource.elapsed<duration>() );
}
	
	// consts -{{{2-
constexpr bool SysClock::is_steady;

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
