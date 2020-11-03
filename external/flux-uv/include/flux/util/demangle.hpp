/*-******************************************************* -- HEADER -{{{1- */
/*-	Demangle typeid().name() strings.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_UTIL_DEMANGLE_HPP_FA2E53F7_0D51_4997_A532_6ED458E152B4
#define FLUX_UTIL_DEMANGLE_HPP_FA2E53F7_0D51_4997_A532_6ED458E152B4

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/namespace.hpp>
#include <string>

//--    >>> namespace = flux::util >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(util)
//--    demangle()                          ///{{{2///////////////////////////

/** Demangle string from `typeid().name()`
 *
 * Example:
   \code
   std::string ret = demangle( typeid(std::string()).name() );
   \endcode
 * `demangle()` would return the string `"std::string"`.
 *
 * \warning `demangle()` is intended to improve output for humans. It is *not*
 *	intended to aid e.g. serialization. As such, the precise format of 
 *	`demangle`'s output is unspecified and should be assumed to be able to 
 *	change without notice.
 *
 * \note Implementation: MSVC's `std::type_info::name()` returns unmangeled
 * 	strings by default. There `demangle()` returns the its argument unmolested.
 *  GCC and Clang provide `abi::_cxa_demangle()` in `<cxxabi.h>`, which 
 *  `demangle()` relies on internally for those compilers.
 */
std::string demangle( char const* aString );

//--    <<< ~ flux::util namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(util)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_UTIL_DEMANGLE_HPP_FA2E53F7_0D51_4997_A532_6ED458E152B4
