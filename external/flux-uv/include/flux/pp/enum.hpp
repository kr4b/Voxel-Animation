/*-******************************************************* -- HEADER -{{{1- */
/*-	Pre-processor enumerations
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_PP_ENUM_HPP_B3831E03_2B4D_4113_A5A3_ED9B51C75AB7
#define FLUX_PP_ENUM_HPP_B3831E03_2B4D_4113_A5A3_ED9B51C75AB7

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/compiler.hpp>

//--    ENUM                        ///{{{1///////////////////////////////////
/** Enumerate from 0 to N
 *
 * Enumerate in preprocessor. Expands to
   \code
   macro(0) separator() macro(1) separator() [...] macro(count-1)
   \endcode
 *
 * The integer argument \a count must be a value in the range 
 * \f$\left[0 ... \mbox{FLUX\_PP\_LIMIT\_MAXINT}\right]\f$.
 *
 * \note `FLUX_PP_ENUM` / `FLUX_PP_ENUM_EX` cannot be nested.
 *
 * \see FLUX_PP_ENUM_EX FLUX_PP_ENUM_NZ FLUX_PP_LIMIT_MAXINT
 */
#define FLUX_PP_ENUM(count, macro, separator)           FLUX_DEFINED_ELSEWHERE
/** Enumerate from 0 to N
 *
 * Enumerate in preprocessor. Expands to
   \code
   macro(0,...) separator() macro(1,...) separator() [...] macro(count-1,...)
   \endcode
 *
 * The integer argument \a count must be a value in the range 
 * \f$\left[0 ... \mbox{FLUX\_PP\_LIMIT\_MAXINT}\right]\f$.
 *
 * \note `FLUX_PP_ENUM` / `FLUX_PP_ENUM_EX` cannot be nested.
 *
 * \see FLUX_PP_ENUM FLUX_PP_ENUM_NZ_EX FLUX_PP_LIMIT_MAXINT
 */
#define FLUX_PP_ENUM_EX(count, macro, separator, ...)   FLUX_DEFINED_ELSEWHERE

/** Enumerate from 1 to N
 *
 * Enumerate in preprocessor. Expands to
   \code
   macro(1) separator() [...] macro(count-1)
   \endcode
 *
 * The integer argument \a count must be a value in the range 
 * \f$\left[0 ... \mbox{FLUX\_PP\_LIMIT\_MAXINT}\right]\f$.
 *
 * \note `FLUX_PP_ENUM_NZ` / `FLUX_PP_ENUM_NZ_EX` cannot be nested.
 *
 * \see FLUX_PP_ENUM FLUX_PP_ENUM_NZ_EX FLUX_PP_LIMIT_MAXINT
 */
#define FLUX_PP_ENUM_NZ(count, macro, sep)              FLUX_DEFINED_ELSEWHERE
/** Enumerate from 1 to N
 *
 * Enumerate in preprocessor. Expands to
   \code
   macro(1,...) separator() [...] macro(count-1,...)
   \endcode
 *
 * The integer argument \a count must be a value in the range 
 * \f$\left[0 ... \mbox{FLUX\_PP\_LIMIT\_MAXINT}\right]\f$.
 *
 * \note `FLUX_PP_ENUM_NZ` / `FLUX_PP_ENUM_NZ_EX` cannot be nested.
 *
 * \see FLUX_PP_ENUM_EX FLUX_PP_ENUM_NZ FLUX_PP_LIMIT_MAXINT
 */
#define FLUX_PP_ENUM_NZ_EX(count, macro, sep, ...)      FLUX_DEFINED_ELSEWHERE

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#if FLUX_COMPILER != FLUX_COMPILER_MSVC
#	include "detail/enum.inl"
#else // MSVC
#	include "detail/enum-msvc.inl"
#endif // ~ FLUX_COMPILER
#endif // FLUX_PP_ENUM_HPP_B3831E03_2B4D_4113_A5A3_ED9B51C75AB7
