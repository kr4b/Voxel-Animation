/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Logger: Native (default) consumer
 *
 * The native consumer outputs typically to stderr (or the closest equivalent).
 */
/*-***************************************************************** -}}}1- */

//TODO
//TODO
//TODO    USE sys/stdio.hpp
//TODO  
//TODO MAYBE: update sys/stdio.hpp to include basic colors? Or add
//TODO          sys/stdio-style.hpp 
//TODO with helpers for semantic colors. color schemes and more foolery.
//TODO
//TODO
//TODO

#include <flux/std/log2_ctrl.hpp>

#include <flux/std/assert.hpp>

#include <flux/compat/optional.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
#	include <unistd.h>
#	include <termios.h>

#	include <sys/types.h>
#endif // ~ !PLATFORM_WIN32

#if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
#	include <flux_dlapi/os/win32.hpp>
#endif // ~ PLATFORM_WIN32

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::log >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(log)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	enum class ENativeStream
	{
		stderror,
		stdoutput
	};

	// TODO: move this to flux-sys (probably);
	// TODO: Create a ConsumerDefaultTerm_ & ConsumerDefaultFile_ that
	//   is selected depending on isTerminal.
	struct NativeOutputCaps_
	{
		bool isTerminal;
		bool hasAnsi;
		bool hasUtf8;
		bool useColors;
	};
	
//#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	struct ConsumerNative_ final : public Consumer
	{
		public:
			ConsumerNative_( NativeOutputCaps_ const& ) noexcept;

		public: /* virtuals from Consumer */
			void log( Message const*, std::size_t ) noexcept override;
			//void flush() noexcept override;

		private:
			//void log_plain_();
			//void log_color_();

			void clear_line_() noexcept;
	
		private:
			NativeOutputCaps_ mCaps;
	};


	NativeOutputCaps_ probe_terminal_( ENativeStream = ENativeStream::stderror ) noexcept;

#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	constexpr unsigned kAnsiResponseWait_ = 50*1000; // microseconds.
#	endif // ~ PLATFORM

#if 0
//#	else // PLATFORM_WIN32
	struct ConsumerNativeWin32_ final : public Consumer
	{
		public:

		public: /* virtuals from Consumer */
			void log( Message const*, std::size_t ) noexcept override;
			void flush() noexcept;

		private:
			
	};

	bool supports_colors_();
//#	endif // ~ PLATFORM
#endif

	//TODO
	//TODO implement + document in environment.md
	//TODO
	compat::optional<NativeOutputCaps_> check_env_() noexcept;
}

//--   consumer_native()                    ///{{{2///////////////////////////
unique_ptr<Consumer> consumer_native( )
{
	auto const outputCaps = check_env_();
	
//#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	auto caps = outputCaps.value_or( probe_terminal_() );

	std::fprintf( stderr, "Caps are: %s %s %s %s\n", 
		caps.isTerminal ? "terminal":"not-a-terminal",
		caps.hasAnsi ? "ANSI-enabled" : "ANSI-disabled",
		caps.hasUtf8 ? "UTF-8-enabled" : "UTF-8-disabled",
		caps.useColors ? "with-colors" : "monochrome" 
	);

	//TODO
	//TODO
	//TODO
	//TODO

	return make_unique<ConsumerNative_>( caps );
//#	else // PLATFORM_WIN32
//#		error "TODO"
//#	endif // ~ PLATFORM
}

//--    $ ConsumerNative_                   ///{{{2///////////////////////////
namespace
{
//#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	ConsumerNative_::ConsumerNative_( NativeOutputCaps_ const& aCaps ) noexcept
		: mCaps( aCaps )
	{}

	//TODO
	//TODO
	//TODO
	//TODO

	void ConsumerNative_::log( Message const* aMessages, std::size_t aCount ) noexcept
	{
		FLUX_ASSERT( aMessages );

	//TODO: LOCKING BETWEEN clear_line_() and actual printing.
		
		//if( mCaps.hasAnsi )
		clear_line_();
		
		//TODO stuff...
		for( std::size_t i = 0; i < aCount; ++i )
		{
			auto const& msg = aMessages[i];
			std::fprintf( stderr, "CONSUMER-LOGGO: (%u,%u,0x%x) [%s:%d // %s]: %.*s\n",
				uint32_t(msg.meta.severity),
				uint32_t(msg.meta.facility),
				uint32_t(msg.meta.flags),
				msg.meta.aux.sourceFile,
				msg.meta.aux.sourceLine,
				msg.meta.aux.sourceFunc,
				int(msg.message.size()), //XXX- int_cast<>
				msg.message.data()
			);
		}
	}

