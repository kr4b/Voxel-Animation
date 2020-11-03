/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- Program helpers
 */
/*-***************************************************************** -}}}1- */

#include <flux/gl/setup/program.hpp>
#include <flux/gl/setup/shader.hpp>

#include <flux/gl/glapi.hpp>
#include <flux/gl/errors.hpp>
#include <flux/gl/checkpoint.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/assert.hpp>
#include <flux/std/format.hpp>

#include <flux/ftl/vector.hpp>
#include <flux/compat/string_view.hpp>

#include <sstream>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::gl >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(gl)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	template< class tItor >
	Program link_program_( tItor, tItor const&, char const* );

#	if FLUXCFG_GL_WITH_FLUXIO
	std::string shader_format_( GL::Enum );

	template< class tItor >
	Program load_program_from_vfs_( tItor, tItor const&, GL::Enum );
#	endif // ~ CFG_GL_WITH_FLUXIO
}

//--    link_program()                      ///{{{2///////////////////////////
namespace
{
	template< typename tItor >
	Program link_program_( tItor aBegin, tItor const& aEnd, char const* aIdent )
	{
		if( !aIdent ) aIdent = "(unknown shader)";

		namespace GL = gl::GL;
		auto const gl = gl::gl_api();

		FLUX_THROW_IF( !gl, error::NoGLApiBound )
			<< einfo::ErrorDesc( "No OpenGL API bound to current thread" )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("program = '{}'"), aIdent )
		;

		// Attach shaders
		FLUX_GL_CHECKPOINT_ALWAYS();
		Program prog{ gl->createProgram() };

		for( ; aBegin != aEnd; ++aBegin )
			gl->attachShader( prog, *aBegin );

		// Link program and check for errors
		FLUX_GL_CHECKPOINT_ALWAYS();
		gl->linkProgram( prog );

		GL::Int success = 0;
		gl->getProgramiv( prog, GL::LINK_STATUS, &success );
		if( !success )
		{
			GL::Int len;
			gl->getProgramiv( prog, GL::INFO_LOG_LENGTH, &len );
			
			ftl::Vector<GL::Char> log(len);
			gl->getProgramInfoLog( prog, len, &len, log.data() );
			
			FLUX_THROW( error::GLShaderCompileFailed )
				<< flux::einfo::ErrorDesc( "Failed to link program" )
				<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("program = '{}'; Info Log = >>>\n{}\n<<<"), aIdent, compat::string_view(log.data(),log.size()) )
			;
		}
		FLUX_GL_CHECKPOINT_ALWAYS();

		return prog;
	}
}

Program setup_program( ftl::VectorBase<GL::UInt> const& aShaders, char const* aIdent )
{
	FLUX_ASSERT( !aShaders.empty() );
	return link_program_( aShaders.begin(), aShaders.end(), aIdent );
}
Program setup_program( ftl::VectorBase<Shader> const& aShaders, char const* aIdent )
{
	FLUX_ASSERT( !aShaders.empty() );
	return link_program_( aShaders.begin(), aShaders.end(), aIdent );
}
Program setup_program( std::initializer_list<GL::UInt> aShaders, char const* aIdent )
{
	FLUX_ASSERT( aShaders.size() );
	return link_program_( aShaders.begin(), aShaders.end(), aIdent );
}

//--    load_program_from_vfs()             ///{{{2///////////////////////////
#if FLUXCFG_GL_WITH_FLUXIO
namespace
{
	template< class tItor >
	Program load_program_from_vfs_( tItor aBegin, tItor const& aEnd, GL::Enum aFormat )
	{
		// Construct a program identifier
		std::size_t longest = ~std::size_t(0);

		auto it = aBegin;
		char const* path = std::get<1>(*it);
		for( ++it; it != aEnd; ++it )
		{
			char const* a = path;
			for( char const* b = std::get<1>(*it); *a && *b && *a == *b; ++a, ++b )
				;

			longest = std::min<std::size_t>( a - path, longest );
		}
		
		if( ~std::size_t(0) == longest )
			longest = 0;

		std::ostringstream oss;
		flux::print( oss, "Program({}:{}{{", shader_format_(aFormat), compat::string_view(path,longest) );

		it = aBegin;
		for( char const* sep = ""; it != aEnd; ++it, sep = "," )
			flux::print( oss, "{}{}", sep, compat::string_view(std::get<1>(*it)+longest) );

		flux::print( oss, "}})" );
		
		// Load shader
		ftl::Vector<Shader> shaders;
		shaders.reserve( std::distance( aBegin, aEnd ) );

		for( ; aBegin != aEnd; ++aBegin )
		{
			shaders.emplace_back( load_shader_from_vfs( 
				std::get<0>(*aBegin),
				std::get<1>(*aBegin),
				aFormat
			) );
		}

		// Link into program
		return setup_program( shaders, oss.str().c_str() );
	}
}

Program load_program_from_vfs( ftl::VectorBase< std::tuple<GL::Enum, char const*> >  const& aSources, GL::Enum aFormat )
{
	FLUX_ASSERT( !aSources.empty() );
	return load_program_from_vfs_( aSources.begin(), aSources.end(), aFormat );
}
Program load_program_from_vfs( std::initializer_list< std::tuple<GL::Enum, char const*> > aSources, GL::Enum aFormat )
{
	FLUX_ASSERT( aSources.size() );
	return load_program_from_vfs_( aSources.begin(), aSources.end(), aFormat );
}
#endif // ~CFG_GL_WITH_FLUXIO

//--    $ shader_*_()                       ///{{{2///////////////////////////
#if FLUXCFG_GL_WITH_FLUXIO
namespace
{
	std::string shader_format_( GL::Enum aFormat )
	{
		namespace GL = gl::GL;

		switch( aFormat )
		{
			case 0: return "glsl";
			case GL::SHADER_BINARY_FORMAT_SPIR_V_ARB: return "spirv";
		}

		return flux::format( FLUX_FMT_STRING("GLenum({:#x})"), unsigned(aFormat) );
	}
}
#endif // ~ CFG_GL_WITH_FLUXIO

//--    <<< ~ flux::gl namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(gl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
