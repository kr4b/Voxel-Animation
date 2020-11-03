/*-******************************************************* -- HEADER -{{{1- */
/*-	Query file system.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SYS_FSQUERY_HPP_B37FADFC_7E22_4FE8_841E_95B6C5A04857
#define FLUX_SYS_FSQUERY_HPP_B37FADFC_7E22_4FE8_841E_95B6C5A04857

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/platform.hpp>

#include <flux/std/optional.hpp>
#include <flux/std/unique_ptr.hpp>

#include <flux/compat/system_error.hpp>

#include <flux/sys/syspath.hpp>
#include <flux/util/enum_bitfield.hpp>

#include <cstdint>

#include <sys/stat.h>
#include <sys/types.h>

#if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
#	include <wchar.h>
#endif // ~ PLATFORM

//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    types                       ///{{{1///////////////////////////////////

#if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
#	if FLUX_PLATCAP_CYGWIN
// Apparently convenience definitions for stat64 are going to be available
// thursday^W friday...
//  ... ah, cygwin, how I hate thee.
using StatBuf = struct stat;
#	else // !PLATCAP_CYGWIN
using StatBuf = struct stat64;
//using DirItorHandle = ::FTS*;
#	endif // ~ PLATCAP_CYGWIN

#else // PLATFORM_WIN32
using StatBuf = struct ::_stat64;
//using DirItorHandle = void*; //HANDLE, but without <windows.h>

#endif // ~ PLATFORM

#if 0
//--    DirEntry                    ///{{{1///////////////////////////////////
class DirEntry
{
	public:
		DirEntry();
		
		explicit DirEntry( SysPathStr&& );
		explicit DirEntry( SysPathStr const& );

	public:
		explicit operator bool() const;

		SysPathStr const& path() const;

	private:
		SysPathStr mPath;
};

//--    DirList                     ///{{{1///////////////////////////////////
struct DirList
{
	virtual ~DirList() = 0;
	virtual DirEntry read() = 0;
};
#endif

//--    constants                   ///{{{1///////////////////////////////////

/* File Kind
 *
 * File kind. For additional information, refer to the C++17 `std::filesystem`
 * [`file_type`](http://en.cppreference.com/w/cpp/filesystem/file_type).
 */
enum class EFileKind
{
	none,
	notFound,
	regular,
	directory,
	symlink,
	block,
	character,
	fifo,
	socket,
	unknown
};

/** Bitfield: desired access
 *
 * Desired access modes for `check_access()`. The exact meaning depends on the
 * type of the file the path refers to. Check target system documentation.
 *
 * \note WIN32 - the `execute` bit doesn't exist in WIN32 as such. It is
 * silently translated to `F_OK`, i.e., a check if the file in question exists.
 */
enum class EPathAccess : std::uint32_t
{
	read     = (1<<0),
	write    = (1<<1),
	execute  = (1<<2)
};

FLUX_UTIL_DEFINE_ENUM_BITFIELD(EPathAccess);

enum class EDirectoryOpt : std::uint32_t
{
	none                 = 0,
	logical              = (1<<0),
	listSpecial          = (1<<1),
	ignoreInaccessible   = (1<<2),
	crossDev             = (1<<3),
};

FLUX_UTIL_DEFINE_ENUM_BITFIELD(EDirectoryOpt);

/* Permissions
 *
 * For additional information, refer to the C++17 `std::filesystem`
 * [`perms`](http://en.cppreference.com/w/cpp/filesystem/perms).
 */

enum class EPermissions : std::uint32_t
{
	none = 0,

	ownerRead    =  0400,
	ownerWrite   =  0200,
	ownerExec    =  0100,
	ownerAll     =  0700,

	groupRead    =   040,
	groupWrite   =   020,
	groupExec    =   010,
	groupAll     =   070,

	othersRead   =    04,
	othersWrite  =    02,
	othersExec   =    01,
	othersAll    =    07,

	all          =  0777,

	setUid       = 04000,
	setGid       = 02000,
	stickyBit    = 01000,
	
	unknown      = 0xffff,
};

FLUX_UTIL_DEFINE_ENUM_BITFIELD(EPermissions);

