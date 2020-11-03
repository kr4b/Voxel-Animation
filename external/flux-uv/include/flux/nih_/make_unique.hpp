/*-******************************************************* -- HEADER -{{{1- */
/*-	make_unique fallback
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_NIH_MAKE_UNIQUE_HPP_4C1590BB_88A8_4758_AD4B_6D69A5C7B389
#define FLUX_NIH_MAKE_UNIQUE_HPP_4C1590BB_88A8_4758_AD4B_6D69A5C7B389

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/std/enable_if.hpp>

#include <memory>
#include <type_traits>

//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    make_unique()               ///{{{1///////////////////////////////////

template< typename tType, typename... tArgs > inline
FLUX_DISABLE_IF( std::is_array<tType> )
(std::unique_ptr<tType>) make_unique( tArgs&&... );

template< typename tType > inline
FLUX_ENABLE_IF_C( std::is_array<tType>::value && (0 == std::extent<tType>::value) )
(std::unique_ptr<tType>) make_unique( std::size_t );

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/make_unique.inl"
#endif // FLUX_NIH_MAKE_UNIQUE_HPP_4C1590BB_88A8_4758_AD4B_6D69A5C7B389
