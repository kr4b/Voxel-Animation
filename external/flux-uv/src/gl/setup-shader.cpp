/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- Shader helpers
 */
/*-***************************************************************** -}}}1- */

#include <flux/gl/setup/shader.hpp>

#include <flux/gl/glapi.hpp>
#include <flux/gl/errors.hpp>
#include <flux/gl/checkpoint.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/assert.hpp>
#include <flux/std/format.hpp>

#include <flux/io/vfs.hpp>
#include <flux/io/buffer.hpp>

#include <flux/base/def/vfs.hpp>
#include <flux/base/default_catalogs.hpp>

#include <flux/ftl/vector.hpp>
#include <flux/util/narrow.hpp>
#include <flux/compat/string_view.hpp>

#include <string>
#include <sstream>
#include <iostream>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::gl >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(gl)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	std::string shader_kind_( GL::Enum aKind );
	std::string shader_format_( GL::Enum aFormat );
}

//--    setup_shader()                      ///{{{2///////////////////////////
Shader setup_shader( GL::Enum aKind, char const* aSource, std::size_t aSourceLength, char const* aIdent )
{
	FLUX_ASSERT( aSource );
	if( !aIdent ) aIdent = "(unknown shader)";

	namespace GL = gl::GL;
	auto const gl = gl::gl_api();

	FLUX_THROW_IF( !gl, error::NoGLApiBound )
		<< einfo::ErrorDesc( "No OpenGL API bound to current thread" )
		<< einfo::ErrorMeta( FLUX_FMT_STRING("shader = '{}' ({})"), aIdent, shader_kind_(aKind) )
	;

	// Compile shader and check for errors
	FLUX_GL_CHECKPOINT_ALWAYS();
	Shader shader{ gl->createShader( aKind ) };

	GL::Int length = 0;
	if( ~std::size_t(0) != aSourceLength )
		length = util::narrow<GL::Int>( aSourceLength );

	gl->shaderSource( shader, 1, &aSource, &length );
	gl->compileShader( shader );

	GL::Int ok = 0;
	gl->getShaderiv( shader, GL::COMPILE_STATUS, &ok );
	if( !ok )
	{
		GL::Int logLength = 0;
		gl->getShaderiv( shader, GL::INFO_LOG_LENGTH, &logLength );

		ftl::Vector<GL::Char> shaderLog(logLength);
		gl->getShaderInfoLog( shader, logLength, &logLength, &shaderLog[0] );

        std::cout << shaderLog.data() << std::endl;
		FLUX_THROW( error::GLShaderCompileFailed )
			<< flux::einfo::ErrorDesc( "Failed to compile shader" )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("shader = '{}' ({}); Info Log = >>>\n{}\n<<<"), aIdent, shader_kind_(aKind), compat::string_view(shaderLog.data(),shaderLog.size()) )
		;
	}
	FLUX_GL_CHECKPOINT_ALWAYS();

	return shader;
}

