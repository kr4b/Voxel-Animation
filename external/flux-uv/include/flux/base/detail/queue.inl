/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- Queue implementation
 */
/*-***************************************************************** -}}}1- */

#include <typeinfo>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::base >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(base)
//--    Queue                               ///{{{2///////////////////////////
template< typename tType > inline
auto Queue::action( Identity<tType> ) -> Action&
{
	return action( std::type_index(typeid(tType)) );
}

template< typename tType > inline
auto Queue::action_for_origin( Endpoint aOrg, Identity<tType> ) -> Action&
{
	return action_for_origin( aOrg, std::type_index(typeid(tType)) );
}

template< typename tType > inline
auto Queue::action_for_dest( Endpoint aDst, Identity<tType> ) -> Action&
{
	return action_for_dest( aDst, std::type_index(typeid(tType)) );
}

//--    <<< ~ flux::base namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(base)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
