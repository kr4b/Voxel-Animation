/*-******************************************************* -- HEADER -{{{1- */
/*-	latch
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_LATCH_HPP_0849E68E_A94C_42D6_91EC_2FC2A6B0F0FE
#define FLUX_COMPAT_LATCH_HPP_0849E68E_A94C_42D6_91EC_2FC2A6B0F0FE

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/stdfeat.hpp>

//--    barrier                     ///{{{1///////////////////////////////////

#if FLUX_STDFEAT_CXX20_BARRIER // latch uses same STDFEAT
#	include <latch>

FLUX_NAMESPACE_ENTER1(compat)
	using std::latch;
FLUX_NAMESPACE_LEAVE1(compat)

#else // flux-nih_ fallback
#	include <flux/nih_/latch.hpp>

FLUX_NAMESPACE_ENTER1(compat)
	using nih_::latch;
FLUX_NAMESPACE_LEAVE1(compat)

#endif // ~ implementations

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_LATCH_HPP_0849E68E_A94C_42D6_91EC_2FC2A6B0F0FE
