/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Language features via feature testing macros
 *
 * Required features; see langfeat_conf-features.hxx for additional info.
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_EXT_LANGFEAT_REQ_CONF_FEATURE_HXX_456CC36D_D96C_488D_BAB5_11BC3B9584C4
#define DETAIL_EXT_LANGFEAT_REQ_CONF_FEATURE_HXX_456CC36D_D96C_488D_BAB5_11BC3B9584C4

//--//////////////////////////////////////////////////////////////////////////
//--    C++11                       ///{{{1///////////////////////////////////

// missing: LANGFEAT_ALIGNMENT

#if defined(__cpp_constexpr) && __cpp_constexpr >= 200704
#	undef FLUX_LANGFEAT_CONSTEXPR
#	define FLUX_LANGFEAT_CONSTEXPR 1
#endif // ~ __cpp_constexpr

#if defined(__cpp_inheriting_constructors) && __cpp_inheriting_constructors >= 200802
#	undef FLUX_LANGFEAT_INHERIT_CTOR
#	define FLUX_LANGFEAT_INHERIT_CTOR 1
#endif // ~ __cpp_inheriting_constructors

// missing: LANGFEAT_INLINE_NAMESPACE

// missing: LANGFEAT_NOEXCEPT

#if defined(__cpp_unicode_characters) && __cpp_unicode_characters >= 200704
#	undef FLUX_LANGFEAT_NEW_CHARS
#	define FLUX_LANGFEAT_NEW_CHARS 1
#endif // ~ __cpp_unicode_characters

#if defined(__cpp_ref_qualifiers) && __cpp_ref_qualifiers >= 200710
#	undef FLUX_LANGFEAT_REF_QUALIFIERS
#	define FLUX_LANGFEAT_REF_QUALIFIERS 1
#endif // ~ __cpp_ref_qualifiers

#if defined(__cpp_attributes) && __cpp_attributes >= 200809
#	undef FLUX_LANGFEAT_STDATTRIBUTE
#	define FLUX_LANGFEAT_STDATTRIBUTE 1
#endif // ~ __cpp_attributes

// missing: LANGFEAT_THREAD_LOCAL

#if defined(__cpp_user_defined_literals) && __cpp_user_defined_literals >= 200809
#	undef FLUX_LANGFEAT_USER_LITERAL
#	define FLUX_LANGFEAT_USER_LITERAL 1
#endif // ~ __cpp_user_defined_literals

//--    $ helper                    ///{{{1///////////////////////////////////

#if !FLUX_LANGFEAT_STDATTRIBUTE || !defined(__has_cpp_attribute)
#	define FLUXTMP_HAS_CPPATTR_(x) 0
#else
#	define FLUXTMP_HAS_CPPATTR_(x) __has_cpp_attribute(x)
#endif // ~ __has_cpp_attribute

//--    C++14                       ///{{{1///////////////////////////////////

#if FLUXTMP_HAS_CPPATTR_(deprecated)
#	undef FLUX_LANGFEAT_ATTR_DEPRECATED
#	define FLUX_LANGFEAT_ATTR_DEPRECATED 1
#endif // ~ attr: deprecated

#if defined(__cpp_binary_literals) && __cpp_binary_literals >= 201304
#	undef FLUX_LANGFEAT_BINARY_LITERAL
#	define FLUX_LANGFEAT_BINARY_LITERAL 1
#endif // ~ __cpp_binary_literals

#if defined(__cpp_decltype_auto) && __cpp_decltype_auto >= 201304
#	undef FLUX_LANGFEAT_DECLTYPE_AUTO
#	define FLUX_LANGFEAT_DECLTYPE_AUTO 1
#endif // ~ __cpp_decltype_auto

#if defined(__cpp_init_captures) && __cpp_init_captures >= 201304
#	undef FLUX_LANGFEAT_GENERALIZED_LAMBDA 
#	define FLUX_LANGFEAT_GENERALIZED_LAMBDA 1
#endif // ~ __cpp_init_captures

#if defined(__cpp_generic_lambdas) && __cpp_generic_lambdas >= 201304
#	undef FLUX_LANGFEAT_GENERIC_LAMBDA
#	define FLUX_LANGFEAT_GENERIC_LAMBDA 1
#endif // ~ __cpp_generic_lambdas

//--    $ cleanup                   ///{{{1///////////////////////////////////

#undef FLUXTMP_HAS_CPPATTR_

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_EXT_LANGFEAT_REQ_CONF_FEATURE_HXX_456CC36D_D96C_488D_BAB5_11BC3B9584C4
