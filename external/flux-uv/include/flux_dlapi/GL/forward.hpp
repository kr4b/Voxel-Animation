/*-******************************************************* -- HEADER -{{{1- */
/*- flux-dlapi_gl forward declarations
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUXDLAPI_GL_FORWARD_HPP_A6DCAB6D_E368_4B10_94D9_F31E0F98D66E
#define FLUXDLAPI_GL_FORWARD_HPP_A6DCAB6D_E368_4B10_94D9_F31E0F98D66E

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

//--    >>> namespace = dlapi::gl >>>           ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(dlapi,gl)
//--    types                           ///{{{2///////////////////////////////

/** OpenGL API functions
 *
 * Contains OpenGL API function pointers, and additional variables that 
 * indicate which parts of the OpenGL API were loaded.
 *
 * **OpenGL API functions**:
 * OpenGL API functions are named as follows:
 *   - the `gl` prefix is stripped of the function's standard name
 *   - the first letter of the function is lowercased.
 *
 * Given a pointer `GLapi const* gl`, the following occurs:
 *
 *   - `glCreateProgram` becomes `gl->createProgram()`
 *   - `glNamedProgramUniformi( prog, loc, 1 ) becomes 
 *     `gl->namedProgramUniformi( prog, loc, 1 )`.
 *
 * **Additional variables**:
 * Further, the following additional variables are defined:
 *   - `bool VERSION_x_y`
 *   - `bool VERSION_x_y_compatibility`
 *   - `bool ARB_foo_bar`, `bool EXT_foo_bar`, `bool VEND_foo_bar`, ...
 *   - `int VERSION_loaded_major`, `int VERSION_loaded_minor`
 *
 * The boolean variables indicate whether or not the core profile,
 * compatibility profile or extension is available (i.e., it was advertised as
 * available and its function entry points were successfully retrieved).
 *
 * The integer variables indicate the highest OpenGL version that was fully
 * and successfully loaded.
 */
struct GLapi;
/** GL constants namespace
 *
 * All GL constants are defined in this namespace, as part of an C++03 untyped
 * enumeration. The constants are named as follows:
 *
 *   - the `GL_` prefix is stripped of the constant's standard name.
 *   - some special constants are named as detailed below to avoid collisions
 *     (see Section *Special Names*)
 *
 * Examples:
 *
 *   - `GL_DEPTH_TEST` becomes `GL::DEPTH_TEST`
 *   - `GL_TEXTURE_2D` becomes `GL::TEXTURE_2D`
 *   - `GL_RGBA` becomes `GL::RGBA`.
 *   - `GL_TRUE` becomes `GL::GLTRUE` (see special names below)
 *   - `GL_FALSE` becomes `GL::GLFALSE` (see special names below)
 *   - `GL_NO_ERROR` becomes `GL::GLNO_ERROR` (see special names below)
 *
 * **Special names:**
 *
 * The following constants are renamed to avoid collisions with `#%defines`
 * from several common headers:
 *
 *   - `GL_TRUE` --> `GL::GLTRUE` (`<windows.h>` defines `TRUE` and `FALSE`)
 *   - `GL_FALSE` --> `GL::GLFALSE` (`<windows.h>` defines `TRUE` and `FALSE`)
 *   - `GL_DOMAIN` --> `GL::GLDOMAIN` (`<math.h>` defines `DOMAIN` under a 
 *      some circumstances)
 *   - `GL_NO_ERROR` -->  `GL::GLNO_ERROR` (`<windows.h>` defines `NO_ERROR`)
 *   - `GL_WAIT_FAILED` -->  `GL::GLWAIT_FAILED` (`<winbase.h>` defines `WAIT_FAILED`)
 *
 * Constants whose name would start with a number are also prefixed with `GL`,
 * so, for example, `GL_2D` --> `GL::GL2D`. Fortunately few of these constants
 * are commonly used in modern OpenGL...
 */
namespace GL {};

//--    <<< ~ dlapi::gl namespace <<<           ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(dlapi,gl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUXDLAPI_GL_FORWARD_HPP_A6DCAB6D_E368_4B10_94D9_F31E0F98D66E
