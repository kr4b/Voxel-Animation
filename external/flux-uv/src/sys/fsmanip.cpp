/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	manipulate files
 *
 * TODO: change exception from FsError to something else?
 */
/*-***************************************************************** -}}}1- */

#include <flux/sys/fsmanip.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/except/einfo.hpp>

#include <flux/sys/errors.hpp>
#include <flux/sys/fsquery.hpp>
#include <flux/sys/pathutil.hpp>
#include <flux/sys/errno_category.hpp>
#include <flux/sys/win32_category.hpp>

#include <flux/compat/system_error.hpp>

#include <flux/util/intent.hpp>
#include <flux/util/narrow.hpp>
#include <flux/util/switch.hpp>
#include <flux/util/scope_exit.hpp>

#if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
#	include <sys/types.h>
#	include <sys/stat.h>

#	include <fts.h>
#	include <fcntl.h>
#	include <unistd.h>
#else // PLATFORM_WIN32
#	include <flux/sys/win32/windows.h>
#endif // ~ PLATFORM

#include "pathbuf.hxx"

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	void dir_create_simple_( SysPathView const& );
	bool dir_create_simple_( SysPathView const&, compat::error_code& ) noexcept;

	void dir_create_recursive_( SysPathView const& );
	bool dir_create_recursive_( SysPathView const&, compat::error_code& ) noexcept;

	void dir_rm_simple_( SysPathView const& );
	bool dir_rm_simple_( SysPathView const&, compat::error_code& ) noexcept;

	void dir_rm_recursive_( SysPathView const& );
	bool dir_rm_recursive_( SysPathView const&, compat::error_code& ) noexcept;

#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
#		define FLUXTMP_SYS_CHAR_(x) x

	mode_t getumask_() noexcept;
	char const* fts_err_str_( int ) noexcept;

#	else // PLATFORM_WIN32
#		define FLUXTMP_SYS_CHAR_(x) L##x

	void next_file_and_rmdir_( ftl::Vector<HANDLE>&, SysPathStr&, WIN32_FIND_DATAW* );

#	endif // ~ PLATFORM
}

//--    dir_create()                        ///{{{2///////////////////////////
void dir_create( SysPathView const& aPath, bool aRecursive )
{
	if( !aRecursive ) dir_create_simple_( aPath );
	else dir_create_recursive_( aPath );
}
bool dir_create( SysPathView const& aPath, bool aRecursive, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	if( !aRecursive ) return dir_create_simple_( aPath, aErrorOut );
	else return dir_create_recursive_( aPath, aErrorOut );
}

//--    dir_remove()                        ///{{{2///////////////////////////
void dir_remove( SysPathView const& aPath, bool aRecursive )
{
	if( !aRecursive ) dir_rm_simple_( aPath );
	else dir_rm_recursive_( aPath );
}
bool dir_remove( SysPathView const& aPath, bool aRecursive, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	if( !aRecursive ) return dir_rm_simple_( aPath, aErrorOut );
	else return dir_rm_recursive_( aPath, aErrorOut );
}

//--    file_create()                       ///{{{2///////////////////////////
void file_create( SysPathView const& aPath, bool aRecursive )
{
	if( aRecursive )
	{
		auto const lp = path_lexical_parent( aPath );
		dir_create( lp, true );
	}

	detail::SysPathBuf path;
	detail::make_zpath( path, aPath );

#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	mode_t const mask = getumask_();
	int fd = ::open( path.data(), O_WRONLY|O_CREAT|O_TRUNC, DEFFILEMODE & ~mask );

	if( -1 == fd )
	{
		int errno_ = errno;
		FLUX_THROW_IF( EACCES == errno_, error::FsPermissionDenied )
			<< einfo::ApiError( make_errno_error_code( errno_ ) )
			<< einfo::ApiFunction( "open" )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
		;
		FLUX_THROW( error::FsError )
			<< einfo::ApiError( make_errno_error_code( errno_ ) )
			<< einfo::ApiFunction( "open" )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
		;
	}

	::close( fd );
#	else // PLATFORM_WIN32
	HANDLE fh = ::CreateFileW( path.data(), GENERIC_READ|GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr );

	if( INVALID_HANDLE_VALUE == fh )
	{
		DWORD err = ::GetLastError();
		FLUX_THROW_IF( ERROR_ACCESS_DENIED == err, error::FsPermissionDenied )
			<< einfo::ApiError( make_win32_error_code(err) )
			<< einfo::ApiFunction( "CreateFileW" )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
		;
		FLUX_THROW( error::FsError )
			<< einfo::ApiError( make_win32_error_code(err) )
			<< einfo::ApiFunction( "CreateFileW" )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
		;
	}

	::CloseHandle( fh );
#	endif // ~ PLATFORM
}
bool file_create( SysPathView const& aPath, bool aRecursive, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	
	if( aRecursive )
	{
		auto const lp = path_lexical_parent( aPath ); //TODO TODO FIXME not noexcept, but only bad_alloc.
		if( !dir_create( lp, true, aErrorOut ) )
			return false;
	}

	detail::SysPathBuf path;
	if( !detail::make_zpath( path, aPath, 0, aErrorOut ) )
		return false;

#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	mode_t const mask = getumask_();
	int fd = ::open( path.data(), O_WRONLY|O_CREAT|O_TRUNC, DEFFILEMODE & ~mask );

	if( -1 == fd )
	{
		switch( int errno_ = errno )
		{
			case EACCES:
				aErrorOut = compat::make_error_code( compat::errc::permission_denied );
				return false;

			default:
				aErrorOut = make_errno_error_code( errno_ );
				return false;
		}
	}

	::close( fd );
#	else // PLATFORM_WIN32
	HANDLE fh = ::CreateFileW( path.data(), GENERIC_READ|GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr );

	if( INVALID_HANDLE_VALUE == fh )
	{
		switch( DWORD err = ::GetLastError() )
		{
			case ERROR_ACCESS_DENIED:
				aErrorOut = compat::make_error_code( compat::errc::permission_denied );
				return false;

			default:
				aErrorOut = make_win32_error_code( err );
				return false;
		}
	}

	::CloseHandle( fh );
#	endif // ~ PLATFORM

	return true;
}


