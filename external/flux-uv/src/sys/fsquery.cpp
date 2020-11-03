/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Filesystem queries
 */
/*-***************************************************************** -}}}1- */

#include <flux/sys/fsquery.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/except/einfo.hpp>

#include <flux/sys/errors.hpp>
#include <flux/sys/pathutil.hpp>
#include <flux/sys/errno_category.hpp>
#include <flux/sys/win32_category.hpp>

#include <flux/util/intent.hpp>
#include <flux/util/narrow.hpp>
#include <flux/util/switch.hpp>
#include <flux/util/scope_exit.hpp>

#include <flux/pp/join.hpp>
#include <flux/pp/stringize.hpp>

#include <flux/compat/system_error.hpp>

#if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
#	include <fts.h>
#	include <dirent.h>
#	include <unistd.h>
#else // PLATFORM_WIN32
#	include <flux/sys/win32/windows.h>
#	include <io.h>
#endif // ~ PLATFORM

#include "pathbuf.hxx"

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
#		if FLUX_PLATCAP_CYGWIN
			// More hate for Cygwin
#			define FLUXTMP_STAT_ ::stat
#			define FLUXTMP_LSTAT_ ::lstat
#		else // !CYGWIN
#			define FLUXTMP_STAT_ ::stat64
#			define FLUXTMP_LSTAT_ ::lstat64
#		endif // ~ CYGWIN

#		define FLUXTMP_IS_(x,b) (FLUX_PP_JOIN(S_IS,x)(b))

#		define FLUXTMP_ACCESS_ ::access
#		define FLUXTMP_X_OK_ X_OK
#		define FLUXTMP_W_OK_ W_OK
#		define FLUXTMP_R_OK_ R_OK

#	if 0
	char const* fts_err_str_( int ) noexcept;

	class DirListFTS_ : public DirList
	{
		public:
			DirListFTS_( FTS*, EDirectoryOpt, bool );
			~DirListFTS_();

		public:
			DirEntry read() override;

		private:
			FTS* mHandle;
			EDirectoryOpt mOptions;
			bool mRecursive;
	};
#	endif

#	else // PLATFORM_WIN32
#		define FLUXTMP_STAT_ ::_wstat64
#		define FLUXTMP_LSTAT_ ::_wstat64
#		define FLUXTMP_IS_(x,b) (FLUX_PP_JOIN(_S_IF,x) & (b))
#		define _S_IFFIFO _S_IFIFO /*HACK: it's S_ISFIFO, but _S_IFIFO :-(*/

#		define FLUXTMP_ACCESS_ ::_waccess
#		define FLUXTMP_X_OK_ 0x0 // 0x0 => check existence.
#		define FLUXTMP_W_OK_ 0x2
#		define FLUXTMP_R_OK_ 0x4

#	if 0
	class DirListWin32_ : public DirList
	{
	};
#	endif

#	endif // ~ PLATFORM
}

//--    stat_path()                         ///{{{2///////////////////////////
optional<StatBuf> stat_path( SysPathView const& aPath )
{
	if( aPath.empty() )
		return {};

	detail::SysPathBuf path;
	detail::make_zpath( path, aPath, true, 0 );

	StatBuf sb;
	if( -1 == FLUXTMP_STAT_( path.data(), &sb ) )
	{
		int errno_ = errno;
		switch( errno_ )
		{
			case ENOENT: FLUX_SWITCH_FALLTHROUGH();
			case ENOTDIR:
				return {};

			case EACCES:
				FLUX_THROW( error::FsPermissionDenied )
					<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
					<< einfo::ApiFunction( FLUX_PP_STRINGIZE(FLUXTMP_STAT_) )
				;

			default:
				FLUX_THROW( error::FsError )
					<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
					<< einfo::ApiError( make_errno_error_code( errno_ ) )
					<< einfo::ApiFunction( FLUX_PP_STRINGIZE(FLUXTMP_STAT_) )
				;
		}

		FLUX_INTENT_UNREACHABLE();
	}
	
	return sb;
}

