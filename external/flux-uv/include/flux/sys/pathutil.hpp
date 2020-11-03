/*-******************************************************* -- HEADER -{{{1- */
/*-	Utilities to manipulate system paths
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SYS_PATHUTIL_HPP_300DD601_634A_4FC9_A1FF_2A95D54B8A7C
#define FLUX_SYS_PATHUTIL_HPP_300DD601_634A_4FC9_A1FF_2A95D54B8A7C

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/platform.hpp>

#include "syspath.hpp"

//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    constants                   ///{{{1///////////////////////////////////

#if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
constexpr SysPathChar kSysPathSeparator = '/';
#else // PLATFORM_WIN32
constexpr SysPathChar kSysPathSeparator = L'\\';
#endif // ~ PLATFORM

//--    functions                   ///{{{1///////////////////////////////////

/** \name Generic paths
 * \brief Convert between generic and system paths
 *
 * Convert generic paths to and from system paths. A generic path is always
 * encoded in UTF-8 and always uses a forward slash ("/") as the directory
 * separator. 
 *
 * `path_from_generic()` will convert a generic path to a system path. First,
 * the path's encoding will be changed to the system path enconding. Secondly,
 * any forward slashes are replaced with `kSysPathSeparator`. Third, any
 * `kSysPathSeparator` are replaced with a forward slash.
 *
 * `path_to_generic()` does the reverse of `path_from_generic()`.
 *
 * For example, the generic path `/a\\b//c` will be translated to
 *   - `PLATFORM_WIN32`: `\a//b\\c` (as `wchar_t`)
 *   - others: `/a\\b//c`
 *
 * Also see `path_from_utf8()` and `path_to_utf8()`, which only convert between
 * the different encodings.
 */
//@{
std::string path_to_generic( SysPathView const& );
SysPathStr path_from_generic( compat::string_view const& );

//SysPathStr path_from_generic( char const* );
//SysPathStr path_from_generic( std::string const& );
//@}

/** \name Path encoding
 * \brief Convert between UTF-8 and the system path encoding
 *
 * Convert from UTF-8 to whatever encoding the target `FLUX_PLATFORM` uses
 * natively.
 *
 * \note Currently, all supported platforms other than Windows
 * (`PLATFORM_WIN32`) use UTF-8; for these platforms, the argument is returned
 * unchanged. For Windows, the strings is converted to and `wchar_t` via the
 * Windows methods `MultiByteToWideChar()` and `WideCharToMultiByte()`.
 *
 * See `path_from_generic()` and `path_to_generic()` for methods that 
 * additionally convert between a generic path notation that uses forward
 * slashes exclusively and the system notation.
 */
//@{
std::string path_to_utf8( SysPathView const& );
SysPathStr path_from_utf8( compat::string_view const& );
//@}

//TODO: docs
//TODO: tests
std::basic_string<wchar_t> path_to_wchar( SysPathStr const& );

SysPathStr path_from_wchar( wchar_t const* );
SysPathStr path_from_wchar( std::basic_string<wchar_t> const& );


/** Join two paths
 *
 * Joins two paths lexically, i.e., by only considering the character contents
 * of the paths. The returned path is formed as follows:
 *   - if either argument is empty: return the other argument unchanged
 *   - if both arguments are empty: return empty path
 *   - join the first path with the second path; the system's preferred
 *     separator is inserted in between if neither the first path ends on a 
 *     preferred separator nor the second path starts with the preferred
 *     separator. If both the first path ends on a preferred separator and the
 *     second path starts with a preferred separator, one of the separators is
 *     omitted.
 *
 * Sequences of multiple separators are preserved otherwise.
 *
 * TODO: what about the `\\?\` prefix? Should be preserved...
 */
SysPathStr path_lexical_join( SysPathView const&, SysPathView const& );

/** Parent path
 *
 * Finds the lexical parents of the given \a SysPathStr. The optional \a int
 * argument specifies which parent should be returned: `-1` indicates the
 * immediate parent, `-2` the grand-parent, `-3` the grand-grand-parent and so
 * on.
 *
 * `path_lexical_parent()` only considers the string contents of the given
 * path, and not the actual directory structure on the system. A path is
 * considered to consist of zero or more segments separated by the system's
 * preferred directory separator ('kSysPathSeparator`). `path_lexical_parent()`
 * removes the specified number of segments from the back of the string, with
 * the following special cases:
 *   - segments consisting of a single `.` are considered self-references,
 *     and are removed but do not count against the to-be-removed segments
 *   - if a previous-directory segment (`..`) is encountered during removal,
 *     the path up to and including this segment is left unchanged
 *   - if fewer than the requested number of segments were removed, a 
 *     corresponding number of `..` segments are appended to the path.
 *
 * Examples:
 *   - `a/b` \f$\Rightarrow\f$ `a`
 *   - `.` \f$\Rightarrow\f$ `..`
 *   - `..` \f$\Rightarrow\f$ `../..`
 *   - `/` \f$\Rightarrow\f$ `/..` (and `//?/` \f$\Rightarrow\f$ `//?/..`)
 *   - `a/../` \f$\Rightarrow\f$ `a/../..`
 *
 * If \a aRestrict is set, `path_lexical_parent()` will reduce the path to at
 * most `/` but not further (so, no `/..`). If the path contained `/..` before
 * the call to `path_lexical_parent()`, these are preserved but not expanded
 * further (e.g., the parent of `/..` \f$\Rightarrow\f$ `/..`).
 *
 * `path_lexical_parent()` ignores trailing `/` in the input path, and omits
 * these in the output path. `path_lexical_parent()` treats multiple directory
 * separators following each other as a single separator.
 */
SysPathStr path_lexical_parent( SysPathView const&, int = -1, bool aRestrict = false );

/** Normalize a path
 *
 * Normalize path lexically, i.e., normalize a path by only considering the
 * elements of the path and not the actual file system.
 *
 * TODO update with regard to \a aRestrict
 * TODO update with regard to the root prefix
 *
 * Normalization performs the following transforms:
 *   - multiple consecutive path separators are folded into a single separator
 *   - no-op segments (`.`) are stripped from the path
 *   - `..`-segments remove the preceding segment if non-empty 
 *     and not equal to `..`
 *
 * The process preserves leading and trailing separators of the input.
 *
 * Examples:
 *   - `a/./b/../c` -> `a/c`
 *   - `/a/./b/../c/` -> `/a/c/`
 *   - `../../b` -> `./../a/../../b`
 *
 * If \a aRestrict is set, absolute paths are restricted from "escaping", i.e.,
 * references to the parent of the root are stripped. So, `/..` is turned into
 * `/`. This only applies to absolute paths.
 *
 * \note Win32: `path_lexical_normalize()` will always preserve a prefix of the
 * type `\\*\`, where `*` is any single character. However, the prefix will not
 * affect other parsing (that is `\\?\a\..` will still be turned into `\\?\`,
 * which is unlike Win32-API functions that do not transform `\\?\` paths at
 * all.
 */
SysPathStr path_lexical_normalize( SysPathView const&, bool aRestrict = false );

//SysPathStr path_lexical_dirpart( SysPathStr const& );
//SysPathStr path_lexical_filepart( SysPathStr const& );

bool path_lexical_is_absolute( SysPathView const& );

/** Return absolute path
 *
 * Returns absolute path to the specified file. The file must exist, or 
 * `path_make_absolute()` may fail. The result is system specific.
 *
 * TODO-fails how????
 *
 */
SysPathStr path_make_absolute( SysPathStr const& );

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_SYS_PATHUTIL_HPP_300DD601_634A_4FC9_A1FF_2A95D54B8A7C
