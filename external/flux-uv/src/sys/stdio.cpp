/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Standard input/output helpers
 *
 * WARNING: Care must be taken if e.g., using FLUX_LOG() and similar, as that
 * code uses functionality from here internally.
 *
 * TODO: EStdioStream::in is a bit weird (c.f. probe_()). How do we handle it?
 */
/*-***************************************************************** -}}}1- */

#include <flux/sys/stdio.hpp>

#include <flux/std/assert.hpp>
#include <flux/std/format.hpp>
#include <flux/std/inttypes.hpp>

#include <flux/compat/exchange.hpp>
#include <flux/compat/shared_mutex.hpp>

#include <flux/util/intent.hpp>
#include <flux/util/scope_exit.hpp>

#include <flux_dlapi/os/win32.hpp>

#if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
#	include <unistd.h>
#	include <termios.h>
#	include <sys/types.h>
#endif // ~ PLATFORM

#include <mutex>
#include <sstream>

#include <cstdlib>
#include <cstring>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	constexpr uint32_t kAnsiResponseWaitUS_ = 50*1000; // microseconds

	constexpr EStdioUsableFmt kDefaultFmts = 
		EStdioUsableFmt::bold | EStdioUsableFmt::faint | EStdioUsableFmt::italic | EStdioUsableFmt::underline
	;
#	else // PLATFORM_WIN32
	namespace Win32_ = dlapi::os::Win32;

	constexpr EStdioUsableFmt kDefaultFmts = 
		EStdioUsableFmt::bold | EStdioUsableFmt::underline
	;

	Win32_::HANDLE win32_handle_( dlapi::os::Win32api const*, EStdioStream ) noexcept;
#	endif // ~ PLATFORM

	compat::shared_mutex gStdioMut_;
	StdioCaps gStdioCaps_[3] = {
		{ false, false, false, false, EStdioColorMode(0), EStdioColorTheme(0), kDefaultFmts },
		{ false, false, false, false, EStdioColorMode(0), EStdioColorTheme(0), kDefaultFmts },
		{ false, false, false, false, EStdioColorMode(0), EStdioColorTheme(0), kDefaultFmts }
	};

	StdioCaps caps_load_( EStdioStream ) noexcept;
	void caps_update_( EStdioStream, StdioCaps const& ) noexcept;

	FILE* stdio_stream_( EStdioStream ) noexcept;

#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	bool string_in_list_( char const*, char const* const*, std::size_t );

	template< class tQuery, class tResponse >
	bool ansi_query_( int aFd, tQuery&&, tResponse&&, long aTimeoutUS = kAnsiResponseWaitUS_ );
#	endif
}

//--    StdioLock                           ///{{{2///////////////////////////
StdioLock::StdioLock() noexcept
	: mLockState(0)
{}
StdioLock::~StdioLock()
{
	if( ~0u == mLockState )
	{
		stdio_unlock( EStdioStream::err );
		stdio_unlock( EStdioStream::out );
	}
	else if( mLockState & 1 )
	{
		stdio_unlock( EStdioStream(mLockState>>1) );
	}
}

StdioLock::StdioLock( EStdioStream aStream, bool aLockOthers ) noexcept
{
	if( aLockOthers && aStream != EStdioStream::in )
	{
		stdio_lock( EStdioStream::out );
		stdio_lock( EStdioStream::err );
		mLockState = ~0u;
	}
	else
	{
		stdio_lock( aStream );
		mLockState = (unsigned(aStream) << 1) | 1;
	}
}


StdioLock::StdioLock( StdioLock&& aOther ) noexcept
	: mLockState( compat::exchange( aOther.mLockState, 0 ) )
{}
StdioLock& StdioLock::operator=( StdioLock&& aOther) noexcept
{
	std::swap( mLockState, aOther.mLockState );
	return *this;
}

