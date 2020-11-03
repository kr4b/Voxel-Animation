/*-******************************************************* -- HEADER -{{{1- */
/*- Output message
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_PP_MESSAGE_HPP_072956CF_E04B_4F4C_AED8_53ACBAC00880
#define FLUX_PP_MESSAGE_HPP_072956CF_E04B_4F4C_AED8_53ACBAC00880

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/compiler.hpp>

//--    MESSAGE                     ///{{{1///////////////////////////////////
/** Emit message during compilation
 *
 * Emit message `msg` during compilation. The message should be a quoted 
 * string. Example:
 *
   \code
   FLUX_PP_MESSAGE( "Beware!" );
   \endcode
 *
 * \note With Clang, messages are treated as warnings. Warnings from macros
 * cause macro stack-traces. This typically generates a boat load of output.
 */
#define FLUX_PP_MESSAGE(msg)               /*IMPLEMENTED_ELSEWHERE*/

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/message.inl"
#endif // FLUX_PP_MESSAGE_HPP_072956CF_E04B_4F4C_AED8_53ACBAC00880
