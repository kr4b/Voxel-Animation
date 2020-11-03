/*-******************************************************* -- HEADER -{{{1- */
/*-	Include concurrent queue
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_EXT_CONCURRENTQUEUE_HPP_09966AE8_031C_4036_9437_038C73035E60
#define FLUX_EXT_CONCURRENTQUEUE_HPP_09966AE8_031C_4036_9437_038C73035E60

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg> 

//--    <moodycamel/concurrentqueue.h>      ///{{{1///////////////////////////

/*
Update: these seem to have been changed into #ifdef's upstream, from just
#if's. Mustn't define them then.

#define MCDBGQ_TRACKMEM 0
#define MCDBGQ_USEDEBUGFREELIST 0
#define MCDBGQ_NOLOCKFREE_FREELIST 0
#define MCDBGQ_NOLOCKFREE_IMPLICITPRODBLOCKINDEX 0
#define MCDBGQ_NOLOCKFREE_IMPLICITPRODHASH 0
*/

#include <moodycamel/concurrentqueue.h>

namespace flux_ext
{
	using moodycamel::ConcurrentQueue;
	using moodycamel::ConcurrentQueueDefaultTraits;

	using moodycamel::ConsumerToken;
	using moodycamel::ProducerToken;
}

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_EXT_CONCURRENTQUEUE_HPP_09966AE8_031C_4036_9437_038C73035E60
