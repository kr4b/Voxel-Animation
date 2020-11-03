/*-******************************************************* -- HEADER -{{{1- */
/*-	flux::format()
 *
 * printf()-like formatting into strings and C++ streams
 *
 * XXX-fixme: replace with the default non-printf() like formatting of 
 * format-lib. fmt::printf() results in quite massive bloat. Compare:
 * https://godbolt.org/z/0MaRyo
 * (Google's bloaty actually pointer at fmt::printf() as well.)
 *
 * That's a rather major operation... 
 *    flux::format() -> flux::format() (both overloads)
 *    flux::printf() -> flux::print()
 *    printf_args + make_printf_args() are probably unused
 *
 *  Expose fmt::memory_buffer and format_to().
 *
 * Update flux_ext/fmtlib.hpp to not export the printf() like syntax.
 *
 * What about FMT_STRING() for compile-time format string checking?
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_FORMAT_HPP_1E0EA493_E54A_4339_AF96_AE2D6C74E45B
#define FLUX_STD_FORMAT_HPP_1E0EA493_E54A_4339_AF96_AE2D6C74E45B

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/namespace.hpp>
#include <flux_ext/fmtlib.hpp>

//--    >>> namespace = flux >>>                    ///{{{1///////////////////
FLUX_NAMESPACE_ENTER()
//--    format()                            ///{{{2///////////////////////////

//TODO
//TODO
//TODO DOCS! DOCS! DOCS!
//TODO
//TODO
//TODO

#define FLUX_FMT_STRING FMT_STRING

using flux_ext::fmt::print;
using flux_ext::fmt::format;

using flux_ext::fmt::arg;

using flux_ext::fmt::make_format_args;
using flux_ext::fmt::vprint;
using flux_ext::fmt::vformat;

//TODO: maybe expose format_to()?
//TODO: probably do NOT expose memory_buffer (too generic of a name for top
//TODO: level namespace!

#if 0
/** `printf()`-like formatting to a `std::string`
 *
 * `flux::format()` formats strings similar to `printf()`, but returns the
 * result in a `std::string`.
 *
 * Example:
   \code
   std::string s = flux::format( "%zu", sizeof(void*) );
   \endcode
 *
 * \note `flux::format()` corresponds to `%fmt::sprintf()`. See <a
 * href="https://github.com/fmtlib/fmt">fmtlib</a> for more information. Flux
 * only exposes the printf-like API.
 */
template< typename... tArgs > inline
std::string format( char const* aFmt, tArgs&&... aArgs )
{
	return flux_ext::fmt::sprintf( aFmt, std::forward<tArgs>(aArgs)... );
}

/** `printf()`-like formatting to a `std::ostream`
 *
 * `flux::format()` formats strings similar to `printf()`, but outputs the
 * results into a C++ `std::ostream`.
 *
 * Example:
   \code
   flux::format( std::cerr, "%zu", sizeof(void*) );
   \endcode
 *
 * \note `flux::format()` corresponds to `%fmt::fprintf()`. See <a
 * href="https://github.com/fmtlib/fmt">fmtlib</a> for more information. Flux
 * only exposes the printf-like API.
 */
template< typename... tArgs > inline
void format( std::ostream& aOs, char const* aFmt, tArgs&&... aArgs )
{
	flux_ext::fmt::fprintf( aOs, aFmt, std::forward<tArgs>(aArgs)... );
}

// New since formatlib:
using flux_ext::fmt::printf;
using flux_ext::fmt::vprintf;

using flux_ext::fmt::printf_args;
using flux_ext::fmt::make_printf_args;

inline
std::string vformat( char const* aFmt, printf_args aArgs )
{
	return flux_ext::fmt::vsprintf( aFmt, aArgs );
}
inline
void vformat( std::ostream& aOs, char const* aFmt, printf_args aArgs )
{
	flux_ext::fmt::vfprintf( aOs, aFmt, aArgs );
}

#if defined(__DOXYGEN__)
/** C++ `printf()` replacement
 *
 * `flux::printf()` formats strings similar to `printf()`, but accepts e.g.,
 * `std::string()` and similar as its arguments.
 *
 * Example:
   \code
   flux::printf( "%zu: %s\n", sizeof(void*), std::string("bytes in a pointer") );
   \endcode
 *
 * \note `flux::printf()` corresponds to `%fmt::printf()`. See <a
 * href="https://github.com/fmtlib/fmt">fmtlib</a> for more information. Flux
 * only exposes the printf-like API.
 */
template< typename... tArgs >
void printf( char const* fmt, tArgs&&... aArgs );
#endif // ~ defined(__DOXYGEN__)
#endif

//--    <<< ~ flux namespace <<<                    ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE()
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_STD_FORMAT_HPP_1E0EA493_E54A_4339_AF96_AE2D6C74E45B
