/* Build the zstd compression library 
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
	FLUX_DIAGNOSTIC_STATE_SET(IGNORED,CAST_TYPE_QUALIFIERS);

	FLUX_DEBUG_OPTIMIZE_BEGIN()
	extern "C"
	{
		// ZSTD_NO_INLINE disables the use of always_inline, which causes problems
		// with the DEBUG_OPTIMIZE things.
#		if FLUX_UTIL_DEBUG_OPTIMIZE
#			define ZSTD_NO_INLINE 1
#		endif // ~ DEBUG_OPTIMIZE
#		include <zstd/lib/unity-compress.c>
	}
	FLUX_DEBUG_OPTIMIZE_END()

#endif // ~ EXT_WITH_ZSTD && !EXT_ZSTD_NO_IMPLEMENTATION
