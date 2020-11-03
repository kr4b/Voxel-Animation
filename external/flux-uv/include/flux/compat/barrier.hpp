/*-******************************************************* -- HEADER -{{{1- */
/*-	barrier & co
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_BARRIER_HPP_6448DDBE_9F6D_4475_BE4A_CBA11A3BE7D6
#define FLUX_COMPAT_BARRIER_HPP_6448DDBE_9F6D_4475_BE4A_CBA11A3BE7D6

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/stdfeat.hpp>

//--    barrier                     ///{{{1///////////////////////////////////

#if FLUX_STDFEAT_CXX20_BARRIER
#	include <barrier>

FLUX_NAMESPACE_ENTER1(compat)
	using std::barrier;
	using std::basic_barrier;
FLUX_NAMESPACE_LEAVE1(compat)

#else // flux-nih_ fallback
#	include <flux/nih_/barrier.hpp>

FLUX_NAMESPACE_ENTER1(compat)
	using nih_::barrier;
	using nih_::basic_barrier;
FLUX_NAMESPACE_LEAVE1(compat)

#endif // ~ implementations

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_BARRIER_HPP_6448DDBE_9F6D_4475_BE4A_CBA11A3BE7D6
