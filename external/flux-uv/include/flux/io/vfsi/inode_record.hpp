/*-******************************************************* -- HEADER -{{{1- */
/*-	Make a generic VfsRecord from VINodes
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_VFSI_INODE_RECORD_HPP_84E2FF87_489A_4DBF_B943_A8256907F27B
#define FLUX_IO_VFSI_INODE_RECORD_HPP_84E2FF87_489A_4DBF_B943_A8256907F27B

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/io/vfs.hpp>
#include <flux/io/vfsi/inode.hpp>

//--    >>> namespace = flux::io::vfsi >>>      ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(io,vfsi)
//--    functions                       ///{{{2///////////////////////////////

VfsRecord make_inode_record( 
	shared_ptr<VINode> aRoot,
	shared_ptr<VINode> aCurrent = shared_ptr<vfsi::VINode>()
);

//--    <<< ~ flux::io::vfsi namespace <<<      ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(io,vfsi)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_VFSI_INODE_RECORD_HPP_84E2FF87_489A_4DBF_B943_A8256907F27B
