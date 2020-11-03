/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	vfsi::UserDir implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/vfsi/userdir.hpp>

#include <flux/io/errors.hpp>
#include <flux/io/vfsi/sysdir.hpp>
#include <flux/io/vfsi/sysfile.hpp>
#include <flux/io/buffer/sysfile.hpp>
#include <flux/io/stream/sysfile.hpp>

#include <flux/std/log.hpp>
#include <flux/std/throw.hpp>
#include <flux/std/assert.hpp>
#include <flux/std/format.hpp>

#include <flux/compat/system_error.hpp>

#include <flux/sys/fsquery.hpp>
#include <flux/sys/fsmanip.hpp>
#include <flux/sys/defpath.hpp>
#include <flux/sys/pathutil.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io::vfsi >>>          ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(io,vfsi)
//--    UserDir                             ///{{{2///////////////////////////

	// [cd]tor -{{{3-
UserDir::UserDir( sys::EUserPath aUserPath, std::string aPath, weak_ptr<VINode> aAncestor )
	: VINode( std::move(aPath), std::move(aAncestor) )
	, mUserPath( aUserPath )
{
	compat::error_code ec;
	FLUX_THROW_IF( !UserDir::update( false, ec ), error::VfsError )
		<< flux::einfo::ApiError( ec )
	;
}

	// virtuals: generic -{{{3-
std::string UserDir::desc() const
{
	return format( "userdir:{}:{}", dirname_(), sys::path_to_utf8(sys::path_get_userdir(mUserPath,sys::EPathOptions::none)).c_str() );
}

bool UserDir::update( bool, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );

	// Assume that the userdirs are accessible, listable and changeable.
	// Mark them as non-removable ... we shouldn't be doing that.
	mStat = VfsStat{};
	mStat.directory = 1;
	mStat.accessible = 1;
	mStat.listable = 1;
	mStat.changeable = 1;
	mStat.flat = 0;
	mStat.removable = 0;
	mStat.hasNativePath = 1;

	// If the directory exists, verify these assumptions
	auto const path = sys::path_get_userdir(mUserPath,sys::EPathOptions::none);
	if( auto sb = sys::stat_path( path ) )
	{
		FLUX_ASSERT( sys::EFileKind::directory == sys::stat_get_type(sb) );

		if( !sys::check_access( path, sys::EPathAccess::execute ) )
		{
			FLUX_LOG( WARNING, "Userdir ({}) permissions: not accessible", dirname_() );
			mStat.accessible = 0;
		}
		if( !sys::check_access( path, sys::EPathAccess::read ) )
		{
			FLUX_LOG( WARNING, "Userdir ({}) permissions: not listable", dirname_() );
			mStat.listable = 0;
		}
		if( !sys::check_access( path, sys::EPathAccess::write ) )
		{
			FLUX_LOG( WARNING, "Userdir ({}) permissions: not changeable", dirname_() );
			mStat.changeable = 0;
		}
	}

	/* If the directory doesn't exist yet, that's OK. We'll try to create it
	 * on-the-fly.
	 */
	
	return true;
}
bool UserDir::remove( bool /*aRecursive*/, compat::error_code& aErrorOut ) noexcept
{
	aErrorOut = compat::make_error_code( compat::errc::not_supported );
	return false;
}

optional<sys::SysPathStr> UserDir::native_path( compat::error_code& ) const
{
	return sys::path_get_userdir(mUserPath,sys::EPathOptions::none);
}

	// virtuals: directory access -{{{3-
