/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	VINode implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/vfsi/inode.hpp>

#include <flux/io/vpath.hpp>
#include <flux/io/buffer.hpp>
#include <flux/io/stream.hpp>
#include <flux/io/errors.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/assert.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io::vfsi >>>          ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(io,vfsi)
//--    constants                           ///{{{2///////////////////////////
namespace
{
	struct Ancestor_ final : VINode
	{
		std::string desc() const override
		{
			return "magic-ancestor-id-node";
		}
	};

	Ancestor_ kMagicAncestor_;
}

VINode const* const kAncestorNode = &kMagicAncestor_;


//--    VINode                              ///{{{2///////////////////////////

	// [cd]tor -{{{3-
VINode::VINode() noexcept /* = default // GCC 4.9 barfs on this with noexcept */
{}

VINode::VINode( std::string aPathFromAncestor, weak_ptr<VINode> aAncestor, VfsStat const& aStat ) noexcept
	: mStat(aStat)
	, mAncestor(std::move(aAncestor))
	, mPathFromAncestor(std::move(aPathFromAncestor))
{}
/*VINode::VINode( VfsStat const& aStat, std::string aPathFromAncestor, shared_ptr<VINode> const& aAncestor ) noexcept
	: mStat(aStat)
	, mAncestor(aAncestor)
	, mPathFromAncestor(std::move(aPathFromAncestor))
{}*/

VINode::~VINode() = default;

	// public -{{{3-
VfsStat VINode::stat() const noexcept
{
	return mStat;
}

std::string VINode::path( VINode const* aBase ) const
{
	if( kAncestorNode == aBase )
		return mPathFromAncestor;
	
	if( auto an = mAncestor.lock() )
	{
		if( aBase == an.get() ) return mPathFromAncestor;
		else return vpath_join( an->path(aBase), mPathFromAncestor );
	}

	if( this == aBase )
		return ".";
	
	return {};
}
compat::string_view VINode::name() const noexcept
{
	return vpath_name( mPathFromAncestor );
}

weak_ptr<VINode> const& VINode::ancestor() const noexcept
{
	return mAncestor;
}

	// virtuals: generic -{{{3-
bool VINode::update( bool, compat::error_code& ) noexcept
{
	return true;
}
bool VINode::remove( bool, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !mStat.removable );

	FLUX_ASSERT( !aErrorOut );
	aErrorOut = compat::make_error_code( compat::errc::not_supported );
	return false;
}

optional<sys::SysPathStr> VINode::native_path( compat::error_code& aErrorOut ) const
{
	FLUX_ASSERT( !mStat.hasNativePath );

	FLUX_ASSERT( !aErrorOut );
	aErrorOut = compat::make_error_code( compat::errc::not_supported );
	return {};
}

	// virtuals: file access -{{{3-
VfsSize VINode::file_size( compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !mStat.sized );

	FLUX_ASSERT( !aErrorOut );
	aErrorOut = compat::make_error_code( compat::errc::not_supported );
	return kInvalidVfsSize;
}

unique_ptr<Buffer> VINode::file_as_buffer( EBufferMode, bool, BufferSize, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !mStat.mappable );

	aErrorOut = compat::make_error_code( compat::errc::not_supported );
	return nullptr;

#	if 0
	FLUX_THROW( error::VfsOpNotSupported )
		<< einfo::VfsDesc( desc() )
		<< einfo::VfsRecordStat( stat() )
		<< flux::einfo::ErrorMeta( "file_as_buffer" )
	;
#	endif
}
unique_ptr<Stream> VINode::file_as_stream( EStreamMode, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !mStat.file );

	aErrorOut = compat::make_error_code( compat::errc::not_supported );
	return nullptr;

#	if 0
	FLUX_THROW( error::VfsOpNotSupported )
		<< einfo::VfsDesc( desc() )
		<< einfo::VfsRecordStat( stat() )
		<< flux::einfo::ErrorMeta( "file_as_stream" )
	;
#	endif
}

	// virtuals: directory access -{{{3-
