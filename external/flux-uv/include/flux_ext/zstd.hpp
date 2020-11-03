/*-******************************************************* -- HEADER -{{{1- */
/*-	Include zstd with appropriate settings
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_EXT_MISC_ZSTD_HPP_16088C51_C59C_4688_A1E1_19E70CEE6683
#define FLUX_EXT_MISC_ZSTD_HPP_16088C51_C59C_4688_A1E1_19E70CEE6683

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg> 
#include "defaults.hpp"

//--    <zstd/zstd.h>               ///{{{1///////////////////////////////////

#if FLUXCFG_EXT_WITH_ZSTD
#	if defined(FLUXCFG_EXT_ZSTD_HEADER)
#		include FLUXCFG_EXT_ZSTD_HEADER
#	else
#		include <zstd/zstd.h>
#	endif
#endif // ~ EXT_WITH_ZSTD

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_EXT_MISC_ZSTD_HPP_16088C51_C59C_4688_A1E1_19E70CEE6683