optional<VfsStat> UserDir::dir_stat( compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
{
	std::string simple;
	if( auto def = dir_stat_defaults( aPath, false, &simple, aErrorOut ) )
		return def.value();
	
	auto const path = make_path_(simple); //XXX- allocates
	if( auto sb = sys::stat_path(path, aErrorOut) )
	{
		VfsStat ret;
		switch( sys::stat_get_type(sb) )
		{
			case sys::EFileKind::regular:
			{
				compat::error_code ec0, ec1;
				auto rr = sys::check_access( path, sys::EPathAccess::read, ec0 );
				auto rw = sys::check_access( path, sys::EPathAccess::write, ec1 );
			
				ret.file = 1;
				ret.readable = rr.value_or( false );
				ret.writable = rw.value_or( false );
				ret.mappable = ret.readable || ret.writable;
				ret.sized = 1;
				ret.hasNativeMap = 1;
				break;
			}

			case sys::EFileKind::directory:
			{
				compat::error_code ec0, ec1, ec2;
				auto dr = sys::check_access( path, sys::EPathAccess::read, ec0 );
				auto dw = sys::check_access( path, sys::EPathAccess::write, ec1 );
				auto dx = sys::check_access( path, sys::EPathAccess::execute, ec2 );

				ret.directory = 1;
				ret.accessible = dx.value_or( false );
				ret.listable = dr.value_or( false );
				ret.changeable = dw.value_or( false );
				ret.flat = 0;
				break;
			}

			default:
			{
				compat::error_code ec0, ec1;
				auto sr = sys::check_access( path, sys::EPathAccess::read, ec0 );
				auto sw = sys::check_access( path, sys::EPathAccess::write, ec1 );

				ret.special = 1;
				ret.readable = sr.value_or( false );
				ret.writable = sw.value_or( false );
				break;
			}
		}
		
		ret.hasNativePath = 1;
		return ret;
	}

	return {};
}
shared_ptr<VINode> UserDir::dir_open( compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
{
	std::string simple;
	if( auto def = dir_open_defaults( aPath, false, &simple, aErrorOut ) )
		return def.value();
	
	auto const path = make_path_(simple); //XXX-allocates
	if( auto sb = sys::stat_path(path, aErrorOut) )
	{
		if( sys::EFileKind::directory == sys::stat_get_type(sb) )
			return make_shared<SysDir>( path, ESysDirConstraints::none, std::string(aPath), shared_from_this() ); //XXX-allocates
		else
			return make_shared<SysFile>( path, std::string(aPath), shared_from_this() ); //XXX-allocates
	}

	if( !aErrorOut )
		aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );

	return nullptr;
}


unique_ptr<Buffer> UserDir::dir_open_buffer( compat::string_view const& aPath, EBufferMode aMode, bool aMustExist, BufferSize aResizeTo, compat::error_code& aErrorOut ) noexcept try
{
	std::string simple;
	if( auto def = dir_open_buffer_defaults( aPath, aMode, aMustExist, aResizeTo, false, &simple, aErrorOut ) )
		return std::move(def.value());

	if( !aMustExist && !!(EBufferMode::write & aMode) )
		sys::path_get_userdir( mUserPath, sys::EPathOptions::create );

	return buffer_create_sysfile( make_path_(simple), aMode, aMustExist, aResizeTo );
}
catch( error::BufferNotAvailable const& )
{
	aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
	return nullptr;

#	if 0
	FLUX_THROW_WITH_NESTED( error::VfsNotFound ) 
		<< einfo::VfsDesc( desc() )
		<< flux::einfo::ErrorDesc( "Requested path does not exist" )
		<< flux::einfo::ErrorMeta( "path = '{}'", aPath )
	;
#	endif
}
unique_ptr<Stream> UserDir::dir_open_stream( compat::string_view const& aPath, EStreamMode aMode, compat::error_code& aErrorOut ) noexcept try
{
	std::string simple;
	if( auto def = dir_open_stream_defaults( aPath, aMode, false, &simple, aErrorOut ) )
		return std::move(def.value());

	if( !!(EStreamMode::write & aMode) )
		sys::path_get_userdir( mUserPath, sys::EPathOptions::create );

	return stream_create_sysfile( make_path_(simple), aMode );
}
catch( error::StreamNotAvailable const& )
{
	aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
	return nullptr;

#	if 0
	FLUX_THROW_WITH_NESTED( error::VfsNotFound ) 
		<< einfo::VfsDesc( desc() )
		<< einfo::VfsRecordStat( stat() )
		<< flux::einfo::ErrorDesc( "Requested path does not exist" )
		<< flux::einfo::ErrorMeta( "path = '{}'", aPath )
	;
#	endif
}


void UserDir::dir_create( compat::string_view const& aPath, bool aRecursive )
{
	std::string simple;
	if( dir_create_defaults( aPath, aRecursive, false, &simple ) )
		return;

	sys::path_get_userdir( mUserPath, sys::EPathOptions::create );

	compat::error_code ec0;
	if( !sys::dir_create( make_path_(simple), aRecursive, ec0 ) )
	{
		FLUX_THROW( error::VfsError )
			<< einfo::VfsDesc( desc() )
			<< einfo::VfsRecordStat( stat() )
			<< flux::einfo::ApiError( ec0 )
			<< flux::einfo::ApiFunction( "sys::dir_create" )
		;
	}

	compat::error_code ec1;
	if( !UserDir::update( false, ec1 ) )
	{
		FLUX_THROW( error::VfsError )
			<< einfo::VfsDesc( desc() )
			<< einfo::VfsRecordStat( stat() )
			<< flux::einfo::ApiError( ec1 )
		;
	}
}

bool UserDir::dir_remove( compat::string_view const& aPath, bool aRecursive )
{
	std::string simple;
	if( auto def = dir_remove_defaults( aPath, aRecursive, false, &simple ) )
		return std::move(def.value());

	auto const path = make_path_(simple);
	if( auto sb = sys::stat_path(path) )
	{
		if( sys::EFileKind::directory == sys::stat_get_type(sb) )
			sys::dir_remove( path, aRecursive );
		else
			sys::file_remove( path );

		compat::error_code ec; //TODO: do something with this
		return UserDir::update( false, ec );
	}

	return false;
}

	// private -{{{3-
std::string UserDir::dirname_() const
{
	switch( mUserPath )
	{
		case sys::EUserPath::home: return "home";
		case sys::EUserPath::data: return "appdata";
		case sys::EUserPath::cache: return "cache";
		case sys::EUserPath::config: return "config";
		case sys::EUserPath::saves: return "saves";
		case sys::EUserPath::screenshots: return "screenshots";
		case sys::EUserPath::desktop: return "desktop";
		case sys::EUserPath::downloads: return "downloads";
	}

	return format( "sys::EUserPath({:d})", int(mUserPath) );
}
sys::SysPathStr UserDir::make_path_( compat::string_view const& aVfsPath )
{
	return sys::path_lexical_join( sys::path_get_userdir(mUserPath,sys::EPathOptions::none), sys::path_lexical_normalize(sys::path_from_generic(aVfsPath)) );
}

//--    <<< ~ flux::io::vfsi namespace <<<          ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(io,vfsi)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
