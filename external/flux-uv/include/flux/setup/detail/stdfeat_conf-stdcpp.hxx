/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	GNU libstdc++ standard library features
 *
 * This is a bit gnarly. Blame the extraordinary __GLIBCXX__ versioning
 * scheme.
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_STDFEAT_CONF_STDCPP_HXX_D2212F78_9AF1_417A_A996_DDBB843CA47F
#define DETAIL_STDFEAT_CONF_STDCPP_HXX_D2212F78_9AF1_417A_A996_DDBB843CA47F

//--//////////////////////////////////////////////////////////////////////////
//--    Fixups                      ///{{{1///////////////////////////////////

/* Mapping __GLIBCXX__ to a particular GCC version can be a bit tricky, see
 * the old ext-stdfeat_req_conf-stdcpp.hxx for a few links / sources.
 *
 * Additionally, stdfeat_fixups.hxx has a somewhat complete list for recent 
 * GCC releases (see GCC BUG 62258, and the related STDQUIRK_UNCAUGHT_RETHROW)
 */

#if !defined(__GLIBCXX__)
#	if !defined(__GLIBCPP__)
#		error "libstdc++ : can't detect version."
#	else
#		define __GLIBCXX__ __GLIBCPP__
#	endif // ~ __GLIBCPP__
#endif // ~ __GLIBCXX__

//--    C++11                       ///{{{1///////////////////////////////////

/* C++11 features require either full C++11 language level, or that 
 * __GXX_EXPERIMENTAL_CXX0X__ is defined.
 */
#if FLUX_LANGLEVEL >= FLUX_LANGLEVEL_CXX11 || defined(__GXX_EXPERIMENTAL_CXX0X__)
	/* value_at_thread_exit() & co unavailable as of 20141030. presumably
	 * available in >= GCC 5.0
	 *
	 * TODO: find GLIBCXXX version where this becomes available
	 */
#	undef FLUX_STDQUIRK_CXX11_NO_AT_THREAD_EXIT
#	define FLUX_STDQUIRK_CXX11_NO_AT_THREAD_EXIT 1

	/* `aligned_union<>` missing until GCC 5.x as well */
#	undef FLUX_STDQUIRK_CXX11_NO_ALIGNED_UNION
#	define FLUX_STDQUIRK_CXX11_NO_ALIGNED_UNION 1

	/* `is_trivially_*<>` generally missing until GCC 5.x */
#	undef FLUX_STDQUIRK_CXX11_NO_TRIVIALLY_TRAITS
#	define FLUX_STDQUIRK_CXX11_NO_TRIVIALLY_TRAITS 1
	/* `assert()` unusable in `constexpr` functions until 5.x */
#	undef FLUX_STDQUIRK_CXX11_NONCONSTEXPR_ASSERT
#	define FLUX_STDQUIRK_CXX11_NONCONSTEXPR_ASSERT 1
	/* old style `uninitialized_fill_n()` (...) functions until 5.x */
#	undef FLUX_STDQUIRK_CXX11_OLD_UNINITIALIZED
#	define FLUX_STDQUIRK_CXX11_OLD_UNINITIALIZED 1

#	if __GLIBCXX__ >= 20150405 /* GCC 5.0.0-alpha20150405 */ && __GLIBCXX__ != 20150623 /* 4.8.5 */ && __GLIBCXX__ != 20150626 /* 4.9.3 */  && __GLIBCXX__ != 20160726 /* 4.9.4*/
#		undef FLUX_STDQUIRK_CXX11_NO_AT_THREAD_EXIT
#		define FLUX_STDQUIRK_CXX11_NO_AT_THREAD_EXIT 0

#		undef FLUX_STDQUIRK_CXX11_NO_TRIVIALLY_TRAITS
#		define FLUX_STDQUIRK_CXX11_NO_TRIVIALLY_TRAITS 0

#		undef FLUX_STDQUIRK_CXX11_NO_ALIGNED_UNION
#		define FLUX_STDQUIRK_CXX11_NO_ALIGNED_UNION 0

#		undef FLUX_STDQUIRK_CXX11_NONCONSTEXPR_ASSERT
#		define FLUX_STDQUIRK_CXX11_NONCONSTEXPR_ASSERT 0

