/*-******************************************************* -- HEADER -{{{1- */
/*-	Create a default entry-point VfsRecord
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_VFS_DEFAULT_ENTRY_RECORD_HPP_7BA8EC02_4150_4FD6_BF95_7B1F844A24DC
#define FLUX_IO_VFS_DEFAULT_ENTRY_RECORD_HPP_7BA8EC02_4150_4FD6_BF95_7B1F844A24DC

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>
#include <flux/io/vfs.hpp>

#include <flux/ftl/vector.hpp>
#include <flux/std/appinfo.hpp>
#include <flux/util/enum_bitfield.hpp>

//--    >>> namespace = flux::io::vfs >>>       ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(io,vfs)
//--    constants                       ///{{{2///////////////////////////////

/** \brief Options for the default entry record
 *
 * TODO
 */
enum class EDefaultEntryOptions
{
	none = 0,

	mountAssetsDir   = (1<<0),
	mountSystemDir   = (1<<1),
};
FLUX_UTIL_DEFINE_ENUM_BITFIELD( EDefaultEntryOptions );

constexpr EDefaultEntryOptions kDefaultEntryOptions =
	  EDefaultEntryOptions::mountAssetsDir
	| EDefaultEntryOptions::mountSystemDir
;

/** \brief Search locations for assets directory
 *
 * Specify where `default_entry_add_asset_dir()` should search for potential
 * asset directories. The final `/@flux/opt/assets` is an union of all found
 * directories.
 *
 * The directories are added in the order specified below, i.e., directories
 * listed first have higher priority. Thus, a file in `EAssetDirSearch::env`
 * will override a identically-named file in `EAssetDirSearch::cwd`.
 * Additionally, new files will end up in the highest-priority directory that
 * is writable.
 */
enum class EAssetDirSearch
{
	none = 0,

	/// Look for the directory specified via the environment variable
	/// `FLUXENV_ASSET_DIR`. Ignored if not set.
	env              = (1<<0),
	/// Look for `assets` in the current working directory
	cwd              = (1<<1),

	/// Look for `assets` in the directory where the binary resides.
	selfdir          = (1<<8),
	/// Look for `assets` in the parent directory of where the binary resides.
	selfdirParent    = (1<<9),
	/// Look for `assets` in the parent directory of the last `bin` directory
	/// in the path of where the binary resides.
	selfdirBinParent = (1<<10),
	
	/// Look for `lib/<appname>` in either the directory where the binary
	/// resides, or, if this directory is named `bin`, in the parent of that
	/// directory. Ignored if no appname is specified.
	appLocalLib      = (1<<16),
	/// Look for `share/<appname>` in either the directory where the binary
	/// resides, or, if this directory is named `bin`, in the parent of that
	/// directory. Ignored if no appname is specified.
	appLocalShare    = (1<<17),
	/// Add `/@flux/app/appdata`
	appUserData      = (1<<18),
	/// Add `/@flux/app/cache`
	appUserCache     = (1<<19),


	/// Look for `base-assets.zip` in any of the directories specified above.
	/// The `base-assets.zip` archive is controlled by the user, so it may
	/// contain resources specific to the application in question.
	baseAssetsZip    = (1<<24),
	/// Look for `flux-default-assets.zip` in any of the directories specified
	/// above. The `flux-default-assets.zip` contains resources used internally
	/// by the flux libraries. It should not be modified to contain application
	/// specific items.
	fluxDefaultZip   = (1<<25),
};
FLUX_UTIL_DEFINE_ENUM_BITFIELD( EAssetDirSearch );

constexpr EAssetDirSearch kDefaultAssetDirSearch =
	/* Just all of them for now */
	  EAssetDirSearch::env
	| EAssetDirSearch::cwd
	| EAssetDirSearch::selfdir
	| EAssetDirSearch::selfdirParent
	| EAssetDirSearch::selfdirBinParent
	| EAssetDirSearch::appLocalLib
	| EAssetDirSearch::appLocalShare
	| EAssetDirSearch::appUserData
	| EAssetDirSearch::appUserCache
	| EAssetDirSearch::baseAssetsZip
	| EAssetDirSearch::fluxDefaultZip
;

/** \brief Search locations for system directory
 *
 * Specify where `default_entry_add_system_dir()` should search for potential
 * asset directories. The final `/@flux/opt/system` is an union of all found
 * directories.
 *
 * The directories are added in the order specified below, i.e., directories
 * listed first have higher priority. Thus, a file in `ESystemDirSearch::env`
 * will override a identically-named file in `ESystemDirSearch::selfdir`.
 * Additionally, new files will end up in the highest-priority directory that
 * is writable.
 *   
 */