optional<VfsStat> VINode::dir_stat( compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
{
	if( auto n = dir_open( aPath, aErrorOut ) )
		return n->stat();

	return {};
}
shared_ptr<VINode> VINode::dir_open( compat::string_view const&, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !mStat.directory );
	aErrorOut = compat::make_error_code( compat::errc::not_supported );
	return nullptr;
}

unique_ptr<Buffer> VINode::dir_open_buffer( compat::string_view const& aPath, EBufferMode aMode, bool aMustExist, BufferSize aResizeTo, compat::error_code& aErrorOut ) noexcept
{
	compat::error_code ec;
	if( auto n = dir_open( aPath, ec ) )
		return n->file_as_buffer( aMode, aMustExist, aResizeTo, aErrorOut );

	aErrorOut = ec;
	return nullptr;

#	if 0
	FLUX_THROW( error::VfsError )
		<< einfo::VfsDesc( desc() )
		<< einfo::VfsRecordStat( stat() )
		<< flux::einfo::ApiError( ec )
		<< flux::einfo::ApiFunction( "VINode::dir_open" )
	;
#	endif
}
unique_ptr<Stream> VINode::dir_open_stream( compat::string_view const& aPath, EStreamMode aMode, compat::error_code& aErrorOut ) noexcept
{
	compat::error_code ec;
	if( auto n = dir_open( aPath, ec ) )
		return n->file_as_stream( aMode, aErrorOut );

	aErrorOut = ec;
	return nullptr;

#	if 0
	FLUX_THROW( error::VfsError )
		<< einfo::VfsDesc( desc() )
		<< einfo::VfsRecordStat( stat() )
		<< flux::einfo::ApiError( ec )
		<< flux::einfo::ApiFunction( "VINode::dir_open" )
	;
#	endif
}

void VINode::dir_create( compat::string_view const&, bool )
{
	FLUX_ASSERT( !mStat.changeable );
	FLUX_THROW( error::VfsOpNotSupported )
		<< einfo::VfsDesc( desc() )
		<< einfo::VfsRecordStat( stat() )
		<< flux::einfo::ErrorMeta( "dir_create" )
	;
}
bool VINode::dir_remove( compat::string_view const&, bool )
{
	FLUX_ASSERT( !mStat.changeable );
	FLUX_THROW( error::VfsOpNotSupported )
		<< einfo::VfsDesc( desc() )
		<< einfo::VfsRecordStat( stat() )
		<< flux::einfo::ErrorMeta( "dir_remove" )
	;
}

	// protected -{{{3-
