/*-******************************************************* -- HEADER -{{{1- */
/*-	Per-process random value
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_PROCESS_RANDOM_HPP_DB86D583_21DB_4426_BF09_4990F3DC0782
#define FLUX_STD_PROCESS_RANDOM_HPP_DB86D583_21DB_4426_BF09_4990F3DC0782

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/namespace.hpp>

#include <cstddef>

//--    >>> namespace = flux >>>                    ///{{{1///////////////////
FLUX_NAMESPACE_ENTER()
//--    constants                           ///{{{2///////////////////////////

/** A per-process random value
 *
 * `flux::kProcessRandom` contains a random value that is generated when the
 * process starts. It remains unchanged for the whole life-time of the process.
 *
 * `flux::kProcessRandom` is intended to be used as a seed for e.g., hasing
 * operations, where the same hash must be computed for identical inputs during
 * the lifetime of a process, but should be unpredictable to avoid algorithmic
 * complexity attacks on hashed containers.
 *
 * \note `fork`ed proesses will have the same `kProcessRandom` as their
 * parents. (Which is useful if in-memory hash structures are shared...)
 *
 * It's possible to manually specify a random value using the environment
 * variable `FLUXENV_PROCESS_RANDOM`. The environment variable's value is
 * parsed using `strtoll()` with base 0. The whole value must be consumed, or
 * else it will be ignored.
 */
extern std::size_t const& kProcessRandom;

//--    <<< ~ flux namespace <<<                    ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE()
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_STD_PROCESS_RANDOM_HPP_DB86D583_21DB_4426_BF09_4990F3DC0782
