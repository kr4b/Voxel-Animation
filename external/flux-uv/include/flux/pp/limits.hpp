/*-******************************************************* -- HEADER -{{{1- */
/*-	Limits for pre-processor module
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_PP_LIMITS_HPP_EF726CD8_6EC7_494B_B57A_0D9E0C4F4F21
#define FLUX_PP_LIMITS_HPP_EF726CD8_6EC7_494B_B57A_0D9E0C4F4F21

//--//////////////////////////////////////////////////////////////////////////
//--    limits                      ///{{{1///////////////////////////////////

/** Limit: largest integer handled by flux-pp
 *
 * Integer arguments to flux-pp macros are generally limited to the range 
 * \f$\left[0...\mbox{FLUX\_PP\_LIMIT\_MAXINT}]\right)\f$.
 */
#define FLUX_PP_LIMIT_MAXINT 16

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_PP_LIMITS_HPP_EF726CD8_6EC7_494B_B57A_0D9E0C4F4F21
