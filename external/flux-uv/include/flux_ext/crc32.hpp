/*-******************************************************* -- HEADER -{{{1- */
/*-	CRC32 functions
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_EXT_MISC_CRC32_HPP_7282EF05_049A_4D18_9D6E_9DE47DF5D774
#define FLUX_EXT_MISC_CRC32_HPP_7282EF05_049A_4D18_9D6E_9DE47DF5D774

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg> 
#include <flux/std/inttypes.hpp>

#include <cstddef>

//--    crc32                       ///{{{1///////////////////////////////////
namespace flux_ext
{
	/* \brief CRC32 computation
	 *
	 * Modified implementation of the CRC32 "slicing-by-4" method described at 
	 *   - https://create.stephan-brumme.com/crc32/
	 *   - https://github.com/stbrumme/crc32
	 *
	 * The original code is (c) 2011-2019 Stephan Brumme and released under
	 * a ZLib license (see https://github.com/stbrumme/crc32/tree/91c3898bc9e7d0b15741cda70d4025b9f37a6bb3). The modified version is
	 * rewritten to avoid (potentially) misaligned loads of 32-bit words.
	 */
	flux::uint32_t crc32_fast(
		void const* aData, std::size_t aLenght,
		flux::uint32_t aPreviousCRC32 = 0
	) noexcept;
};

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_EXT_MISC_CRC32_HPP_7282EF05_049A_4D18_9D6E_9DE47DF5D774
