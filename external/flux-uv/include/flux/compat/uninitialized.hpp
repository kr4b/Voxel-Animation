/*-******************************************************* -- HEADER -{{{1- */
/*-	uninitialized_*()
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_UNINITIALIZED_HPP_3A337611_8C29_4768_A5D2_093A54258DD9
#define FLUX_COMPAT_UNINITIALIZED_HPP_3A337611_8C29_4768_A5D2_093A54258DD9

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/stdfeat.hpp>
#include <flux/setup/namespace.hpp>

//--    uninitialized_*() et al.    ///{{{1///////////////////////////////////

#if FLUX_STDFEAT_CXX17_UNINITIALIZED
#	include <memory>

FLUX_NAMESPACE_ENTER1(compat)
	using std::uninitialized_move;
	using std::uninitialized_move_n;

	using std::uninitialized_value_construct;
	using std::uninitialized_value_construct_n;

	using std::uninitialized_default_construct;
	using std::uninitialized_default_construct_n;

	using std::destroy;
	using std::destroy_n;
	using std::destroy_at;
FLUX_NAMESPACE_LEAVE1(compat)

#else // flux-nih_ fallback
#	include <flux/nih_/uninitialized.hpp>

FLUX_NAMESPACE_ENTER1(compat)
	using nih_::uninitialized_move;
	using nih_::uninitialized_move_n;

	using nih_::uninitialized_value_construct;
	using nih_::uninitialized_value_construct_n;

	using nih_::uninitialized_default_construct;
	using nih_::uninitialized_default_construct_n;

	using nih_::destroy;
	using nih_::destroy_n;
	using nih_::destroy_at;
FLUX_NAMESPACE_LEAVE1(compat)

#endif // ~ implementations

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_UNINITIALIZED_HPP_3A337611_8C29_4768_A5D2_093A54258DD9
