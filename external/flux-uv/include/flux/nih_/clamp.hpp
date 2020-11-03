/*-******************************************************* -- HEADER -{{{1- */
/*-	std::clamp fallback
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_NIH_CLAMP_HPP_48D91326_A618_4452_9F7C_B04C86930C98
#define FLUX_NIH_CLAMP_HPP_48D91326_A618_4452_9F7C_B04C86930C98

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <functional>

//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    clamp()                     ///{{{1///////////////////////////////////

/* Based of the example implementation from N4536 */

template< typename tType > inline constexpr
tType const& clamp( tType const& aX, tType const& aLo, tType const& aHi )
{
	return /*assert( !(aHi < aLo) ),*/
		(aX < aLo) ? aLo : (aHi < aX) ? aHi : aX
	;
}

template< typename tType, class tComp > inline constexpr
tType const& clamp( tType const& aX, tType const& aLo, tType const& aHi, tComp aComp )
{
	return /*assert( !aComp( aHi, aLo ) ),*/
		aComp( aX, aLo ) ? aLo : aComp( aHi, aX ) ? aHi : aX
	;
}

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_NIH_CLAMP_HPP_48D91326_A618_4452_9F7C_B04C86930C98
