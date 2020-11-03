/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Detect TR1 library features
 *
 * Detect and setup FLUX_STDFEAT_TR1_* and related constants.
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_EXT_STDFEAT_TR1_HXX_8869A0EF_2804_4E14_B223_1F7CC8BC4A15
#define DETAIL_EXT_STDFEAT_TR1_HXX_8869A0EF_2804_4E14_B223_1F7CC8BC4A15

//--//////////////////////////////////////////////////////////////////////////
//--    Select features             ///{{{1///////////////////////////////////

#if FLUX_STDLIB == FLUX_STDLIB_STDCPP
#	include "ext-stdfeat_tr1_conf-stdcpp.hxx"

#elif FLUX_STDLIB == FLUX_STDLIB_LIBCPP
	
	/* libcpp was never supported in flux-gianttoe */

#elif FLUX_STDLIB == FLUX_STDLIB_VSCPP
#	include "ext-stdfeat_tr1_conf-vscpp.hxx"

#elif FLUX_STDLIB == FLUX_STDLIB_UNKNOWN
/* Unknown - leave undefined */

#else // sanity check
#	error "FLUX_STDLIB: unknown value"
#endif // ~ FLUX_STDLIB

/* And finally, apply fixups and workarounds...
 */
#include "ext-stdfeat_tr1_fixups.hxx"

#if FLUX_COMPILER_COMPAT == FLUX_COMPILER_NVCC
#	include "ext-stdfeat_tr1_fixups-nvcc.hxx"
#endif // ~ NVCC


//--    TR1LIB_HEADER               ///{{{1///////////////////////////////////

/* Are the TR1 headers in a subdirectory?
 *
 * Normally: yes
 * On Visual Studio: no.
 */
#if FLUX_STDLIB == FLUX_STDLIB_VSCPP
#	define FLUXTMP_SETUP_TR1LIB_SUBDIR_ 0
#else
#	define FLUXTMP_SETUP_TR1LIB_SUBDIR_ 1
#endif

#if FLUXTMP_SETUP_TR1LIB_SUBDIR_
#	define FLUXTMP_SETUP_TR1LIB_HEADER_PATH_(name) tr1/name
#else
#	define FLUXTMP_SETUP_TR1LIB_HEADER_PATH_(name) name
#endif 

#define FLUXTMP_SETUP_TR1LIB_HEADER_QUOTE_(head) <head>

#undef FLUX_TR1LIB_HEADER
#define FLUX_TR1LIB_HEADER(head)                                   \
	FLUXTMP_SETUP_TR1LIB_HEADER_QUOTE_(                            \
		FLUXTMP_SETUP_TR1LIB_HEADER_PATH_(head)                    \
	) /*ENDM*/

#undef FLUXTMP_SETUP_TR1LIB_SUBDIR_

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_EXT_STDFEAT_TR1_HXX_8869A0EF_2804_4E14_B223_1F7CC8BC4A15
