/*-******************************************************* -- HEADER -{{{1- */
/*-	FLUX_LOG() et al.
 *
 * WIP - new iteration
 *
 * TODO: Alloc passing of source_location() to the various LOG() variants?
 * TODO: human-readable default names for facilities? Overrideable?
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_LOG2_HPP_0411F856_CE8C_4283_B3A4_837B771702DF
#define FLUX_STD_LOG2_HPP_0411F856_CE8C_4283_B3A4_837B771702DF

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/namespace.hpp>
#include <flux/setup/buildcfg.hpp>

#include <flux/compat/string_view.hpp>
#include <flux/compat/current_func.hpp>
#include <flux/compat/integer_sequence.hpp>

#include <flux/std/inttypes.hpp>

#include <flux/util/hint.hpp>

#include <flux_ext/fmtlib.hpp>

#include <typeinfo>

//--    configuration               ///{{{1///////////////////////////////////

#define FLUXCFG_STD_CHECKED_FMTSTRING   1 // TODO #ifdef et al. for outside config

//--    macro interface             ///{{{1///////////////////////////////////

#define FLUX_LOG_SCOPE( spec, ... ) //TODO TODO TODO TODO
#define FLUX_LOG_BADNESS( ... ) // TODO TODO TODO TODO

#if FLUXCFG_STD_CHECKED_FMTSTRING
	/* To check the format string using the FLUX_FMT_STRING() / FMT_STRING()
	 * macros, we need to separate the first argument from the __VA_ARGS__.
	 * (Note: we can't do FLUX_LOG( spec, fmt, ... ), since the C++ standard
	 * requires macro varargs to have at least one argument; compilers don't
	 * always enforce this, though). This is a bit tricky to do in a fully
	 * conformant manner; C++20 improves things slight with __VA_OPT__.
	 *
	 * In short, we insert an additional dummy argument, that is either ignored
	 * (first argument) or that we strip out later via template stuff.  See
	 * detail::LogFwd at the bottom of this file for the second.
	 */

#	define FLUX_LOG( spec, ... ) do {                                           \
		auto const meta = FLUX_LOG_RECORD_METADATA_(spec);                      \
		if( uint32_t(meta.severity) < ::flux::log::kStaticMinSeverity ) break;  \
		FLUX_HINT_ASSUME( meta.facility < 32 );                                 \
		if( !((1u<<meta.facility) & ::flux::log::kStaticFacilityMask) ) break;  \
		::flux::log::detail::LogFwd( meta,                                      \
			FLUX_LOG_CHECKED_FIRSTARG_(__VA_ARGS__,),                           \
			FLUX_LOG_CHECKED_REST_(__VA_ARGS__, flux::log::detail::Sentinel{})  \
		);                                                                      \
	} while(0) /*ENDM*/
#	define FLUX_LOG_IF( expr, spec, ... )  do { if( (expr) ) {                  \
		auto const meta = FLUX_LOG_RECORD_METADATA_(spec);                      \
		if( uint32_t(meta.severity) < ::flux::log::kStaticMinSeverity ) break;  \
		FLUX_HINT_ASSUME( meta.facility < 32 );                                 \
		if( !((1u<<meta.facility) & ::flux::log::kStaticFacilityMask) ) break;  \
		::flux::log::detail::LogFwd( meta,                                      \
			FLUX_LOG_CHECKED_FIRSTARG_(__VA_ARGS__,),                           \
			FLUX_LOG_CHECKED_REST_(__VA_ARGS__, flux::log::detail::Sentinel{})  \
		);                                                                      \
	} } while(0) /*ENDM*/

#	define FLUX_LOG_CHECKED_FIRSTARG_(a,...)  FMT_STRING(a)
#	define FLUX_LOG_CHECKED_REST_(a,...)  __VA_ARGS__

#else // !CHECKED_FMTSTRING
#	define FLUX_LOG     FLUX_LOG_UNCHECKED
#	define FLUX_LOG_IF  FLUX_LOG_IF_UNCHECKED
#endif

	// Unechecked interfaces:
#define FLUX_LOG_UNCHECKED( spec, ... ) do {                                    \
		auto const meta = FLUX_LOG_RECORD_METADATA_(spec);                      \
		if( uint32_t(meta.severity) < ::flux::log::kStaticMinSeverity ) break;  \
		FLUX_HINT_ASSUME( meta.facility < 32 );                                 \
		if( !((1u<<meta.facility) & ::flux::log::kStaticFacilityMask) ) break;  \
		::flux::log::log( meta, __VA_ARGS__ );                                  \
	} while(0) /*ENDM*/