//--    file_remove()                       ///{{{2///////////////////////////
void file_remove( SysPathView const& aPath )
{
	detail::SysPathBuf path;
	detail::make_zpath( path, aPath );
	
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	if( -1 == ::unlink( path.data() ) )
	{
		int errno_ = errno;
		FLUX_THROW_IF( EACCES == errno_, error::FsPermissionDenied )
			<< einfo::ApiError( make_errno_error_code( errno_ ) )
			<< einfo::ApiFunction( "unlink" )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
		;
		FLUX_THROW( error::FsError )
			<< einfo::ApiError( make_errno_error_code( errno_ ) )
			<< einfo::ApiFunction( "unlink" )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
		;
	}
#	else // PLATFORM_WIN32
	if( !::DeleteFileW( path.data() ) )
	{
		DWORD err = ::GetLastError();
		FLUX_THROW_IF( ERROR_ACCESS_DENIED == err, error::FsPermissionDenied )
			<< einfo::ApiError( make_win32_error_code(err) )
			<< einfo::ApiFunction( "DeleteFileW" )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
		;
		FLUX_THROW( error::FsError )
			<< einfo::ApiError( make_win32_error_code(err) )
			<< einfo::ApiFunction( "DeleteFileW" )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
		;
	}
#	endif // ~ PLATFORM
}
bool file_remove( SysPathView const& aPath, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );

	detail::SysPathBuf path;
	if( !detail::make_zpath( path, aPath, 0, aErrorOut ) )
		return false;
	
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	if( -1 == ::unlink( path.data() ) )
	{
		switch( int errno_ = errno )
		{
			case EACCES:
				aErrorOut = compat::make_error_code( compat::errc::permission_denied );
				return false;

			default:
				aErrorOut = make_errno_error_code( errno_ );
				return false;
		}
	}
#	else // PLATFORM_WIN32
	if( !::DeleteFileW( path.data() ) )
	{
		switch( DWORD err = ::GetLastError() )
		{
			case ERROR_ACCESS_DENIED:
				aErrorOut = compat::make_error_code( compat::errc::permission_denied );
				return false;

			default:
				aErrorOut = make_win32_error_code( err );
				return false;
		}
	}
#	endif // ~ PLATFORM

	return true;
}


//--    file_resize()                       ///{{{2///////////////////////////
void file_resize( SysPathView const& aPath, std::uintmax_t aNewSize )
{
	detail::SysPathBuf path;
	detail::make_zpath( path, aPath );
	
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	if( -1 == ::truncate( path.data(), util::narrow<off_t>(aNewSize) ) )
	{
		int errno_ = errno;
		FLUX_THROW_IF( EACCES == errno_, error::FsPermissionDenied )
			<< einfo::ApiError( make_errno_error_code( errno_ ) )
			<< einfo::ApiFunction( "truncate" )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
		;
		FLUX_THROW( error::FsError )
			<< einfo::ApiError( make_errno_error_code( errno_ ) )
			<< einfo::ApiFunction( "truncate" )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}' to size {:d}"), path_to_utf8(aPath), aNewSize )
		;
	}
