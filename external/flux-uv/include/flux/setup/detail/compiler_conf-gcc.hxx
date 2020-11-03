/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Setup: GCC / G++
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_COMPILER_CONF_GCC_HXX_5A71627E_4E9C_4BFC_88E9_DBD15CE00C16
#define DETAIL_COMPILER_CONF_GCC_HXX_5A71627E_4E9C_4BFC_88E9_DBD15CE00C16

//--//////////////////////////////////////////////////////////////////////////
//--    FLUX_COMPILER*              ///{{{1///////////////////////////////////

#undef FLUX_COMPILER_ACTUAL
#define FLUX_COMPILER_ACTUAL FLUX_COMPILER_GCC

#undef FLUX_COMPILER_FAMILY
#define FLUX_COMPILER_FAMILY FLUX_COMPILER_GCC

//--    FLUX_LANGLEVEL              ///{{{1///////////////////////////////////

#if defined(__cplusplus) && __cplusplus >= 201703L
#	undef FLUX_LANGLEVEL
#	define FLUX_LANGLEVEL FLUX_LANGLEVEL_CXX17

#elif defined(__cplusplus) && __cplusplus >= 201402L
#	undef FLUX_LANGLEVEL
#	define FLUX_LANGLEVEL FLUX_LANGLEVEL_CXX14

#elif defined(__cplusplus) && __cplusplus >= 201103L
#	undef FLUX_LANGLEVEL
#	define FLUX_LANGLEVEL FLUX_LANGLEVEL_CXX11
#endif

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_COMPILER_CONF_GCC_HXX_5A71627E_4E9C_4BFC_88E9_DBD15CE00C16
