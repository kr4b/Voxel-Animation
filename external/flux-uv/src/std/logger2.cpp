/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Logger implementation
 */
/*-***************************************************************** -}}}1- */

#include "logger2.hxx"

#include <flux/std/log2.hpp>
#include <flux/std/clock.hpp>

#include <thread>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::log::detail >>>       ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(log,detail)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
}

//--    Logger                              ///{{{2///////////////////////////
#if 0
Logger::Logger( bool aDefaultSetup ) noexcept
{
	//TODO: before fully setup want to avoid recursive calls into this?
	//TODO: probably can't do that from here, though.

	//TODO: probably need to defer some setup (e.g., default messages and
	//      so on to first use of e.g. process_()). Set some sort of state
	//      here that causes a first-time setup to be run later, when the
	//      critical stuff here is done.
		
	try
	{
		//TODO
		//TODO
		//TODO
		//TODO
		init_basic_();
	}
	catch( /*TODO*/ ... )
	{
		//TODO
		//TODO  panic. set self to disabled state
		//TODO
	}
}
Logger::~Logger()
{
	worker_stop_and_join_();
}

void Logger::log( LogMeta const& aMeta, flux_ext::fmt::memory_buffer&& aMsg, Clock::time_point aTime )
{
	bool discard = false;
	ProcessFn_ process = nullptr;

	// Discard message if it's been filtered via run-time settings. This
	// requires a (read-only) mutex. Fetch the current processing mode at the
	// same time, using the same mutex.
	{
		compat::shared_lock<compat::shared_mutex> lock( mLoggerState.mutex );

		auto const& st = mLoggerState;

		auto const fac = uint32_t(aMeta.facility);
		if( !((1u<<fac) & st.enabledFacilities) )
			discard = true;

		auto const minSev = (ELogSeverity::inherit == st.facilityMinSeverity[fac])
			? st.globalMinSeverity
			: st.facilityMinSeverity[fac]
		;

		auto const sev = uint32_t(aMeta.severity);
		if( sev < uint32_t(minSev) )
			discard = true;

		process = st.process;
	}
	
	if( discard ) 
		return;

	// Assemble Message
	auto* msg = message_alloc_();

	msg->text = std::move(aMsg);

	msg->msg.kind       = EMessageType::message;
	msg->msg.meta       = aMeta;
	//TODO: nesting
	msg->msg.timestamp  = aTime;
	msg->msg.message    = compat::string_view{ msg->text.data(), msg->text.size() };
	//TODO: threadid
	//TODO: facility info
	
	//TODO: create message and process.
	//TODO:   - what about deferred or otherwise special/flagged messages?
	//TODO:     - defer: handle_deferred_()
	//TODO:     - prelusive: pass to thread/queue; thead won't print until non-prelusive is received
	//TODO:      - transient: just passed on to consumers

	FLUX_ASSERT( process );
	(this->*process)( Event{ msg } );

#	if 0
	using MicroU64_ = std::chrono::duration<uint64_t,std::micro>;
	auto const age = std::chrono::duration_cast<MicroU64_>(aTime-mStartTime).count();

	// Process
	(this->*process)( LoggerEvent{
		std::move(aMsg),
		std::move(aMeta),
		age,
		aScopeDepth,
		
		//TODO...
		
		ELoggerCmd::message
	} );
#	endif
}

void Logger::panic( LogMeta const& aMeta, char const* aMessage, char const* aMessageMeta, char const* aErr, char const* aWhat, Clock::time_point aPoint ) noexcept
{
	//TODO
	//TODO  This must be NOEXCEPT!
	//TODO    note: the Queue never throws if constructors/destructors don't. Yay :-)
	//TODO    note: acquiring a shared_mutex may throw.
	//TODO
	//TODO  Probably just try-catch!
	//TODO

	//process_synch_( LoggerEvent{
	//	
	//} );
	std::fprintf( stderr, "PANIX: (%u,%u,0x%x) [%s:%d // %s]: \"%s\" - %s: %s\n", 
		uint32_t(aMeta.severity),
		uint32_t(aMeta.facility),
		uint32_t(aMeta.flags),
		aMeta.aux.sourceFile,
		aMeta.aux.sourceLine,
		aMeta.aux.sourceFunc,
		aMessage,
		aErr,
		aWhat
	);
}

void Logger::panic( LogMeta const& aMeta, char const* aMessage, compat::string_view const& aMessageMeta, char const* aErr, char const* aWhat, Clock::time_point aPoint ) noexcept
{
	//TODO
	//TODO  This must be NOEXCEPT!
	//TODO    note: the Queue never throws if constructors/destructors don't. Yay :-)
	//TODO    note: acquiring a shared_mutex may throw.
	//TODO
	//TODO  Probably just try-catch!
	//TODO

	//process_synch_( LoggerEvent{
	//	
	//} );
	std::fprintf( stderr, "PANIX: (%u,%u,0x%x) [%s:%d // %s]: \"%s\" - %s: %s\n", 
		uint32_t(aMeta.severity),
		uint32_t(aMeta.facility),
		uint32_t(aMeta.flags),
		aMeta.aux.sourceFile,
		aMeta.aux.sourceLine,
		aMeta.aux.sourceFunc,
		aMessage,
		aErr,
		aWhat
	);
}


