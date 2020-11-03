/*-******************************************************* -- HEADER -{{{1- */
/*-	OpenGL API 
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_GL_GLAPI_HPP_E1106874_0246_4BA8_9115_21AB6E90815D
#define FLUX_GL_GLAPI_HPP_E1106874_0246_4BA8_9115_21AB6E90815D

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux_dlapi/GL/gl.hpp>

//--    >>> namespace = flux::gl >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(gl)
//--    aliases                             ///{{{2///////////////////////////

using ::flux::dlapi::gl::GLapi;
namespace GL = ::flux::dlapi::gl::GL;

//--    $ detail                            ///{{{2///////////////////////////
namespace detail
{
	struct ThreadGLApi final
	{
		GLapi const* api;
		void const* key;

		static ThreadGLApi& instance() noexcept;

		private:
			ThreadGLApi() noexcept, ~ThreadGLApi();
	};
};

//--    functions                           ///{{{2///////////////////////////

/** Get current `GLapi` instance
 *
 * Get the thread's currently bound `GLapi` instance. The `GLapi` instance is
 * specified per-thread and can be modified using `set_gl_api()`.
 */
GLapi const* gl_api() noexcept;

/** Get associated API key
 *
 * The key identifies the setter of the currently bound `GLapi` instance. The
 * key may be set together with the `GLapi` instance using `set_gl_api()`. It
 * is treated as an opaque value (i.e., the pointer is never dereferenced).
 *
 * \see `set_gl_api()`
 */
void const* gl_api_key() noexcept;

/** Set current `GLapi` instance
 *
 * Set the thread's current `GLapi` instance. Future calls to `gl_api()` will
 * return the specified value.
 *
 *`set_gl_api()` will `acquire_gl()` on the `GLapi`, incrementing the reference
 * count of the `GLapi` instance by one. `release_gl()` is automatically called
 * at the following two points: (a) the thread is about to exit, (b) the
 * current `GLapi` instance is being reset via `set_gl_api()`.
 *
 * It is valid to pass a `nullptr` to `set_gl_api()` to indicate that no
 * `GLapi` is available at this time. 
 *
 * `set_gl_api()` optionally takes an API key that identifies the current
 * setter of the `GLapi` instance. Flux objects that bind the `GLapi` (such as
 * `gl::Context`) will set a key, and will only automatically unbind the
 * current `GLapi` onif the key still matches.
 * 
 * \see gl_api()
 */
void set_gl_api( GLapi const*, void const* = nullptr ) noexcept;

//--    <<< ~ flux::gl namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(gl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/glapi.inl"
#endif // FLUX_GL_GLAPI_HPP_E1106874_0246_4BA8_9115_21AB6E90815D
