/*-******************************************************* -- HEADER -{{{1- */
/*-	std::exchange fallback
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_NIH_EXCHANGE_HPP_76834F3D_126B_4E7C_AB5C_84ECAB33CEAF
#define FLUX_NIH_EXCHANGE_HPP_76834F3D_126B_4E7C_AB5C_84ECAB33CEAF

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/compat/constexpr.hpp>

#include <utility>

//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    exchange()                  ///{{{1///////////////////////////////////

template< class tT, class tU > FLUX_CONSTEXPR_EX
tT exchange( tT& aT, tU&& aU )
{
	tT old = std::move(aT);
	aT = std::forward<tU>(aU);
	return old;
}

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_NIH_EXCHANGE_HPP_76834F3D_126B_4E7C_AB5C_84ECAB33CEAF
