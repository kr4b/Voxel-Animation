/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- SOBuffer<> implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/std/assert.hpp>

#include <flux/compat/launder.hpp>
#include <flux/compat/constexpr.hpp>

#include <new>
#include <memory>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    $ local                             ///{{{2///////////////////////////
namespace detail
{
	template< typename tType, std::size_t tSize, std::size_t tAlign >
	struct SOBufferEligible
		: std::integral_constant< 
			bool,
			   sizeof(tType) <= tSize
			&& ((tAlign%alignof(tType)) == 0)
		>
	{};

	template< typename tType, typename tBuffer > inline FLUX_CONSTEXPR_EX
	tType* sobuffer_get_ptr( tBuffer& aBuffer, std::true_type ) noexcept
	{
		/* The below line causes an ICE with MSVC 19.14.26433... */
#		if FLUX_COMPILER != FLUX_COMPILER_MSVC || _MSC_FULL_VER != 191426433
		return compat::launder( reinterpret_cast<tType*>(&aBuffer) );
#		else
		return reinterpret_cast<tType*>(&aBuffer);
#		endif
	}
	template< typename tType, typename tBuffer > inline FLUX_CONSTEXPR_EX
	tType* sobuffer_get_ptr( tBuffer& aBuffer, std::false_type ) noexcept
	{
		return static_cast<tType*>( *reinterpret_cast<void**>(&aBuffer) );
	};

	template< typename tType, std::size_t, std::size_t, typename tBuffer, typename... tArgs > inline FLUX_CONSTEXPR_EX
	tType* sobuffer_construct( tBuffer& aBuffer, std::true_type, tArgs&&... aArgs ) noexcept(std::is_nothrow_constructible<tType,tArgs...>::value)
	{
		return new (&aBuffer) tType( std::forward<tArgs>(aArgs)... );
	}
	template< typename tType, std::size_t tSize, std::size_t tAlign, typename tBuffer, typename... tArgs > inline
	tType* sobuffer_construct( tBuffer& aBuffer, std::false_type, tArgs&&... aArgs )
	{
		FLUX_STATIC_ASSERTM( tSize >= sizeof(void*), "SOBuffer<>: can't hold arbitrary type, buffer is too small to hold a pointer" );
		FLUX_STATIC_ASSERTM( (tAlign%alignof(void*)) == 0, "SOBuffer<>: can't hold arbitrary type, buffer is misaligned w.r.t. pointer types" );

		tType* ret;
		using VoidPtr_ = void*;
		new (&aBuffer) VoidPtr_( (ret = new tType( std::forward<tArgs>(aArgs)... )) );
		return ret;
	}

	template< typename tType, std::size_t, std::size_t, class tAlloc, typename tBuffer, typename... tArgs > inline FLUX_CONSTEXPR_EX
	tType* sobuffer_construct_a( tAlloc& aAlloc, tBuffer& aBuffer, std::true_type, tArgs&&... aArgs )
	{
		using ATraits_ = std::allocator_traits<tAlloc>;

		tType* ptr = sobuffer_get_ptr<tType>(aBuffer, std::true_type{});
		ATraits_::construct( aAlloc, ptr, std::forward<tArgs>(aArgs)... );
		return ptr;
	}
	template< typename tType, std::size_t tSize, std::size_t tAlign, class tAlloc, typename tBuffer, typename... tArgs > inline
	tType* sobuffer_construct_a( tAlloc& aAlloc, tBuffer& aBuffer, std::false_type, tArgs&&... aArgs )
	{
		FLUX_STATIC_ASSERTM( tSize >= sizeof(void*), "SOBuffer<>: can't hold out-of-place type, in-place storage is too small to hold a pointer" );
		FLUX_STATIC_ASSERTM( (tAlign%alignof(void*)) == 0, "SOBuffer<>: can't hold out-of-place type, in-place storage is misaligned w.r.t. pointer types" );

		using ATraits_ = std::allocator_traits<tAlloc>;

		tType* ptr = ATraits_::allocate( aAlloc, 1 );
		try
		{
			ATraits_::construct( aAlloc, ptr, std::forward<tArgs>(aArgs)... );
		}
		catch( ... )
		{
			ATraits_::deallocate( aAlloc, ptr, 1 );
			throw;
		}

		using VoidPtr_ = void*;
		new (&aBuffer) VoidPtr_( ptr );

		return ptr;
	}


