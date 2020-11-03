/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Vfs implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/vfs.hpp>
#include <flux/io/vpath.hpp>
#include <flux/io/buffer.hpp>
#include <flux/io/stream.hpp>
#include <flux/io/errors.hpp>

#include <flux/io/vfsi/inode.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/format.hpp>

#include "vfs_record_factory.hxx"

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	compat::string_view adjust_( bool aAbs, compat::string_view const& aPath ) noexcept
	{
		if( !aAbs )
		{
			FLUX_ASSERT( '/' != aPath.front() );
			return aPath;
		}

		FLUX_ASSERT( '/' == aPath.front() );
		auto beg = aPath.data();
		auto end = aPath.data()+aPath.size();

		while( beg != end && '/' == *beg )
			++beg;

		return compat::string_view( beg, end-beg );
	}
}

//--    VfsRecord                           ///{{{2///////////////////////////
	
	// [cd]tor -{{{3-
VfsRecord::VfsRecord() = default;
VfsRecord::~VfsRecord() = default;

	// public -{{{3-
shared_ptr<vfsi::VINode> const& VfsRecord::self() const
{
	return mSelf;
}
shared_ptr<vfsi::VINode> const& VfsRecord::root() const
{
	return mRoot;
}

	// private -{{{3-
VfsRecord::VfsRecord( shared_ptr<vfsi::VINode> aSelf, shared_ptr<vfsi::VINode> aRoot ) noexcept
	: mSelf( std::move(aSelf) )
	, mRoot( std::move(aRoot) )
{}

//--    vfs_stat()                          ///{{{2///////////////////////////
VfsStat vfs_stat( VfsRecord const& aRec ) noexcept
{
	if( auto p = aRec.self() )
		return p->stat();
	
	return VfsStat();
}

optional<VfsStat> vfs_stat( VfsRecord const& aRec, compat::string_view const& aPath ) noexcept
{
	compat::error_code ec;
	return vfs_stat( aRec, aPath, ec );
}
optional<VfsStat> vfs_stat( VfsRecord const& aRec, compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	auto const absolute = vpath_is_absolute( aPath );
	auto& node = (absolute ? aRec.root() : aRec.self());
	FLUX_ASSERT( node );

	return node->dir_stat( adjust_(absolute,aPath), aErrorOut );
}

//--    vfs_is_*()                          ///{{{2///////////////////////////
bool vfs_is_file( VfsRecord const& aRec ) noexcept
{
	return !!vfs_stat(aRec).file;
}
bool vfs_is_dir( VfsRecord const& aRec ) noexcept
{
	return !!vfs_stat(aRec).directory;
}
bool vfs_is_special( VfsRecord const& aRec ) noexcept
{
	return !!vfs_stat(aRec).special;
}

//--    vfs_open()                          ///{{{2///////////////////////////
VfsRecord vfs_open( VfsRecord const& aRec, compat::string_view const& aPath )
{
	auto const absolute = vpath_is_absolute( aPath );
	auto& node = (absolute ? aRec.root() : aRec.self());
	FLUX_ASSERT( node );

	//TODO: skip initial / if absolute?
	compat::error_code ec;
	if( auto ee = node->dir_open( adjust_(absolute,aPath), ec ) )
		return detail::VfsRecordFactory::make( ee, aRec.root() );

	try
	{
		using compat::errc;
		FLUX_THROW_IF( errc::not_supported == ec, error::VfsOpNotSupported );
		FLUX_THROW_IF( errc::permission_denied == ec, error::VfsDenied );
		FLUX_THROW_IF( errc::no_such_file_or_directory == ec, error::VfsNotFound );
		FLUX_THROW( error::VfsError );
	}
	catch( error::VfsError& eError )
	{
		eError 
			<< einfo::VfsRecordStat( aRec.self()->stat() )
			<< einfo::VfsDesc( aRec.self()->desc() )
			<< flux::einfo::ApiError( ec )
			<< flux::einfo::ApiFunction( "VINode::dir_open" )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("path = '{}'"), aPath )
		;
		throw;
	}
}

