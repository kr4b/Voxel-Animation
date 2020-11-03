/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Badness
 */
/*-***************************************************************** -}}}1- */

#include <flux/report/instr/instrumentor.hpp>

#include <flux/report/instr/event.hpp>
#include <flux/report/instr/common.hpp>
#include <flux/report/instr/method.hpp>
#include <flux/report/instr/context.hpp>

#include <flux/std/log.hpp>
#include <flux/std/clock.hpp>
#include <flux/std/assert.hpp>

#include <flux/ftl/vector.hpp>

#include <flux/util/hint.hpp>

#include <deque>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::report::instr >>>     ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(report,instr)
//--    $ local                         ///{{{2///////////////////////////////
namespace
{
	struct Event_
	{
		EEvent type;
		Clock::time_point cpuTimestamp;
	};

	using Events_ = ftl::VectorBase<Event_>;
}

//--    Instrumentor::Self_             ///{{{2///////////////////////////////
struct Instrumentor::Self_
{
	explicit Self_( Context& aContext )
		: context(aContext)
	{}
	
	std::size_t frameDelay = 1;
	std::size_t scopeStackDepth = 0;
	bool inFrame = false;

	Context& context;

	Clock::time_point modeStartTime;
	
	Events_ events;

	std::uint64_t activeMethods;
	ftl::Vector<unique_ptr<MethodInstance>> methods;
	
	std::deque<Events_> delayed; // TODO-XXX rename to "pending"

	EventBuffer eventBuffer;
	//ftl::VectorBase<Event> eventBuffer;
};

//--    Instrumentor                    ///{{{2///////////////////////////////

	// [cd]tor
Instrumentor::Instrumentor( Context& aContext )
	: self( make_unique<Self_>(aContext) )
{
	// TODO: initialize for default mode!
	// TODO: initialize methods
	
	self->modeStartTime = Clock::now();
	self->activeMethods = 0;
}

Instrumentor::~Instrumentor() = default;

	// public
void Instrumentor::method_enable( std::size_t aSlot )
{
	//TODO verify stuff
	FLUX_ASSERT( aSlot < kMethodMaxSlotNum );
	self->context.request_method( *this, aSlot );
	self->activeMethods |= (std::uint64_t(1)<<aSlot);

}
void Instrumentor::method_disable( std::size_t aSlot )
{
	//TODO
	//TODO
	//TODO
	//TODO
	self->activeMethods &= ~(std::uint64_t(1)<<aSlot);
}

void Instrumentor::mode_transition( )
{
	auto& event = self->events.emplace_back();
	event.type = EEvent::frameBegin;

	self->inFrame = true;

	self->modeStartTime = Clock::now();
}


void Instrumentor::frame_begin( )
{
	// Sanity checks
	if( FLUX_HINT_UNLIKELY(self->inFrame) )
	{
		FLUX_LOG( ERROR, "Instrumentation: frame_begin(): already in frame!" );
		return;
	}

	// Record event
	auto& event = self->events.emplace_back();
	event.type = EEvent::frameBegin;

	//TODO
	//TODO

	self->inFrame = true;

	//TODO
	//TODO	


	// Record the CPU time just before returning.
	event.cpuTimestamp = Clock::now();
}
void Instrumentor::frame_end( )
{
	// Record CPU time at earliest possible time
	auto const cpuEnd = Clock::now();

	// Sanity checks
	if( FLUX_HINT_UNLIKELY(!self->inFrame) )
	{
		FLUX_LOG( ERROR, "Instrumentation: frame_end(): not in frame!" );
		return;
	}
	if( FLUX_HINT_UNLIKELY(self->scopeStackDepth != 0) )
	{
		FLUX_LOG( ERROR, "Instrumentation: frame_end(): scope stack is not empty!" );
		//TODO: cleanup frame by balancing the scope stack.
		return;
	}

	// Record event
	auto& event = self->events.emplace_back();
	event.type = EEvent::frameEnd;
	event.cpuTimestamp = cpuEnd;
	

	//TODO
	//TODO

	self->inFrame = false;

	//TODO
	//TODO	
	
	// Resolve and process events
	self->delayed.emplace_back( std::move(self->events) );

	while( self->delayed.size() > self->frameDelay )
	{
		process_();
		self->events = std::move(self->delayed.front());
		self->delayed.pop_front();
	}

	// Append the frameProcessed information if possible
	// This should be the last thing in the method -- in particular, the 
	// Clock::now() should be called just before returning.
	if( !self->delayed.empty() )
	{
		auto& event = self->delayed.back().emplace_back();
		event.type = EEvent::frameProcessed;
		event.cpuTimestamp = Clock::now();
	}
}
void Instrumentor::frame_advance( )
{
	if( self->inFrame ) 
		frame_end();

	frame_begin();
}


void Instrumentor::scope_enter( ) noexcept
{
	auto& event = self->events.emplace_back();
	event.type = EEvent::scopeEnter;

	++self->scopeStackDepth;

	// Record the CPU time just before returning.
	event.cpuTimestamp = Clock::now();
}
void Instrumentor::scope_leave( ) noexcept
{
	// Record CPU time at earliest possible time
	auto const cpuEnd = Clock::now();
	
	// Sanity check
	if( FLUX_HINT_UNLIKELY(self->scopeStackDepth == 0) )
	{
		FLUX_LOG( ERROR, "Instrumentation: scope_leave(): scope stack is empty!" );
		return;
	}

	--self->scopeStackDepth;

	// record event
	auto& event = self->events.emplace_back();
	event.type = EEvent::scopeLeave;
	event.cpuTimestamp = cpuEnd;

	//TODO: in linear mode, should we submit stuff from here?
	//  note: we can only really submit when the scopeStackDepth becomes zero.
}


void Instrumentor::counter_set( )
{
}
void Instrumentor::counter_inc( )
{
}
void Instrumentor::counter_add( )
{
}
void Instrumentor::counter_sub( )
{
}


void Instrumentor::rate_feed( )
{
}


void Instrumentor::flush( )
{
}


void Instrumentor::set_params( std::size_t aSlot, MethodParams const* aParams )
{
	//TODO: error if aSlot is invalid?
	FLUX_ASSERT( aSlot < self->methods.size() );
	self->methods[aSlot]->set_params( aParams );
}
void Instrumentor::set_frame_delay( std::size_t aDelay ) noexcept
{
	self->frameDelay = aDelay;
}


void Instrumentor::configure_method( std::size_t aSlot, unique_ptr<MethodInstance> aMethod )
{
	FLUX_ASSERT( aSlot < kMethodMaxSlotNum );
	if( self->methods.size() <= aSlot )
		self->methods.resize( aSlot+1 );

	self->methods[aSlot] = std::move(aMethod);
}

	// private
void Instrumentor::process_()
{
	if( self->delayed.empty() )
		return;
	
	auto& events = self->delayed.front();
	auto& buffer = self->eventBuffer;

	FLUX_ASSERT( buffer.types.empty() );
	buffer.values.resize( 1 ); //TODO: to size of largest method index 
	
	for( auto const& event : events )
	{
		buffer.types.emplace_back( event.type );

		// CPU time value
		using Msd_ = std::chrono::duration<double,std::milli>;
		buffer.values[0].emplace_back( std::chrono::duration_cast<Msd_>(event.cpuTimestamp-self->modeStartTime).count() );
	}

	//TODO: resolve method event stuff
	
	buffer = self->context.consume( std::move(buffer) );
}

//--    <<< ~ flux::report::instr namespace <<<     ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(report,instr)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