Shader setup_shader( GL::Enum aKind, GL::Enum aFormat, compat::byte const* aData, std::size_t aSize, char const* aIdent )
{
	FLUX_ASSERT( aData );
	if( !aIdent ) aIdent = "(unknown shader)";

	namespace GL = gl::GL;
	auto const gl = gl::gl_api();

	FLUX_THROW_IF( !gl, error::NoGLApiBound )
		<< einfo::ErrorDesc( "No OpenGL API bound to current thread" )
		<< einfo::ErrorMeta( FLUX_FMT_STRING("shader = '{}' ({}, {})"), aIdent, shader_kind_(aKind), shader_format_(aFormat) )
	;
	FLUX_THROW_IF( !gl->VERSION_4_1, error::MissingExtension )
		<< einfo::ErrorDesc( FLUX_FMT_STRING("Insufficient OpenGL version (have {}.{}, need 4.1)"), gl->VERSION_loaded_major, gl->VERSION_loaded_minor )
		<< einfo::ErrorMeta( FLUX_FMT_STRING("shader = '{}' ({}, {})"), aIdent, shader_kind_(aKind), shader_format_(aFormat) )
	;
	FLUX_THROW_IF( !gl->ARB_gl_spirv, error::MissingExtension )
		<< einfo::ErrorDesc( "GL_ARB_gl_spirv not available" )
		<< einfo::ErrorMeta( FLUX_FMT_STRING("shader = '{}' ({}, {})"), aIdent, shader_kind_(aKind), shader_format_(aFormat) )
	;

	// Compile shader and check for errors
	FLUX_GL_CHECKPOINT_ALWAYS();
	Shader shader{ gl->createShader( aKind ) };

	gl->shaderBinary( 1, &shader, aFormat, aData, util::narrow<GL::Sizei>(aSize) );
	gl->specializeShaderARB( shader, "main", 0, nullptr, nullptr ); //XXX-loader ARB

	GL::Int ok = 0;
	gl->getShaderiv( shader, GL::COMPILE_STATUS, &ok );
	if( !ok )
	{
		GL::Int logLength = 0;
		gl->getShaderiv( shader, GL::INFO_LOG_LENGTH, &logLength );

		ftl::Vector<GL::Char> shaderLog(logLength);
		gl->getShaderInfoLog( shader, logLength, &logLength, &shaderLog[0] );

		FLUX_THROW( error::GLShaderCompileFailed )
			<< flux::einfo::ErrorDesc( "Failed to compile shader" )
			<< flux::einfo::ErrorMeta( FLUX_FMT_STRING("shader = '{}' ({}, {}); Info Log = >>>\n{}\n<<<"), aIdent, shader_kind_(aKind), shader_format_(aFormat), compat::string_view(shaderLog.data(),shaderLog.size()) )
		;
	}
	FLUX_GL_CHECKPOINT_ALWAYS();

	return shader;
}

//--    load_shader_from_vfs()              ///{{{2///////////////////////////
#if FLUXCFG_GL_WITH_FLUXIO
Shader load_shader_from_vfs( GL::Enum aKind, char const* aVfsPath, GL::Enum aFormat )
{
	FLUX_ASSERT( aVfsPath );

	namespace GL = gl::GL;
	auto const gl = gl::gl_api();

	FLUX_THROW_IF( !gl, error::NoGLApiBound )
		<< einfo::ErrorDesc( "No OpenGL API bound to current thread" )
		<< einfo::ErrorMeta( FLUX_FMT_STRING("shader = '{}' ({}, {})"), aVfsPath, shader_kind_(aKind), shader_format_(aFormat) )
	;

	// Load the shader code
	auto const& root = base::thread_catalog().acquire< base::def::ProcessVfs >();

	auto buf = io::vfs_open_buffer( root, aVfsPath );
	auto map = buf->map( io::EBufferMap::ro );
	FLUX_UTIL_ON_SCOPE_EXIT
	{
		buf->unmap( map );
	};

	// Compile shader
	if( 0 == aFormat )
		return setup_shader( aKind, reinterpret_cast<char const*>(map.data()), map.size(), aVfsPath );
	else
		return setup_shader( aKind, aFormat, map.data(), map.size(), aVfsPath );
}
#endif // ~CFG_GL_WITH_FLUXIO

//--    $ shader_*_()                       ///{{{2///////////////////////////
namespace
{
	std::string shader_kind_( GL::Enum aKind )
	{
		namespace GL = gl::GL;

		switch( aKind )
		{
			case GL::VERTEX_SHADER: return "vertex";
			case GL::FRAGMENT_SHADER: return "fragment";
			case GL::GEOMETRY_SHADER: return "geometry";
			case GL::COMPUTE_SHADER: return "compute";
			case GL::TESS_CONTROL_SHADER: return "tess. control";
			case GL::TESS_EVALUATION_SHADER: return "tess. eval";
		}

		return flux::format( FLUX_FMT_STRING("GL::Enum({:#x}"), unsigned(aKind) );
	}
	std::string shader_format_( GL::Enum aFormat )
	{
		namespace GL = gl::GL;

		switch( aFormat )
		{
			case 0: return "glsl";
			case GL::SHADER_BINARY_FORMAT_SPIR_V_ARB: return "spirv";
		}

		return flux::format( FLUX_FMT_STRING("GL::Enum({:#x})"), unsigned(aFormat) );
	}
}

//--    <<< ~ flux::gl namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(gl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
