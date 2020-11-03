/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Required language features, PGI
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_EXT_LANGFEAT_REQ_CONF_PGI_HXX_7D061A07_B4F9_4732_A18C_27FEE0ED2044
#define DETAIL_EXT_LANGFEAT_REQ_CONF_PGI_HXX_7D061A07_B4F9_4732_A18C_27FEE0ED2044

//--//////////////////////////////////////////////////////////////////////////
//--    C++11                       ///{{{1///////////////////////////////////

/* PGI seems to implement C++11 fully, at least if they advertise C++11 as the
 * langlevel.
 */

#if FLUX_LANGLEVEL >= FLUX_LANGLEVEL_CXX11
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

#	define FLUX_LANGFEAT_ALIGNMENT           1
#	define FLUX_LANGFEAT_CONSTEXPR           1
#	define FLUX_LANGFEAT_INHERIT_CTOR        1
#	define FLUX_LANGFEAT_INLINE_NAMESPACE    1
#	define FLUX_LANGFEAT_NOEXCEPT            1
#	define FLUX_LANGFEAT_NEW_CHARS           1
#	define FLUX_LANGFEAT_REF_QUALIFIERS      1
#	define FLUX_LANGFEAT_STDATTRIBUTE        1
#	define FLUX_LANGFEAT_THREAD_LOCAL        1
#	define FLUX_LANGFEAT_USER_LITERAL        1
#endif // ~ FLUX_LANGLEVEL_CXX11

//--    C++14                       ///{{{1///////////////////////////////////

#if FLUX_LANGLEVEL >= FLUX_LANGLEVEL_CXX14
#	undef FLUX_LANGFEAT_ATTR_DEPRECATED
#	undef FLUX_LANGFEAT_BINARY_LITERAL
//#	undef FLUX_LANGFEAT_DECLTYPE_AUTO
#	undef FLUX_LANGFEAT_GENERALIZED_LAMBDA
#	undef FLUX_LANGFEAT_GENERIC_LAMBDA

#	define FLUX_LANGFEAT_ATTR_DEPRECATED     1
#	define FLUX_LANGFEAT_BINARY_LITERAL      1
//#	define FLUX_LANGFEAT_DECLTYPE_AUTO       1
#	define FLUX_LANGFEAT_GENERALIZED_LAMBDA  1
#	define FLUX_LANGFEAT_GENERIC_LAMBDA      1
#endif // ~ FLUX_LANGLEVEL_CXX14

#if __PGIC__ < 17 || (__PGIC__ == 17 && __PGIC__MINOR__ <= 1)
	/* Grr - pgc++ is a stinking liar. It defines __cpp_decltype_auto and 
	 * advertises C++14+ language level despite barfing on "decltype(auto)".
	 *
	 * Idiots.
	 */
#	undef FLUX_LANGFEAT_DECLTYPE_AUTO
#	define FLUX_LANGFEAT_DECLTYPE_AUTO 0
#endif // ~ PGI 17.1 and older

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_EXT_LANGFEAT_REQ_CONF_PGI_HXX_7D061A07_B4F9_4732_A18C_27FEE0ED2044
