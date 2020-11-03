/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	vfsi::VDir implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/vfsi/vdir.hpp>
#include <flux/io/vfsi/masquerade.hpp>

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
//--    VDir                                ///{{{2///////////////////////////

	// [cd]tor -{{{3-
VDir::VDir( std::string aPathFromAncestor, weak_ptr<VINode> aAncestor, bool aReadOnly )
	: VINode( std::move(aPathFromAncestor), std::move(aAncestor) )
	, mReadOnly( aReadOnly )
{
	mStat.directory = 1;
	mStat.accessible = 1;
	mStat.listable = 1;
	mStat.changeable = !aReadOnly;
	mStat.flat = 0;
	
	//XXX-technically a property of the parent directory...
	mStat.removable = !aReadOnly;
}

	// public -{{{3-
void VDir::add( shared_ptr<VINode> aEntry )
{
	FLUX_ASSERT( aEntry );
	mDir.insert( std::make_pair(aEntry->name(),aEntry) );
}
void VDir::add( shared_ptr<VINode> aEntry, compat::string_view const& aName )
{
	FLUX_ASSERT( aEntry );

	auto masq = inode_masquerade( std::move(aEntry), std::string(aName), shared_from_this() );
	mDir.insert( std::make_pair(masq->name(),masq) );
}

void VDir::remove( shared_ptr<VINode> aEntry )
{
	FLUX_ASSERT( aEntry );

	auto it = mDir.find( aEntry->name() );
	if( mDir.end() != it )
		mDir.erase( it );
}
void VDir::remove( compat::string_view const& aName )
{
	auto it = mDir.find( aName );
	if( mDir.end() != it )
		mDir.erase( it );
}

	// virtuals: generic -{{{3-
std::string VDir::desc() const
{
	return format( "vdir:{}", name() );
}

bool VDir::update( bool aRecursive, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	if( aRecursive )
	{
		for( auto& oe : mDir )
		{
			if( !oe.second->update( true, aErrorOut )) 
				return false;
		}
	}

	return true;
}
bool VDir::remove( bool aRecursive, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	if( mReadOnly )
	{
		aErrorOut = compat::make_error_code( compat::errc::permission_denied );
		return false;
	}
	
	if( aRecursive )
	{
		// This is kinda dangerous... Considering the first and main use
		// of the VDir is to create the "@flux" overlay that contains 
		// stuff like "@flux/user/home" and other fun.
		//
		// So... for now:
#		if 0
		for( auto& oe : mDir )
		{
			if( !oe.second->remove( true, aErrorOut ) )
				return false;
		}
#		endif

		aErrorOut = compat::make_error_code( compat::errc::operation_not_supported );
		return false;
	}

	mDir.clear();

	//TODO: we can't actually remove ourselves from the parent directory 
	// at this point in time. Hmm.
	
	return true;
}

	// virtuals: directory access -{{{3-
