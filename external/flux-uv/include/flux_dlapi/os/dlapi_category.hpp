/*-******************************************************* -- HEADER -{{{1- */
/*-	DLAPI error category
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUXDLAPI_OS_DLAPI_CATEGORY_HPP_5EC8E975_45AC_4CF4_BE21_B52BA7F27035
#define FLUXDLAPI_OS_DLAPI_CATEGORY_HPP_5EC8E975_45AC_4CF4_BE21_B52BA7F27035

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/compat/system_error.hpp>

//--    >>> namespace = flux::dlapi::os >>>         ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(dlapi,os)
//--    constants                   ///{{{1///////////////////////////////////
enum class EDLAPIError : int
{
	success      = 0,

	unavailable,
	notLoaded
};

//--    functions                   ///{{{1///////////////////////////////////

/** Error category: dlapi-related errors
 */
compat::error_category const& dlapi_category() noexcept;

compat::error_code make_dlapi_error_code( EDLAPIError ) noexcept;

//--    <<< ~ flux::dlapi::os namespace <<<         ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(dlapi,os)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUXDLAPI_OS_DLAPI_CATEGORY_HPP_5EC8E975_45AC_4CF4_BE21_B52BA7F27035