//--    functions                   ///{{{1///////////////////////////////////

/** \name `stat`
 * \brief `stat` provided path
 *
 * `stat` the provided path. Returns a `StatBuf` (which aliases to the systems
 * `struct stat`-equivalent) if successful, or nothing if the specified path
 * could not be found.
 * 
 * `stat_path_link()` corresponds to `lstat()` where supported, and as such,
 * the returned `StatBuf` may indicate that the path is a symbolic link. This
 * is never the case with `stat_path()`, and additionally never the case on
 * Win32 (which doesn't have a `lstat()` method).
 */
//@{
optional<StatBuf> stat_path( SysPathView const& );
optional<StatBuf> stat_path_link( SysPathView const& );

optional<StatBuf> stat_path( SysPathView const&, compat::error_code& ) noexcept;
optional<StatBuf> stat_path_link( SysPathView const&, compat::error_code& ) noexcept;

//TODO stat_handle( StatBuf&, SysFileHandle );
//@}


/** \name Permissions
 * \brief Get file permissions
 *
 * Get file permissions from a `StatBuf` previously returned by `stat_path()`.
 * If called on an empty `optional<StatBuf>`, `stat_get_perms()` will return
 * a default-constructed `perms`.
 * 
 * \note The file permissions relate to the owner of the file and not the 
 * current user!
 */
//@{
EPermissions stat_get_perms( StatBuf const& ) noexcept;
EPermissions stat_get_perms( optional<StatBuf> const& ) noexcept;
//@}

/** \name File type
 * \brief Get file type
 *
 * Get the file type from a `StatBuf` previously returned by `stat_path()`. The
 * file type indicates if the file is a regular file, a directory or on of the
 * other `EFileKind` types.
 *
 * If called on an empty `optional<StatBuf>`, `stat_get_type()` will return 
 * `file_type::not_found`.
 */
//@{
EFileKind stat_get_type( StatBuf const& ) noexcept;
EFileKind stat_get_type( optional<StatBuf> const& ) noexcept;
//@}

/** \name File properties
 * \brief Get file properties
 *
 * Get file properties (such as size) from a `StatBuf` previously returned by
 * `stat_path()`. Calling these on empty `optional<StatBuf>`s will return 
 * reasonable default values (zero size, etc).
 */
//@{
std::uintmax_t stat_get_size( StatBuf const& ) noexcept;
std::uintmax_t stat_get_size( optional<StatBuf> const& ) noexcept;

std::uintmax_t stat_get_link_count( StatBuf const& ) noexcept;
std::uintmax_t stat_get_link_count( optional<StatBuf> const& ) noexcept;
//@}

//TODO stat_get_mtime(), stat_get_atime(), stat_get_ctime()

/** Check access to a path
 *
 * Checks if the calling process can access the specified path for reading,
 * writing or "executing". Returns `true` if all requested operations are
 * authorized, and `false` otherwise.
 *
 * The result of `check_access()` is no guarantee that the requested operation
 * will succeed in the future, as the state of the requested path may have
 * changed between the call to `check_access()` and the attempt to perform
 * whatever operation..
 *
 * \warning `check_access()` uses `::%access()` (or equivalent) and thus
 * inherits its problems. In particular, one should never `check_access()`
 * before attempting to e.g., open a file (but rather just attempt to open it
 * and handle any errors that arise there).
 */
bool check_access( SysPathView const&, EPathAccess = EPathAccess::read );
optional<bool> check_access( SysPathView const&, EPathAccess, compat::error_code& ) noexcept;

//
//TODO check_access( SysFileHandle, EPathAccess = EPathAccess::read );



bool dir_is_empty( SysPathView const& );
optional<bool> dir_is_empty( SysPathView const&, compat::error_code& ) noexcept;

#if 0
unique_ptr<DirList> dir_open( SysPathStr const&, EDirectoryOpt = EDirectoryOpt::none );
unique_ptr<DirList> dir_open_recursive( SysPathStr const&, EDirectoryOpt = EDirectoryOpt::none );
#endif

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_SYS_FSTAT_HPP_B37FADFC_7E22_4FE8_841E_95B6C5A04857
