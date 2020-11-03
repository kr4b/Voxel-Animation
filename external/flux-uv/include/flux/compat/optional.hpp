/*-******************************************************* -- HEADER -{{{1- */
/*-	optional & co
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_OPTIONAL_HPP_11ECE5DB_38AF_4D7A_97D2_99A6DF8738F3
#define FLUX_COMPAT_OPTIONAL_HPP_11ECE5DB_38AF_4D7A_97D2_99A6DF8738F3

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/stdfeat.hpp>

//--    optional<>                  ///{{{1///////////////////////////////////

#if FLUX_STDFEAT_CXX17_OPTIONAL
#	include <optional>

FLUX_NAMESPACE_ENTER1(compat)
	using std::optional;
	using std::make_optional;

	using std::nullopt;
	using std::nullopt_t;

	using std::in_place; //TODO-move to separate header, shared by any and variant
	using std::in_place_t; //TODO-move to separate header, shared by any and variant

	using std::bad_optional_access;
FLUX_NAMESPACE_LEAVE1(compat)

// NOTE: omitting STDFEAT_XP_OPTIONAL since it lacks several easy-to-define
// methods and constructors that *are* in our nih_ version.

#else // flux-nih_ fallback
#	include <flux/nih_/optional.hpp>

FLUX_NAMESPACE_ENTER1(compat)
	using nih_::optional;
	using nih_::make_optional;

	using nih_::nullopt;
	using nih_::nullopt_t;

	using nih_::in_place; //TODO-move to separate header...
	using nih_::in_place_t; //TODO-move to separate header...

	using nih_::bad_optional_access;
FLUX_NAMESPACE_LEAVE1(compat)

#endif // ~ implementations

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_OPTIONAL_HPP_11ECE5DB_38AF_4D7A_97D2_99A6DF8738F3
