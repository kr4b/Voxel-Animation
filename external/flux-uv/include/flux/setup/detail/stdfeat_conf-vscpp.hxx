/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Standard library features for Visual Studio
 *
 * The STL by STL.
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_STDFEAT_CONF_VSCPP_HXX_42067DD8_EA4D_410D_927E_F7C67EA32691
#define DETAIL_STDFEAT_CONF_VSCPP_HXX_42067DD8_EA4D_410D_927E_F7C67EA32691

//--//////////////////////////////////////////////////////////////////////////
//--    C++11                       ///{{{1///////////////////////////////////

#undef FLUX_STDQUIRK_CXX11_NO_TRIVIALLY_TRAITS
#define FLUX_STDQUIRK_CXX11_NO_TRIVIALLY_TRAITS 1

// call_once() with exceptions has non-standard behaviour
#undef FLUX_STDQUIRK_CXX11_CALL_ONCE_EXCEPT
#define FLUX_STDQUIRK_CXX11_CALL_ONCE_EXCEPT 1


#if _MSC_VER >= 1700 /* Visual Studio 2012 */
#	undef FLUX_STDFEAT_CXX11_SYSERROR
#	define FLUX_STDFEAT_CXX11_SYSERROR      1
#endif // ~ Visual Studio 2012

#if _MSC_VER >= 1900 /* Visual Studio 2015 */
#	undef FLUX_STDQUIRK_CXX11_NO_TRIVIALLY_TRAITS
#	define FLUX_STDQUIRK_CXX11_NO_TRIVIALLY_TRAITS 0

#	undef FLUX_STDQUIRK_CXX11_CALL_ONCE_EXCEPT
#	define FLUX_STDQUIRK_CXX11_CALL_ONCE_EXCEPT 0
#endif // ~ Visual Studio 2015	

//--    C++14                       ///{{{1///////////////////////////////////

#if _MSC_VER >= 1800 /* Visual Studio 2013 */
#	undef FLUX_STDFEAT_CXX14_MAKE_UNIQUE
#	define FLUX_STDFEAT_CXX14_MAKE_UNIQUE 1
#endif // ~ Visual Studio 2013

// TODO-update: verified with the following, but possibly available earlier?
#if _MSC_FULL_VER >= 190024210 /* Visual Studio 2015 U3 */
#	undef FLUX_STDFEAT_CXX14_INTEGER_SEQ
#	undef FLUX_STDFEAT_CXX14_TUPLES_BY_TYPE
#	undef FLUX_STDFEAT_CXX14_EXCHANGE

#	define FLUX_STDFEAT_CXX14_INTEGER_SEQ 1
#	define FLUX_STDFEAT_CXX14_TUPLES_BY_TYPE 1
#	define FLUX_STDFEAT_CXX14_EXCHANGE 1

	// Although mostly available, some things (e.g., std::decay_t seems to be 
	// broken here.
//#	undef FLUX_STDFEAT_CXX14_TYPE_TRAITS
//#	define FLUX_STDFEAT_CXX14_TYPE_TRAITS 1
#endif // ~ Visual Studio 2015 U3

#if _MSC_VER >= 1910 // Visual Studio 2017 (probably)
#	undef FLUX_STDFEAT_CXX14_TYPE_TRAITS
#	define FLUX_STDFEAT_CXX14_TYPE_TRAITS 1
#endif // ~ Visual Studio 2017

//--    C++17                       ///{{{1///////////////////////////////////

/* Verify this: but Visual Studio 2015 may have had SFINAE friendly 
 * iterator_traits already
 */
#if _MSC_VER >= 1900
#	undef FLUX_STDFEAT_CXX17_ITERATOR_TRAITS
#	define FLUX_STDFEAT_CXX17_ITERATOR_TRAITS 1
#endif // ~ Visual Studio 2015

/* _MSVC_LANG availble from at least Visual Studio 2015 Update -- new language
 * features are also no longer automatically available. The language version is
 * controlled via
 *  - /std:c++14 (default) ==> _MSVC_LANG == 201402
 *  - /std:c++latest ==> _MSVC_LANG > 201402 (at the time of writing)
 *      e.g. 201704
 */

#if defined(_MSVC_LANG) && _MSVC_LANG > 201402

#	if _MSC_FULL_VER >=  190024210 // Visual Studio 2015 Update 3
#		undef FLUX_STDFEAT_CXX17_CLAMP
#		undef FLUX_STDFEAT_CXX17_SHARED_MUTEX // Maybe Update 2?
//#		undef FLUX_STDFEAT_CXX17_INVOKE
	
	/* Note: missing std::invoke_result<> until Visual Studio 2018 PREVIEW */

