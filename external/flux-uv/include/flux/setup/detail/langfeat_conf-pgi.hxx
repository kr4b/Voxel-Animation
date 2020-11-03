/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Language features in PGI's C++
 *
 * Not actively maintained, so YMMV.
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_LANGFEAT_CONF_PGI_HXX_005B5B7E_8F13_4DEC_8F69_8B323F2E54F8
#define DETAIL_LANGFEAT_CONF_PGI_HXX_005B5B7E_8F13_4DEC_8F69_8B323F2E54F8

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

// 17.1 not supported: LANGFEAT_NESTED_NAMESPACES
// 17.1 not supported: LANGFEAT_SHORT_STATIC_ASSERT
// 17.1 not supported: LANGFEAT_ATTR_FALLTHROUGH
// 17.1 not supported: LANGFEAT_ATTR_NODISCARD
// 17.1 not supported: LANGFEAT_ATTR_MAYBE_UNUSED
// 17.1 not supported: LANGFEAT_CONSTEXPR_IF
// 17.1 not supported: LANGFEAT_INIT_STATEMENT
// 17.1 not supported: LANGFEAT_STRUCTURED_BINDINGS
// 17.1 not supported: LANGFEAT_TEMPLATE_AUTO

// missing: LANGFEAT_TYPEDEF_NOEXCEPT

/* 17.1 seems to support this, even in non-C++17 modes. Assume that PGI has,
 * like GCC and Clang supported hex-floats for a long time.
 */
#undef FLUX_LANGFEAT_HEXFLOAT
#define FLUX_LANGFEAT_HEXFLOAT 1

//--    C++2a                       ///{{{1///////////////////////////////////

// missing: LANGFEAT_CONSTEVAL

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_LANGFEAT_CONF_PGI_HXX_005B5B7E_8F13_4DEC_8F69_8B323F2E54F8
