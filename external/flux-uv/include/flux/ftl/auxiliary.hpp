/*-******************************************************* -- HEADER -{{{1- */
/*-	Auxiliary definitions
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FTL_AUXILIARY_HPP_742FAAC8_524C_4540_852D_693575BB8E21
#define FLUX_FTL_AUXILIARY_HPP_742FAAC8_524C_4540_852D_693575BB8E21

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    types                               ///{{{2///////////////////////////

/** Tag: perform default initialization
 *
 * Used as an optional argument to instruct functions/containers to prefer
 * default initialization over value initialization.
 */
struct DefaultInitTag {};

/** Tag: perform value initialization
 *
 * Used as an optional argument to instruct functions/containers to prefer
 * value initialization when this is not the default behaviour. 
 *
 * \note APIs that do not accept `ValueInitTag` as an optional argument usually
 * perform value initialization by default.
 */
struct ValueInitTag {};

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_FTL_AUXILIARY_HPP_742FAAC8_524C_4540_852D_693575BB8E21
