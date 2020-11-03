/*-******************************************************* -- HEADER -{{{1- */
/*-	launder()
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_LAUNDER_HPP_94E60A31_17BF_400A_BB09_3CA625769E5E
#define FLUX_COMPAT_LAUNDER_HPP_94E60A31_17BF_400A_BB09_3CA625769E5E

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/stdfeat.hpp>
#include <flux/setup/namespace.hpp>

//--    launder()                   ///{{{1///////////////////////////////////

#if FLUX_STDFEAT_CXX17_LAUNDER
#	include <new>

FLUX_NAMESPACE_ENTER1(compat)
	using std::launder;
FLUX_NAMESPACE_LEAVE1(compat)

#else // flux-nih_ fallback

/* We can't really do anything sensible other than praying that the UB is 
 * benign (well, whatever that means). So, just provide a stub `launder()`.
 */

FLUX_NAMESPACE_ENTER1(compat)
	template< typename tType > inline constexpr
	tType* launder( tType* aPtr ) { return aPtr; }
FLUX_NAMESPACE_LEAVE1(compat)

#endif // ~ implementations

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_LAUNDER_HPP_94E60A31_17BF_400A_BB09_3CA625769E5E