#	else // PLATFORM_WIN32
	HANDLE fh = ::CreateFileW( path.data(), GENERIC_WRITE, 0, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr );

	if( INVALID_HANDLE_VALUE == fh )
	{
		DWORD err = ::GetLastError();
		FLUX_THROW_IF( ERROR_ACCESS_DENIED == err, error::FsPermissionDenied )
			<< einfo::ApiError( make_win32_error_code(err) )
			<< einfo::ApiFunction( "CreateFileW" )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
		;
		FLUX_THROW( error::FsError )
			<< einfo::ApiError( make_win32_error_code(err) )
			<< einfo::ApiFunction( "CreateFileW" )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
		;
	}

	FLUX_UTIL_ON_SCOPE_EXIT
	{
		::CloseHandle( fh );
	};

	LARGE_INTEGER li;
	li.QuadPart = util::narrow<decltype(li.QuadPart)>(aNewSize);
	auto const rr = ::SetFilePointerEx( fh, li, nullptr, FILE_BEGIN );

	if( rr == INVALID_SET_FILE_POINTER )
	{
		FLUX_THROW( error::FsError )
			<< einfo::ApiError( make_win32_error_code() )
			<< einfo::ApiFunction( "SetFilePointerEx" )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("on file at path '{}'"), path_to_utf8(aPath) )
		;
	}

	if( !::SetEndOfFile( fh ) )
	{
		FLUX_THROW( error::FsError )
			<< einfo::ApiError( make_win32_error_code() )
			<< einfo::ApiFunction( "SetEndOfFile" )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("on file at path '{}'"), path_to_utf8(aPath) )
		;

	}
#	endif // ~ PLATFORM
}

bool file_resize( SysPathView const& aPath, std::uintmax_t aNewSize, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	
	detail::SysPathBuf path;
	if( !detail::make_zpath( path, aPath, 0, aErrorOut ) )
		return false;
	
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	if( -1 == ::truncate( path.data(), util::narrow<off_t>(aNewSize) ) )
	{
		switch( int errno_ = errno )
		{
			case EACCES:
				aErrorOut = compat::make_error_code( compat::errc::permission_denied );
				return false;

			default:
				aErrorOut = make_errno_error_code( errno_ );
				return false;
		}
	}
#	else // PLATFORM_WIN32
	HANDLE fh = ::CreateFileW( path.data(), GENERIC_WRITE, 0, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr );

	if( INVALID_HANDLE_VALUE == fh )
	{
		switch( DWORD err = ::GetLastError() )
		{
			case ERROR_ACCESS_DENIED:
				aErrorOut = compat::make_error_code( compat::errc::permission_denied );
				return false;

			default:
				aErrorOut = make_win32_error_code( err );
				return false;
		}
	}

	FLUX_UTIL_ON_SCOPE_EXIT
	{
		::CloseHandle( fh );
	};

	LARGE_INTEGER li;
	li.QuadPart = util::narrow<decltype(li.QuadPart)>(aNewSize);
	auto const rr = ::SetFilePointerEx( fh, li, nullptr, FILE_BEGIN );

	if( rr == INVALID_SET_FILE_POINTER )
	{
		aErrorOut = make_win32_error_code();
		return false;
	}

	if( !::SetEndOfFile( fh ) )
	{
		aErrorOut = make_win32_error_code();
		return false;
	}
#	endif // ~ PLATFORM

	return true;
}

//--    $ dir_create_simple_()              ///{{{2///////////////////////////
namespace
{
	void dir_create_simple_( SysPathView const& aPath )
	{
		SysPathStr path(aPath);
		
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		mode_t const mask = getumask_();
		if( -1 == ::mkdir( path.c_str(), ACCESSPERMS & ~mask ) )
		{
			int errno_ = errno;
			FLUX_THROW_IF( EACCES == errno_, error::FsPermissionDenied )
				<< einfo::ApiError( make_errno_error_code( errno_ ) )
				<< einfo::ApiFunction( "mkdir" )
				<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
			;
			FLUX_THROW( error::FsError )
				<< einfo::ApiError( make_errno_error_code( errno_ ) )
				<< einfo::ApiFunction( "mkdir" )
				<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
			;
		}
#		else // PLATFORM_WIN32
		if( !::CreateDirectoryW( path.c_str(), nullptr ) )
		{
			DWORD err = ::GetLastError();
			FLUX_THROW_IF( ERROR_ACCESS_DENIED == err, error::FsPermissionDenied )
				<< einfo::ApiError( make_win32_error_code(err) )
				<< einfo::ApiFunction( "CreateDirectoryW" )
				<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
			;
			FLUX_THROW( error::FsError )
				<< einfo::ApiError( make_win32_error_code(err) )
				<< einfo::ApiFunction( "CreateDirectoryW" )
				<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
			;
		}
#		endif // ~ PLATFORM
	}

