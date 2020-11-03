/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	get<TYPE>() implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/compat/constexpr.hpp>

#include <cstddef>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    $ detail                            ///{{{2///////////////////////////
namespace detail
{
	template< std::size_t tIndex, typename tType, class... tElems >
	struct TupleFindIndex;

	template< std::size_t tIndex, typename tType, class... tTail >
	struct TupleFindIndex<tIndex,tType,tType,tTail...>
	{
		static constexpr std::size_t value = tIndex;
	};
	template< std::size_t tIndex, typename tType, typename tHead, class... tTail >
	struct TupleFindIndex<tIndex,tType,tHead,tTail...>
		: TupleFindIndex<tIndex+1,tType,tTail...>
	{};
}

//--    get()                               ///{{{2///////////////////////////

template< typename tType, class... tElems > FLUX_CONSTEXPR_EX inline
tType& get( std::tuple<tElems...>& aTuple ) noexcept
{
	constexpr auto index = detail::TupleFindIndex<0,tType,tElems...>::value;
	return std::get<index>(aTuple);
}
template< typename tType, class... tElems > FLUX_CONSTEXPR_EX inline
tType&& get( std::tuple<tElems...>&& aTuple ) noexcept
{
	constexpr auto index = detail::TupleFindIndex<0,tType,tElems...>::value;
	return std::get<index>(static_cast<std::tuple<tElems...>&&>(aTuple));
}
template< typename tType, class... tElems > FLUX_CONSTEXPR_EX inline
tType const& get( std::tuple<tElems...> const& aTuple ) noexcept
{
	constexpr auto index = detail::TupleFindIndex<0,tType,tElems...>::value;
	return std::get<index>(aTuple);
}
template< typename tType, class... tElems > FLUX_CONSTEXPR_EX inline
tType const&& get( std::tuple<tElems...> const&& aTuple ) noexcept
{
	constexpr auto index = detail::TupleFindIndex<0,tType,tElems...>::value;
	return std::get<index>(static_cast<std::tuple<tElems...> const&&>(aTuple));
}

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
