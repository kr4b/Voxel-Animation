/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Extended language features for NVCC
 *
 * WARNING: untested
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_EXT_LANGFEAT_REQ_CONF_NVCC_HXX_E5F155C4_BE97_4491_B1C5_E95D3D1F82DC
#define DETAIL_EXT_LANGFEAT_REQ_CONF_NVCC_HXX_E5F155C4_BE97_4491_B1C5_E95D3D1F82DC

//--//////////////////////////////////////////////////////////////////////////
//--    C++11                       ///{{{1///////////////////////////////////

#if !defined(CUDART_VERSION)
#	error "Undefined: CUDART_VERSION"
#endif

/* Old NVCCs don't support C++11 stuff. Earliest NVCC that I've tested the
 * -std=c++11 on is 6.5, so let's go for that at the moment.
 */
#if CUDART_VERSION < 6050
#	undef FLUX_LANGFEAT_DECLTYPE
#	undef FLUX_LANGFEAT_AUTO_TYPE
#	undef FLUX_LANGFEAT_RANGE_FOR
#	undef FLUX_LANGFEAT_STATIC_ASSERT
#	undef FLUX_LANGFEAT_INITIALIZER_LIST
#	undef FLUX_LANGFEAT_RVALUE_REFERENCE
#	undef FLUX_LANGFEAT_TEMPLATE_ALIASES
#	undef FLUX_LANGFEAT_VARIADIC_TEMPLATES
#	undef FLUX_LANGFEAT_LAMBDA_EXPRESSIONS
#	undef FLUX_LANGFEAT_EXPLICIT_CTORS
#	undef FLUX_LANGFEAT_EXPLICIT_CONVOPS
#	undef FLUX_LANGFEAT_OVERRIDE_CONTROL
#	undef FLUX_LANGFEAT_TRAILING_RETURN
#	undef FLUX_LANGFEAT_TYPED_ENUM
#	undef FLUX_LANGFEAT_NULLPTR
#	undef FLUX_LANGFEAT_DEF_FN_TEMPLATE

#	define FLUX_LANGFEAT_DECLTYPE            0
#	define FLUX_LANGFEAT_AUTO_TYPE           0
#	define FLUX_LANGFEAT_RANGE_FOR           0
#	define FLUX_LANGFEAT_STATIC_ASSERT       0
#	define FLUX_LANGFEAT_INITIALIZER_LIST    0
#	define FLUX_LANGFEAT_RVALUE_REFERENCE    0
#	define FLUX_LANGFEAT_TEMPLATE_ALIASES    0
#	define FLUX_LANGFEAT_VARIADIC_TEMPLATES  0
#	define FLUX_LANGFEAT_LAMBDA_EXPRESSIONS  0
#	define FLUX_LANGFEAT_EXPLICIT_CTORS      0
#	define FLUX_LANGFEAT_EXPLICIT_CONVOPS    0
#	define FLUX_LANGFEAT_OVERRIDE_CONTROL    0
#	define FLUX_LANGFEAT_TRAILING_RETURN     0
#	define FLUX_LANGFEAT_TYPED_ENUM          0
#	define FLUX_LANGFEAT_NULLPTR             0
#	define FLUX_LANGFEAT_DEF_FN_TEMPLATE     0
#endif // ~ NVCC version

/*  After dropping VS2013: */
/* NVCC 6.5 seems to be the earliest version of NVCC that kinda works with
 * C++11...
 *
 * WARNING: I don't have NVCC 6.5 available anymore, so some of the 7.5 
 * stuff might be available on 6.5 already.
 *
 * !IMPORTANT! These refer to the capabilities on the host side, there's no
 * guarantee that the LANGFEATs are supported in device code (if at all
 * applicable).
 */

#if CUDART_VERSION < 6050
	/* These believed to work on 6.5 (based on earlier versions of this header)
	 * Verified on 7.5
	 */
#	undef FLUX_LANGFEAT_CONSTEXPR
#	undef FLUX_LANGFEAT_NEW_CHARS
#	undef FLUX_LANGFEAT_NOEXCEPT
#	undef FLUX_LANGFEAT_THREAD_LOCAL

#	define FLUX_LANGFEAT_CONSTEXPR 0
#	define FLUX_LANGFEAT_NEW_CHARS 0
#	define FLUX_LANGFEAT_NOEXCEPT 0
#	define FLUX_LANGFEAT_THREAD_LOCAL 0
#endif // ~ NVCC before 6.5

#if CUDART_VERSION < 7050
	/* These verified to work on 7.5 */
#	undef FLUX_LANGFEAT_ALIGNMENT
#	undef FLUX_LANGFEAT_INHERIT_CTOR
#	undef FLUX_LANGFEAT_INLINE_NAMESPACE
#	undef FLUX_LANGFEAT_REF_QUALIFIERS
#	undef FLUX_LANGFEAT_STDATTRIBUTE

#	define FLUX_LANGFEAT_ALIGNMENT 0
#	define FLUX_LANGFEAT_INHERIT_CTOR 0
#	define FLUX_LANGFEAT_INLINE_NAMESPACE 0
#	define FLUX_LANGFEAT_REF_QUALIFIERS 0
#	define FLUX_LANGFEAT_STDATTRIBUTE 0
#endif // ~ NVCC before 7050


#if CUDART_VERSION < 8000
/* User literals seem to be broken until NVCC 8.0. E.g.:
 * \code
   constexpr long double operator"" _deg ( long double deg )
   {
  		return deg*3.141592/180;
   }
  
   int main()
   {
  		long double v = 2.0_deg;
  	return 0;
   }
   \endcode
 * (example code from cppreference)
 *
 * This fails with "error: unable to find numeric literal "operator"" L_deg".
 * Note the extra "L" there. Defining an operator "L_deg" fixes the problem,
 * albeit generates warnings that "L_deg" violates the C++11 standard, since
 * user literals must begin with an "_".
 *
 * \note Verified to be fixed on NVCC 8.0
 */
#	undef FLUX_LANGFEAT_USER_LITERAL 
#	define FLUX_LANGFEAT_USER_LITERAL 0
#endif // ~ NVCC before 8.0

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

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_EXT_LANGFEAT_REQ_CONF_NVCC_HXX_E5F155C4_BE97_4491_B1C5_E95D3D1F82DC
