/*-******************************************************* -- HEADER -{{{1- */
/*-	System Path types
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SYS_SYSPATH_HPP_7CAF4BD6_46E2_4BC3_8080_40C250931B54
#define FLUX_SYS_SYSPATH_HPP_7CAF4BD6_46E2_4BC3_8080_40C250931B54

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/platform.hpp>

#include <flux/compat/string_view.hpp>

#include <string>

//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    types                       ///{{{1///////////////////////////////////

#if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
using SysPathChar = char;
#else // PLATFORM_WIN32
using SysPathChar = wchar_t;
#endif // ~ PLATFORM

using SysPathStr = std::basic_string<SysPathChar>;
using SysPathView = compat::basic_string_view<SysPathChar>;

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_SYS_SYSPATH_HPP_7CAF4BD6_46E2_4BC3_8080_40C250931B54
