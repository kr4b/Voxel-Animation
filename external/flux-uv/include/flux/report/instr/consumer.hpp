/*-******************************************************* -- HEADER -{{{1- */
/*- Consumer interface.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_REPORT_INSTR_CONSUMER_HPP_AAE41658_DB31_4D62_8F40_BB5DCBD96E2B
#define FLUX_REPORT_INSTR_CONSUMER_HPP_AAE41658_DB31_4D62_8F40_BB5DCBD96E2B

//--//////////////////////////////////////////////////////////////////////////
//--    include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/util/enum_bitfield.hpp>

#include "forward.hpp"

//--    >>> namespace = flux::report::instr >>> ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(report,instr)
//--    constants                       ///{{{2///////////////////////////////

enum class EConsumerDataFormat
{
	raw     = (1<<0),
};

FLUX_UTIL_DEFINE_ENUM_BITFIELD( EConsumerDataFormat );


//--    Consumer                        ///{{{2///////////////////////////////
struct Consumer
{
	virtual ~Consumer();

	virtual EConsumerDataFormat data_formats() = 0;

	virtual void feed_raw( Info const&, EventBuffer const& );
};

//--    <<< ~ flux::report::instr namespace <<< ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(report,instr)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_REPORT_INSTR_CONSUMER_HPP_AAE41658_DB31_4D62_8F40_BB5DCBD96E2B
