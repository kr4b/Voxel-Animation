/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Logger implementation
 */
/*-***************************************************************** -}}}1- */

#include "logger.hxx"

#include <flux/std/log2.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::log::detail >>>       ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(log,detail)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	// Default names
	constexpr char const* const kDefaultNames[kMaxLogFacilities] = {
		"",
		"flux", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
		"usr0", "usr1", "usr2", "usr3", "usr4", "usr5", "usr6", "usr7",
		"usr8", "usr9", "usrA", "usrB", "usrC", "usrD", "usrE", "usrF"
	};
	constexpr char const* const kDefaultUtf8s[kMaxLogFacilities] = {
		"",
		"𝘧𝘭𝘶𝘹", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
		"usr⁰", "usr¹", "usr²", "usr³", "usr⁴", "usr⁵", "usr⁶", "usr⁷",
		"usr⁸", "usr⁹", "usrᵃ", "usrᵇ", "usrᶜ", "usrᵈ", "usrᵉ", "usrᶠ"
	};
}

//--    Logger                              ///{{{2///////////////////////////
Logger::Logger( bool aDefaultSetup ) noexcept
{
	init_();
	if( aDefaultSetup ) init_defaults_();
}
Logger::~Logger()
{
	worker_stop_and_join_();
	
	for( std::size_t i = 0; i < kMaxLogFacilities; ++i )
	{
		if( mWorkerData.facilityName[i] != kDefaultNames[i] ) 
			delete [] mWorkerData.facilityName[i];
		if( mWorkerData.facilityUtf8[i] != kDefaultUtf8s[i] ) 
			delete [] mWorkerData.facilityUtf8[i];
	}
}

void Logger::log( LogMeta const& aMeta, flux_ext::fmt::memory_buffer aMsg, Clock::time_point aTime, std::size_t aScopeDepth )
{
	bool discard = false;
	ProcessFn_ process = nullptr;

	// Discard message if it's been filtered via run-time settings. This
	// requires a (read-only) mutex. Fetch the current processing mode at the
	// same time, using the same mutex.
	{
		auto const& st = mLoggerState;

		compat::shared_lock<compat::shared_mutex> lock( mLoggerState.mutex );
		process = mLoggerState.process;

		auto const fac = uint32_t(aMeta.facility);
		if( !((1u<<fac) & st.facilityMask) )
			discard = true;

		auto const minSev = (ELogSeverity::inherit == st.facilityMinSeverity[fac])
			? st.minSeverity
			: st.facilityMinSeverity[fac]
		;

		auto const sev = uint32_t(aMeta.severity);
		if( sev < uint32_t(minSev) )
			discard = true;
	}
	
	if( discard ) 
		return;

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
}

