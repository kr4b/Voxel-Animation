/*-******************************************************* -- HEADER -{{{1- */
/*-	string_view & co
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_STRING_VIEW_HPP_FC305744_FA82_4D33_8CE3_F7112728A115
#define FLUX_COMPAT_STRING_VIEW_HPP_FC305744_FA82_4D33_8CE3_F7112728A115

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/stdfeat.hpp>

//--    string_view                 ///{{{1///////////////////////////////////

#if FLUX_STDFEAT_CXX17_STRING_VIEW
#	include <string_view>
#	include <flux_ext/string_view.hpp>

FLUX_NAMESPACE_ENTER1(compat)
	using std::basic_string_view;
	using std::string_view;

	using ::nonstd::to_string;
	using ::nonstd::to_string_view;
FLUX_NAMESPACE_LEAVE1(compat)

#else // !CXX17_STRING_VIEW
#	include <flux_ext/string_view.hpp>

FLUX_NAMESPACE_ENTER1(compat)
	using ::nonstd::basic_string_view;
	using ::nonstd::string_view;

	using ::nonstd::to_string;
	using ::nonstd::to_string_view;
FLUX_NAMESPACE_LEAVE1(compat)

#endif // ~ CXX17_STRING_VIEW

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_STRING_VIEW_HPP_FC305744_FA82_4D33_8CE3_F7112728A115