enum class ESystemDirSearch
{
	none = 0,
	
	/// Look for the directory specified via the environment variable
	/// `FLUXENV_SYSTEM_DIR`. Ignored if not set.
	env              = (1<<0),

	/// Look for `system` in the directory where the binary resides.
	selfdir          = (1<<8),
	/// Look for `system` in the parent directory of where the binary resides.
	selfdirParent    = (1<<9),
	/// Look for `system` in the parent directory of the last `bin` directory
	/// in the path of where the binary resides.
	selfdirBinParent = (1<<10),

	/// Look for `lib/<appname>` in either the directory where the binary
	/// resides, or, if this directory is named `bin`, in the parent of that
	/// directory. Ignored if no appname is specified.
	appLocalLib      = (1<<16),
	/// Add `/@flux/app/appdata`
	appUserData      = (1<<17),
};
FLUX_UTIL_DEFINE_ENUM_BITFIELD( ESystemDirSearch );

constexpr ESystemDirSearch kDefaultSystemDirSearch =
	/* Just all of them for now */
	  ESystemDirSearch::env
	| ESystemDirSearch::selfdir
	| ESystemDirSearch::selfdirParent
	| ESystemDirSearch::selfdirBinParent
	| ESystemDirSearch::appLocalLib
	| ESystemDirSearch::appUserData
;

//--    functions                       ///{{{2///////////////////////////////

