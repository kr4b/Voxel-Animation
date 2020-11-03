/*-******************************************************* -- HEADER -{{{1- */
/*-	VFSinternal - VFS entry representing a ZIP archive
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_VFSI_ZIPDIR_HPP_F41CD1F2_CAE2_42CD_800A_B9B001F650CE
#define FLUX_IO_VFSI_ZIPDIR_HPP_F41CD1F2_CAE2_42CD_800A_B9B001F650CE

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>
#include <flux/io/forward.hpp>

#include "inode.hpp"

//--    >>> namespace = flux::io::vfsi >>>      ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(io,vfsi)
//--    functions                       ///{{{2///////////////////////////////

shared_ptr<VINode> inode_zip_archive( 
	shared_ptr<VINode> const&,
	std::string = std::string(),
	weak_ptr<VINode> = weak_ptr<VINode>()
);
shared_ptr<VINode> inode_zip_archive( 
	unique_ptr<Stream>, 
	std::string,
	weak_ptr<VINode>
);

//--    <<< ~ flux::io::vfsi namespace <<<      ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(io,vfsi)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_VFSI_ZIPDIR_HPP_F41CD1F2_CAE2_42CD_800A_B9B001F650CE
