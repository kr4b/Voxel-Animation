/*-******************************************************* -- HEADER -{{{1- */
/*-	"byte" type
 *
 * C++17 std::byte or fallback.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_BYTE_HPP_CFE97EC6_BDCE_442F_AD7C_7AD147DBD57A
#define FLUX_STD_BYTE_HPP_CFE97EC6_BDCE_442F_AD7C_7AD147DBD57A

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/namespace.hpp>
#include <flux/compat/byte.hpp>

//--    >>> namespace = flux >>>                    ///{{{1///////////////////
FLUX_NAMESPACE_ENTER()
//--    byte                                ///{{{2///////////////////////////

using flux::compat::byte;

//--    <<< ~ flux: namespace <<<                   ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE()
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_STD_BYTE_HPP_CFE97EC6_BDCE_442F_AD7C_7AD147DBD57A
