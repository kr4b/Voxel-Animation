/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	vfsi::LazyDir implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/vfsi/lazydir.hpp>

#include <flux/io/vpath.hpp>
#include <flux/io/errors.hpp>
#include <flux/io/vfsi/sysdir.hpp>
#include <flux/io/vfsi/sysfile.hpp>
#include <flux/io/buffer/sysfile.hpp>
#include <flux/io/stream/sysfile.hpp>

#include <flux/std/log.hpp>
#include <flux/std/throw.hpp>
#include <flux/std/assert.hpp>
#include <flux/std/format.hpp>
#include <flux/std/except/query.hpp>

#include <flux/compat/system_error.hpp>

#include <flux/sys/fsquery.hpp>
#include <flux/sys/fsmanip.hpp>
#include <flux/sys/defpath.hpp>
#include <flux/sys/pathutil.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io::vfsi >>>          ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(io,vfsi)
//--    LazyDir                             ///{{{2///////////////////////////

	// [cd]tor -{{{3-
LazyDir::LazyDir( std::string aPath, shared_ptr<VINode> aParent )
	: VINode( std::move(aPath), aParent )
	, mParent( std::move(aParent) )

{
	FLUX_THROW_IF( !mParent || !mParent->stat().directory, error::VfsNotADir )
		<< flux::einfo::ErrorDesc( "LazyDir: parent must be a directory" )
		<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("parent = {}"), mParent ? mParent->desc().c_str() : "(null)" )
	;

	compat::error_code ec;
	if( !LazyDir::update( false, ec ) )
	{
		FLUX_THROW_IF( compat::errc::not_a_directory == ec, error::VfsNotADir )
			<< flux::einfo::ApiError( ec )
			<< flux::einfo::ErrorDesc( "LazyDir: exists, but not a directory" )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("{}{{/{}}}"), mParent->desc(), mPathFromAncestor )
		;
		FLUX_THROW_IF( compat::errc::permission_denied == ec, error::VfsDenied )
			<< flux::einfo::ApiError( ec )
			<< flux::einfo::ErrorDesc( "LazyDir: parent not accessible/changeable " )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("{}{{/{}}}"), mParent->desc(), mPathFromAncestor )
		;
		FLUX_THROW( error::VfsError )
			<< flux::einfo::ApiError( ec )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("{}{{/{}}}"), mParent->desc(), mPathFromAncestor )
		;
	}
}

	// virtuals: generic -{{{3-
std::string LazyDir::desc() const
{
	return format( FLUX_FMT_STRING("{}{{/{}}}"), mParent->desc(), mPathFromAncestor );
}

bool LazyDir::update( bool, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );

	mStat = VfsStat{};

	compat::error_code ec;
	auto const stat = mParent->dir_stat( mPathFromAncestor, ec );
	if( stat )
	{
		mStat = stat.value();
		if( !mStat.directory )
		{
			aErrorOut = compat::make_error_code( compat::errc::not_a_directory );
			return false;
		}
	}
	else if( !ec )
	{
		if( !mParent->stat().changeable )
		{
			aErrorOut = compat::make_error_code( compat::errc::permission_denied );
			return false;
		}

		mStat.directory = 1;
		mStat.accessible = 1;
		mStat.listable = 1;
		mStat.changeable = 1;
		mStat.flat = 0;
		mStat.removable = 1;
		mStat.hasNativePath = 0;
	}
	else
	{
		aErrorOut = ec;
		return false;
	}

	return true;
}
bool LazyDir::remove( bool aRecursive, compat::error_code& aErrorOut ) noexcept
{
	// If the directory does not exist: "successfully" remove it.
	compat::error_code ec1;
	auto const stat = mParent->dir_stat( mPathFromAncestor, ec1 );
	if( !stat )
	{
		if( !ec1 ) return true;
		
		aErrorOut = ec1;
		return false;
	}

	// Ok, actually remove it.
	//XXX workaround: dir_remove() throws! need a version that doesn't do that.
	try
	{
		if( !mParent->dir_remove( mPathFromAncestor, aRecursive ) )
			return false;
	}
	catch( flux::error::Exception const& eErr )
	{
		// TODO: error_code support "unknown error"
		aErrorOut = flux::einfo::query_error_info<flux::einfo::ApiError>(eErr).value_or( compat::make_error_code( compat::errc::state_not_recoverable ) );
		return false;
	}
	catch( ... )
	{
		// TODO: error_code support "really unknown error"
		aErrorOut = compat::make_error_code( compat::errc::state_not_recoverable );
		return false;
	}

	return update( false, aErrorOut );
}

	// virtuals: directory access -{{{3-
