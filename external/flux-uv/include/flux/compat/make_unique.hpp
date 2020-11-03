/*-******************************************************* -- HEADER -{{{1- */
/*-	make_unique()
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_MAKE_UNIQUE_HPP_069E88D4_640A_4DA2_B698_8D2E3B3A2BCC
#define FLUX_COMPAT_MAKE_UNIQUE_HPP_069E88D4_640A_4DA2_B698_8D2E3B3A2BCC

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/stdfeat.hpp>
#include <flux/setup/namespace.hpp>

//--    make_unique()               ///{{{1///////////////////////////////////

#if FLUX_STDFEAT_CXX14_MAKE_UNIQUE
#	include <memory>

FLUX_NAMESPACE_ENTER1(compat)
	using std::make_unique;
FLUX_NAMESPACE_LEAVE1(compat)

#else // fallback to nih_
#	include <flux/nih_/make_unique.hpp>

FLUX_NAMESPACE_ENTER1(compat)
	using nih_::make_unique;
FLUX_NAMESPACE_LEAVE1(compat)

#endif // ~ implementations

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_MAKE_UNIQUE_HPP_069E88D4_640A_4DA2_B698_8D2E3B3A2BCC
