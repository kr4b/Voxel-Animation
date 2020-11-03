/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	File mapping implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/sys/filemap.hpp>

#include <flux/sys/info.hpp>
#include <flux/sys/errors.hpp>
#include <flux/sys/pathutil.hpp>
#include <flux/sys/errno_category.hpp>
#include <flux/sys/win32_category.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/format.hpp>

#include <flux/util/intent.hpp>
#include <flux/util/narrow.hpp>
#include <flux/util/scope_exit.hpp>

#if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
#	include <sys/types.h>
#	include <sys/stat.h>

#	include <errno.h>
#	include <fcntl.h>
#	include <unistd.h>
#	include <sys/mman.h>
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
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	mode_t getumask_() noexcept;

	int mapmode_to_mmapprot_( EFileMapMode ) noexcept;
	int mapmode_to_mmapflags_( EFileMapMode ) noexcept;
	int mapmode_to_openflags_( EFileMapMode, bool ) noexcept;
#	else // PLATFORM_WIN32
	void to_dwords_( std::uint32_t, DWORD&, DWORD& ) noexcept;
	void to_dwords_( std::uint64_t, DWORD&, DWORD& ) noexcept;

	DWORD mapmode_to_createfileflag_( EFileMapMode ) noexcept;
	DWORD mapmode_to_createfileshare_( EFileMapMode ) noexcept;
	DWORD mapmode_to_protmode_( EFileMapMode ) noexcept;
	DWORD mapmode_to_mapviewflags_( EFileMapMode ) noexcept;
#	endif // ~ PLATFORM
}

//--    FileMapInfo                         ///{{{2///////////////////////////
FileMapInfo::FileMapInfo()
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	: fd(-1)
	, size(0)
#	else // PLATFORM_WIN32
	: file(INVALID_HANDLE_VALUE)
	, mapping(INVALID_HANDLE_VALUE)
	, size(0)
#	endif // ~ PLATFORM
{}

//--    file mapping - generic              ///{{{2///////////////////////////
#if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
FileMapInfo file_map_prepare( SysPathView const& aPath, EFileMapMode aMode, bool aMustExist )
{
	detail::SysPathBuf path;
	detail::make_zpath( path, aPath );
	
	mode_t const mask = getumask_();
	int fd = ::open( path.data(), mapmode_to_openflags_(aMode,aMustExist), DEFFILEMODE & ~mask );

	int errno_ = errno;
	FLUX_THROW_IF( -1 == fd, sys::error::MapPrepareFailed )
		<< einfo::ApiFunction( "open" )
		<< einfo::ApiError( make_errno_error_code(errno_) )
		<< einfo::ErrorMeta( path_to_utf8(aPath).c_str() )
	;

	FLUX_UTIL_ON_SCOPE_EXIT{ if( -1 != fd ) ::close( fd ); };

	struct stat sb;
	if( 0 != fstat( fd, &sb ) )
	{
		int errno_ = errno;
		FLUX_THROW( sys::error::MapPrepareFailed )
			<< einfo::ApiFunction( "fstat" )
			<< einfo::ApiError( make_errno_error_code(errno_) )
			<< einfo::ErrorMeta( path_to_utf8(aPath).c_str() )
		;
	}

	// ok
	FileMapInfo ret;
	ret.fd = fd;
	ret.size = util::narrow<std::uintmax_t>(sb.st_size);

	fd = -1;
	return ret;
}
optional<FileMapInfo> file_map_prepare( SysPathView const& aPath, EFileMapMode aMode, bool aMustExist, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	
	detail::SysPathBuf path;
	if( !detail::make_zpath( path, aPath, 0, aErrorOut ) )
		return {};
	
	mode_t const mask = getumask_();
	int fd = ::open( path.data(), mapmode_to_openflags_(aMode,aMustExist), DEFFILEMODE & ~mask );
	if( -1 == fd )
	{
		aErrorOut = make_errno_error_code();
		return {};
	}
	
	FLUX_UTIL_ON_SCOPE_EXIT{ if( -1 != fd ) ::close( fd ); };

	struct stat sb;
	if( 0 != fstat( fd, &sb ) )
	{
		aErrorOut = make_errno_error_code();
		return {};
	}

	// ok
	auto x = util::narrow<std::uintmax_t>(sb.st_size, aErrorOut);
	if( !x )
		return {};
	
	FileMapInfo ret;
	ret.fd = fd;
	ret.size = x.value();

	fd = -1;
	return ret;
}

