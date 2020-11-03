/*-******************************************************* -- HEADER -{{{1- */
/*-	System native io::Stream
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_STREAM_SYSFILE_HPP_4C2EEEAC_26D7_45A1_ADB7_4BF09D36FC3C
#define FLUX_IO_STREAM_SYSFILE_HPP_4C2EEEAC_26D7_45A1_ADB7_4BF09D36FC3C

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/io/stream.hpp>

#include <flux/sys/syspath.hpp>
#include <flux/std/unique_ptr.hpp>

//--    >>> namespace = flux::io >>>            ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    functions                       ///{{{2///////////////////////////////

unique_ptr<Stream> stream_create_sysfile( 
	sys::SysPathView const&,
	EStreamMode = EStreamMode::read,
	std::string = std::string()
);

/** \brief Stream: temporary file
 *
 * Returns a stream that will write to and read from a temporary file. The file
 * is deleted when the stream is closed. Files are always created as both
 * readable and writable.
 *
 * \note Implementation notes: Uses `mkstemp()` on linux, with the template
 * `<tempdir>/fluxtmp-XXXXXX`, where `<tempdir>` is the directory returned by
 * `sys::path_get_tempdir()`. The file is `unlink()`ed immediately after
 * creation. On windows, uses `GetTempFileName()` and `CreateFile()` with
 * `FILE_FLAG_DELETE_ON_CLOSE` and `FILE_FLAG_TEMPORARY`. 
 */
unique_ptr<Stream> stream_create_systemp();

//--    <<< ~ flux::io namespace <<<            ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_STREAM_SYSFILE_HPP_4C2EEEAC_26D7_45A1_ADB7_4BF09D36FC3C
