/*-******************************************************* -- HEADER -{{{1- */
/*-	Include monocypher 
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_EXT_MONOCYPHER_HPP_93B7C413_5E59_4FF3_BFE0_8884995A4121
#define FLUX_EXT_MONOCYPHER_HPP_93B7C413_5E59_4FF3_BFE0_8884995A4121

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include "defaults.hpp"

//--    <monocypher.h>              ///{{{1///////////////////////////////////

#if FLUXCFG_EXT_WITH_MONOCYPHER
extern "C"
{
#	if defined(FLUXCFG_EXT_MONOCYPHER_HEADER)
#		include FLUXCFG_EXT_MONOCYPHER_HEADER
#	else
#		include <monocypher.h>
#	endif
}
#endif // ~ EXT_WITH_MONOCYPHER

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_EXT_MONOCYPHER_HPP_93B7C413_5E59_4FF3_BFE0_8884995A4121
