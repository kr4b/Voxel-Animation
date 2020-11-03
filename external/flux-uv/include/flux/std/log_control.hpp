/*-******************************************************* -- HEADER -{{{1- */
/*- Control logging
 *
 * This is essentially a thin wrapper around loguru. See ext_misc_ and 
 * externals.md for additional information.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_LOG_CONTROL_HPP_524A56F5_E5E9_4C45_98F6_D4A2531BB18E
#define FLUX_STD_LOG_CONTROL_HPP_524A56F5_E5E9_4C45_98F6_D4A2531BB18E

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/namespace.hpp>

#include <flux/std/unique_ptr.hpp>

#include <flux_ext/loguru.hpp>

//--    >>> namespace = flux >>>                    ///{{{1///////////////////
FLUX_NAMESPACE_ENTER()
//--    constants                           ///{{{2///////////////////////////
namespace log
{
	using flux_ext::loguru::FileMode;
	using flux_ext::loguru::Verbosity;

	using flux_ext::loguru::Verbosity_OFF;
	using flux_ext::loguru::Verbosity_INFO;
	using flux_ext::loguru::Verbosity_WARNING;
	using flux_ext::loguru::Verbosity_ERROR;
	using flux_ext::loguru::Verbosity_FATAL;
}

//--    types                               ///{{{2///////////////////////////
namespace log
{
	using Message = flux_ext::loguru::Message;
	
	using LogFn = flux_ext::loguru::log_handler_t;
	using CloseFn = flux_ext::loguru::close_handler_t;
	using FlushFn = flux_ext::loguru::flush_handler_t;

	struct Consumer
	{
		virtual ~Consumer() = 0;

		virtual void log( Message const& ) noexcept = 0;
		virtual void flush() noexcept;
	};
}

//--    functions                           ///{{{2///////////////////////////
namespace log
{
	void init_main();
	void init_main( int& aArgc, char* aArgv[], char const* = "-v" );

	void init_thread( char const* );

	void add_raw_file( 
		char const*, 
		FileMode, 
		Verbosity 
	);
	void add_callback(
		char const*,
		LogFn,
		CloseFn,
		FlushFn,
		void*,
		Verbosity
	);
	void add_consumer(
		char const*,
		unique_ptr<Consumer>,
		Verbosity
	);

	void remove( char const* );

	Verbosity set_stderr_verbosity( Verbosity );
}

//--    <<< ~ flux namespace <<<                    ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE()
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_STD_LOG_CONTROL_HPP_524A56F5_E5E9_4C45_98F6_D4A2531BB18E
