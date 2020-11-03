/*-******************************************************* -- HEADER -{{{1- */
/*- flux.report configuration
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_REPORT_DEFAULTS_HPP_7CB925AC_BB6E_442E_AAB4_2EAD05FFA538
#define FLUX_REPORT_DEFAULTS_HPP_7CB925AC_BB6E_442E_AAB4_2EAD05FFA538

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

//--    defaults            ///{{{1///////////////////////////////////////////

#if !defined(FLUXCFG_REPORT_BADNESS)
#	define FLUXCFG_REPORT_BADNESS 1
#endif // ~ !defined(FLUXCFG_REPORT_BADNESS)

#if !defined(FLUXCFG_REPORT_INSTRUMENT)
#	define FLUXCFG_REPORT_INSTRUMENT 1
#endif // ~ !defined(FLUXCFG_REPORT_INSTRUMENT)


//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_REPORT_DEFAULTS_HPP_7CB925AC_BB6E_442E_AAB4_2EAD05FFA538