	void ConsumerNative_::clear_line_() noexcept
	{
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		if( mCaps.hasAnsi )
		{
::flockfile( stderr );
::flockfile( stdout );
		
			// The half-way line detection works by inspecting the current
			// position in the terminal. If there is buffered output remaining,
			// that will possibly mess up the method. So flush both buffers
			// first.
			std::fflush( stdout ); std::fflush( stderr );

			auto const fd = ::fileno( stderr );
			FLUX_ASSERT( -1 != fd );
			
			termios initial{};
			if( -1 != ::tcgetattr( fd, &initial ) )
			{
				termios modified = initial;
				modified.c_lflag &= ~(ECHO|ICANON);
				::tcsetattr( fd, TCSANOW, &modified );

				// read cursor pos
				std::fprintf( stderr, "\033[6n" ); std::fflush( stderr );

				int x, y;
				if( 2 == std::scanf( "\033[%d;%dR", &y, &x ) )
				{
					if( x != 1 ) std::fputc( '\n', stderr );
				}
				
				::tcsetattr( fd, TCSANOW, &initial );
			}

::funlockfile( stdout );
::funlockfile( stderr );
		}
#		else // PLATFORM_WIN32
		if( mCaps.isTerminal )
		{
			namespace Win32_ = dlapi::os::Win32;
			auto const* w32 = dlapi::os::win32();

			if( !w32 )
				return;

			Win32_::HANDLE handle = w32->getStdHandle( Win32_::std_error_handle );
			FLUX_ASSERT( Win32_::invalid_handle_value != handle );

			// See comment above (non-Win32).
			std::fflush( stdout ); std::fflush( stderr );

			Win32_::CONSOLE_SCREEN_BUFFER_INFO sbi{};
			if( !w32->getConsoleScreenBufferInfo( handle, &sbi ) )
				return;

			if( 0 != sbi.dwCursorPosition.X )
				std::fputc( '\n', stderr );
		}
#		endif // ~ PLATFORM
	}
//#	endif // ~ !PLATFORM_WIN32
}