	bool dir_create_simple_( SysPathView const& aPath, compat::error_code& aErrorOut ) noexcept
	{
		FLUX_ASSERT( !aErrorOut );
		
		detail::SysPathBuf path;
		if( !detail::make_zpath(path, aPath, 0, aErrorOut) )
			return false;
		
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		mode_t const mask = getumask_();
		if( -1 == ::mkdir( path.data(), ACCESSPERMS & ~mask ) )
		{
			switch( int errno_ = errno )
			{
				case EACCES:
					aErrorOut = compat::make_error_code( compat::errc::permission_denied );
					return false;
				default:
					aErrorOut = make_errno_error_code( errno_ );
					return false;
			}
		}
#		else // PLATFORM_WIN32
		if( !::CreateDirectoryW( path.data(), nullptr ) )
		{
			switch( DWORD err = ::GetLastError() )
			{
				case ERROR_ACCESS_DENIED:
					aErrorOut = compat::make_error_code( compat::errc::permission_denied );
					return false;

				default:
					aErrorOut = make_win32_error_code( err );
					return false;
			}
		}
#		endif // ~ PLATFORM

		return true;
	}
}

//--    $ dir_create_recursive_()           ///{{{2///////////////////////////
namespace
{
	void dir_create_recursive_( SysPathView const& aPath )
	{
		// sanity: ignore empty paths.
		if( aPath.empty() )
			return;

		// early outs: if the full path exists, don't do anything
		auto const sb = stat_path( aPath );
		if( EFileKind::directory == stat_get_type( sb ) )
			return;

		// assume that most of the path tends to exist already, so we'll make
		// a backwards linear search for the first directory that exists
		detail::SysPathBuf path;
		path.reserve( aPath.size()+1 );
		path.insert( path.begin(), aPath.begin(), aPath.end() );
		while( !path.empty() && kSysPathSeparator == path.back() )
			path.pop_back();

		if( path.empty() )
			return;

		path.emplace_back( FLUXTMP_SYS_CHAR_('\0') );

		auto rbeg = path.rbegin(), rend = path.rend();
		while( rbeg != rend )
		{
			for( ; rbeg != rend && kSysPathSeparator != *rbeg; ++rbeg )
				;

			for( ; rbeg != rend && kSysPathSeparator == *rbeg; ++rbeg )
				*rbeg = FLUXTMP_SYS_CHAR_('\0');

			if( rbeg != rend )
			{
				auto const ty = stat_get_type( stat_path( path.data() ) );
				if( EFileKind::directory == ty )
					break;

				FLUX_THROW_IF( EFileKind::notFound != ty, error::FsTypeMismatch )
					<< einfo::ErrorMeta( FLUX_FMT_STRING("not a directory '{}'"), path_to_utf8(path.data()) )
					<< einfo::ErrorDesc( FLUX_FMT_STRING("recursive create dir '{}'"), path_to_utf8(aPath) )
				;
			}
		}

		auto fbeg = rbeg.base(), fend = path.end()-1;
		for( ; fbeg != fend && *fbeg == FLUXTMP_SYS_CHAR_('\0'); ++fbeg )
			*fbeg = kSysPathSeparator;

		// create directories..
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		mode_t const mask = getumask_();
#		endif // ~ PLATFORM

		while( fbeg != fend )
		{
#			if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
			if( -1 == ::mkdir( path.data(), ACCESSPERMS & ~mask ) )
			{
				int errno_ = errno;
				FLUX_THROW_IF( EACCES == errno_, error::FsPermissionDenied )
					<< einfo::ApiError( make_errno_error_code( errno_ ) )
					<< einfo::ApiFunction( "mkdir" )
					<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(path.data()) )
					<< einfo::ErrorDesc( FLUX_FMT_STRING("recursive create dir '{}'"), path_to_utf8(aPath) )
				;
				FLUX_THROW_IF( errno_ != EEXIST, error::FsError )
					<< einfo::ApiError( make_errno_error_code( errno_ ) )
					<< einfo::ApiFunction( "mkdir" )
					<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(path.data()) )
					<< einfo::ErrorDesc( FLUX_FMT_STRING("recursive create dir '{}'"), path_to_utf8(aPath) )
				;
			}
#			else // PLATFORM_WIN32
			if( !::CreateDirectoryW( path.data(), nullptr ) )
			{
				DWORD err = ::GetLastError();
				FLUX_THROW_IF( ERROR_ACCESS_DENIED == err, error::FsPermissionDenied )
					<< einfo::ApiError( make_win32_error_code(err) )
					<< einfo::ApiFunction( "CreateDirectoryW" )
					<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(path.data()) )
					<< einfo::ErrorDesc( FLUX_FMT_STRING("recursive create dir '{}'"), path_to_utf8(aPath) )
				;
				FLUX_THROW_IF( ERROR_ALREADY_EXISTS != err, error::FsError )
					<< einfo::ApiError( make_win32_error_code(err) )
					<< einfo::ApiFunction( "CreateDirectoryW" )
					<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(path.data()) )
					<< einfo::ErrorDesc( FLUX_FMT_STRING("recursive create dir '{}'"), path_to_utf8(aPath) )
				;
			}
