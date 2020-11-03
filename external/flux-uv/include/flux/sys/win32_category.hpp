/*-******************************************************* -- HEADER -{{{1- */
/*-	Win32 error category
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SYS_WIN32_CATEGORY_HPP_D853D063_61FD_43D3_905E_94ED0860BC55
#define FLUX_SYS_WIN32_CATEGORY_HPP_D853D063_61FD_43D3_905E_94ED0860BC55

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/compat/system_error.hpp>

//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    functions                   ///{{{1///////////////////////////////////

/** Error category: Win32 errors
 *
 * `compat::error_category` for Win32 errors. 
 *
 * This is an error category for Win32 error codes, as returned by
 * `GetLastError()` and similar.
 *
 * \note Uses (obviously) Windows-only functions. On non-Windows platforms, the
 * `win32_category()` implementation will be largely stubbed out.
 */
compat::error_category const& win32_category() noexcept;


compat::error_code make_win32_error_code() noexcept;
compat::error_code make_win32_error_code( unsigned long ) noexcept;

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_SYS_WIN32_CATEGORY_HPP_D853D063_61FD_43D3_905E_94ED0860BC55
