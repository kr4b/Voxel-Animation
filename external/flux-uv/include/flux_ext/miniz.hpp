/*-******************************************************* -- HEADER -{{{1- */
/*-	Include miniz 
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_EXT_MINIZ_HPP_BBEAAC09_2E7B_43E1_86D4_7375FA62FB3B
#define FLUX_EXT_MINIZ_HPP_BBEAAC09_2E7B_43E1_86D4_7375FA62FB3B

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include "defaults.hpp"

//--    <miniz.h>                   ///{{{1///////////////////////////////////

/* <flux/setup.pkg> causes a few default things to be defined, in particular
 * _LARGEFILE64_SOURCE. miniz will remember that.
 */

#if FLUXCFG_EXT_WITH_MINIZ
#	if defined(FLUXCFG_EXT_MINIZ_HEADER)
#		include FLUXCFG_EXT_MINIZ_HEADER
#	else
#		include <miniz/miniz.h>
#	endif
#endif // ~ EXT_WITH_MINIZ

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_EXT_MINIZ_HPP_BBEAAC09_2E7B_43E1_86D4_7375FA62FB3B