optional<StatBuf> stat_path( SysPathView const& aPath, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	
	if( aPath.empty() )
	{
		aErrorOut = compat::make_error_code( compat::errc::invalid_argument );
		return {};
	}

	detail::SysPathBuf path;
	if( !detail::make_zpath( path, aPath, true, 0, aErrorOut ) )
		return {};

	StatBuf sb;
	if( -1 == FLUXTMP_STAT_( path.data(), &sb ) )
	{
		int errno_ = errno;
		switch( errno_ )
		{
			case ENOENT: FLUX_SWITCH_FALLTHROUGH();
			case ENOTDIR:
				aErrorOut = compat::error_code();
				return {};

			case EACCES:
				aErrorOut = compat::make_error_code( compat::errc::permission_denied );
				return {};

			default:
				aErrorOut = make_errno_error_code( errno_ );
				return {};
		}

		FLUX_INTENT_UNREACHABLE();
	}
	
	return sb;
}

//--    stat_path_link()                    ///{{{2///////////////////////////
optional<StatBuf> stat_path_link( SysPathView const& aPath )
{
	if( aPath.empty() )
		return {};

	/* Windows doesn't have a lstat() as far as I can see, and doesn't seem to
	 * define _S_IFLNK either. Windows will currently never return information
	 * about symlinks.
	 *
	 * Note: We could manually check with `GetFileAttributesW()` and look for
	 * `FILE_ATTRIBUTE_REPARSE_POINT()`, but we'd need to figure out a way to
	 * return this in the StatBuf.
	 */

	detail::SysPathBuf path;
	detail::make_zpath( path, aPath, true, 0 );

	StatBuf sb;
	if( -1 == FLUXTMP_LSTAT_( path.data(), &sb ) )
	{
		int errno_ = errno;
		switch( errno_ )
		{
			case ENOENT: FLUX_SWITCH_FALLTHROUGH();
			case ENOTDIR:
				return {};

			case EACCES:
				FLUX_THROW( error::FsPermissionDenied )
					<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
					<< einfo::ApiFunction( FLUX_PP_STRINGIZE(FLUXTMP_STAT_) )
				;

			default:
				FLUX_THROW( error::FsError )
					<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
					<< einfo::ApiError( make_errno_error_code( errno_ ) )
					<< einfo::ApiFunction( FLUX_PP_STRINGIZE(FLUXTMP_STAT_) )
				;
		}

		FLUX_INTENT_UNREACHABLE();
	}
	
	return sb;
}
optional<StatBuf> stat_path_link( SysPathView const& aPath, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );

	if( aPath.empty() )
		return {};

	detail::SysPathBuf path;
	if( !detail::make_zpath( path, aPath, true, 0, aErrorOut ) )
		return {};

	// See stat_path_link() overload above regarding Windows fun.

	StatBuf sb;
	if( -1 == FLUXTMP_LSTAT_( path.data(), &sb ) )
	{
		int errno_ = errno;
		switch( errno_ )
		{
			case ENOENT: FLUX_SWITCH_FALLTHROUGH();
			case ENOTDIR:
				aErrorOut = compat::error_code();
				return {};

			case EACCES:
				aErrorOut = compat::make_error_code( compat::errc::permission_denied );
				return {};

			default:
				aErrorOut = make_errno_error_code( errno_ );
				return {};
		}

		FLUX_INTENT_UNREACHABLE();
	}
	
	return sb;
}

//--    stat_get_perms()                    ///{{{2///////////////////////////
EPermissions stat_get_perms( StatBuf const& aStat ) noexcept
{
	return static_cast<EPermissions>(aStat.st_mode & 07777);
}
EPermissions stat_get_perms( optional<StatBuf> const& aStat ) noexcept
{
	return aStat ? stat_get_perms( aStat.value() ) : EPermissions{};
}

//--    stat_get_type()                     ///{{{2///////////////////////////
EFileKind stat_get_type( StatBuf const& aStat ) noexcept
{
	if( FLUXTMP_IS_(DIR, aStat.st_mode) )
		return EFileKind::directory;
	else if( FLUXTMP_IS_(REG, aStat.st_mode) )
		return EFileKind::regular;
	else if( FLUXTMP_IS_(CHR, aStat.st_mode) )
		return EFileKind::character;
	else if( FLUXTMP_IS_(FIFO, aStat.st_mode) )
		return EFileKind::fifo;
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	else if( FLUXTMP_IS_(SOCK, aStat.st_mode) )
		return EFileKind::socket;
	else if( FLUXTMP_IS_(LNK, aStat.st_mode) )
		return EFileKind::symlink;
	else if( FLUXTMP_IS_(BLK, aStat.st_mode) )
		return EFileKind::block;
#	endif // ~ PLATFORM

	return EFileKind::unknown;
}
EFileKind stat_get_type( optional<StatBuf> const& aStat ) noexcept
{
	return aStat ? stat_get_type( aStat.value() ) : EFileKind::notFound;
}