#			endif // ~ PLATFORM

			for( ; fbeg != fend && *fbeg != FLUXTMP_SYS_CHAR_('\0'); ++fbeg )
				;

			for( ; fbeg != fend && *fbeg == FLUXTMP_SYS_CHAR_('\0'); ++fbeg )
				*fbeg = kSysPathSeparator;
		}
	}
	bool dir_create_recursive_( SysPathView const& aPath, compat::error_code& aErrorOut ) noexcept
	{
		FLUX_ASSERT( !aErrorOut );
		
		// sanity: ignore empty paths.
		if( aPath.empty() )
			return true;

		// early outs: if the full path exists, don't do anything
		auto const sb = stat_path( aPath, aErrorOut );
		if( !sb && aErrorOut )
			return false;

		if( EFileKind::directory == stat_get_type( sb ) )
			return true;

		// assume that most of the path tends to exist already, so we'll make
		// a backwards linear search for the first directory that exists
		detail::SysPathBuf path;
		try
		{
			path.reserve( aPath.size()+1 );
			path.insert( path.begin(), aPath.begin(), aPath.end() );
			while( !path.empty() && kSysPathSeparator == path.back() )
				path.pop_back();

			if( path.empty() )
				return true;

			path.emplace_back( FLUXTMP_SYS_CHAR_('\0') );
		}
		catch( std::bad_alloc const& )
		{
			aErrorOut = compat::make_error_code( compat::errc::not_enough_memory );
			return false;
		}

		auto rbeg = path.rbegin(), rend = path.rend();
		while( rbeg != rend )
		{
			for( ; rbeg != rend && kSysPathSeparator != *rbeg; ++rbeg )
				;

			for( ; rbeg != rend && kSysPathSeparator == *rbeg; ++rbeg )
				*rbeg = FLUXTMP_SYS_CHAR_('\0');

			if( rbeg != rend )
			{
				auto const ty = stat_get_type( stat_path( path.data(), aErrorOut ) );
				if( EFileKind::directory == ty )
					break;

				if( EFileKind::notFound != ty )
				{
					aErrorOut = compat::make_error_code( compat::errc::not_a_directory );
					return false;
				}
			}
		}

		auto fbeg = rbeg.base(), fend = path.end()-1;
		for( ; fbeg != fend && *fbeg == FLUXTMP_SYS_CHAR_('\0'); ++fbeg )
			*fbeg = kSysPathSeparator;

#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		mode_t const mask = getumask_();
#		endif // ~ PLATFORM

		while( fbeg != fend )
		{
#			if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
			if( -1 == ::mkdir( path.data(), ACCESSPERMS & ~mask ) )
			{
				switch( int errno_ = errno )
				{
					case EEXIST:
						break;

					case EACCES:
						aErrorOut = compat::make_error_code( compat::errc::permission_denied );
						return false;

					default:
						aErrorOut = make_errno_error_code( errno_ );
						return false;
				}
			}
#			else // PLATFORM_WIN32
			if( !::CreateDirectoryW( path.data(), nullptr ) )
			{
				switch( DWORD err = ::GetLastError() )
				{
					case ERROR_ALREADY_EXISTS:
						break;

					case ERROR_ACCESS_DENIED:
						aErrorOut = compat::make_error_code( compat::errc::permission_denied );
						return false;

					default:	
						aErrorOut = make_win32_error_code( err );
						return false;
				}
			}
#			endif // ~ PLATFORM

			for( ; fbeg != fend && *fbeg != FLUXTMP_SYS_CHAR_('\0'); ++fbeg )
				;

			for( ; fbeg != fend && *fbeg == FLUXTMP_SYS_CHAR_('\0'); ++fbeg )
				*fbeg = kSysPathSeparator;
		}

		return true;
	}
}