#define FLUX_LOG_IF_UNCHECKED( expr, spec, ... )  do { if( (expr) ) {           \
		auto const meta = FLUX_LOG_RECORD_METADATA_(spec);                      \
		if( uint32_t(meta.severity) < ::flux::log::kStaticMinSeverity ) break;  \
		FLUX_HINT_ASSUME( meta.facility < 32 );                                 \
		if( !((1u<<meta.facility) & ::flux::log::kStaticFacilityMask) ) break;  \
		::flux::log::log( meta, __VA_ARGS__ );                                  \
	} } while(0) /*ENDM*/

	// Helpers
#define FLUX_LOG_RECORD_METAAUX_(fn) { __FILE__, fn, uint32_t(__LINE__) }
#define FLUX_LOG_RECORD_METADATA_(spec) [] (char const* aFunc) {                \
		using namespace ::flux::log::detail::short_names;                       \
		using namespace ::flux::extend::log_short_names;                        \
		return +(::flux::log::detail::LogSpec_<>{                               \
			0,                                                                  \
			::flux::uint16_t(::flux::log::ELogFacility::unspecified),           \
			0,                                                                  \
			FLUX_LOG_RECORD_METAAUX_(aFunc)                                     \
		}|spec);                                                                \
	}(FLUX_COMPAT_FUNCTION_NAME) /*ENDM*/

//--    >>> namespace = flux::extend >>>            ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(extend)
//--    log_short_names                     ///{{{2///////////////////////////

/** \brief User extensions: additional short names for `FLUX_LOG()`
 *
 * By default, `FLUX_LOG()` allows the use of a few predefined short names for
 * severities, facilities and flags. The `log_short_names` namespace can be
 * used to add additional short names for e.g., user facilities:
   \code
   FLUX_NAMESPACE_ENTER2(extend,log_short_names)
	constexpr log::ELogFacility CUSTOM = log::ELogFacility::user0;
   FLUX_NAMESPACE_LEAVE2(extend,log_short_names)
  
   FLUX_LOG( DEBUG|CUSTOM, "A log message with the user0 facility" );
   \endcode
 */
namespace log_short_names {};

//--    <<< ~ flux::extend namespace <<<             ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(extend)
//--    >>> namespace = flux::log >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(log)
//--    constants                           ///{{{2///////////////////////////

enum class ELogSeverity
{
	trace = 0u,
	debug, 
	info,
	notice,
	warn,
	error,
	critical,
	alert,
	emergency,

	// meta
	off = 0x1fu,
	inherit = 0x1eu
};

enum class ELogFacility
{
	unspecified = 0,
	
	// flux internals
	flux = 1,
	//fluxio, fluxvk, fluxgl, //XXX??
	
	// user
	user0 = 16, user1, user2, user3, user4, user5, user6, user7,
	user8, user9, user10, user11, user12, user13, user14, user15,

	// meta
	max
};

enum class ELogFlag
{
	/** \brief Omit entry unless followed by a non-prelusive one
	 *
	 * A log entry flagged `prelusive` may only be recorded once a message not
	 * flagged prelusive is entered. Prelusive messages maybe used to record a
	 * premable/prelude with boilerplate information without explicitly forcing
	 * log files (etc) to be created only for the boilerplate information.
	 *
	 * \note According to Google, the definition of "prelusive" is
     * > adjective: prelusive
     * > serving as a prelude or introduction; preliminary.
	 * It is also a neat portmanteau of "prelude" and "elusive". Messages
	 * marked this way are elusive (they might not appear) and they serve as an
	 * prelude for more more important messages.
	 */
	prelusive,
	//supplementary
	//transient
	
	//flush, // TODO: flush and synch??? also add below.

	// TODO:
	//postpone,   // "future", " ;;; postpone output until shutdown(). similar to badness. use sparingly.
};

	// Compile-time cutoffs
#if defined(FLUXCFG_LOG_MIN_SEVERITY)
constexpr uint32_t kStaticMinSeverity = uint32_t(FLUXCFG_LOG_MIN_SEVERITY);
#elif FLUX_DEBUG
constexpr uint32_t kStaticMinSeverity = uint32_t(ELogSeverity::trace);
#else
constexpr uint32_t kStaticMinSeverity = uint32_t(ELogSeverity::info);
#endif // ~ static min. severity

#if defined(FLUXCFG_LOG_FACILITY_MASK)
constexpr uint32_t kStaticFacilityMask = FLUXCFG_LOG_FACILITY_MASK;
#else
constexpr uint32_t kStaticFacilityMask = 0xffffffffu;
#endif // ~ static facility mask

//--    functions                           ///{{{2///////////////////////////

struct LogMeta
{
	uint16_t severity;
	uint16_t facility;
	uint32_t flags;

