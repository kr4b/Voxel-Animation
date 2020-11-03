#ifndef DETAIL_LOGGER_HXX_B937A185_9252_4E35_A3C5_AB2CFE683FAA
#define DETAIL_LOGGER_HXX_B937A185_9252_4E35_A3C5_AB2CFE683FAA

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/buildcfg.hpp>

#include <flux/std/log2.hpp>
#include <flux/std/log2_ctrl.hpp>
#include <flux/std/clock.hpp>
#include <flux/std/inttypes.hpp>
#include <flux/std/unique_ptr.hpp>

#include <flux/compat/string_view.hpp>
#include <flux/compat/shared_mutex.hpp>

#include <flux/ftl/vector.hpp>

#include <flux_ext/blockingconcurrentqueue.hpp>

#include <mutex>
#include <thread>

//--    >>> namespace = flux::log::detail >>>       ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(log,detail)
//--    constants                           ///{{{2///////////////////////////

constexpr std::size_t kMaxLogFacilities = std::size_t(ELogFacility::max);
static_assert( kMaxLogFacilities <= 32, "Only up to 32 facilities supported." );

constexpr char const* kLogDefaultConf = "native";

enum class ELoggerCmd
{
	message,


	panic,
	

	flush,
	terminate
};

//--    types                               ///{{{2///////////////////////////

using LogFacilityMask = std::uint32_t;

// XXX Should we allocate these from a pool/queue? 
// The main question is what "message" does. Will it maintain internal buffers?
// (IIRC it's a SSO optimized thing.)
//
// Then we can pass this around as a pointer and not worry about moving/copying.
struct LoggerEvent
{
	/* XXX RENAME/REFACTOR AND MAKE NOT SUCK */
	flux_ext::fmt::memory_buffer message;

	LogMeta meta; //TODO: unpack meta here, so we can easily access the flags?
	uint64_t age;//Clock::time_point age;
	std::size_t nesting;
	
	ELoggerCmd cmd;
	//TODO...

	// Move only
	LoggerEvent() = default;
	LoggerEvent( flux_ext::fmt::memory_buffer aMessage, LogMeta aMeta, uint64_t aAge, std::size_t aNesting, ELoggerCmd aCmd )
		: message( std::move(aMessage) )
		, meta( std::move(aMeta) )
		, age( std::move(aAge) )
		, nesting( std::move(aNesting) )
		, cmd( aCmd )
	{}
	
	LoggerEvent( LoggerEvent const& ) = delete;
	LoggerEvent& operator= (LoggerEvent const&) = delete;

	LoggerEvent( LoggerEvent&& aOther ) noexcept
		: message( std::move(aOther.message) )
		, meta( std::move(aOther.meta) )
		, age( std::move(aOther.age) )
		, nesting( std::move(aOther.nesting) )
		, cmd( std::move(aOther.cmd) )
	{}
	LoggerEvent& operator= (LoggerEvent&& aOther) noexcept
	{
		message = std::move(aOther.message);
		meta = std::move(aOther.meta);
		age = std::move(aOther.age);
		nesting = std::move(aOther.nesting);
		cmd = std::move(aOther.cmd);
		return *this;
	}
};

//--    Logger                              ///{{{2///////////////////////////
class Logger final
{
	public:
		Logger( bool aDefaultSetup = true ) noexcept, ~Logger();

	public:
		void log( 
			LogMeta const&, 
			flux_ext::fmt::memory_buffer,
			Clock::time_point,
			std::size_t aScopeDepth
		);

		//void flush(); TODO!!!

		// Log panics: avoid more allocations, e..g, memory_buffer. Called if
		// format() in log fails in funny ways. Note the two versions. :-/
		void panic(
			LogMeta const&,
			char const*,
			char const*,
			char const*,
			char const*,
			Clock::time_point,
			std::size_t aScopeDepth
		) noexcept;
		void panic(
			LogMeta const&,
			char const*,
			compat::string_view const&,
			char const*,
			char const*,
			Clock::time_point,
			std::size_t aScopeDepth
		) noexcept;

	public:
		void set_min_severity( ELogSeverity ) noexcept;

		void facility_enable( ELogFacility ) noexcept;
		void facility_disable( ELogFacility ) noexcept;
		void facility_min_severity( ELogFacility, ELogSeverity ) noexcept;
		void facility_rename( ELogFacility, char const*, char const* ) noexcept;
		
		void consumer_add( unique_ptr<Consumer>, char const* );
		void consumer_remove( char const* );

		void toggle_threading( bool ); //TODO

	private:
		using ProcessFn_ = void (Logger::*)( LoggerEvent&& );

	private:
		void worker_();

		void process_synch_( LoggerEvent&& ); /*should this be NOEXCEPT?*/
		void process_queue_( LoggerEvent&& ); /*should *this* be NOEXCEPT? Queue guarantees no throwing... */

		//TODO:
		//void process_( LoggerEvent ); //XXX


		void init_() noexcept;
		void init_defaults_() noexcept;

		void worker_stop_and_join_(); //TODO

	private:
		struct LoggerState_
		{
			compat::shared_mutex mutex;

			ProcessFn_ process;
			LogFacilityMask facilityMask = ~LogFacilityMask(0);
			
#			if FLUX_DEBUG
			ELogSeverity minSeverity = ELogSeverity::trace;
#			else // !DEBUG
			ELogSeverity minSeverity = ELogSeverity::info;
#			endif // ~ DEBUG

			ELogSeverity facilityMinSeverity[kMaxLogFacilities];

		} mLoggerState;

		Clock::time_point mStartTime;
		flux_ext::BlockingConcurrentQueue<LoggerEvent> mQueue;
	
		struct WorkerData_
		{
			std::mutex mutex;
			
			ftl::Vector<unique_ptr<Consumer>> consumers;

			char const* facilityName[kMaxLogFacilities];
			char const* facilityUtf8[kMaxLogFacilities];

			ftl::Vector<std::string> groups;
		} mWorkerData;


		std::thread mWorkerThread;
};

Logger& logger() noexcept;

//--    <<< ~ flux::log::detail namespace <<<       ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(log,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_LOGGER_HXX_B937A185_9252_4E35_A3C5_AB2CFE683FAA