void file_map_update( FileMapInfo& aMapInfo )
{
	FLUX_THROW_IF( -1 == aMapInfo.fd, error::InvalidFileMap )
		<< einfo::ErrorDesc( "fd is -1" )
	;

	struct stat sb;
	if( 0 != fstat( aMapInfo.fd, &sb ) )
	{
		int errno_ = errno;
		FLUX_THROW( sys::error::MapPrepareFailed )
			<< einfo::ApiFunction( "fstat" )
			<< einfo::ApiError( make_errno_error_code(errno_) )
		;
	}

	aMapInfo.size = util::narrow<std::uintmax_t>(sb.st_size);
}
bool file_map_update( FileMapInfo& aMapInfo, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	if( -1 == aMapInfo.fd )
	{
		aErrorOut = compat::make_error_code( compat::errc::bad_file_descriptor );
		return false;
	}
	
	struct stat sb;
	if( 0 != fstat( aMapInfo.fd, &sb ) )
	{
		aErrorOut = make_errno_error_code();
		return false;
	}

	auto x = util::narrow<std::uintmax_t>(sb.st_size, aErrorOut);
	if( !x )
		return false;

	aMapInfo.size = x.value();
	return true;
}

void file_map_resize( FileMapInfo& aMapInfo, std::uintmax_t aSize )
{
	FLUX_THROW_IF( -1 == aMapInfo.fd, error::InvalidFileMap )
		<< einfo::ErrorDesc( "fd is -1" )
	;

	if( -1 == ::ftruncate( aMapInfo.fd, util::narrow<off_t>(aSize) ) )
	{
		int errno_ = errno;
		FLUX_THROW( error::MapResizeFailed )
			<< einfo::ApiFunction( "ftruncate" )
			<< einfo::ApiError( make_errno_error_code(errno_) )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("from {:d} to {:d} bytes"), aMapInfo.size, aSize )
		;
	}

	aMapInfo.size = aSize;
}
bool file_map_resize( FileMapInfo& aMapInfo, std::uintmax_t aSize, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	if( -1 == aMapInfo.fd )
	{
		aErrorOut = compat::make_error_code( compat::errc::bad_file_descriptor );
		return false;
	}

	auto x = util::narrow<off_t>(aSize, aErrorOut);
	if( !x )
		return false;
	
	if( -1 == ::ftruncate( aMapInfo.fd, x.value() ) )
	{
		aErrorOut = make_errno_error_code();
		return false;
	}

	aMapInfo.size = aSize;
	return true;
}


void file_map_destroy( FileMapInfo& aMapInfo ) noexcept
{
	if( -1 != aMapInfo.fd )
	{
		::close( aMapInfo.fd );
		aMapInfo.fd = -1;
	}

	aMapInfo.size = 0;
}


void* file_map_view_create( FileMapInfo const& aMapInfo, EFileMapMode aMode, std::uintmax_t aBegin, std::uintmax_t aLength )
{
	FLUX_THROW_IF( -1 == aMapInfo.fd, error::InvalidFileMap )
		<< einfo::ErrorDesc( "fd is -1" )
	;

	FLUX_THROW_IF( aBegin >= aMapInfo.size, error::MapOutOfRange )
		<< einfo::ErrorMeta( FLUX_FMT_STRING("size {:d}, requested at {:d}"), aMapInfo.size, aBegin )
	;
	
	auto const aligned = aBegin & ~(file_map_offset_minimum_alignment()-1);
	FLUX_THROW_IF( aligned != aBegin, error::MapBadAlignment )
		<< einfo::ErrorMeta( FLUX_FMT_STRING("minimum alignment {:d}, requested at {:d}"), file_map_offset_minimum_alignment(), aBegin )
	;

	//
	auto const prot = mapmode_to_mmapprot_( aMode );
	auto const flags = mapmode_to_mmapflags_( aMode );
	auto const size = (~std::uintmax_t(0) != aLength) ? aLength : (aMapInfo.size - aBegin);

	void* addr = ::mmap( nullptr, size, prot, flags, aMapInfo.fd, aligned );

	int errno_ = errno;
	FLUX_THROW_IF( MAP_FAILED == addr, error::MapError )
		<< einfo::ApiFunction( "mmap" )
		<< einfo::ApiError( make_errno_error_code(errno_) )
		<< einfo::ErrorMeta( FLUX_FMT_STRING("range {:d}+{:d} ({})"), aligned, size, to_string(aMode) )
	;

	return addr;
}
void* file_map_view_create( FileMapInfo const& aMapInfo, EFileMapMode aMode, std::uintmax_t aBegin, std::uintmax_t aLength, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	if( -1 == aMapInfo.fd )
	{
		aErrorOut = compat::make_error_code( compat::errc::bad_file_descriptor );
		return nullptr;
	}
	
	if( aBegin >= aMapInfo.size )
	{
		aErrorOut = compat::make_error_code( compat::errc::argument_out_of_domain );
		return nullptr;
	}

	auto const aligned = aBegin & ~(file_map_offset_minimum_alignment()-1);
	if( aligned != aBegin )
	{
		aErrorOut = compat::make_error_code( compat::errc::argument_out_of_domain );
		return nullptr;
	}

	//
	auto const prot = mapmode_to_mmapprot_( aMode );
	auto const flags = mapmode_to_mmapflags_( aMode );
	auto const size = (~std::uintmax_t(0) != aLength) ? aLength : (aMapInfo.size - aBegin);

	void* addr = ::mmap( nullptr, size, prot, flags, aMapInfo.fd, aligned );

	if( MAP_FAILED == addr )
	{
		aErrorOut = make_errno_error_code();
		return nullptr;
	}

	FLUX_ASSERT( addr ); // probably isn't nullptr, but...
	return addr;
}