/** \brief Create default VFS entry record
 *
 * This creates a default VFS entry record. The VFS is set up as follows:
 *
 *   - relative paths map to the current working dir(*)
 *   - absolute paths map to system absolute paths according to the platform
 *     specific rules detailed in the following section
 *   - there is a special `/@flux` directory described below.
 *
 * \note (*) Currently, this is the CWD at the time `default_entry_record()` 
 * was called; it does not track changes to the CWD via system API calls.
 *
 * Absolute paths map to system paths according to the following rules:
 *   - Linux: straight mapping, so VFS `/etc` maps to system `/etc`
 *   - Windows:
 *     - Drive letters are exposed as the first level directory, such that the
 *       VFS path `/C:/Windows` map to `C:\Windows`.
 *     - TODO: other paths, \\?\, etc
 *
 * A special `/@flux` directory exists. It contains the following elements:
 *   - `/@flux/self.exe`  - a file node corresponding to the currently
 *                          executing binary
 *   - `/@flux/self/`     - a directory node mapping to the directory where
 *                          the currently running binary is located
 *   - `/@flux/temp/`     - a directory node mapping to the system's temporary
 *                          directory.
 *   - `/@flux/initial_working_dir`  -  a directory node mapping to the current
 *                                      working directory at the time of the
 *                                      call to `default_entry_record()`.
 *   - `/@flux/user`      - user-specific directories (see below)
 *   - `/@flux/app`       - application-specific directories (see below)
 *   - `/@flux/<portal>`  - special "portal" directory links (see below)
 *   - `/@flux/opt`       - optional directories (see below)
 *
 * The per-user directories at `/@flux/user` map to well-known per-user
 * directories, i.e., directories that belong to the user currently running the
 * program. (**Note:** These directories are shared across all applications, so
 * one should create a application-specific subdirectories in these locations.
 * For this, see the application specific paths, `/@flux/app/`, below). The
 * following well-known directories are defined:
 *   - `/@flux/user/home`     - the user's home directory
 *   - `/@flux/user/appdata`  - Directory where user-specific application data
 *                              may be stored. Under Windows, this is
 *                              `FOLDERID_LocalAppData`. Under Linux, it is
 *                              `~/.local/share` unless otherwise specified via
 *                              the `XDG_DATA_HOME` environment variable. This
 *                              location is suitable for storing large-ish
 *                              volumes of data.
 *   - `/@flux/user/cache`    - Directory where user-specific data may be
 *                              cached. Under Windows, this is
 *                              `FOLDERID_LocalAppDataLow`. Under Linux, it is
 *                              `~/.cache` unless otherwise specified via the
 *                              `XDG_CACHE_HOME` environment variable. This
 *                              location is suitable for large-ish volumes of
 *                              data where integrity is not an important issue
 *                              (data in this location can be recreated and
 *                              does not contain sensitive information).
 *   - `/@flux/user/config`   - Directory where user-specific configuration
 *                              settings may be stored. Under Windows, this is
 *                              `FOLDERID_RoamingAppData`. Under Linux, it is
 *                              `~/.config` unless otherwise specified via the
 *                              `XDG_CONFIG_HOME` environment variable. This
 *                              location is suitable for small volumes of
 *                              persistent data.  <b>Warning:</b> under no
 *                              circumstances should this be used for bulk
 *                              storage of any kind.
 *   - `/@flux/user/saves`    - Per-user saved games folder. Under Windows, 
 *                              this is `FOLDERID_SavedGames`. Under Linux, it
 *                              is the same directory as `appdata`). 
 *   - `/@flux/users/screenshots`   - Per-user screenshots/pictures directory.
 *                              Under Windows, this is `FOLDERID_Screenshots`.
 *                              Under Linux, it is `~/Pictures`.
 *   - `/@flux/users/desktop`  - The user's desktop directory. Under Windows,
 *                              this is `FOLDERID_Desktop`. Under Linux, it is
 *                              `~/Desktop`. Under normal circumstances, the
 *                              desktop should be avoided, as users will get
 *                              unhappy if their desktop gets cluttered
 *                              unnecessarily.
 *   - `/@flux/users/downloads`  - The user's downloads directory. Under Windows,
 *                              this is `FOLDERID_Downloads`. Under Linux, it is
 *                              `~/Downloads`.
 *
 * The user-specific directories are created on demand. Attempting to open such
 * a directory (`vfs_open()`) will return a valid record even if the physical
 * directory does not exist. Attempting to create a new entry (file/directory)
 * inside one these directories will create it on the fly.
 *
 * Typically, items should not be placed directly in these directories. Rather,
 * a sub-directory belonging to the current application should be created and
 * files placed in that (/@flux/user/downloads` and `/@flux/user/desktop` are
 * the exceptions, as it is unusual for applications to create subdirectories
 * here). 
 *
 * The `/@flux/app/<foo>` paths refer to sub-directories in the same-named
 * directories in `/@flux/users/`, named based on the current application
 * identifier (see `flux::appinfo_ident()`). For examplle, the path
 * `/@flux/app/appdata` is equivalent to `/$flux/user/appdata/${ident}`, where
 * `${ident}' is replaced by the string returned by `flux::appinfo_ident()`.
 * The folders are also created laziliy.
 *   - `/@flux/app/appdata`   - Directory where application-specific data
 *                              may be stored. This is an `appinfo_ident()`-
 *                              named subdirectory to `/@flux/user/appdata`.
 *                              This location is suitable for storing large-ish
 *                              volumes of data.
 *   - `/@flux/app/cache`     - Directory where app-specific data may be
 *                              cached. This is an `appinfo_ident()`-named
 *                              subdirectory to `/@flux/user/cache`. This
 *                              location is suitable for storing large-ish
 *                              volumes of data where integrity is not an
 *                              important issue (data in this location can be
 *                              recreated and does not contain sensitive
 *                              information).
 *   - `/@flux/app/config`    - Directory where app-specific configuration
 *                              settings may be stored. This is an
 *                              `appinfo_ident()`-named subdirectory to
 *                              `/@flux/user/config`. This location is suitable
 *                              for small volumes of persistent data.
 *                              <b>Warning:</b> under no circumstances should
 *                              this be used for bulk storage of any kind.
 *   - `/@flux/app/saves`     - App-specific saved games folder. This is an
 *                              `appinfo_ident()`-named subdirectory to
 *                              `/@flux/user/saves`.
 *   - `/@flux/app/screenshots`  - App-specifc screenshots/pictures directory.
 *                              This is an `appinfo_ident()`-named subdirectory
 *                              to `/@flux/user/screenshots`.
 *
 * The special `/@flux/<portal>` paths can be used to get an reference to a
 * well-known directory as it were accessed via an absolute path. For example,
 * both `/@flux/self` and `/@flux/<portal>/self` correspond to the directory
 * that contains the currently executing binary. The following example
 * highlights the differences:
   \code
   auto const root = default_entry_record();
   auto const selfNormal = vfs_open( root, "/@flux/self" );
   auto const normalParent = vfs_open( selfNormal, ".." );
  
   auto const selfPortal = vfs_open( root, "/@flux/<portal>/self" );
   auto const portalParent = vfs_open( selfPortal, ".." );
   \endcode
 * In the above example `normalParent` corresponds to the `/@flux` virtual
 * directory. `portalParent` instead corresponds to the system directory that
 * is the physical parent directory of the "self directory".
 *
 * \warning Directly accessing `/@flux/<portal>/self/..` returns the `<portal>`
 * special directory. This may or may not change in the future.
 *
 * The `/@flux/<portal>/` special directory contains the following entries:
 *   - `/@flux/<portal>/self`   - Directory containing the currently running
 *                                binary. See `/@flux/self`.
 *   - `/@flux/<portal>/temp`   - The system's temporary files directory. See
 *                                `/@flux/temp`.
 *   - `/@flux/<portal>/initial_working_dir`  - The current working directory
 *                                at the time when `default_entry_record()` was
 *                                executed. See `/@flux/initial_working_dir`.
 *
 * Special directories can be added to `/@flux/opt`:
 *   - `/@flux/opt/assets`   - Union of several directories where run-time 
 *                             assets may be found. The directory is "mounted"
 *                             using the `default_entry_add_asset_dir()` method
 *                             or automatically if specified via \a
 *                             EDefaultEntryOptions. See the aforementioned
 *                             method for additional information.
 *   - `/@flux/opt/system`   - Union of several directories where run-time
 *                             binaries and libraries may be found. Unlike
 *                             `assets`, these are "normal" system directories,
 *                             from which executables can be launched and
 *                             shared libraries be loaded. The directory may be
 *                             "mounted" using `default_entry_add_system_dir()`
 *                             or automatically if specified via \a
 *                             EDefaultEntryOptions. See the aforementioned
 *                             method for additional information.
 */
