/*-******************************************************* -- HEADER -{{{1- */
/*- Events.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_REPORT_INSTR_EVENT_HPP_04C846B1_43E2_4320_A17B_E77798140E11
#define FLUX_REPORT_INSTR_EVENT_HPP_04C846B1_43E2_4320_A17B_E77798140E11

//--//////////////////////////////////////////////////////////////////////////
//--    include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/ftl/vector.hpp>

#include "common.hpp"

//--    >>> namespace = flux::report::instr >>> ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(report,instr)
//--    types                           ///{{{2///////////////////////////////

using EventData = double;
using EventDataBuffer = ftl::VectorBase<EventData>;

//--    EventBuffer                     ///{{{2///////////////////////////////
struct EventBuffer
{
	ftl::VectorBase<EEvent> types;
	ftl::Vector<EventDataBuffer> values;

	//TODO: EventName field?
	// Atom? Pointer? Either or?

	// Move-only
	EventBuffer() = default;

	EventBuffer( EventBuffer const& ) = delete;
	EventBuffer& operator= (EventBuffer const&) = delete;

	EventBuffer( EventBuffer&& ) = default;
	EventBuffer& operator= (EventBuffer&&) = default;
};

//--    <<< ~ flux::report::instr namespace <<< ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(report,instr)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_REPORT_INSTR_EVENT_HPP_04C846B1_43E2_4320_A17B_E77798140E11
