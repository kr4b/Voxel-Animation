/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	GCC language features
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_EXT_LANGFEAT_REQ_CONF_GCC_HXX_3AF6FC36_F24E_4B21_9CFC_CE8DF972B23C
#define DETAIL_EXT_LANGFEAT_REQ_CONF_GCC_HXX_3AF6FC36_F24E_4B21_9CFC_CE8DF972B23C

//--//////////////////////////////////////////////////////////////////////////
//--    C++11                       ///{{{1///////////////////////////////////

#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)

// Features supported since GCC 4.3
#	undef FLUX_LANGFEAT_RVALUE_REFERENCE
#	define FLUX_LANGFEAT_RVALUE_REFERENCE 1

#	undef FLUX_LANGFEAT_VARIADIC_TEMPLATES
#	define FLUX_LANGFEAT_VARIADIC_TEMPLATES 1

#	undef FLUX_LANGFEAT_DECLTYPE
#	define FLUX_LANGFEAT_DECLTYPE 1

#	undef FLUX_LANGFEAT_DEF_FN_TEMPLATE
#	define FLUX_LANGFEAT_DEF_FN_TEMPLATE 1

// Features supported since GCC 4.4
#	if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 4)
#		undef FLUX_LANGFEAT_INITIALIZER_LIST
#		define FLUX_LANGFEAT_INITIALIZER_LIST 1

#		undef FLUX_LANGFEAT_STATIC_ASSERT
#		define FLUX_LANGFEAT_STATIC_ASSERT 1

#		undef FLUX_LANGFEAT_AUTO_TYPE
#		define FLUX_LANGFEAT_AUTO_TYPE 1

#		undef FLUX_LANGFEAT_EXPLICIT_CTORS
#		define FLUX_LANGFEAT_EXPLICIT_CTORS 1

#		undef FLUX_LANGFEAT_TRAILING_RETURN
#		define FLUX_LANGFEAT_TRAILING_RETURN 1

#		undef FLUX_LANGFEAT_TYPED_ENUM
#		define FLUX_LANGFEAT_TYPED_ENUM 1

#	endif // ~ GCC 4.4 and later

// Features supported since GCC 4.5
#	if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)
		/* Lambda expressions supported, but GCC Bug 48523 prevents capturing
		 * "this" in a member method of a template class. Fixed in 4.6.1
		 */
//#		undef FLUX_LANGFEAT_LAMBDA_EXPRESSIONS
//#		define FLUX_LANGFEAT_LAMBDA_EXPRESSIONS 1

#		undef FLUX_LANGFEAT_EXPLICIT_CONVOPS
#		define FLUX_LANGFEAT_EXPLICIT_CONVOPS 1
#	endif // ~ GCC 4.5 and later

// Features supported since GCC 4.6
#	if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#		undef FLUX_LANGFEAT_LAMBDA_EXPRESSIONS
#		define FLUX_LANGFEAT_LAMBDA_EXPRESSIONS 1

#		undef FLUX_LANGFEAT_RANGE_FOR
#		define FLUX_LANGFEAT_RANGE_FOR 1

#		undef FLUX_LANGFEAT_NULLPTR
#		define FLUX_LANGFEAT_NULLPTR 1
#	endif // ~ GCC 4.7 and later

// Features supported since GCC 4.7
#	if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)
#		undef FLUX_LANGFEAT_TEMPLATE_ALIASES
#		define FLUX_LANGFEAT_TEMPLATE_ALIASES 1

#		undef FLUX_LANGFEAT_OVERRIDE_CONTROL
#		define FLUX_LANGFEAT_OVERRIDE_CONTROL 1

		// Experimental: does GCC 4.7 have std::underlying_type?
//#		undef FLUX_LANGFEAT_TYPED_ENUM
//#		define FLUX_LANGFEAT_TYPED_ENUM 1
#	endif // ~ GCC 4.7 and later

#endif // ~ __cplusplus || _GXX_EXPERIMENTAL_CXX0X_

//--    C++11, dropping VS2013      ///{{{1///////////////////////////////////

#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)

// Features supported since GCC 4.4
#	if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 4)
#		undef FLUX_LANGFEAT_INLINE_NAMESPACE
#		define FLUX_LANGFEAT_INLINE_NAMESPACE 1

#		undef FLUX_LANGFEAT_NEW_CHARS
#		define FLUX_LANGFEAT_NEW_CHARS 1
#	endif // ~ GCC 4.4 and later

// Features supported since GCC 4.6
#	if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#		undef FLUX_LANGFEAT_CONSTEXPR
#		define FLUX_LANGFEAT_CONSTEXPR 1

#		undef FLUX_LANGFEAT_NOEXCEPT
#		define FLUX_LANGFEAT_NOEXCEPT 1
#	endif // ~ GCC 4.7 and later

// Features supported since GCC 4.7
#	if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)
#		undef FLUX_LANGFEAT_USER_LITERAL
#		define FLUX_LANGFEAT_USER_LITERAL 1
#	endif // ~ GCC 4.7 and later

// Features supported since GCC 4.8
#	if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)
#		undef FLUX_LANGFEAT_ALIGNMENT
#		define FLUX_LANGFEAT_ALIGNMENT 1

#		undef FLUX_LANGFEAT_INHERIT_CTOR
#		define FLUX_LANGFEAT_INHERIT_CTOR 1

#		undef FLUX_LANGFEAT_STDATTRIBUTE
#		define FLUX_LANGFEAT_STDATTRIBUTE 1

#		undef FLUX_LANGFEAT_THREAD_LOCAL
#		define FLUX_LANGFEAT_THREAD_LOCAL 1
#	endif // ~ GCC 4.8 and later

// Only available from patchlevel 4.8.1 on and onward, according to
// http://en.cppreference.com/w/cpp/compiler_support
#	if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 8) || (__GNUC__ == 4 && __GNUC_MINOR__ == 8 && __GNUC_PATCHLEVEL__ >= 1)
#		undef FLUX_LANGFEAT_REF_QUALIFIERS
#		define FLUX_LANGFEAT_REF_QUALIFIERS 1
#	endif // ~ GCC 4.8.1 and later

#endif // ~ __cplusplus || _GXX_EXPERIMENTAL_CXX0X_

//--    C++14                       ///{{{1///////////////////////////////////

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3)
#	undef FLUX_LANGFEAT_BINARY_LITERAL
#	define FLUX_LANGFEAT_BINARY_LITERAL 1
#endif // ~ GCC 4.3

#if __cplusplus >= 201300L /* Used by GCC 4.x for std=c++1y. */

#	if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)
#		undef FLUX_LANGFEAT_DECLTYPE_AUTO
#		define FLUX_LANGFEAT_DECLTYPE_AUTO 1
#	endif // ~ GCC 4.9

#	if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 9)
#		undef FLUX_LANGFEAT_ATTR_DEPRECATED
#		define FLUX_LANGFEAT_ATTR_DEPRECATED 1

#		undef FLUX_LANGFEAT_GENERALIZED_LAMBDA
#		define FLUX_LANGFEAT_GENERALIZED_LAMBDA 1

#		undef FLUX_LANGFEAT_GENERIC_LAMBDA
#		define FLUX_LANGFEAT_GENERIC_LAMBDA 1
#	endif // ~ GCC 4.9

#endif // ~ __cplusplus >= 201300L

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_EXT_LANGFEAT_REQ_CONF_GCC_HXX_3AF6FC36_F24E_4B21_9CFC_CE8DF972B23C
