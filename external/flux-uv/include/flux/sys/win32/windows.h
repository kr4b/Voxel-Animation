/*-******************************************************* -- HEADER -{{{1- */
/*- Wrapper: include <windows.h> but with a wee bit less insanity.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SYS_WIN32_WINDOWS_H_02267B08_5510_4F65_B3B3_DB5798A1773D
#define FLUX_SYS_WIN32_WINDOWS_H_02267B08_5510_4F65_B3B3_DB5798A1773D

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////
#if !defined(__DOXYGEN__) // Don't put this into doxygen.

/* For compat: were near & far defined as macros? Ugh :-( */
#if defined(near)
#	define FLUXTMP_HAS_NEAR_ 1
#else
#	define FLUXTMP_HAS_NEAR_ 0
#endif
#if defined(far)
#	define FLUXTMP_HAS_FAR_ 1
#else
#	define FLUXTMP_HAS_FAR_ 0
#endif

/* Make sure NOMINMAX is defined when including <windows.h> .. because evil. */
#if !defined(NOMINMAX)
#	define FLUXTMP_UNDEF_NOMINMAX_ 1
#	define NOMINMAX 1 /* See http://support.microsoft.com/kb/143208 */
#else
#	define FLUXTMP_UNDEF_NOMINMAX_ 0
#endif

/* Make sure to remove other cruft. */
#if !defined(WIN32_LEAN_AND_MEAN)
#	define FLUXTMP_UNDEF_LAM_ 1
#	define WIN32_LEAN_AND_MEAN 1 /* See http://support.microsoft.com/kb/166474 */
#else
#	define FLUXTMP_UNDEF_LAM_ 0
#endif

/* Include <windows.h> */
#include <windows.h>

/* Clean up stuff we defined */
#if FLUXTMP_UNDEF_LAM_
#	undef WIN32_LEAN_AND_MEAN
#endif

#if FLUXTMP_UNDEF_NOMINMAX_
#	undef NOMINMAX
#endif

#undef FLUXTMP_UNDEF_LAM_
#undef FLUXTMP_UNDEF_NOMINMAX_

/* Clean up near/far macros */
#if !FLUXTMP_HAS_FAR_ && defined(far)
#	undef far
#endif

#if !FLUXTMP_HAS_NEAR_ && defined(near)
#	undef near
#endif

#undef FLUXTMP_HAS_FAR_
#undef FLUXTMP_HAS_NEAR_

#endif // ~ !deefined(__DOXYGEN__)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_SYS_WIN32_WINDOWS_H_02267B08_5510_4F65_B3B3_DB5798A1773D