void file_map_view_delete( FileMapInfo const& aMapInfo, void* aAddr, std::uintmax_t aLength )
{
	FLUX_THROW_IF( -1 == aMapInfo.fd, error::InvalidFileMap )
		<< einfo::ErrorDesc( "fd is -1" )
	;

	auto const size = (~std::uintmax_t(0) != aLength) ? aLength : aMapInfo.size;
	if( -1 == ::munmap( aAddr, size ) )
	{
		int errno_ = errno;
		FLUX_THROW( error::MapError )
			<< einfo::ApiFunction( "munmap" )
			<< einfo::ApiError( make_errno_error_code(errno_) )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("range {:p}+{:d}"), aAddr, size )
		;
	}
}
bool file_map_view_delete( FileMapInfo const& aMapInfo, void* aAddr, std::uintmax_t aLength, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	if( -1 == aMapInfo.fd )
	{
		aErrorOut = compat::make_error_code( compat::errc::bad_file_descriptor );
		return false;
	}

	auto const size = (~std::uintmax_t(0) != aLength) ? aLength : aMapInfo.size;
	if( -1 == ::munmap( aAddr, size ) )
	{
		aErrorOut = make_errno_error_code();
		return false;
	}

	return true;
}

void file_map_view_flush( FileMapInfo const& aMapInfo, void* aAddr, std::uintmax_t aLength, bool aAsync )
{
	FLUX_THROW_IF( -1 == aMapInfo.fd, error::InvalidFileMap )
		<< einfo::ErrorDesc( "fd is -1" )
	;

	auto const sync = aAsync ? MS_ASYNC : MS_SYNC;
	auto const size = (~std::uintmax_t(0) != aLength) ? aLength : aMapInfo.size;

	if( -1 == msync( aAddr, size, sync ) )
	{
		int errno_ = errno;
		FLUX_THROW( error::MapError )
			<< einfo::ApiFunction( "msync" )
			<< einfo::ApiError( make_errno_error_code(errno_) )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("range {:p}+{:d} ({})"), aAddr, size, aAsync ? "async":"sync" )
		;
	}
}
bool file_map_view_flush( FileMapInfo const& aMapInfo, void* aAddr, std::uintmax_t aLength, bool aAsync, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	if( -1 == aMapInfo.fd )
	{
		aErrorOut = compat::make_error_code( compat::errc::bad_file_descriptor );
		return false;
	}

	auto const sync = aAsync ? MS_ASYNC : MS_SYNC;
	auto const size = (~std::uintmax_t(0) != aLength) ? aLength : aMapInfo.size;

	if( -1 == msync( aAddr, size, sync ) )
	{
		aErrorOut = make_errno_error_code();
		return false;
	}

	return true;
}

