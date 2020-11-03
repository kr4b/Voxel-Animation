/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Get native paths
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/vfs/native_path.hpp>

#include <flux/io/errors.hpp>
#include <flux/io/vfsi/inode.hpp>

#include <flux/std/throw.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io::vfs >>>           ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(io,vfs)
//--    default_entry_record()              ///{{{2///////////////////////////
sys::SysPathStr native_path( VfsRecord const& aRecord )
{
	FLUX_ASSERT( aRecord.self() );
	FLUX_THROW_IF( !aRecord.self(), error::VfsInvalidRecord )
		<< flux::einfo::ErrorDesc( "VfsRecord is invalid" )
		<< flux::einfo::ErrorMeta( "self is nullptr" )
	;
	
	compat::error_code ec;
	if( auto np = aRecord.self()->native_path( ec ) )
		return np.value();

	FLUX_THROW_IF( compat::errc::not_supported == ec, error::VfsOpNotSupported )
		<< flux::einfo::ErrorDesc( "VfsRecord: can't get native path" )
		<< einfo::VfsDesc( aRecord.self()->desc() )
		<< einfo::VfsRecordStat( aRecord.self()->stat() )
	;
	FLUX_THROW( error::VfsError )
		<< flux::einfo::ApiError( ec )
		<< einfo::VfsDesc( aRecord.self()->desc() )
		<< einfo::VfsRecordStat( aRecord.self()->stat() )
	;
}

optional<sys::SysPathStr> native_path( VfsRecord const& aRecord, compat::error_code& aErrOut )
{
	if( auto np = aRecord.self()->native_path( aErrOut ) )
		return np.value();

	return {};
}

//--    <<< ~ flux::io::vfs namespace <<<           ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(io,vfs)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