#		undef FLUX_STDQUIRK_CXX11_OLD_UNINITIALIZED
#		define FLUX_STDQUIRK_CXX11_OLD_UNINITIALIZED 0

		/* <system_error> works from now on regardless of the runtime version
		 * of the libstdc++.
		 */
#		undef FLUX_STDFEAT_CXX11_SYSERROR
#		define FLUX_STDFEAT_CXX11_SYSERROR 1
#	endif // ~ __GLIBCXX__ >= 20150405

#endif // ~ full or experimental C++11 language support

//--    C++14                       ///{{{1///////////////////////////////////

/* C++11 features require either full C++14 language level, or that 
 * __cplusplus is strictly larger than 201103L (= -std=C++1y)
 */
#if FLUX_LANGLEVEL >= FLUX_LANGLEVEL_CXX14 || __cplusplus > 201103L /* c++1y */

#	if __GLIBCXX__ >= 20140422 /* 4.9.0 */ && __GLIBCXX__ != 20140522 /* 4.8.3 */ && __GLIBCXX__ != 20141219 /* 4.8.4 */ && __GLIBCXX__ != 20150623 /* 4.8.5 */
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
#	endif // ~ __GLIBCXX__ >= 20140422

#endif // ~ full or experimental C++14 language support

//--    C++17                      ///{{{1///////////////////////////////////

/* As far as I know, the libstdc++ iterator_traits have always been SFINAE
 * friendly, and thus this is available from the get go
 */
#undef FLUX_STDFEAT_CXX17_ITERATOR_TRAITS
#define FLUX_STDFEAT_CXX17_ITERATOR_TRAITS 1

#if FLUX_LANGLEVEL >= FLUX_LANGLEVEL_CXX17 || __cplusplus > 201402L /* c++1z */

#	if __GLIBCXX__ >= 20160427 /* 6.1.x */ && __GLIBCXX__ != 20171010 /* 5.5.x */ && __GLIBCXX__ != 20160603 /* 5.4.x */
		// TODO: perhaps earlier (6.x)?
#		undef FLUX_STDFEAT_CXX17_VOIDT
#		undef FLUX_STDFEAT_CXX17_SHARED_MUTEX
#		undef FLUX_STDFEAT_CXX17_UNCAUGHT_EXCEPTIONS

	/* Note: missing std::invoke_result until 7.1 */

#		define FLUX_STDFEAT_CXX17_VOIDT   1
#		define FLUX_STDFEAT_CXX17_SHARED_MUTEX  1
#		define FLUX_STDFEAT_CXX17_UNCAUGHT_EXCEPTIONS 1
#	endif // ~ __GLIBCXX__ >= 20160427

#	if __GLIBCXX__ >= 20170215 /* 7.0.1 20170215 snapshot */ && __GLIBCXX__ != 20171010 /* 5.5.x */
#		undef FLUX_STDFEAT_CXX17_CLAMP
#		undef FLUX_STDFEAT_CXX17_OPTIONAL
#		undef FLUX_STDFEAT_CXX17_LAUNDER

#		define FLUX_STDFEAT_CXX17_CLAMP     1
#		define FLUX_STDFEAT_CXX17_OPTIONAL  1
#		define FLUX_STDFEAT_CXX17_LAUNDER   1
#	endif // ~ __GLIBCXX__ >= 20170215

#	if __GLIBCXX__ >= 20170502 /* 7.1.0 */ && __GLIBCXX__ != 20171010 /* 5.5.x */
		// TODO: perhaps earlier?
#		undef FLUX_STDFEAT_CXX17_BYTE
#		undef FLUX_STDFEAT_CXX17_VARIANT
#		undef FLUX_STDFEAT_CXX17_STRING_VIEW
#		undef FLUX_STDFEAT_CXX17_TYPE_TRAITS
#		undef FLUX_STDFEAT_CXX17_UNINITIALIZED
#		undef FLUX_STDFEAT_CXX17_INVOKE

#		define FLUX_STDFEAT_CXX17_BYTE           1
#		define FLUX_STDFEAT_CXX17_VARIANT        1
#		define FLUX_STDFEAT_CXX17_STRING_VIEW    1
#		define FLUX_STDFEAT_CXX17_TYPE_TRAITS    1
#		define FLUX_STDFEAT_CXX17_UNINITIALIZED  1
#		define FLUX_STDFEAT_CXX17_INVOKE         1
#	endif // ~ __GLIBCXX__ >= 20170502

