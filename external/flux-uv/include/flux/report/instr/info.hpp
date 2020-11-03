/*-******************************************************* -- HEADER -{{{1- */
/*- Source information.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_REPORT_INSTR_INFO_HPP_8E3F4613_2319_4EF1_B0E7_132609B836DE
#define FLUX_REPORT_INSTR_INFO_HPP_8E3F4613_2319_4EF1_B0E7_132609B836DE

//--//////////////////////////////////////////////////////////////////////////
//--    include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include "common.hpp"

//--    >>> namespace = flux::report::instr >>> ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(report,instr)
//--    Info                            ///{{{2///////////////////////////////
struct Info
{
	std::size_t threadUniqueId, threadActiveId;

	char const* ident;
	char const* thread;
	char const* stage;

	std::size_t numMethods;
	char const* const* methodNames;
	char const* const* methodUnits;

	//TODO: active method names
	//TODO: active method units
	
	//TODO: total time spent in this stage
};

//--    <<< ~ flux::report::instr namespace <<< ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(report,instr)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_REPORT_INSTR_INFO_HPP_8E3F4613_2319_4EF1_B0E7_132609B836DE
