/*-******************************************************* -- HEADER -{{{1- */
/*-	span & co
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_SPAN_HPP_F62A4756_A8A3_4949_BD7A_AC20334F9346
#define FLUX_COMPAT_SPAN_HPP_F62A4756_A8A3_4949_BD7A_AC20334F9346

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/stdfeat.hpp>

//--    span<>                     ///{{{1///////////////////////////////////

#if FLUX_STDFEAT_CXX20_SPAN
#	include <span>

FLUX_NAMESPACE_ENTER1(compat)
	using std::span;
	using std::dynamic_extent;

	//TODO: make_span() fallback!
FLUX_NAMESPACE_LEAVE1(compat)

#else // !CXX20_SPAN
#	include <flux_ext/span.hpp>

FLUX_NAMESPACE_ENTER1(compat)
	using ::nonstd::span;

	using ::nonstd::dynamic_extent;
	using ::nonstd::with_container; // XXX-TODO-extension?

	using ::nonstd::make_span; // WARNING extension from span-lite
FLUX_NAMESPACE_LEAVE1(compat)
#endif // ~ CXX20_SPAN

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_SPAN_HPP_F62A4756_A8A3_4949_BD7A_AC20334F9346
