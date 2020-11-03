/*-******************************************************* -- HEADER -{{{1- */
/*-	Buffer-backed read-only io::Streams
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_STREAM_BUFREAD_HPP_5162AF40_6048_416D_895C_495632F044CC
#define FLUX_IO_STREAM_BUFREAD_HPP_5162AF40_6048_416D_895C_495632F044CC

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/io/stream.hpp>
#include <flux/io/forward.hpp>

#include <flux/ftl/not_null.hpp>
#include <flux/std/function.hpp>
#include <flux/std/unique_ptr.hpp>

//--    >>> namespace = flux::io >>>            ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    functions                       ///{{{2///////////////////////////////

unique_ptr<Stream> stream_create_bufread( //TODO
	ftl::not_null<Buffer*>,
	BufferSize = 0,
	BufferSize = ~BufferSize(0)
);
unique_ptr<Stream> stream_create_bufread( //TODO
	unique_ptr<Buffer>,
	BufferSize = 0,
	BufferSize = ~BufferSize(0)
);

//--    <<< ~ flux::io namespace <<<            ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_STREAM_BUFREAD_HPP_5162AF40_6048_416D_895C_495632F044CC
