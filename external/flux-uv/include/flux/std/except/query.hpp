/*-******************************************************* -- HEADER -{{{1- */
/*-	Query exception info
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_EXCEPT_QUERY_HPP_88453F38_949F_4E2B_AC25_41B0E8B8F153
#define FLUX_STD_EXCEPT_QUERY_HPP_88453F38_949F_4E2B_AC25_41B0E8B8F153

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/optional.hpp>
#include <flux/std/enable_if.hpp>


#include <string>

#include "einfo.hpp"

//--    >>> namespace = flux::einfo >>>             ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(einfo)
//--    functions                           ///{{{2///////////////////////////

template< class tInfo, class tExcept >
FLUX_ENABLE_IF( std::is_base_of<error::Exception, tExcept> )
(optional<typename tInfo::type>) query_error_info( tExcept const& );
template< class tInfo, class tExcept >
FLUX_DISABLE_IF( std::is_base_of<error::Exception, tExcept> )
(optional<typename tInfo::type>) query_error_info( tExcept const& );

template< class tExcept >
FLUX_ENABLE_IF( std::is_base_of<error::Exception, tExcept> )
(std::string) query_diagnostic_info( tExcept const& );
template< class tExcept >
FLUX_DISABLE_IF( std::is_base_of<error::Exception, tExcept> )
(std::string) query_diagnostic_info( tExcept const& );

template< class tExcept >
FLUX_ENABLE_IF( std::is_base_of<error::Exception, tExcept> )
(char const*) query_diagnostic_info_what( tExcept const& ) noexcept;
template< class tExcept >
FLUX_DISABLE_IF( std::is_base_of<error::Exception, tExcept> )
(char const*) query_diagnostic_info_what( tExcept const& ) noexcept;

namespace detail
{
	std::string query_diagnostic_info_( error::Exception const& );
	char const* query_diagnostic_info_what_( error::Exception const& ) noexcept;

	std::string query_diagnostic_info_( std::exception const& );
	char const* query_diagnostic_info_what_( std::exception const& ) noexcept;
}

//--    <<< ~ flux::einfo namespace <<<             ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(einfo)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "../detail/except-query.inl"
#endif // FLUX_STD_EXCEPT_QUERY_HPP_88453F38_949F_4E2B_AC25_41B0E8B8F153
