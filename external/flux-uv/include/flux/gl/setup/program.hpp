/*-******************************************************* -- HEADER -{{{1- */
/*-	Helpers for loading/linking programs
 *
 * EXPERIMENTAL!
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_GL_SETUP_PROGRAM_HPP_7142A4BC_A6D6_4C74_8241_B9FCF786A30F
#define FLUX_GL_SETUP_PROGRAM_HPP_7142A4BC_A6D6_4C74_8241_B9FCF786A30F

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/gl/glapi.hpp>
#include <flux/gl/objects.hpp>
#include <flux/gl/defaults.hpp>

#include <flux/ftl/vector.hpp>

#include <tuple>
#include <initializer_list>

//--    >>> namespace = flux::gl >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(gl)
//--    functions                           ///{{{2///////////////////////////

Program setup_program(
	ftl::VectorBase<GL::UInt> const&,
	char const* aProgramIdent = nullptr
);
Program setup_program(
	ftl::VectorBase<Shader> const&,
	char const* aProgramIdent = nullptr
);
Program setup_program( 
	std::initializer_list<GL::UInt>,
	char const* aProgramIdent = nullptr
);

#if FLUXCFG_GL_WITH_FLUXIO
Program load_program_from_vfs( 
	ftl::VectorBase< std::tuple<GL::Enum, char const*> > const&, 
	GL::Enum aFormat = 0
);
Program load_program_from_vfs( 
	std::initializer_list< std::tuple<GL::Enum, char const*> >, 
	GL::Enum aFormat = 0 
);
#endif // ~CFG_GL_WITH_FLUXIO

//--    <<< ~ flux::gl namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(gl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_GL_SETUP_PROGRAM_HPP_7142A4BC_A6D6_4C74_8241_B9FCF786A30F
