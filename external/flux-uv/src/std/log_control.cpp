/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Log control implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/std/log_control.hpp>

#include <flux/std/assert.hpp>
#include <flux/std/appinfo.hpp>

#include <flux/ftl/vector.hpp>

#include <utility>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux >>>                    ///{{{1///////////////////
FLUX_NAMESPACE_ENTER()
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	void log_( void* aPtr, log::Message const& aMsg )
	{
		static_cast<log::Consumer*>(aPtr)->log( aMsg );
	}
	void flush_( void* aPtr )
	{
		static_cast<log::Consumer*>(aPtr)->flush();
	}
	void close_( void* aPtr )
	{
		delete static_cast<log::Consumer*>(aPtr);
	}
}

//--    Consumer                            ///{{{2///////////////////////////
namespace log
{
	Consumer::~Consumer()
	{}

	void Consumer::flush() noexcept
	{}
}

//--    init_*()                            ///{{{2///////////////////////////
namespace log
{
	void init_main()
	{
		int fakeArgc = 1;

		auto const* ident = appinfo_ident();
		auto const ilen = std::strlen( ident );

		ftl::Vector<char> fakeName( ident, ident+ilen+1 );
		char* fakeArgv[] = { fakeName.data(), nullptr };

		flux_ext::loguru::init( fakeArgc, fakeArgv );
	}
	void init_main( int& aArgc, char* aArgv[], char const* aVerb )
	{
		FLUX_ASSERT( aArgv );
		flux_ext::loguru::init( aArgc, aArgv, aVerb );
	}

	void init_thread( char const* aName )
	{
		FLUX_ASSERT( aName );
		flux_ext::loguru::set_thread_name( aName );
	}
}

//--    add_*()                             ///{{{2///////////////////////////
namespace log
{
	void add_raw_file( char const* aFileName, FileMode aMode, Verbosity aVerb )
	{
		FLUX_ASSERT( aFileName );
		flux_ext::loguru::add_file( aFileName, aMode, aVerb );
	}
	void add_callback( char const* aId, LogFn aLogCb, CloseFn aCloseCb, FlushFn aFlushCb, void* aUser, Verbosity aVerb )
	{
		FLUX_ASSERT( aId );
		flux_ext::loguru::add_callback( aId, aLogCb, aUser, aVerb, aCloseCb, aFlushCb );
	}
	void add_consumer( char const* aId, unique_ptr<Consumer> aConsumer, Verbosity aVerb )
	{
		FLUX_ASSERT( aId );
		FLUX_ASSERT( aConsumer );

		flux_ext::loguru::add_callback(
			aId,
			log_,
			aConsumer.release(),
			aVerb,
			close_,
			flush_
		);
	}
}

//--    remove()                            ///{{{2///////////////////////////
namespace log
{
	void remove( char const* aName )
	{
		flux_ext::loguru::remove_callback( aName );
	}
}

//--    set_stderr_verbosity()              ///{{{2///////////////////////////
namespace log
{
	Verbosity set_stderr_verbosity( Verbosity aVerb )
	{
		std::swap( aVerb, flux_ext::loguru::g_stderr_verbosity );
		return aVerb;
	}
}

//--    <<< ~ flux namespace <<<                    ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE()
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
