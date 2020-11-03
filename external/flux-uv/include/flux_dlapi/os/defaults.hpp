/*-******************************************************* -- HEADER -{{{1- */
/*-	flux.dlapi_os configuration
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUXDLAPI_OS_DEFAULTS_HPP_91A635DA_29F7_4C9C_8C48_907A30FA0440
#define FLUXDLAPI_OS_DEFAULTS_HPP_91A635DA_29F7_4C9C_8C48_907A30FA0440

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

//--    defaults                    ///{{{1///////////////////////////////////

/** Config: DLAPI: Allow loading from `/@flux/opt/system`
 *
 * If enabled, extended searches for dynamically loaded APIs will consider
 * paths in under the `/@flux/opt/system` VFS entry (if the corresponding
 * flag in the `EExtendedOptions` is set). If disabled, extended searches 
 * will always behave as if the `EExtendedOptions::vfsSystem` flag were
 * disabled. `FLUXCFG_DLAPI_LOAD_VFSSYSTEM` is enabled by default.
 */
#if !defined(FLUXCFG_DLAPI_LOAD_VFSSYSTEM)
#	define FLUXCFG_DLAPI_LOAD_VFSSYSTEM FLUX_WITH_FLUXIO
#endif // ~ defined(LOAD_VFSSYSTEM)

/** Config: DLAPI: Allow autofetching dependencies
 *
 * If enabled, cross the stream to get water ("gå över ån efter vatten"), i.e.,
 * try autofetching select dependencies from the interwebs (if the
 * corresponding flag in the `EExtendedOptions` is set for the search). If
 * disabled, extended searches will always behave as if the
 * `EExtendedOptions::autofetch` flag were disabled.
 * `FLUXCFG_DLAPI_LOAD_AUTOFETCH` is enabled by default.
 *
 * Autofetching performs the following:
 *  - Check `/@flux/app/data/<os>/<arch>/<name>` for previously fetched
 *    instances. If found, that instance will be used.
 *  - Attempt to fetch the .dmod file for the selected API for the current
 *    OS and architecture. The .dmod file contains a signed .dll/.so and
 *    possibly related files (license statements, etc).
 *  - If the signature matches, the files are extracted to the aforementioned
 *    locations, from which the library will be loaded.
 */
#if !defined(FLUXCFG_DLAPI_LOAD_AUTOFETCH)
#	define FLUXCFG_DLAPI_LOAD_AUTOFETCH FLUX_WITH_FLUXIO
#endif // ~ defined(LOAD_AUTOFETCH)

/** Config: DLAPI: Autofetch timeout
 *
 * `FLUXCFG_DLAPI_AUTOFETCH_TIMEOUT` specifies, in milliseconds, the maximum
 * time that autofetching may take. If the process takes longer, a timeout
 * occurs and fetching is aborted.
 *
 * The default value is 3000 millisconds.
 */
#if !defined(FLUXCFG_DLAPI_AUTOFETCH_TIMEOUT)
#	define FLUXCFG_DLAPI_AUTOFETCH_TIMEOUT 3000
#endif // ~ defined(AUTOFETCH_TIMEOUT)

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUXDLAPI_OS_DEFAULTS_HPP_91A635DA_29F7_4C9C_8C48_907A30FA0440
