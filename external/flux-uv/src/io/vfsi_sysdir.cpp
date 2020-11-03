/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	vfsi::SysDir implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/vfsi/sysdir.hpp>

#include <flux/io/vpath.hpp>
#include <flux/io/errors.hpp>
#include <flux/io/vfsi/sysfile.hpp>
#include <flux/io/buffer/sysfile.hpp>
#include <flux/io/stream/sysfile.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/assert.hpp>
#include <flux/std/format.hpp>

#include <flux/sys/errors.hpp>
#include <flux/sys/fsquery.hpp>
#include <flux/sys/fsmanip.hpp>
#include <flux/sys/pathutil.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io::vfsi >>>          ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(io,vfsi)
//--    SysDir                              ///{{{2///////////////////////////

	// [cd]tor -{{{3-
SysDir::SysDir( sys::SysPathStr const& aSysPath, ESysDirConstraints aConstraints )
	: mSelfPath( aSysPath )
	, mConstraints( aConstraints )
{
	compat::error_code ec;
	FLUX_THROW_IF( !SysDir::update( false, ec ), error::VfsError )
		<< einfo::VfsDesc( desc() )
		<< flux::einfo::ApiError( ec )
	;
		
}
SysDir::SysDir( sys::SysPathStr const& aSysPath, ESysDirConstraints aConstraints, std::string aPath, weak_ptr<VINode> aAncestor )
	: VINode( std::move(aPath), std::move(aAncestor) )
	, mSelfPath( aSysPath )
	, mConstraints( aConstraints )
{
	compat::error_code ec;
	FLUX_THROW_IF( !SysDir::update( false, ec ), error::VfsError )
		<< einfo::VfsDesc( desc() )
		<< flux::einfo::ApiError( ec )
	;
}

	// virtuals: generic -{{{3-
std::string SysDir::desc() const
{
	return format( "sysdir:{}", sys::path_to_utf8(mSelfPath) );
}

bool SysDir::update( bool, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	
	mStat = VfsStat{};
	if( auto sb = sys::stat_path( mSelfPath ) )
	{
		FLUX_ASSERT( sys::EFileKind::directory == sys::stat_get_type(sb) );

		mStat.directory = 1;
		mStat.accessible = sys::check_access( mSelfPath, sys::EPathAccess::read );
		mStat.listable = sys::check_access( mSelfPath, sys::EPathAccess::read );
		mStat.changeable = sys::check_access( mSelfPath, sys::EPathAccess::write );

		mStat.flat = 0;

		// XXX-technically depends on the parent directory.. :-/
		mStat.removable = mStat.changeable;
	}
	else
	{
		if( aErrorOut )
			return false;
		
		mStat.directory = 1;
	}

	mStat.hasNativePath = 1;
	return true;
}
bool SysDir::remove( bool aRecursive, compat::error_code& aErrorOut ) noexcept
{
	if( !sys::dir_remove( mSelfPath, aRecursive, aErrorOut ) )
		return false;

	return SysDir::update( false, aErrorOut );
}

optional<sys::SysPathStr> SysDir::native_path( compat::error_code& ) const
{
	return mSelfPath;
}

	// virtuals: directory access -{{{3-
optional<VfsStat> SysDir::dir_stat( compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
{
	bool const cont = !!(ESysDirConstraints::contain & mConstraints);

	std::string simple;
	if( auto def = dir_stat_defaults( aPath, cont, &simple, aErrorOut ) )
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
shared_ptr<VINode> SysDir::dir_open( compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
{
	bool const cont = !!(ESysDirConstraints::contain & mConstraints);

	std::string simple;
	if( auto def = dir_open_defaults( aPath, cont, &simple, aErrorOut ) )
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


unique_ptr<Buffer> SysDir::dir_open_buffer( compat::string_view const& aPath, EBufferMode aMode, bool aMustExist, BufferSize aResizeTo, compat::error_code& aErrorOut ) noexcept try
{
	bool const cont = !!(ESysDirConstraints::contain & mConstraints);
	
	std::string simple;
	if( auto def = dir_open_buffer_defaults( aPath, aMode, aMustExist, aResizeTo, cont, &simple, aErrorOut ) )
		return std::move(def.value());

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
unique_ptr<Stream> SysDir::dir_open_stream( compat::string_view const& aPath, EStreamMode aMode, compat::error_code& aErrorOut ) noexcept try
{
	bool const cont = !!(ESysDirConstraints::contain & mConstraints);

	std::string simple;
	if( auto def = dir_open_stream_defaults( aPath, aMode, cont, &simple, aErrorOut ) )
		return std::move(def.value());

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


void SysDir::dir_create( compat::string_view const& aPath, bool aRecursive ) try
{
	bool const cont = !!(ESysDirConstraints::contain & mConstraints);

	std::string simple;
	if( dir_create_defaults( aPath, aRecursive, cont, &simple ) )
		return;

	sys::dir_create( make_path_(simple), aRecursive );

	compat::error_code ec;
	if( !SysDir::update( false, ec ) )
		FLUX_THROW( error::VfsError )
			<< einfo::VfsDesc( desc() )
			<< einfo::VfsRecordStat( stat() )
			<< flux::einfo::ApiError( ec )
		;
}
catch( sys::error::FsError const& )
{
	FLUX_THROW_WITH_NESTED( error::VfsNotFound ) 
		<< einfo::VfsDesc( desc() )
		<< einfo::VfsRecordStat( stat() )
		<< flux::einfo::ErrorDesc( "Requested path does not exist" )
		<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("path = '{}'"), aPath )
	;
}

bool SysDir::dir_remove( compat::string_view const& aPath, bool aRecursive )
{
	bool const cont = !!(ESysDirConstraints::contain & mConstraints);

	std::string simple;
	if( auto def = dir_remove_defaults( aPath, aRecursive, cont, &simple ) )
		return std::move(def.value());

	auto const path = make_path_(simple);
	if( auto sb = sys::stat_path(path) )
	{
		if( sys::EFileKind::directory == sys::stat_get_type(sb) )
			sys::dir_remove( path, aRecursive );
		else
			sys::file_remove( path );

		compat::error_code ec; //TODO: do something with this
		return SysDir::update( false, ec );
	}

	return false;
}

	// private -{{{3-
sys::SysPathStr SysDir::make_path_( compat::string_view const& aVfsPath )
{
	return sys::path_lexical_join( mSelfPath, sys::path_lexical_normalize(sys::path_from_generic(aVfsPath)) );
}

//--    <<< ~ flux::io::vfsi namespace <<<          ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(io,vfsi)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
