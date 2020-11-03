/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Language features from language level
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_LANGFEAT_CONF_MSVC_HXX_0DF84CA3_3E4A_40D2_A88D_B61C13C49889
#define DETAIL_LANGFEAT_CONF_MSVC_HXX_0DF84CA3_3E4A_40D2_A88D_B61C13C49889

//--//////////////////////////////////////////////////////////////////////////
//--    C++14                       ///{{{1///////////////////////////////////

#if _MSC_VER >= 1900 // Visual Studio 2015
#	undef FLUX_LANGFEAT_SIZED_DEALLOCATIONS
#	define FLUX_LANGFEAT_SIZED_DEALLOCATIONS 1
#endif // ~ Visual Studio 2015

#if _MSC_FULL_VER >= 190024210 // Visual Studio 2015 Update 3 maybe
	/* cppreference lists cl.exe version 14.2, which I guess corresponds to
	 * Visual Studio 2015 Update 2. Couldn't find the _MSC_FULL_VER for that. 
	 *
	 * Did anybody ever tell the VS dudes that their versioning system with
	 * multiple concurrent schemes is fucking obnoxious? As is their inability
	 * to have this information easily accessible.
	 */
#	undef FLUX_LANGFEAT_VARIABLE_TEMPLATE
#	define FLUX_LANGFEAT_VARIABLE_TEMPLATE 1
#endif // ~ Visual Studio 2015 Update 3 maybe.

#if _MSC_VER >= 1910 // Visual Studio 2017 PREVIEW
#	undef FLUX_LANGFEAT_EXTENDED_CONSTEXPR
#	define FLUX_LANGFEAT_EXTENDED_CONSTEXPR 1
#endif // ~ Visual Studio 2017 PREVIEW

#if _MSC_VER <= 1916 // msvc v19.16 on godbolt
#	undef FLUX_LANGQUIRK_PARTIAL_STDATTRIB
#	define FLUX_LANGQUIRK_PARTIAL_STDATTRIB 1
#endif // ~ v19.16

//--    C++17                       ///{{{1///////////////////////////////////

/* _MSVC_LANG availble from at least Visual Studio 2015 Update -- new language
 * features are also no longer automatically available. The language version is
 * controlled via
 *  - /std:c++14 (default) ==> _MSVC_LANG == 201402
 *  - /std:c++17 ==> _MSVC_LANG == 201703
 *  - /std:c++latest ==> _MSVC_LANG >= 201703 (201704 at the time of writing)
 */
#if defined(_MSVC_LANG) && _MSVC_LANG >= 201703

#	if _MSC_FULL_VER >= 190024210 // Visual Studio 2015 Update 3 maybe
#		undef FLUX_LANGFEAT_NESTED_NAMESPACES
#		define FLUX_LANGFEAT_NESTED_NAMESPACES 1
#	endif // ~ Visual Studio 2015 Update 3 maybe

#	if _MSC_VER >= 1910 // Visual Studio 2017 PREVIEW
#		undef FLUX_LANGFEAT_ATTR_FALLTHROUGH
#		undef FLUX_LANGFEAT_SHORT_STATIC_ASSERT

#		define FLUX_LANGFEAT_ATTR_FALLTHROUGH 1
#		define FLUX_LANGFEAT_SHORT_STATIC_ASSERT 1
#	endif // ~ Visual Studio 2017 PREVIEW

#	if _MSC_FULL_VER >= 191125601 // Visual Studio 2017 Update 3 maybe
#		undef FLUX_LANGFEAT_CONSTEXPR_IF
#		undef FLUX_LANGFEAT_STRUCTURED_BINDINGS
#		undef FLUX_LANGFEAT_ATTR_NODISCARD
#		undef FLUX_LANGFEAT_ATTR_MAYBE_UNUSED

#		define FLUX_LANGFEAT_CONSTEXPR_IF 1
#		define FLUX_LANGFEAT_STRUCTURED_BINDINGS 1
#		define FLUX_LANGFEAT_ATTR_NODISCARD 1
#		define FLUX_LANGFEAT_ATTR_MAYBE_UNUSED 1
#	endif // ~ Visual Studio 2017.3

#	if _MSC_VER >= 1914 // Visual Studio 2018 PREVIEW; FULL VER = 191426423
#		undef FLUX_LANGFEAT_INIT_STATEMENT
#		undef FLUX_LANGFEAT_TEMPLATE_AUTO
#		undef FLUX_LANGFEAT_HEXFLOAT
#		undef FLUX_LANGFEAT_INLINE_VARIABLE
#		undef FLUX_LANGFEAT_TYPEDEF_NOEXCEPT

#		define FLUX_LANGFEAT_INIT_STATEMENT 1
#		define FLUX_LANGFEAT_TEMPLATE_AUTO 1
#		define FLUX_LANGFEAT_HEXFLOAT 1
#		define FLUX_LANGFEAT_INLINE_VARIABLE 1
#		define FLUX_LANGFEAT_TYPEDEF_NOEXCEPT 1
#	endif // ~ Visual Studio 2018 PREVIEW

#endif // ~ _MSVC_LANG >= 201703 (C++17)

//--    C++2a                       ///{{{1///////////////////////////////////

// missing: LANGFEAT_CONSTEVAL

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_LANGFEAT_CONF_MSVC_HXX_0DF84CA3_3E4A_40D2_A88D_B61C13C49889