//--    stat_get_size()                     ///{{{2///////////////////////////
std::uintmax_t stat_get_size( StatBuf const& aStat ) noexcept
{
	return util::narrow<std::uintmax_t>( aStat.st_size );
}
std::uintmax_t stat_get_size( optional<StatBuf> const& aStat ) noexcept
{
	return aStat ? stat_get_size( aStat.value() ) : 0;
}

//--    stat_get_link_count()               ///{{{2///////////////////////////
std::uintmax_t stat_get_link_count( StatBuf const& aStat ) noexcept
{
	return util::narrow<std::uintmax_t>( aStat.st_nlink );
}
std::uintmax_t stat_get_link_count( optional<StatBuf> const& aStat ) noexcept
{
	return aStat ? stat_get_link_count( aStat.value() ) : 0;
}


//--    check_access()                      ///{{{2///////////////////////////
bool check_access( SysPathView const& aPath, EPathAccess aAccess )
{
	detail::SysPathBuf path;
	detail::make_zpath( path, aPath, true, 0 );

	int mode = 0;
	if( !!(EPathAccess::read & aAccess) ) mode |= FLUXTMP_R_OK_;
	if( !!(EPathAccess::write & aAccess) ) mode |= FLUXTMP_W_OK_;
	if( !!(EPathAccess::execute & aAccess) ) mode |= FLUXTMP_X_OK_;

	return -1 != FLUXTMP_ACCESS_( path.data(), mode );
}
optional<bool> check_access( SysPathView const& aPath, EPathAccess aAccess, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );

	detail::SysPathBuf path;
	if( !detail::make_zpath( path, aPath, true, 0, aErrorOut ) )
		return {};

	int mode = 0;
	if( !!(EPathAccess::read & aAccess) ) mode |= FLUXTMP_R_OK_;
	if( !!(EPathAccess::write & aAccess) ) mode |= FLUXTMP_W_OK_;
	if( !!(EPathAccess::execute & aAccess) ) mode |= FLUXTMP_X_OK_;

	aErrorOut = compat::error_code();
	return -1 != FLUXTMP_ACCESS_( path.data(), mode );
}

//--    dir_is_empty()                      ///{{{2///////////////////////////
bool dir_is_empty( SysPathView const& aPath )
{
	using FT_ = EFileKind;
	
	auto const ty = sys::stat_get_type( sys::stat_path( aPath ) );
	FLUX_THROW_IF( FT_::directory != ty, error::FsPermissionDenied )
		<< einfo::ErrorMeta( FLUX_FMT_STRING("not a directory '{}'"), path_to_utf8(aPath) )
	;

	detail::SysPathBuf path;
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	detail::make_zpath( path, aPath, false, 0 );

	DIR* dir = ::opendir( path.data() );
	if( !dir )
	{
		int errno_ = errno;
		FLUX_THROW_IF( EACCES == errno_, error::FsPermissionDenied )
			<< einfo::ApiFunction( "::opendir" )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
		;

		FLUX_THROW( error::FsError )
			<< einfo::ApiError( make_errno_error_code( errno_ ) )
			<< einfo::ApiFunction( "::opendir" )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
		;
	}

	FLUX_UTIL_ON_SCOPE_EXIT
	{
		::closedir( dir );
	};

	errno = 0;
	if( !::readdir( dir ) )
	{
		int errno_ = errno;
		FLUX_THROW_IF( 0 != errno_, error::FsError )
			<< einfo::ApiError( make_errno_error_code( errno_ ) )
			<< einfo::ApiFunction( "::readdir" )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
		;

		return true;
	}

	return false;

#	else // PLATFORM_WIN32
	// FindFirstFileW() requires that the path end on "\*". Append that.
	detail::make_zpath( path, aPath, false, 2 );

	auto end = path.size();
	if( L'\\' != path[end-1] )
		path[end-1] = L'\\';

	path.emplace_back( L'*' );
	path.emplace_back( L'\0' );

	WIN32_FIND_DATAW wfd;
	HANDLE h = ::FindFirstFileW( path.data(), &wfd );

	FLUX_THROW_IF( INVALID_HANDLE_VALUE == h, error::FsError )
		<< einfo::ApiError( make_win32_error_code() )
		<< einfo::ApiFunction( "::FindFirstFileW" )
		<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
	;

	FLUX_UTIL_ON_SCOPE_EXIT
	{
		::FindClose( h );
	};

	do
	{
		if( !(L'.' == wfd.cFileName[0] && (L'\0' == wfd.cFileName[1] || (L'.' == wfd.cFileName[1] && L'\0' == wfd.cFileName[2]) )) )
		{
			return false;
		}
	}
	while( ::FindNextFile( h, &wfd ) );

	DWORD err = ::GetLastError();
	FLUX_THROW_IF( ERROR_NO_MORE_FILES != err, error::FsError )
		<< einfo::ApiError( make_win32_error_code( err ) )
		<< einfo::ApiFunction( "::FindNextFile" )
	;

	return true;
#	endif // ~ PLATFORM
}

