/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	checkpoint implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/gl/checkpoint.hpp>
#include <flux/gl/errors.hpp>
#include <flux/gl/error_category.hpp>

#include <flux/std/throw.hpp>

#include <iostream>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::gl >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(gl)
//--    $ detail                            ///{{{2///////////////////////////
namespace detail
{
	FLUX_ATTR_NORETURN
	void throw_gl_checkpoint_error( char const* aFile, char const* aFunc, int aLine, int aErrNum )
	{
		//TODO: log the error as well??

        std::cout << aFile << ", "
            << aLine << ", "
            << aFunc << ", "
            << make_gl_error(aErrNum) << std::endl;
		
		FLUX_THROW( gl::error::GLCheckpoint )
			<< einfo::SourceFile( aFile )
			<< einfo::SourceLine( aLine )
			<< einfo::SourceFunc( aFunc )
			<< einfo::ApiError( make_gl_error(aErrNum) )
		;
	}
}
//--    <<< ~ flux::gl namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(gl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
