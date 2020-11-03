/*-******************************************************* -- HEADER -{{{1- */
/*-	Conditional
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_PP_IF_HPP_AFC73C4E_A895_4B40_ADB3_183C890EBFB5
#define FLUX_PP_IF_HPP_AFC73C4E_A895_4B40_ADB3_183C890EBFB5

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/compiler.hpp>

//--    IF                          ///{{{1///////////////////////////////////

/** Conditional
 *
 * Expands into the equivalent of
   \code
   (cond) ? trueMacro(...) : falseMacro(...)
   \endcode
 * where `...` are the arguments passed as the last parameter.
 *
 * The condition \a cond must be an pre-processor expression convertible to a
 * boolean value with `FLUX_PP_BOOL()`.
 *
 * Example:
   \code
   #define ON_TRUE(a,b) FLUX_PP_JOIN(a,b)
   #define ON_FALSE(a,b) FLUX_PP_JOIN(b,a)
   
   FLUX_PP_IF( boolValue, ON_TRUE, ON_FALSE, foo, bar )
   \endcode
 * This evalulates to `foobar` or `barfoo` depending on `boolValue` during 
 * preprocessing.
 *
 * \see FLUX_PP_BOOL FLUX_PP_IF_SYM
 */
#define FLUX_PP_IF( cond, trueMacro, falseMacro, ... )  FLUX_DEFINED_ELSEWHERE

/** Conditional
 *
 * Expands into the equivalent of
   \code
   (cond) ? trueSym : falseSym
   \endcode
 *
 * The condition \a cond must be an pre-processor expression convertible to a
 * boolean value with `FLUX_PP_BOOL()`.
 *
 * Example:
   \code
   FLUX_PP_IF_SYM( boolValue, yay, nay )
   \endcode
 * This evaluates to `yay` or `nay` depending on `boolValue` during 
 * preprocessing.
 *
 * \see FLUX_PP_BOOL FLUX_PP_IF
 */
#define FLUX_PP_IF_SYM( cond, trueSym, falseSym )       FLUX_DEFINED_ELSEWHERE

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#if FLUX_COMPILER != FLUX_COMPILER_MSVC
#	include "detail/if.inl"
#else // MSVC
#	include "detail/if-msvc.inl"
#endif // ~ FLUX_COMPILER
#endif // FLUX_PP_IF_HPP_AFC73C4E_A895_4B40_ADB3_183C890EBFB5