optional<VfsRecord> vfs_open( VfsRecord const& aRec, compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
{
	auto const absolute = vpath_is_absolute( aPath );
	auto& node = (absolute ? aRec.root() : aRec.self());
	FLUX_ASSERT( node );

	//TODO: skip initial / if absolute?
	if( auto ee = node->dir_open( adjust_(absolute,aPath), aErrorOut ) )
		return detail::VfsRecordFactory::make( ee, aRec.root() );

	return {};
}

//--    vfs_open_buffer()                   ///{{{2///////////////////////////
unique_ptr<Buffer> vfs_open_buffer( VfsRecord const& aRec, EBufferMode aMode, bool aMustExist, BufferSize aResizeTo )
{
	compat::error_code ec;
	if( auto ret = aRec.self()->file_as_buffer( aMode, aMustExist, aResizeTo, ec ) )
		return ret;

	try
	{
		using compat::errc;
		FLUX_THROW_IF( errc::not_supported == ec, error::VfsOpNotSupported );
		FLUX_THROW_IF( errc::permission_denied == ec, error::VfsDenied );
		FLUX_THROW_IF( errc::no_such_file_or_directory == ec, error::VfsNotFound );
		FLUX_THROW( error::VfsError );
	}
	catch( error::VfsError& eError )
	{
		eError 
			<< einfo::VfsRecordStat( aRec.self()->stat() )
			<< einfo::VfsDesc( aRec.self()->desc() )
			<< einfo::BufferMode( aMode )
			<< flux::einfo::ApiError( ec )
		;
		throw;
	}
}

unique_ptr<Buffer> vfs_open_buffer( VfsRecord const& aRec, compat::string_view const& aPath, EBufferMode aMode, bool aMustExist, BufferSize aResizeTo )
{
	auto const absolute = vpath_is_absolute( aPath );
	auto& node = (absolute ? aRec.root() : aRec.self());
	FLUX_ASSERT( node );

	compat::error_code ec;
	if( auto ret = node->dir_open_buffer( adjust_(absolute,aPath), aMode, aMustExist, aResizeTo, ec ) )
		return ret;

	try
	{
		using compat::errc;
		FLUX_THROW_IF( errc::not_supported == ec, error::VfsOpNotSupported );
		FLUX_THROW_IF( errc::permission_denied == ec, error::VfsDenied );
		FLUX_THROW_IF( errc::no_such_file_or_directory == ec, error::VfsNotFound );
		FLUX_THROW( error::VfsError );
	}
	catch( error::VfsError& eError )
	{
		eError 
			<< einfo::VfsRecordStat( aRec.self()->stat() )
			<< einfo::VfsDesc( aRec.self()->desc() )
			<< einfo::BufferMode( aMode )
			<< flux::einfo::ApiError( ec )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("path: '{}'"), aPath )
		;
		throw;
	}
}

//--    vfs_open_stream()                   ///{{{2///////////////////////////
unique_ptr<Stream> vfs_open_stream( VfsRecord const& aRec, EStreamMode aMode )
{
	compat::error_code ec;
	if( auto ret = aRec.self()->file_as_stream( aMode, ec ) )
		return ret;

	try
	{
		using compat::errc;
		FLUX_THROW_IF( errc::not_supported == ec, error::VfsOpNotSupported );
		FLUX_THROW_IF( errc::permission_denied == ec, error::VfsDenied );
		FLUX_THROW_IF( errc::no_such_file_or_directory == ec, error::VfsNotFound );
		FLUX_THROW( error::VfsError );
	}
	catch( error::VfsError& eError )
	{
		eError 
			<< einfo::VfsRecordStat( aRec.self()->stat() )
			<< einfo::VfsDesc( aRec.self()->desc() )
			<< einfo::StreamMode( aMode )
			<< flux::einfo::ApiError( ec )
		;
		throw;
	}
}

