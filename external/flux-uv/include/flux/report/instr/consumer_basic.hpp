/*-******************************************************* -- HEADER -{{{1- */
/*- Basic consumer.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_REPORT_INSTR_CONSUMER_BASIC_HPP_1DC9D8BD_53AF_40CA_BFB6_760C807BF66C
#define FLUX_REPORT_INSTR_CONSUMER_BASIC_HPP_1DC9D8BD_53AF_40CA_BFB6_760C807BF66C

//--//////////////////////////////////////////////////////////////////////////
//--    include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/ftl/not_null.hpp>
#include <flux/std/unique_ptr.hpp>

#include <cstdio>

#include "consumer.hpp"

//--    >>> namespace = flux::report::instr >>> ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(report,instr)
//--    functions                       ///{{{2///////////////////////////////

unique_ptr<Consumer> consumer_basic_create( 
	ftl::not_null<std::FILE*> = stdout,
	bool aCloseOnDestroy = false
);

//--    <<< ~ flux::report::instr namespace <<< ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(report,instr)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_REPORT_INSTR_CONSUMER_BASIC_HPP_1DC9D8BD_53AF_40CA_BFB6_760C807BF66C
