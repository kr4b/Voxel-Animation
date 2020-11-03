/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	File Buffer implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/buffer/file.hpp>
#include <flux/io/buffer/sysfile.hpp>

#include <flux/sys/pathutil.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(io)
//--    buffer_create_file()                ///{{{2///////////////////////////
unique_ptr<Buffer> buffer_create_file( char const* aPath, EBufferMode aMode, bool aMustExist, BufferSize aResizeTo )
{
	FLUX_ASSERT( aPath );
	return buffer_create_sysfile( 
		sys::path_from_generic(aPath),
		aMode,
		aMustExist, aResizeTo
	);
}

//--    <<< ~ flux::io namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(io)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
