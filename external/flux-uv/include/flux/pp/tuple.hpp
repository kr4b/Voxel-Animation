/*-******************************************************* -- HEADER -{{{1- */
/*-	Pre-processor tuples
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_PP_TUPLE_HPP_F1C4C074_5560_45C5_9086_9F94AD378307
#define FLUX_PP_TUPLE_HPP_F1C4C074_5560_45C5_9086_9F94AD378307

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/compiler.hpp>

//--    tuple                       ///{{{1///////////////////////////////////

/** Get tuple head
 *
 * Get first element from the input tuple \a t.
 *
 * Example:
   \code
   #define T (a,b,c)
   FLUX_PP_TUPLE_HEAD(T)
   \endcode
 * This evaluates to `a`.
 *
 * \note Empty tuples are OK - `FLUX_PP_TUPLE_HEAD(())` evaluates to nothing.
 *		MSVC may in that case emit warning C4003 ("not enough actual 
 *		parameters for macro FLUX_PP_TUPLE_HEAD_"). The result is correct
 *		nevertheless (at least under VisualStudio 2013).
 *
 * \see FLUX_PP_TUPLE_TAIL
 */
#define FLUX_PP_TUPLE_HEAD(t)              FLUX_DEFINED_ELSEWHERE
/** Get tuple tail
 *
 * Return tuple containing all elements but the first given the input
 * tuple \a t.
 *
 * Example:
   \code
   #define T (a,b,c)
   FLUX_PP_TUPLE_TAIL(T)
   \endcode
 * This evaluates to `(b,c,)`.
 *
 * \note Empty tuples are OK - `FLUX_PP_TUPLE_TAIL(())` evalutes to nothing.
 *		Additionally, `FLUX_PP_TUPLE_TAIL()` may return an empty tuple if the
 *		input tuple contained a single element. In either case, MSVC may emit
 *		warning C4003 ("not enough actual parameters for macro 
 *		FLUX_PP_TUPLE_TAIL_"). The result is correct nevertheless.
 *
 * \see FLUX_PP_TUPLE_HEAD
 */
#define FLUX_PP_TUPLE_TAIL(t)              FLUX_DEFINED_ELSEWHERE

/** Get n:th element
 *
 * Evaluates to the n:th element of the tuple \a t.
 *
 * Example:
   \code
   #define T (a,b,c)
   FLUX_PP_TUPLE_GETN(1,T)
   \endcode
 * This evaluates to `b`.
 *
 * The integer argument \a n must be in the range \f$\left[0 ... 
 * \mbox{FLUX\_PP\_LIMIT\_MAXINT}]\right)\f$.
 *
 * \see FLUX_PP_LIMIT_MAXINT
 */
#define FLUX_PP_TUPLE_GETN(n,t)            FLUX_DEFINED_ELSEWHERE

/** Convert tuple to list
 *
 * Convert input tuple \a t to comma separated list.
 *
 * Example:
   \code
   #define T (a,b,c)
   FLUX_PP_TUPLE_TO_LIST(T)
   \endcode
 * This evaluates to `a,b,c`
 */
#define FLUX_PP_TUPLE_TO_LIST(t)           FLUX_DEFINED_ELSEWHERE
/** Create tuple from argument list
 *
 * Create tuple from the macro's arguments.
 *
 * Example:
   \code
   FLUX_PP_TUPLE_FROM_ARGS(a,b,c)
   \endcode
 * This evaluate to the tuple `(a,b,c)`.
 */
#define FLUX_PP_TUPLE_FROM_ARGS(...)       (__VA_ARGS__)

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#if FLUX_COMPILER != FLUX_COMPILER_MSVC
#	include "detail/tuple.inl"
#else // MSVC
#	include "detail/tuple-msvc.inl"
#endif // ~ FLUX_COMPILER
#endif // FLUX_PP_TUPLE_HPP_F1C4C074_5560_45C5_9086_9F94AD378307
