/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Vector<> private methods
 */
/*-***************************************************************** -}}}1- */

#include <flux/ftl/detail/vector-private.hxx>

#include <flux/util/hint.hpp>
#include <flux/std/format.hpp>

#include <new>
#include <limits>
#include <stdexcept>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::ftl::detail >>>       ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(ftl,detail)
//--    *alloc_n()                          ///{{{2///////////////////////////
void* malloc_n( std::size_t aSize, std::size_t aCount )
{
	if( FLUX_HINT_UNLIKELY(aCount == 0) )
		return nullptr;

	//TODO: use __builtin_umull_overflow()
	if( FLUX_HINT_UNLIKELY(aCount > std::numeric_limits<std::size_t>::max()/aSize) )
		throw std::bad_array_new_length{};

	return std::malloc( aSize*aCount );
}
void* realloc_n( void* aPrev, std::size_t aSize, std::size_t aCount )
{
	if( FLUX_HINT_UNLIKELY(aCount == 0) )
	{
		std::free( aPrev );
		return nullptr;
	}

	//TODO: use __builtin_umull_overflow()
	if( FLUX_HINT_UNLIKELY(aCount > std::numeric_limits<std::size_t>::max()/aSize) )
		throw std::bad_array_new_length{};

	return std::realloc( aPrev, aSize*aCount );
}

//--    vec_throw_bad_*()                   ///{{{2///////////////////////////
FLUX_ATTR_NORETURN
void vec_throw_bad_alloc()
{
	throw std::bad_alloc();
}
FLUX_ATTR_NORETURN
void vec_throw_bad_array_new_length()
{
	throw std::bad_array_new_length{};
}

//--    vec_throw_out_of_range()            ///{{{2///////////////////////////
FLUX_ATTR_NORETURN
void vec_throw_out_of_range( char const* aWho, std::size_t aIndex, std::size_t aLimit )
{
	throw std::out_of_range( format( "{}(): out of range ({:d} >= {:d})", aWho, aIndex, aLimit ) );
}

//--    <<< ~ flux::ftl::detail namespace <<<       ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(ftl,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
