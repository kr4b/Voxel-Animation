/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	vfsi::OverlayDir implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/vfsi/overlay.hpp>

#include <flux/io/vpath.hpp>
#include <flux/io/buffer.hpp>
#include <flux/io/stream.hpp>
#include <flux/io/errors.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/assert.hpp>
#include <flux/std/format.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io::vfsi >>>          ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(io,vfsi)
//--    OverlayDir                          ///{{{2///////////////////////////

	// [cd]tor -{{{3-
OverlayDir::OverlayDir( shared_ptr<VINode> aBase, bool aPermitOverlayChanges )
	: VINode( aBase->path(kAncestorNode), aBase->ancestor() )
	, mBase( std::move(aBase) )
	, mPermitOverlayChanges( aPermitOverlayChanges )
{
	FLUX_ASSERT( mBase && mBase->stat().directory );

	compat::error_code ec;
	FLUX_THROW_IF( !OverlayDir::update( false, ec ), error::VfsError )
		<< einfo::VfsDesc( desc() )
		<< flux::einfo::ApiError( ec )
	;
}

	// public -{{{3-
void OverlayDir::add( shared_ptr<VINode> aOverlay )
{
	FLUX_ASSERT( aOverlay );
	mOverlay.insert( std::make_pair(aOverlay->name(),aOverlay) );
}
void OverlayDir::remove( shared_ptr<VINode> aOverlay )
{
	FLUX_ASSERT( aOverlay );

	auto it = mOverlay.find( aOverlay->name() );
	if( mOverlay.end() != it )
		mOverlay.erase( it );
}
void OverlayDir::remove( compat::string_view const& aName )
{
	auto it = mOverlay.find( aName );
	if( mOverlay.end() != it )
		mOverlay.erase( it );
}

	// virtuals: generic -{{{3-
std::string OverlayDir::desc() const
{
	return format( "overlay-on-{}", mBase->desc() );
}

bool OverlayDir::update( bool aRecursive, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	if( !mBase->update( aRecursive, aErrorOut ) )
		return false;

	if( aRecursive )
	{
		for( auto& oe : mOverlay )
		{
			if( !oe.second->update( true, aErrorOut ) )
				return false;
		}
	}
	
	mStat = mBase->stat();
	mStat.removable = (mStat.removable && mPermitOverlayChanges);
	return true;
}
bool OverlayDir::remove( bool aRecursive, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	if( !mPermitOverlayChanges )
	{
		aErrorOut = compat::make_error_code( compat::errc::permission_denied );
		return false;
	}
	
	if( !mBase->remove( aRecursive, aErrorOut ) )
		return false;
		
	mOverlay.clear();
	return OverlayDir::update( false, aErrorOut );
}

	// virtuals: directory access -{{{3-
optional<VfsStat> OverlayDir::dir_stat( compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
{
	std::string simple;
	if( auto def = dir_stat_defaults( aPath, false, &simple, aErrorOut ) )
		return def.value();

	auto const parts = vpath_split_first(simple);
	auto const it = mOverlay.find( std::get<0>(parts) );
	if( mOverlay.end() != it )
	{
		if( std::get<1>(parts).empty() ) return it->second->stat();
		else return it->second->dir_stat( std::get<1>(parts), aErrorOut );
	}

	return mBase->dir_stat( simple, aErrorOut );
}
shared_ptr<VINode> OverlayDir::dir_open( compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
{
	std::string simple;
	if( auto def = dir_open_defaults( aPath, false, &simple, aErrorOut ) )
		return def.value();

	auto const parts = vpath_split_first(simple);
	auto const it = mOverlay.find( std::get<0>(parts) );
	if( mOverlay.end() != it )
	{
		if( std::get<1>(parts).empty() ) return it->second;
		else return it->second->dir_open( std::get<1>(parts), aErrorOut );
	}

	return mBase->dir_open( simple, aErrorOut );
}

unique_ptr<Buffer> OverlayDir::dir_open_buffer( compat::string_view const& aPath, EBufferMode aMode, bool aMustExist, BufferSize aResizeTo, compat::error_code& aErrorOut ) noexcept
{
	std::string simple;
	if( auto def = dir_open_buffer_defaults( aPath, aMode, aMustExist, aResizeTo, false, &simple, aErrorOut ) )
		return std::move(def.value());

	auto const parts = vpath_split_first(simple);
	auto const it = mOverlay.find( std::get<0>(parts) );
	if( mOverlay.end() != it )
	{
		if( std::get<1>(parts).empty() ) return it->second->file_as_buffer( aMode, aMustExist, aResizeTo, aErrorOut );
		else return it->second->dir_open_buffer( std::get<1>(parts), aMode, aMustExist, aResizeTo, aErrorOut );
	}

	return mBase->dir_open_buffer( simple, aMode, aMustExist, aResizeTo, aErrorOut );
}
unique_ptr<Stream> OverlayDir::dir_open_stream( compat::string_view const& aPath, EStreamMode aMode, compat::error_code& aErrorOut ) noexcept
{
	std::string simple;
	if( auto def = dir_open_stream_defaults( aPath, aMode, false, &simple, aErrorOut ) )
		return std::move(def.value());

	auto const parts = vpath_split_first(simple);
	auto const it = mOverlay.find( std::get<0>(parts) );
	if( mOverlay.end() != it )
	{
		if( std::get<1>(parts).empty() ) return it->second->file_as_stream( aMode, aErrorOut );
		else return it->second->dir_open_stream( std::get<1>(parts), aMode, aErrorOut );
	}

	return mBase->dir_open_stream( simple, aMode, aErrorOut );
}

void OverlayDir::dir_create( compat::string_view const& aPath, bool aRecursive )
{
	std::string simple;
	if( dir_create_defaults( aPath, aRecursive, false, &simple ) )
		return;

	auto const parts = vpath_split_first(simple);
	auto const it = mOverlay.find( std::get<0>(parts) );
	if( mOverlay.end() != it )
	{
		FLUX_THROW_IF( !it->second->stat().directory, error::VfsNotADir )
			<< einfo::VfsDesc( desc() )
			<< einfo::VfsRecordStat( stat() )
			<< flux::einfo::ErrorDesc( "Target already exists and is not a directory" )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("{}"), std::get<0>(parts) )
		;

		if( std::get<1>(parts).empty() ) return;
		else return it->second->dir_create( std::get<1>(parts), aRecursive );
	}

	return mBase->dir_create( simple, aRecursive );
}

bool OverlayDir::dir_remove( compat::string_view const& aPath, bool aRecursive )
{
	std::string simple;
	if( auto def = dir_remove_defaults( aPath, aRecursive, false, &simple ) )
		return def.value();

	auto const parts = vpath_split_first(simple);
	auto const it = mOverlay.find( std::get<0>(parts) );
	if( mOverlay.end() != it )
	{
		if( std::get<1>(parts).empty() )
		{
			FLUX_THROW_IF( !mPermitOverlayChanges, error::VfsDenied )
				<< einfo::VfsDesc( desc() )
				<< einfo::VfsRecordStat( stat() )
				<< flux::einfo::ErrorDesc( "OverlayDir: changes not permitted" )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("removing '{}'"), std::get<0>(parts) )
			;

			mOverlay.erase( it );
			return true;
		}
		
		return it->second->dir_remove( std::get<1>(parts), aRecursive );
	}

	return mBase->dir_remove( simple, aRecursive );
}

//--    <<< ~ flux::io::vfsi namespace <<<          ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(io,vfsi)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
