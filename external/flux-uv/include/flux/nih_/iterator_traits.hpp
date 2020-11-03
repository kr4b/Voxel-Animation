/*-******************************************************* -- HEADER -{{{1- */
/*-	std::iterator_traits<> fallback
 *
 * The fallback is based on N3844:
 * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3844.pdf
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_NIH_ITERATOR_TRAITS_HPP_2E0C9DB3_6211_4611_8499_497625B71BAF
#define FLUX_NIH_ITERATOR_TRAITS_HPP_2E0C9DB3_6211_4611_8499_497625B71BAF

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <cstddef>
#include <iterator>

//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    iterator_traits()           ///{{{1///////////////////////////////////
namespace detail
{
	template< typename... >
	struct Void_ { using type = void; };

	template< typename... tArgs > 
	using void_t = typename Void_<tArgs...>::type;


	template< typename, class = void >
	struct IterTypes { };

	template< typename tIter >
	struct IterTypes<tIter, void_t<typename tIter::difference_type
		, typename tIter::value_type
		, typename tIter::pointer
		, typename tIter::reference
		, typename tIter::iterator_category
		>               
	> {
		using difference_type   = typename tIter::difference_type;
		using value_type        = typename tIter::value_type;
		using pointer           = typename tIter::pointer;
		using reference         = typename tIter::reference;
		using iterator_category = typename tIter::iterator_category;
	};

	template< typename tType >
	struct IterTypes<tType*>
	{
		using difference_type = std::ptrdiff_t;
		using value_type = tType;
		using pointer = tType*;
		using reference = tType&;
		using iterator_category = std::random_access_iterator_tag;
	};
	template< typename tType >
	struct IterTypes<tType const*>
	{
		using difference_type = std::ptrdiff_t;
		using value_type = tType;
		using pointer = tType const*;
		using reference = tType const&;
		using iterator_category = std::random_access_iterator_tag;
	};
}

template< class tIter >
struct iterator_traits
	: detail::IterTypes<tIter>
{};

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_NIH_ITERATOR_TRAITS_HPP_2E0C9DB3_6211_4611_8499_497625B71BAF
