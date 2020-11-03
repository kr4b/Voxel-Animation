/*-******************************************************* -- HEADER -{{{1- */
/*-	std::clamp()
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_CLAMP_HPP_C2164562_C850_43FC_9BAD_FA398B5AAA87
#define FLUX_COMPAT_CLAMP_HPP_C2164562_C850_43FC_9BAD_FA398B5AAA87

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/stdfeat.hpp>
#include <flux/setup/namespace.hpp>

//--    clamp()                     ///{{{1///////////////////////////////////

#if FLUX_STDFEAT_CXX17_CLAMP
#	include <algorithm>

FLUX_NAMESPACE_ENTER1(compat)
	using std::clamp;
FLUX_NAMESPACE_LEAVE1(compat)

#else // flux-nih_ fallback
#	include <flux/nih_/clamp.hpp>

FLUX_NAMESPACE_ENTER1(compat)
	using nih_::clamp;
FLUX_NAMESPACE_LEAVE1(compat)

#endif // ~ implementations

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_CLAMP_HPP_C2164562_C850_43FC_9BAD_FA398B5AAA87
