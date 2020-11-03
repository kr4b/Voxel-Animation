/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Detect standard library features
 *
 * Detect and setup FLUX_STDFEAT_* and related constants.
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_STDFEAT_HXX_3CEC4B3E_ECBC_4C8D_B27C_C1B4AC423EFE
#define DETAIL_STDFEAT_HXX_3CEC4B3E_ECBC_4C8D_B27C_C1B4AC423EFE

//--//////////////////////////////////////////////////////////////////////////
//--    Feature tests               ///{{{1///////////////////////////////////

#include "stdfeat_conf-feature.hxx"

//--    Detection                   ///{{{1///////////////////////////////////

#if defined(FLUXCFG_STDFEAT_FROM_LANGLEVEL) && FLUXCFG_STDFEAT_FROM_LANGLEVEL
/* If FLUXCFG_STDFEAT_FROM_LANGLEVEL is defined and enabled, always get the
 * standard library features from the language level.
 */
#	include "stdfeat_conf-langlevel.hxx"

#else
/* Otherwise, determine these from the standard library.
 */
#	if FLUX_STDLIB == FLUX_STDLIB_STDCPP
#		include "stdfeat_conf-stdcpp.hxx"

#	elif FLUX_STDLIB == FLUX_STDLIB_LIBCPP
#		include "stdfeat_conf-libcpp.hxx"

#	elif FLUX_STDLIB == FLUX_STDLIB_VSCPP
#		include "stdfeat_conf-vscpp.hxx"

#	elif FLUX_STDLIB == FLUX_STDLIB_UNKNOWN
/* If the standard library is unknown, always determine supported features
 * from the language level.
 */
#		include "stdfeat_conf-langlevel.hxx"

#	else // sanity check
#		error "FLUX_STDLIB: unknown value"
#	endif // ~ FLUX_STDLIB

#endif // ~ FLUXCFG_STDFEAT_FROM_LANGLEVEL

/* And finally, apply fixups and workarounds...
 */
#include "stdfeat_fixups.hxx"

#if FLUX_COMPILER_COMPAT == FLUX_COMPILER_NVCC
#	include "stdfeat_fixups-nvcc.hxx"
#endif // ~ NVCC

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_STDFEAT_HXX_3CEC4B3E_ECBC_4C8D_B27C_C1B4AC423EFE
