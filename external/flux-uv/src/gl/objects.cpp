/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Rsrc<> implementations
 */
/*-***************************************************************** -}}}1- */

#include <flux/gl/glapi.hpp>

#include <flux/std/log.hpp>
#include <flux/std/assert.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::gl >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(gl)
//--    aspect::delete_*                    ///{{{2///////////////////////////
namespace aspect
{
	void delete_fbo( GL::UInt aFbo ) noexcept
	{
		FLUX_ASSERT( 0 != aFbo );
		if( auto const gl = flux::gl::gl_api() )
		{
			FLUX_ASSERT( gl->deleteFramebuffers );
			gl->deleteFramebuffers( 1, &aFbo );
		}
		else
			FLUX_LOG( ERROR, "No OpenGL API bound, leaking FBO #{}", aFbo );
	}
	void delete_rbo( GL::UInt aRbo ) noexcept
	{
		FLUX_ASSERT( 0 != aRbo );
		if( auto const gl = flux::gl::gl_api() )
		{
			FLUX_ASSERT( gl->deleteRenderbuffers );
			gl->deleteRenderbuffers( 1, &aRbo );
		}
		else
			FLUX_LOG( ERROR, "No OpenGL API bound, leaking RBO #{}", aRbo );
	}
	void delete_vao( GL::UInt aVao ) noexcept
	{
		FLUX_ASSERT( 0 != aVao );
		if( auto const gl = flux::gl::gl_api() )
		{
			FLUX_ASSERT( gl->deleteVertexArrays );
			gl->deleteVertexArrays( 1, &aVao );
		}
		else
			FLUX_LOG( ERROR, "No OpenGL API bound, leaking VAO #{}", aVao );
	}
	void delete_query( GL::UInt aQuery ) noexcept
	{
		FLUX_ASSERT( 0 != aQuery );
		if( auto const gl = flux::gl::gl_api() )
		{
			FLUX_ASSERT( gl->deleteQueries );
			gl->deleteQueries( 1, &aQuery );
		}
		else
			FLUX_LOG( ERROR, "No OpenGL API bound, leaking query object #{}", aQuery );
	}
	void delete_buffer( GL::UInt aBuffer ) noexcept
	{
		FLUX_ASSERT( 0 != aBuffer );
		if( auto const gl = flux::gl::gl_api() )
		{
			FLUX_ASSERT( gl->deleteBuffers );
			gl->deleteBuffers( 1, &aBuffer );
		}
		else
			FLUX_LOG( ERROR, "No OpenGL API bound, leaking buffer #{}", aBuffer );
	}
	void delete_shader( GL::UInt aShader ) noexcept
	{
		FLUX_ASSERT( 0 != aShader );
		if( auto const gl = flux::gl::gl_api() )
		{
			FLUX_ASSERT( gl->deleteShader );
			gl->deleteShader( aShader );
		}
		else
			FLUX_LOG( ERROR, "No OpenGL API bound, leaking shader object #{}", aShader );
	}
	void delete_program( GL::UInt aProgram ) noexcept
	{
		FLUX_ASSERT( 0 != aProgram );
		if( auto const gl = flux::gl::gl_api() )
		{
			FLUX_ASSERT( gl->deleteProgram );
			gl->deleteShader( aProgram );
		}
		else
			FLUX_LOG( ERROR, "No OpenGL API bound, leaking program #{}", aProgram );
	}
	void delete_sampler( GL::UInt aSampler ) noexcept
	{
		FLUX_ASSERT( 0 != aSampler );
		if( auto const gl = flux::gl::gl_api() )
		{
			FLUX_ASSERT( gl->deleteSamplers );
			gl->deleteSamplers( 1, &aSampler );
		}
		else
			FLUX_LOG( ERROR, "No OpenGL API bound, leaking sampler object #{}", aSampler );
	}
	void delete_texture( GL::UInt aTexture ) noexcept
	{
		FLUX_ASSERT( 0 != aTexture );
		if( auto const gl = flux::gl::gl_api() )
		{
			FLUX_ASSERT( gl->deleteTextures );
			gl->deleteTextures( 1, &aTexture );
		}
		else
			FLUX_LOG( ERROR, "No OpenGL API bound, leaking texture #{}", aTexture );
	}
}

//--    <<< ~ flux::gl namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(gl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
