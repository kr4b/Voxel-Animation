/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Language features from language level
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_EXT_LANGFEAT_REQ_CONF_MSVC_HXX_846EEE62_8019_4681_80F6_F784374F7BA9
#define DETAIL_EXT_LANGFEAT_REQ_CONF_MSVC_HXX_846EEE62_8019_4681_80F6_F784374F7BA9

//--//////////////////////////////////////////////////////////////////////////
//--    C++11                       ///{{{1///////////////////////////////////

// See https://msdn.microsoft.com/en-us/library/vstudio/hh567368(v=vs.110).aspx

#if _MSC_VER >= 1600 // Visual Studio 2010
#	undef FLUX_LANGFEAT_DECLTYPE
#	undef FLUX_LANGFEAT_AUTO_TYPE
#	undef FLUX_LANGFEAT_STATIC_ASSERT
#	undef FLUX_LANGFEAT_RVALUE_REFERENCE
#	undef FLUX_LANGFEAT_LAMBDA_EXPRESSIONS
#	undef FLUX_LANGFEAT_OVERRIDE_CONTROL
#	undef FLUX_LANGFEAT_TRAILING_RETURN
#	undef FLUX_LANGFEAT_TYPED_ENUM
#	undef FLUX_LANGFEAT_NULLPTR

#	define FLUX_LANGFEAT_DECLTYPE 1
#	define FLUX_LANGFEAT_AUTO_TYPE 1
#	define FLUX_LANGFEAT_STATIC_ASSERT 1
#	define FLUX_LANGFEAT_RVALUE_REFERENCE 1
#	define FLUX_LANGFEAT_LAMBDA_EXPRESSIONS 1
#	define FLUX_LANGFEAT_OVERRIDE_CONTROL 1 /* Partial: nonstandard keyword */
#	define FLUX_LANGFEAT_TRAILING_RETURN 1
#	define FLUX_LANGFEAT_TYPED_ENUM 1 /* Partial */
#	define FLUX_LANGFEAT_NULLPTR 1
#endif // ~ Visual Studio 2010

#if _MSC_VER >= 1700 // Visual Studio 2012
#	undef FLUX_LANGFEAT_RANGE_FOR
#	define FLUX_LANGFEAT_RANGE_FOR 1
#endif // ~ Visual Studio 2012

#if _MSC_VER >= 1800 // Visual Studio 2013
#	undef FLUX_LANGFEAT_INITIALIZER_LIST
#	undef FLUX_LANGFEAT_TEMPLATE_ALIASES
#	undef FLUX_LANGFEAT_VARIADIC_TEMPLATES
#	undef FLUX_LANGFEAT_EXPLICIT_CTORS
#	undef FLUX_LANGFEAT_EXPLICIT_CONVOPS
#	undef FLUX_LANGFEAT_DEF_FN_TEMPLATE

#	define FLUX_LANGFEAT_INITIALIZER_LIST 1
#	define FLUX_LANGFEAT_TEMPLATE_ALIASES 1
#	define FLUX_LANGFEAT_VARIADIC_TEMPLATES 1
#	define FLUX_LANGFEAT_EXPLICIT_CTORS 1 /* Partial: no default moves */
#	define FLUX_LANGFEAT_EXPLICIT_CONVOPS 1
#	define FLUX_LANGFEAT_DEF_FN_TEMPLATE 1 /* NOTE: 2013 or earlier/later? */

#	if _MSC_FULL_VER >= 180021114 // Visual C++ Compiler November 2013 CTP
/* See http://blogs.msdn.com/b/vcblog/archive/2013/11/18/announcing-the-visual-c-compiler-november-2013-ctp.aspx */
//#		undef FLUX_LANGFEAT_CONSTEXPR /* Partial: Not on member fns */
//#		define FLUX_LANGFEAT_CONSTEXPR 1

//#		undef FLUX_LANGFEAT_NOEXCEPT /* Partial^W Not at all */
//#		define FLUX_LANGFEAT_NOEXCEPT 1

//#		undef FLUX_LANGQUIRK_PARTIAL_NOEXCEPT
//#		define FLUX_LANGQUIRK_PARTIAL_NOEXCEPT 1
#	endif // ~ CTP
#endif // ~ Visual Studio 2013