	struct Aux
	{
		char const* sourceFile;
		char const* sourceFunc;
		uint32_t sourceLine;
	} aux;
};

//TODO-maybe: a single log() that doesn't take additional args?
//TODO-for now no version of log() that just takes a FMT_STRING().
template< typename... tArgs >
void log( char const*, tArgs&&... ) noexcept;

template< typename tFmt, typename... tArgs >
void log( ELogSeverity, tFmt const&, tArgs&&... ) noexcept;
template< typename tFmt, typename... tArgs >
void log( LogMeta const&, tFmt const&, tArgs&&... ) noexcept;


template< typename... tArgs >
void log_scope_enter( ELogSeverity, char const*, tArgs&&... ) noexcept; //TODO
template< typename... tArgs >
void log_scope_enter( LogMeta const&, char const*, tArgs&&... ) noexcept; //TODO

void log_scope_leave( ELogSeverity ) noexcept; //TODO
void log_scope_leave( LogMeta const& ) noexcept; //TODO

//...
namespace detail
{
	void log( 
		LogMeta const&, 
		flux_ext::fmt::memory_buffer 
	);

	void log_scope_enter( 
		LogMeta const&, 
		flux_ext::fmt::memory_buffer
	);

	void panic(
		LogMeta const&,
		char const*,
		char const*,
		char const*,
		char const*
	) noexcept;
	void panic(
		LogMeta const&,
		char const*,
		compat::string_view const&,
		char const*,
		char const*
	) noexcept;
}

//--    helpers                             ///{{{2///////////////////////////
namespace detail
{
	namespace short_names
	{
		// Severities
		constexpr ELogSeverity TRACE = ELogSeverity::trace;
		constexpr ELogSeverity DEBUG = ELogSeverity::debug;
		constexpr ELogSeverity INFO = ELogSeverity::info;
		constexpr ELogSeverity NOTICE = ELogSeverity::notice;
		constexpr ELogSeverity WARN = ELogSeverity::warn;
		constexpr ELogSeverity ERROR = ELogSeverity::error;
		constexpr ELogSeverity CRITICAL = ELogSeverity::critical;
		constexpr ELogSeverity EMERGENCY = ELogSeverity::emergency;

		// Facilities
		constexpr ELogFacility UNSPECIFIED = ELogFacility::unspecified;
		constexpr ELogFacility FLUX = ELogFacility::flux;
		
		// Flags
		constexpr ELogFlag PRELUSIVE = ELogFlag::prelusive;
	}
	
	template< bool = false, bool = false >
	struct LogSpec_
	{
		uint16_t severity;
		uint16_t facility;
		uint32_t flags;
		
		LogMeta::Aux aux;
	};

	template< bool tFac > constexpr
	LogSpec_<true,tFac> operator| (LogSpec_<false,tFac>, ELogSeverity) noexcept;
	template< bool tSev > constexpr
	LogSpec_<tSev,true> operator| (LogSpec_<tSev,false>, ELogFacility ) noexcept;
	template< bool tSev, bool tFac > constexpr
	LogSpec_<tSev,tFac> operator| (LogSpec_<tSev,tFac>, ELogFlag) noexcept;
	template< bool tSev, bool tFac > constexpr
	LogMeta operator+ (LogSpec_<tSev,tFac>) noexcept;


	struct LogScope_
	{
		LogScope_( uint16_t, uint16_t, uint32_t ) noexcept;
		~LogScope_();
		
		private:
			uint16_t severity;
			uint16_t facility;
			uint32_t flags;
	};

	constexpr 
	char const* convert_fmtstr( char const* ) noexcept;
	compat::string_view convert_fmtstr( fmt::basic_string_view<char> const& ) noexcept;

#	if FLUXCFG_STD_CHECKED_FMTSTRING
	struct Sentinel
	{};

	class LogFwd
	{
		template< std::size_t > struct Key_ {};
		
		public:
			template< typename... tArgs >
			LogFwd( LogMeta const& aMeta, tArgs&&... aArgs ) noexcept;

		private:
			template< std::size_t... tIndices, typename tTuple >
			LogFwd( Key_<0>, LogMeta const& aMeta, compat::index_sequence<tIndices...>, tTuple&& ) noexcept;

			template< typename... tArgs >
			LogFwd( Key_<1>, LogMeta const& aMeta, tArgs&&... ) noexcept;
	};
#	endif // ~ CHECKED_FMTSTRING
}

//--    <<< ~ flux::log namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(log)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/log2.inl"
#endif // FLUX_STD_LOG2_HPP_0411F856_CE8C_4283_B3A4_837B771702DF