std::uintmax_t file_map_view_max_size( FileMapInfo const& aMapInfo, std::uintmax_t aBegin )
{
	FLUX_THROW_IF( -1 == aMapInfo.fd, error::InvalidFileMap )
		<< einfo::ErrorDesc( "fd is -1" )
	;
	FLUX_THROW_IF( aBegin >= aMapInfo.size, error::MapOutOfRange )
		<< einfo::ErrorMeta( FLUX_FMT_STRING("size {:d}, requested offset {:d}"), aMapInfo.size, aBegin )
	;

	return aMapInfo.size - aBegin;
}
optional<std::uintmax_t> file_map_view_max_size( FileMapInfo const& aMapInfo, std::uintmax_t aBegin, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	if( -1 == aMapInfo.fd )
	{
		aErrorOut = compat::make_error_code( compat::errc::bad_file_descriptor );
		return {};
	}
	
	if( aBegin >= aMapInfo.size )
	{
		aErrorOut = compat::make_error_code( compat::errc::argument_out_of_domain );
		return {};
	}

	return aMapInfo.size - aBegin;
}

std::uintmax_t file_map_offset_minimum_alignment() noexcept
{
	return get_page_size();
}

//--    file mapping - win32                ///{{{2///////////////////////////
#else // PLATFORM_WIN32
FileMapInfo file_map_prepare( SysPathView const& aPath, EFileMapMode aMode, bool aMustExist )
{
	detail::SysPathBuf path;
	detail::make_zpath( path, aPath );
	
	HANDLE fh = ::CreateFileW(
		path.data(),
		mapmode_to_createfileflag_(aMode),
		mapmode_to_createfileshare_(aMode),
		0,
		aMustExist ? OPEN_EXISTING : OPEN_ALWAYS,
		0,
		0
	);

	FLUX_THROW_IF( INVALID_HANDLE_VALUE == fh, error::MapPrepareFailed )
		<< einfo::ApiFunction( "CreateFileW" )
		<< einfo::ApiError( make_win32_error_code() )
		<< einfo::ErrorMeta( FLUX_FMT_STRING("on '{}'"), path_to_utf8(aPath) )
	;

	FLUX_UTIL_ON_SCOPE_EXIT{ if( INVALID_HANDLE_VALUE != fh ) ::CloseHandle(fh); };
	
	LARGE_INTEGER li;
	FLUX_THROW_IF( !::GetFileSizeEx( fh, &li ), error::MapPrepareFailed )
		<< einfo::ApiFunction( "GetFileSizeEx" )
		<< einfo::ApiError( make_win32_error_code() )
		<< einfo::ErrorMeta( FLUX_FMT_STRING("on '{}'"), path_to_utf8(aPath) )
	;
	
	HANDLE mh = ::CreateFileMapping(
		fh,
		0,
		mapmode_to_protmode_(aMode),
		0, 0,
		0
	);

	FLUX_THROW_IF( INVALID_HANDLE_VALUE == mh, error::MapPrepareFailed )
		<< einfo::ApiFunction( "CreateFileMapping" )
		<< einfo::ApiError( make_win32_error_code() )
		<< einfo::ErrorMeta( FLUX_FMT_STRING("on '{}'"), path_to_utf8(aPath) )
	;

	FileMapInfo ret;
	ret.file = fh;
	ret.mapping = mh;
	ret.size = util::narrow<std::uintmax_t>( li.QuadPart );

	fh = INVALID_HANDLE_VALUE;

	return ret;
}
optional<FileMapInfo> file_map_prepare( SysPathView const& aPath, EFileMapMode aMode, bool aMustExist, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	
	detail::SysPathBuf path;
	if( !detail::make_zpath( path, aPath, 0, aErrorOut ) )
		return {};
	
	HANDLE fh = ::CreateFileW(
		path.data(),
		mapmode_to_createfileflag_(aMode),
		mapmode_to_createfileshare_(aMode),
		0,
		aMustExist ? OPEN_EXISTING : OPEN_ALWAYS,
		0,
		0
	);

	if( INVALID_HANDLE_VALUE == fh )
	{
		aErrorOut = make_win32_error_code();
		return {};
	};

	FLUX_UTIL_ON_SCOPE_EXIT{ if( INVALID_HANDLE_VALUE != fh ) ::CloseHandle(fh); };
	
	LARGE_INTEGER li;
	if( !::GetFileSizeEx( fh, &li )  )
	{
		aErrorOut = make_win32_error_code();
		return {};
	}
	
	HANDLE mh = ::CreateFileMapping(
		fh,
		0,
		mapmode_to_protmode_(aMode),
		0, 0,
		0
	);

	if( INVALID_HANDLE_VALUE == mh )
	{
		aErrorOut = make_win32_error_code();
		return {};
	}
	
	auto x = util::narrow<std::uintmax_t>( li.QuadPart, aErrorOut );
	if( !x )
		return {};
	
	FileMapInfo ret;
	ret.file = fh;
	ret.mapping = mh;
	ret.size = x.value();

	fh = INVALID_HANDLE_VALUE;
	return ret;
}