#if _MSC_VER >= 1900 // Visual Studio 2015
	/* Following tested on _MSC_FULL_VER = 190023918 */
#	undef FLUX_LANGFEAT_USER_LITERAL
#	undef FLUX_LANGFEAT_REF_QUALIFIED_FN

#	define FLUX_LANGFEAT_USER_LITERAL 1
#	define FLUX_LANGFEAT_REF_QUALIFIED_FN 1
#endif // ~ Visual Studio 2015

//--    C++11, dropping VS2013      ///{{{1///////////////////////////////////

#undef FLUX_LANGQUIRK_NO_DEFAULT_MOVE
#undef FLUX_LANGQUIRK_PARTIAL_THREAD_LOCAL

#define FLUX_LANGQUIRK_NO_DEFAULT_MOVE 1
#define FLUX_LANGQUIRK_PARTIAL_THREAD_LOCAL 1

// See https://msdn.microsoft.com/en-us/library/vstudio/hh567368(v=vs.110).aspx

#if _MSC_VER >= 1900 // Visual Studio 2015
	/* Following tested on _MSC_FULL_VER = 190023918 */
#	undef FLUX_LANGFEAT_ALIGNMENT
#	undef FLUX_LANGFEAT_CONSTEXPR
#	undef FLUX_LANGFEAT_INHERIT_CTOR
#	undef FLUX_LANGFEAT_INLINE_NAMESPACE
#	undef FLUX_LANGFEAT_NOEXCEPT
#	undef FLUX_LANGFEAT_NEW_CHARS
#	undef FLUX_LANGFEAT_REF_QUALIFIERS
#	undef FLUX_LANGFEAT_STDATTRIBUTE
#	undef FLUX_LANGFEAT_THREAD_LOCAL
#	undef FLUX_LANGFEAT_USER_LITERAL

#	define FLUX_LANGFEAT_ALIGNMENT 1
#	define FLUX_LANGFEAT_CONSTEXPR 1
#	define FLUX_LANGFEAT_INHERIT_CTOR 1
#	define FLUX_LANGFEAT_INLINE_NAMESPACE 1
#	define FLUX_LANGFEAT_NOEXCEPT 1
#	define FLUX_LANGFEAT_NEW_CHARS 1
#	define FLUX_LANGFEAT_REF_QUALIFIERS 1
#	define FLUX_LANGFEAT_STDATTRIBUTE 1
#	define FLUX_LANGFEAT_THREAD_LOCAL 1
#	define FLUX_LANGFEAT_USER_LITERAL 1


#	undef FLUX_LANGQUIRK_NO_DEFAULT_MOVE
#	undef FLUX_LANGQUIRK_PARTIAL_THREAD_LOCAL

#	define FLUX_LANGQUIRK_NO_DEFAULT_MOVE 0
#	define FLUX_LANGQUIRK_PARTIAL_THREAD_LOCAL 0
#endif // ~ Visual Studio 2015

//--    C++14                       ///{{{1///////////////////////////////////

#if _MSC_VER >= 1900 // Visual Studio 2015
#	undef FLUX_LANGFEAT_ATTR_DEPRECATED
#	undef FLUX_LANGFEAT_BINARY_LITERAL
#	undef FLUX_LANGFEAT_DECLTYPE_AUTO
#	undef FLUX_LANGFEAT_GENERALIZED_LAMBDA
#	undef FLUX_LANGFEAT_GENERIC_LAMBDA

#	define FLUX_LANGFEAT_ATTR_DEPRECATED 1
#	define FLUX_LANGFEAT_BINARY_LITERAL 1
#	define FLUX_LANGFEAT_DECLTYPE_AUTO 1
#	define FLUX_LANGFEAT_GENERALIZED_LAMBDA 1
#	define FLUX_LANGFEAT_GENERIC_LAMBDA 1
#endif // ~ Visual Studio 2015

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_EXT_LANGFEAT_REQ_CONF_MSVC_HXX_846EEE62_8019_4681_80F6_F784374F7BA9
