/*-******************************************************* -- HEADER -{{{1- */
/*-	VFSinternal - VFS entry masking a different entry
 *
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_VFSI_MASQUERADE_HPP_B7E9FB60_BAB2_4588_9FE6_7CB1BFF3011E
#define FLUX_IO_VFSI_MASQUERADE_HPP_B7E9FB60_BAB2_4588_9FE6_7CB1BFF3011E

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>
#include <flux/std/shared_ptr.hpp>

#include <string>

#include "inode.hpp"

//--    >>> namespace = flux::io::vfsi >>>      ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(io,vfsi)
//--    functions                       ///{{{2///////////////////////////////

shared_ptr<VINode> inode_masquerade( 
	weak_ptr<VINode>,
	std::string aPathFromAncestor,
	weak_ptr<VINode>
);
shared_ptr<VINode> inode_masquerade( 
	shared_ptr<VINode>,
	std::string aPathFromAncestor,
	weak_ptr<VINode>
);

//--    <<< ~ flux::io::vfsi namespace <<<      ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(io,vfsi)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_VFSI_MASQUERADE_HPP_B7E9FB60_BAB2_4588_9FE6_7CB1BFF3011E
