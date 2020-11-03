/*-******************************************************* -- HEADER -{{{1- */
/*-	Standard input/output helpers
 *
 * The functions herein concern stdin, stdout and stderr. It is often useful
 * to know if these are directed to/from an interactive terminal or something
 * different.
 *
 * See stdio-style.hpp for helpers with styling.
 *
 * TODO: enable use of environment variables to control some of the settings
 * TODO: config file for e.g., usableFmts? I don't think we can determine that
 *       one via run-time queries.
 *
 * TODO: update caps
 *    - usableFmts: xterm supports italics
 *    - usableFmts: rxvt-unicode doesn't 
 *    - HANDLE rxvt-unicode
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SYS_STDIO_HPP_BBE944FD_7BB0_49F8_85F5_337C28CC99EC
#define FLUX_SYS_STDIO_HPP_BBE944FD_7BB0_49F8_85F5_337C28CC99EC

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/inttypes.hpp>
#include <flux/util/enum_bitfield.hpp>

#include <string>

/* Quick notes on some additional terminal/console/VTnnn tricks/functions.
 *
 * (a) Windows console actually supports these, although virtual terminal
 *     processing needs to be enabled first. `stdio_probe()` will enable it
 *     unless told not to.
 *
 *     Microsoft's documentation on the virtual terminal sequences [1] is
 *     probably one of the easier-to-access ones out there. The sequences there
 *     are probably somewhat commonly supported.
 *
 *     [1] https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences#simple-cursor-positioning
 *  
 * (b) Some seemingly useful sequences come with a few gotchas: 
 *     
 *   - Non-default scrolling margins, `ESC[<t>;<b>r`, seem to mess with
 *     scroll-back history on many terminals. Setting a top-margin (<t> != 1)
 *     tends to discard any lines that are pushed out of the viewport (e.g., on
 *     xterm, uxterm, terminology). Setting a bottom-margin (<b> != viewport
 *     height) fares a bit better (in my tests, only terminology discarded the
 *     lines).  Somewhat unsurprisingly, the Linux console pushes lines into
 *     the scroll-back history in either case. Win32 default console seems to
 *     match xterm behaviour.
 *
 *     Unfortunately (as a terminology user), this makes using this feature to
 *     provide static information by default unfeasible, since the scroll-back
 *     history is kinda important.
 *
 *   - Left-right margins seem to be unsupported by xterm? Either way, they
 *     cause scrolled-off lines to disappear under terminology as described
 *     above.
 */

//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    constants                   ///{{{1///////////////////////////////////

enum class EStdioStream : unsigned
{
	in,    // stdin; XXX-note: currently unhandled.
	out,   // stdout
	err    // stderr
};

enum class EStdioColorMode
{
	none   = 1,  // ANSI unsupported / cannot use ANSI colors
	basic,       // basic colors, i.e., \e[3Nm et al.
	_8bit,       // 8-bit colors, i.e., \e[38;5;Nm et al.
	rgb          // 24-bit colors, i.e., \e[38;2;r;g;bm et al.
};

enum class EStdioColorTheme
{
	plain,
	light,
	dark
};

enum class EStdioUsableFmt
{
	bold = (1<<0),
	faint = (1<<1),
	italic = (1<<2),
	underline = (1<<3),

	// It is indeed odd that nobody has ever considered implementing time-
	// varying formatting. For example, blinking text could be a powerful (and
	// entirely abuse-free) tool to draw a reader's attention to a specific
	// element. Maybe in the future? </blink>
};

FLUX_UTIL_DEFINE_ENUM_BITFIELD( EStdioUsableFmt );

//--    types                       ///{{{1///////////////////////////////////

