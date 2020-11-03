/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	vfsi::PDir implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/vfsi/pdir.hpp>

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
//--    PDir                                ///{{{2///////////////////////////

	// [cd]tor -{{{3-
PDir::PDir( std::string aPathFromAncestor, weak_ptr<VINode> aAncestor )
	: VINode( std::move(aPathFromAncestor), std::move(aAncestor) )
{
	mStat.directory = 1;
	mStat.accessible = 1;
	mStat.listable = 1;
	mStat.changeable = 0;
	mStat.flat = 0;
	
	//XXX-technically a property of the parent directory...
	mStat.removable = 0;
}

	// public -{{{3-
void PDir::add( compat::string_view const& aName, shared_ptr<VINode> aEntry )
{
	FLUX_ASSERT( aEntry );
	mDir.insert( std::make_pair(std::string(aName),aEntry) );
}

void PDir::remove( compat::string_view const& aName )
{
	// TODO: no heterogeneous lookup for unordered_map, even under C++14. :-(
	auto it = mDir.find( std::string(aName) );
	if( mDir.end() != it )
		mDir.erase( it );
}

	// virtuals: generic -{{{3-
std::string PDir::desc() const
{
	return format( "pdir:{}", name() );
}

bool PDir::update( bool aRecursive, compat::error_code& aErrorOut ) noexcept
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
bool PDir::remove( bool, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	aErrorOut = compat::make_error_code( compat::errc::permission_denied );
	return false;
}

	// virtuals: directory access -{{{3-
optional<VfsStat> PDir::dir_stat( compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
{
	std::string simple;
	if( auto def = dir_stat_defaults( aPath, false, &simple, aErrorOut ) )
		return def.value();

	auto const parts = vpath_split_first(simple);
	auto const it = mDir.find( std::string(std::get<0>(parts)) ); // XXX-allocs
	if( mDir.end() != it )
	{
		if( std::get<1>(parts).empty() ) return it->second->stat();
		else return it->second->dir_stat( std::get<1>(parts), aErrorOut );
	}

	aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
	return {};
}

shared_ptr<VINode> PDir::dir_open( compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
{
	std::string simple;
	if( auto def = dir_open_defaults( aPath, false, &simple, aErrorOut ) )
		return def.value();

	auto const parts = vpath_split_first(simple);
	auto const it = mDir.find( std::string(std::get<0>(parts)) ); //XXX-allocs
	if( mDir.end() != it )
	{
		if( std::get<1>(parts).empty() ) return it->second;
		else return it->second->dir_open( std::get<1>(parts), aErrorOut );
	}
	
	aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
	return nullptr;
}

unique_ptr<Buffer> PDir::dir_open_buffer( compat::string_view const& aPath, EBufferMode aMode, bool aMustExist, BufferSize aResizeTo, compat::error_code& aErrorOut ) noexcept
{
	std::string simple;
	if( auto def = dir_open_buffer_defaults( aPath, aMode, aMustExist, aResizeTo, false, &simple, aErrorOut ) )
		return std::move(def.value());

	auto const parts = vpath_split_first(simple);
	auto const it = mDir.find( std::string(std::get<0>(parts)) ); //XXX-allocs
	if( mDir.end() != it )
	{
		if( std::get<1>(parts).empty() ) return it->second->file_as_buffer( aMode, aMustExist, aResizeTo, aErrorOut );
		else return it->second->dir_open_buffer( std::get<1>(parts), aMode, aMustExist, aResizeTo, aErrorOut );
	}

	aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
	return nullptr;

#	if 0
	//TODO: just return nullptr here??
	FLUX_THROW( error::VfsNotFound )
		<< einfo::VfsDesc( desc() )
		<< einfo::VfsRecordStat( stat() )
		<< flux::einfo::ErrorDesc( "Requested path does not exist" )
		<< flux::einfo::ErrorMeta( "path = '{}'", aPath )
	;
#	endif
}
unique_ptr<Stream> PDir::dir_open_stream( compat::string_view const& aPath, EStreamMode aMode, compat::error_code& aErrorOut ) noexcept
{
	std::string simple;
	if( auto def = dir_open_stream_defaults( aPath, aMode, false, &simple, aErrorOut ) )
		return std::move(def.value());

	auto const parts = vpath_split_first(simple);
	auto const it = mDir.find( std::string(std::get<0>(parts)) ); //XXX-allocs
	if( mDir.end() != it )
	{
		if( std::get<1>(parts).empty() ) return it->second->file_as_stream( aMode, aErrorOut );
		else return it->second->dir_open_stream( std::get<1>(parts), aMode, aErrorOut );
	}

	aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
	return nullptr;

#	if 0
	//TODO: just return nullptr here??
	FLUX_THROW( error::VfsNotFound )
		<< einfo::VfsDesc( desc() )
		<< einfo::VfsRecordStat( stat() )
		<< flux::einfo::ErrorDesc( "Requested path does not exist" )
		<< flux::einfo::ErrorMeta( "path = '{}'", aPath )
	;
#	endif
}

void PDir::dir_create( compat::string_view const& aPath, bool aRecursive )
{
	std::string simple;
	if( dir_create_defaults( aPath, aRecursive, false, &simple ) )
		return;

	auto const parts = vpath_split_first(simple);
	auto const it = mDir.find( std::string(std::get<0>(parts)) ); //XXX-allocs
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
			<< flux::einfo::ErrorDesc( FLUX_FMT_STRING("PDir: directory '{}' does not exist"), std::get<0>(parts) )
		;
	}

	FLUX_THROW( error::VfsDenied )
		<< einfo::VfsDesc( desc() )
		<< einfo::VfsRecordStat( stat() )
		<< flux::einfo::ErrorDesc( "PDir: read only" )
	;
}

bool PDir::dir_remove( compat::string_view const& aPath, bool aRecursive )
{
	std::string simple;
	if( auto def = dir_remove_defaults( aPath, aRecursive, false, &simple ) )
		return def.value();

	auto const parts = vpath_split_first(simple);
	auto const it = mDir.find( std::string(std::get<0>(parts)) );
	if( mDir.end() != it )
	{
		if( std::get<1>(parts).empty() )
		{
			FLUX_THROW( error::VfsDenied )
				<< einfo::VfsDesc( desc() )
				<< einfo::VfsRecordStat( stat() )
				<< flux::einfo::ErrorDesc( "PDir: read only" )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("removing '{}'"), std::get<0>(parts) )
			;
		}
		
		return it->second->dir_remove( std::get<1>(parts), aRecursive );
	}

	return false;
}

//--    <<< ~ flux::io::vfsi namespace <<<          ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(io,vfsi)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
