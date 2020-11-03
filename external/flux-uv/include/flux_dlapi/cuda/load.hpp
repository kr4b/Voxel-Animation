/*-******************************************************* -- HEADER -{{{1- */
/*- flux-dlapi_cuda API loading
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUXDLAPI_CUDA_LOAD_HPP_60561129_AABD_4467_9354_F7C2197B1286
#define FLUXDLAPI_CUDA_LOAD_HPP_60561129_AABD_4467_9354_F7C2197B1286

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include "forward.hpp"

//--    >>> namespace = dlapi::cuda >>>         ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(dlapi,cuda)
//--    functions                       ///{{{2///////////////////////////////

/** \name Load CUDA API
 *
 * Load CUDA API `CUapi`. Uses `aGetFn` to retrieve individual function 
 * pointers. The default version simply uses the platform's standard dynamic
 * loader mechanism.
 *
 * The returned `CUapi` structure has a reference count of one, and its `valid`
 * field is set to true.
 *
 * \see unload_cuda(), acquire_cuda(), release_cuda()
 */
//@{
CUapi const* load_cuda();
CUapi const* load_cuda(
	void* (*aGetFn)( char const* aFnName, void* aUser ),
	void* aUser
);
//@}

/** Unload CUDA API
 *
 * Unloads the CUDA API previously loaded by `load_cuda()`. Unloading decreases
 * the reference count and frees internal memory if it reaches zero. Further, 
 * the `valid` field of the struct is set to false, to indicate that contexts
 * associated with this `CUapi` instance are no longer available.
 *
 * 
 * \see acquire_cuda(), release_cuda()
 */
void unload_cuda( CUapi const* );


/** Increment reference count
 *
 * Increments the `CUapi`'s reference count by one, preventing it from being
 * freed until the count reaches zero.
 *
 * \note A `CUapi` pointer can be held by an object by calling `acquire_gl()`.
 * However, since the `CUapi` operates on a CUDA context, the context might be
 * invalid regardless. This is indicated by the `valid` field of the `CUapi`.
 * It is recommended that the `valid` field is checked at each use if the
 * `CUapi` pointer is retained in this manner.
 * 
 * \see `release_cuda()`
 */
void acquire_cuda( CUapi const* );
/** Decrement reference count
 *
 * Decrement `CUapi` reference counter by one, freeing internal memory if it
 * reaches zero.
 *
 * \see `acquire_cuda()`
 */
void release_cuda( CUapi const* );

//--    <<< ~ dlapi::cuda namespace <<<         ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(dlapi,cuda)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUXDLAPI_CUDA_LOAD_HPP_60561129_AABD_4467_9354_F7C2197B1286
