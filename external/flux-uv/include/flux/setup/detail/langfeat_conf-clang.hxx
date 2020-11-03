/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Clang++ language features
 *
 * Note: most of these are covered by the standard feature detection!
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_LANGFEAT_CONF_CLANG_HXX_9BC7439E_7B7C_4022_9EAA_D16D6BFF5196
#define DETAIL_LANGFEAT_CONF_CLANG_HXX_9BC7439E_7B7C_4022_9EAA_D16D6BFF5196

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

//--    C++14                       ///{{{1///////////////////////////////////

#if __has_extension(cxx_relaxed_constexpr)
#	undef FLUX_LANGFEAT_EXTENDED_CONSTEXPR
#	define FLUX_LANGFEAT_EXTENDED_CONSTEXPR 1
#endif // ~ cxx_relaxed_constexpr

#if __clang_major__ > 3 || (__clang_major__ == 3 && __clang_minor__ >= 4)
#	undef FLUX_LANGFEAT_SIZED_DEALLOCATIONS
#	define FLUX_LANGFEAT_SIZED_DEALLOCATIONS 1
#endif // ~ Clang 3.4

#if __has_extension(cxx_variable_templates) || defined(__cpp_variable_templates)
#	undef FLUX_LANGFEAT_VARIABLE_TEMPLATE
#	define FLUX_LANGFEAT_VARIABLE_TEMPLATE 1
#endif // ~ cxx_variable_template

//--    C++17                       ///{{{1///////////////////////////////////

// Apparently, clang has always supported these.
#undef FLUX_LANGFEAT_HEXFLOAT
#define FLUX_LANGFEAT_HEXFLOAT 1


#if __has_cpp_attribute(fallthrough)
#	undef FLUX_LANGFEAT_ATTR_FALLTHROUGH
#	define FLUX_LANGFEAT_ATTR_FALLTHROUGH 1
#endif // ~ __has_cpp_attribute(fallthrough)

#if __has_cpp_attribute(nodiscard)
#	undef FLUX_LANGFEAT_ATTR_NODISCARD
#	define FLUX_LANGFEAT_ATTR_NODISCARD 1
#endif // ~ __has_cpp_attribute(nodiscard)

#if __has_cpp_attribute(maybe_unused)
#	undef FLUX_LANGFEAT_ATTR_MAYBE_UNUSED
#	define FLUX_LANGFEAT_ATTR_MAYBE_UNUSED 1
#endif // ~ __has_cpp_attribute(maybe_unused)


// Other C++17 features use feature testing macros via langfeat_conf-feature.hxx
// Clang seems to prefer these when they are covered by the SD-6 stuff. See
// http://llvm.org/viewvc/llvm-project/cfe/trunk/lib/Lex/PPMacroExpansion.cpp?view=markup

#if __clang_major__ > 3 || (__clang_major__ == 3 && __clang_major__ >= 9)
/* Not (yet?) covered by the SD-6 macros, and there doesn't seem to be a 
 * __has_feature() version either.
 */
#	undef FLUX_LANGFEAT_INIT_STATEMENT
#	define FLUX_LANGFEAT_INIT_STATEMENT 1
#endif // ~ Clang 3.9


//--    C++2a                       ///{{{1///////////////////////////////////

// missing: LANGFEAT_CONSTEVAL is in trunk (2019-06-24), but not a release

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_LANGFEAT_CONF_CLANG_HXX_9BC7439E_7B7C_4022_9EAA_D16D6BFF5196