//--    $ dir_rm_simple_()                  ///{{{2///////////////////////////
namespace
{
	void dir_rm_simple_( SysPathView const& aPath )
	{
		detail::SysPathBuf path;
		detail::make_zpath( path, aPath );
		
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		if( -1 == ::rmdir( path.data() ) )
		{
			int errno_ = errno;
			FLUX_THROW_IF( EACCES == errno_, error::FsPermissionDenied )
				<< einfo::ApiError( make_errno_error_code( errno_ ) )
				<< einfo::ApiFunction( "rmdir" )
				<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
			;
			FLUX_THROW( error::FsError )
				<< einfo::ApiError( make_errno_error_code( errno_ ) )
				<< einfo::ApiFunction( "rmdir" )
				<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
			;
		}

#		else // PLATFORM_WIN32
		if( !::RemoveDirectoryW( path.data() ) )
		{
			DWORD err = ::GetLastError();
			FLUX_THROW_IF( ERROR_ACCESS_DENIED == err, error::FsPermissionDenied )
				<< einfo::ApiError( make_win32_error_code(err) )
				<< einfo::ApiFunction( "RemoveDirectoryW" )
				<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
			;
			FLUX_THROW( error::FsError )
				<< einfo::ApiError( make_win32_error_code(err) )
				<< einfo::ApiFunction( "RemoveDirectoryW" )
				<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
			;
		}
#		endif // ~ PLATFORM
	}
	bool dir_rm_simple_( SysPathView const& aPath, compat::error_code& aErrorOut ) noexcept
	{
		detail::SysPathBuf path;
		if( !detail::make_zpath( path, aPath, 0, aErrorOut ) )
			return false;
		
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		if( -1 == ::rmdir( path.data() ) )
		{
			switch( int errno_ = errno )
			{
				case EACCES:
					aErrorOut = compat::make_error_code( compat::errc::permission_denied );
					return false;

				default:
					aErrorOut = make_errno_error_code( errno_ );
					return false;
			}
		}

#		else // PLATFORM_WIN32
		if( !::RemoveDirectoryW( path.data() ) )
		{
			switch( DWORD err = ::GetLastError() )
			{
				case EACCES:
					aErrorOut = compat::make_error_code( compat::errc::permission_denied );
					return false;

				default:
					aErrorOut = make_win32_error_code( err );
					return false;
			}
		}
#		endif // ~ PLATFORM

		return true;
	}

}

//--    $ dir_rm_recursive_()               ///{{{2///////////////////////////
namespace
{
	void dir_rm_recursive_( SysPathView const& aPath )
	{
		// we require the initial path to be a directory. (or a symlink to one)
		auto const ty = sys::stat_get_type( sys::stat_path( aPath ) );
		if( EFileKind::notFound == ty )
			return;

		FLUX_THROW_IF( EFileKind::directory != ty, error::FsTypeMismatch )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("not a directory '{}'"), path_to_utf8(aPath) )
			<< einfo::ErrorDesc( "recursive delete dir" )
		;

#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		/* Well ... we have plenty of choice here; there's fts(2), ftw(2), 
		 * opendir() & co, and probably a whole slew of others.
		 *
		 *  - fts has the most sane API, but is BSD (and not POSIX)
		 *  - (n)ftw is POSIX, but needs callbacks and thus sucks
		 *  - opendir() has us manually filtering symlinks and stuff
		 *
		 * Anyway, for now, use fts because it has the most convenient API.
		 * See http://stackoverflow.com/a/27808574
		 */
		detail::SysPathBuf path;
		detail::make_zpath( path, aPath );
		
		char* dirlist[] = { path.data(), nullptr };
		if( FTS* fts = fts_open( dirlist, FTS_NOCHDIR|FTS_PHYSICAL|FTS_XDEV|FTS_COMFOLLOW, nullptr ) )
		{
			FLUX_UTIL_ON_SCOPE_EXIT
			{
				::fts_close( fts );
			};
			
			while( FTSENT* cur = ::fts_read( fts ) )
			{
				switch( cur->fts_info )
				{
					case FTS_NS: FLUX_SWITCH_FALLTHROUGH();
					case FTS_DNR: FLUX_SWITCH_FALLTHROUGH();
					case FTS_ERR:
						FLUX_THROW( error::FsError )
							<< einfo::ApiFunction( "fts_read" )
							<< einfo::ApiError( make_errno_error_code( cur->fts_errno ) )
							<< einfo::ErrorDesc( FLUX_FMT_STRING("{} ({:d})"), fts_err_str_(cur->fts_info), cur->fts_info )
							<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), cur->fts_path )
						;
						break;

					case FTS_D:
						// are removed when ascending (FTS_DP), so ignore.
						break;

					case FTS_DP: FLUX_SWITCH_FALLTHROUGH();
					case FTS_F: FLUX_SWITCH_FALLTHROUGH();
					case FTS_SL: FLUX_SWITCH_FALLTHROUGH();
					case FTS_SLNONE: FLUX_SWITCH_FALLTHROUGH();
					case FTS_DEFAULT:
						if( -1 == ::remove( cur->fts_path ) )
						{
							FLUX_THROW( error::FsError )
								<< einfo::ApiError( make_errno_error_code() )
								<< einfo::ApiFunction( "remove" )
								<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), cur->fts_path )
							;
						}
						break;

					FLUX_SWITCH_UNREACHABLE( FTS_DC );
					FLUX_SWITCH_UNREACHABLE( FTS_DOT );
					FLUX_SWITCH_UNREACHABLE( FTS_NSOK );
				}
			}
		}
		else
		{
			int errno_ = errno;
			FLUX_THROW_IF( EACCES == errno_, error::FsPermissionDenied )
				<< einfo::ApiError( make_errno_error_code(errno_) )
				<< einfo::ApiFunction( "fts_open" )
				<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
			;
			FLUX_THROW( error::FsError )
				<< einfo::ApiError( make_errno_error_code(errno_) )
				<< einfo::ApiFunction( "fts_open" )
				<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
			;
		}

