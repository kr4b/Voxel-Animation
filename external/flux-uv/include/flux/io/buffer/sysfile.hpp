/*-******************************************************* -- HEADER -{{{1- */
/*-	System File buffer
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_BUFFER_SYSFILE_HPP_FCFB5A18_71ED_4145_98E7_BC55669B4E26
#define FLUX_IO_BUFFER_SYSFILE_HPP_FCFB5A18_71ED_4145_98E7_BC55669B4E26

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>
#include <flux/io/buffer.hpp>

#include <flux/compat/system_error.hpp>

#include <flux/sys/syspath.hpp>
#include <flux/std/unique_ptr.hpp>

//--    >>> namespace = flux::io >>>            ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    functions                       ///{{{2///////////////////////////////

unique_ptr<Buffer> buffer_create_sysfile( 
	sys::SysPathView const&, 
	EBufferMode = EBufferMode::read,
	bool aMustExist = true,
	BufferSize = ~BufferSize(0)
);
unique_ptr<Buffer> buffer_create_sysfile(
	sys::SysPathView const&,
	EBufferMode,
	bool aMustExist,
	BufferSize aResizeTo,
	compat::error_code&
) noexcept;

//--    <<< ~ flux::io namespace <<<            ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_BUFFER_SYSFILE_HPP_FCFB5A18_71ED_4145_98E7_BC55669B4E26

