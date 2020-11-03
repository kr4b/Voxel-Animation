/*-******************************************************* -- HEADER -{{{1- */
/*-	VFSinternal - VFS entry representing system root
 *
 * TODO: how do we handle paths other than drive letters under windows?
 *    - `/?/<thing>` --> \\?\<thing>
 *    - What about \\Foo\Bar in Windows? Simply do with `/Foo/bar` and screw
 *      over people that have a \\X:\Bar share. Is that even possible?
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_VFSI_SYSROOT_HPP_C2CB2075_67D1_4E5E_851A_FCA93F8212F0
#define FLUX_IO_VFSI_SYSROOT_HPP_C2CB2075_67D1_4E5E_851A_FCA93F8212F0

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include "inode.hpp"

//--    >>> namespace = flux::io::vfsi >>>      ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(io,vfsi)
//--    functions                       ///{{{2///////////////////////////////

shared_ptr<VINode> inode_create_sysroot();

//--    <<< ~ flux::io::vfsi namespace <<<      ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(io,vfsi)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_VFSI_SYSROOT_HPP_C2CB2075_67D1_4E5E_851A_FCA93F8212F0
