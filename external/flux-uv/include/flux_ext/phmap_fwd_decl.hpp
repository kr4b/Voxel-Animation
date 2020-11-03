/*-******************************************************* -- HEADER -{{{1- */
/*-	Forward declarations for the parallel hashmap types
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_EXT_MISC_PHMAP_FWD_DECL_HPP_0BF9A778_1DC4_4E9E_A649_E9952CA181A6
#define FLUX_EXT_MISC_PHMAP_FWD_DECL_HPP_0BF9A778_1DC4_4E9E_A649_E9952CA181A6

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg> 

//--    <parallel_hashmap/phmap_fwd_decl.h>     ///{{{1///////////////////////

#include <parallel_hashmap/phmap_fwd_decl.h>

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
#endif // FLUX_EXT_MISC_PHMAP_FWD_DECL_HPP_0BF9A778_1DC4_4E9E_A649_E9952CA181A6
