/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Processing state
 */
/*-***************************************************************** -}}}1- */

#include <flux/report/state.hpp>

#include <flux/std/assert.hpp>

#include <atomic>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::report >>>            ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(report)
//--    $ local                         ///{{{2///////////////////////////////
namespace
{
	std::atomic<EState> gState( EState::cold );
	thread_local EState tlState = EState::inherit;
}

//--    thread_state()                  ///{{{2///////////////////////////////
EState thread_state() noexcept
{
	auto const ts = tlState;
	if( EState::inherit == ts )
		return gState;
	return ts;
}

//--    set_thread_state()              ///{{{2///////////////////////////////
EState set_thread_state( EState aState ) noexcept
{
	EState oldState = tlState;
	tlState = aState;
	return oldState;
}

//--    set_process_state()             ///{{{2///////////////////////////////
EState set_process_state( EState aState ) noexcept
{
	FLUX_ASSERT( EState::inherit != aState );
	return gState.exchange( aState );
}

//--    ScopedThreadState               ///{{{2///////////////////////////////
ScopedThreadState::ScopedThreadState( EState aState ) noexcept
	: mOldState( set_thread_state(aState) )
{}

ScopedThreadState::~ScopedThreadState() noexcept
{
	set_thread_state( mOldState );
}

//--    ScopedProcessState               ///{{{2///////////////////////////////
ScopedProcessState::ScopedProcessState( EState aState ) noexcept
	: mOldState( set_process_state(aState) )
{}

ScopedProcessState::~ScopedProcessState() noexcept
{
	set_process_state( mOldState );
}

//--    <<< ~ flux::report namespace <<<            ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(report)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
