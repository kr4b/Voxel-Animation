/*-******************************************************* -- HEADER -{{{1- */
/*-	Identity<> helper
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_IDENTITY_HPP_134A9F08_738D_43E5_B6B5_08A2FC38A7D2
#define FLUX_STD_IDENTITY_HPP_134A9F08_738D_43E5_B6B5_08A2FC38A7D2

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/namespace.hpp>

//--    >>> namespace = flux >>>                    ///{{{1///////////////////
FLUX_NAMESPACE_ENTER()
//--    Identity<>                          ///{{{2///////////////////////////

template< typename tType >
struct Identity
{
	typedef tType type;
};

//--    <<< ~ flux: namespace <<<                   ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE()
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_STD_IDENTITY_HPP_134A9F08_738D_43E5_B6B5_08A2FC38A7D2
