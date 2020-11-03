/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Generic VfsRecord
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/vfsi/inode_record.hpp>
#include <flux/std/assert.hpp>

#include "vfs_record_factory.hxx"

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io::vfsi >>>          ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(io,vfsi)
//--    default_entry_record()              ///{{{2///////////////////////////
VfsRecord make_inode_record( shared_ptr<VINode> aRoot, shared_ptr<VINode> aCurrent )
{
	FLUX_ASSERT( aRoot );
	return detail::VfsRecordFactory::make( 
		aCurrent ? aCurrent : aRoot,
		aRoot
	);
}

//--    <<< ~ flux::io::vfsi namespace <<<          ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(io,vfsi)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
