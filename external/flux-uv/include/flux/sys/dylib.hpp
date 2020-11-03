/*-******************************************************* -- HEADER -{{{1- */
/*-	Dynamic Library Loading
 *
 * TODO: integrate with paths.hpp
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SYS_DYLIB_HPP_EC880D14_90AB_49AF_8F30_83DD61740908
#define FLUX_SYS_DYLIB_HPP_EC880D14_90AB_49AF_8F30_83DD61740908

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/sys/syspath.hpp>

//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    types                       ///{{{1///////////////////////////////////

namespace detail
{
	struct DylibHandleType;
};

/** Dynamically loaded library handle
 *
 * Handle to a dynamically loaded library/shared object. This opaque type is
 * `reinterpret_cast`able to and from a `HMODULE` (Windows) or a `void*` 
 * (Linux) for use with the native APIs.
 */
using DylibHandle = detail::DylibHandleType*;
using DylibProc = void (*)();

//--    functions                   ///{{{1///////////////////////////////////

/** Open library
 * 
 * Open a dynamically loaded library/shared object by name. Returns a 
 * `DylibHandle` on success, and `nullptr` otherwise. Handles must be closed
 * via `dylib_close()` after use.
 */
auto dylib_open( char const* ) -> DylibHandle;
/** \copydoc dylib_open() */
auto dylib_open( SysPathStr const& ) -> DylibHandle;
/** Close library
 *
 * Close a library previously loaded via `dylib_open()`.
 */
void dylib_close( DylibHandle );

/** Get pointer to variable
 *
 * Get a pointer to a variable symbol with the given name. For functions, the
 * `dylib_get_proc()` method should be used instead.
 */
auto dylib_get_var( DylibHandle, char const* ) -> void*;
/** Get pointer to a function
 *
 * Get a pointer to a function symbol with the given name. For variables, the
 * `dylib_get_var()` method should be used instead.
 */
auto dylib_get_proc( DylibHandle, char const* ) -> DylibProc;

/** Get pointer to a symbol
 *
 * Get pointer to a symbol with the given name. Will, based on \a tType, call
 * either `dylib_get_var()` (\a tType is not a function pointer) or
 * `dylib_get_proc()` (\a tType is a function pointer).
 */
template< typename tType >
auto dylib_get_sym( DylibHandle, char const* ) -> tType;

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/dylib.inl"
#endif // FLUX_SYS_DYLIB_HPP_EC880D14_90AB_49AF_8F30_83DD61740908
