/*-******************************************************* -- HEADER -{{{1- */
/*-	ZStandard Filter Stream
 *
 * See https://facebook.github.io/zstd/
 *
 * TODO: add flags to compression? E.g., no checksums?
 * TODO: support compression/decompression with an external dictionary?
 *   Probably only need a few overloads...
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_STREAM_ZSTD_HPP_63A450F2_6F48_4111_816E_38173100E01C
#define FLUX_IO_STREAM_ZSTD_HPP_63A450F2_6F48_4111_816E_38173100E01C

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/io/forward.hpp>
#include <flux/ftl/not_null.hpp>
#include <flux/std/unique_ptr.hpp>

//--    >>> namespace = flux::io >>>            ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    constants                       ///{{{2///////////////////////////////

constexpr unsigned kZStdDefCompLevel = 3;

constexpr StreamSize kZStdDefDecompInBuffer = ~StreamSize(0);
constexpr StreamSize kZStdDefDecompOutBuffer = ~StreamSize(0);

constexpr StreamSize kZStdDefCompOutBuffer = ~StreamSize(0);

//--    functions                       ///{{{2///////////////////////////////

/** \brief Filter: ZStandard (.zst) compressed stream reader
 *
 * TODO
 */
unique_ptr<Stream> stream_create_zstd_read(
	ftl::not_null<unique_ptr<Stream>>,
	StreamSize = kZStdDefDecompInBuffer,
	StreamSize = kZStdDefDecompOutBuffer
);
/** \copydoc stream_create_zstd_read() */
unique_ptr<Stream> stream_create_zstd_read( 
	ftl::not_null<Stream*>, 
	StreamSize aMaxRead = ~StreamSize(0),
	StreamSize = kZStdDefDecompInBuffer,
	StreamSize = kZStdDefDecompOutBuffer
);

/** \brief Filter: ZStandard .zstd compressed stream writer
 *
 * TODO
 */
unique_ptr<Stream> stream_create_zstd_write( 
	ftl::not_null<unique_ptr<Stream>>,
	unsigned aCompLevel = kZStdDefCompLevel,
	StreamSize aPledgedSize = ~StreamSize(0),
	StreamSize = kZStdDefCompOutBuffer
);
/** \copydoc stream_create_zstd_write() */
unique_ptr<Stream> stream_create_zstd_write( 
	ftl::not_null<Stream*>,
	unsigned aCompLevel = kZStdDefCompLevel,
	StreamSize aPledgedSize = ~StreamSize(0),
	StreamSize = kZStdDefCompOutBuffer
);

//--    <<< ~ flux::io namespace <<<            ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_STREAM_ZSTD_HPP_63A450F2_6F48_4111_816E_38173100E01C
