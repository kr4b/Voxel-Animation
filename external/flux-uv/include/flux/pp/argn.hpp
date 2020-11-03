/*-******************************************************* -- HEADER -{{{1- */
/*-	Get n:th argument
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_PP_ARGN_HPP_174A8C59_619E_4D02_A0EE_B2E1329959B4
#define FLUX_PP_ARGN_HPP_174A8C59_619E_4D02_A0EE_B2E1329959B4

//--//////////////////////////////////////////////////////////////////////////
//--    ARGN                        ///{{{1///////////////////////////////////
/** Get n:th argument
 *
 * Get n:th argument from the argument list passed to the macro. The integer
 * argument \a n must be in the range \f$\left[0 ...
 * \mbox{FLUX\_PP\_LIMIT\_MAXINT}\right)\f$.
 *
 * \see FLUX_PP_LIMIT_MAXINT
 */
#define FLUX_PP_ARGN(n,...)                FLUX_DEFINED_ELSEWHERE

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/argn.inl"
#endif // FLUX_PP_ARGN_HPP_174A8C59_619E_4D02_A0EE_B2E1329959B4
