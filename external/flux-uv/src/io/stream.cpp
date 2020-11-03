/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Stream implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/stream.hpp>
#include <flux/io/errors.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/format.hpp>

#include <sstream>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    Stream                              ///{{{2///////////////////////////

Stream::~Stream() = default;

StreamSize Stream::read( void*, StreamSize, EIoRequest )
{
	FLUX_ASSERT( !caps().readable );
	FLUX_THROW( error::StreamOpNotSupported )
		<< einfo::StreamDesc( description() )
		<< flux::einfo::ErrorDesc( "Stream not readable" )
	;
}
StreamSize Stream::write( void const*, StreamSize, EIoRequest )
{
	FLUX_ASSERT( !caps().readable );
	FLUX_THROW( error::StreamOpNotSupported )
		<< einfo::StreamDesc( description() )
		<< flux::einfo::ErrorDesc( "Stream not writable" )
	;
}

StreamSize Stream::size() const
{
	FLUX_ASSERT( !caps().size );
	FLUX_THROW( error::StreamOpNotSupported )
		<< einfo::StreamDesc( description() )
		<< flux::einfo::ErrorDesc( "Stream's size is not known" )
	;
}
StreamSize Stream::tell() const
{
	FLUX_ASSERT( !caps().tell );
	FLUX_THROW( error::StreamOpNotSupported )
		<< einfo::StreamDesc( description() )
		<< flux::einfo::ErrorDesc( "Stream's current offset not known" )
	;
}
StreamSize Stream::seek( StreamOffset, ESeekWhence )
{
	FLUX_ASSERT( !caps().seekFwd && !caps().seekRev );
	FLUX_THROW( error::StreamOpNotSupported )
		<< einfo::StreamDesc( description() )
		<< flux::einfo::ErrorDesc( "Stream not seekable" )
	;
}
StreamSize Stream::remaining() const
{
	FLUX_ASSERT( !caps().remaining );
	FLUX_THROW( error::StreamOpNotSupported )
		<< einfo::StreamDesc( description() )
		<< flux::einfo::ErrorDesc( "Stream's remaining size not known" )
	;
}

void Stream::flush()
{}

unique_ptr<Stream> Stream::clone() const
{
	FLUX_ASSERT( !caps().clonable );
	FLUX_THROW( error::StreamOpNotSupported )
		<< einfo::StreamDesc( description() )
		<< flux::einfo::ErrorDesc( "Stream not clonable" )
	;
}

//--    to_string()                         ///{{{2///////////////////////////
std::string to_string( EIoRequest aReq )
{
	switch( aReq )
	{
		case EIoRequest::bestEffort: return "EIoRequest::bestEffort";
		case EIoRequest::mustComplete: return "EIoRequest::mustComplete";
	}

	return format( "EIoRequest({:d})", int(aReq) );
}

std::string to_string( ESeekWhence aWhence )
{
	switch( aWhence )
	{
		case ESeekWhence::cur: return "ESeekWhence::cur";
		case ESeekWhence::set: return "ESeekWhence::set";
		case ESeekWhence::end: return "ESeekWhence::end";
	}

	return format( "ESeekWhence({:d})", int(aWhence) );
}

std::string to_string( EStreamMode aMode )
{
	std::ostringstream oss;

	bool sep = false;
	auto append_ = [&] (char const* aFlag) { if( sep ) oss << "|"; oss << aFlag; sep = true; };

	oss << "EStreamMode(";
	if( (EStreamMode::read & aMode) == EStreamMode::read ) append_( "read" );
	if( (EStreamMode::write & aMode) == EStreamMode::write ) append_( "write" );
	if( (EStreamMode::text & aMode) == EStreamMode::text ) append_( "text" );
	if( (EStreamMode::append & aMode) == EStreamMode::append ) append_( "append" );
	oss << ")";

	return oss.str();
}

std::string to_string( EStreamStatus aStatus )
{
	std::ostringstream oss;

	bool sep = false;
	auto append_ = [&] (char const* aFlag) { if( sep ) oss << "|"; oss << aFlag; sep = true; };

	oss << "EStreamMode(";
	if( !!(EStreamStatus::readable & aStatus )) append_( "readable" );
	if( !!(EStreamStatus::writable & aStatus) ) append_( "writable" );
	if( !!(EStreamStatus::error & aStatus) ) append_( "error" );
	oss << ")";

	return oss.str();
}


std::string to_string( StreamCaps aCaps )
{
	return format( "StreamCaps{{{:c}{:c}:{:c}{:c}{:c}{:c}}}",
		aCaps.readable ? 'r':'-',
		aCaps.writable ? 'w':'-',
		aCaps.size ? 'S':'-',
		aCaps.tell ? 'T':'-',
		aCaps.seekFwd ? 'F':'-',
		aCaps.seekRev ? 'R':'-'
	);
}

//--    <<< ~ flux::io namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
