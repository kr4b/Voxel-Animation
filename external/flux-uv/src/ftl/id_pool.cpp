/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	IdPool<>
 */
/*-***************************************************************** -}}}1- */

#include <flux/ftl/id_pool.hpp>
#include <flux/ftl/errors.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/assert.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::ftl::detail >>>       ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(ftl,detail)
//--    throw_id_pool_*()                   ///{{{2///////////////////////////
FLUX_ATTR_NORETURN void throw_id_pool_mem_limit()
{
	FLUX_THROW( ftl::error::IdPoolMemLimit );
}

FLUX_ATTR_NORETURN void throw_id_pool_exhausted()
{
	FLUX_THROW( ftl::error::IdPoolExhausted );
}

FLUX_ATTR_NORETURN void throw_id_pool_not_allocated( int64_t aId, std::size_t aRange )
{
	FLUX_THROW( ftl::error::IdPoolNotAllocated )
		<< einfo::ErrorDesc( FLUX_FMT_STRING("IdPool: Range [{:d} {:d}+{:d}] not allocated"), aId, aId, aRange )
	;
}

//--    <<< ~ flux::ftl::detail namespace <<<       ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(ftl,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
