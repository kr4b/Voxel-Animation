/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	vfsi::UnionDir implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/vfsi/union.hpp>
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
//--    UnionDir                            ///{{{2///////////////////////////

	// [cd]tor -{{{3-
UnionDir::UnionDir( std::string aPathFromAncestor, weak_ptr<VINode> aAncestor )
	: VINode( std::move(aPathFromAncestor), std::move(aAncestor) )
{
	mStat.directory = 1;
	mStat.accessible = 1;
	mStat.listable = 0; //TODO
	mStat.changeable = 0;
	mStat.flat = 0;
	
	//XXX-technically a property of the parent directory...
	mStat.removable = false; //XXX
}

	// public -{{{3-
void UnionDir::add_back( shared_ptr<VINode> aEntry )
{
	FLUX_ASSERT( aEntry && aEntry->stat().directory );

	auto ta = ancestor().lock();
	auto ea = aEntry->ancestor().lock();

	if( ta == ea && path(kAncestorNode) == aEntry->path(kAncestorNode) )
	{
		mStat.changeable = mStat.changeable || aEntry->stat().changeable;
		mStack.emplace_back( std::move(aEntry) );
		return;
	}
	
	mStat.changeable = mStat.changeable || aEntry->stat().changeable;
	mStack.emplace_back( inode_masquerade( std::move(aEntry), path(kAncestorNode), ta ) );
}

void UnionDir::add_front( shared_ptr<VINode> aEntry )
{
	FLUX_ASSERT( aEntry && aEntry->stat().directory );

	auto ta = ancestor().lock();
	auto ea = aEntry->ancestor().lock();

	if( ta == ea && path(kAncestorNode) == aEntry->path(kAncestorNode) )
	{
		mStat.changeable = mStat.changeable || aEntry->stat().changeable;
		mStack.emplace( mStack.begin(), std::move(aEntry) );
		return;
	}
	
	mStat.changeable = mStat.changeable || aEntry->stat().changeable;
	mStack.emplace( mStack.begin(), inode_masquerade( std::move(aEntry), path(kAncestorNode), ta ) );
}


void UnionDir::remove( shared_ptr<VINode> aEntry )
{
	FLUX_ASSERT( aEntry );

	auto it = std::find( mStack.begin(), mStack.end(), aEntry );
	if( mStack.end() != it )
		mStack.erase( it );
}


std::size_t UnionDir::count() const noexcept
{
	return mStack.size();
}
shared_ptr<VINode const> UnionDir::get( std::size_t aIdx ) const noexcept
{
	FLUX_ASSERT( aIdx < mStack.size() );
	return mStack[aIdx];
}

	// virtuals: generic -{{{3-
std::string UnionDir::desc() const
{
	return format( "stack:{}", name() );
}

bool UnionDir::update( bool aRecursive, compat::error_code& aErrorOut ) noexcept
{
	FLUX_ASSERT( !aErrorOut );
	if( aRecursive )
	{
		bool changeable = false;
		for( auto& oe : mStack )
		{
			if( !oe->update( true, aErrorOut )) 
				return false;

			changeable = changeable || oe->stat().changeable;
		}

		mStat.changeable = changeable;
	}

	return true;
}
bool UnionDir::remove( bool /*aRecursive*/, compat::error_code& aErrorOut ) noexcept
{
	//TODO: Hmm. what should this do?
	FLUX_ASSERT( !aErrorOut );
	aErrorOut = compat::make_error_code( compat::errc::permission_denied );
	return false;
}

	// virtuals: directory access -{{{3-
optional<VfsStat> UnionDir::dir_stat( compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
{
	std::string simple;
	if( auto def = dir_stat_defaults( aPath, false, &simple, aErrorOut ) )
		return def.value();

	compat::error_code last = compat::make_error_code( compat::errc::no_such_file_or_directory );
	for( auto& d : mStack )
	{
		compat::error_code ec;
		if( auto s = d->dir_stat( aPath, ec ) )
			return s;

		last = ec;
	}

	aErrorOut = last;
	return {};
}

shared_ptr<VINode> UnionDir::dir_open( compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
{
	std::string simple;
	if( auto def = dir_open_defaults( aPath, false, &simple, aErrorOut ) )
		return def.value();

	compat::error_code last = compat::make_error_code( compat::errc::no_such_file_or_directory );
	for( auto& d : mStack )
	{
		compat::error_code ec;
		if( auto n = d->dir_open( aPath, ec ) )
			return n;

		last = ec;
	}

	aErrorOut = last;
	return nullptr;
}

unique_ptr<Buffer> UnionDir::dir_open_buffer( compat::string_view const& aPath, EBufferMode aMode, bool aMustExist, BufferSize aResizeTo, compat::error_code& aErrorOut ) noexcept
{
	std::string simple;
	if( auto def = dir_open_buffer_defaults( aPath, aMode, aMustExist, aResizeTo, false, &simple, aErrorOut ) )
		return std::move(def.value());

	compat::error_code last = compat::make_error_code( compat::errc::no_such_file_or_directory );
	for( auto& d : mStack )
	{
		compat::error_code ec;
		if( auto b = d->dir_open_buffer( aPath, aMode, aMustExist, aResizeTo, ec ) )
			return b;

		last = ec;
	}

	aErrorOut = last;
	return nullptr;
}
unique_ptr<Stream> UnionDir::dir_open_stream( compat::string_view const& aPath, EStreamMode aMode, compat::error_code& aErrorOut ) noexcept
{
	std::string simple;
	if( auto def = dir_open_stream_defaults( aPath, aMode, false, &simple, aErrorOut ) )
		return std::move(def.value());

	compat::error_code last = compat::make_error_code( compat::errc::no_such_file_or_directory );
	for( auto& d : mStack )
	{
		compat::error_code ec;
		if( auto s = d->dir_open_stream( aPath, aMode, ec ) )
			return s;

		last = ec;
	}

	aErrorOut = last;
	return nullptr;
}

void UnionDir::dir_create( compat::string_view const& aPath, bool aRecursive )
{
	std::string simple;
	if( dir_create_defaults( aPath, aRecursive, false, &simple ) )
		return;

	for( auto& d : mStack )
	{
		//TODO
		//TODO  use the error codes...
		//TODO
		//TODO
		try
		{
			d->dir_create( aPath, aRecursive );
			return;
		}
		catch( ... )
		{
			if( d == mStack.back() )
				throw;
		}
	}
}

bool UnionDir::dir_remove( compat::string_view const& aPath, bool aRecursive )
{
	std::string simple;
	if( auto def = dir_remove_defaults( aPath, aRecursive, false, &simple ) )
		return def.value();

	bool ret = false;
	for( auto& d : mStack )
	{
		//TODO
		//TODO  use the error codes...
		//TODO
		//TODO
		try
		{
			ret = d->dir_remove( aPath, aRecursive ) || ret;
			return ret;
		}
		catch( ... )
		{
			if( d == mStack.back() )
				throw;
		}
	}

	return ret;
}

//--    <<< ~ flux::io::vfsi namespace <<<          ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(io,vfsi)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
