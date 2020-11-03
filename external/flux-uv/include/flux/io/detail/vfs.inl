/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- VfsStat implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/compat/constexpr.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    VfsStat                             ///{{{2///////////////////////////

inline FLUX_CONSTEXPR_EX
VfsStat::VfsStat() noexcept
	: removable(0)
	, file(0)
	, readable(0)
	, writable(0)
	, mappable(0)
	, sized(0)
	, directory(0)
	, accessible(0)
	, listable(0)
	, changeable(0)
	, flat(0)
	, special(0)
	, hasNativeMap(0)
	, hasNativePath(0)
	//, vfsName(0)
{}

//--    <<< ~ flux::io namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
