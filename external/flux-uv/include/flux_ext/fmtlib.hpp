/*-******************************************************* -- HEADER -{{{1- */
/*-	Include fmtlib
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_EXT_FMTLIB_HPP_4C4807BE_F45C_4CB6_9DAB_7E622BD0D639
#define FLUX_EXT_FMTLIB_HPP_4C4807BE_F45C_4CB6_9DAB_7E622BD0D639

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg> 
#include <flux/util/diagnostics.hpp>

//--    <format.h>                  ///{{{1///////////////////////////////////

FLUX_DIAGNOSTIC_STATE_PUSH_AND_SET(IGNORED,SIGN_PROMO)
FLUX_DIAGNOSTIC_STATE_PUSH_AND_SET(IGNORED,UNDEFINED_MACRO)
#include <fmt/format.h>
#include <fmt/printf.h>
#include <fmt/ostream.h>
FLUX_DIAGNOSTIC_STATE_POP_OR_DEFAULT(UNDEFINED_MACRO)
FLUX_DIAGNOSTIC_STATE_POP_OR_DEFAULT(SIGN_PROMO)

namespace flux_ext
{
	namespace fmt
	{
		using ::fmt::print;
		using ::fmt::format;

		using ::fmt::memory_buffer;
		using ::fmt::format_to;

		using ::fmt::basic_format_args;
		using ::fmt::make_format_args;
		using ::fmt::vprint;
		using ::fmt::vformat;

		using ::fmt::arg;
	}
}

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_EXT_FMTLIB_HPP_4C4807BE_F45C_4CB6_9DAB_7E622BD0D639
