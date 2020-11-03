/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Setup: MSVC
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_COMPILER_CONF_MSVC_FAE5D7DE_15F4_4FCA_99C7_7F50CC91EA67
#define DETAIL_COMPILER_CONF_MSVC_FAE5D7DE_15F4_4FCA_99C7_7F50CC91EA67

//--//////////////////////////////////////////////////////////////////////////
//--    FLUX_COMPILER*              ///{{{1///////////////////////////////////

#undef FLUX_COMPILER_ACTUAL
#define FLUX_COMPILER_ACTUAL FLUX_COMPILER_MSVC

#undef FLUX_COMPILER_FAMILY
#define FLUX_COMPILER_FAMILY FLUX_COMPILER_MSVC

//--    FLUX_LANGLEVEL              ///{{{1///////////////////////////////////

/* Note: Visual Studio doesn't set __cplusplus correctly at all times. Version
 * 2015 Update 3 introduces _MSVC_LANG, which can be used instead.
 */

#define FLUXTMP_LANGVER_ __cplusplus
#if _MSC_VER >= 1910 // Visual Studio 2017
#	undef FLUXTMP_LANGVER_
#	define FLUXMTP_LANGVER_ _MSVC_LANG
#endif // ~ _MSC_VER

#if _MSVC_LANG >= 201703L
#	undef FLUX_LANGLEVEL
#	define FLUX_LANGLEVEL FLUX_LANGLEVEL_CXX17

#elif _MSVC_LANG >= 201402L
#	undef FLUX_LANGLEVEL
#	define FLUX_LANGLEVEL FLUX_LANGLEVEL_CXX14

#elif _MSVC_LANG >= 201103L
#	undef FLUX_LANGLEVEL
#	define FLUX_LANGLEVEL FLUX_LANGLEVEL_CXX11
#endif

#undef FLUXTMP_LANGVER_

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_COMPILER_CONF_MSVC_FAE5D7DE_15F4_4FCA_99C7_7F50CC91EA67
