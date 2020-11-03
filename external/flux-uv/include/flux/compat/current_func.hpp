/*-******************************************************* -- HEADER -{{{1- */
/*-	Current function
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_CURRENT_FUNC_HPP_0CFE67AD_08C9_402B_B6AD_AFB1F329B490
#define FLUX_COMPAT_CURRENT_FUNC_HPP_0CFE67AD_08C9_402B_B6AD_AFB1F329B490

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

//--    COMPAT_FUNCTION             ///{{{1///////////////////////////////////

/** Macro: current function name
 *
 * Evaluates to a string containing the current function's name.
 * 
 * Example:
   \code
   void func_name() { printf( "%s\n", FLUX_COMPAT_FUNCTION_NAME ); }
   \endcode
 *
 * This prints `func_name` (GCC,Clang,MSVC).
 */
#define FLUX_COMPAT_FUNCTION_NAME "<unknown>" /*OVERRIDDEN_ELSEWHERE*/

/** Macro: current function prototype
 *
 * Evaluates to a string containing the current function's name, and if 
 * available, the full prototype.
 * 
 * Example:
   \code
   void func_sig() { printf( "%s\n", FLUX_PP_FUNCTION_PROTO ); }
   \endcode
 * This prints `void func_sig()` (GCC,Clang,MSVC).
 *
 * \note MSVC additionally contains the calling convention (typically 
 * `__cdecl`).
 */
#define FLUX_COMPAT_FUNCTION_PROTO "<unknown>" /*OVERRIDDEN_ELSEWHERE*/

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/current_func.inl"
#endif // FLUX_COMPAT_CURRENT_FUNC_HPP_0CFE67AD_08C9_402B_B6AD_AFB1F329B490
