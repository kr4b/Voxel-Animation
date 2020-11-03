/*-******************************************************* -- HEADER -{{{1- */
/*-	Join pre-processor tokens
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_PP_JOIN_HPP_522EF725_C1D3_4CBA_8A02_CB3FFC0731CE
#define FLUX_PP_JOIN_HPP_522EF725_C1D3_4CBA_8A02_CB3FFC0731CE

//--//////////////////////////////////////////////////////////////////////////
//--    JOIN                        ///{{{1///////////////////////////////////

/** Join two tokens
 *
 * `FLUX_PP_JOIN` evaluates its arguments.
 *
 * \see FLUX_PP_JOIN2
 */
#define FLUX_PP_JOIN(a,b)                FLUX_PP_JOIN_IMPL_(a,b)


/** Join three tokens
 *
 * `FLUX_PP_JOIN2` evaluates its arguments.
 *
 * Useful for creating tokens with a specific postfix, for example
   \code
   FLUX_PP_JOIN2(internal,__LINE__,_)
   \endcode
 * generates the token `internal32_`.
 *
 * \see FLUX_PP_JOIN
 */
#define FLUX_PP_JOIN2(a,b,c)             FLUX_PP_JOIN2_IMPL_(a,b,c)

//--    JOIN (reverse)              ///{{{1///////////////////////////////////

/** Join token with last element of token list
 *
 * Joins token \a a from the right side to \a __VA_ARGS__. Useful for dealing
 * with sequences when the expansion should contain commas:
 * \code
 * #define SEQ_0_(a) a SEQ_X_
 * #define SEQ_X_(a) , a SEQ_Y_
 * #define SEQ_Y_(a) , a SEQ_X
 *
 * #define SEQ_X_SENTINEL_
 * #define SEQ_Y_SENTINEL_
 *
 * FLUX_PP_REV_JOIN( SENTINEL_, SEQ_0_ (a)(b)(c) ) // ==> a, b, c
 * \endcode
 */
#define FLUX_PP_REV_JOIN(a,...)          FLUX_PP_REV_JOIN_IMPL_(a,__VA_ARGS__)

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/join.inl"
#endif // FLUX_PP_JOIN_HPP_522EF725_C1D3_4CBA_8A02_CB3FFC0731CE