bool Logger::global_min_severity( ELogSeverity aSeverity ) noexcept try
{
	if( std::uint32_t(aSeverity) < kStaticMinSeverity )
		return false;

	std::unique_lock<compat::shared_mutex> lock(mLoggerState.mutex);
	mLoggerState.globalMinSeverity = aSeverity;
	return true;
}
catch( ... )
{
	return false;
}

bool Logger::facility_toggle( ELogFacility aFacility, bool aEnable ) noexcept try
{
	FLUX_ASSERT( std::size_t(aFacility) < kMaxLogFacilities );

	auto const bit = LogFacilityMask(1) << std::size_t(aFacility);

	std::unique_lock<compat::shared_mutex> lock(mLoggerState.mutex); //XXX-this throws
	
	if( aEnable )
		mLoggerState.enabledFacilities |= bit;
	else
		mLoggerState.enabledFacilities &= ~bit;

	return true;
}
catch( ... )
{
	return false;
}
bool Logger::facility_min_severity( ELogFacility aFacility, ELogSeverity aSeverity ) noexcept try
{
	FLUX_ASSERT( std::size_t(aFacility) < kMaxLogFacilities );

	if( std::uint32_t(aSeverity) < kStaticMinSeverity )
		return false;

	std::unique_lock<compat::shared_mutex> lock(mLoggerState.mutex);
	mLoggerState.facilityMinSeverity[std::size_t(aFacility)] = aSeverity;
	return true;
}
catch( ... )
{
	return false;
}

void Logger::facility_rename( ELogFacility aFacility, char const* aUtf8, char const* aName )
{
	FLUX_ASSERT( std::size_t(aFacility) < kMaxLogFacilities );

	//TODO
	//TODO
	//TODO

#	if 0
	auto const idx = std::size_t(aFacility);
	if( aUtf8 == nullptr )
	{
		std::unique_lock<std::mutex> lock(mLogMutex);
		mFacilityName[idx] = kDefaultNames[idx];
		mFacilityUtf8[idx] = kDefaultUtf8s[idx];
	}
	else
	{
		auto const dup_ = [] (char const* aStr) -> char const* {
			try
			{
				auto const len = std::strlen(aStr);
				char* ret = new char[len+1];
				std::memcpy( ret, aStr, len+1 );
				return ret;
			}
			catch( ... )
			{
				return nullptr;
			}
		};

		if( !aName ) aName = aUtf8;

		char const* name = dup_( aName );
		if( !name ) name = kDefaultNames[idx];

		char const* utf8 = dup_( aUtf8 );
		if( !utf8 ) utf8 = kDefaultUtf8s[idx];

		std::unique_lock<std::mutex> lock(mLogMutex);
		mFacilityName[idx] = name;
		mFacilityUtf8[idx] = utf8;
	}
#	endif
}

void Logger::consumer_add( unique_ptr<Consumer> aConsumer, char const* aGroup )
{
	//TODO
	//TODO
	//TODO

#	if 0
	FLUX_ASSERT( aConsumer );
	
	std::unique_lock<std::mutex> lock(mWorkerData.mutex);
	mWorkerData.consumers.emplace_back( std::move(aConsumer) );
	mWorkerData.groups.emplace_back( aGroup ? std::string(aGroup) : std::string() );

	FLUX_ASSERT( mWorkerData.consumers.size() == mWorkerData.groups.size() );
#	endif
}
void Logger::consumer_remove( char const* aGroupName )
{
	//TODO
	//TODO
	//TODO

#	if 0
	FLUX_ASSERT( aGroupName );
	compat::string_view name( aGroupName );

	std::unique_lock<std::mutex> lock(mWorkerData.mutex);
	
	/* Note: this is currently implemented in a super lame way. */
	std::size_t const count = mWorkerData.consumers.size();
	FLUX_ASSERT( count == mWorkerData.groups.size() );
	
	for( std::size_t i = 0; i < count; ++i )
	{
		if( name == compat::string_view(mWorkerData.groups[i]) )
			mWorkerData.consumers[i].reset();
	}

	mWorkerData.consumers.erase(
		std::remove_if( mWorkerData.consumers.begin(), mWorkerData.consumers.end(),
			[] ( unique_ptr<Consumer> const& aX ) { return !aX; } 
		),
		mWorkerData.consumers.end()
	);
	mWorkerData.groups.erase(
		std::remove_if( mWorkerData.groups.begin(), mWorkerData.groups.end(),
			[&name] ( std::string const& aX ) {
				return name == compat::string_view(aX);
			} 
		),
		mWorkerData.groups.end()
	);

	FLUX_ASSERT( mWorkerData.consumers.size() == mWorkerData.groups.size() );
#	endif
}


