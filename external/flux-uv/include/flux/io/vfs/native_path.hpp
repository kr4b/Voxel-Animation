/*-******************************************************* -- HEADER -{{{1- */
/*-	Get native path.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_VFS_NATIVE_PATH_HPP_87703735_5BE3_4964_A571_3963AB548118
#define FLUX_IO_VFS_NATIVE_PATH_HPP_87703735_5BE3_4964_A571_3963AB548118

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/io/vfs.hpp>
#include <flux/sys/syspath.hpp>

//--    >>> namespace = flux::io::vfs >>>       ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(io,vfs)

//--    functions                       ///{{{2///////////////////////////////

sys::SysPathStr native_path( VfsRecord const& );
optional<sys::SysPathStr> native_path( VfsRecord const&, compat::error_code& );

//--    <<< ~ flux::io::vfs namespace <<<       ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(io,vfs)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_VFS_NATIVE_PATH_HPP_87703735_5BE3_4964_A571_3963AB548118
