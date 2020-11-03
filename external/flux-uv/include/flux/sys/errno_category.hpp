/*-******************************************************* -- HEADER -{{{1- */
/*-	`errno` error category
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SYS_ERRNO_CATEGORY_HPP_E9963276_42D8_453E_9EA8_344ADB1072AF
#define FLUX_SYS_ERRNO_CATEGORY_HPP_E9963276_42D8_453E_9EA8_344ADB1072AF

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/compat/system_error.hpp>

//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    functions                   ///{{{1///////////////////////////////////

/** Error category: `errno` errors
 *
 * `compat::error_category` for `errno` errors. 
 *
 * This is largely similar to the `compat::system_category()` on Linux (and
 * similar systems). Unlike the `system_category()` it is however guaranteed to
 * handle `errno` errors, even on Windows (which has a few POSIX compatibility
 * functions).
 *
 * Further, it always maps errors to the `generic_category()` via the
 * `default_error_condition()` method. (Something that `system_category()` is
 * not required to do.)
 */
compat::error_category const& errno_category() noexcept;

compat::error_code make_errno_error_code() noexcept;
compat::error_code make_errno_error_code( int ) noexcept;

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_SYS_ERRNO_CATEGORY_HPP_E9963276_42D8_453E_9EA8_344ADB1072AF
