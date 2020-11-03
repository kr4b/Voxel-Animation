/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Per-process random value
 */
/*-***************************************************************** -}}}1- */

#include <flux/std/process_random.hpp>

#include <random>

#include <ctime>
#include <cctype>
#include <cstdlib>

//--//////////////////////////////////////////////////////////////////////////
//--    config                              ///{{{2///////////////////////////

#define FLUX_PROCESS_RANDOM_ENV_ "FLUXENV_PROCESS_RANDOM"

//--    >>> namespace = flux >>>                    ///{{{1///////////////////
FLUX_NAMESPACE_ENTER()
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	size_t generate_process_random_();
	size_t gProcessRandomValue_ = generate_process_random_();
}

//--    l: generate_process_random_()       ///{{{2///////////////////////////
namespace
{
	size_t generate_process_random_()
	{
		// overridden in environment?
		if( char const* rvs = getenv(FLUX_PROCESS_RANDOM_ENV_) )
		{
			char* end = nullptr;
			long long randomValue = strtoll( rvs, &end, 0 );

			if( end && end != rvs && *end == '\0' )
			{
				return std::size_t(randomValue);
			}
		}
		
		// generate random value
		/* random_device::entropy() *would* tell us if the random_device is
		 * deterministic, if it weren't for broken implementations, as noted
		 * on
		 *
		 * http://en.cppreference.com/w/cpp/numeric/random/random_device/entropy
		 *
		 * Instead, instantiate three devices, and assume non-determinism if
		 * one of the three returns a different value.
		 */
		std::random_device rd1, rd2, rd3;
		unsigned int r1 = rd1(), r2 = rd2(), r3 = rd3();

		if( r1 != r2 || r1 != r3 )
		{
			std::uniform_int_distribution<std::size_t> dist;
			return dist(rd1);
		}

		// fallback: use PRNG with time as seed
		/* Either random_device is deterministic, or we just won the lottery.
		 * In the latter case ... well, shit.
		 */
		std::mt19937 prng( unsigned(time(nullptr)) ); //TODO: better time source
		std::uniform_int_distribution<std::size_t> dist;

		return dist(prng);
	}
}

//--    constants                           ///{{{2///////////////////////////

size_t const& kProcessRandom = gProcessRandomValue_;

//--    <<< ~ flux namespace <<<                    ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE()
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
