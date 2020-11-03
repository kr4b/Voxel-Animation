/*-******************************************************* -- HEADER -{{{1- */
/*-	source_location
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_SOURCE_LOCATION_HPP_54281570_8838_46FB_9831_69767138A0BE
#define FLUX_COMPAT_SOURCE_LOCATION_HPP_54281570_8838_46FB_9831_69767138A0BE

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/stdfeat.hpp>

//--    source_location             ///{{{1///////////////////////////////////

#if FLUX_STDFEAT_CXX20_SOURCE_LOCATION
#	include <source_location>
#	define FLUX_COMPAT_SOURCE_LOCATION_IS_STUB 0

FLUX_NAMESPACE_ENTER1(compat)
	using std::source_location;
FLUX_NAMESPACE_LEAVE1(compat)

#elif FLUX_STDFEAT_XP_SOURCE_LOCATION
#	include <experimental/source_location>
#	define FLUX_COMPAT_SOURCE_LOCATION_IS_STUB 0

FLUX_NAMESPACE_ENTER1(compat)
	using std::experimental::source_location;
FLUX_NAMESPACE_LEAVE1(compat)

#else // fallback to nih_
#	include <flux/nih_/source_location.hpp>
#	define FLUX_COMPAT_SOURCE_LOCATION_IS_STUB FLUX_NIH_SOURCE_LOCATION_IS_STUB

FLUX_NAMESPACE_ENTER1(compat)
	using nih_::source_location;
FLUX_NAMESPACE_LEAVE1(compat)

#endif // ~ implementations

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_SOURCE_LOCATION_HPP_54281570_8838_46FB_9831_69767138A0BE
