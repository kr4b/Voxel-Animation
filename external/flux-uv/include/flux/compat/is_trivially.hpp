/*-******************************************************* -- HEADER -{{{1- */
/*-	is_trivially_*<> fallbacks
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_IS_TRIVIALLY_HPP_B7ABED0C_3F88_4789_9CC3_D88889DA6F42
#define FLUX_COMPAT_IS_TRIVIALLY_HPP_B7ABED0C_3F88_4789_9CC3_D88889DA6F42

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/stdfeat.hpp>


//--    type traits                 ///{{{1///////////////////////////////////

#if !FLUX_STDQUIRK_CXX11_NO_TRIVIALLY_TRAITS
#	include <type_traits>

FLUX_NAMESPACE_ENTER1(compat)
	using std::is_trivially_copyable;

	using std::is_trivially_constructible;
	using std::is_trivially_copy_constructible;
	using std::is_trivially_move_constructible;
	using std::is_trivially_default_constructible;

	using std::is_trivially_assignable;
	using std::is_trivially_copy_assignable;
	using std::is_trivially_move_assignable;

	using std::is_trivially_destructible;
FLUX_NAMESPACE_LEAVE1(compat)

#else // fallback
#	include <flux/nih_/is_trivially.hpp>

FLUX_NAMESPACE_ENTER1(compat)
	using nih_::is_trivially_copyable;

	using nih_::is_trivially_constructible;
	using nih_::is_trivially_copy_constructible;
	using nih_::is_trivially_move_constructible;
	using nih_::is_trivially_default_constructible;

	using nih_::is_trivially_assignable;
	using nih_::is_trivially_copy_assignable;
	using nih_::is_trivially_move_assignable;

	using nih_::is_trivially_destructible;

FLUX_NAMESPACE_LEAVE1(compat)

#endif // ~ implementations

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_IS_TRIVIALLY_HPP_B7ABED0C_3F88_4789_9CC3_D88889DA6F42
