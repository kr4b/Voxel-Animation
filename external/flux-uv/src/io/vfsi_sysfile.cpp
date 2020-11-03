/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	vfsi::SysFile implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/vfsi/sysfile.hpp>

#include <flux/io/errors.hpp>
#include <flux/io/buffer/sysfile.hpp>
#include <flux/io/stream/sysfile.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/assert.hpp>
#include <flux/std/format.hpp>

#include <flux/sys/fsquery.hpp>
#include <flux/sys/fsmanip.hpp>
#include <flux/sys/pathutil.hpp>

#include <flux/util/narrow.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io::vfsi >>>          ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(io,vfsi)
//--    SysFile                              ///{{{2///////////////////////////

	// [cd]tor -{{{3-
SysFile::SysFile( sys::SysPathStr const& aPath )
	: mSelfPath( aPath )
{
	compat::error_code ec;
	FLUX_THROW_IF( !SysFile::update( false, ec ), error::VfsError )
		<< einfo::VfsDesc( desc() )
		<< flux::einfo::ApiError( ec )
	;
}
SysFile::SysFile( sys::SysPathStr const& aSysPath, std::string aPath, weak_ptr<VINode> aAncestor )
	: VINode( std::move(aPath), std::move(aAncestor) )
	, mSelfPath( aSysPath )
{
	compat::error_code ec;
	FLUX_THROW_IF( !SysFile::update( false, ec ), error::VfsError )
		<< einfo::VfsDesc( desc() )
		<< flux::einfo::ApiError( ec )
	;
}

	// virtuals: generic -{{{3-
std::string SysFile::desc() const
{
	return format( "sysfile:{}", sys::path_to_utf8(mSelfPath) );
}

bool SysFile::update( bool, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	
	mStat = VfsStat{};
	if( auto sb = sys::stat_path( mSelfPath, aErrorOut ) )
	{
		FLUX_ASSERT( sys::EFileKind::directory != sys::stat_get_type(sb) );
		switch( sys::stat_get_type(sb) )
		{
			case sys::EFileKind::regular:
				mStat.file = 1;
				break;

			default:
				mStat.special = 1;
		}

		compat::error_code ec0, ec1;
		mStat.readable = sys::check_access( mSelfPath, sys::EPathAccess::read, ec0 ).value_or( false );
		mStat.writable = sys::check_access( mSelfPath, sys::EPathAccess::write, ec1 ).value_or( false );
		(void)ec0;
		(void)ec1;

		// XXX-verify:
		mStat.mappable = mStat.readable ||  mStat.writable;
		mStat.sized = mStat.readable;

		// XXX-technically depends on the parent directory.. :-/
		mStat.removable = mStat.writable;

		mStat.hasNativeMap = mStat.mappable;
	}
	else
	{
		mStat.file = 1;
	}

	mStat.hasNativePath = 1;
	return true;
}
bool SysFile::remove( bool, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	if( !sys::file_remove( mSelfPath, aErrorOut ) )
		return false;

	return SysFile::update( false, aErrorOut );
}

optional<sys::SysPathStr> SysFile::native_path( compat::error_code& aErrOut ) const
{
	FLUX_ASSERT( !aErrOut );
	return mSelfPath;
}

	// virtuals: file access -{{{3-
VfsSize SysFile::file_size( compat::error_code& aErrorOut ) noexcept
{
	if( auto sb = sys::stat_path( mSelfPath, aErrorOut ) )
		return util::narrow<VfsSize>(sys::stat_get_size( sb ));

	compat::error_code ec;
	(void)SysFile::update( false, ec );

	if( !aErrorOut ) aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
	return kInvalidVfsSize;
}

unique_ptr<Buffer> SysFile::file_as_buffer( EBufferMode aMode, bool aMustExist, BufferSize aResizeTo, compat::error_code& aErrorOut ) noexcept try
{
	auto x = buffer_create_sysfile( mSelfPath, aMode, aMustExist, aResizeTo );

	compat::error_code ec;
	(void)SysFile::update( false, ec );

	return x;
}
catch( error::BufferNotAvailable const& )
{
	aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
	return nullptr;
	
#	if 0
	FLUX_THROW_WITH_NESTED( error::VfsNotFound ) << einfo::VfsDesc( desc() );
#	endif
}
unique_ptr<Stream> SysFile::file_as_stream( EStreamMode aMode, compat::error_code& aErrorOut ) noexcept try
{
	auto x = stream_create_sysfile( mSelfPath, aMode );

	compat::error_code ec;
	(void)SysFile::update( false, ec );

	return x;
}
catch( error::StreamNotAvailable const& )
{
	aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
	return nullptr;

#	if 0
	FLUX_THROW_WITH_NESTED( error::VfsNotFound ) << einfo::VfsDesc( desc() );
#	endif
}

//--    <<< ~ flux::io::vfsi namespace <<<          ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(io,vfsi)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
