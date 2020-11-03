/*-******************************************************* -- HEADER -{{{1- */
/*-	Manipulate file system
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SYS_FSMANIP_HPP_BFA0320B_FC07_47FC_B09C_8CE2614A2D84
#define FLUX_SYS_FSMANIP_HPP_BFA0320B_FC07_47FC_B09C_8CE2614A2D84

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/platform.hpp>

#include <flux/sys/syspath.hpp>

#include <flux/compat/system_error.hpp>

#include <cstdint>

//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    functions                   ///{{{1///////////////////////////////////

/** Create directory
 *
 * Create directory. Directories may be created recursively (\a aRecursive is
 * true), i.e., the method will attempt to create all necessary directories in
 * the path up to and including the final segment of the path. If not recursive
 * (\a aRecursive false), the path up to but excluding the final segment must
 * exist, otherwise the function will fail.
 *
 * Failure is indicated via a `error::FsError` (or a subclass thereof)
 * exception, or by returning false and setting the `compat::error_code`
 * appropriately (noexcept overload).
 *
 * \note If recursive, the function will not fail if the specified directory
 * already exists. If not recursive, the function <b>will</b> fail if the
 * specified directory exists. This emulates the CLI `mkdir` command with 
 * and without the `-p` flag, respectively.
 */
void dir_create( SysPathView const&, bool aRecursive = true );
bool dir_create( SysPathView const&, bool aRecursive, compat::error_code& ) noexcept;
/** Remove directory
 *
 * Remove directory. Directory must be empty unless \a aRecursive is set to
 * true. If \a aRecursive is set to true, `dir_remove()` will recursively
 * remove the contents of the directory and before removing the directory
 * itself.
 *
 * \note The initial path must be a directory (or a symlink to a directory). If
 * the initial path does not exist, `dir_remove()` will succeed if in recursive
 * mode, and fail if not recursive.
 *
 * `dir_remove()` does *not* follow symlinks in the directory, but rather it
 * will remove the symlink (but not the target). It will follow an initial
 * symlink (i.e., if the path passed to `dir_remove()` is a symlink to a
 * directory). `dir_remove()` does not cross to other file systems (on
 * non-WIN32 systems).
 */
void dir_remove( SysPathView const&, bool aRecursive = false );
bool dir_remove( SysPathView const&, bool aRecursive, compat::error_code& ) noexcept;


void file_create( SysPathView const&, bool aRecursive = false );
bool file_create( SysPathView const&, bool aRecursive, compat::error_code& ) noexcept;

void file_remove( SysPathView const& );
bool file_remove( SysPathView const&, compat::error_code& ) noexcept;

void file_resize( SysPathView const&, std::uintmax_t );
bool file_resize( SysPathView const&, std::uintmax_t, compat::error_code& ) noexcept;


#if 0
void symlink_create( SysPathView const&, SysPathStr const& aTarget ); //TODO


void path_unlink( SysPathView const& ); //TODO
void path_rename( SysPathView const&, SysPathStr const& ); //TODO


void path_copy_dir( SysPathView const&, SysPathView const& /*TODO: copy_options*/ ); //TODO
void path_copy_file( SysPathView const&, SysPathView const& /*TODO: copy_options*/ ); //TODO
#endif

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_SYS_FMANIP_HPP_BFA0320B_FC07_47FC_B09C_8CE2614A2D84
