/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Instrumentation Context
 */
/*-***************************************************************** -}}}1- */

#include <flux/report/instr/context.hpp>

#include <flux/report/identity.hpp>

#include <flux/report/instr/info.hpp>
#include <flux/report/instr/event.hpp>
#include <flux/report/instr/method.hpp>
#include <flux/report/instr/consumer.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::report::instr >>>     ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(report,instr)
//--    $ local                         ///{{{2///////////////////////////////
namespace
{
}


//--    Context::Self_                  ///{{{2///////////////////////////////
struct Context::Self_
{
	//TODO HAX
	unique_ptr<Consumer> consumer;

	ftl::Vector<unique_ptr<Method>> methods;
};


//--    Context                         ///{{{2///////////////////////////////

	// [cd]tor -{{{3-
Context::Context()
	: self( make_unique<Self_>() )
{}

Context::~Context() = default;

	// public -{{{3-
void Context::configure_frame_delay( )
{
}
void Context::configure_linear_delay( )
{
}


void Context::group_define( )
{
}
void Context::method_define( std::size_t aSlot, unique_ptr<Method> aMethod )
{
	//TODO
	//TODO Make sure aSlot is in bounds
	//TODO Make sure aSlot is not zero (=> reserved for CPU)
	//TODO Other sanitization?
	//TODO
	if( self->methods.size() <= aSlot )
		self->methods.resize( aSlot+1 );

	self->methods[aSlot] = std::move(aMethod);
}


void Context::consumer_configure( EMode aMode, unique_ptr<Consumer> aConsumer )
{
	//TODO hax
	self->consumer = std::move(aConsumer);
}
void Context::consumers_clear( )
{
}
void Context::consumers_clear( EMode aMode )
{
}


void Context::flush()
{
}
void Context::flush( EMode aMode )
{
}



void Context::attach( Instrumentor& )
{
}
void Context::detach( Instrumentor& )
{
}

EventBuffer Context::consume( EventBuffer aBuffer )
{
	//TODO: synchronization? Or pass to handler thread. Probably that.
	
	Info info;
	info.threadUniqueId = thread_unique_index(); //TODO: either cache these
	info.threadActiveId = thread_active_index(); //   or make them collectible in a single call. Need to move them elsewhere anyway.
	
	info.ident = identity();
	info.thread = thread_name();
	info.stage = "YOLO";


	char const* names[] = { "CPU" };
	char const* units[] = { "ms" };

	info.numMethods = 1;
	info.methodNames = names;
	info.methodUnits = units;
	
	self->consumer->feed_raw( info, aBuffer );

	
#	if 0
	for( std::size_t i = 0; i < aBuffer.types.size(); ++i )
	{
		printf( "%3d: %f\n", int(aBuffer.types[i]), aBuffer.values[0][i] );
	}
#	endif

	return EventBuffer();
}


void Context::request_method( Instrumentor& aInstr, std::size_t aSlot ) const
{
	//TODO acquire mutex
	//TODO check if the method is defined
	//TODO configure_method() on the provided Instrumentor
}

//--    <<< ~ flux::report::instr namespace <<<     ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(report,instr)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
