/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	exception.hpp implementation
 */
/*-***************************************************************** -}}}1- */

#include <utility>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::error::detail >>>     ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(error,detail)
//--    ExceptAmalgamation                  ///{{{2///////////////////////////
template< class tOrg > inline
ExceptAmalgamation<tOrg>::ExceptAmalgamation( tOrg&& aOrg ) noexcept(std::is_nothrow_move_constructible<tOrg>::value)
	: tOrg(std::move(aOrg))
{}
template< class tOrg > inline
ExceptAmalgamation<tOrg>::ExceptAmalgamation( tOrg const& aOrg ) noexcept(std::is_nothrow_copy_constructible<tOrg>::value)
	: tOrg(aOrg)
{}

//--    enable_einfo()                      ///{{{2///////////////////////////
template< class tExcept > 
FLUX_ENABLE_IF( std::is_base_of<Exception,compat::remove_reference_t<tExcept>> )
(tExcept&&) enable_einfo( tExcept&& aExcept ) noexcept
{
	return std::forward<tExcept>(aExcept);
}

template< class tExcept >
FLUX_DISABLE_IF( std::is_base_of<Exception,compat::remove_reference_t<tExcept>> )
(ExceptAmalgamation<compat::decay_t<tExcept>>) enable_einfo( tExcept&& aExcept )
{
	return ExceptAmalgamation<compat::decay_t<tExcept>>(std::forward<tExcept>(aExcept));
}

//--    <<< ~ flux::error::detail namespace <<<     ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(error,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
