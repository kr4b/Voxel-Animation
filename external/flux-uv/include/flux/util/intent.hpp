/*-******************************************************* -- HEADER -{{{1- */
/*-	Mark intentions in code.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_UTIL_INTENT_HPP_BF25D19F_C705_4A2C_A68F_5CB91B94A4F0
#define FLUX_UTIL_INTENT_HPP_BF25D19F_C705_4A2C_A68F_5CB91B94A4F0

//--//////////////////////////////////////////////////////////////////////////
//--    macros                      ///{{{1///////////////////////////////////

/** Mark variable as unused
 *
 * This suppresses warnings about unused variables.
 *
 * Example:
   \code
   void f( int aArg )
   {
   	float val;
  
   	FLUX_INTENT_UNUSED(val);
   	FLUX_INTENT_UNUSED(aArg);
   }
   \endcode
 *
 * \note Using a variable that's declared unused with `FLUX_INTENT_UNUSED(x)`,
 * should be considered an error (some compilers emit warnings about this).
 */
#define FLUX_INTENT_UNUSED(x) (void)x

/** Mark code location as unreachable
 *
 * Mark a code location as unreachable. This has two purposes. First, the
 * `FLUX_INTENT_UNREACHABLE()` macro can be used to suppress warnings about
 * missing return values. Secondly, it may enable additional optimizations, as
 * the compiler may assume that a certain branch cannot be taken. For the
 * latter, more appropriate methods exist in some cases (see e.g.,
 * `FLUX_SWITCH_UNREACHABLE()`).
 *
 * \warning Behaviour is undefined should a code location marked as unreachable
 * be reached anyway. (Although, under `FLUX_DEBUG`, this simply triggers an
 * assertion fault.)
 */
#define FLUX_INTENT_UNREACHABLE() FLUX_DEFINED_ELSEWHERE

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/intent.inl"
#endif // FLUX_UTIL_INTENT_HPP_BF25D19F_C705_4A2C_A68F_5CB91B94A4F0
