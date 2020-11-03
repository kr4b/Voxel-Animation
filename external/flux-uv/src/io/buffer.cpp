/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Buffer implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/buffer.hpp>
#include <flux/std/format.hpp>

#include <sstream>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    Buffer                              ///{{{2///////////////////////////

Buffer::~Buffer() = default;

//--    to_string()                         ///{{{2///////////////////////////

std::string to_string( EBufferMode aMode )
{
	std::ostringstream oss;

	bool sep = false;
	auto append_ = [&] (char const* aFlag) { if( sep ) oss << "|"; oss << aFlag; sep = true; };

	oss << "EBufferMode(";
	if( (EBufferMode::read & aMode) == EBufferMode::read ) append_( "read" );
	if( (EBufferMode::write & aMode) == EBufferMode::write ) append_( "write" );
	oss << ")";

	return oss.str();
}

std::string to_string( EBufferMap aMap )
{
	if( EBufferMap::ro == aMap )
		return "EBufferMap::ro";

	if( EBufferMap::wo == aMap )
		return "EBufferMap::wo";
	
	if( EBufferMap::rw == aMap )
		return "EBufferMap::rw";

	return format( "EBufferMap({:d})", aMap.value() );
}

std::string to_string( EBufferMapRO aMapRO )
{
	switch( aMapRO )
	{
		case EBufferMapRO::ro: return "EBufferMapRO::ro";
	}

	return format( "EBufferMapRO({:d})", int(aMapRO) );
}
std::string to_string( EBufferMapRW aMapRW )
{
	switch( aMapRW )
	{
		case EBufferMapRW::wo: return "EBufferMapRO::wo";
		case EBufferMapRW::rw: return "EBufferMapRO::rw";
	}

	return format( "EBufferMapRW({:d})", int(aMapRW) );
}


std::string to_string( BufferCaps aCaps )
{
	return format( "BufferCaps{{{:c}{:c}:{}{}{:c}:{:c}}}",
		aCaps.readable ? 'r':'-',
		aCaps.writable ? 'w':'-',
		aCaps.partialMap ? "pM":"--",
		aCaps.partialUnmap ? "pU":"--",
		aCaps.multiMap ? '*':'-',
		aCaps.resizable ? 'S':'-'
	);
}

//--    <<< ~ flux::io namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