void file_map_update( FileMapInfo& aMapInfo )
{
	FLUX_THROW_IF( INVALID_HANDLE_VALUE == aMapInfo.file, error::InvalidFileMap )
		<< einfo::ErrorDesc( "file handle is invalid" )
	;

	LARGE_INTEGER li;
	FLUX_THROW_IF( !::GetFileSizeEx( aMapInfo.file, &li ), error::MapPrepareFailed )
		<< einfo::ApiFunction( "GetFileSizeEx" )
		<< einfo::ApiError( make_win32_error_code() )
	;

	aMapInfo.size = util::narrow<std::uintmax_t>( li.QuadPart );
}
bool file_map_update( FileMapInfo& aMapInfo, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	if( INVALID_HANDLE_VALUE == aMapInfo.file )
	{
		aErrorOut = compat::make_error_code( compat::errc::bad_file_descriptor );
		return false;
	}

	LARGE_INTEGER li;
	if( !::GetFileSizeEx( aMapInfo.file, &li ) )
	{
		aErrorOut = make_win32_error_code();
		return false;
	}
	
	auto x = util::narrow<std::uintmax_t>( li.QuadPart, aErrorOut );
	if( !x )
		return false;

	aMapInfo.size = x.value();
	return true;
}

void file_map_resize( FileMapInfo& aMapInfo, std::uintmax_t aSize )
{
	FLUX_THROW_IF( INVALID_HANDLE_VALUE == aMapInfo.file, error::InvalidFileMap )
		<< einfo::ErrorDesc( "invalid file handle" )
	;
	FLUX_THROW_IF( INVALID_HANDLE_VALUE == aMapInfo.mapping, error::InvalidFileMap )
		<< einfo::ErrorDesc( "invalid mapping handle" )
	;

	// Close old mapping
	::CloseHandle( aMapInfo.mapping );
	aMapInfo.mapping = INVALID_HANDLE_VALUE;

	// Recreate mapping with new size
	// XXX- we don't know the old mode. Assume that it included writing,
	// because otherwise we can't resize it anyway.
	DWORD hi, lo;
	to_dwords_( aSize, hi, lo );
	
	HANDLE mh = ::CreateFileMapping(
		aMapInfo.file,
		0,
		PAGE_READWRITE,
		hi, lo,
		0
	);

	FLUX_THROW_IF( INVALID_HANDLE_VALUE == mh, error::MapResizeFailed )
		<< einfo::ApiFunction( "CreateFileMapping" )
		<< einfo::ApiError( make_win32_error_code() )
		<< einfo::ErrorMeta( FLUX_FMT_STRING("from {:d} to {:d}"), aMapInfo.size, aSize )
	;

	aMapInfo.mapping = mh;
	aMapInfo.size = aSize;
}
bool file_map_resize( FileMapInfo& aMapInfo, std::uintmax_t aSize, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	if( INVALID_HANDLE_VALUE == aMapInfo.file || INVALID_HANDLE_VALUE == aMapInfo.mapping )
	{
		aErrorOut = compat::make_error_code( compat::errc::bad_file_descriptor );
		return false;
	}

	// Close old mapping
	::CloseHandle( aMapInfo.mapping );
	aMapInfo.mapping = INVALID_HANDLE_VALUE;

	// Recreate mapping with new size
	// XXX- we don't know the old mode. Assume that it included writing,
	// because otherwise we can't resize it anyway.
	DWORD hi, lo;
	to_dwords_( aSize, hi, lo );
	
	HANDLE mh = ::CreateFileMapping(
		aMapInfo.file,
		0,
		PAGE_READWRITE,
		hi, lo,
		0
	);

	if( INVALID_HANDLE_VALUE == mh )
	{
		aErrorOut = make_win32_error_code();
		return false;
	}

	aMapInfo.mapping = mh;
	aMapInfo.size = aSize;
	return true;
}


