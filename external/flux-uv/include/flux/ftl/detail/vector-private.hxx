/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Private methods
 */
/*-***************************************************************** -}}}1- */

#ifndef VECTOR_PRIVATE_HXX_946C0F90_B7BD_4530_AD8B_BAA03200AFDA
#define VECTOR_PRIVATE_HXX_946C0F90_B7BD_4530_AD8B_BAA03200AFDA

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/util/attrib.hpp>

#include <cstddef>

//--    >>> namespace = flux::ftl::detail >>>       ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(ftl,detail)
//--    functions                           ///{{{2///////////////////////////

void* malloc_n( std::size_t, std::size_t );
void* realloc_n( void*, std::size_t, std::size_t );


FLUX_ATTR_NORETURN void vec_throw_bad_alloc();
FLUX_ATTR_NORETURN void vec_throw_bad_array_new_length();

FLUX_ATTR_NORETURN
void vec_throw_out_of_range( char const*, std::size_t, std::size_t );

//--    <<< ~ flux::ftl::detail namespace <<<       ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(ftl,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // VECTOR_PRIVATE_HXX_946C0F90_B7BD_4530_AD8B_BAA03200AFDA
