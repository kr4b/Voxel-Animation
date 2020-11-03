/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	VecTraits
 *
 * TODO: cleanup; most of the stuff has been factored into [un]initialized_*
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_E86B7521_4EAF_4003_A793_4AC896D4E600
#define DETAIL_E86B7521_4EAF_4003_A793_4AC896D4E600

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/std/assert.hpp>
#include <flux/std/static_assert.hpp>

#include <flux/ftl/is_bitwise.hpp>
#include <flux/ftl/uninitialized.hpp>

#include <flux/util/hint.hpp>
#include <flux/util/scope_exit.hpp>
#include <flux/compat/type_traits.hpp>

#include <type_traits>

#include <cstring>

#include "vector-private.hxx"

//--    >>> namespace = flux::ftl::detail >>>       ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(ftl,detail)
//--    VecReallocImpl_                     ///{{{2///////////////////////////
namespace check_
{
	template< typename tType >
	std::false_type tag_is_realloc_alloc( tType&& );
};

template< typename tType, class tAlloc >
struct VecReallocImplDef_
{
	using ATraits_ = std::allocator_traits<tAlloc>;
	using pointer = typename ATraits_::pointer;

	static pointer realloc( tAlloc& aAlloc, std::size_t aReqSize, pointer aBuf, std::size_t aBufSize, pointer aBeg, pointer aEnd, pointer& aEndOut )
	{
		//TODO: revisit ... what happens when exceptions are thrown?
		
		pointer buf = ATraits_::allocate( aAlloc, aReqSize );
		if( FLUX_HINT_UNLIKELY(!buf) )
			vec_throw_bad_alloc();

		pointer beg = buf, end = buf;
		std::size_t size = aReqSize;

		FLUX_UTIL_ON_SCOPE_EXIT
		{
			ftl::destroy( aAlloc, beg, end );
			if( buf ) ATraits_::deallocate( aAlloc, buf, size );
		};

		ftl::uninitialized_relocate_if_noexcept_p( aAlloc, aBeg, aEnd, end );
		aEndOut = end;
		end = beg;

		buf = aBuf;
		size = aBufSize;
		return beg;
	}
};

template< typename tType, class tAlloc >
struct VecReallocImplRealloc_
{
	using ATraits_ = std::allocator_traits<tAlloc>;
	using pointer = typename ATraits_::pointer;

	FLUX_STATIC_ASSERT(ftl::is_bitwise_relocatable<tType>::value || (compat::is_trivially_copyable<tType>::value && compat::is_trivially_destructible<tType>::value) );

	FLUX_DIAGNOSTIC_STATE_PUSH_AND_SET(IGNORED,CLASS_MEMACCESS)
	static pointer realloc( tAlloc& aAlloc, std::size_t aReqSize, pointer aBuf, std::size_t aBufSize, pointer aBeg, pointer aEnd, pointer& aEndOut )
	{
		/* Note (for further investigation). realloc() will always copy the 
		 * whole buffer. In our case, this may include unused parts. If
		 * realloc() can just expand the buffer, that's OK. If it needs to 
		 * allocate a new buffer, and copy data over to that buffer, this may
		 * be a pessimization, as it may end up copying too large a buffer.
		 */
		
		pointer buf = aAlloc.realloc( aReqSize, aBuf, aBufSize );
		if( FLUX_HINT_UNLIKELY(!buf) )
			vec_throw_bad_alloc();

		FLUX_ASSERT( !aBuf || aBuf == aBeg );
		FLUX_ASSERT( 0 == aBufSize || aBufSize >= std::size_t(aEnd-aBeg) );

		if( !aBuf )
		{
			FLUX_ASSERT( (buf && aBeg) || aEnd == aBeg );
			auto const count = aEnd-aBeg;
			if( count ) std::memcpy( buf, aBeg, sizeof(tType)*count );
		}

		aEndOut = buf + (aEnd-aBeg);
		return buf;
		FLUX_DIAGNOSTIC_STATE_POP_OR_DEFAULT(CLASS_MEMACCESS)
	}
};

using check_::tag_is_realloc_alloc;

template< typename tType, class tAlloc >
struct VecReallocImpl_
	: std::conditional<
		std::is_same<std::true_type,decltype(tag_is_realloc_alloc(std::declval<tAlloc>()))>::value,
		VecReallocImplRealloc_<tType,tAlloc>,
		VecReallocImplDef_<tType,tAlloc>
	>::type
{};

//--    VecTraits                           ///{{{2///////////////////////////
template< typename tType, class tAlloc >
struct VecTraits
	: VecReallocImpl_< tType, tAlloc >
{
	using has_realloc = decltype(tag_is_realloc_alloc(std::declval<tAlloc>()));
};

//--    <<< ~ flux::ftl::detail namespace <<<       ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(ftl,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_E86B7521_4EAF_4003_A793_4AC896D4E600
