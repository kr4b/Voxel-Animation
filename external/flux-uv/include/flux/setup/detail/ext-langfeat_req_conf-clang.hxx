/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Clang++ language features
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_EXT_LANGFEAT_REQ_CONF_CLANG_HXX_906410CE_A44E_4437_BBCD_11A0A690D6F6
#define DETAIL_EXT_LANGFEAT_REQ_CONF_CLANG_HXX_906410CE_A44E_4437_BBCD_11A0A690D6F6

//--//////////////////////////////////////////////////////////////////////////
//--    compat.                     ///{{{1///////////////////////////////////

#if !defined(__has_feature)
#	define __has_feature(x) 0
#endif

#if !defined(__has_extension)
#	define __has_extension __has_feature
#endif 

#if !defined(__has_cpp_attribute)
#	define __has_cpp_attribute(x) 0
#endif

//--    C++11                       ///{{{1///////////////////////////////////

#if __has_extension(cxx_decltype)
#	undef FLUX_LANGFEAT_DECLTYPE
#	define FLUX_LANGFEAT_DECLTYPE 1
#endif // ~ cxx_decltype

#if __has_extension(cxx_auto_type)
#	undef FLUX_LANGFEAT_AUTO_TYPE
#	define FLUX_LANGFEAT_AUTO_TYPE 1
#endif // ~ cxx_auto_type

#if __has_extension(cxx_range_for)
#	undef FLUX_LANGFEAT_RANGE_FOR
#	define FLUX_LANGFEAT_RANGE_FOR 1
#endif // ~ cxx_range_for

#if __has_extension(cxx_static_assert)
#	undef FLUX_LANGFEAT_STATIC_ASSERT
#	define FLUX_LANGFEAT_STATIC_ASSERT 1
#endif // ~ cxx_static_assert

#if __has_extension(cxx_generalized_initializers)
#	undef FLUX_LANGFEAT_INITIALIZER_LIST
#	define FLUX_LANGFEAT_INITIALIZER_LIST 1
#endif // ~ cxx_generalized_initializers

#if __has_feature(cxx_rvalue_references) // Has feature since we also need std::move() 
#	undef FLUX_LANGFEAT_RVALUE_REFERENCE
#	define FLUX_LANGFEAT_RVALUE_REFERENCE 1
#endif // ~ cxx_rvalue_references

#if __has_extension(cxx_alias_templates)
#	undef FLUX_LANGFEAT_TEMPLATE_ALIASES
#	define FLUX_LANGFEAT_TEMPLATE_ALIASES 1
#endif // ~ cxx_alias_templates

#if __has_extension(cxx_variadic_templates)
#	undef FLUX_LANGFEAT_VARIADIC_TEMPLATES
#	define FLUX_LANGFEAT_VARIADIC_TEMPLATES 1
#endif // ~ cxx_variadic_templates

#if __has_extension(cxx_lambdas)
#	undef FLUX_LANGFEAT_LAMBDA_EXPRESSIONS
#	define FLUX_LANGFEAT_LAMBDA_EXPRESSIONS 1
#endif // ~ cxx_lambdas

#if __has_extension(cxx_defaulted_functions) && __has_extension(cxx_deleted_functions)
#	undef FLUX_LANGFEAT_EXPLICIT_CTORS
#	define FLUX_LANGFEAT_EXPLICIT_CTORS 1
#endif // ~ cxx_defaulted_functions && cxx_deleted_functions

#if __has_extension(cxx_explicit_conversions)
#	undef FLUX_LANGFEAT_EXPLICIT_CONVOPS
#	define FLUX_LANGFEAT_EXPLICIT_CONVOPS 1
#endif // ~ cxx_explicit_conversions

#if __has_extension(cxx_override_control)
#	undef FLUX_LANGFEAT_OVERRIDE_CONTROL
#	define FLUX_LANGFEAT_OVERRIDE_CONTROL 1
#endif // ~ cxx_override_control

#if __has_extension(cxx_trailing_return)
#	undef FLUX_LANGFEAT_TRAILING_RETURN
#	define FLUX_LANGFEAT_TRAILING_RETURN 1
#endif // ~ cxx_trailing_return

#if __has_extension(cxx_strong_enums)
#	undef FLUX_LANGFEAT_TYPED_ENUM
#	define FLUX_LANGFEAT_TYPED_ENUM 1
#endif // ~ cxx_strong_enums

#if __has_extension(cxx_nullptr)
#	undef FLUX_LANGFEAT_NULLPTR
#	define FLUX_LANGFEAT_NULLPTR 1
#endif // ~ cxx_nullptr

#if __has_extension(cxx_default_function_template_args)
#	undef FLUX_LANGFEAT_DEF_FN_TEMPLATE
#	define FLUX_LANGFEAT_DEF_FN_TEMPLATE 1
#endif // ~ cxx_default_function_template_args

