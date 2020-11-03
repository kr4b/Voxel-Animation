/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Clang libc++/libcxx standard library features
 *
 * Support for libc++ was added later. For now, we assume that versions of
 * _LIBCPP_VERSION >= 3700 include full C++11 features. The listings on
 * cppreference[1] seem to agree.
 *
 * [1] https://en.cppreference.com/w/cpp/compiler_support
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_STDFEAT_CONF_LIBCPP_HXX_D36D985D_8038_4A15_9F13_260018F203E6
#define DETAIL_STDFEAT_CONF_LIBCPP_HXX_D36D985D_8038_4A15_9F13_260018F203E6

//--//////////////////////////////////////////////////////////////////////////
//--    checks                      ///{{{1///////////////////////////////////

#if !defined(_LIBCPP_VERSION)
#	error "libc++ : can't detect version."
#endif // ~ _LIBCPP_VERSION

//--    C++11                       ///{{{1///////////////////////////////////

#if FLUX_LANGLEVEL >= FLUX_LANGLEVEL_CXX11
#	if _LIBCPP_VERSION >= 3700
#		undef FLUX_STDFEAT_CXX11_SYSERROR
#		define FLUX_STDFEAT_CXX11_SYSERROR 1
#	endif // ~ _LIBCPP_VERSION
#endif // ~ C++11 language support

//--    C++14                       ///{{{1///////////////////////////////////

/* According to cppreference, libc++ 3.4 includes full C++14 support. Again,
 * _LIBCPP_VERSION >= 3700 is used, since that's the earliest version that
 * follows the Clang version naming.
 */

#if FLUX_LANGLEVEL >= FLUX_LANGLEVEL_CXX14 || __cplusplus > 201103L /* c++1y */
#	if _LIBCPP_VERSION >= 3700
#		undef FLUX_STDFEAT_CXX14_MAKE_UNIQUE
#		undef FLUX_STDFEAT_CXX14_TUPLES_BY_TYPE
#		undef FLUX_STDFEAT_CXX14_TYPE_TRAITS
#		undef FLUX_STDFEAT_CXX14_INTEGER_SEQ
#		undef FLUX_STDFEAT_CXX14_EXCHANGE

#		define FLUX_STDFEAT_CXX14_MAKE_UNIQUE     1
#		define FLUX_STDFEAT_CXX14_TUPLES_BY_TYPE  1
#		define FLUX_STDFEAT_CXX14_TYPE_TRAITS     1
#		define FLUX_STDFEAT_CXX14_INTEGER_SEQ     1
#		define FLUX_STDFEAT_CXX14_EXCHANGE        1
#	endif // ~ _LIBCPP_VERSION
#endif // ~ full or experimental C++14 language support

//--    C++17                      ///{{{1///////////////////////////////////

#if FLUX_LANGLEVEL >= FLUX_LANGLEVEL_CXX17 || __cplusplus > 201402L /* c++1z */
#	if _LIBCPP_VERSION >= 3700
		/* Note: shared mutex may have been available earlier even. */
#		undef FLUX_STDFEAT_CXX17_UNCAUGHT_EXCEPTIONS
#		undef FLUX_STDFEAT_CXX17_VOIDT
#		undef FLUX_STDFEAT_CXX17_SHARED_MUTEX

#		define FLUX_STDFEAT_CXX17_UNCAUGHT_EXCEPTIONS  1
#		define FLUX_STDFEAT_CXX17_VOIDT          1
#		define FLUX_STDFEAT_CXX17_SHARED_MUTEX   1
#	endif // ~ _LIBCPP_VERSION

#	if _LIBCPP_VERSION >= 3800
#		undef FLUX_STDFEAT_CXX17_TYPE_TRAITS
#		define FLUX_STDFEAT_CXX17_TYPE_TRAITS    1
#	endif // ~ _LIBCPP_VERSION

#	if _LIBCPP_VERSION >= 3900
#		undef FLUX_STDFEAT_CXX17_CLAMP
#		define FLUX_STDFEAT_CXX17_CLAMP          1
#	endif // ~ _LIBCPP_VERSION

#	if _LIBCPP_VERSION >= 4000
#		undef FLUX_STDFEAT_CXX17_OPTIONAL
#		undef FLUX_STDFEAT_CXX17_STRING_VIEW
#		undef FLUX_STDFEAT_CXX17_UNINITIALIZED

#		define FLUX_STDFEAT_CXX17_OPTIONAL       1
#		define FLUX_STDFEAT_CXX17_STRING_VIEW    1
#		define FLUX_STDFEAT_CXX17_UNINITIALIZED  1
#	endif // ~ _LIBCPP_VERSION

#	if _LIBCPP_VERSION >= 5000
#		undef FLUX_STDFEAT_CXX17_BYTE
#		define FLUX_STDFEAT_CXX17_BYTE           1
#	endif // ~ _LIBCPP_VERSION


#	if _LIBCPP_VERSION >= 6000
#		undef FLUX_STDFEAT_CXX17_INVOKE
#		undef FLUX_STDFEAT_CXX17_LAUNDER

#		define FLUX_STDFEAT_CXX17_INVOKE         1
#		define FLUX_STDFEAT_CXX17_LAUNDER        1
#	endif // ~ _LIBCPP_VERSION

#	if _LIBCPP_VERSION >= 7000
#		undef FLUX_STDFEAT_CXX17_FILESYSTEM
#		undef FLUX_STDFEAT_CXX17_VARIANT

#		define FLUX_STDFEAT_CXX17_FILESYSTEM     1
#		define FLUX_STDFEAT_CXX17_VARIANT        1
#	endif // ~ _LIBCPP_VERSION
#endif // ~ full or experimental C++17 language support

//--    C++20/2a                    ///{{{1///////////////////////////////////

#if FLUX_LANGLEVEL >= FLUX_LANGLEVEL_CXX20 || __cplusplus > 201707L // Clang 7
#	if _LIBCPP_VERSION >= 6000
#		undef FLUX_STDFEAT_CXX20_TO_ADDRESS
#		define FLUX_STDFEAT_CXX20_TO_ADDRESS     1
#	endif // ~ _LIBCPP_VERSION

#	if _LIBCPP_VERSION >= 7000
#		undef FLUX_STDFEAT_CXX20_SPAN
#		define FLUX_STDFEAT_CXX20_SPAN           1
#	endif // ~ _LIBCPP_VERSION

#	if _LIBCPP_VERSION >= 9000
#		undef FLUX_STDFEAT_CXX20_IS_CONSTEVAL
#		define FLUX_STDFEAT_CXX20_IS_CONSTEVAL   1
#	endif // ~ _LIBCPP_VERSION

#endif // ~ full or experimental C++20 language support

//MISSING: STDFEAT_CXX20_BIT_CAST
//MISSING: STDFEAT_CXX20_SOURCE_LOCATION
//MISSING: STDFEAT_CXX20_BARRIER

//--    experimental features       ///{{{1///////////////////////////////////

// Not supported under libc++.

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_STDFEAT_CONF_LIBCPP_HXX_D36D985D_8038_4A15_9F13_260018F203E6
