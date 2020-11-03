/* Miniz. 
 *
 * - Make sure the extra configuration is applied
 * - Optimize this, if possible.
 */
#include <flux_ext/miniz.hpp>
#include <flux_ext/defaults.hpp>

#if FLUXCFG_EXT_WITH_MINIZ && !FLUXCFG_EXT_MINIZ_NO_IMPLEMENTATION
#	include <flux/util/diagnostics.hpp>
#	include <flux/util/debug_optimize.hpp>

	FLUX_DIAGNOSTIC_STATE_SET(IGNORED,EXTRA);
	FLUX_DIAGNOSTIC_STATE_SET(IGNORED,ZERO_AS_NULLPTR);
	FLUX_DIAGNOSTIC_STATE_SET(IGNORED,STRICT_ALIASING);
	FLUX_DIAGNOSTIC_STATE_SET(IGNORED,UNUSED_ARGUMENT);

	FLUX_DEBUG_OPTIMIZE_BEGIN()
#	include <miniz/miniz.c>
	FLUX_DEBUG_OPTIMIZE_END()

#endif // ~ EXT_WITH_MINIZ && !EXT_MINIZ_NO_IMPLEMENTATION
