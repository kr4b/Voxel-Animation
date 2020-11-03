#ifndef DETAIL_LOAD_EXTENDED_HXX_91DB2101_E2BA_4792_A683_5B7F597263F1
#define DETAIL_LOAD_EXTENDED_HXX_91DB2101_E2BA_4792_A683_5B7F597263F1

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/sys/dylib.hpp>
#include <flux/util/enum_bitfield.hpp>

#include <flux_dlapi/os/defaults.hpp>

//--    >>> namespace = flux::dalpi::os::detail >>> ///{{{1///////////////////
FLUX_NAMESPACE_ENTER3(dlapi,os,detail)
//--    constants                           ///{{{2///////////////////////////

/** \brief Search options for extended loading
 *
 * Search options for the extended module search/loading.
 *
 * \note If multiple flags are specified, the extended search performs each
 * search in the order in which the flags are defined below.
 *
 * \see extended_search()
 */
enum class EExtendedOptions
{
	none = 0,

	/** Attempt loading system-library
	 *
	 * Attempts loading via the system's default mechanisms for finding shared
	 * libraries. Each name in the \a aSystemNames array is checked in turn,
	 * until a corresponding library is found or there are no more names in the
	 * array. The array must be terminated with a `nullptr` element.
	 *
	 * Names are passed directly to `sys::dylib_open()`, which in turn uses
	 * system-specific mechanisms such as `dlopen()` or `LoadLibrary()`. 
	 *
	 * If this flag is selected, the \a aSystemNames argument to
	 * `extended_search()` may not be `nullptr`.
	 */
	unspecified     = (1<<0),

	/** Try searching in `/@flux/opt/system`.
	 *
	 * TODO-FIXME: this needs multiple names for different platforms...
	 *   (maybe not vfsSystemPlain...)
	 * 
	 * TODO: - need to abstract base name and version str
	 * TODO: - what if the opt/system path isn't added?
	 * TODO-doc: disabled via FLUXCFG...
	 */
	vfsSystem       = (1<<8),
	vfsSystemPlain  = (1<<9),


	/** Check .dmod caches (by `bindist` or `autofetch`)
	 *
	 * Check for cached files resulting from a previous (successful) search
	 * with either the `bindist` or the `autofetch` flag. Cached files are
	 * searched for in `/@flux/app/data/{os}/{arch}`.
	 *
	 * See `#bindist` or `#autofetch` for additional information. This flag
	 * should always be specified if one of `bindist` or `autofetch` is set.
	 * 
	 * If this flag is selected, the \a aDmodBaseName and \a aDmodVersionStr
	 * arguments to `extended_search()` may not be `nullptr`.
	 *
	 * Requires `FLUX_WITH_FLUXIO`. If disabled, the flag is ignored.
	 *
	 * TODO- should this have a separate config, instead of just using
	 * `FLUX_WITH_FLUXIO`?
	 */
	cached          = (1<<16),
	
	/** Check for (packed) binary distribution
	 *
	 * Binaries may be distributed as compressed .dmod archives that contain
	 * the library and auxiliary files (license, readme, ..). The .dmod files
	 * are signed with a private key (not distributed), and are verified using
	 * the corresponding public key (included in the sources).
	 *
	 * The `bindist` option checks `/@flux/opt/system/bindist` for a .dmod
	 * named `{aDmodBaseName}-{aDmodVersionStr}.dmod_{os}-{arch}`, where `{os}`
	 * and `{arch}` are replaced by the strings returned by
	 * `sys::get_os_ident()` and `sys::get_arch_ident()`, respectively. If
	 * found, the signature is verified, and if it matches, the contents are
	 * decompressed to `/@flux/app/data/{os}/{arch}`. The contained library is
	 * then subsequently loaded.
	 *
	 * \warning This flag should always be combined with the `cached` flag, to
	 * check for previously unpacked files in the aforementioned location.
	 *
	 * If this flag is selected, the \a aDmodBaseName and \a aDmodVersionStr
	 * arguments to `extended_search()` may not be `nullptr`.
	 *
	 * Requires `FLUX_WITH_FLUXIO`. If disabled, the flag is ignored.
	 *
	 * TODO- should this have a separate config, instead of just using
	 * `FLUX_WITH_FLUXIO`?
	 */
	bindist         = (1<<17),

	/** Attempt autofetching
	 *
	 * Autofetching attempts to download the requested library via HTTP. The
	 * downloaded file is a .dmod archive (described in the documentation of
	 * the `bindist` flag). Note that no files are extracted from the fetched
	 * .dmod if its signature cannot be verified.
	 *
	 * \warning This flag should always be combined with the `cached` flag, to
	 * unsure that previously decompressed files are used first.
	 *
	 * If this flag is selected, the \a aDmodBaseName and \a aDmodVersionStr
	 * arguments to `extended_search()` may not be `nullptr`.
	 *
	 * Autofetching can be completely disabled at compile-time by setting the
	 * `FLUXCFG_DLAPI_AUTOFETCH` switch to 0. If so disabled, the `autofetch`
	 * flag is ignored.
	 *
	 */
	autofetch       = (1<<18),
};

FLUX_UTIL_DEFINE_ENUM_BITFIELD(EExtendedOptions);

/** \brief DLAPI autofetching timput
 *
 * Defines the timeout, in milliseconds, for downloading a DLAPI module
 * (.dmod). If fetching the .dmod takes longer than the timeout, the operation
 * is aborted and fails.
 */
constexpr unsigned kAutofetchTimeout = FLUXCFG_DLAPI_AUTOFETCH_TIMEOUT; /* milliseconds */

//--    functions                           ///{{{2///////////////////////////

/** \brief Perform extended search for DLAPI module
 *
 * Perform an extended search, according to `EExtendedOptions`.
 *
 * TODO
 */
sys::DylibHandle extended_search( 
	EExtendedOptions,
	char const* const* aSystemNames,
	char const* aExtendedName,
	char const* aDmodBaseName, char const* aDmodVersionStr
); // TODO: noexcept??? */

//--    <<< ~ flux::dlapi::os::detail namespace <<< ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE3(dlapi,os,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_LOAD_EXTENDED_HXX_91DB2101_E2BA_4792_A683_5B7F597263F1
