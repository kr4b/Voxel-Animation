/*-******************************************************* -- HEADER -{{{1- */
/*-	Include (blocking) concurrent queue
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_EXT_BLOCKINGCONCURRENTQUEUE_HPP_C700FF32_2D9F_42A8_8E36_639467E5D111
#define FLUX_EXT_BLOCKINGCONCURRENTQUEUE_HPP_C700FF32_2D9F_42A8_8E36_639467E5D111

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg> 

//--    <moodycamel/blockingconcurrentqueue.h>  ///{{{1///////////////////////

#include <moodycamel/blockingconcurrentqueue.h>

namespace flux_ext
{
	using moodycamel::BlockingConcurrentQueue;
	using moodycamel::ConcurrentQueueDefaultTraits;

	using moodycamel::ConsumerToken;
	using moodycamel::ProducerToken;
}

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_EXT_BLOCKINGCONCURRENTQUEUE_HPP_C700FF32_2D9F_42A8_8E36_639467E5D111
