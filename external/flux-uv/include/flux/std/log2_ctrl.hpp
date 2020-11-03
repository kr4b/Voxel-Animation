/*-******************************************************* -- HEADER -{{{1- */
/*-	Control logging
 *
 * WIP - new iteration
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_LOG2_CTRL_HPP_E2C4E9A7_FC93_4BDF_9B90_77BF3B5F2826
#define FLUX_STD_LOG2_CTRL_HPP_E2C4E9A7_FC93_4BDF_9B90_77BF3B5F2826

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/log2.hpp>
#include <flux/std/unique_ptr.hpp>

#include <flux/compat/string_view.hpp>

#include <flux/pp/join.hpp>
#include <flux/io/forward.hpp>

//--    macros                      ///{{{1///////////////////////////////////

#define FLUX_LOG_SUPPRESS_IN_THIS_SCOPE()           \
	::flux::log::detail::SuppressInScope            \
		FLUX_PP_JOIN2(fluxLogSuppress,__LINE__,_)   \
	; /*ENDM*/

//--    >>> namespace = flux::log >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(log)
//--    constants                           ///{{{2///////////////////////////

enum class ELogMsgKind
{
	//TODO  ??
	//TODO   ??
	message,

	scopeEnter,
	scopeLeave,

	badness
};

//TODO:
enum class ENativeConsumerColor
{
	enable,
	disable,
	forced
};

//--    types                               ///{{{2///////////////////////////

struct Message
{
	//char const* message;
	//char const* threadid;
	//char const* facility;
	compat::string_view message;
	compat::string_view threadid;
	compat::string_view facilityUtf8, facilityAscii;

	uint64_t timestamp;
	uint8_t nesting;

	LogMeta meta;
	ELogMsgKind kind;
};

struct Consumer
{
	virtual ~Consumer() = 0;

	//TODO: log() with multiple messages at once. So we don't have to do
	//weirdo stuff like clear_line_() for each message.
	virtual void log( Message const*, std::size_t ) noexcept = 0;
	virtual void flush() noexcept;
};

namespace detail
{
	struct SuppressInScope
	{
		SuppressInScope() noexcept, ~SuppressInScope();
	};
}

//--    functions                           ///{{{2///////////////////////////

//TODO
//   execenv.hpp  (c.f. appinfo.hpp)
//     - set thread name
//     - install global signal handler???

	// Runtime configuration

//TODO-fixme:  all of the config_() can throw, because acquiring mutexes isn't noexcept. :-/

void config_min_severity( ELogSeverity ) noexcept;

void config_facility_enable( ELogFacility ) noexcept;
void config_facility_disable( ELogFacility ) noexcept;
void config_facility_min_severity( ELogFacility, ELogSeverity ) noexcept;

void config_facility_rename( 
	ELogFacility, 
	char const* aNameUtf8, 
	char const* aNameAscii = nullptr 
) noexcept;

// TODO: allow aIdent be a group of names separated by $SOMETHING, e.g. UNIT SEPARATOR \037
//   Note: internally prepend and append SEPARATOR to string so we can always just look for 
//   "\037%\037".
void config_consumer_add( unique_ptr<Consumer>, char const* aIdent = nullptr );
void config_consumers_remove( char const* aIdent );

//TODO: config_consumer_facility_min_severity( char const*, ELogFacility, ELogSeverity );
//TODO-maybe: config_consumer_string( char const*, char const* );

void config_log_threading( bool aDedicatedThread = true );

	// Special
void shutdown(); // Do everything we'd do on a shutdown anyway.
void flush(); // XXX

//void reroute( Consumer* = nullptr ); //TODO-doc: temporarily for tests etc
//void resume(); // un reroute().

//TODO: suppress teporarily / in a scope     }   For tests, so we can suppress
//TODO: reroute temporarily                  }     LOG() spam there.
bool log_suppress( Consumer* ); //XXX helper to do this in a scope.
void log_resume();  // XXX nested, counter, stack with fixed depth?


	// Consumers
unique_ptr<Consumer> consumer_native( /*TODO: options */ );

#if FLUX_WITH_FLUXIO
unique_ptr<Consumer> consumer_stream( unique_ptr<io::Stream> ); 
//TODO: default stream that is .gz and rotates? Where should it go?
#endif // ~ WITH_FLUXIO

//TODO
unique_ptr<Consumer> consumer_syslog(
	/* metadata */
);

//--    <<< ~ flux::log namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(log)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_STD_LOG2_CTRL_HPP_E2C4E9A7_FC93_4BDF_9B90_77BF3B5F2826