VfsRecord default_entry_record( EDefaultEntryOptions = kDefaultEntryOptions );


/** \brief "Mount" `/@flux/opt/assets`
 *
 * Search for matching `assets` directories, and create `/@flux/opt/assets`,
 * which is an union of all found directories.
 *
 * \a EAssetDirSearch controls what default directories are searched. See its
 * documentation for information on the different locations. The directories
 * are added in the order they are listed in the enum, with the highest
 * priority first.
 *
 * Additional directories can be specified via the \a aRequired and \a
 * aRequested parameters. The directories should specified via their VFS paths.
 * Inaccessible items in \a aRequired cause `default_entry_add_asset_dir()` to
 * throw an appropriate exception. If an item in \a aRequested cannot be found,
 * it will be ignored. Items in \a aRequired have priority over \a aRequested,
 * which have priority over the default locations determined by \a
 * EAssetDirSearch.
 */
void default_entry_add_asset_dir(
	VfsRecord aDefaultEntry,
	char const* aAppName = appinfo_ident(),
	EAssetDirSearch = kDefaultAssetDirSearch,
	ftl::Vector<std::string> aRequired = {},
	ftl::Vector<std::string> aRequested = {}
);
/** \brief "Mount" `/@flux/opt/systems`
 *
 * Search for matching `system` directories, and create `/@flux/opt/system`,
 * which is an union of all found directories.
 *
 * \a ESystemDirSearch controls what default directories are searched. See its
 * documentation for information on the different locations. The directories
 * are added in the order they are listed in the enum, with the highest
 * priority first.
 *
 * Additional directories can be specified via the \a aRequired and \a
 * aRequested parameters. The directories should specified via their VFS paths.
 * Inaccessible items in \a aRequired cause `default_entry_add_system_dir()` to
 * throw an appropriate exception. If an item in \a aRequested cannot be found,
 * it will be ignored. Items in \a aRequired have priority over \a aRequested,
 * which have priority over the default locations determined by \a
 * ESystemDirSearch.
 *
 * \warning Executable binaries and shared libraries (DLLs or SOs) are required
 * to have system-native paths. Thus, adding certain special directories, such
 * as .zip archives or similar, whose entries do not have system-native paths
 * is discouraged.
 */
void default_entry_add_system_dir(
	VfsRecord aDefaultEntry,
	char const* aAppName = appinfo_ident(),
	ESystemDirSearch = kDefaultSystemDirSearch,
	ftl::Vector<std::string> aRequired = {},
	ftl::Vector<std::string> aRequested = {}
);

//--    <<< ~ flux::io::vfs namespace <<<       ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(io,vfs)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_VFS_DEFAULT_ENTRY_RECORD_HPP_7BA8EC02_4150_4FD6_BF95_7B1F844A24DC