//--    $ check_env_()                      ///{{{2///////////////////////////
namespace
{
	compat::optional<NativeOutputCaps_> check_env_() noexcept
	{
		//TODO
		//TODO
		//TODO
		
		return compat::nullopt;
	}
}
//--    $ probe_terminal_()                 ///{{{2///////////////////////////
namespace
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	NativeOutputCaps_ probe_terminal_( ENativeStream aStreamId ) noexcept
	{
		FILE* stream = nullptr;
		switch( aStreamId )
		{
			case ENativeStream::stderror: stream = stderr; break;
			case ENativeStream::stdoutput: stream = stdout; break;
		}
		FLUX_ASSERT( stream );

		NativeOutputCaps_ ret{};

		// Check if we're on a tty
		// If we're not on a TTY, assume UTF-8 by default.
		auto const fd = ::fileno( stream );
		if( -1 == fd || !::isatty( fd ) )
		{
			ret.hasUtf8 = true;
			return ret;
		}
		
		ret.isTerminal = true;

		// UTF-8 and ANSI support
		// Engage trickery. Based around https://stackoverflow.com/a/30698932
		//
		// Essentially, issue an ansi sequence that reads the cursor position.
		// If we don't get a response relatively quickly, assume that the
		// terminal doesn't support ansi sequences. This requires us to put
		// the input of the terminal into non-blocking mode (and additionally
		// we disable echo:ing of stdin for the duration -- the responses to
		// the ansi sequences end up there).
		//
		// Secondly, this outputs a 3-byte UTF-8 codepoint. If that moves the
		// cursor forward by one, assume that we're UTF-8 enabled. Otherwise,
		// assume that we're not.
		//
		// The downside of this method is that truly limited terminals will
		// have a tiny amount of garbage characters at startup...
		{
::flockfile( stderr );
::flockfile( stdout );
		
			// Flush both stdout and stderr, so that they don't interfere
			// with the probing here (there seems to be some problems if
			// there are leftovers in either buffer).
			std::fflush( stdout ); std::fflush( stderr );
			
			termios initial{};
			if( -1 != ::tcgetattr( fd, &initial ) )
			{
				termios modified = initial;
				modified.c_lflag &= ~(ECHO|ICANON);
				::tcsetattr( fd, TCSANOW, &modified );

				// Store cursor pos, read cursor pos, [utf-8 char], read again
				std::fprintf( stream, "\033[s\033[6n\xe2\x88\xb4\033[6n" );
				std::fflush( stream );
				
				::fd_set rd;
				FD_ZERO( &rd );
				FD_SET( fd, &rd );

				::timeval wait;
				wait.tv_sec = 0;
				wait.tv_usec = kAnsiResponseWait_;

				if( 1 == ::select( fd+1, &rd, nullptr, nullptr, &wait ) )
				{
					// Read back 2x cursor position report
					int ix, iy, cx, cy;
					if( 4 == std::scanf( "\033[%d;%dR\033[%d;%dR", &iy, &ix, &cy, &cx ) )
					{
						ret.hasAnsi = true;
						if( ix+1 == cx ) 
							ret.hasUtf8 = true;

						// Restore cursor pos, and erase from cursor to EOL
						std::fprintf( stream, "\033[u\033[0K" ); 
						std::fflush( stream );
					}
				}

				::tcsetattr( fd, TCSANOW, &initial );
			}

::funlockfile( stdout );
::funlockfile( stderr );
		}

		// Color support
		// XXX- also use ret.hasAnsi? Technically, that would be a
		// prerequisite. (However, due to the timeout, we might get a false
		// negative there.)
		if( char const* term = std::getenv( "TERM" ) )
		{
			// Based on the list used by loguru
			// See https://github.com/emilk/loguru
			ret.useColors = 
			       0 == std::strcmp( term, "linux" )
				|| 0 == std::strcmp( term, "xterm" )
				|| 0 == std::strcmp( term, "xterm-color" )
				|| 0 == std::strcmp( term, "xterm-256color" )
				|| 0 == std::strcmp( term, "xterm-termite" )
				|| 0 == std::strcmp( term, "screen" )
				|| 0 == std::strcmp( term, "screen-256color" )
				|| 0 == std::strcmp( term, "screen.xterm-256color" )
				|| 0 == std::strcmp( term, "tmux-256color" )
				|| 0 == std::strcmp( term, "rxvt-unicode-256color" )
				|| 0 == std::strcmp( term, "cygwin" )
			;
		}
		else
			ret.useColors = false;
	
		return ret;
	}
#	else // PLATFORM_WIN32
	NativeOutputCaps_ probe_terminal_( ENativeStream aStreamId ) noexcept
	{
		NativeOutputCaps_ ret{};
		ret.hasUtf8 = true; // Default to UTF8 support, disable if it only if we're on a terminal without UTF8

		// Win32 API
		namespace Win32_ = dlapi::os::Win32;
		auto const* w32 = dlapi::os::win32();

		if( !w32 )
			return ret;

		// Resolve stream
		Win32_::DWORD handleId = 0;
		switch( aStreamId )
		{
			case ENativeStream::stderror: handleId = Win32_::std_error_handle; break;
			case ENativeStream::stdoutput: handleId = Win32_::std_output_handle; break;
		}
		FLUX_ASSERT( handleId );

		Win32_::HANDLE handle = w32->getStdHandle( handleId );
		if( Win32_::invalid_handle_value == handle )
			return ret;

		// Check if it's a terminal and set up the terminal
		auto const ftype = w32->getFileType( handle );
		if( Win32_::file_type_char == ftype )
		{
			ret.isTerminal = true;

			if( !w32->setConsoleOutputCP( Win32_::cp_utf8 ) )
				ret.hasUtf8 = false;

			Win32_::DWORD mode;
			if( w32->getConsoleMode( handle, &mode ) )
			{
				mode |= Win32_::enable_virtual_terminal_processing;
				if( w32->setConsoleMode( handle, mode ) )
				{
					ret.hasAnsi = true;
					ret.useColors = true;
				}
			}
		}
		else if( Win32_::file_type_pipe == ftype )
		{
			// Note: the Cygwin terminal has Win32_::file_type_pipe. See
			// Haskell mintty library for logic of detecting mintty:
			// https://github.com/RyanGlScott/mintty

			//TODO
			//TODO note that this requires additional logic with 
			//TODO e.g., clear_line_()
			//TODO
		}

		return ret;
	}
#	endif // ~ PLATFORM
}

//--    <<< ~ flux::log namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(log)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