void Logger::worker_()
{
	std::fprintf( stderr, "RANDOM THREAD SAYS HELLO\n" );

	//ftl::Vector<LoggerEvent> ancillary;
	
	Event events[kWorkerDequeueBuffer]; //XXX

	//TODO mutexy fun stuff.

	bool carryOn = true;
	while( carryOn )
	{
		//XXX-timed? why?
		auto const items = mQueue.wait_dequeue_bulk( &events[0], kWorkerDequeueBuffer ); 
		for( std::size_t i = 0; i < items; ++i )
		{
			auto const& event = events[i];
			switch( event.type )
			{
				case EEventType::message: {
					// TODO-except if ancillary. If so, drain those frist
					// and then do this.
				#if 0
					std::fprintf( stderr, "LOGGO: (%u,%u,0x%x) [%s:%d // %s]: %s\n", 
						uint32_t(item.meta.severity),
						uint32_t(item.meta.facility),
						uint32_t(item.meta.flags),
						item.meta.aux.sourceFile,
						item.meta.aux.sourceLine,
						item.meta.aux.sourceFunc,
						std::string( item.message.data(), item.message.data()+item.message.size() ).c_str()
					);
				#endif

					message_free_( event.message ); //XXX XXX XXX XXX
				} break;

#if 0
				case ELoggerCmd::panic: {
					//TODO
					//TODO
					//TODO
					//TODO
				} break;

				case ELoggerCmd::flush: {
					//TODO
					//TODO
					//TODO
					//TODO
				} break;
#endif

				case EEventType::terminate: {
					// TODO: notify consumers + send a final goodbye to them?
					i = items;
					carryOn = false;
				} break;
			}
		}
	}
}

void Logger::process_synch_( Event&& )
{
}
void Logger::process_queue_( Event&& aEvent )
{
	mQueue.enqueue( std::move(aEvent) );
}

void Logger::init_basic_()
{
	for( std::size_t i = 0; i < kMaxLogFacilities; ++i )
		mLoggerState.facilityMinSeverity[i] = ELogSeverity::inherit;

	mLoggerState.process = &Logger::process_queue_;

	//TODO
	//TODO
	//TODO
	/*for( std::size_t i = 0; i < kMaxLogFacilities; ++i )
	{
		mWorkerData.facilityName[i] = kDefaultNames[i];
		mWorkerData.facilityUtf8[i] = kDefaultUtf8s[i];
	}*/

	mStartupTime = Clock::now();
	mWorkerThread = std::thread( [&] { worker_(); } );
}

#if 0
void Logger::init_defaults_() noexcept
{
	auto const* configstr = kLogDefaultConf;
	if( auto const* env = std::getenv( "FLUXENV_LOG_CONSUMERS" ) )
		configstr = env;

	compat::string_view config(configstr);
	if( config.empty() )
		return;
	
	bool any = false;
	std::size_t pos = 0;
	do
	{
		auto const next = config.find( ';', pos );
		auto const sub = config.substr( pos, next );
		if( sub == "native" )
		{
			mWorkerData.consumers.emplace_back(
				consumer_native()
			);

			any = true;
		}
		//TODO
		//TODO
		//TODO
		else
		{
			//TODO log: ancillary: consumer ... not known
		}

		pos = next;
	} while( compat::string_view::npos != pos );

	if( !any )
	{
		//TODO: log: ancillary: no known consumers. falling back to native.
		mWorkerData.consumers.emplace_back(
			consumer_native()
		);
	}
	
	// TODO  Default setup:
	// TODO    -- add default preamble
}
#endif

void Logger::worker_stop_and_join_()
{
	//TODO
	//TODO: is this a race condition? Probably. Which mutex?
	//TODO
	if( mWorkerThread.joinable() )
	{
		mQueue.enqueue( Event{ EvTerminate{} } );
		mWorkerThread.join();
	}

	//TODO: should we clear the queue here? Probably. THat definitively
	//requires a mutex...
}

EvMessage* Logger::message_alloc_()
{
	//TODO: eventually: use a pool?
	return new EvMessage;
}
void Logger::message_free_( EvMessage* aMessage )
{
	//TODO: eventually: use a pool?
	FLUX_ASSERT( aMessage );
	delete aMessage;
}
#endif

//--    logger()                            ///{{{2///////////////////////////
Logger& logger() noexcept
{
	static Logger instance;
	return instance;
}

//--    <<< ~ flux::log::detail namespace <<<       ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(log,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