	template< typename tType, typename tBuffer > inline FLUX_CONSTEXPR_EX
	void sobuffer_destroy( tBuffer& aBuffer, std::true_type ) noexcept(std::is_nothrow_destructible<tType>::value)
	{
		sobuffer_get_ptr<tType>(aBuffer, std::true_type{})->~tType();
	}
	template< typename tType, typename tBuffer > inline
	void sobuffer_destroy( tBuffer& aBuffer, std::false_type )
	{
		delete sobuffer_get_ptr<tType>(aBuffer, std::false_type{});
	}

	template< typename tType, class tAlloc, typename tBuffer > inline
	void sobuffer_destroy_a( tAlloc& aAlloc, tBuffer& aBuffer, std::true_type )
	{
		using ATraits_ = std::allocator_traits<tAlloc>;

		/* The below line causes an ICE with MSVC 19.14.26433... */
#		if FLUX_COMPILER != FLUX_COMPILER_MSVC || _MSC_FULL_VER != 191426433
		ATraits_::destroy( aAlloc, sobuffer_get_ptr<tType>(aBuffer, std::true_type{}) );
#		else // _MSC_FULL_VER == 191426433
		tType* ptr =  reinterpret_cast<tType*>(&aBuffer);
		ATraits_::destroy( aAlloc, ptr );
#		endif // ~ _MSC_FULL_VER
	}
	template< typename tType, class tAlloc, typename tBuffer > inline
	void sobuffer_destroy_a( tAlloc& aAlloc, tBuffer& aBuffer, std::false_type )
	{
		using ATraits_ = std::allocator_traits<tAlloc>;

		tType* ptr = sobuffer_get_ptr<tType>(aBuffer, std::false_type{});
		ATraits_::destroy( aAlloc, ptr );
		ATraits_::deallocate( aAlloc, ptr, 1 );
	}
}

//--    SOBuffer<>                          ///{{{2///////////////////////////
template< std::size_t tSize, std::size_t tAlign > template< typename tType, typename... tArgs > inline FLUX_CONSTEXPR_EX
tType* SOBuffer<tSize,tAlign>::construct( tArgs&&... aArgs ) noexcept((IsNothrowConstructible<tType,tArgs...>::value))
{
	using Eligible_ = detail::SOBufferEligible<tType,tSize,tAlign>;
	return detail::sobuffer_construct<tType,tSize,tAlign>( mBuffer, Eligible_{}, std::forward<tArgs>(aArgs)... );
}
template< std::size_t tSize, std::size_t tAlign > template< typename tType, class tAlloc, typename... tArgs > inline FLUX_CONSTEXPR_EX
tType* SOBuffer<tSize,tAlign>::construct_with_allocator( tAlloc& aAlloc, tArgs&&... aArgs )
{
	using Eligible_ = detail::SOBufferEligible<tType,tSize,tAlign>;
	return detail::sobuffer_construct_a<tType,tSize,tAlign>( aAlloc, mBuffer, Eligible_{}, std::forward<tArgs>(aArgs)... );
}


template< std::size_t tSize, std::size_t tAlign > template< typename tType > inline FLUX_CONSTEXPR_EX
void SOBuffer<tSize,tAlign>::destroy() noexcept(IsNothrowDestructible<tType>::value)
{
	using Eligible_ = detail::SOBufferEligible<tType,tSize,tAlign>;
	detail::sobuffer_destroy<tType>( mBuffer, Eligible_{} );
}
template< std::size_t tSize, std::size_t tAlign > template< typename tType, class tAlloc > inline FLUX_CONSTEXPR_EX
void SOBuffer<tSize,tAlign>::destroy_with_allocator( tAlloc& aAlloc )
{
	using Eligible_ = detail::SOBufferEligible<tType,tSize,tAlign>;
	detail::sobuffer_destroy_a<tType>( aAlloc, mBuffer, Eligible_{} );
}

