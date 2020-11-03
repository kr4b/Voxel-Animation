/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- meta::Set implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/ftl/meta_bool.hpp>

#include <type_traits>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::ftl::meta >>>         ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(ftl,meta)
//--    set_contains_element<>              ///{{{2///////////////////////////
namespace detail
{
	template< typename, typename... > struct SetContainsElem0;
	template< typename tItem, typename tHead, typename... tTail >
	struct SetContainsElem0< tItem, tHead, tTail... >
		: SetContainsElem0< tItem, tTail... >
	{};

	template< typename tItem, typename... tTail >
	struct SetContainsElem0< tItem, tItem, tTail...  >
		: std::true_type
	{};
	template< typename tItem >
	struct SetContainsElem0< tItem >
		: std::false_type
	{};
};

template< typename... tItems, typename tItem >
struct set_contains_element< Set<tItems...>, tItem >
	: detail::SetContainsElem0< tItem, tItems... >
{};

//--    set_contains_set<>                  ///{{{2///////////////////////////
namespace detail
{
	template< class, typename... > struct SetContainsSet0;
	template< class tSuper, typename tHead, typename... tTail >
	struct SetContainsSet0< tSuper, tHead, tTail... >
		: All< /* XXX-could probably optimize by stopping of set_contains<> is false. */
			set_contains_element< tSuper, tHead >,
			SetContainsSet0< tSuper, tTail... >
		>
	{};
	template< class tSuper >
	struct SetContainsSet0< tSuper >
		: std::true_type
	{};
}

template< typename... tItemsA, typename... tItemsB >
struct set_contains_set< Set<tItemsA...>, Set<tItemsB...> >
	: detail::SetContainsSet0< Set<tItemsA...>, tItemsB... >
{};

//--    set_equal<>                         ///{{{2///////////////////////////
template< typename... tItemsA, typename... tItemsB >
struct set_equal< Set<tItemsA...>, Set<tItemsB...> >
	: All<
		set_contains_set< Set<tItemsA...>, Set<tItemsB...> >,
		set_contains_set< Set<tItemsB...>, Set<tItemsA...> >
	>
{};

//--    <<< ~ flux::ftl::meta namespace <<<         ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(ftl,meta)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
