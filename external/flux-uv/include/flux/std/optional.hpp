/*-******************************************************* -- HEADER -{{{1- */
/*-	optionalr<>
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_OPTIONAL_HPP_9751C007_CDBF_4792_9FE8_649E35B45A01
#define FLUX_STD_OPTIONAL_HPP_9751C007_CDBF_4792_9FE8_649E35B45A01

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/compat/optional.hpp>

//--    shared_ptr<>                ///{{{1///////////////////////////////////

FLUX_NAMESPACE_ENTER()
	using compat::optional;
	using compat::make_optional;

	using compat::nullopt;
FLUX_NAMESPACE_LEAVE()

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_STD_OPTIONAL_HPP_9751C007_CDBF_4792_9FE8_649E35B45A01