optional<bool> dir_is_empty( SysPathView const& aPath, compat::error_code& aErrorOut ) noexcept
{
	using FT_ = EFileKind;
	FLUX_ASSERT( !aErrorOut );

	auto const sb = stat_path( aPath, aErrorOut );
	if( !sb && aErrorOut )
		return {};
	
	auto const ty = sys::stat_get_type( sb );
	if( FT_::directory != ty )
	{
		aErrorOut = compat::make_error_code( compat::errc::not_a_directory );
		return {};
	}
	
	detail::SysPathBuf path;
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	if( !detail::make_zpath( path, aPath, false, 0, aErrorOut ) )
		return {};

	DIR* dir = ::opendir( path.data() );
	if( !dir )
	{
		int errno_ = errno;
		switch( errno_ )
		{
			case EACCES:
				aErrorOut = compat::make_error_code( compat::errc::permission_denied );
				return {};
			
			default:
				aErrorOut = make_errno_error_code( errno_ );
				return {};
		}
	}

	FLUX_UTIL_ON_SCOPE_EXIT
	{
		::closedir( dir );
	};

	errno = 0;
	if( !::readdir( dir ) )
	{
		int errno_ = errno;
		if( 0 != errno_ )
		{
			aErrorOut = make_errno_error_code( errno_ );
			return {};
		}

		return true;
	}

	return false;

#	else // PLATFORM_WIN32
	// FindFirstFileW() requires that the path end on "\*". Append that.
	if( !detail::make_zpath( path, aPath, false, 2, aErrorOut ) )
		return {};

	auto end = path.size();
	if( L'\\' != path[end-1] )
		path[end-1] = L'\\';

	path.emplace_back( L'*' );
	path.emplace_back( L'\0' );

	WIN32_FIND_DATAW wfd;
	HANDLE h = ::FindFirstFileW( path.data(), &wfd );

	if( INVALID_HANDLE_VALUE == h )
	{
		aErrorOut = make_win32_error_code();
		return {};
	}

	FLUX_UTIL_ON_SCOPE_EXIT
	{
		::FindClose( h );
	};

	do
	{
		if( !(L'.' == wfd.cFileName[0] && (L'\0' == wfd.cFileName[1] || (L'.' == wfd.cFileName[1] && L'\0' == wfd.cFileName[2]) )) )
		{
			return false;
		}
	}
	while( ::FindNextFile( h, &wfd ) );

	DWORD err = ::GetLastError();
	if( ERROR_NO_MORE_FILES != err )
	{
		aErrorOut = make_win32_error_code( err );
		return {};
	}

	return true;
#	endif // ~ PLATFORM
}


#if 0
//--    dir_open()                          ///{{{2///////////////////////////
unique_ptr<DirList> dir_open( SysPathStr const& aPath, EDirectoryOpt aOptions )
{
	//TODO use detail::SysPathBuf and detail::make_zpath()!
	
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	char* dirlist[] = { const_cast<char*>(aPath.c_str()), nullptr };

	int options = FTS_NOCHDIR|FTS_COMFOLLOW;
	if( !!(EDirectoryOpt::logical & aOptions) )
		options |= FTS_LOGICAL;
	else
		options |= FTS_PHYSICAL;

	if( !!(EDirectoryOpt::listSpecial & aOptions) )
		options |= FTS_SEEDOT;

	if( !(EDirectoryOpt::crossDev & aOptions) )
		options |= FTS_XDEV;
	
	FTS* fts = fts_open( dirlist, options, nullptr );
	FLUX_THROW_IF( !fts, error::FsError ) //TODO: EACCES special handling?
		<< einfo::ApiError( make_errno_error_code() )
		<< einfo::ApiFunction( "::fts_open" )
		<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
	;

	FTSENT* entry = fts_read( fts );
	FLUX_THROW_IF( !entry, error::FsError )
		<< einfo::ApiFunction( "::fts_read" )
		<< einfo::ErrorDesc( "returned null" )
	;

	printf( "entry: %s %d\n", fts_err_str_(entry->fts_info), entry->fts_info );
	
	if( !entry || entry->fts_info != FTS_D || entry->fts_path != aPath )
	{
		FLUX_THROW( error::FsPathUnexpected )
			<< einfo::ApiFunction( "::fts_read" )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("path '{}'"), path_to_utf8(aPath) )
			<< einfo::ErrorDesc( "unexpected first entry in FTS" )
		;
	}

	return make_unique<DirListFTS_>( fts, aOptions, false );