#		else // PLATFORM_WIN32
		/* On Win32 we get Win32. In hindsight, having a choice isn't always
		 * that bad.
		 *
		 * Anyway, the MSDN example suggest that one append '\*' to the path
		 * before passing it to FindFirstFile(). Other examples suggest that
		 * this is not needed in all cases, but whatever. MSDN knows better.
		 */
		detail::SysPathBuf ipath;
		detail::make_zpath( ipath, aPath, 2 );

		ipath.back() = L'\\';
		ipath.push_back( L'*' );
		ipath.push_back( L'\0' );

		WIN32_FIND_DATAW wfd;
		HANDLE h = ::FindFirstFileW( ipath.data(), &wfd );

		FLUX_THROW_IF( INVALID_HANDLE_VALUE == h, error::FsError )
			<< einfo::ApiError( make_win32_error_code() )
			<< einfo::ApiFunction( "FindFirstFileW" )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aPath) )
		;

		SysPathStr path{ aPath };
		ftl::Vector<HANDLE> stack{ { h } };

		FLUX_UTIL_ON_SCOPE_EXIT
		{
			// If we leave by exception -- under normal circumstances, the
			// stack will be empty.
			for( auto h : stack )
				::FindClose( h );
		};

		do
		{
			// directory? Note: reparse points / symlinks to directories DO NOT have the
			// FILE_ATRIBUTE_DIRECTORY attribute set, so we won't follow these.
			if( FILE_ATTRIBUTE_DIRECTORY & wfd.dwFileAttributes )
			{
				// But, nevertheless, check this. Don't want to accidentally follow a 
				// malicious reparse point all over the place.
				FLUX_THROW_IF( FILE_ATTRIBUTE_REPARSE_POINT & wfd.dwFileAttributes, error::FsPathUnexpected )
					<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(path) )
					<< einfo::ErrorDesc( "encountered directory that is also a reparse point. this should not happen" )
				;

				// Ignore "." and ".." otherwise terrible things will happen. Srsly.
				if( L'.' == wfd.cFileName[0] && (L'\0' == wfd.cFileName[1] || (L'.' == wfd.cFileName[1] && L'\0' == wfd.cFileName[2]) ) )
				{
					next_file_and_rmdir_( stack, path, &wfd );
					continue;
				}

				// Descend into nested directory. next_file_and_rmdir_() will
				// delete the directory once it's empty.
				auto nextPath = path_lexical_join( path, wfd.cFileName );
				auto nextHandle = ::FindFirstFileW( path_lexical_join(nextPath,L"\\*").c_str(), &wfd );

				FLUX_THROW_IF( INVALID_HANDLE_VALUE == nextHandle, error::FsError )
					<< einfo::ApiError( make_win32_error_code() )
					<< einfo::ApiFunction( "FindFirstFileW" )
					<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(nextPath) )
				;

				stack.push_back( nextHandle );
				path = std::move(nextPath);
			}
			else
			{
				// Not a directory, delete the file
				auto const fpath = path_lexical_join( path, wfd.cFileName );
				if( !::DeleteFileW( fpath.c_str() ) )
				{
					DWORD err = ::GetLastError();
					FLUX_THROW_IF( ERROR_ACCESS_DENIED == err, error::FsPermissionDenied )
						<< einfo::ApiError( make_win32_error_code(err) )
						<< einfo::ApiFunction( "DeleteFileW" )
						<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(fpath) )
					;
					FLUX_THROW( error::FsError )
						<< einfo::ApiError( make_win32_error_code(err) )
						<< einfo::ApiFunction( "DeleteFileW" )
						<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(fpath) )
					;
					break;
				}

				next_file_and_rmdir_( stack, path, &wfd );
			}
		}
		while( !stack.empty() );

