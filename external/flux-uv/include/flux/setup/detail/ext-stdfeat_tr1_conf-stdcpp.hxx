/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	GNU libstdc++ TR1 library features
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_EXT_STDFEAT_TR1_CONF_STDCPP_HXX_DFC4A630_FC47_416C_84B6_4C37EB45D479
#define DETAIL_EXT_STDFEAT_TR1_CONF_STDCPP_HXX_DFC4A630_FC47_416C_84B6_4C37EB45D479

//--//////////////////////////////////////////////////////////////////////////
//--    __GLIBCXX__                 ///{{{1///////////////////////////////////

// ensure that we've got __GLIBCXX__
#include <cstddef>

#if !defined(__GLIBCXX__)
#	if !defined(__GLIBCPP__)
#		error "libstdc++ : can't detect version."
#	else
#		define __GLIBCXX__ __GLIBCPP__
#	endif // ~ __GLIBCPP__
#endif // ~ __GLIBCXX__

//--    TR1                         ///{{{1///////////////////////////////////

// Date from wayback machine. 
// https://web.archive.org/web/20090420033528/http://gcc.gnu.org/onlinedocs/
//   libstdc++/manual/status.html
#if __GLIBCXX__ >= 20090420
#	undef FLUX_STDFEAT_TR1_SHAREDPTR
#	define FLUX_STDFEAT_TR1_SHAREDPTR 1

#	undef FLUX_STDFEAT_TR1_FUNCTIONAL
#	define FLUX_STDFEAT_TR1_FUNCTIONAL 1

#	undef FLUX_STDFEAT_TR1_TYPE_TRAITS 
#	define FLUX_STDFEAT_TR1_TYPE_TRAITS 1

#	undef FLUX_STDFEAT_TR1_RANDOM
#	define FLUX_STDFEAT_TR1_RANDOM 1

#	undef FLUX_STDFEAT_TR1_EXTRAMATH
#	define FLUX_STDFEAT_TR1_EXTRAMATH 1

#	undef FLUX_STDFEAT_TR1_TUPLE
#	define FLUX_STDFEAT_TR1_TUPLE 1

#	undef FLUX_STDFEAT_TR1_UNORDERED
#	define FLUX_STDFEAT_TR1_UNORDERED 1
#endif // ~ __GLIBCXX__ >= 20090420

/* Nobody ever implemented the TR1 regex... As of 20141030, attempts to use
 * TR1 regex fail with a linking error. (The C++11 version does work, though.)
 */

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_EXT_STDFEAT_TR1_CONF_STDCPP_HXX_DFC4A630_FC47_416C_84B6_4C37EB45D479