struct StdioCaps
{
	/** \brief Is the selected stream connected to a terminal? 
	 */
	bool isTerminal; // TODO: update to enum -> terminal, file or pipe(!)
	/** \brief Does the selected stream support ANSI escape sequences?
	 *
	 * See https://en.wikipedia.org/wiki/ANSI_escape_code. Typically, only
	 * terminals (see `isTerminal`) support ANSI escape sequences; however, not
	 * all terminals do so (e.g., old Windows console).
	 * 
	 * TODO: FLUXENV_SYS_FOOBAR to override
	 */
	bool hasAnsi;
	/** \brief Does the selected stream support UTF-8 output?
	 *
	 * Non-terminals are assumed to support UTF-8 output. Most terminals
	 * support UTF-8 output (the old Windows console possibly being an
	 * exception). Note: the amount of UTF-8 that is supported likely depends
	 * on a number of factors, including fonts.
	 *
	 * On Win32, UTF-8 support is determined via the Win32 API. On non-Win32,
	 * the code probes support by attempting to print a multi-byte sequence,
	 * and checking how many characters the cursor positon advances. This
	 * requires ANSI escape code support.
	 *
	 * TODO: FLUXENV_SYS_FOOBAR to override
	 */
	bool hasUtf8;
	/** \brief Does the selected stream support locking?
	 *
	 * Determines whether or not `stdio_lock()` can succeed.
	 *
	 * \see stdio_lock(), stdio_unlock(), StdioLock
	 */
	bool canLock;

	/** \brief Supported color mode
	 *
	 * ANSI SGR supported color modes. Typically, only terminals will support
	 * colors at all. Under Win32, the support depends on whether or not
	 * virtual terminal processing can be enabled via the Win32 API. On
	 * non-Win32, color mode is determined somewhat heuristically, based on the
	 * value of the `$TERM` environment variable. 
	 * 
	 * TODO: FLUXENV_SYS_FOOBAR to override
	 */
	EStdioColorMode colorMode;
	/** \brief Color "theme" used by the terminal
	 *
	 * This indicates whether or not the terminal uses a light or dark color
	 * theme. On Win32, the assumption is that the default console settings
	 * are being used (i.e., a dark color theme). On non-Win32 APIs, the
	 * value is determined either via ANSI queries, via heuristics based 
	 * on the `$TERM` environment variable, or via `$COLORFGBG`.
	 * 
	 * TODO: FLUXENV_SYS_FOOBAR to override
	 */
	EStdioColorTheme colorTheme;
	/** \brief Supported ANSI SGR formatting
	 *
	 * This indicates what ANSI SGR formatting options are supported by the
	 * terminal.
	 *
	 * On Win32, only bold/bright and underline are supported according to the
	 * "Console Virtual Terminal Sequences"[1] documentation. On non-Win32,
	 * there is no automatic way of querying support. However, we assume that
	 * all options are supported, unless a specific terminal (e.g., linux
	 * console) is detected. TODO TODO TODO IMPLEMENT
	 *
	 * TODO: FLUXENV_SYS_FOOBAR to override
	 * TODO: config file?
	 */
	EStdioUsableFmt usableFmt; //TODO implement!

	//uint32_t width, height; //TODO
};

//--    classes                     ///{{{1///////////////////////////////////
/** \brief RAII-style lock for standard IO streams
 *
 * RAII-style lock for standard IO streams. See `stdio_lock()` for additional
 * information on locking standard streams.
 *
 * A `StdioLock` on an output stream (either `EStdioStream::out` or
 * `EStdioStream::err`) will by default lock both output streams. See
 * `StdioLock::StdioLock` for additional information.
 *
 * \see stdio_lock(), stdio_unlock()
 */
class StdioLock
{
	public:
		StdioLock() noexcept, ~StdioLock();

		/** \brief Acquire lock
		 *
		 * Acquire a lock on \a EStdioStream, as if by `stdio_lock()`. If
		 * \EStdioStream is an output stream (i.e., either `EStdioStream::out`
		 * or `EStdioStream::err`), and \a aLockOthers is true, both output
		 * streams are locked (the locks are always acquired in the order of
		 * `out` then `err`).
		 *
		 * \see stdio_lock()
		 */
		explicit StdioLock( EStdioStream, bool aLockOthers = true ) noexcept;

		StdioLock( StdioLock const& ) = delete;
		StdioLock& operator= (StdioLock const&) = delete;
		
		StdioLock( StdioLock&& ) noexcept;
		StdioLock& operator= (StdioLock&&) noexcept;

	private: 
		unsigned mLockState;
};

//--    functions                   ///{{{1///////////////////////////////////

