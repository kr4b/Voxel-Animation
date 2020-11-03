/*-******************************************************* -- HEADER -{{{1- */
/*-	Aliases for Standard OpenGL constant and function names
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_GL_GLALIASES_HPP_6203B165_ABDD_4582_920C_AE98E20F29E3
#define FLUX_GL_GLALIASES_HPP_6203B165_ABDD_4582_920C_AE98E20F29E3

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/gl/glapi.hpp>

//--    >>> namespace = flux::gl >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(gl)
//--    setup                               ///{{{2///////////////////////////

/* gl-aliases.hpp below uses this to fetch the function aliases. 
 *
 * For example, glClear() would exapnd to FLUXGL_GETFN(clear)().
 */
#define FLUXGL_GETFN(fn) (::flux::gl::gl_api()->fn)

//--    include                             ///{{{2///////////////////////////

/* #defines for glClear() and GL_ONE etc are defined below. */
#include <flux_dlapi/GL/gl-aliases.hpp>

//--    <<< ~ flux::gl namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(gl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_GL_GLALIASES_HPP_6203B165_ABDD_4582_920C_AE98E20F29E3
