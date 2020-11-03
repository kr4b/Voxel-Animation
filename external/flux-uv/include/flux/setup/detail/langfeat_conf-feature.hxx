/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Language features via feature testing macros
 *
 * See 
 *   
 *   - http://en.cppreference.com/w/cpp/experimental/feature_test
 *   - http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0096r3.html#recs.cpp17
 *   - http://llvm.org/viewvc/llvm-project/cfe/trunk/lib/Frontend/InitPreprocessor.cpp?view=markup
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_LANGFEAT_CONF_FEATURE_HXX_0A86D83B_8F46_4599_98BA_046474A012FE
#define DETAIL_LANGFEAT_CONF_FEATURE_HXX_0A86D83B_8F46_4599_98BA_046474A012FE

//--//////////////////////////////////////////////////////////////////////////
//--    $ helper                    ///{{{1///////////////////////////////////

#if defined(__has_cpp_attribute)
#	define FLUXTMP_HAS_CPPATTR_(x) __has_cpp_attribute(x)
#else
#	define FLUXTMP_HAS_CPPATTR_(x) 0
#endif // ~ __has_cpp_attribute

//--    C++14                       ///{{{1///////////////////////////////////

#if defined(__cpp_constexpr) && __cpp_constexpr >= 201304
#	undef FLUX_LANGFEAT_EXTENDED_CONSTEXPR
#	define FLUX_LANGFEAT_EXTENDED_CONSTEXPR 1
#endif // ~ __cpp_constexpr

#if defined(__cpp_sized_deallocation) && __cpp_sized_deallocation >= 201309 
#	undef FLUX_LANGFEAT_SIZED_DEALLOCATIONS
#	define FLUX_LANGFEAT_SIZED_DEALLOCATIONS 1
#endif // ~ __cpp_sized_deallocation

#if defined(__cpp_variable_tenmplates) && __cpp_variable_tenmplates >= 201304
#	undef FLUX_LANGFEAT_VARIABLE_TEMPLATE
#	define FLUX_LANGFEAT_VARIABLE_TEMPLATE 1
#endif // ~ __cpp_variable_tenmplates

//--    C++17                       ///{{{1///////////////////////////////////

#if FLUXTMP_HAS_CPPATTR_(fallthrough)
#	undef FLUX_LANGFEAT_ATTR_FALLTHROUGH
#	define FLUX_LANGFEAT_ATTR_FALLTHROUGH 1
#endif // ~ attr: fallthrough

#if FLUXTMP_HAS_CPPATTR_(maybe_unused)
#	undef FLUX_LANGFEAT_ATTR_MAYBE_UNUSED
#	define FLUX_LANGFEAT_ATTR_MAYBE_UNUSED 1
#endif // ~ attr: maybe_unused

#if FLUXTMP_HAS_CPPATTR_(nodiscard)
#	undef FLUX_LANGFEAT_ATTR_NODISCARD
#	define FLUX_LANGFEAT_ATTR_NODISCARD 1
#endif // ~ attr: nodiscard

#if defined(__cpp_if_constexpr) && __cpp_if_constexpr >= 201606 // XXX - clang source
#	undef FLUX_LANGFEAT_CONSTEXPR_IF
#	define FLUX_LANGFEAT_CONSTEXPR_IF 1
#endif // ~ __cpp_if_constexpr

// missing: LANGFEAT_INIT_STATEMENT

#if defined(__cpp_nested_namespace_definitions) && __cpp_nested_namespace_definitions >= 201411
#	undef FLUX_LANGFEAT_NESTED_NAMESPACES
#	define FLUX_LANGFEAT_NESTED_NAMESPACES 1
#endif // ~ __cpp_nested_namespace_definitions

#if defined(__cpp_static_assert) && __cpp_static_assert >= 201411
#	undef FLUX_LANGFEAT_SHORT_STATIC_ASSERT
#	define FLUX_LANGFEAT_SHORT_STATIC_ASSERT 1
#endif // ~ __cpp_static_assert

#if defined(__cpp_structured_bindings) && __cpp_structured_bindings >= 201606 // XXX - clang source
#	undef FLUX_LANGFEAT_STRUCTURED_BINDINGS
#	define FLUX_LANGFEAT_STRUCTURED_BINDINGS 1
#endif // ~ __cpp_structured_bindings

#if defined(__cpp_template_auto) // XXX - recommendation from P0127R2
#	undef FLUX_LANGFEAT_TEMPLATE_AUTO
#	define FLUX_LANGFEAT_TEMPLATE_AUTO 1
#endif // ~ __cpp_template_auto

#if defined(__cpp_hex_float) && __cpp_hex_float >= 201603
#	undef FLUX_LANGFEAT_HEXFLOAT
#	define FLUX_LANGFEAT_HEXFLOAT 1
#endif // ~ __cpp_hex_float

#if defined(__cpp_inline_variables) && __cpp_inline_variables >= 201606
#	undef FLUX_LANGFEAT_INLINE_VARIABLE
#	define FLUX_LANGFEAT_INLINE_VARIABLE 1
#endif // ~ __cpp_inline_variables

#if defined(__cpp_noexcept_function_type) && __cpp_noexcept_function_type >= 201510
#	undef FLUX_LANGFEAT_TYPEDEF_NOEXCEPT
#	define FLUX_LANGFEAT_TYPEDEF_NOEXCEPT 1
#endif // ~ __cpp_noexcept_function_type

//--    C++20                       ///{{{1///////////////////////////////////

// missing: LANGFEAT_CONSTEVAL

//--    $ cleanup                   ///{{{1///////////////////////////////////

#undef FLUXTMP_HAS_CPPATTR_

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_LANGFEAT_CONF_FEATURE_HXX_0A86D83B_8F46_4599_98BA_046474A012FE