optional<VfsStat> LazyDir::dir_stat( compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
{
	std::string simple;
	if( auto def = dir_stat_defaults( aPath, false, &simple, aErrorOut ) )
		return def.value();
	
	auto const path = make_path_(aPath); //XXX- allocates
	return mParent->dir_stat( path, aErrorOut );
}
shared_ptr<VINode> LazyDir::dir_open( compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
{
	std::string simple;
	if( auto def = dir_open_defaults( aPath, false, &simple, aErrorOut ) )
		return def.value();
	
	auto const path = make_path_(aPath); //XXX-allocates
	return mParent->dir_open( path, aErrorOut );
}


unique_ptr<Buffer> LazyDir::dir_open_buffer( compat::string_view const& aPath, EBufferMode aMode, bool aMustExist, BufferSize aResizeTo, compat::error_code& aErrorOut ) noexcept try
{
	std::string simple;
	if( auto def = dir_open_buffer_defaults( aPath, aMode, aMustExist, aResizeTo, false, &simple, aErrorOut ) )
		return std::move(def.value());

	if( !aMustExist && !!(EBufferMode::write & aMode) )
	{
		if( !try_make_dir_( aErrorOut ) )
			return nullptr;
	}

	return mParent->dir_open_buffer( make_path_(aPath), aMode, aMustExist, aResizeTo, aErrorOut );
}
catch( error::BufferNotAvailable const& )
{
	aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
	return nullptr;
}
unique_ptr<Stream> LazyDir::dir_open_stream( compat::string_view const& aPath, EStreamMode aMode, compat::error_code& aErrorOut ) noexcept try
{
	std::string simple;
	if( auto def = dir_open_stream_defaults( aPath, aMode, false, &simple, aErrorOut ) )
		return std::move(def.value());

	if( !!(EStreamMode::write & aMode) )
	{
		if( !try_make_dir_( aErrorOut ) )
			return nullptr;
	}

	return mParent->dir_open_stream( make_path_(aPath), aMode, aErrorOut );
}
catch( error::StreamNotAvailable const& )
{
	aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
	return nullptr;
}


void LazyDir::dir_create( compat::string_view const& aPath, bool aRecursive )
{
	std::string simple;
	if( dir_create_defaults( aPath, aRecursive, false, &simple ) )
		return;

	compat::error_code ec;
	if( !try_make_dir_( ec ) )
	{
		FLUX_THROW( error::VfsError )
			<< flux::einfo::ApiError( ec )
			<< flux::einfo::ErrorDesc( "Couldn't create directory" )
			<< einfo::VfsDesc( desc() )
		;
	}

	mParent->dir_create( make_path_(aPath), aRecursive );
}

bool LazyDir::dir_remove( compat::string_view const& aPath, bool aRecursive )
{
	std::string simple;
	if( auto def = dir_remove_defaults( aPath, aRecursive, false, &simple ) )
		return std::move(def.value());

	// Note: this will fail if LazyDir doesn't exist. That is OK, however,
	// since that implies that the directory referred to by aPath also didn't
	// exist.

	return mParent->dir_remove( make_path_(aPath), aRecursive );
}

	// private -{{{3-
std::string LazyDir::make_path_( compat::string_view const& aVfsPath )
{
	return vpath_join( mPathFromAncestor, aVfsPath );
}
bool LazyDir::try_make_dir_( compat::error_code& aErrorOut ) noexcept try
{
	compat::error_code ec;
	if( !mParent->dir_stat( mPathFromAncestor, ec ) )
	{
		if( ec )
		{
			aErrorOut = ec;
			return false;
		}

		//TODO-FIXME: need a dir_create() that doesn't throw but returns an
		//error_code!
		mParent->dir_create( mPathFromAncestor, true );

		compat::error_code ec0;
		if( !LazyDir::update( false, ec0 ) )
		{
			if( ec0 ) aErrorOut = ec0;
			else aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
			return false;
		}
	}

	return true;
}
catch( flux::error::Exception const& eErr )
{
	// TODO: error_code support "unknown error"
	aErrorOut = flux::einfo::query_error_info<flux::einfo::ApiError>(eErr).value_or( compat::make_error_code( compat::errc::state_not_recoverable ) );
	return false;
}
catch( ... )
{
	// TODO: error_code support "really unknown error"
	aErrorOut = compat::make_error_code( compat::errc::state_not_recoverable );
	return false;
}


//--    <<< ~ flux::io::vfsi namespace <<<          ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(io,vfsi)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
