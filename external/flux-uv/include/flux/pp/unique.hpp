/*-******************************************************* -- HEADER -{{{1- */
/*-	Unique
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_PP_UNIQUE_HPP_971243BD_2631_4E53_9D56_1B5CD4B3E880
#define FLUX_PP_UNIQUE_HPP_971243BD_2631_4E53_9D56_1B5CD4B3E880

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/pp/join.hpp>

//--    UNIQUE                      ///{{{1///////////////////////////////////

/** Unique symbol
 *
 * Evaluates to an unique symbol (typically, an integer) each time it is 
 * called within a translation unit. The symbols may be (and typically are)
 * repeated across different translation units.
 *
 * The unique symbol is paste-able using e.g. FLUX_PP_JOIN().
 */
#define FLUX_PP_UNIQUE __COUNTER__

/** Unique identifier.
 *
 * Evaluates to an unique identifier each time it is called within a 
 * translation unit.
 *
 * Example:
   \code
   int FLUX_PP_UNIQUE_IDENT(a) = 1, FLUX_PP_UNIQUE_IDENT(a) = 2;
   \endcode
 *
 * \see FLUX_PP_UNIQUE
 */
#define FLUX_PP_UNIQUE_IDENT(prefix) FLUX_PP_JOIN2(prefix,FLUX_PP_UNIQUE,_)

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_PP_UNIQUE_HPP_971243BD_2631_4E53_9D56_1B5CD4B3E880
