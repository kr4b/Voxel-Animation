/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Extended language features for NVCC
 *
 * Note: unlike other langfeat_conf-* headers, this header turns off features
 * again.
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_LANGFEAT_CONF_NVCC_HXX_18A9A7D0_E2AC_4459_A3F2_0DBFDBAECB7E
#define DETAIL_LANGFEAT_CONF_NVCC_HXX_18A9A7D0_E2AC_4459_A3F2_0DBFDBAECB7E

//--//////////////////////////////////////////////////////////////////////////
//--    sanity                      ///{{{1///////////////////////////////////

#if !defined(CUDART_VERSION)
#	error "Undefined: CUDART_VERSION"
#endif

//--    C++14                       ///{{{1///////////////////////////////////

#if CUDART_VERSION < 7500
/* These seem to work with GCC and NVCC 7.5. XXX what about MSVC+NVCC? */
#	undef LANGFEAT_BINARY_LITERAL
#	define LANGFEAT_BINARY_LITERAL 0
#endif // ~ NVCC before 7.5

#if CUDART_VERSION < 8000
/* [[deprecated]] seems to work ok with NVCC 8.0 (i.e., without extra warnings
 * about insufficient language version or unknown attributes). 
 * XXX what about MSVC+NVCC? 
 */
#	undef FLUX_LANGFEAT_ATTR_DEPRECATED
#	define FLUX_LANGFEAT_ATTR_DEPRECATED 0
#endif // ~ NVCC before 8.0


#if CUDART_VERSION < 9000
#	undef FLUX_LANGFEAT_DECLTYPE_AUTO
#	undef FLUX_LANGFEAT_GENERALIZED_LAMBDA
#	undef FLUX_LANGFEAT_GENERIC_LAMBDA

#	define FLUX_LANGFEAT_DECLTYPE_AUTO       0
#	define FLUX_LANGFEAT_GENERALIZED_LAMBDA  0
#	define FLUX_LANGFEAT_GENERIC_LAMBDA      0
#endif // ~ NVCC before 9.0

//--    C++17                       ///{{{1///////////////////////////////////

// TODO: update these for NVCC 9.0
// TODO: update these for NVCC 10.0
// TODO: update these for NVCC 11.0
//     .. :-/

#if 1
/* See above */
#	undef FLUX_LANGFEAT_ATTR_FALLTHROUGH
#	undef FLUX_LANGFEAT_ATTR_MAYBE_UNUSED
#	undef FLUX_LANGFEAT_ATTR_NODISCARD
#	undef FLUX_LANGFEAT_CONSTEXPR_IF
#	undef FLUX_LANGFEAT_INIT_STATEMENT
#	undef FLUX_LANGFEAT_NESTED_NAMESPACES
#	undef FLUX_LANGFEAT_SHORT_STATIC_ASSERT
#	undef FLUX_LANGFEAT_STRUCTURED_BINDINGS
#	undef FLUX_LANGFEAT_TEMPLATE_AUTO

#	define FLUX_LANGFEAT_ATTR_FALLTHROUGH    0
#	define FLUX_LANGFEAT_ATTR_MAYBE_UNUSED   0
#	define FLUX_LANGFEAT_ATTR_NODISCARD      0
#	define FLUX_LANGFEAT_CONSTEXPR_IF        0
#	define FLUX_LANGFEAT_INIT_STATEMENT      0
#	define FLUX_LANGFEAT_NESTED_NAMESPACES   0
#	define FLUX_LANGFEAT_SHORT_STATIC_ASSERT 0
#	define FLUX_LANGFEAT_STRUCTURED_BINDINGS 0
#	define FLUX_LANGFEAT_TEMPLATE_AUTO       0
#	define FLUX_LANGFEAT_INLINE_VARIABLE     0

// missing: LANGFEAT_TYPEDEF_NOEXCEPT
#endif // ~ known NVCCs

// Note: NVCC seems to accept HEXFLOAT on Linux (i.e., with GCC).

//--    C++2a                       ///{{{1///////////////////////////////////

// missig: LANGFEAT_CONSTEVAL

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_LANGFEAT_CONF_NVCC_HXX_18A9A7D0_E2AC_4459_A3F2_0DBFDBAECB7E
