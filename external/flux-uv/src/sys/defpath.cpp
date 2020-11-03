/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Default paths
 */
/*-***************************************************************** -}}}1- */

#include <flux/sys/defpath.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/assert.hpp>
#include <flux/std/format.hpp>
#include <flux/std/except/einfo.hpp>

#include <flux/sys/errors.hpp>
#include <flux/sys/syspath.hpp>
#include <flux/sys/pathutil.hpp>
#include <flux/sys/fsquery.hpp>
#include <flux/sys/fsmanip.hpp>
#include <flux/sys/errno_category.hpp>
#include <flux/sys/win32_category.hpp>

#include <flux/util/narrow.hpp>
#include <flux/util/switch.hpp>

#include <sstream>

#if FLUX_PLATFORM == FLUX_PLATFORM_LINUX
#	include <cwchar>

#	include <stdio.h>
#	include <unistd.h>
#	include <libgen.h>
#	include <fcntl.h>
#	include <cstring>
#	include <sys/stat.h>
#	include <sys/types.h>
#elif FLUX_PLATFORM == FLUX_PLATFORM_WIN32
#	include <io.h>
#	include <fcntl.h>
#	include <wchar.h>
#	include <flux/sys/win32/windows.h>

	// other windows headers use near/far in some cases. :-(
#	define far
#	define near
#	include <ShlObj.h>
#	include <KnownFolders.h>
#	undef near
#	undef far
#else // other platform
#	error "flux-sys [paths]: Unsupported platform / not implemented yet"
#endif  // ~ FLUX_PLATFORM

#include "pathbuf.hxx"

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	SysPathStr get_user_path_( EUserPath );
#	else // PLATFORM_WIN32
	KNOWNFOLDERID get_known_folder_id_( EUserPath );
#	endif // ~ PLATFORM
}

//--    path_get_self()                     ///{{{2///////////////////////////
SysPathStr path_get_self()
{
#	if FLUX_PLATFORM == FLUX_PLATFORM_LINUX
	/* Read the symlink at /proc/self/exe */

	detail::SysPathBuf buf;
	buf.resize( buf.capacity() );

	while( 1 )
	{
		auto const rr = ::readlink( "/proc/self/exe", buf.data(), buf.size() );
		FLUX_THROW_IF( -1 == rr, error::FsError )
			<< einfo::ApiError( make_errno_error_code() )
			<< einfo::ApiFunction( "::readlink" )
		;

		if( util::narrow<std::size_t>(rr) == buf.size() )
		{
			buf.resize( buf.size() * 2 );
		}
		else
		{
			FLUX_ASSERT( rr >= 0 && util::narrow_cast<std::size_t>(rr) < buf.size() );
			buf[rr] = '\0';
			break;
		}
	}

	return SysPathStr( buf.data() );

#	elif FLUX_PLATFORM == FLUX_PLATFORM_WIN32
	detail::SysPathBuf buf;
	buf.resize( buf.capacity() );

	while( 1 )
	{
		auto const rr = ::GetModuleFileNameW( nullptr, buf.data(), util::narrow<DWORD>(buf.size()) );
		FLUX_THROW_IF( 0 == rr, error::FsError )
			<< einfo::ApiError( make_win32_error_code() )
			<< einfo::ApiFunction( "::GetModuleFileNameW" )
		;

		if( rr == buf.size() )
		{
			buf.resize( buf.size() * 2 );
		}
		else
			break;
	}

	return SysPathStr( buf.data() );
#	else // unsupported platform
	return SysPathStr();
#	endif // ~ PLATFORM
}

//--    path_get_selfdir()                  ///{{{2///////////////////////////
SysPathStr path_get_selfdir()
{
	return path_lexical_parent( path_get_self() );
}

//--    path_get_tempdir()                  ///{{{2///////////////////////////
SysPathStr path_get_tempdir()
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	/* First, check the environment variables TMPDIR, TMP, TEMP and TEMPDIR
	 * (similar to temp_directory_path() of the Boost.Filesystem). Then fall-
	 * back to P_tmpdir and _PATH_TMP (if defined), and finally, guess "/tmp"
	 * as a last resort.
	 *
	 * __ANDROID__: use /data/local/tmp (again, see Boost.Filesytem)
	 */
	SysPathStr ret;

	if( char const* tmpdir = std::getenv( "TMPDIR" ) )
		ret = SysPathStr( tmpdir );
	else if( char const* tmpdir = std::getenv( "TMP" ) )
		ret = SysPathStr( tmpdir );
	else if( char const* tmpdir = std::getenv( "TEMP" ) )
		ret = SysPathStr( tmpdir );
	else if( char const* tmpdir = std::getenv( "TEMPDIR" ) )
		ret = SysPathStr( tmpdir );

#	if defined(P_tmpdir)
	if( ret.empty() )
		ret = SysPathStr( P_tmpdir );
#	elif defined(_PATH_TMP)
	if( ret.empty() )
		ret = SysPathStr( _PATH_TMP );
#	endif

	if( ret.empty() )
	{
#		if FLUX_PLATFORM == FLUX_PLATFORM_ANDROID
		ret = SysPathStr( "/data/local/tmp" );
#		else // !PLATFORM_ANDROID
		ret = SysPathStr( "/tmp" );
#		endif // ~ PLATFORM
	}

	if( EFileKind::directory == stat_get_type( stat_path( ret ) ) && check_access( ret, sys::EPathAccess::write ) )
		return ret;