void file_map_destroy( FileMapInfo& aMapInfo ) noexcept
{
	if( INVALID_HANDLE_VALUE != aMapInfo.mapping )
	{
		::CloseHandle( aMapInfo.mapping );
		aMapInfo.mapping = INVALID_HANDLE_VALUE;
	}
	if( INVALID_HANDLE_VALUE != aMapInfo.file )
	{
		::CloseHandle( aMapInfo.file );
		aMapInfo.file = INVALID_HANDLE_VALUE;
	}

	aMapInfo.size = 0;
}


void* file_map_view_create( FileMapInfo const& aMapInfo, EFileMapMode aMode, std::uintmax_t aBegin, std::uintmax_t aLength )
{
	FLUX_THROW_IF( INVALID_HANDLE_VALUE == aMapInfo.file, error::InvalidFileMap )
		<< einfo::ErrorDesc( "invalid file handle" )
	;
	FLUX_THROW_IF( INVALID_HANDLE_VALUE == aMapInfo.mapping, error::InvalidFileMap )
		<< einfo::ErrorDesc( "invalid mapping handle" )
	;

	FLUX_THROW_IF( aBegin >= aMapInfo.size, error::MapOutOfRange )
		<< einfo::ErrorMeta( FLUX_FMT_STRING("size {:d}, requested at {:d}"), aMapInfo.size, aBegin )
	;
	
	auto const aligned = aBegin & ~(file_map_offset_minimum_alignment()-1);
	FLUX_THROW_IF( aligned != aBegin, error::MapBadAlignment )
		<< einfo::ErrorMeta( FLUX_FMT_STRING("minimum alignment {:d}, requested at {:d}"), file_map_offset_minimum_alignment(), aBegin )
	;

	//
	DWORD offHi, offLo;
	to_dwords_( aligned, offHi, offLo );

	auto const size = (~std::uintmax_t(0) != aLength) ? aLength : 0;

	LPVOID addr = ::MapViewOfFile(
		aMapInfo.mapping,
		mapmode_to_mapviewflags_(aMode),
		offHi, offLo,
		util::narrow<SIZE_T>(size)
	);

	FLUX_THROW_IF( !addr, error::MapError )
		<< einfo::ApiFunction( "MapViewOfFile" )
		<< einfo::ApiError( make_win32_error_code() )
		<< einfo::ErrorMeta( FLUX_FMT_STRING("range {:d}+{:d} ({})"), aligned, size, to_string(aMode) )
	;

	return addr;
}
void* file_map_view_create( FileMapInfo const& aMapInfo, EFileMapMode aMode, std::uintmax_t aBegin, std::uintmax_t aLength, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	if( INVALID_HANDLE_VALUE == aMapInfo.file || INVALID_HANDLE_VALUE == aMapInfo.mapping )
	{
		aErrorOut = compat::make_error_code( compat::errc::bad_file_descriptor );
		return false;
	}

	if( aBegin >= aMapInfo.size )
	{
		aErrorOut = compat::make_error_code( compat::errc::argument_out_of_domain );
		return nullptr;
	}

	auto const aligned = aBegin & ~(file_map_offset_minimum_alignment()-1);
	if( aligned != aBegin )
	{
		aErrorOut = compat::make_error_code( compat::errc::argument_out_of_domain );
		return nullptr;
	}

	//
	DWORD offHi, offLo;
	to_dwords_( aligned, offHi, offLo );

	auto const size = util::narrow<SIZE_T>((~std::uintmax_t(0) != aLength) ? aLength : 0, aErrorOut);
	if( !size )
		return nullptr;

	LPVOID addr = ::MapViewOfFile(
		aMapInfo.mapping,
		mapmode_to_mapviewflags_(aMode),
		offHi, offLo,
		size.value()
	);

	if( !addr )
	{
		aErrorOut = make_win32_error_code();
		return nullptr;
	}

	return addr;
}


