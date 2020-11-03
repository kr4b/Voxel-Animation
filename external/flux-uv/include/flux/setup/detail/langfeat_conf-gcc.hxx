/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	GCC language features
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_LANGFEAT_CONF_GCC_HXX_62507001_2F26_4F1B_A9EE_9B3BC38E262B
#define DETAIL_LANGFEAT_CONF_GCC_HXX_62507001_2F26_4F1B_A9EE_9B3BC38E262B

//--//////////////////////////////////////////////////////////////////////////
//--    C++14                       ///{{{1///////////////////////////////////

#if __cplusplus >= 201300L /* Used by GCC 4.x for std=c++1y. */

#	if __GNUC__ >= 5
#		undef FLUX_LANGFEAT_EXTENDED_CONSTEXPR
#		define FLUX_LANGFEAT_EXTENDED_CONSTEXPR 1

#		undef FLUX_LANGFEAT_SIZED_DEALLOCATIONS
#		define FLUX_LANGFEAT_SIZED_DEALLOCATIONS 1

#		undef FLUX_LANGFEAT_VARIABLE_TEMPLATE
#		define FLUX_LANGFEAT_VARIABLE_TEMPLATE 1
#	endif // ~ GCC 5.0

#endif // ~ __cplusplus >= 201300L

//--    C++17                       ///{{{1///////////////////////////////////

// HEXFLOAT: GCC has supported these since early versions (3.x), but disables
// them for non-C++17 builds in versions 6.x and later. Unfortunately, their
// presence is still advertised via the feature macros even if otherwise
// disallowed.
#if __GNUC__ < 6
#	undef FLUX_LANGFEAT_HEXFLOAT
#	define FLUX_LANGFEAT_HEXFLOAT 1
#else
	// GCC 6.x and later: HEXFLOAT re-enabled if C++17 / C++1z is used.
#	undef FLUX_LANGFEAT_HEXFLOAT
#	define FLUX_LANGFEAT_HEXFLOAT 0
#endif // ~ GCC 6

#if __cplusplus >= 201500L /* Used by GCC 5.x for std=c++1z */
#	undef FLUX_LANGFEAT_HEXFLOAT
#	define FLUX_LANGFEAT_HEXFLOAT 1

#	if __GNUC__ >= 6
#		undef FLUX_LANGFEAT_NESTED_NAMESPACES
#		define FLUX_LANGFEAT_NESTED_NAMESPACES 1

#		undef FLUX_LANGFEAT_SHORT_STATIC_ASSERT
#		define FLUX_LANGFEAT_SHORT_STATIC_ASSERT 1
#	endif // ~ GCC 6.0

#	if __GNUC__ >= 7
#		undef FLUX_LANGFEAT_FALLTHROUGH
#		define FLUX_LANGFEAT_ATTR_FALLTHROUGH 1

#		undef FLUX_LANGFEAT_ATTR_NODISCARD
#		define FLUX_LANGFEAT_ATTR_NODISCARD 1

#		undef FLUX_LANGFEAT_ATTR_MAYBE_UNUSED
#		define FLUX_LANGFEAT_ATTR_MAYBE_UNUSED 1

#		undef FLUX_LANGFEAT_CONSTEXPR_IF
#		define FLUX_LANGFEAT_CONSTEXPR_IF 1

#		undef FLUX_LANGFEAT_INIT_STATEMENT
#		define FLUX_LANGFEAT_INIT_STATEMENT 1

#		undef FLUX_LANGFEAT_STRUCTURED_BINDINGS
#		define FLUX_LANGFEAT_STRUCTURED_BINDINGS 1

#		undef FLUX_LANGFEAT_TEMPLATE_AUTO
#		define FLUX_LANGFEAT_TEMPLATE_AUTO 1

#		undef FLUX_LANGFEAT_INLINE_VARIABLE
#		define FLUX_LANGFEAT_INLINE_VARIABLE 1

#		undef FLUX_LANGFEAT_TYPEDEF_NOEXCEPT
#		define FLUX_LANGFEAT_TYPEDEF_NOEXCEPT 1
#	endif // ~ GCC 7.0

#endif // ~ __cplusplus >= 201500L

//--    C++2a                       ///{{{1///////////////////////////////////

// missing: LANGFEAT_CONSTEVAL

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_LANGFEAT_CONF_GCC_HXX_62507001_2F26_4F1B_A9EE_9B3BC38E262B
