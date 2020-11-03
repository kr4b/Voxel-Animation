/*-******************************************************* -- HEADER -{{{1- */
/*-	File buffer
 *
 * Note: this is now just a simple wrapper around sysfile.hpp but without
 * pulling in <flux/sys/paths.hpp>.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_BUFFER_FILE_HPP_F814C713_A07E_4DE3_BC14_EE95737B8CB8
#define FLUX_IO_BUFFER_FILE_HPP_F814C713_A07E_4DE3_BC14_EE95737B8CB8

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>
#include <flux/io/buffer.hpp>

#include <flux/std/unique_ptr.hpp>

//--    >>> namespace = flux::io >>>            ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    functions                       ///{{{2///////////////////////////////

unique_ptr<Buffer> buffer_create_file( 
	char const*, 
	EBufferMode = EBufferMode::read,
	bool aMustExist = true,
	BufferSize = ~BufferSize(0)
);

//--    <<< ~ flux::io namespace <<<            ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_BUFFER_FILE_HPP_F814C713_A07E_4DE3_BC14_EE95737B8CB8

