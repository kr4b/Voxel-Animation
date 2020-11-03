/*-******************************************************* -- HEADER -{{{1- */
/*-	C FILE* io::Stream
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_STREAM_CFILE_HPP_983252AB_1C89_4A34_9561_240A489A1A77
#define FLUX_IO_STREAM_CFILE_HPP_983252AB_1C89_4A34_9561_240A489A1A77

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/io/stream.hpp>

#include <flux/ftl/not_null.hpp>
#include <flux/std/unique_ptr.hpp>

#include <cstdio>

//--    >>> namespace = flux::io >>>            ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    functions                       ///{{{2///////////////////////////////

unique_ptr<Stream> stream_create_cfile( 
	ftl::not_null<FILE*>,
	EStreamMode = EStreamMode::read,
	std::string = std::string()
);
unique_ptr<Stream> stream_create_cfile( 
	char const*, 
	EStreamMode = EStreamMode::read
);

#if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
unique_ptr<Stream> stream_create_cfile( 
	wchar_t const*, 
	EStreamMode = EStreamMode::read 
);
#endif // ~ PLATFORM_WIN32

//--    <<< ~ flux::io namespace <<<            ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_STREAM_CFILE_HPP_983252AB_1C89_4A34_9561_240A489A1A77
