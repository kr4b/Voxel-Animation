/*-******************************************************* -- HEADER -{{{1- */
/*-	Include cxx_function 
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_EXT_CXX_FUNCTION_HPP_F6D2B48D_296F_4E91_8046_E69EEEB6B0FF
#define FLUX_EXT_CXX_FUNCTION_HPP_F6D2B48D_296F_4E91_8046_E69EEEB6B0FF

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg> 
#include <flux/util/diagnostics.hpp>

//--    <cxx_function.hpp>          ///{{{1///////////////////////////////////

FLUX_DIAGNOSTIC_STATE_PUSH_AND_SET(IGNORED,ZERO_AS_NULLPTR)
FLUX_DIAGNOSTIC_STATE_PUSH_AND_SET(IGNORED,CLASS_MEMACCESS)
FLUX_DIAGNOSTIC_STATE_PUSH_AND_SET(IGNORED,UNUSED_ARGUMENT)

#include "impl/cxx_function.hpp"

FLUX_DIAGNOSTIC_STATE_POP_OR_DEFAULT(UNUSED_ARGUMENT)
FLUX_DIAGNOSTIC_STATE_POP_OR_DEFAULT(CLASS_MEMACCESS)
FLUX_DIAGNOSTIC_STATE_POP_OR_DEFAULT(ZERO_AS_NULLPTR)

namespace flux_ext
{
	using cxx_function::function;
	using cxx_function::unique_function;
}

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_EXT_CXX_FUNCTION_HPP_F6D2B48D_296F_4E91_8046_E69EEEB6B0FF