optional<optional<VfsStat>> VINode::dir_stat_defaults( compat::string_view const& aPath, bool aContain, std::string* aSimpleOut, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );

	std::string simple;
	if( vpath_escapes( aPath, &simple ) )
	{
		if( aContain )
		{
			aErrorOut = compat::make_error_code( compat::errc::permission_denied );
			return optional<VfsStat>();
		}
		
		if( auto ap = mAncestor.lock() )
			return ap->dir_stat( vpath_join( mPathFromAncestor, simple ), aErrorOut );
	}

	if( "." == simple )
		return optional<VfsStat>(stat());

	if( aSimpleOut )
		*aSimpleOut = std::move(simple);

	return {};
}
optional<shared_ptr<VINode>> VINode::dir_open_defaults( compat::string_view const& aPath, bool aContain, std::string* aSimpleOut, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );

	std::string simple;
	if( vpath_escapes( aPath, &simple ) )
	{
		if( aContain )
		{
			aErrorOut = compat::make_error_code( compat::errc::permission_denied );
			return shared_ptr<VINode>();
		}
		
		if( auto ap = mAncestor.lock() )
			return ap->dir_open( vpath_join( mPathFromAncestor, simple ), aErrorOut );
	}

	if( "." == simple )
		return shared_from_this();

	if( aSimpleOut )
		*aSimpleOut = std::move(simple);

	return {};
}
optional<unique_ptr<Buffer>> VINode::dir_open_buffer_defaults( compat::string_view const& aPath, EBufferMode aMode, bool aMustExist, BufferSize aResizeTo, bool aContain, std::string* aSimpleOut, compat::error_code& aErrorOut ) noexcept
{
	std::string simple;
	if( vpath_escapes( aPath, &simple ) )
	{
		if( aContain )
		{
			aErrorOut = compat::make_error_code( compat::errc::permission_denied );
			return unique_ptr<Buffer>();
		}
		
#		if 0
		FLUX_THROW_IF( aContain, error::VfsDenied )
			<< einfo::VfsDesc( desc() )
			<< einfo::VfsRecordStat( stat() )
			<< flux::einfo::ErrorDesc( "Path may not escape this directory" )
			<< flux::einfo::ErrorMeta( "path = '{}'", simple )
		;
#		endif

		if( auto ap = mAncestor.lock() )
			return ap->dir_open_buffer( vpath_join( mPathFromAncestor, simple ), aMode, aMustExist, aResizeTo, aErrorOut );
	}

	// Defer to the underlying VINode implementation, and let it decide whether
	// or not this makes sense (it could for e.g. a .zip archive or similar).
	//if( "." == simple )
	//	return {};

	if( aSimpleOut )
		*aSimpleOut = std::move(simple);

	return {};
}
optional<unique_ptr<Stream>> VINode::dir_open_stream_defaults( compat::string_view const& aPath, EStreamMode aMode, bool aContain, std::string* aSimpleOut, compat::error_code& aErrorOut ) noexcept
{
	std::string simple;
	if( vpath_escapes( aPath, &simple ) )
	{
		if( aContain )
		{
			aErrorOut = compat::make_error_code( compat::errc::permission_denied );
			return unique_ptr<Stream>();
		}
		
#		if 0
		FLUX_THROW_IF( aContain, error::VfsDenied )
			<< einfo::VfsDesc( desc() )
			<< einfo::VfsRecordStat( stat() )
			<< flux::einfo::ErrorDesc( "Path may not escape this directory" )
			<< flux::einfo::ErrorMeta( "path = '{}'", simple )
		;
#		endif

		if( auto ap = mAncestor.lock() )
			return ap->dir_open_stream( vpath_join( mPathFromAncestor, simple ), aMode, aErrorOut );
	}

	// See above: open_buffer_defaults()
	//if( "." == simple )
	//	return {};

	if( aSimpleOut )
		*aSimpleOut = std::move(simple);

	return {};
}

bool VINode::dir_create_defaults( compat::string_view const& aPath, bool aRecursive, bool aContain, std::string* aSimpleOut )
{
	std::string simple;
	if( vpath_escapes( aPath, &simple ) )
	{
		FLUX_THROW_IF( aContain, error::VfsDenied )
			<< einfo::VfsDesc( desc() )
			<< einfo::VfsRecordStat( stat() )
			<< flux::einfo::ErrorDesc( "Path may not escape this directory" )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("path = '{}'"), simple )
		;

		if( !aRecursive )
			return true;
		
		if( auto ap = mAncestor.lock() )
		{
			ap->dir_create( vpath_join( mPathFromAncestor, simple ), aRecursive );
			return true;
		}
	}

	if( "." == simple )
		return true;

	if( aSimpleOut )
		*aSimpleOut = std::move(simple);

	return false;
}
optional<bool> VINode::dir_remove_defaults( compat::string_view const& aPath, bool aRecursive, bool aContain, std::string* aSimpleOut )
{
	std::string simple;
	if( vpath_escapes( aPath, &simple ) )
	{
		FLUX_THROW_IF( aContain, error::VfsDenied )
			<< einfo::VfsDesc( desc() )
			<< einfo::VfsRecordStat( stat() )
			<< flux::einfo::ErrorDesc( "Path may not escape this directory" )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("path = '{}'"), simple )
		;

		if( auto ap = mAncestor.lock() )
			return ap->dir_remove( vpath_join( mPathFromAncestor, simple ), aRecursive );
	}

	if( "." == simple )
	{
		compat::error_code ec; //TODO TODO do something with this.
		return remove( aRecursive, ec );
	}

	if( aSimpleOut )
		*aSimpleOut = std::move(simple);

	return {};
}

//--    <<< ~ flux::io::vfsi namespace <<<          ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(io,vfsi)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
