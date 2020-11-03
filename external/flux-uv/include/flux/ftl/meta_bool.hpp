/*-******************************************************* -- HEADER -{{{1- */
/*-	Boolean meta-function
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FTL_META_BOOL_HPP_58E488FF_79DA_4242_9EBB_8F8828A2D82B
#define FLUX_FTL_META_BOOL_HPP_58E488FF_79DA_4242_9EBB_8F8828A2D82B

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <type_traits>

//--    >>> namespace = flux::ftl::meta >>>         ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(ftl,meta)
//--    meta functions                      ///{{{2///////////////////////////
namespace detail
{
	template< typename... tArgs >
	struct AnyImpl_;

	template< typename tHead, typename... tTail >
	struct AnyImpl_<tHead,tTail...>
		: std::integral_constant< bool, tHead::value || AnyImpl_<tTail...>::value >
	{};
	template< typename tBool >
	struct AnyImpl_<tBool>
		: tBool
	{};


	template< typename... tArgs >
	struct AllImpl_;

	template< typename tHead, typename... tTail >
	struct AllImpl_<tHead,tTail...>
		: std::integral_constant< bool, tHead::value && AllImpl_<tTail...>::value >
	{};
	template< typename tBool >
	struct AllImpl_<tBool>
		: tBool
	{};
}

/** `Not` unary meta-function
 */
template< class tBool > 
struct Not : std::integral_constant< bool, !tBool::value >
{};

/** `Any` meta-function
 *
 * Takes boolean meta values and reduces them to a single value using logical
 * or. I.e., `Any<...>` evalutes to true if any of the boolean meta values were
 * true, and to false if none were.
 */
template< class... tBools >
struct Any
	: std::integral_constant< bool, detail::AnyImpl_<tBools...>::value >
{};

/** `All` meta-function
 *
 * Takes boolean meta values and reduces them to a single value using logical
 * and. I.e., `All<...>` evalutes to ture if all of the boolean meta values were
 * true, and to false otherwise.
 */
template< class... tBools >
struct All
	: std::integral_constant< bool, detail::AllImpl_<tBools...>::value >
{};

//--    <<< ~ flux::ftl::meta namespace <<<         ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(ftl,meta)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_FTL_META_BOOL_HPP_58E488FF_79DA_4242_9EBB_8F8828A2D82B
