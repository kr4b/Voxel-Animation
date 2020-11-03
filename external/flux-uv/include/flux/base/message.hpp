/*-******************************************************* -- HEADER -{{{1- */
/*-	Message
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_BASE_MESSAGE_HPP_3C8F4584_C582_47A8_9C4E_3AFF24127665
#define FLUX_BASE_MESSAGE_HPP_3C8F4584_C582_47A8_9C4E_3AFF24127665

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/identity.hpp>

#include <flux/ftl/any.hpp>
#include <flux/ftl/meta_set.hpp>

//--    >>> namespace = flux::base >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(base)
//--    types                               ///{{{2///////////////////////////

using Msg = ftl::AnyT<
	ftl::meta::make_set< 
		ftl::aspect::AnyFeatEqual, 
		ftl::aspect::AnyFeatLess,
		ftl::aspect::AnyFeatHash
	>,
	24
>;

using Endpoint = void*;

//--    Message                             ///{{{2///////////////////////////
struct Message final
{
	Msg msg;
	Endpoint org = nullptr, dest = nullptr;

	Message() = default;

	explicit 
	Message( Msg aMsg, Endpoint aDst = nullptr, Endpoint aOrg = nullptr ) noexcept
		: msg(std::move(aMsg))
		, org(aOrg)
		, dest(aDst)
	{}

	template< typename tMsgType, typename... tArgs >
	Message( Identity<tMsgType>, tArgs&&... aArgs )
		: msg( Identity<tMsgType>{}, std::forward<tArgs>(aArgs)... )
	{}
	template< typename tMsgType, typename... tArgs >
	Message( Endpoint aDst, Identity<tMsgType>, tArgs&&... aArgs )
		: msg( Identity<tMsgType>{}, std::forward<tArgs>(aArgs)... )
		, dest(aDst)
	{}
	template< typename tMsgType, typename... tArgs >
	Message( Endpoint aDst, Endpoint aOrg, Identity<tMsgType>, tArgs&&... aArgs )
		: msg( Identity<tMsgType>{}, std::forward<tArgs>(aArgs)... )
		, org(aOrg)
		, dest(aDst)
	{}
};

//--    <<< ~ flux::base namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(base)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_BASE_MESSAGE_HPP_3C8F4584_C582_47A8_9C4E_3AFF24127665