#		define FLUX_STDFEAT_CXX17_CLAMP 1
#		define FLUX_STDFEAT_CXX17_SHARED_MUTEX 1
//#		define FLUX_STDFEAT_CXX17_INVOKE 1
#	endif // ~ Visual Studio 2015 Update 3

// TODO-update: verified with the following, but possibly available earlier?
#	if _MSC_FULL_VER >= 191024629 /* CL 19 RC */
#		undef FLUX_STDFEAT_CXX17_OPTIONAL
#		define FLUX_STDFEAT_CXX17_OPTIONAL 1
#	endif // ~ CL 19 RC

// TODO-update: verified with the following, but possibly available earlier?
#	if _MSC_FULL_VER >= 191025017 /* CL 19 */
#		undef FLUX_STDFEAT_CXX17_VOIDT
#		undef FLUX_STDFEAT_CXX17_VARIANT
#		undef FLUX_STDFEAT_CXX17_TYPE_TRAITS
#		undef FLUX_STDFEAT_CXX17_STRING_VIEW

#		define FLUX_STDFEAT_CXX17_VOIDT 1
#		define FLUX_STDFEAT_CXX17_VARIANT 1
#		define FLUX_STDFEAT_CXX17_TYPE_TRAITS 1
#		define FLUX_STDFEAT_CXX17_STRING_VIEW 1
#	endif // ~ CL 19

#	if _MSC_VER >= 1912 // Visual Studio 2017 15.??; FULL = 191225830
#		undef FLUX_STDFEAT_CXX17_BYTE
#		undef FLUX_STDFEAT_CXX17_INVOKE
#		undef FLUX_STDFEAT_CXX17_UNINITIALIZED
#		undef FLUX_STDFEAT_CXX17_UNCAUGHT_EXCEPTIONS

#		define FLUX_STDFEAT_CXX17_BYTE 1
#		define FLUX_STDFEAT_CXX17_INVOKE 1
#		define FLUX_STDFEAT_CXX17_UNINITIALIZED 1
#		define FLUX_STDFEAT_CXX17_UNCAUGHT_EXCEPTIONS 1
#	endif // ~ VS 2017 Update 15.??

#	if _MSC_VER >= 1914 /* Visual Studio 2018 PREVIEW; FULL = 191426423 */
		// LAUNDER and FILESYSTEM are not on 191326128
#		undef FLUX_STDFEAT_CXX17_LAUNDER
#		undef FLUX_STDFEAT_CXX17_FILESYSTEM

#		define FLUX_STDFEAT_CXX17_LAUNDER 1
#		define FLUX_STDFEAT_CXX17_FILESYSTEM 1
#	endif // ~ Visual Studio 2018 PREVIEW

#endif // ~ _MSVC_LANG > 201402

//--    C++20/2a                    ///{{{1///////////////////////////////////

//MISSING: STDFEAT_CXX20_SPAN
//MISSING: STDFEAT_CXX20_BIT_CAST
//MISSING: STDFEAT_CXX20_TO_ADDRESS
//MISSING: STDFEAT_CXX20_IS_CONSTEVAL
//MISSING: STDFEAT_CXX20_SOURCE_LOCATION
//MISSING: STDFEAT_CXX20_BARRIER

//--    experimental features       ///{{{1///////////////////////////////////

#if _MSC_VER >= 1900 /* Visual Studio 2015 */
	/* Apparently, a version of <filesystem> has been around size 2012,
	 * although it's being described as "hairy". 
	 *
	 * The version in 2013 is definitively different enough from the standard
	 * to not really be that useful 
	 */
#	undef FLUX_STDFEAT_XP_FILESYSTEM
#	define FLUX_STDFEAT_XP_FILESYSTEM 1
#endif // ~ _MSC_VER >= 1900
#if _MSC_VER >= 1923 /* Visual Studio 2019.? */
	/* <experimental/filesystem> is deprecated in VisualStudio 2019 
	 */
#	undef FLUX_STDFEAT_XP_FILESYSTEM
#	define FLUX_STDFEAT_XP_FILESYSTEM 0
#endif // ~ _MSC_VER >= 1923

//MISSING: STDFEAT_XP_OPTIONAL
//MISSING: STDFEAT_XP_SOURCE_LOCATION
//MISSING: STDFEAT_XP_OBSERVER_PTR

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_STDFEAT_CONF_MSVC_HXX_42067DD8_EA4D_410D_927E_F7C67EA32691
