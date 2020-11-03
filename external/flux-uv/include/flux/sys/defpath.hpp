/*-******************************************************* -- HEADER -{{{1- */
/*-	Default/well-known paths
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SYS_DEFPATH_HPP_627F1462_0D47_463F_A632_2A13841F6219
#define FLUX_SYS_DEFPATH_HPP_627F1462_0D47_463F_A632_2A13841F6219

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include "syspath.hpp"

//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    constants                   ///{{{1///////////////////////////////////
/** User specific paths
 *
 * User specific paths. Note that these are the base paths, and in many cases
 * standard practice requires creation of sub-directories for each application.
 *
 * \note No files should ever be created in `home` or `desktop` without first
 * asking the user (e.g., via an appropriate dialog). `config` is roaming on
 * Windows, so care should be taken to minimize the size of the file placed 
 * there.
 *
 * <b>Platform specific notes:</b>
 *   - Linux: uses the XDG directories where applicable. `screenshots`, 
 *     `desktop` and `downloads` default to `$HOME/Pictures`, `$HOME/Desktop`
 *     and `$HOME/Downloads`, respectively. `saves` aliases to `data`.
 *
 *   - Win32: uses `KNOWNFOLDERID`s. `data` maps to `LocalAppData`, `cache` to
 *     `LocalAppDataLow`, and `config` to `RoamingAppData`. 
 *
 * See `path_get_userdir()` for additional information.
 */
enum class EUserPath
{
	home,          //!< the user's home directory
	data,          //!< persistent application data
	cache,         //!< caches for application data
	config,        //!< configuration data (note: roaming on windows)
	saves,         //!< saved games
	screenshots,   //!< screenshots 
	desktop,       //!< desktop directory
	downloads      //!< downloads directory
};

/** Path options
 *
 * Path options passed to `path_get_userdir()`. These control whether what (if
 * any) operations/checks are performed before returning a user path.
 */
enum class EPathOptions
{
	none,         //!< don't perform any checks
	check,        //!< check that the requested path exists and is a directory
	create        //!< attempt to create the directory if it doesn't exist
	
	//TODO: readable, writable, ... (bitfield)
};

//--    functions                   ///{{{1///////////////////////////////////
/** Special path: path to self
 *
 * Returns a path to the executable that is the current process is running.
 */
SysPathStr path_get_self();

/* Special path: path to directory containing current executable
 *
 * Returns a path to the directory containing the current executable.
 */
SysPathStr path_get_selfdir();

/** Special path: current working directory
 *
 * Returns the absolute path to the current working directory. Essentially
 * equivalent to `path_make_absolute( path_from_generic( "." ) )`.
 */
SysPathStr path_get_current();

/** System temp dir
 *
 * Get the system-specific temporary directory. A non-empty path will only be
 * returned if the temporary directory is writable; if no writable temporary
 * directory can be found, `path_get_tempdir()` will return an empty path.
 */
SysPathStr path_get_tempdir();

/** User directories
 *
 * Retrieve paths to well-known user directories. See `EUserPath` for a list of
 * the directories for which paths may be requested. 
 *
 * By default, i.e., with the \a EPathOptions equal to `EPathOptions::check`,
 * `path_get_userdir()` checks that the requested directory indeed exists, and
 * returns an empty path if it doesn't. The check can be suppressed by passing
 * `EPathOptions::none`. With this option, `path_get_userdir()` will return the
 * default path where the requested directory would reside if it had existed.
 * Finally, `EPathOptions::create` asks `path_get_userdir()` to attempt to 
 * create the directories. An empty is returned if creation fails.
 *
 * Regardless of the options, empty paths may be returned if such paths are not
 * defined for the current system, or, for example, if the current user does not
 * have a home directory.
 *
 * See `EUserPath` for platform specific notes.
 *
 * \note Users can override the `EUserPath::home` directory via the environment
 * variable `FLUXENV_SYS_USER_HOME`. If defined, queries for `EUserPath::home`
 * return the specified path instead. The environment variable does not affect
 * other, more specific user paths. `FLUXENV_SYS_USER_DESKTOP` will similarly
 * override the path of `EUserPath::desktop`.
 */
SysPathStr path_get_userdir( EUserPath, EPathOptions = EPathOptions::check );

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_SYS_DEFPATH_HPP_627F1462_0D47_463F_A632_2A13841F6219