void Logger::panic( LogMeta const& aMeta, char const* aMessage, char const* aMessageMeta, char const* aErr, char const* aWhat, Clock::time_point aPoint, std::size_t aSopeDepth ) noexcept
{
	//TODO
	//TODO  This must be NOEXCEPT!
	//TODO    note: the Queue never throws if constructors/destructors don't. Yay :-)
	//TODO    note: acquiring a shared_mutex may throw.
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

void Logger::panic( LogMeta const& aMeta, char const* aMessage, compat::string_view const& aMessageMeta, char const* aErr, char const* aWhat, Clock::time_point aPoint, std::size_t aSopeDepth ) noexcept
{
	//TODO
	//TODO  This must be NOEXCEPT!
	//TODO    note: the Queue never throws if constructors/destructors don't. Yay :-)
	//TODO    note: acquiring a shared_mutex may throw.
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


void Logger::set_min_severity( ELogSeverity aSeverity ) noexcept
{
	std::unique_lock<compat::shared_mutex> lock(mLoggerState.mutex); //XXX-this throws
	mLoggerState.minSeverity = aSeverity;
}

void Logger::facility_enable( ELogFacility aFacility ) noexcept
{
	FLUX_ASSERT( std::size_t(aFacility) < kMaxLogFacilities );

	std::unique_lock<compat::shared_mutex> lock(mLoggerState.mutex); //XXX-this throws
	mLoggerState.facilityMask |= LogFacilityMask(1) << std::size_t(aFacility);
}
void Logger::facility_disable( ELogFacility aFacility ) noexcept
{
	FLUX_ASSERT( std::size_t(aFacility) < kMaxLogFacilities );

	std::unique_lock<compat::shared_mutex> lock(mLoggerState.mutex); //XXX-this throws
	mLoggerState.facilityMask &= ~(LogFacilityMask(1) << std::size_t(aFacility));
}
void Logger::facility_min_severity( ELogFacility aFacility, ELogSeverity aSeverity ) noexcept
{
	FLUX_ASSERT( std::size_t(aFacility) < kMaxLogFacilities );

	std::unique_lock<compat::shared_mutex> lock(mLoggerState.mutex); //XXX-this throws
	mLoggerState.facilityMinSeverity[std::size_t(aFacility)] = aSeverity;
}

void Logger::facility_rename( ELogFacility aFacility, char const* aUtf8, char const* aName ) noexcept
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
	FLUX_ASSERT( aConsumer );
	
	std::unique_lock<std::mutex> lock(mWorkerData.mutex);
	mWorkerData.consumers.emplace_back( std::move(aConsumer) );
	mWorkerData.groups.emplace_back( aGroup ? std::string(aGroup) : std::string() );

	FLUX_ASSERT( mWorkerData.consumers.size() == mWorkerData.groups.size() );
}
void Logger::consumer_remove( char const* aGroupName )
{
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
}


void Logger::worker_()
{
	std::fprintf( stderr, "RANDOM THREAD SAYS HELLO\n" );

	ftl::Vector<LoggerEvent> ancillary;
	
	LoggerEvent cmds[16]; //XXX

	//TODO mutexy fun stuff.

	bool carryOn = true;
	while( carryOn )
	{
		//XXX-timed? why?
		auto const items = mQueue.wait_dequeue_bulk( &cmds[0], 16 ); //XXX
		for( std::size_t i = 0; i < items; ++i )
		{
			auto const& item = cmds[i];
			switch( item.cmd )
			{
				case ELoggerCmd::message: {
					// TODO-except if ancillary. If so, drain those frist
					// and then do this.
					
					std::fprintf( stderr, "LOGGO: (%u,%u,0x%x) [%s:%d // %s]: %s\n", 
						uint32_t(item.meta.severity),
						uint32_t(item.meta.facility),
						uint32_t(item.meta.flags),
						item.meta.aux.sourceFile,
						item.meta.aux.sourceLine,
						item.meta.aux.sourceFunc,
						std::string( item.message.data(), item.message.data()+item.message.size() ).c_str()
					);
				} break;

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

				case ELoggerCmd::terminate: {
					// TODO: notify consumers + send a final goodbye to them?
					i = items;
					carryOn = false;
				} break;
			}
		}
	}
}

void Logger::process_synch_( LoggerEvent&& )
{
}
void Logger::process_queue_( LoggerEvent&& aEvent )
{
	mQueue.enqueue( std::move(aEvent) );
}

void Logger::init_() noexcept
{
	for( std::size_t i = 0; i < kMaxLogFacilities; ++i )
		mLoggerState.facilityMinSeverity[i] = ELogSeverity::inherit;

	mLoggerState.process = &Logger::process_queue_;

	for( std::size_t i = 0; i < kMaxLogFacilities; ++i )
	{
		mWorkerData.facilityName[i] = kDefaultNames[i];
		mWorkerData.facilityUtf8[i] = kDefaultUtf8s[i];
	}

	mStartTime = Clock::now();
	mWorkerThread = std::thread( [&] { worker_(); } );
}
void Logger::init_defaults_() noexcept
{
	auto const* configstr = kLogDefaultConf;
	if( auto const* env = std::getenv( "FLUXENV_LOG_CONSUMERS" ) )
		configstr = env;

	compat::string_view config(configstr);
	if( config.empty() )
		return;
	
	bool any = false;;
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

void Logger::worker_stop_and_join_()
{
	LoggerEvent cmd;
	cmd.cmd = ELoggerCmd::terminate;

	mQueue.enqueue( std::move(cmd) );

	mWorkerThread.join();
}

//--    logger()                            ///{{{2///////////////////////////
Logger& logger() noexcept
{
	static Logger instance;
	return instance;
}

//--    <<< ~ flux::log::detail namespace <<<       ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(log,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