void file_map_view_delete( FileMapInfo const& aMapInfo, void* aAddr, std::uintmax_t aLength )
{
	FLUX_THROW_IF( INVALID_HANDLE_VALUE == aMapInfo.file, error::InvalidFileMap )
		<< einfo::ErrorDesc( "invalid file handle" )
	;
	FLUX_THROW_IF( INVALID_HANDLE_VALUE == aMapInfo.mapping, error::InvalidFileMap )
		<< einfo::ErrorDesc( "invalid mapping handle" )
	;

	FLUX_THROW_IF( !::UnmapViewOfFile( aAddr ), error::MapError )
		<< einfo::ApiFunction( "MapViewOfFile" )
		<< einfo::ApiError( make_win32_error_code() )
		<< einfo::ErrorMeta( FLUX_FMT_STRING("at {:p}"), aAddr )
	;
}
bool file_map_view_delete( FileMapInfo const& aMapInfo, void* aAddr, std::uintmax_t aLength, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	if( INVALID_HANDLE_VALUE == aMapInfo.file || INVALID_HANDLE_VALUE == aMapInfo.mapping )
	{
		aErrorOut = compat::make_error_code( compat::errc::bad_file_descriptor );
		return false;
	}

	if( !::UnmapViewOfFile( aAddr ) )
	{
		aErrorOut = make_win32_error_code();
		return false;
	}
	
	return true;
}

void file_map_view_flush( FileMapInfo const& aMapInfo, void* aAddr, std::uintmax_t aLength, bool )
{
	FLUX_THROW_IF( INVALID_HANDLE_VALUE == aMapInfo.file, error::InvalidFileMap )
		<< einfo::ErrorDesc( "invalid file handle" )
	;
	FLUX_THROW_IF( INVALID_HANDLE_VALUE == aMapInfo.mapping, error::InvalidFileMap )
		<< einfo::ErrorDesc( "invalid mapping handle" )
	;

	auto const size = (~std::uintmax_t(0) != aLength)
		? util::narrow<SIZE_T>(aLength)
		: util::narrow<SIZE_T>(aMapInfo.size)
	;

	FLUX_THROW_IF( !::FlushViewOfFile( aAddr, size ), error::MapError )
		<< einfo::ApiFunction( "FlushViewOfFile" )
		<< einfo::ApiError( make_win32_error_code() )
		<< einfo::ErrorMeta( FLUX_FMT_STRING("range {:p}+{:d}"), aAddr, size )
	;
}
bool file_map_view_flush( FileMapInfo const& aMapInfo, void* aAddr, std::uintmax_t aLength, bool, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	if( INVALID_HANDLE_VALUE == aMapInfo.file || INVALID_HANDLE_VALUE == aMapInfo.mapping )
	{
		aErrorOut = compat::make_error_code( compat::errc::bad_file_descriptor );
		return false;
	}

	auto const size = (~std::uintmax_t(0) != aLength)
		? util::narrow<SIZE_T>(aLength, aErrorOut)
		: util::narrow<SIZE_T>(aMapInfo.size, aErrorOut)
	;

	if( !size )
		return false;

	if( !::FlushViewOfFile( aAddr, size.value() ) )
	{
		aErrorOut = make_win32_error_code();
		return false;
	}

	return true;
}


std::uintmax_t file_map_view_max_size( FileMapInfo const& aMapInfo, std::uintmax_t aBegin )
{
	FLUX_THROW_IF( INVALID_HANDLE_VALUE == aMapInfo.file, error::InvalidFileMap )
		<< einfo::ErrorDesc( "invalid file handle" )
	;
	FLUX_THROW_IF( INVALID_HANDLE_VALUE == aMapInfo.mapping, error::InvalidFileMap )
		<< einfo::ErrorDesc( "invalid mapping handle" )
	;

	FLUX_THROW_IF( aBegin >= aMapInfo.size, error::MapOutOfRange )
		<< einfo::ErrorMeta( FLUX_FMT_STRING("size {:d}, requested offset {:d}"), aMapInfo.size, aBegin )
	;

	return aMapInfo.size - aBegin;
}
optional<std::uintmax_t> file_map_view_max_size( FileMapInfo const& aMapInfo, std::uintmax_t aBegin, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	if( INVALID_HANDLE_VALUE == aMapInfo.file || INVALID_HANDLE_VALUE == aMapInfo.mapping )
	{
		aErrorOut = compat::make_error_code( compat::errc::bad_file_descriptor );
		return {};
	}

	if( aBegin >= aMapInfo.size )
	{
		aErrorOut = compat::make_error_code( compat::errc::argument_out_of_domain );
		return {};
	}

	return aMapInfo.size - aBegin;
}


std::uintmax_t file_map_offset_minimum_alignment() noexcept
{
	SYSTEM_INFO sysInfo;
	::GetSystemInfo( &sysInfo );

	return sysInfo.dwAllocationGranularity;
}
#endif // ~ PLATFORM

