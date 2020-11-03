#ifndef DETAIL_LOGGER2_HXX_BA4693B0_CCF1_4776_9FB0_AF00FDAA087D
#define DETAIL_LOGGER2_HXX_BA4693B0_CCF1_4776_9FB0_AF00FDAA087D

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/log2.hpp>
#include <flux/std/log2_ctrl.hpp>
#include <flux/std/clock.hpp>

#include <flux/compat/shared_mutex.hpp>

#include <flux/ftl/vector.hpp>

#include <flux_ext/blockingconcurrentqueue.hpp>

#include <thread>

#include <cstddef>

//--    >>> namespace = flux::log::detail >>>       ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(log,detail)
//--    constants                           ///{{{2///////////////////////////

//XXX: the following almost mirrors EMessageType in log2_ctrl.hpp 
//     either make that guaranteed somehow, or just use that one.
enum class EEventType
{
	message,

	terminate

#	if 0
	message,
	badness, //XXX?

	scopeEnter,
	scopeLeave,

	flush,

	panic,

	terminate
#	endif
};

constexpr std::size_t kMaxLogFacilities = std::size_t(ELogFacility::max);
static_assert( kMaxLogFacilities <= 32, "Only support up to 32 facilities" );

constexpr std::size_t kWorkerDequeueBuffer = 16;

constexpr ELogSeverity kDefaultMinSeverity = 
#	if FLUX_DEBUG
	ELogSeverity::trace
#	else // !DEBUG
	ELogSeverity::info
#	endif // ~ DEBUG
;

//--    types                               ///{{{2///////////////////////////

using LogFacilityMask = std::uint32_t;


struct ScopeInfo; // Opaque.

#if 0
struct ScopeInfo
{
	std::size_t nestingDepth; // XXX maybe not needed?

	Clock::time_point enteredAt;
	fml::memory_buffer label;
};
#endif

//--    Event                               ///{{{2///////////////////////////

struct EvTerminate {}; // Tag type.
struct EvMessage
{
	Message msg;
	flux_ext::fmt::memory_buffer text;
};

struct Event
{
	Event() = default;
	explicit Event( EvMessage* ) noexcept;
	explicit Event( EvTerminate ) noexcept;
	
	union
	{
		EvMessage* message;
	};

	EEventType type;
};

//--    Logger                              ///{{{2///////////////////////////
class Logger final
{
	public:
		explicit Logger( bool aDefaultSetup = true ) noexcept;
		~Logger();

	public:
		void log(
			LogMeta const&,
			flux_ext::fmt::memory_buffer&&,
			Clock::time_point
		);

		//TODO: badness() ?

		void panic(
			LogMeta const&,
			char const*,
			char const*,
			char const*,
			char const*,
			Clock::time_point
		) noexcept;
		void panic(
			LogMeta const&,
			char const*,
			compat::string_view const&,
			char const*,
			char const*,
			Clock::time_point
		) noexcept;

		ScopeInfo const* scope_enter(); //TODO
		void scope_enter( ScopeInfo const* ); //TODO
	
	public:
		bool global_min_severity( ELogSeverity ) noexcept;

		bool facility_toggle( ELogFacility, bool ) noexcept;
		bool facility_min_severity( ELogFacility, ELogSeverity ) noexcept;

		void facility_rename( ELogFacility, char const*, char const* );

		void consumer_add( unique_ptr<Consumer>, char const* );
		void consumer_remove( char const* );

	public:
		void toggle_threading( bool );

	private:
		using ProcessFn_ = void (Logger::*)( Event&& );

	private:
		void init_basic_(); //TODO : also see init_default_();

		void process_synch_( Event&& );
		void process_queue_( Event&& );

		void worker_();
		void worker_stop_and_join_();

		//TODO: these need to be thread safe. For now, just use new/delete,
		//but could use a pool later on.
		EvMessage* message_alloc_();
	 	void message_free_( EvMessage* );

		void handle_deferred_( ); //TODO log() with ELogFlag::defer

	private:
		//TODO-MAYBE: alignas() the following to avoid false sharing across
		//hardware_destructive_interference_size or whatever that is
	
		Clock::time_point mStartupTime;

		flux_ext::BlockingConcurrentQueue<Event> mQueue;
		
		//TODO: bool : mActive => for "soft shutdown"
	
		struct LoggerState_
		{
			compat::shared_mutex mutex;

			LogFacilityMask enabledFacilities = ~LogFacilityMask(0);
			ELogSeverity globalMinSeverity = kDefaultMinSeverity;

			ELogSeverity facilityMinSeverity[kMaxLogFacilities];

			ProcessFn_ process;
		} mLoggerState;

		struct WorkerState_
		{
			std::mutex mutex;

			ftl::Vector<unique_ptr<Consumer>> consumers;

			// Config
			//TODO: auto-flush timeout and severity
			//TODO: facility names
		
			// Various groups of messages.
			ftl::Vector<Message*> pending;
			ftl::Vector<Message*> elusive;
			ftl::Vector<Message*> prelude;
			ftl::Vector<Message*> deferred;

			// Housekeeping
			ftl::Vector<std::string> consumerGroupStrings;
		} mWorkerState;

		std::thread mWorkerThread;
};


//--    functions                           ///{{{2///////////////////////////

Logger& logger() noexcept;

//--    <<< ~ flux::log::detail namespace <<<       ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(log,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "logger2.inl"
#endif // DETAIL_LOGGER2_HXX_BA4693B0_CCF1_4776_9FB0_AF00FDAA087D
