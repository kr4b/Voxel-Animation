/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Required standard library features
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_EXT_STDFEAT_REQUIRED_HXX_75D24CE4_C9DD_40BD_9201_80E1F450175D
#define DETAIL_EXT_STDFEAT_REQUIRED_HXX_75D24CE4_C9DD_40BD_9201_80E1F450175D

//--//////////////////////////////////////////////////////////////////////////
//--    Select features             ///{{{1///////////////////////////////////

#if defined(FLUXCFG_STDFEAT_FROM_LANGLEVEL) && FLUXCFG_STDFEAT_FROM_LANGLEVEL
/* If FLUXCFG_STDFEAT_FROM_LANGLEVEL is defined and enabled, always get the
 * standard library features from the language level.
 */
#	include "ext-stdfeat_req_conf-langlevel.hxx"

#else
/* Otherwise, determine these from the standard library.
 */
#	if FLUX_STDLIB == FLUX_STDLIB_STDCPP
#		include "ext-stdfeat_req_conf-stdcpp.hxx"

#	elif FLUX_STDLIB == FLUX_STDLIB_LIBCPP
#		include "ext-stdfeat_req_conf-libcpp.hxx"

#	elif FLUX_STDLIB == FLUX_STDLIB_VSCPP
#		include "ext-stdfeat_req_conf-vscpp.hxx"

#	elif FLUX_STDLIB == FLUX_STDLIB_UNKNOWN
/* If the standard library is unknown, always determine supported features
 * from the language level.
 */
#		include "ext-stdfeat_req_conf-langlevel.hxx"

#	else // sanity check
#		error "FLUX_STDLIB: unknown value"
#	endif // ~ FLUX_STDLIB

#endif // ~ FLUXCFG_STDFEAT_FROM_LANGLEVEL

/* And finally, apply fixups and workarounds...
 */
#include "ext-stdfeat_req_fixups.hxx"

#if FLUX_COMPILER_COMPAT == FLUX_COMPILER_NVCC
#	include "ext-stdfeat_req_fixups-nvcc.hxx"
#endif // ~ nvcc
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_EXT_STDFEAT_REQUIRED_HXX_75D24CE4_C9DD_40BD_9201_80E1F450175D