/** \brief Determine capabilities of standard streams
 *
 * Determine the capabilities of \a EStdioStream. Detected capabilities are
 * returned in the `StdioCaps` struct.
 *
 * `stdio_probe()` caches its results internally, making it reasonably cheap to
 * call. The caching can be bypassed by setting \a aRefresh to true. This will
 * also update the internal caches.
 *
 * If \a EStdioStream corresponds to an interactive terminal, `stdio_probe()`
 * will by default try to configure the interactive terminal to maximize the
 * supported capabilities. This behaviour can be disabled via \a aTryUpgrade.
 *
 * \warning In particular, under Windows, `stdio_probe()` will try to enable
 * virtual terminal processing and switch the codepage to UTF8 unless disabled
 * with `aTryUpgrade`.
 *
 * \note \a aTryUpgrade will only take effect on the first call to the method
 * or when \a aRefresh is set to true. Setting \a aTryUpgrade to false will not
 * revert the terminal to its original settings if the upgrade took place
 * earlier.
 */
StdioCaps stdio_probe( 
	EStdioStream, 
	bool aTryUpgrade = true, 
	bool aRefresh = false
) noexcept;

/** \brief Lock/unlock standard IO
 *
 * Lock/unlock \a EStdioStream, if supported. Under POSIX, stdio functions are
 * thread safe, using some internal locking mechanisms. POSIX provides
 * `flockfile()` et al. to interact with those locks (`_lock_file()` under
 * Windows). 
 *
 * \note `stdio_lock()`/`stdio_unlock()` deal with consistency between threads;
 * they do not lock the underlying files (as `flock()` / `LockFile()` would
 * do). The API exposed here is aimed at reducing interspersed gibberish when
 * outputting to standard output facilities from multiple threads.
 *
 * Each call to `stdio_lock()` must be matched by a call to `stdio_unlock()` in
 * the same thread.
 */
void stdio_lock( EStdioStream ) noexcept;
/** \copydoc stdio_lock() */
void stdio_unlock( EStdioStream ) noexcept;

//TODO: store/restore cursor position
//TODO: clear line
//TODO: read cursor position?
//TODO: set cursor position?
//TODO: alternative window mode? with lockouts?

//TODO: default color handling?


/** \brief (terminal) Clear line if not already empty
 *
 * `stdio_clear_line()` determines if the corresponding \a EStdioStream is
 * connected to an interactive tty/terminal or if it's directed elsewhere.  In
 * the former case, it will try to determine if the current output position is
 * at the beginning of the line. If not, it will move the current output
 * position to the beginning of the next line. If the output is not an
 * interactive terminal, the terminal does not support the necessary
 * capabilities, or the output position is at the beginning of a new line,
 * `stdio_clear_line()` does not change the output position.
 *
 * `stdio_clear_line()` returns true if the operation was supported, and false
 * otherwise (not a terminal, or unsupported operations).
 *
 * If \a aFlush is set to true, `stdio_clear_line()` flushes *both* stdout and
 * stderr before determining output position.
 *
 * \warning `stdio_clear_line()` does not lock the output (`StdioLock` or
 * `stdio_lock()`).  Concurrent output might thus change the output position
 * immediately again. It is therefore recommended that the calling code lock
 * output for the duration of `stdio_clear_line()` and the following output
 * commands.
 *
 * `stdio_clear_line()` exists for improving output quality from e.g., logging
 * facilities when writing to stdout/stderr. Unlike logging to ordinary files,
 * the stdout/stderr facilities are shared for the whole process and many
 * different users are likely to use access this single resource. It is
 * therefore difficult to track e.g., output state internally (other users of
 * the stdout/stderr would likely bypass this tracking).
 */
bool stdio_clear_line( EStdioStream, bool aFlush = true ) noexcept;
//bool stdio_clear_window( EStdioStream ) noexcept; //TODO


std::string to_string( EStdioStream );
std::string to_string( EStdioColorMode );
std::string to_string( EStdioColorTheme );
std::string to_string( EStdioUsableFmt );

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_SYS_STDIO_HPP_BBE944FD_7BB0_49F8_85F5_337C28CC99EC
