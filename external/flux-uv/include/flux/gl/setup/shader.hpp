/*-******************************************************* -- HEADER -{{{1- */
/*-	Helpers for loading/compiling shader
 *
 * EXPERIMENTAL!
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_GL_SETUP_SHADER_HPP_CAFD394D_22CF_4837_B181_0E1E46EEC0A7
#define FLUX_GL_SETUP_SHADER_HPP_CAFD394D_22CF_4837_B181_0E1E46EEC0A7

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/gl/glapi.hpp>
#include <flux/gl/objects.hpp>
#include <flux/gl/defaults.hpp>

#include <flux/compat/byte.hpp>

//--    >>> namespace = flux::gl >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(gl)
//--    functions                           ///{{{2///////////////////////////

/** Setup shader from GLSL source
 *
 * TODO
 */
Shader setup_shader( 
	GL::Enum aShaderKind, 
	char const* aSource, std::size_t aSourceLength = ~std::size_t(0), 
	char const* aShaderIdent = nullptr
);

/** Setup shader from binary data
 *
 * TODO
 */
Shader setup_shader(
	GL::Enum aShaderKind, GL::Enum aBinaryFormat,
	compat::byte const*, std::size_t,
	char const* aShaderIdent 
);

#if FLUXCFG_GL_WITH_FLUXIO
Shader load_shader_from_vfs( GL::Enum, char const*, GL::Enum aFormat = 0 );
#endif // ~CFG_GL_WITH_FLUXIO

//--    <<< ~ flux::gl namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(gl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_GL_SETUP_SHADER_HPP_CAFD394D_22CF_4837_B181_0E1E46EEC0A7
