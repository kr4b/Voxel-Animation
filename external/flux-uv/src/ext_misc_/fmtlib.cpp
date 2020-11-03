/* This is a hack. My current build doesn't pick up *.cc files, which is what
 * fmtlib uses as its extension. This just includes them so that we don't have
 * to rename files (and so that we can keep them in a separate directory).
 *
 * It also allows us to easily apply FLUX_DEBUG_OPTIMIZE...
 */
#include <flux/util/debug_optimize.hpp>
#include <flux/util/diagnostics.hpp>

FLUX_DIAGNOSTIC_STATE_PUSH_AND_SET(IGNORED,SIGN_PROMO)
FLUX_DIAGNOSTIC_STATE_PUSH_AND_SET(IGNORED,UNDEFINED_MACRO)
FLUX_DEBUG_OPTIMIZE_BEGIN()
#include <fmt/format.cc>
FLUX_DEBUG_OPTIMIZE_END()
FLUX_DIAGNOSTIC_STATE_POP_OR_DEFAULT(UNDEFINED_MACRO)
FLUX_DIAGNOSTIC_STATE_POP_OR_DEFAULT(SIGN_PROMO)
