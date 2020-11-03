/*-******************************************************* -- HEADER -{{{1- */
/*- OpenGL checkpoints
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_GL_CHECKPOINT_HPP_0864FC7E_03BC_41BE_A76B_3B4341044FBC
#define FLUX_GL_CHECKPOINT_HPP_0864FC7E_03BC_41BE_A76B_3B4341044FBC

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/gl/glapi.hpp>

#include <flux/util/attrib.hpp>

//--    >>> namespace = flux::gl >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(gl)
//--    macros                              ///{{{2///////////////////////////

#define FLUX_GL_CHECKPOINT_ALWAYS()    FLUX_DEFINED_ELSEWHERE
#define FLUX_GL_CHECKPOINT_DEBUG()     FLUX_DEFINED_ELSEWHERE

//--    functions                           ///{{{2///////////////////////////
namespace detail
{
	FLUX_ATTR_NORETURN
	void throw_gl_checkpoint_error(
		char const*,
		char const*,
		int, 
		int 
	);
}

//--    <<< ~ flux::gl namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(gl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/checkpoint.inl"
#endif // FLUX_GL_CHECKPOINT_HPP_0864FC7E_03BC_41BE_A76B_3B4341044FBC
