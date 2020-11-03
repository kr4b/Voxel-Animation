/*-******************************************************* -- HEADER -{{{1- */
/*-	Special io::Streams
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_STREAM_SPECIAL_HPP_B87E45BF_6135_4278_9E21_F067263ACE22
#define FLUX_IO_STREAM_SPECIAL_HPP_B87E45BF_6135_4278_9E21_F067263ACE22

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>

#include <flux/io/stream.hpp>
#include <flux/std/unique_ptr.hpp>

//--    >>> namespace = flux::io >>>            ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    functions                       ///{{{2///////////////////////////////

unique_ptr<Stream> stream_create_null();
unique_ptr<Stream> stream_create_zero();


unique_ptr<Stream> stream_create_stdin();
unique_ptr<Stream> stream_create_stdout();
unique_ptr<Stream> stream_create_stderr();

/*
unique_ptr<Stream> stream_create_cin(); //TODO
unique_ptr<Stream> stream_create_cout(); //TODO
unique_ptr<Stream> stream_create_clog(); //TODO
unique_ptr<Stream> stream_create_cerr(); //TODO
*/

//--    <<< ~ flux::io namespace <<<            ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_STREAM_SPECIAL_HPP_B87E45BF_6135_4278_9E21_F067263ACE22
