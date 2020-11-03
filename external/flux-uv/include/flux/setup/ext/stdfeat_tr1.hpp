/*-******************************************************* -- HEADER -{{{1- */
/*-	TR1 availability
 *
 * (deprecated - do not use)
 *
 * Formerly (flux-gianttoe) optional, and now unused, standard TR1 library
 * features. Kept around for reference.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SETUP_EXT_STDFEAT_TR1_HPP_915DC6D7_A877_4DCA_9CC0_6F875F46A606
#define FLUX_SETUP_EXT_STDFEAT_TR1_HPP_915DC6D7_A877_4DCA_9CC0_6F875F46A606

//--//////////////////////////////////////////////////////////////////////////
//--    Include                     ///{{{1///////////////////////////////////

#include "../compiler.hpp"
#include "../platform.hpp"

//--    TR1 features                ///{{{1///////////////////////////////////

#define FLUX_STDFEAT_TR1_EXTRAMATH       0 /*OVERRIDDEN_ELSEWHERE*/
#define FLUX_STDFEAT_TR1_FUNCTIONAL      0 /*OVERRIDDEN_ELSEWHERE*/
#define FLUX_STDFEAT_TR1_RANDOM          0 /*OVERRIDDEN_ELSEWHERE*/
#define FLUX_STDFEAT_TR1_REGEX           0 /*OVERRIDDEN_ELSEWHERE*/
#define FLUX_STDFEAT_TR1_SHAREDPTR       0 /*OVERRIDDEN_ELSEWHERE*/
#define FLUX_STDFEAT_TR1_TUPLE           0 /*OVERRIDDEN_ELSEWHERE*/
#define FLUX_STDFEAT_TR1_TYPE_TRAITS     0 /*OVERRIDDEN_ELSEWHERE*/
#define FLUX_STDFEAT_TR1_UNORDERED       0 /*OVERRIDDEN_ELSEWHERE*/

//--    TR1 quirks                 ///{{{1///////////////////////////////////
/** Are some of the `std::tr1` random distributions broken?
 *
 * libstdc++ TR1 implementation of 
 *  - `binomial_distribution`
 *  - `poisson_distribution`
 *  - `normal_distribution`
 *
 * seem to be broken. Drawing a random number hangs.
 *
 * Also see
 * http://stackoverflow.com/questions/15013365/generating-number-from-binomial-distribution-using-c-tr1
 *
 * \see FLUX_STDFEAT_TR1_RANDOM
 */
#define FLUX_STDQUIRK_TR1_RANDOM_BROKEN_DISTS  0 /*OVERRIDDEN_ELSEWHERE*/

/** Is `std::tr1::add_reference` MIA?
 *
 * TR1 `std::tr1::add_reference` is mysteriously missing from the VisualStudio
 * 2015 standard library. Pretty sure it was around for 2013.
 */
#define FLUX_STDQUIRK_TR1_NO_ADD_REFERENCE     0 /*OVERRIDDEN_ELSEWHERE*/

//--    helpers                     ///{{{1///////////////////////////////////

#define FLUX_TR1LIB_HEADER(header)    IMPLEMENTED_ELSEWHERE

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "../detail/ext-stdfeat_tr1.hxx"
#endif // FLUX_SETUP_EXT_STDFEAT_TR1_HPP_915DC6D7_A877_4DCA_9CC0_6F875F46A606
