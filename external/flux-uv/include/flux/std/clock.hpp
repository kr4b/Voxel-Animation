/*-******************************************************* -- HEADER -{{{1- */
/*-	Standard clock
 *
 * Note: used to provide a fallback to sys::SysClock for early versions where
 * std::chrono::high_resolution_clock was inaccurate. Current standard library
 * implementation no longer require this, but the shortcut to flux::Clock is
 * nevertheless useful.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_CLOCK_HPP_43C4B82C_06D7_4E0B_BE19_C6EE2519BFCC
#define FLUX_STD_CLOCK_HPP_43C4B82C_06D7_4E0B_BE19_C6EE2519BFCC

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/namespace.hpp>

#include <chrono>

//--    Clock                      ///{{{1///////////////////////////////////

FLUX_NAMESPACE_ENTER()
	typedef std::chrono::high_resolution_clock Clock;
FLUX_NAMESPACE_LEAVE()

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_STD_CLOCK_HPP_43C4B82C_06D7_4E0B_BE19_C6EE2519BFCC