template< std::size_t tSize, std::size_t tAlign > template< typename tType > inline FLUX_CONSTEXPR_EX
tType& SOBuffer<tSize,tAlign>::reference() & noexcept
{
	using Eligible_ = detail::SOBufferEligible<tType,tSize,tAlign>;
	return *detail::sobuffer_get_ptr<tType>( mBuffer, Eligible_{} );
};
template< std::size_t tSize, std::size_t tAlign > template< typename tType > inline FLUX_CONSTEXPR_EX
tType&& SOBuffer<tSize,tAlign>::reference() && noexcept
{
	using Eligible_ = detail::SOBufferEligible<tType,tSize,tAlign>;
	return std::move(*detail::sobuffer_get_ptr<tType>( mBuffer, Eligible_{} ));
};
template< std::size_t tSize, std::size_t tAlign > template< typename tType > inline FLUX_CONSTEXPR_EX
tType const& SOBuffer<tSize,tAlign>::reference() const& noexcept
{
	using Eligible_ = detail::SOBufferEligible<tType,tSize,tAlign>;
	return *detail::sobuffer_get_ptr<tType const>( mBuffer, Eligible_{} );
};

template< std::size_t tSize, std::size_t tAlign > FLUX_CONSTEXPR_EX inline
void* SOBuffer<tSize,tAlign>::get_raw_ptr( std::true_type ) noexcept
{
	return reinterpret_cast<void*>(&mBuffer);
}
template< std::size_t tSize, std::size_t tAlign > FLUX_CONSTEXPR_EX inline
void* SOBuffer<tSize,tAlign>::get_raw_ptr( std::false_type ) noexcept
{
	return *reinterpret_cast<void**>(&mBuffer);
}
template< std::size_t tSize, std::size_t tAlign > FLUX_CONSTEXPR_EX inline
void const* SOBuffer<tSize,tAlign>::get_raw_ptr( std::true_type ) const noexcept
{
	return reinterpret_cast<void const*>(&mBuffer);
}
template< std::size_t tSize, std::size_t tAlign > FLUX_CONSTEXPR_EX inline
void const* SOBuffer<tSize,tAlign>::get_raw_ptr( std::false_type ) const noexcept
{
	return *reinterpret_cast<void const**>(&mBuffer);
}

template< std::size_t tSize, std::size_t tAlign > inline
void* SOBuffer<tSize,tAlign>::allocate_raw( std::size_t aSize, std::true_type ) noexcept
{
	FLUX_ASSERT( aSize <= tSize ), (void)aSize;
	return get_raw_ptr( std::true_type{} );
}
template< std::size_t tSize, std::size_t tAlign > inline
void* SOBuffer<tSize,tAlign>::allocate_raw( std::size_t aSize, std::false_type )
{
	FLUX_STATIC_ASSERTM( tSize >= sizeof(void*), "SOBuffer<>: can't use out-of-place memory, in-place buffer is too small to hold a pointer" );
	FLUX_STATIC_ASSERTM( (tAlign%alignof(void*)) == 0, "SOBuffer<>: can't use out-of-place memory, in-place buffer is misaligned w.r.t. pointer types" );

	void* ret;
	using VoidPtr_ = void*;
	new (&mBuffer) VoidPtr_( (ret = ::operator new( aSize )) );
	return ret;
}

template< std::size_t tSize, std::size_t tAlign > inline
void SOBuffer<tSize,tAlign>::free_raw( std::true_type ) noexcept
{}
template< std::size_t tSize, std::size_t tAlign > inline
void SOBuffer<tSize,tAlign>::free_raw( std::false_type )
{
	::operator delete( get_raw_ptr( std::false_type{} ) );
}


//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
