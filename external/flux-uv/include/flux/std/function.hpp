/*-******************************************************* -- HEADER -{{{1- */
/*-	flux::function() && flux::unique_function
 *
 * A replacement for `std::function` with a few nice bonus properties, such
 * as being noexcept movable.
 *
 * TODO: fall back to std::function for VS2013??
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_FUNCTION_HPP_BC39DE60_39A0_4321_9262_97BF1067F184
#define FLUX_STD_FUNCTION_HPP_BC39DE60_39A0_4321_9262_97BF1067F184

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/namespace.hpp>
#include <flux/setup/compiler.hpp>

#if FLUX_COMPILER != FLUX_COMPILER_MSVC || _MSC_VER > 1800 // not VisualStudio 2013
#	include <flux_ext/cxx_function.hpp>
#else // VisualStudio 2013
#	include <functional>
#endif // ~ VisualStudio 2013

//--    >>> namespace = flux >>>                    ///{{{1///////////////////
FLUX_NAMESPACE_ENTER()
//--    function                            ///{{{2///////////////////////////

#if FLUX_COMPILER != FLUX_COMPILER_MSVC || _MSC_VER > 1800 // not VisualStudio 2013
using flux_ext::function;
using flux_ext::unique_function;
#else // VisualStudio 2013
using ::std::function;

// The following is technically a lie, since `std::function` is not an
// unique function. But it seems to mostly work.
template< typename... tWhatevs >
using unique_function = ::std::function<tWhatevs...>;
#endif // ~ VisualStudio 2013

#if defined(__DOXYGEN__)
/** Replacement for `std::function`
 *
 * `flux::function` is a drop-in replacement for `std::function` with a 
 * few additional properties, mainly it being `noexcept' movable. 
 *
 * `flux::function` is an alias to `cxx_function::function`. See <a
 * href="https://github.com/potswa/cxx_function">cxx_function</a> for more
 * information.
 *
 * \note On VisualStudio 2013, this falls back to `std::function`, as the
 * `cxx_function` does not compile under VS2013.
 */
template< typename... tSig >
class function;

/** Unique function, a non-copyable version of `function`
 *
 * `flux::unqiue_function` is a non-copyable (but movable) version of 
 * `flux::function`.
 *
 * `flux::unique_function` is an alias to `cxx_function::unique_function`. See
 * <a href="https://github.com/potswa/cxx_function">cxx_function</a> for more
 * information.
 *
 * \note On VisualStudio 2013, this falls back to `std::function`, as the
 * `cxx_function` does not compile under VS2013. This is furthermore
 * technically incorrect, but the properties of `unique_function` are not
 * strictly relied on in flux at the moment.
 */
template< typename... tSig >
class unique_function;
#endif // ~ defined(__DOXYGEN__)

//--    <<< ~ flux namespace <<<                    ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE()
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_STD_FUNCTION_HPP_BC39DE60_39A0_4321_9262_97BF1067F184
