/*-******************************************************* -- HEADER -{{{1- */
/*- SFINAE friendly iterator_traits<>
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_ITERATOR_TRAITS_HPP_6F30400F_859A_4FB5_9307_17D47DEB095B
#define FLUX_COMPAT_ITERATOR_TRAITS_HPP_6F30400F_859A_4FB5_9307_17D47DEB095B

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/stdfeat.hpp>

//--    iterator traits             ///{{{1///////////////////////////////////

#if FLUX_STDFEAT_CXX17_ITERATOR_TRAITS
#	include <iterator>

FLUX_NAMESPACE_ENTER1(compat)
	using std::iterator_traits;
FLUX_NAMESPACE_LEAVE1(compat)

#else // fallback
#	include <flux/nih_/iterator_traits.hpp>

FLUX_NAMESPACE_ENTER1(compat)
	using nih_::iterator_traits;
FLUX_NAMESPACE_LEAVE1(compat)

#endif // ~ implementations

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_ITERATOR_TRAITS_HPP_6F30400F_859A_4FB5_9307_17D47DEB095B
