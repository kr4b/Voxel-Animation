/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Setup: Intel ICC
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_COMPILER_CONF_INTEL_HXX_100AEDD9_4094_41C5_BBC0_61BCB3FD6B1A
#define DETAIL_COMPILER_CONF_INTEL_HXX_100AEDD9_4094_41C5_BBC0_61BCB3FD6B1A

//--//////////////////////////////////////////////////////////////////////////
//--    FLUX_COMPILER*              ///{{{1///////////////////////////////////

#undef FLUX_COMPILER_ACTUAL
#define FLUX_COMPILER_ACTUAL FLUX_COMPILER_INTEL

#undef FLUX_COMPILER_FAMILY
#define FLUX_COMPILER_FAMILY FLUX_COMPILER_INTEL

//--    FLUX_LANGLEVEL              ///{{{1///////////////////////////////////

#if defined(__cplusplus) && __cplusplus >= 201402L
#	undef FLUX_LANGLEVEL
#	define FLUX_LANGLEVEL FLUX_LANGLEVEL_CXX14

#elif defined(__cplusplus) && __cplusplus >= 201103L
#	undef FLUX_LANGLEVEL
#	define FLUX_LANGLEVEL FLUX_LANGLEVEL_CXX11
#endif

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_COMPILER_CONF_INTEL_HXX_100AEDD9_4094_41C5_BBC0_61BCB3FD6B1A
