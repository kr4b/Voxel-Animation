/*-******************************************************* -- HEADER -{{{1- */
/*- Instrumentation.
 *
 * User-facing API.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_REPORT_INSTRUMENT_HPP_69BBFB83_FD6D_47EF_BC80_A93C0E353DB3
#define FLUX_REPORT_INSTRUMENT_HPP_69BBFB83_FD6D_47EF_BC80_A93C0E353DB3

//--//////////////////////////////////////////////////////////////////////////
//--    include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <chrono>

#include "defaults.hpp"

//--    macros              ///{{{1///////////////////////////////////////////

#define FLUX_INSTRUMENT_DEFINE_GROUP()
#define FLUX_INSTRUMENT_DEFINE_METHOD()

#define FLUX_INSTRUMENT_METHOD_ENABLE()
#define FLUX_INSTRUMENT_METHOD_DISABLE()
#define FLUX_INSTRUMENT_METHOD_SET_CONTEXT()

//#define FLUX_INSTRUMENT_DECLARE_SCOPE()
#define FLUX_INSTRUMENT_DECLARE_COUNTER()
#define FLUX_INSTRUMENT_DECLARE_RATE()
#define FLUX_INSTRUMENT_DECLARE_EXTERNAL()

#define FLUX_INSTRUMENT_MODE_TRANSITION()

#define FLUX_INSTRUMENT_FRAME_BEGIN()
#define FLUX_INSTRUMENT_FRAME_END()
#define FLUX_INSTRUMENT_FRAME_ADVANCE()

#define FLUX_INSTRUMENT_SCOPE()
#define FLUX_INSTRUMENT_SCOPE_ENTER()
#define FLUX_INSTRUMENT_SCOPE_LEAVE()
#define FLUX_INSTRUMENT_SCOPE_INSERT() // Manually insert time into Scope => support for quick CPU counters.

#define FLUX_INSTRUMENT_COUNTER_SET()
#define FLUX_INSTRUMENT_COUNTER_INC()
#define FLUX_INSTRUMENT_COUNTER_ADD()
#define FLUX_INSTRUMENT_COUNTER_SUB()

#define FLUX_INSTRUMENT_RATE_FEED()

#define FLUX_INSTRUMENT_EXTERNAL_ENABLE()
#define FLUX_INSTRUMENT_EXTERNAL_DISABLE()
#define FLUX_INSTRUMENT_EXTERNAL_SET_CONTEXT()

#define FLUX_INSTRUMENT_FLUSH()

//--    >>> namespace = flux::report >>>        ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(report)
//--    macros                          ///{{{2///////////////////////////////


//--    types                           ///{{{2///////////////////////////////

struct InstrumentScope
{
	char const* name;
};

struct InstrumentCounter
{
	char const* name;
};

struct InstrumentRate
{
	char const* name;
	std::chrono::duration<double> historyLength;
};

struct InstrumentExternal
{
	char const* name;
	std::chrono::duration<double> sampleFrequency;
};

//--    functions                       ///{{{2///////////////////////////////


//--    <<< ~ flux::report namespace <<<        ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(report)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_REPORT_INSTRUMENT_HPP_69BBFB83_FD6D_47EF_BC80_A93C0E353DB3
