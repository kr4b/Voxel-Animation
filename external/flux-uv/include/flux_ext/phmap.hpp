/*-******************************************************* -- HEADER -{{{1- */
/*-	Parallel hashmap types
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_EXT_MISC_PHMAP_HPP_DE7EAD04_2827_4D5D_88A0_F1F57A3AD1C6
#define FLUX_EXT_MISC_PHMAP_HPP_DE7EAD04_2827_4D5D_88A0_F1F57A3AD1C6

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/arch.hpp>
#include <flux/setup/compiler.hpp>

//--    fixups                          ///{{{1///////////////////////////////

/* Workaround: "error C3861:_BitScanForward{,64}: identifier not found" when
 * building under MSVC.
 */
#if FLUX_COMPILER_FAMILY == FLUX_COMPILER_MSVC
#	include <intrin.h>
#	pragma intrinsic(_BitScanForward)
#	if FLUX_ARCH == FLUX_ARCH_ARM64 || FLUX_ARCH == FLUX_ARCH_X86_64
#		pragma intrinsic(_BitScanForward64)
#	endif // ~ ARCH
#endif // ~ MSVC

//--    <parallel_hashmap/phmap.h>      ///{{{1///////////////////////////////

#include <parallel_hashmap/phmap.h>

/* Also place the types in flux_ext
 */
namespace flux_ext
{
	using phmap::flat_hash_set;
	using phmap::flat_hash_map;

	using phmap::node_hash_set;
	using phmap::node_hash_map;

	using phmap::parallel_flat_hash_set;
	using phmap::parallel_flat_hash_map;

	using phmap::parallel_node_hash_set;
	using phmap::parallel_node_hash_map;
}

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_EXT_MISC_PHMAP_HPP_DE7EAD04_2827_4D5D_88A0_F1F57A3AD1C6
