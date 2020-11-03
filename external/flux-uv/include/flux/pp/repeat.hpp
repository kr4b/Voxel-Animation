/*-******************************************************* -- HEADER -{{{1- */
/*-	Pre-processor repetition
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_PP_REPEAT_HPP_BA313737_A6F1_4805_8350_B9E010154683
#define FLUX_PP_REPEAT_HPP_BA313737_A6F1_4805_8350_B9E010154683

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/compiler.hpp>

//--    REPEAT                      ///{{{1///////////////////////////////////

/** Repeat N times
 *
 * Expands into
   \code
   macro(0) macro(1) [...] macro(count-1)
   \endcode
 *
 * The integer argument \a count must be a value in the range 
 * \f$\left[0 ... \mbox{FLUX\_PP\_LIMIT\_MAXINT}\right]\f$.
 *
 * \note `FLUX_PP_REPEAT` / `FLUX_PP_REPEAT_EX` cannot be nested.
 *
 * \see FLUX_PP_REPEAT_NZ FLUX_PP_REPEAT_EX FLUX_PP_LIMIT_MAXINT
 */
#define FLUX_PP_REPEAT( count, macro )                  FLUX_DEFINED_ELSEWHERE

/** Repeat N times
 *
 * Expands into
  \code
  macro(0, ...) macro(1, ...) [...] macro(count-1, ...)
  \endcode
 *
 * The integer argument \a count must be a value in the range 
 * \f$\left[0 ... \mbox{FLUX\_PP\_LIMIT\_MAXINT}\right]\f$.
 *
 * \note `FLUX_PP_REPEAT` / `FLUX_PP_REPEAT_EX` cannot be nested.
 *
 * \see FLUX_PP_REPEAT_NZ_EX FLUX_PP_REPEAT FLUX_PP_LIMIT_MAXINT
 */
#define FLUX_PP_REPEAT_EX( count, macro, ... )          FLUX_DEFINED_ELSEWHERE


/** Repeat N times, but skip 0
 *
 * Expands into
   \code
   macro(1) [...] macro(count-1)
   \endcode
 *
 * The integer argument \a count must be a value in the range 
 * \f$\left[0 ... \mbox{FLUX\_PP\_LIMIT\_MAXINT}\right]\f$.
 *
 * \note `FLUX_PP_REPEAT_NZ` / `FLUX_PP_REPEAT_NZ_EX` cannot be nested.
 *
 * \see FLUX_PP_REPEAT FLUX_PP_REPEAT_NZ_EX FLUX_PP_LIMIT_MAXINT
 */
#define FLUX_PP_REPEAT_NZ( count, macro )               FLUX_DEFINED_ELSEWHERE

/** Repeat N times, but skip 0
 *
 * Expands into
   \code
   macro(1, ...) [...] macro(count-1, ...)
   \endcode
 *
 * The integer argument \a count must be a value in the range 
 * \f$\left[0 ... \mbox{FLUX\_PP\_LIMIT\_MAXINT}\right]\f$.
 *
 * \note `FLUX_PP_REPEAT_NZ` / `FLUX_PP_REPEAT_NZ_EX` cannot be nested.
 *
 * \see FLUX_PP_REPEAT_EX FLUX_PP_REPEAT_NZ FLUX_PP_LIMIT_MAXINT
 */
#define FLUX_PP_REPEAT_NZ_EX( count, macro, ... )       FLUX_DEFINED_ELSEWHERE

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#if FLUX_COMPILER != FLUX_COMPILER_MSVC
#	include "detail/repeat.inl"
#else // MSVC
#	include "detail/repeat-msvc.inl"
#endif // ~ FLUX_COMPILER
#endif // FLUX_PP_REPEAT_HPP_BA313737_A6F1_4805_8350_B9E010154683
