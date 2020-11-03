/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	libc++ required standard library features
 *
 * Since support for libc++ was added later, we'll mainly just assume that
 * versions >= 3700 support everything necessary. _LIBCPP_VERSION == 3700 is
 * the first version where the libc++ version derives from the Clang version,
 * according to godbolt.org. Clang 3.6 has _LIBCPP_VERSION == 1101. Versions
 * prior to and including 3.4.1, do not include <ciso646>.
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_EXT_STDFEAT_REQ_CONF_LIBCPP_HXX_941E933B_2164_4E13_A852_BB1FC5A5692A
#define DETAIL_EXT_STDFEAT_REQ_CONF_LIBCPP_HXX_941E933B_2164_4E13_A852_BB1FC5A5692A

//--//////////////////////////////////////////////////////////////////////////
//--    checks                      ///{{{1///////////////////////////////////

#if !defined(_LIBCPP_VERSION)
#	error "libc++ : can't detect version."
#endif // ~ _LIBCPP_VERSION

//--    C++11                       ///{{{1///////////////////////////////////

#if FLUX_LANGLEVEL >= FLUX_LANGLEVEL_CXX11

#	if _LIBCPP_VERSION >= 3700
#		undef FLUX_STDFEAT_CXX11_TYPE_TRAITS
#		define FLUX_STDFEAT_CXX11_TYPE_TRAITS   1

#		undef FLUX_STDFEAT_CXX11_FUNCTIONAL
#		define FLUX_STDFEAT_CXX11_FUNCTIONAL    1

#		undef FLUX_STDFEAT_CXX11_TUPLE
#		define FLUX_STDFEAT_CXX11_TUPLE         1

#		undef FLUX_STDFEAT_CXX11_UNIQUEPTR
#		define FLUX_STDFEAT_CXX11_UNIQUEPTR     1

#		undef FLUX_STDFEAT_CXX11_SHAREDPTR
#		define FLUX_STDFEAT_CXX11_SHAREDPTR     1

#		undef FLUX_STDFEAT_CXX11_UNORDERED
#		define FLUX_STDFEAT_CXX11_UNORDERED     1

#		undef FLUX_STDFEAT_CXX11_RANDOM
#		define FLUX_STDFEAT_CXX11_RANDOM        1

#		undef FLUX_STDFEAT_CXX11_CHRONO
#		define FLUX_STDFEAT_CXX11_CHRONO        1

#		undef FLUX_STDFEAT_CXX11_THREAD
#		define FLUX_STDFEAT_CXX11_THREAD        1

#		undef FLUX_STDFEAT_CXX11_FUTURE
#		define FLUX_STDFEAT_CXX11_FUTURE        1

#		undef FLUX_STDFEAT_CXX11_RATIO
#		define FLUX_STDFEAT_CXX11_RATIO         1

#		undef FLUX_STDFEAT_CXX11_EXCEPTPTR
#		define FLUX_STDFEAT_CXX11_EXCEPTPTR     1

#		undef FLUX_STDFEAT_CXX11_ATOMIC
#		define FLUX_STDFEAT_CXX11_ATOMIC 1

#		undef FLUX_STDQUIRK_CXX11_NO_UNDERLYING_TYPE
#		define FLUX_STDQUIRK_CXX11_NO_UNDERLYING_TYPE 0

#		undef FLUX_STDFEAT_CXX11_PTR_TRAITS
#		define FLUX_STDFEAT_CXX11_PTR_TRAITS 1
#	endif // ~ _LIBCPP_VERSION >= 3700

#endif // ~ C++11 language support

/* Added after dropping VS2013 support: */

#if FLUX_LANGLEVEL >= FLUX_LANGLEVEL_CXX11
#	if _LIBCPP_VERSION >= 3700
#		undef FLUX_STDFEAT_CXX11_BADARRAYNEW
#		define FLUX_STDFEAT_CXX11_BADARRAYNEW 1

#		undef FLUX_STDFEAT_CXX11_REGEX
#		define FLUX_STDFEAT_CXX11_REGEX 1
#	endif // ~ _LIBCPP_VERSION
#endif // ~ C++11 language support

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_EXT_STDFEAT_REQ_CONF_LIBCPP_HXX_941E933B_2164_4E13_A852_BB1FC5A5692A
