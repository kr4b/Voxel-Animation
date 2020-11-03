/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Vector realloc() allocator
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_0C4F6B66_0995_422D_BEF6_3BE7968B0A36
#define DETAIL_0C4F6B66_0995_422D_BEF6_3BE7968B0A36

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <new>
#include <limits>
#include <type_traits>

#include <cstdlib>

#include "vector-private.hxx"

//--    >>> namespace = flux::ftl::detail >>>       ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(ftl,detail)
//--    VecReallocAlloc                     ///{{{2///////////////////////////
template< typename tType >
struct VecReallocAlloc
{
	// default interface
	using value_type = tType;
	using pointer = tType*;

	using propagate_on_container_copy_assignment = std::false_type;
	using propagate_on_container_move_assignment = std::true_type;
	using propagate_on_container_swap = std::false_type;

	VecReallocAlloc() noexcept = default;

	VecReallocAlloc( VecReallocAlloc const& ) noexcept = default;
	VecReallocAlloc& operator= (VecReallocAlloc const&) noexcept = default;

	VecReallocAlloc( VecReallocAlloc&& ) noexcept = default;
	VecReallocAlloc& operator= (VecReallocAlloc&&) noexcept = default;

	template< typename tOther >
	VecReallocAlloc( VecReallocAlloc<tOther> const& ) noexcept
	{}
	template< typename tOther >
	VecReallocAlloc& operator= (VecReallocAlloc<tOther> const&) noexcept
	{}

	pointer allocate( std::size_t aCount ) const
	{
		return static_cast<tType*>(malloc_n(sizeof(tType), aCount));
	}
	void deallocate( pointer aType, std::size_t ) const noexcept
	{
		std::free( aType );
	}

	template< typename tOther >
	bool operator== (VecReallocAlloc<tOther> const&) const noexcept
	{
		return true;
	}
	template< typename tOther >
	bool operator!= (VecReallocAlloc<tOther> const&) const noexcept
	{
		return false;
	}

	// extended interaface
	pointer realloc( std::size_t aRequired, pointer aPrev, std::size_t /*aOld*/ ) const
	{
		return static_cast<tType*>(realloc_n( aPrev, sizeof(tType), aRequired ));
	}
};

template< typename tType >
std::true_type tag_is_realloc_alloc( VecReallocAlloc<tType>&& );

//--    <<< ~ flux::ftl::detail namespace <<<       ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(ftl,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_0C4F6B66_0995_422D_BEF6_3BE7968B0A36
