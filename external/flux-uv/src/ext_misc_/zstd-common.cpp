/* Build the zstd common library 
 *
 * - Make sure the extra configuration is applied
 * - Optimize this, if possible.
 */
#include <flux_ext/zstd.hpp>
#include <flux_ext/defaults.hpp>

#if FLUXCFG_EXT_WITH_ZSTD && !FLUXCFG_EXT_ZSTD_NO_IMPLEMENTATION
#	include <flux/util/diagnostics.hpp>
#	include <flux/util/debug_optimize.hpp>

	FLUX_DIAGNOSTIC_STATE_SET(IGNORED,IMPLICIT_FALLTHROUGH);
	FLUX_DEBUG_OPTIMIZE_BEGIN()
	extern "C"
	{
#		include <zstd/lib/unity-common.c>
	}
	FLUX_DEBUG_OPTIMIZE_END()

#endif // ~ EXT_WITH_ZSTD && !EXT_ZSTD_NO_IMPLEMENTATION
