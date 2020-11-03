/*-******************************************************* -- HEADER -{{{1- */
/*- Common definitions.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_REPORT_INSTR_COMMON_HPP_E633048D_5558_46B4_AB39_AA59F95B773C
#define FLUX_REPORT_INSTR_COMMON_HPP_E633048D_5558_46B4_AB39_AA59F95B773C

//--//////////////////////////////////////////////////////////////////////////
//--    include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>
#include <flux/std/inttypes.hpp>

#include "forward.hpp"

//--    >>> namespace = flux::report::instr >>> ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(report,instr)
//--    constants                       ///{{{2///////////////////////////////

constexpr std::size_t kMethodMaxSlotNum = 64; // std::uint64_t slot mask

enum class EMode : uint8_t
{
	linear,
	framed
};

enum class EEvent : uint8_t
{
	frameBegin,
	frameEnd,
	frameProcessed,

	scopeEnter,
	scopeLeave
};

//--    <<< ~ flux::report::instr namespace <<< ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(report,instr)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_REPORT_INSTR_COMMON_HPP_E633048D_5558_46B4_AB39_AA59F95B773C