optional<VfsStat> VDir::dir_stat( compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
{
	std::string simple;
	if( auto def = dir_stat_defaults( aPath, false, &simple, aErrorOut ) )
		return def.value();

	auto const parts = vpath_split_first(simple);
	auto const it = mDir.find( std::get<0>(parts) );
	if( mDir.end() != it )
	{
		if( std::get<1>(parts).empty() ) return it->second->stat();
		else return it->second->dir_stat( std::get<1>(parts), aErrorOut );
	}

	aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
	return {};
}

shared_ptr<VINode> VDir::dir_open( compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
{
	std::string simple;
	if( auto def = dir_open_defaults( aPath, false, &simple, aErrorOut ) )
		return def.value();

	auto const parts = vpath_split_first(simple);
	auto const it = mDir.find( std::get<0>(parts) );
	if( mDir.end() != it )
	{
		if( std::get<1>(parts).empty() ) return it->second;
		else return it->second->dir_open( std::get<1>(parts), aErrorOut );
	}
	
	aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
	return nullptr;
}

unique_ptr<Buffer> VDir::dir_open_buffer( compat::string_view const& aPath, EBufferMode aMode, bool aMustExist, BufferSize aResizeTo, compat::error_code& aErrorOut ) noexcept
{
	std::string simple;
	if( auto def = dir_open_buffer_defaults( aPath, aMode, aMustExist, aResizeTo, false, &simple, aErrorOut ) )
		return std::move(def.value());

	auto const parts = vpath_split_first(simple);
	auto const it = mDir.find( std::get<0>(parts) );
	if( mDir.end() != it )
	{
		if( std::get<1>(parts).empty() ) return it->second->file_as_buffer( aMode, aMustExist, aResizeTo, aErrorOut );
		else return it->second->dir_open_buffer( std::get<1>(parts), aMode, aMustExist, aResizeTo, aErrorOut );
	}

	aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
	return nullptr;
}
unique_ptr<Stream> VDir::dir_open_stream( compat::string_view const& aPath, EStreamMode aMode, compat::error_code& aErrorOut ) noexcept
{
	std::string simple;
	if( auto def = dir_open_stream_defaults( aPath, aMode, false, &simple, aErrorOut ) )
		return std::move(def.value());

	auto const parts = vpath_split_first(simple);
	auto const it = mDir.find( std::get<0>(parts) );
	if( mDir.end() != it )
	{
		if( std::get<1>(parts).empty() ) return it->second->file_as_stream( aMode, aErrorOut );
		else return it->second->dir_open_stream( std::get<1>(parts), aMode, aErrorOut );
	}

	aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
	return nullptr;
}

void VDir::dir_create( compat::string_view const& aPath, bool aRecursive )
{
	std::string simple;
	if( dir_create_defaults( aPath, aRecursive, false, &simple ) )
		return;

	auto const parts = vpath_split_first(simple);
	auto const it = mDir.find( std::get<0>(parts) );
	if( mDir.end() != it )
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

	if( !std::get<1>(parts).empty() && !aRecursive )
	{
		FLUX_THROW( error::VfsNotFound )
			<< einfo::VfsDesc( desc() )
			<< einfo::VfsRecordStat( stat() )
			<< flux::einfo::ErrorDesc( FLUX_FMT_STRING("VDir: directory '{}' does not exist"), std::get<0>(parts) )
		;
	}

	FLUX_THROW_IF( mReadOnly, error::VfsDenied )
		<< einfo::VfsDesc( desc() )
		<< einfo::VfsRecordStat( stat() )
		<< flux::einfo::ErrorDesc( "VDir: read only" )
		<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("path = '{}'"), aPath )
	;

	// So... what? A new VDir?
	// What is anybody going to do with that? *shrug*
	auto nd = make_shared<VDir>( 
		std::string(std::get<0>(parts)), 
		shared_from_this(), 
		mReadOnly 
	);
	add( nd );
	
	if( !std::get<1>(parts).empty() )
		nd->dir_create( std::get<1>(parts), aRecursive );
}

bool VDir::dir_remove( compat::string_view const& aPath, bool aRecursive )
{
	std::string simple;
	if( auto def = dir_remove_defaults( aPath, aRecursive, false, &simple ) )
		return def.value();

	auto const parts = vpath_split_first(simple);
	auto const it = mDir.find( std::get<0>(parts) );
	if( mDir.end() != it )
	{
		if( std::get<1>(parts).empty() )
		{
			FLUX_THROW_IF( mReadOnly, error::VfsDenied )
				<< einfo::VfsDesc( desc() )
				<< einfo::VfsRecordStat( stat() )
				<< flux::einfo::ErrorDesc( "VDir: read only" )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("removing '{}'"), std::get<0>(parts) )
			;

			mDir.erase( it );
			return true;
		}
		
		return it->second->dir_remove( std::get<1>(parts), aRecursive );
	}

	return false;
}

//--    <<< ~ flux::io::vfsi namespace <<<          ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(io,vfsi)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