//--    $ ansi_query_()                     ///{{{2///////////////////////////
namespace
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	template< class tQuery, class tResponse > inline
	bool ansi_query_( int aFd, tQuery&& aQuery, tResponse&& aResponse, long aTimeoutUS )
	{
		/* First lock both stdout and stderr, to avoid other threads
		 * interfering with the following (very non-atomic) code. Then flush
		 * both streams, as unflushed data may interfere with the method as
		 * well.
		 */
		::flockfile( stdout ); ::flockfile( stderr );
		FLUX_UTIL_ON_SCOPE_EXIT
		{
			::funlockfile( stderr ); ::funlockfile( stdout );
		};
		
		std::fflush( stdout ); std::fflush( stderr );
		//TODO flush stdin?? (note: **not** with fflush(stdin) - nonstandard)
		
		// For the sequence, disable echoing and enter noncanonical mode. See
		// `man 3 termios` for details on what that entails.
		bool ret = false;

		termios initial{};
		if( -1 != ::tcgetattr( aFd, &initial ) )
		{
			termios modified = initial;
			modified.c_lflag &= ~(ECHO|ICANON);
			::tcsetattr( aFd, TCSANOW, &modified );

			aQuery();
			
			// Wait (briefly) for a response
			::fd_set rd;
			FD_ZERO( &rd );
			FD_SET( aFd, &rd );

			::timeval wait;
			wait.tv_sec = 0;
			wait.tv_usec = aTimeoutUS;

			if( 1 == ::select( aFd+1, &rd, nullptr, nullptr, &wait ) )
			{
				ret = aResponse();
				//TODO flush stdin?? (note: **not** with fflush(stdin) - nonstandard)
			}

			// Restore original terminal settings
			::tcsetattr( aFd, TCSANOW, &initial );
		}

		return ret;
	}
#	endif
}

