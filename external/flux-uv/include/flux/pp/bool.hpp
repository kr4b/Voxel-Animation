/*-******************************************************* -- HEADER -{{{1- */
/*-	Reduce argument to boolean value
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_PP_BOOL_HPP_CE395109_4A8E_450C_A827_75459C598E97
#define FLUX_PP_BOOL_HPP_CE395109_4A8E_450C_A827_75459C598E97

//--//////////////////////////////////////////////////////////////////////////
//--    BOOL                        ///{{{1///////////////////////////////////
/** Reduce argument to boolean
 *
 * Reduce argument to boolean values represented by 0 (false) and 1 (true). 
 * Argument \a a must evaluate to an integer in the range 
 * \f$\left[0 ... \mbox{FLUX\_PP\_LIMIT\_MAXINT}\right)\f$.
 *
 * \see FLUX_PP_LIMIT_MAXINT
 */
#define FLUX_PP_BOOL(a)                    FLUX_DEFINED_ELSEWHERE

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/bool.inl"
#endif // FLUX_PP_BOOL_HPP_CE395109_4A8E_450C_A827_75459C598E97