// After dropping VS2013:
#if __has_extension(cxx_alignas) && __has_extension(cxx_alignof)
#	undef FLUX_LANGFEAT_ALIGNMENT
#	define FLUX_LANGFEAT_ALIGNMENT 1
#endif // ~ cxx_alignas && cxx_alignof

#if __has_extension(cxx_constexpr)
#	undef FLUX_LANGFEAT_CONSTEXPR
#	define FLUX_LANGFEAT_CONSTEXPR 1
#endif // ~ cxx_constexpr

#if __has_extension(cxx_inheriting_constructors)
#	undef FLUX_LANGFEAT_INHERIT_CTOR
#	define FLUX_LANGFEAT_INHERIT_CTOR 1
#endif // ~ cxx_inheriting_constructors

#if __has_extension(cxx_inline_namespaces)
#	undef FLUX_LANGFEAT_INLINE_NAMESPACE
#	define FLUX_LANGFEAT_INLINE_NAMESPACE 1
#endif // ~ cxx_inline_namespaces

#if __has_extension(cxx_noexcept)
#	undef FLUX_LANGFEAT_NOEXCEPT
#	define FLUX_LANGFEAT_NOEXCEPT 1
#endif // ~ cxx_noexcept

#if __has_extension(cxx_unicode_literals)
#	undef FLUX_LANGFEAT_NEW_CHARS
#	define FLUX_LANGFEAT_NEW_CHARS 1
#endif // ~ cxx_unicode_literals

#if __has_extension(cxx_reference_qualified_functions)
#	undef FLUX_LANGFEAT_REF_QUALIFIERS
#	define FLUX_LANGFEAT_REF_QUALIFIERS 1
#endif // ~ cxx_reference_qualified_functions

#if __has_extension(cxx_attributes)
#	undef FLUX_LANGFEAT_STDATTRIBUTE
#	define FLUX_LANGFEAT_STDATTRIBUTE 1
#endif // ~ cxx_attributes

#if __has_extension(cxx_user_literals)
#	undef FLUX_LANGFEAT_USER_LITERAL
#	define FLUX_LANGFEAT_USER_LITERAL 1
#endif // ~ cxx_user_literals


/* Thread local: the thread_local is a bit weird. The language feature *might*
 * be supported, but the C++ standard library may yet not support it. So, we
 * need to check for that too.
 */
#if __has_feature(cxx_thread_local)
#	include <flux/setup/platform.hpp>

#	if FLUX_STDLIB == FLUX_STDLIB_STDCPP
#		include <bits/c++config.h>
#		if defined(_GLIBCXX_HAVE___CXA_THREAD_ATEXIT_IMPL)
#			undef FLUX_LANGFEAT_THREAD_LOCAL
#			define FLUX_LANGFEAT_THREAD_LOCAL 1
#		endif /// _GLIBCXX_HAVE___CXA_THREAD_ATEXIT_IMPL
#	else // FLUX_STDLIB
#		undef FLUX_LANGFEAT_THREAD_LOCAL
#		define FLUX_LANGFEAT_THREAD_LOCAL 1
#	endif // ~ FLUX_STDLIB
#endif // ~ cxx_thread_local

//--    C++14                       ///{{{1///////////////////////////////////

#if __has_cpp_attribute(deprecated)
#	undef FLUX_LANGFEAT_ATTR_DEPRECATED
#	define FLUX_LANGFEAT_ATTR_DEPRECATED 1
#endif // ~ __has_cpp_attribute(deprecated)

#if __has_extension(cxx_binary_literals)
#	undef FLUX_LANGFEAT_BINARY_LITERAL
#	define FLUX_LANGFEAT_BINARY_LITERAL 1
#endif // ~ cxx_binary_literals

#if __has_extension(cxx_decltype_auto)
#	undef FLUX_LANGFEAT_DECLTYPE_AUTO
#	define FLUX_LANGFEAT_DECLTYPE_AUTO 1
#endif // ~ cxx_decltype_auto

#if __has_extension(cxx_init_captures)
#	undef FLUX_LANGFEAT_GENERALIZED_LAMBDA
#	define FLUX_LANGFEAT_GENERALIZED_LAMBDA 1
#endif // ~ cxx_init_captures

#if __has_extension(cxx_generic_lambdas)
#	undef FLUX_LANGFEAT_GENERIC_LAMBDA
#	define FLUX_LANGFEAT_GENERIC_LAMBDA 1
#endif // ~ cxx_generic_lambdas

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_EXT_LANGFEAT_REQ_CONF_CLANG_HXX_906410CE_A44E_4437_BBCD_11A0A690D6F6