//--    stdio_probe()                       ///{{{2///////////////////////////
StdioCaps stdio_probe( EStdioStream aStream, bool aTryUpgrade, bool aRefresh ) noexcept
{
	StdioCaps ret = caps_load_( aStream );

	if( !aRefresh && EStdioColorMode(0) != ret.colorMode )
		return ret;

	ret = StdioCaps{};
	ret.colorMode = EStdioColorMode::none;
	ret.colorTheme = EStdioColorTheme::plain;

#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	FLUX_INTENT_UNUSED(aTryUpgrade);
	
	auto const stream = stdio_stream_( aStream );
	auto const fd = ::fileno( stream );
	if( -1 == fd || !::isatty( fd ) )
	{
		ret.hasUtf8 = true;

		caps_update_( aStream, ret );
		return ret;
	}

	ret.isTerminal = true;

	/* UTF-8 and ANSI support
	 * Engage trickery. Based around https://stackoverflow.com/a/30698932
	 *
	 * Essentially, issue an ansi sequence that reads the cursor position.
	 * If we don't get a response relatively quickly, assume that the
	 * terminal doesn't support ansi sequences. This requires us to put
	 * the input of the terminal into non-blocking mode (and additionally
	 * we disable echo:ing of stdin for the duration -- the responses to
	 * the ansi sequences end up there).
	 *
	 * Secondly, this outputs a 3-byte UTF-8 codepoint. If that moves the
	 * cursor forward by one, assume that we're UTF-8 enabled. Otherwise,
	 * assume that we're not.
	 *
	 * The downside of this method is that truly limited terminals will
	 * have a tiny amount of garbage characters at startup...
	 */
	ansi_query_( fd, 
		[&] {
			// Print the sequence for
			//   (1) Store cursor pos
			//   (2) read cursor pos
			//   (3) [3-byte-utf-8 char]
			//   (2) read cursor pos again
			std::fputs( "\033[s\033[6n\xe2\x88\xb4\033[6n", stream );
			std::fflush( stream );
		},
		[&] () -> bool {
			// Read back 2x cursor position report
			int ix, iy, cx, cy;
			if( 4 == std::scanf( "\033[%d;%dR\033[%d;%dR", &iy, &ix, &cy, &cx ) )
			{
				ret.hasAnsi = true;
				if( ix+1 == cx ) 
					ret.hasUtf8 = true;

				// Restore cursor pos, and erase from cursor to EOL
				std::fputs( "\033[u\033[0K", stream ); 
				std::fflush( stream );
				return true;
			}
			
			return false;
		}
	);
	
	// Determine color support
	/* Note: technically ret.hasAnsi is a prerequisite. However, due to the
	 * timeout above, there might be false negatives. For this, instead 
	 * trust whatever the system is configured to tell us.
	 */
	auto const* term = std::getenv( "TERM" );
	if( term )
	{
		// Based on the list used by loguru
		// See https://github.com/emilk/loguru
		//
		// Some adjustments: linux console supports RGB colors, "screen.linux"
		// doesn't.
		//
		// I guess most "xterm" will actually be "xterm-256color", but let's
		// leave that as-is.
		//
		// Screen seems to generally reduce things to 8bit colors (or the
		// weird 88-color mode?) according to some quick tests. Not sure what
		// happens when screen is run in a non-color terminal; the linux console
		// becomes "screen.linux". However, screen.xterm-256color for example 
		// doesn't allow RGB anymore.
		//
		// rxvt seems to be odd. By default, I don't get anything beyond basic
		// colors. However, when running under `screen`, 8bit colors seem to
		// work. 
		static constexpr char const* rgb[] = {
			"xterm-256color", "tmux-256color", "rxvt-unicode-256color",
			"cygwin", "linux"
		};
		static constexpr char const* _8bit[] = {
			"screen", "screen-256color", "screen.xterm-256color"
		};
		static constexpr char const* basic[] = {
			"xterm", "xterm-color", "xterm-termite", "screen.linux", "rxvt",
			"rxvt-unicode"
		};

		if( string_in_list_( term, rgb, sizeof(rgb)/sizeof(rgb[0]) ) )
			ret.colorMode = EStdioColorMode::rgb;
		else if( string_in_list_( term, _8bit, sizeof(_8bit)/sizeof(_8bit[0]) ) )
			ret.colorMode = EStdioColorMode::_8bit;
		else if( string_in_list_( term, basic, sizeof(basic)/sizeof(basic[0]) ) )
			ret.colorMode = EStdioColorMode::basic;
	}
	
	/* Determine color "theme".
	 *
	 * This is based on several sources:
	 *  (1)  The logic of Vim: https://github.com/vim/vim/blob/05c00c038bc16e862e17f9e5c8d5a72af6cf7788/src/option.c#L3974
	 *  (2)  The mysterious xterm qcontrol sequences that extend (?) ANSI
	 *       X3.64 include queries that suprising numbers of terminals seem to
	 *       support. https://stackoverflow.com/questions/2507337/how-to-determine-a-terminals-background-color and https://bugzilla.gnome.org/show_bug.cgi?id=733423#c1
	 */
	bool haveTheme = false;

	// See list from vim. Note that the cygwin terminal seems to support
	// the xterm-query sequence, so it's not included here.
	//
	// All these have a dark color theme by default.
	if( term )
	{
		static constexpr char const* fixed[] = {
			"linux", "screen.linux", "putty"
		};
		
		if( string_in_list_( term, fixed, sizeof(fixed)/sizeof(fixed[0]) ) )
		{
			haveTheme = true;
			ret.colorTheme = EStdioColorTheme::dark;
		}
	}

	// If COLORFGBG is set, use that (even though it might be 
	// inconsistent (? -- see linked Gnome report above)).
	if( !haveTheme )
	{
		if( auto const* cfgbg = std::getenv( "COLORFGBG" ) )
		{
			// See Vim code linked above.
			if( auto const* p = std::strrchr( cfgbg, ';' ) )
			{
				if( p[1] != '\0' && p[2] == '\0' )
				{
					haveTheme = true;
					if( (p[1] >= '0' && p[1] <= '6') || p[1] == '8' ) 
						ret.colorTheme = EStdioColorTheme::dark;
					else
						ret.colorTheme = EStdioColorTheme::light;
				}
			}
		}
	}

	// Try the xterm query sequence
	if( !haveTheme && ret.hasAnsi )
	{
		ansi_query_( fd,
			[&] {
				// Print the sequence for
				//   (1) Store cursor pos
				//   (2) Query sequence, ending in \a (=bell)
				std::fputs( "\033[s\e]11;?\a", stream );
				std::fflush( stream );
			},
			[&] () -> bool {
				float r = -1.f, g = -1.f, b = -1.f;

				// Read back report. The report has the same form as the query
				// except that the `?` is replaced by the response. The
				// response takes a few different forms. See e.g. xtermcontrol
				// man page, or "Color Names" in X (7x): 
				// https://www.x.org/releases/X11R6.7.0/doc/X.7.html
				//
				// We handle
				// - "rgb:R/G/B", "rgb:RR/GG/BB", or "rgb:RRRR/GGGG/BBBB"
				// - "#RGB", #RRGGBB, #RRRRGGGGBBBB
				//
				// NOTE: Terminology terminates the response with ESC instead
				// of BEL and occasionally appends some additional garbage to
				// it. Try to be resistant to this.
				unsigned ir, ig, ib;
				char buf[128], term = '\0';

				//if( 1 == std::scanf( "\033]11;%[^\a]\a", buffer ) )
				if( 2 == std::scanf( "\033]11;%[a-zA-Z0-9/#:]%c", buf, &term ) )
				{
					buf[127] = '\0';

					float factor = -1.f;
					if( '#' == buf[0] )
					{
						char szr[6], szg[6], szb[6];
						switch( std::strlen(buf) )
						{
							case 4: { // '#RGB'
								szr[0] = buf[1]; szr[1] = '\0';
								szg[0] = buf[2]; szg[1] = '\0';
								szb[0] = buf[3]; szb[1] = '\0';
								factor = float(0xf);
								break;
							}
							case 7: { // '#RRGGBB'
								szr[0] = buf[1]; szr[1] = buf[2]; szr[2] = '\0';
								szg[0] = buf[1]; szg[1] = buf[2]; szg[2] = '\0';
								szb[0] = buf[1]; szb[1] = buf[2]; szb[2] = '\0';
								factor = float(0xff);
								break;
							}
							case 13: { // '#RRRRGGGGBBBB'
								std::memcpy( szr, buf+1+0*4, 4 ); szr[5] = '\0';
								std::memcpy( szg, buf+1+1*4, 4 ); szg[5] = '\0';
								std::memcpy( szb, buf+1+2*4, 4 ); szb[5] = '\0';
								factor = float(0xffff);
								break;
							}
						}

						if( factor >= 0.f
							&& 1 == std::sscanf( szr, "%x", &ir )
							&& 1 == std::sscanf( szg, "%x", &ig )
							&& 1 == std::sscanf( szb, "%x", &ib ) )
						{
							r = ir/factor;
							g = ig/factor;
							b = ib/factor;
						}
					}
					else if( 3 == std::sscanf( buf, "rgb:%x/%x/%x", &ir, &ig, &ib ) )
					{
						float factor = -1.f;
						switch( std::strlen( buf ) )
						{
							case 9: // rgb:R/G/B 
								factor = float(0xf);
								break;
							case 12: // rgb:RR/GG/BB 
								factor = float(0xff);
								break;
							case 18: // rgb:RRRR/GGGG/BBBB 
								factor = float(0xffff);
								break;
						}

						if( factor >= 0.f )
						{
							r = ir/factor;
							g = ig/factor;
							b = ib/factor;
						}
					}
				}

				// NOTE-terminology workaround: flush extra trailing '\\'
				if( '\033' == term )
				{
					char c = std::getchar();
					FLUX_ASSERT( '\\' == c ); //XXX-???
				}

				// Restore cursor pos, and erase from cursor to EOL
				std::fputs( "\033[u\033[0K", stream ); 
				std::fflush( stream );

				// And figure out if we're light or dark Compute the Y
				// (a.k.a the Yuminance) in YIQ and see if it's more than 0.5
				// or not. :shrug:
				if( r >= 0.f && g >= 0.f && b >= 0.f )
				{
					if( 0.299f * r + 0.587f * g + 0.114f * b >= 0.5f )
						ret.colorTheme = EStdioColorTheme::light;
					else
						ret.colorTheme = EStdioColorTheme::dark;

					haveTheme = true;
					return true;
				}

				return false;
			}
		);
	}

	// Take a final guess, see list from Vim (linked above)
	if( !haveTheme && term )
	{
		static constexpr char const* dark[] = {
			"cygwin"
		};

		if( string_in_list_( term, dark, sizeof(dark)/sizeof(dark[0]) ) )
		{
			haveTheme = true;
			ret.colorTheme = EStdioColorTheme::dark;
		}
		
		/*
		static constexpr char const* light[] = {
			"---" // none atm?
		};

		if( string_in_list_( term, light, sizeof(light)/sizeof(light[0]) ) )
		{
			haveTheme = true;
			ret.colorTheme = EStdioColorTheme::light;
		}
		*/
	}

	// Usable formatting
	ret.usableFmt = kDefaultFmts;

	if( term )
	{
		/* The linux console understands most of the formatting sequences, but
		 * emulates these via coloring. For now, allow only bold to be used
		 * there, and avoid light (cyan), italic (green), underline (also cyan).
		 */
		static constexpr char const* linuxConsole[] = {
			"linux", "screen.linux"
		};

		if( string_in_list_( term, linuxConsole, sizeof(linuxConsole)/sizeof(linuxConsole[0]) ) )
		{
			ret.usableFmt = EStdioUsableFmt::bold;
		}

		/* I guess rxvt tries to do italic, but I only get [][][] out of that.
		 * Not sure if that's a missing font issue, or some other problem.
		 * Avoid using italic there for now.
		 */
		static constexpr char const* rxvts[] = {
			"rxvt", "rxvt-unicode", 
		};

		if( string_in_list_( term, rxvts, sizeof(rxvts)/sizeof(rxvts[0]) ) )
		{
			ret.usableFmt = EStdioUsableFmt::bold | EStdioUsableFmt::underline;
		}

		/* Screen seems to mess up italic in almost all cases.
		 */
		static constexpr char const* screenPlus[] = {
			"screen-256color", "screen.xterm-256color"
		};
		static constexpr char const* screenMinus[] = {
			"screen"
		};

		if( string_in_list_( term, screenPlus, sizeof(screenPlus)/sizeof(screenPlus[0]) ) )
		{
			ret.usableFmt = EStdioUsableFmt::bold | EStdioUsableFmt::faint | EStdioUsableFmt::underline;
		}

		if( string_in_list_( term, screenMinus, sizeof(screenMinus)/sizeof(screenMinus[0]) ) )
		{
			ret.usableFmt = EStdioUsableFmt::bold | EStdioUsableFmt::underline;
		}
	}

	caps_update_( aStream, ret );
	return ret;
#	else // PLATFORM_WIN32
	/* Default to UTF8 support. Disable it only if we detect a terminal that
	 * for sure doesn't support it.
	 */
	ret.hasUtf8 = true;

	// Load win32 API
	auto const* w32 = dlapi::os::win32();

	if( !w32 )
	{
		caps_update_( aStream, ret );
		return ret;
	}

	// Check if it's a terminal.
	auto const handle = win32_handle_( w32, aStream );
	if( Win32_::invalid_handle_value == handle )
	{
		caps_update_( aStream, ret );
		return ret;
	}

	auto const ftype = w32->getFileType( handle );
	if( Win32_::file_type_char == ftype )
	{
		ret.isTerminal = true;

		if( Win32_::cp_utf8 != w32->getConsoleOutputCP() )
		{
			if( !aTryUpgrade || !w32->setConsoleOutputCP( Win32_::cp_utf8 ) )
				ret.hasUtf8 = false;
		}

		Win32_::DWORD mode;
		if( w32->getConsoleMode( handle, &mode ) )
		{
			if( !(mode & Win32_::enable_virtual_terminal_processing) )
			{
				mode |= Win32_::enable_virtual_terminal_processing;
				if( aTryUpgrade && w32->setConsoleMode( handle, mode ) )
				{
					ret.hasAnsi = true;
					ret.colorMode = EStdioColorMode::rgb;
				}
			}
			else
			{
				ret.hasAnsi = true;
				ret.colorMode = EStdioColorMode::rgb;
			}
		}

		/* TODO: Figure out the current colors, via
		 * GetConsoleScreenBufferInfoEx(?)
		 * 
		 * For now assume that it's a dark theme (which holds true for the
		 * standard windows consoles).
		 *
		 * Win32 console supports bold and underline, according to the Virtual
		 * Terminal documentation: https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences#text-formatting
		 */
		ret.colorTheme = EStdioColorTheme::dark;
		ret.usableFmt = kDefaultFmts;
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

	caps_update_( aStream, ret );
	return ret;
#	endif // ~ PLATFORM
}

//--    stdio_[un]lock()                    ///{{{2///////////////////////////
void stdio_lock( EStdioStream aStream ) noexcept
{
	auto const stream = stdio_stream_( aStream );

#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	::flockfile( stream );
#	else // PLATFORM_WIN32
	::_lock_file( stream );
#	endif // ~ PLATFORM
}

void stdio_unlock( EStdioStream aStream ) noexcept
{
	auto const stream = stdio_stream_( aStream );
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	::funlockfile( stream );
#	else // PLATFORM_WIN32
	::_unlock_file( stream );
#	endif // ~ PLATFORM
}

//--    stdio_clear_line()                  ///{{{2///////////////////////////
bool stdio_clear_line( EStdioStream aStream, bool aFlush ) noexcept
{
	auto const ret = stdio_probe( aStream );
	if( !ret.isTerminal )
		return false;

	auto const stream = stdio_stream_( aStream );

#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	if( !ret.hasAnsi )
		return false;

	if( aFlush )
	{
		/* Note, we need to flush both stdout and stderr. It seems that various
		 * prints will flush pending stuff internally. :-(
		 */
		std::fflush( stdout ); 
		std::fflush( stderr ); 
	}

	// The half-way line detection works by inspecting the current position in
	// the terminal. If there is buffered output remaining, that will possibly
	// mess up the method.

	auto const fd = ::fileno( stream );
	FLUX_ASSERT( -1 != fd );
	
	termios initial{};
	if( -1 != ::tcgetattr( fd, &initial ) )
	{
		termios modified = initial;
		modified.c_lflag &= ~(ECHO|ICANON);
		::tcsetattr( fd, TCSANOW, &modified );

		// read cursor pos and if it's not at the beginning, add a newline
		std::fputs( "\033[6n", stream );
		std::fflush( stream );

		int x, y;
		if( 2 == std::scanf( "\033[%d;%dR", &y, &x ) )
		{
			if( x != 1 ) std::fputc( '\n', stream );
		}
		
		::tcsetattr( fd, TCSANOW, &initial );
	}
#	else // PLATFORM_WIN32
	auto const* w32 = dlapi::os::win32();

	if( !w32 )
		return false;

	Win32_::HANDLE handle = win32_handle_( w32, aStream );
	FLUX_ASSERT( Win32_::invalid_handle_value != handle );

	if( aFlush )
	{
		std::fflush( stdout ); 
		std::fflush( stderr ); 
	}

	Win32_::CONSOLE_SCREEN_BUFFER_INFO sbi{};
	if( !w32->getConsoleScreenBufferInfo( handle, &sbi ) )
		return false;

	if( 0 != sbi.dwCursorPosition.X )
		std::fputc( '\n', stream );
#	endif // ~ PLATFORM

	return true;
}

//--    to_string()                         ///{{{2///////////////////////////
std::string to_string( EStdioStream aStream )
{
	switch( aStream )
	{
		case EStdioStream::in: return "stdin";
		case EStdioStream::out: return "stdout";
		case EStdioStream::err: return "stderr";
	}
	
	return format( "EStdioStream({:d})", int(aStream) );
}
std::string to_string( EStdioColorMode aMode )
{
	switch( aMode )
	{
		case EStdioColorMode::none: return "EStdioColorMode::none";
		case EStdioColorMode::basic: return "EStdioColorMode::basic";
		case EStdioColorMode::_8bit: return "EStdioColorMode::_8bit";
		case EStdioColorMode::rgb: return "EStdioColorMode::rgb";
	}
	
	return format( "EStdioColorMode({:d})", int(aMode) );
}
std::string to_string( EStdioColorTheme aTheme )
{
	switch( aTheme )
	{
		case EStdioColorTheme::plain: return "EStdioColorTheme::plain";
		case EStdioColorTheme::light: return "EStdioColorTheme::light";
		case EStdioColorTheme::dark: return "EStdioColorTheme::dark";
	}
	
	return format( "EStdioColorTheme({:d})", int(aTheme) );
}

std::string to_string( EStdioUsableFmt aFmt )
{
	bool first = true;
	std::ostringstream oss;

	auto const separator_ = [&] {
		if( first ) first = false;
		else oss << "|";
	};

	oss << "{ ";
	if( !!(EStdioUsableFmt::bold & aFmt) )
	{
		separator_();
		oss << "bold";
		aFmt &= ~EStdioUsableFmt::bold;
	}
	if( !!(EStdioUsableFmt::faint & aFmt) )
	{
		separator_();
		oss << "faint";
		aFmt &= ~EStdioUsableFmt::faint;
	}
	if( !!(EStdioUsableFmt::italic & aFmt) )
	{
		separator_();
		oss << "italic";
		aFmt &= ~EStdioUsableFmt::italic;
	}
	if( !!(EStdioUsableFmt::underline & aFmt) )
	{
		separator_();
		oss << "underline";
		aFmt &= ~EStdioUsableFmt::underline;
	}

	if( 0 != unsigned(aFmt) )
	{
		print( oss, "{:#x}", unsigned(aFmt) );
	}
	
	oss << " }";
	
	return oss.str();
}

//--    $ stdio_stream_()                   ///{{{2///////////////////////////
namespace
{
	FILE* stdio_stream_( EStdioStream aStream ) noexcept
	{
		switch( aStream )
		{
			case EStdioStream::in: return stdin;
			case EStdioStream::out: return stdout;
			case EStdioStream::err: return stderr;
		}

		FLUX_INTENT_UNREACHABLE();
		return nullptr;
	}
}

//--    $ win32_handle_()                   ///{{{2///////////////////////////
namespace
{
#	if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
	Win32_::HANDLE win32_handle_( dlapi::os::Win32api const* aW32, EStdioStream aStream ) noexcept
	{
		FLUX_ASSERT( aW32 );
		
		Win32_::DWORD handleId = 0;
		switch( aStream )
		{
			case EStdioStream::in: handleId = Win32_::std_input_handle; break;
			case EStdioStream::out: handleId = Win32_::std_output_handle; break;
			case EStdioStream::err: handleId = Win32_::std_error_handle; break;
		}

		return aW32->getStdHandle( handleId );
	}
#	endif // ~ PLATFORM
}

//--    $ string_in_list_()                 ///{{{2///////////////////////////
namespace
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	bool string_in_list_( char const* aNeedle, char const* const* aHaystack, std::size_t aCount )
	{
		for( std::size_t i = 0; i < aCount; ++i )
		{
			if( 0 == std::strcmp( aNeedle, aHaystack[i] ) )
				return true;
		}
		
		return false;
	}
#	endif
}

//--    $ caps_*_()                         ///{{{2///////////////////////////
namespace
{
	StdioCaps caps_load_( EStdioStream aStream ) noexcept
	{
		FLUX_ASSERT( std::size_t(aStream) < sizeof(gStdioCaps_)/sizeof(gStdioCaps_[0]) );
		
		try
		{
			compat::shared_lock<compat::shared_mutex> lock( gStdioMut_ );
			return gStdioCaps_[std::size_t(aStream)];
		}
		catch( std::exception const& )
		{
			//XXX-TODO-FLUX_LOG_PANIC()?
			FLUX_ASSERT( false );
			return StdioCaps{};
		}
	}

	void caps_update_( EStdioStream aStream, StdioCaps const& aCaps ) noexcept
	{
		FLUX_ASSERT( std::size_t(aStream) < sizeof(gStdioCaps_)/sizeof(gStdioCaps_[0]) );
		
		try
		{
			std::unique_lock<compat::shared_mutex> lock( gStdioMut_ );
			gStdioCaps_[std::size_t(aStream)] = aCaps;
		}
		catch( std::exception const& )
		{
			//XXX-TODO- FLUX_LOG_PANIC()?
			FLUX_ASSERT( false );
		}
	}
}

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
