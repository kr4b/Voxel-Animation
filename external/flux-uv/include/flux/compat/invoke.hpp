/*-******************************************************* -- HEADER -{{{1- */
/*-	invoke & co
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_INVOKE_HPP_431E8FB6_ED71_4DB9_9633_A64C47BD313B
#define FLUX_COMPAT_INVOKE_HPP_431E8FB6_ED71_4DB9_9633_A64C47BD313B

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/stdfeat.hpp>

//--    invoke                      ///{{{1///////////////////////////////////

#if FLUX_STDFEAT_CXX17_INVOKE
#	include <functional>
#	include <type_traits>

FLUX_NAMESPACE_ENTER1(compat)
	using std::invoke;
	using std::invoke_result;
FLUX_NAMESPACE_LEAVE1(compat)

#else // !CXX17_INVOKE
#	include <flux_ext/invoke.hpp>

FLUX_NAMESPACE_ENTER1(compat)
	using ::nonstd::invoke;
	using ::nonstd::invoke_result;
FLUX_NAMESPACE_LEAVE1(compat)
#endif // ~ CXX17_INVOKE

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_INVOKE_HPP_431E8FB6_ED71_4DB9_9633_A64C47BD313B