#	else // PLATFORM_WIN32
	
	return unique_ptr<DirList>();
#	endif // ~ PLATFORM
}
#endif

#if 0
//--    DirList                             ///{{{2///////////////////////////
DirList::~DirList()
{}

//--    DirEntry                            ///{{{2///////////////////////////
DirEntry::DirEntry() = default;

DirEntry::DirEntry( SysPathStr&& aPath )
	: mPath( std::move(aPath) )
{}
DirEntry::DirEntry( SysPathStr const& aPath )
	: mPath( aPath )
{}

DirEntry::operator bool() const
{
	return !mPath.empty();
}

SysPathStr const& DirEntry::path() const
{
	return mPath;
}

//--    $ DirListFTS_                       ///{{{2///////////////////////////
namespace
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	DirListFTS_::DirListFTS_( FTS* aHandle, EDirectoryOpt aOptions, bool aRecursive )
		: mHandle( aHandle )
		, mOptions( aOptions )
		, mRecursive( aRecursive )
	{}

	DirListFTS_::~DirListFTS_()
	{
		::fts_close( mHandle );
	}
	
	DirEntry DirListFTS_::read()
	{
		while( FTSENT* entry = ::fts_read( mHandle ) )
		{
			switch( entry->fts_info )
			{
				case FTS_NS:  FLUX_SWITCH_FALLTHROUGH();
				case FTS_ERR: FLUX_SWITCH_FALLTHROUGH();
				case FTS_DNR:
					if( EACCES == entry->fts_errno  )
					{
						bool ignore = !!(EDirectoryOpt::ignoreInaccessible & mOptions);
						FLUX_THROW_IF( !ignore, error::FsPermissionDenied )
							<< einfo::ApiFunction( "::fts_read" )
							<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), entry->fts_path )
						;
					}
					else
					{
						FLUX_THROW( error::FsError )
							<< einfo::ApiFunction( "::fts_read" )
							<< einfo::ApiError( make_errno_error_code( entry->fts_errno ) )
							<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), entry->fts_path )
						;
					}
					break;

				case FTS_D:
					if( !mRecursive )
					{
						auto err = fts_set( mHandle, entry, FTS_SKIP );
						FLUX_THROW_IF( -1 == err, error::FsError )
							<< einfo::ApiFunction( "::fts_set" )
							<< einfo::ApiError( make_errno_error_code() )
							<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), entry->fts_path )
						;
					}
					return DirEntry( entry->fts_path /*TODO*/ );

				case FTS_DP:
					break;

				case FTS_F: FLUX_SWITCH_FALLTHROUGH();
				case FTS_SL: FLUX_SWITCH_FALLTHROUGH();
				case FTS_DOT: FLUX_SWITCH_FALLTHROUGH();
				case FTS_SLNONE: FLUX_SWITCH_FALLTHROUGH();
				case FTS_DEFAULT:
					return DirEntry( entry->fts_path /*TODO*/ );
					
				FLUX_SWITCH_UNREACHABLE( FTS_DC );
				FLUX_SWITCH_UNREACHABLE( FTS_NSOK );
			}
		}
		
		return DirEntry();
	}
#	endif // ~ !PLATFORM_WIN32
}

//--    $ DirListWin32_                     ///{{{2///////////////////////////
namespace
{
#	if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
	//TODO
#	endif // ~ PLATFORM_WIN32
}
#endif

//--    $ fts_err_str_()                    ///{{{2///////////////////////////
namespace
{
#if 0
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	char const* fts_err_str_( int aCode ) noexcept
	{
		switch( aCode )
		{
			case FTS_DNR: return "unable to read directory"; 
			case FTS_ERR: return "generic error";
			case FTS_NS: return "unable to stat file";

			default: return "no error";
		}

		FLUX_INTENT_UNREACHABLE();
	}
#	endif // ~ !PLATFORM_WIN32
#endif
}

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
