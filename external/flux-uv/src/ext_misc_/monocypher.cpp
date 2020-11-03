/* Build the monocypher source. 
 *
 * - Make sure the extra configuration is applied
 * - Optimize this, if possible.
 */
#include <flux_ext/monocypher.hpp>
#include <flux_ext/defaults.hpp>

#if FLUXCFG_EXT_WITH_MONOCYPHER && !FLUXCFG_EXT_MONOCYPHER_NO_IMPLEMENTATION
#	include <flux/util/diagnostics.hpp>
#	include <flux/util/debug_optimize.hpp>

	FLUX_DIAGNOSTIC_STATE_SET(IGNORED,CONVERSION);
	FLUX_DIAGNOSTIC_STATE_SET(IGNORED,CONVERSION_SIZET);
	FLUX_DIAGNOSTIC_STATE_SET(IGNORED,DECLARATION_PARENS);

	FLUX_DEBUG_OPTIMIZE_BEGIN()
	extern "C" 
	{
#		if defined(FLUXCFG_EXT_MONOCYPHER_SOURCE)
#			include FLUXCFG_EXT_MONOCYPHER_SOURCE
#		else
#			include <monocypher.c>
#		endif
	}
	FLUX_DEBUG_OPTIMIZE_END()

#endif // ~ EXT_WITH_MONOCYPHER && !EXT_MONOCYPHER_NO_IMPLEMENTATION
