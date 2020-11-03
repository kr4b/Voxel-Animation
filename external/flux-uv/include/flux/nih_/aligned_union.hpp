/*-******************************************************* -- HEADER -{{{1- */
/*- aligned_union<> fallback
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_NIH_ALIGNED_UNION_HPP_80B16F6B_930B_4A64_A237_022F33285AFF
#define FLUX_NIH_ALIGNED_UNION_HPP_80B16F6B_930B_4A64_A237_022F33285AFF

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <cstddef>

#include <type_traits>

//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    aligned_union<>                     ///{{{2///////////////////////////
namespace detail
{
	template< typename... tTypes >
	struct FindMaxAlign_;

	template< typename tHead, typename... tTail >
	struct FindMaxAlign_< tHead, tTail... >
	{
		constexpr static std::size_t head_ = alignof(tHead);
		constexpr static std::size_t tail_ = FindMaxAlign_<tTail...>::value;

		constexpr static std::size_t value = head_ > tail_ ? head_ : tail_;
	};
	template< typename tHead >
	struct FindMaxAlign_<tHead>
	{
		constexpr static std::size_t value = alignof(tHead);
	};
}

template< std::size_t tLength, typename... tTypes >
struct aligned_union
{
	constexpr static std::size_t required_ = detail::FindMaxAlign_<tTypes...>::value;

	using type = typename std::aligned_storage< tLength, required_ >::type;
};

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_NIH_ALIGNED_UNION_HPP_80B16F6B_930B_4A64_A237_022F33285AFF
