/*-******************************************************* -- HEADER -{{{1- */
/*-	get<TYPE>() fallback
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_NIH_TUPLE_GET_HPP_64999DF7_DF58_41A1_8FA5_D5B5B04DC250
#define FLUX_NIH_TUPLE_GET_HPP_64999DF7_DF58_41A1_8FA5_D5B5B04DC250

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/compat/constexpr.hpp>

#include <tuple>

//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    get()                       ///{{{1///////////////////////////////////

template< typename tType, class... tElems > FLUX_CONSTEXPR_EX
tType& get( std::tuple<tElems...>& ) noexcept;
template< typename tType, class... tElems > FLUX_CONSTEXPR_EX
tType&& get( std::tuple<tElems...>&& ) noexcept;
template< typename tType, class... tElems > FLUX_CONSTEXPR_EX
tType const& get( std::tuple<tElems...> const& ) noexcept;
template< typename tType, class... tElems > FLUX_CONSTEXPR_EX
tType const&& get( std::tuple<tElems...> const&& ) noexcept;

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/tuple_get.inl"
#endif // FLUX_NIH_TUPLE_GET_HPP_64999DF7_DF58_41A1_8FA5_D5B5B04DC250