#		endif // ~ PLATFORM
	}
	bool dir_rm_recursive_( SysPathView const& aPath, compat::error_code& aErrorOut ) noexcept
	{
		FLUX_ASSERT( !aErrorOut );
			
		// we require the initial path to be a directory. (or a symlink to one)
		auto const sb = sys::stat_path( aPath, aErrorOut );
		if( !sb && aErrorOut )
			return false;

		auto const ty = sys::stat_get_type( sb );
		if( EFileKind::notFound == ty )
			return true;

		if( EFileKind::directory != ty )
		{
			aErrorOut = compat::make_error_code( compat::errc::not_a_directory );
			return false;
		}

#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		detail::SysPathBuf path;
		if( !detail::make_zpath( path, aPath, 0, aErrorOut ) )
			return false;
		
		char* dirlist[] = { path.data(), nullptr };
		if( FTS* fts = fts_open( dirlist, FTS_NOCHDIR|FTS_PHYSICAL|FTS_XDEV|FTS_COMFOLLOW, nullptr ) )
		{
			FLUX_UTIL_ON_SCOPE_EXIT
			{
				::fts_close( fts );
			};
			
			while( FTSENT* cur = ::fts_read( fts ) )
			{
				switch( cur->fts_info )
				{
					case FTS_NS: FLUX_SWITCH_FALLTHROUGH();
					case FTS_DNR: FLUX_SWITCH_FALLTHROUGH();
					case FTS_ERR:
						switch( cur->fts_errno )
						{
							case EACCES:
								aErrorOut = compat::make_error_code( compat::errc::permission_denied );
								break;

							default:
								aErrorOut = make_errno_error_code( cur->fts_errno );
						}
						return false;

					case FTS_D:
						// are removed when ascending (FTS_DP), so ignore.
						break;

					case FTS_DP: FLUX_SWITCH_FALLTHROUGH();
					case FTS_F: FLUX_SWITCH_FALLTHROUGH();
					case FTS_SL: FLUX_SWITCH_FALLTHROUGH();
					case FTS_SLNONE: FLUX_SWITCH_FALLTHROUGH();
					case FTS_DEFAULT:
						if( -1 == ::remove( cur->fts_path ) )
						{
							switch( int errno_ = errno )
							{
								case EACCES:
									aErrorOut = compat::make_error_code( compat::errc::permission_denied );
									break;

								default:
									aErrorOut = make_errno_error_code( errno_ );
							}

							return false;
						}
						break;

					FLUX_SWITCH_UNREACHABLE( FTS_DC );
					FLUX_SWITCH_UNREACHABLE( FTS_DOT );
					FLUX_SWITCH_UNREACHABLE( FTS_NSOK );
				}
			}
		}
		else
		{
			switch( int errno_ = errno )
			{
				case EACCES:
					aErrorOut = compat::make_error_code( compat::errc::permission_denied );
					break;

				default:
					aErrorOut = make_errno_error_code( errno_ );
			}

			return false;
		}

#		else // PLATFORM_WIN32
		// TODO: maybe some other time
		// TODO
		try
		{
			dir_rm_recursive_( aPath );
		}
		catch( std::bad_alloc const& )
		{
			aErrorOut = compat::make_error_code( compat::errc::permission_denied );
			return false;
		}
		catch( error::FsError const& )
		{
			//TODO query_error_info() et al.
			aErrorOut = compat::make_error_code( compat::errc::state_not_recoverable );
			return false;
		}
#		endif // ~ PLATFORM

		return true;
	}
}

//--    $ getumask_()                       ///{{{2///////////////////////////
namespace
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	mode_t getumask_() noexcept
	{
		/* getumask(): "This function is documented but not implemented yet in
		 * glibc, as at version 2.9". It's equivalent to the implementation
		 * given here, minus the race condition if somebody else is modifying
		 * the umask of the process concurrently.
		 *
		 * Oh, well.
		 */
		mode_t ret = umask(0);
		umask(ret);
		return ret;
	}
#	endif // ~ !PLATFORM_WIN32
}

//--    $ fts_err_str_()                    ///{{{2///////////////////////////
namespace
{
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
}

//--    $ find_next_file_()                 ///{{{2///////////////////////////
namespace
{
#	if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
	void next_file_and_rmdir_( ftl::Vector<HANDLE>& aStack, SysPathStr& aPath, WIN32_FIND_DATAW* aWFD )
	{
		if( !::FindNextFile( aStack.back(), aWFD ) )
		{
			DWORD err = ::GetLastError();
			if( ERROR_NO_MORE_FILES == err )
			{
				::FindClose( aStack.back() );
				if( !::RemoveDirectoryW( aPath.c_str() ) )
				{
					FLUX_THROW( error::FsError )
						<< einfo::ApiError( make_win32_error_code() )
						<< einfo::ApiFunction( "RemoveDirectoryW" )
						<< einfo::ErrorMeta( FLUX_FMT_STRING("on path '{}'"), path_to_utf8(aWFD->cFileName) )
					;
				}

				aStack.pop_back();
				aPath = path_lexical_parent( aPath );

				if( !aStack.empty() )
					next_file_and_rmdir_( aStack, aPath, aWFD );
			}
			else
			{
				FLUX_THROW( error::FsError )
					<< einfo::ApiError( make_win32_error_code() )
					<< einfo::ApiFunction( "FindNextFile" )
				;
			}
		}
	}
#	endif // ~ PLATFORM_WIN32
}

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
