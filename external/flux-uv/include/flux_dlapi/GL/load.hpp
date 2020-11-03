/*-******************************************************* -- HEADER -{{{1- */
/*- flux-dlapi_gl API loading
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUXDLAPI_GL_LOAD_HPP_15F7CA16_74B2_4FAF_AE66_8B2F391899E2
#define FLUXDLAPI_GL_LOAD_HPP_15F7CA16_74B2_4FAF_AE66_8B2F391899E2

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include "forward.hpp"

//--    >>> namespace = dlapi::gl >>>           ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(dlapi,gl)
//--    functions                       ///{{{2///////////////////////////////

/** Load OpenGL API
 *
 * Load OpenGL API using the \a aGetFn` and \a aCheckExt` callbacks up to the
 * version `aMajor.aMinor`. Extensions may optionally be limited to the list
 * specified.
 *
 * The default values (`aMajor = -1`, `aMinor = -1` and `aExtList = NULL`) 
 * will attempt to load any version and all known extensions.
 *
 * \note `load_gl()` will load <b>up to</b> the specified versions, but will
 * return a `GLapi` pointer even if the requested version/extensions are
 * <b>not</b> available. If a specific version is <i>required</i>, the user
 * <b>must</b> check the `VERSION_loaded_major`/`VERSION_loaded_minor` or the
 * `VERSION_X_Y_core` fields! The same applies to extensions -- `load_gl()`
 * will only attempt to load the requested extensions, but will return
 * successfully even if these are not available.
 *
 * The returned `GLapi` structure has a reference count of one, and has its
 * `valid` field set to true.
 *
 * \see unload_gl(), acquire_gl(), release_gl() and invalidate_gl().
 */
GLapi const* load_gl(
	bool (*aCheckExt)( char const* aExtName, void* aUser ),
	void* (*aGetFn)( char const* aFnName, void* aUser ),
	void* aUser,
	int aMajor = -1, int aMinor = -1,
	char const* aExtList = nullptr
);

/** Unload OpenGL API
 *
 * Unloads the OpenGL API previously loaded by `load_gl()`. Unloading decreases
 * the reference count and frees internal memory if it reaches zero. In contrast
 * to `release_gl()`, `unload_gl()` further marks the API as unloaded by setting
 * the `valid` field of the struct to false.
 *
 * \see acquire_gl(), release_gl()
 */
void unload_gl( GLapi const* );


/** Increment reference count
 *
 * Increments the `GLapi`'s reference count by one, preventing it from being
 * freed until the count reaches zero.
 *
 * \note A `GLapi` pointer can be held by an object by calling `acquire_gl()`.
 * However, since the `GLapi` operates on a GL context, the context might be
 * invalid regardless. This is indicated by the `valid` field of the `GLapi`.
 * It is recommended that the `valid` field is checked at each use if the
 * `GLapi` pointer is retained in this manner.
 * 
 * \see `release_gl()`
 */
void acquire_gl( GLapi const* );
/** Decrement reference count
 *
 * Decrement `GLapi` reference counter by one, freeing internal memory if it
 * reaches zero.
 *
 * \see `acquire_gl()`
 */
void release_gl( GLapi const* );

//--    <<< ~ dlapi::gl namespace <<<           ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(dlapi,gl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUXDLAPI_GL_LOAD_HPP_15F7CA16_74B2_4FAF_AE66_8B2F391899E2
