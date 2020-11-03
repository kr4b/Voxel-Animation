/*-******************************************************* -- HEADER -{{{1- */
/*-	std::uninitialized_* fallbacks
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_NIH_UNINITIALIZED_HPP_C283207D_E81A_4126_BBC5_DBB3E20DD5EF
#define FLUX_NIH_UNINITIALIZED_HPP_C283207D_E81A_4126_BBC5_DBB3E20DD5EF

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <utility>
#include <cstddef>

//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    uninitialized_*()           ///{{{1///////////////////////////////////

template< typename tInput, typename tForward >
tForward uninitialized_move( tInput, tInput, tForward );
template< typename tInput, typename tForward >
std::pair<tInput,tForward> uninitialized_move_n( tInput, std::size_t, tForward );

template< typename tForward >
void uninitialized_value_construct( tForward, tForward );
template< typename tForward >
tForward uninitialized_value_construct_n( tForward, std::size_t );

template< typename tForward >
void uninitialized_default_construct( tForward, tForward );
template< typename tForward >
tForward uninitialized_default_construct_n( tForward, std::size_t );


template< typename tForward >
void destroy( tForward, tForward );
template< typename tForward >
tForward destroy_n( tForward, std::size_t );
template< typename tType >
void destroy_at( tType* );

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/uninitialized.inl"
#endif // FLUX_NIH_UNINITIALIZED_HPP_C283207D_E81A_4126_BBC5_DBB3E20DD5EF