unique_ptr<Stream> vfs_open_stream( VfsRecord const& aRec, compat::string_view const& aPath, EStreamMode aMode )
{
	auto const absolute = vpath_is_absolute( aPath );
	auto& node = (absolute ? aRec.root() : aRec.self());
	FLUX_ASSERT( node );

	compat::error_code ec;
	if( auto ret = node->dir_open_stream( adjust_(absolute,aPath), aMode, ec ) )
		return ret;

	try
	{
		using compat::errc;
		FLUX_THROW_IF( errc::not_supported == ec, error::VfsOpNotSupported );
		FLUX_THROW_IF( errc::permission_denied == ec, error::VfsDenied );
		FLUX_THROW_IF( errc::no_such_file_or_directory == ec, error::VfsNotFound );
		FLUX_THROW( error::VfsError );
	}
	catch( error::VfsError& eError )
	{
		eError 
			<< einfo::VfsRecordStat( aRec.self()->stat() )
			<< einfo::VfsDesc( aRec.self()->desc() )
			<< einfo::StreamMode( aMode )
			<< flux::einfo::ApiError( ec )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("path: '{}'"), aPath )
		;
		throw;
	}
}

//--    vfs_mkdir()                         ///{{{2///////////////////////////
void vfs_mkdir( VfsRecord const& aRec, compat::string_view const& aPath, bool aRecursive )
{
	auto const absolute = vpath_is_absolute( aPath );
	auto& node = (absolute ? aRec.root() : aRec.self());
	FLUX_ASSERT( node );

	node->dir_create( adjust_(absolute,aPath), aRecursive );
}

//--    vfs_remove()                        ///{{{2///////////////////////////
void vfs_remove( VfsRecord const& aRec, compat::string_view const& aPath, bool aRecursive )
{
	auto const absolute = vpath_is_absolute( aPath );
	auto& node = (absolute ? aRec.root() : aRec.self());
	FLUX_ASSERT( node );

	node->dir_remove( adjust_(absolute,aPath), aRecursive );
}

//--    vfs_delete_record()                 ///{{{2///////////////////////////
void vfs_delete_record( VfsRecord const& aRec, bool aRecursive )
{
	compat::error_code ec;
	if( !aRec.self()->remove( aRecursive, ec ) )
	{
		FLUX_THROW( error::VfsError )
			<< einfo::VfsDesc( aRec.self()->desc() )
			<< einfo::VfsRecordStat( aRec.self()->stat() )
			<< flux::einfo::ApiError( ec )
			<< flux::einfo::ApiFunction( "VINode::remove" )
		;
	}
}

//--    vfs_abspath()                       ///{{{2///////////////////////////
std::string vfs_abspath( VfsRecord const& aRec )
{
	return format( "/{}", aRec.self()->path( aRec.root().get() ) );
}

//--    vfs_description()                   ///{{{2///////////////////////////
std::string vfs_description( VfsRecord const& aRec )
{
	return aRec.self()->desc();
}

//--    to_string()                         ///{{{2///////////////////////////
std::string to_string( VfsStat const& aStat )
{
	return format( "VfsStat{{{:c}{:c}{:c}:{}:{}:{}{}}}",
		aStat.file ? 'f':'-',
		aStat.directory ? 'd':'-',
		aStat.special ? 's':'-',
		!aStat.file ? "" : format( "{:c}{:c}{:c}{:c}", 
			aStat.readable ? 'r':'-',
			aStat.writable ? 'w':'-',
			aStat.mappable ? 'M':'-',
			aStat.sized ? 'S':'-'
		),
		!aStat.directory ? "": format( "{:c}{:c}{:c}{:c}",
			aStat.accessible ? 'a':'-',
			aStat.listable ? 'r':'-',
			aStat.changeable ? 'w':'-',
			aStat.flat ? 'F':'-'
		),
		aStat.hasNativeMap ? "Nm":"-",
		aStat.hasNativePath ? "Np":"-"
	);
}

//--    <<< ~ flux::io namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
