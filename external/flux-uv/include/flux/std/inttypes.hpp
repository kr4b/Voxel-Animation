/*-******************************************************* -- HEADER -{{{1- */
/*-	Integer types used throughout flux
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_INTTYPES_HPP_21302393_378E_480A_9549_B94603D18253
#define FLUX_STD_INTTYPES_HPP_21302393_378E_480A_9549_B94603D18253

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <cstddef>
#include <cstdint>

#if FLUX_STDLIB == FLUX_STDLIB_VSCPP
#	include <wchar.h> // for off_t
#	include <BaseTsd.h> // for SSIZE_T
#else
#	include <sys/types.h> // for off_t
#endif // ~ FLUX_STDLIB

//--    >>> namespace = flux::inttypes >>>          ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(inttypes)
//--    types                               ///{{{2///////////////////////////

/** \namespace flux::inttypes
 * \brief Integer types used throughout flux
 *
 * The namespace `flux::inttypes` contains the standard integer types that are
 * used throughout flux. These types are additionally injected into the root
 * `flux` namespace.
 *
 * If you wish to use these types elsewhere, you can import them with `using
 * namespace flux::inttypes` - this avois importing all the other cruft from
 * `flux::`.
 */

using ::int8_t;
using ::int16_t;
using ::int32_t;
using ::int64_t;

using ::uint8_t;
using ::uint16_t;
using ::uint32_t;
using ::uint64_t;

using ::intptr_t;
using ::uintptr_t;

using ::off_t;

using std::size_t;
using std::ptrdiff_t;

#if FLUX_STDLIB == FLUX_STDLIB_VSCPP
typedef SSIZE_T ssize_t;
#else
using ::ssize_t;
#endif // ~ FLUX_STDLIB

//--    <<< ~ flux::inttypes namespace <<<          ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(inttypes)
//--    >>> namespace = flux >>>                    ///{{{1///////////////////
FLUX_NAMESPACE_ENTER()
//--    types                               ///{{{2///////////////////////////

using namespace ::flux::inttypes;

//--    <<< ~ flux namespace <<<                    ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE()
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_STD_INTTYPES_HPP_21302393_378E_480A_9549_B94603D18253