#	else // PLATFORM_WIN32
	SysPathChar buffer[MAX_PATH+2]; // maximal return value is MAX_PATH+1!
	DWORD ret = GetTempPathW( MAX_PATH+1, buffer );

	FLUX_ASSERT( ret < MAX_PATH+2 );
	buffer[ret] = L'\0';

	SysPathStr tmp(buffer);
	if( EFileKind::directory == stat_get_type( stat_path( tmp ) ) && check_access( tmp, sys::EPathAccess::write ) )
		return tmp;
#	endif // ~ PLATFORM

	return SysPathStr();
}

//--    path_get_current()                  ///{{{2///////////////////////////
SysPathStr path_get_current()
{
	return path_make_absolute( path_from_generic( "." ) );
}

//--    path_get_userdir()                  ///{{{2///////////////////////////
SysPathStr path_get_userdir( EUserPath aPath, EPathOptions aOptions )
{
	// Special overrides
	// XXX-maybe : apply EPathOptions to these as well?
	if( EUserPath::home == aPath )
	{
		if( char const* env = std::getenv( "FLUXENV_SYS_USER_HOME" ) )
			return path_from_utf8( env );
	}
	else if( EUserPath::desktop == aPath )
	{
		if( char const* env = std::getenv( "FLUXENV_SYS_USER_DESKTOP" ) )
			return path_from_utf8( env );
	}
	
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	SysPathStr upath = get_user_path_( aPath );

	if( EPathOptions::none != aOptions && !upath.empty() )
	{
		switch( stat_get_type( sys::stat_path( upath ) ) )
		{
			case EFileKind::directory:
				break;

			case EFileKind::notFound:
				if( EPathOptions::create == aOptions )
				{
					try
					{
						dir_create( upath, true );
						break;
					}
					catch( error::SysError const& )
					{
						//TODO: log but then ignore
					}
				}

				FLUX_SWITCH_FALLTHROUGH();

			default: 
				return SysPathStr();
		}
	}

	return upath;

#	else // PLATFORM_WIN32
	DWORD flags;
	switch( aOptions )
	{
		case EPathOptions::none: 
			flags = KF_FLAG_DONT_VERIFY;
			break;

		case EPathOptions::check:
			flags = KF_FLAG_DEFAULT;
			break;

		case EPathOptions::create:
			flags = KF_FLAG_CREATE | KF_FLAG_INIT;
			break;
	}
	
	SysPathChar* res = nullptr;
	if( S_OK != ::SHGetKnownFolderPath( get_known_folder_id_( aPath ), flags, 0, &res ) )
		return SysPathStr();

	FLUX_ASSERT( res );
	FLUX_UTIL_ON_SCOPE_EXIT
	{
		CoTaskMemFree( res );
	};

	return SysPathStr(res);

#	endif // ~ PLATFORM_WIN32
}

//--    l: get_user_path_()                 ///{{{2///////////////////////////
namespace
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	SysPathStr get_user_path_( EUserPath aPath )
	{
		char const* env = nullptr;
		switch( aPath )
		{
			case EUserPath::home: 
				env = "HOME";
				break;

			case EUserPath::saves: FLUX_SWITCH_FALLTHROUGH();
			case EUserPath::data:
				env = "XDG_DATA_HOME";
				break;
			case EUserPath::cache:
				env = "XDG_CACHE_HOME";
				break;
			case EUserPath::config:
				env = "XDG_CONFIG_HOME";
				break;

			case EUserPath::screenshots: FLUX_SWITCH_FALLTHROUGH();
			case EUserPath::desktop: FLUX_SWITCH_FALLTHROUGH();
			case EUserPath::downloads:
				break;
		}

		if( char const* ret = env ? std::getenv( env ) : nullptr )
			return SysPathStr( ret );
	
		// construct default path
		char const* home = std::getenv( "HOME" );
		if( !home )
		{
			//TODO: log this, but don't error; services might run without a
			//proper environment
			return SysPathStr();
		}

		switch( aPath )
		{
			case EUserPath::home: 
				return SysPathStr( home );
				break;

			case EUserPath::saves: FLUX_SWITCH_FALLTHROUGH();
			case EUserPath::data:
				return format( "{}/.local/share", home );

			case EUserPath::cache:
				return format( "{}/.cache", home );
			case EUserPath::config:
				return format( "{}/.config", home );

			case EUserPath::screenshots:
				return format( "{}/Pictures", home );
			case EUserPath::desktop:
				return format( "{}/Desktop", home );
			case EUserPath::downloads:
				return format( "{}/Downloads", home );
		}

		return SysPathStr();
	}
#	endif // ~ PLATFORM
}

//--    l: get_known_folder_id_()           ///{{{2///////////////////////////
namespace
{
#	if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
	KNOWNFOLDERID get_known_folder_id_( EUserPath aPath )
	{
		switch( aPath )
		{
			case EUserPath::home:
				return FOLDERID_Profile;
			case EUserPath::data:
				return FOLDERID_LocalAppData;
			case EUserPath::cache:
				return FOLDERID_LocalAppDataLow;
			case EUserPath::config:
				return FOLDERID_RoamingAppData;
			case EUserPath::saves:
				return FOLDERID_SavedGames;
			case EUserPath::screenshots:
				return FOLDERID_Screenshots;
			case EUserPath::desktop:
				return FOLDERID_Desktop;
			case EUserPath::downloads:
				return FOLDERID_Downloads;
		}

		return KNOWNFOLDERID();
	}
#	endif // ~ PLATFORM_WIN32
}

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
