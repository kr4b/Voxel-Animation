/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Configuration checks
 */
/*-***************************************************************** -}}}1- */

#include <flux_dlapi/os/defaults.hpp>

#include <flux/setup.pkg>
#include <flux_ext/defaults.hpp>

#if FLUXCFG_DLAPI_LOAD_VFSSYSTEM && !FLUX_WITH_FLUXIO
#	error "FLUXCFG_DLAPI_LOAD_VFSSYSTEM requires flux-io (FLUXCFG_WITH_FLUXIO)"
#endif
#if FLUXCFG_DLAPI_LOAD_AUTOFETCH 
#	if !FLUX_WITH_FLUXIO
#		error "FLUXCFG_DLAPI_LOAD_AUTOFETCH requires flux-io (FLUXCFG_WITH_FLUXIO)"
#	endif
#	if !FLUXCFG_EXT_WITH_ZSTD
#		error "FLUXCFG_DLAPI_LOAD_AUTOFETCH requires 3rd-party zstd (FLUXCFG_EXT_WITH_ZSTD)"
#	endif
#endif 

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
