/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	except/query.hpp implementation
 */
/*-***************************************************************** -}}}1- */

#include "except_data.hxx"

#include <typeinfo>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::einfo >>>             ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(einfo)
//--    query_einfo()                       ///{{{2///////////////////////////
template< class tInfo, class tExcept > inline
FLUX_ENABLE_IF( std::is_base_of<error::Exception, tExcept> )
(optional<typename tInfo::type>) query_error_info( tExcept const& aExcept )
{
	auto const& edata = error::detail::exception_data(aExcept);
	return edata.template get<tInfo>();
}

template< class tInfo, class tExcept > inline
FLUX_DISABLE_IF( std::is_base_of<error::Exception, tExcept> )
(optional<typename tInfo::type>) query_error_info( tExcept const& aExcept )
{
	if( auto ptr = dynamic_cast<error::Exception const*>(&aExcept) )
		return query_error_info<tInfo>( *ptr );

	return nullopt;
}

//--    query_diagnostic_info()             ///{{{2///////////////////////////
template< class tExcept > inline
FLUX_ENABLE_IF( std::is_base_of<error::Exception, tExcept> )
(std::string) query_diagnostic_info( tExcept const& aEx )
{
	return detail::query_diagnostic_info_( static_cast<error::Exception const&>(aEx) );
}
template<  class tExcept > inline
FLUX_DISABLE_IF( std::is_base_of<error::Exception, tExcept> )
(std::string) query_diagnostic_info( tExcept const& aExcept )
{
	if( auto ptr = dynamic_cast<error::Exception const*>(&aExcept) )
		return detail::query_diagnostic_info_( *ptr );
	else if( auto ptr = dynamic_cast<std::exception const*>(&aExcept) )
		return detail::query_diagnostic_info_( *ptr );

	return typeid(aExcept).name();
}

//--    query_diagnostic_info_what()        ///{{{2///////////////////////////
template< class tExcept > inline
FLUX_ENABLE_IF( std::is_base_of<error::Exception, tExcept> )
(char const*) query_diagnostic_info_what( tExcept const& aEx ) noexcept
{
	return detail::query_diagnostic_info_what_( static_cast<error::Exception const&>(aEx) );
}
template< class tExcept > inline
FLUX_DISABLE_IF( std::is_base_of<error::Exception, tExcept> )
(char const*) query_diagnostic_info_what( tExcept const& aExcept ) noexcept
{
	if( auto ptr = dynamic_cast<error::Exception const*>(&aExcept) )
		return detail::query_diagnostic_info_what_( *ptr );
	else if( auto ptr = dynamic_cast<std::exception const*>(&aExcept) )
		return detail::query_diagnostic_info_what_( *ptr );

	return typeid(aExcept).name();
}

//--    <<< ~ flux::einfo namespace <<<             ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(einfo)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
