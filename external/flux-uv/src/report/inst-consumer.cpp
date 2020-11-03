/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Instrumentation Constumer
 */
/*-***************************************************************** -}}}1- */

#include <flux/report/instr/consumer.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::report::instr >>>     ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(report,instr)
//--    Consumer                        ///{{{2///////////////////////////////

Consumer::~Consumer() = default;

void Consumer::feed_raw( Info const&, EventBuffer const& )
{}

//--    <<< ~ flux::report::instr namespace <<<     ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(report,instr)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