#	if __GLIBCXX__ >= 20170814 /* 7.2.x */ && __GLIBCXX__ != 20171010 /* 5.5.x */
#	endif // ~ _GLIBCXX__ >= 20170814

#endif // ~ full or experimental C++17 language support

//MISSING: STDFEAT_CXX17_FILESYSTEM

//--    C++20/2a                    ///{{{1///////////////////////////////////

#if FLUX_LANGLEVEL >= FLUX_LANGLEVEL_CXX20 || __cplusplus > 201709L /* C++2a GCC 8.1 */

#	if __GLIBCXX__ >= 20180502 /* 8.1.0 */
#		undef FLUX_STDFEAT_CXX20_TO_ADDRESS
#		define FLUX_STDFEAT_CXX20_TO_ADDRESS 1
#	endif // ~ __GLIBCXX__ >= 20180502

#	if __GLIBCXX__ >= 20190503 /* 9.1.0 */
#		undef FLUX_STDFEAT_CXX20_IS_CONSTEVAL
#		define FLUX_STDFEAT_CXX20_IS_CONSTEVAL 1
#	endif // ~ __GLIBCXX__ >= 20190503

#endif // ~ full or experimental C++20 language support

//MISSING: STDFEAT_CXX20_SPAN
//MISSING: STDFEAT_CXX20_BIT_CAST
//MISSING: STDFEAT_CXX20_SOURCE_LOCATION
//MISSING: STDFEAT_CXX20_BARRIER

//--    experimental features       ///{{{1///////////////////////////////////

#if FLUX_LANGLEVEL >= FLUX_LANGLEVEL_CXX11 || defined(__GXX_EXPERIMENTAL_CXX0X__)
/* Note: <experimental/filesystem> seems to be unavailable on Cygwin, even if
 * the version of the libstd++ indicates otherwise. See stdfeat_fixup.hxx for
 * the Cygwin-specific fixup that disables <filesystem> again.
 */
#	if __GLIBCXX__ >= 20151204 /* GCC 5.3 */ && __GLIBCXX__ != 20160726 /*4.9.4*/
#		undef FLUX_STDFEAT_XP_FILESYSTEM
#		define FLUX_STDFEAT_XP_FILESYSTEM 1
#	endif //  ~ __GLIBCXX__ >= 20151204
#endif // ~ full or experimental C++11 language support

#if FLUX_LANGLEVEL >= FLUX_LANGLEVEL_CXX14 || __cplusplus > 201103L /* c++1y */
/* This header requires support for the forthcoming ISO 2014 C++ standard */
#	if __GLIBCXX__ >= 20140422 /* GCC 4.9.0 */
#		undef FLUX_STDFEAT_XP_OPTIONAL
#		define FLUX_STDFEAT_XP_OPTIONAL 1
#	endif // ~ __GLIBCXX__ >= 20140422

/* GCC 6.1 seems to support experimental/observer_ptr, but doesn't set the
 * feature test macro. *le sigh*
 *
 * XXX- check GCC 6.0?
 */
#	if __GLIBCXX__ >= 20160427 /* GCC 6.1.0 */ && __GLIBCXX__ != 20160726 /*4.9.4*/ && __GLIBCXX__ != 20160603 /*5.4.0*/
#		undef FLUX_STDFEAT_XP_OBSERVER_PTR
#		define FLUX_STDFEAT_XP_OBSERVER_PTR 1
#	endif // ~ __GLIBCXX__ >= 20160527

/* GCC 7.2 seems to support experimental/source_location, but doesn't set the
 * appropriate feature test macro.
 */
#	if __GLIBCXX__ >= 20170815 /* GCC 7.2.0 */
#		undef FLUX_STDFEAT_XP_SOURCE_LOCATION
#		define FLUX_STDFEAT_XP_SOURCE_LOCATION 1
#	endif // ~ __GLIBCXX__ >= 20170815
#endif // ~ full or experimental C++14 language support


//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_STDFEAT_CONF_STDCPP_HXX_D2212F78_9AF1_417A_A996_DDBB843CA47F
