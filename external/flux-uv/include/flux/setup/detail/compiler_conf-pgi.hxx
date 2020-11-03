/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Setup: PGI
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_COMPILER_CONF_PGI_HXX_E02ADC80_3DD5_49A4_A772_821DDAB02671
#define DETAIL_COMPILER_CONF_PGI_HXX_E02ADC80_3DD5_49A4_A772_821DDAB02671

//--//////////////////////////////////////////////////////////////////////////
//--    FLUX_COMPILER*              ///{{{1///////////////////////////////////

#undef FLUX_COMPILER_ACTUAL
#define FLUX_COMPILER_ACTUAL FLUX_COMPILER_PGI

#undef FLUX_COMPILER_FAMILY
#define FLUX_COMPILER_FAMILY FLUX_COMPILER_PGI

//--    FLUX_LANGLEVEL              ///{{{1///////////////////////////////////

#if defined(__cplusplus) && __cplusplus >= 201402L
#	undef FLUX_LANGLEVEL
#	define FLUX_LANGLEVEL FLUX_LANGLEVEL_CXX14

#elif defined(__cplusplus) && __cplusplus >= 201103L
#	undef FLUX_LANGLEVEL
#	define FLUX_LANGLEVEL FLUX_LANGLEVEL_CXX11
#endif

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_COMPILER_CONF_PGI_HXX_E02ADC80_3DD5_49A4_A772_821DDAB02671
