/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- StreamCaps implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/compat/constexpr.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    StreamCaps                          ///{{{2///////////////////////////

inline FLUX_CONSTEXPR_EX
StreamCaps::StreamCaps() noexcept
	: readable(0)
	, writable(0)
	, size(0)
	, tell(0)
	, seekFwd(0)
	, seekRev(0)
	, remaining(0)
	, infinite(0)
	, clonable(0)
	//, lockable(0)
{}

//--    <<< ~ flux::io namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
