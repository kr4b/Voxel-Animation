/*-******************************************************* -- HEADER -{{{1- */
/*-	getaddrinfo error category
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SYS_GAI_CATEGORY_HPP_1378E94B_B026_466E_BA6A_A973C78E867E
#define FLUX_SYS_GAI_CATEGORY_HPP_1378E94B_B026_466E_BA6A_A973C78E867E

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/compat/system_error.hpp>

//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    functions                   ///{{{1///////////////////////////////////

/** Error category: `getaddrinfo()`-related errors
 *
 * `compat::error_category` for `getaddrinfo()` errors. 
 *
 * Note that the implementation of the `gai_category()` depends on the
 * underlying system. On non-Windows platforms, the implementation uses the
 * system error codes and `gai_strerror()`. On Windows, it relies on the
 * `dlapi::os::Ws2`/`dlapi::os::Ws2api` code and definitions.
 *
 * \warning In particular, on Windows, the error messages may display numeric
 * errors and not resolve to a string if either the Win32 API or the Ws2 API
 * wasn't previously loaded.
 */
compat::error_category const& gai_category() noexcept;

compat::error_code make_gai_error_code( int ) noexcept;

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_SYS_GAI_CATEGORY_HPP_1378E94B_B026_466E_BA6A_A973C78E867E