//--    to_string()                         ///{{{2///////////////////////////
std::string to_string( EFileMapMode aMapMode )
{
	switch( aMapMode )
	{
		case EFileMapMode::ro: return "EFileMapMode::ro";
		case EFileMapMode::wo: return "EFileMapMode::wo";
		case EFileMapMode::rw: return "EFileMapMode::rw";
	}

	return format( "EFileMapMode({:d})", int(aMapMode) );
}

//--    $ local - generic                   ///{{{2///////////////////////////
#if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
namespace
{
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

	int mapmode_to_mmapprot_( EFileMapMode aMode ) noexcept
	{
		int ret = 0;
		switch( aMode )
		{
			case EFileMapMode::ro: ret = PROT_READ; break;
			case EFileMapMode::wo: ret = PROT_WRITE; break;
			case EFileMapMode::rw: ret = PROT_READ|PROT_WRITE; break;
		}

		return ret;

	}
	int mapmode_to_mmapflags_( EFileMapMode aMode ) noexcept
	{
		int ret = 0;
		switch( aMode )
		{
			case EFileMapMode::ro: 
				ret = MAP_PRIVATE; break;

			case EFileMapMode::wo:
			case EFileMapMode::rw: 
				ret = MAP_SHARED;
				break;
		}

		return ret;
	}
	int mapmode_to_openflags_( EFileMapMode aMode, bool aMustExist ) noexcept
	{
		/* For mmap() to succeed, the file descriptor always needs at least
		 * read access. So, we can never open the file handle with O_WRONLY.
		 */

		int ret = 0;
		switch( aMode )
		{
			case EFileMapMode::ro: ret = O_RDONLY; break;
			case EFileMapMode::wo: ret = O_RDWR /*see above*/; break;
			case EFileMapMode::rw: ret = O_RDWR; break;
		}

		// create if it doesn't exist?
		if( !aMustExist ) 
			ret |= O_CREAT;

		return ret;
	}
}

//--    $ local - win32                     ///{{{2///////////////////////////
#else // PLATFORM_WIN32
namespace
{
	void to_dwords_( std::uint32_t aX, DWORD& aHi, DWORD& aLo ) noexcept
	{
		aHi = 0;
		aLo = aX;
	}
	void to_dwords_( std::uint64_t aX, DWORD& aHi, DWORD& aLo ) noexcept
	{
		aHi = DWORD(aX >> 32);
		aLo = DWORD(aX);
	}

	//FLUX_INTENT_UNUSED(to_dwords_);

	DWORD mapmode_to_createfileflag_( EFileMapMode aMode ) noexcept
	{
		DWORD ret = 0;
		switch( aMode )
		{
			case EFileMapMode::ro: 
				ret = GENERIC_READ; 
				break;
			case EFileMapMode::wo:
			case EFileMapMode::rw: 
				/* Note: EFileMapMode::wo also needs GENERIC_READ, otherwise
				 * certain operations (e.g., file_map_resize()) will fail.
				 */
				ret = GENERIC_READ|GENERIC_WRITE;
				break;
		}

		return ret;
	}
	DWORD mapmode_to_createfileshare_( EFileMapMode aMode ) noexcept
	{
		//TODO: Should we just FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE?
		//  Most permissive thing.
		
		/* If we're reading from the file, allow others to also read from that
		 * file. If we're writing to it, don't share access (share mode = 0).
		 */
		DWORD ret = 0;
		switch( aMode )
		{
			case EFileMapMode::ro:
				ret = FILE_SHARE_READ;
				break;
			case EFileMapMode::wo:
			case EFileMapMode::rw:
				ret = 0;
				break;
		}

		return ret;
	}
	DWORD mapmode_to_protmode_( EFileMapMode aMode ) noexcept
	{
		DWORD ret = 0;
		switch( aMode )
		{
			case EFileMapMode::ro: ret = PAGE_READONLY; break;
			case EFileMapMode::wo: ret = PAGE_READWRITE; break;
			case EFileMapMode::rw: ret = PAGE_READWRITE; break;
		}

		return ret;
	}
	DWORD mapmode_to_mapviewflags_( EFileMapMode aMode ) noexcept
	{
		DWORD ret = 0;
		switch( aMode )
		{
			case EFileMapMode::ro: ret = FILE_MAP_READ; break;
			case EFileMapMode::wo: ret = FILE_MAP_WRITE; break;
			case EFileMapMode::rw: ret = FILE_MAP_ALL_ACCESS; break;
		}

		return ret;
	}
}
#	endif // ~ PLATFORM

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
