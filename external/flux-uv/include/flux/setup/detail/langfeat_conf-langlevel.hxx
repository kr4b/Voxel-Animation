/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Language features from language level
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_LANGFEAT_CONF_LVL_HXX_EA2075D4_3FF0_436C_8BB4_BC0BBB50EC16
#define DETAIL_LANGFEAT_CONF_LVL_HXX_EA2075D4_3FF0_436C_8BB4_BC0BBB50EC16

//--//////////////////////////////////////////////////////////////////////////
//--    C++14                       ///{{{1///////////////////////////////////

#if FLUX_LANGLEVEL >= FLUX_LANGLEVEL_CXX14
#	undef FLUX_LANGFEAT_EXTENDED_CONSTEXPR
#	undef FLUX_LANGFEAT_SIZED_DEALLOCATIONS
#	undef FLUX_LANGFEAT_VARIABLE_TEMPLATE

#	define FLUX_LANGFEAT_EXTENDED_CONSTEXPR  1
#	define FLUX_LANGFEAT_SIZED_DEALLOCATIONS 1
#	define FLUX_LANGFEAT_VARIABLE_TEMPLATE   1
#endif // ~ FLUX_LANGLEVEL_CXX14

//--    C++17                       ///{{{1///////////////////////////////////

#if FLUX_LANGLEVEL >= FLUX_LANGLEVEL_CXX17
#	undef FLUX_LANGFEAT_ATTR_FALLTHROUGH
#	undef FLUX_LANGFEAT_ATTR_MAYBE_UNUSED
#	undef FLUX_LANGFEAT_ATTR_NODISCARD
#	undef FLUX_LANGFEAT_CONSTEXPR_IF
#	undef FLUX_LANGFEAT_INIT_STATEMENT
#	undef FLUX_LANGFEAT_NESTED_NAMESPACES
#	undef FLUX_LANGFEAT_SHORT_STATIC_ASSERT
#	undef FLUX_LANGFEAT_STRUCTURED_BINDINGS
#	undef FLUX_LANGFEAT_TEMPLATE_AUTO
#	undef FLUX_LANGFEAT_HEXFLOAT
#	undef FLUX_LANGFEAT_INLINE_VARIABLE

#	define FLUX_LANGFEAT_ATTR_FALLTHROUGH    1
#	define FLUX_LANGFEAT_ATTR_MAYBE_UNUSED   1
#	define FLUX_LANGFEAT_ATTR_NODISCARD      1
#	define FLUX_LANGFEAT_CONSTEXPR_IF        1
#	define FLUX_LANGFEAT_INIT_STATEMENT      1
#	define FLUX_LANGFEAT_NESTED_NAMESPACES   1
#	define FLUX_LANGFEAT_SHORT_STATIC_ASSERT 1
#	define FLUX_LANGFEAT_STRUCTURED_BINDINGS 1
#	define FLUX_LANGFEAT_TEMPLATE_AUTO       1
#	define FLUX_LANGFEAT_HEXFLOAT            1
#	define FLUX_LANGFEAT_INLINE_VARIABLE     1
#endif // ~ FLUX_LANGLEVEL_CXX17

//--    C++20                       ///{{{1///////////////////////////////////

#if FLUX_LANGLEVEL >= FLUX_LANGLEVEL_CXX20
#	undef FLUX_LANGFEAT_CONSTEVAL

#	define FLUX_LANGFEAT_CONSTEVAL 1
#endif // ~ FLUX_LANGLEVEL_CXX20

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_LANGFEAT_CONF_LVL_HXX_EA2075D4_3FF0_436C_8BB4_BC0BBB50EC16
