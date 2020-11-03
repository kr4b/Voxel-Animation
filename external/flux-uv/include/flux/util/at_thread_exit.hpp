/*-******************************************************* -- HEADER -{{{1- */
/*- at_thread_exit()
 *
 * Register callbacks to be called at thread exit. Technically, it's trivial
 * under C++11 with the `thread_local`. However, GCC doesn't support it until
 * 4.8+, clang requires GCC's libstdc++ to support it (and so, also depends on
 * GCC 4.8+ technically). MSVC supports thread_local only for primitive types
 * without destructors.
 *
 * So...
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_UTIL_AT_THREAD_EXIT_HPP_64001147_83BF_4127_A77F_64E7E7BE513C
#define FLUX_UTIL_AT_THREAD_EXIT_HPP_64001147_83BF_4127_A77F_64E7E7BE513C

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <functional>

//--    >>> namespace = flux::util >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(util)
//--    types                               ///{{{2///////////////////////////

typedef std::function<void ()> AtThreadExitCallback;

//--    at_thread_exit()                    ///{{{2///////////////////////////

/** Call function at thread exit
 *
 * Call function when the current thread exits. Registered functions are 
 * called in the reverse order of them being added. This also applies to the
 * main thread, albeit the order in which callbacks added via `std::atexit()`
 * and `at_thread_exit()` is unspecified (they may be called interleaved, as
 * one of the implementations use `atexit()` on the main thread).
 *
 * \note Callbacks may not throw.
 */
void at_thread_exit( AtThreadExitCallback const& );

//--    <<< ~ flux::util namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(util)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_UTIL_AT_THREAD_EXIT_HPP_64001147_83BF_4127_A77F_64E7E7BE513C
