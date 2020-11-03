/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- Vector<> main implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/std/assert.hpp>
#include <flux/compat/to_address.hpp>

#include <flux/ftl/in_buffer.hpp>
#include <flux/ftl/is_bitwise.hpp>
#include <flux/ftl/initialized.hpp>
#include <flux/ftl/uninitialized.hpp>

#include <flux/util/hint.hpp>
#include <flux/util/scope_exit.hpp>

#include <algorithm>
#include <stdexcept>
#include <functional>

#include "vector-traits.hxx"
#include "vector-realloc.hxx"
#include "vector-private.hxx"

#include "../defaults.hpp"
#include "../uninitialized.hpp"

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    $ local                             ///{{{2///////////////////////////
namespace detail
{
	// Default allocator => std::allocator
	template< bool, typename tType >
	struct VecDefaultAllocatorImpl_
	{
		using type = std::allocator<tType>;
	};

#	if FLUXCFG_FTL_REALLOC_ALLOC
	// realloc()-enabled
	template< typename tType >
	struct VecDefaultAllocatorImpl_<true,tType>
	{
		using type = detail::VecReallocAlloc<tType>;
	};
#	endif // ~ CFG_FTL_REALLOC_ALLOC
};

//--    d: VecDefaultAllocator              ///{{{2///////////////////////////
namespace detail
{
	template< typename tType >
	struct VecDefaultAllocator
	{
		using type = typename VecDefaultAllocatorImpl_<
			ftl::is_bitwise_relocatable<tType>::value,
			tType
		>::type;
	};
}

//--    VectorBase<>::Impl_                 ///{{{2///////////////////////////
template< typename tType, class tAlloc > inline
VectorBase<tType,tAlloc>::Impl_::Impl_( tAlloc&& aAllc, pointer aBeg, pointer aCap )
	: tAlloc(std::move(aAllc))
	, beg(aBeg)
	, end(aBeg)
	, capacity(aCap)
{}
template< typename tType, class tAlloc > inline
VectorBase<tType,tAlloc>::Impl_::Impl_( tAlloc const& aAllc, pointer aBeg, pointer aCap )
	: tAlloc(aAllc)
	, beg(aBeg)
	, end(aBeg)
	, capacity(aCap)
{}

//--    VectorBase<>                        ///{{{2///////////////////////////
#define FLUXTMP_VECTOR_CLASS_ VectorBase<tType,tAlloc>
#define FLUXTMP_VECTOR_TEMPL_ template< typename tType, class tAlloc >

	// [cd]tor -{{{3-
FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_::VectorBase()
	: mImpl( tAlloc() )
{}
FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_::VectorBase( size_type aSize, value_type const& aVal, tAlloc const& aAlloc )
	: mImpl( aAlloc )
{
	fill_val_( aSize, aVal );
}

FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_::VectorBase( tAlloc const& aAlloc )
	: mImpl( aAlloc )
{}
FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_::VectorBase( size_type aSize, tAlloc const& aAlloc )
	: mImpl( aAlloc )
{
	fill_def_( aSize, ValueInitTag{} );
}
FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_::VectorBase( size_type aSize, DefaultInitTag, tAlloc const& aAlloc )
	: mImpl( aAlloc )
{
	fill_def_( aSize, DefaultInitTag{} );
}


FLUXTMP_VECTOR_TEMPL_ template< typename tIter, class tCat > inline
FLUXTMP_VECTOR_CLASS_::VectorBase( tIter aBeg, tIter aEnd, tAlloc const& aAlloc )
	: mImpl( aAlloc )
{
	assign_copy_( aBeg, aEnd, tCat{} );
}

FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_::VectorBase( std::initializer_list<value_type> aList, tAlloc const& aAlloc )
	: mImpl( aAlloc )
{
	assign( aList );
}


FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_::~VectorBase()
{
	ftl::destroy( mImpl, mImpl.beg, mImpl.end );

	if( !in_place() && mImpl.beg )
		ATraits_::deallocate( mImpl, mImpl.beg, capacity() );
}

	// copy/move -{{{3-
FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_::VectorBase( VectorBase&& aOther )
	: mImpl( std::move(static_cast<tAlloc&>(aOther.mImpl)) )
{
	if( aOther.empty() )
		return;

	if( !aOther.in_place() )
	{
		mImpl.beg = aOther.mImpl.beg;
		mImpl.end = aOther.mImpl.end;
		mImpl.capacity = aOther.mImpl.capacity;

		aOther.mImpl.beg = aOther.mImpl.end = nullptr;
		aOther.mImpl.capacity.set_pointer( nullptr );
	}
	else
	{
		reserve( aOther.size() );

		auto beg = aOther.mImpl.beg;
		ftl::uninitialized_relocate_if_noexcept_p( mImpl, beg, aOther.mImpl.end, mImpl.end );
		aOther.mImpl.end = aOther.mImpl.beg;
	}
}
FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_::VectorBase( VectorBase&& aOther, tAlloc const& aAlloc )
	: mImpl( aAlloc )
{
	if( !aOther.in_place() && static_cast<tAlloc&>(mImpl) == static_cast<tAlloc&>(aOther.mImpl) )
	{
		mImpl.beg = aOther.mImpl.beg;
		mImpl.end = aOther.mImpl.end;
		mImpl.capacity = aOther.mImpl.capacity;

		aOther.mImpl.beg = aOther.mImpl.end = nullptr;
		aOther.mImpl.capacity.set_pointer( nullptr );
	}
	else
	{
		reserve( aOther.size() );

		auto beg = aOther.mImpl.beg;
		ftl::uninitialized_move_if_noexcept_p( mImpl, beg, aOther.mImpl.end, mImpl.end );

		aOther.clear();
	}
}

FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_& FLUXTMP_VECTOR_CLASS_::operator= (VectorBase&& aOther)
{
	FLUX_ASSERT( this != &aOther );

	assign_move_other_( std::move(aOther), typename ATraits_::propagate_on_container_move_assignment{} );

	return *this;
}

FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_::VectorBase( VectorBase const& aOther )
	: mImpl( ATraits_::select_on_container_copy_construction(aOther.mImpl) )
{
	assign_copy_( aOther.mImpl.beg, aOther.mImpl.end, std::random_access_iterator_tag{} );
}
FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_::VectorBase( VectorBase const& aOther, tAlloc const& aAlloc )
	: mImpl( aAlloc )
{
	assign_copy_( aOther.mImpl.beg, aOther.mImpl.end, std::random_access_iterator_tag{} );
}

FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_& FLUXTMP_VECTOR_CLASS_::operator= (VectorBase const& aOther)
{
	if( this == &aOther )
		return *this;

	if( ATraits_::propagate_on_container_copy_assignment::value )
	{
		tAlloc newAlloc{ aOther.mImpl };
		if( newAlloc != mImpl )
		{
			ftl::destroy( mImpl, mImpl.beg, mImpl.end );

			if( !in_place() )
			{
				ATraits_::deallocate( mImpl, mImpl.beg, capacity() );

				mImpl.beg = nullptr;
				mImpl.capacity.set_pointer( nullptr );
			}

			mImpl.end = mImpl.beg;
		}

		static_cast<tAlloc&>(mImpl) = std::move(newAlloc);
	}

	assign_copy_( aOther.mImpl.beg, aOther.mImpl.end, std::random_access_iterator_tag{} );
	return *this;
}

FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_& FLUXTMP_VECTOR_CLASS_::operator= (std::initializer_list<value_type> aList )
{
	assign( aList );
	return *this;
}

	// [cd]tor internal -{{{3-
FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_::VectorBase( InplaceStorage_, VoidPtr_ aBeg, VoidPtr_ aCap, size_type aCount, tAlloc const& aAlloc )
	: mImpl( aAlloc, pointer(aBeg), pointer(aCap) )
{
	fill_def_( aCount, ValueInitTag{} );
}
FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_::VectorBase( InplaceStorage_, VoidPtr_ aBeg, VoidPtr_ aCap, size_type aCount, DefaultInitTag, tAlloc const& aAlloc )
	: mImpl( aAlloc, pointer(aBeg), pointer(aCap) )
{
	fill_def_( aCount, DefaultInitTag{} );
}
FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_::VectorBase( InplaceStorage_, VoidPtr_ aBeg, VoidPtr_ aCap, size_type aCount, value_type const& aVal, tAlloc const& aAlloc )
	: mImpl( aAlloc, pointer(aBeg), pointer(aCap) )
{
	fill_val_( aCount, aVal );
}

FLUXTMP_VECTOR_TEMPL_ template< typename tIter, class tCat > inline
FLUXTMP_VECTOR_CLASS_::VectorBase( InplaceStorage_, VoidPtr_ aBeg, VoidPtr_ aCap, tIter aInBeg, tIter aInEnd, tAlloc const& aAlloc )
	: mImpl( aAlloc, pointer(aBeg), pointer(aCap) )
{
	assign_copy_( aInBeg, aInEnd, tCat{} );
}

	// public operators -{{{3-
FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::operator[] (size_type aIndex) -> reference
{
	FLUX_ASSERT( aIndex < size() );
	return mImpl.beg[aIndex];
}
FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::operator[] (size_type aIndex) const -> const_reference
{
	FLUX_ASSERT( aIndex < size() );
	return mImpl.beg[aIndex];
}

	// public -{{{3-
FLUXTMP_VECTOR_TEMPL_ inline
bool FLUXTMP_VECTOR_CLASS_::empty() const noexcept
{
	return mImpl.beg == mImpl.end;
}

FLUXTMP_VECTOR_TEMPL_ inline
bool FLUXTMP_VECTOR_CLASS_::in_place() const noexcept
{
	return !mImpl.capacity.flag();
}

FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::size() const noexcept -> size_type
{
	return mImpl.end - mImpl.beg;
}
FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::capacity() const noexcept -> size_type
{
	return mImpl.capacity.pointer() - mImpl.beg;
}
FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::max_size() const noexcept -> size_type
{
	/* In C++17, max_size() is supposed to adjust for the element size by 
	 * dividing with sizeof(value_type); however, this is not (necessarily)
	 * the case previously. Essentially, before C++17, this returns a more
	 * fantastic value than after.
	 */
	return ATraits_::max_size( mImpl );
}

FLUXTMP_VECTOR_TEMPL_ inline
void FLUXTMP_VECTOR_CLASS_::reserve( size_type aCount )
{
	if( capacity() < aCount )
		reserve_( aCount );
}
FLUXTMP_VECTOR_TEMPL_ inline
void FLUXTMP_VECTOR_CLASS_::shrink_to_fit()
{
	/* Shrinking the in-place buffer is never a good idea, so don't do that.
	 * It's a non-binding request anyway.
	 */
	if( in_place() )
		return;

	size_type const cap = capacity();
	size_type const required = size();

	if( cap > required )
	{
		pointer end;
		pointer buf = VTraits_::realloc( mImpl, required, mImpl.beg, cap, mImpl.beg, mImpl.end, end );

		FLUX_ASSERT( buf );

		mImpl.beg = buf;
		mImpl.end = end;
		mImpl.capacity.set_pointer( buf + required );

		FLUX_ASSERT( !in_place() );
	}
}

FLUXTMP_VECTOR_TEMPL_ inline
void FLUXTMP_VECTOR_CLASS_::resize( size_type aRequested )
{
	size_type const current = size();
	if( aRequested < current )
	{
		size_type const kill = current-aRequested;
		pointer const end = mImpl.end - kill;

		ftl::destroy( mImpl, end, mImpl.end );
		mImpl.end = end;
	}
	else
	{
		if( capacity() < aRequested )
			reserve_( aRequested );

		size_type const spawn = aRequested-current;
		ftl::uninitialized_value_construct_pn( mImpl, mImpl.end, spawn );
	}
}
FLUXTMP_VECTOR_TEMPL_ inline
void FLUXTMP_VECTOR_CLASS_::resize( size_type aRequested, value_type const& aVal )
{
	// Need this if aVal is a value inside ourselves.
	value_type value( aVal );
	
	size_type const current = size();
	if( aRequested < current )
	{
		resize( aRequested );
		return;
	}

	if( capacity() < aRequested )
		reserve_( aRequested );

	FLUX_ASSERT( mImpl.end );

	size_type const spawn = aRequested-current;
	ftl::uninitialized_fill_pn( mImpl, mImpl.end, spawn-1, value );
	ATraits_::construct( mImpl, compat::to_address(mImpl.end), std::move(value) );
	++mImpl.end;
}
FLUXTMP_VECTOR_TEMPL_ inline
void FLUXTMP_VECTOR_CLASS_::resize( size_type aRequested, DefaultInitTag )
{
	size_type const current = size();
	if( aRequested < current )
	{
		size_type const kill = current-aRequested;
		pointer const end = mImpl.end - kill;

		ftl::destroy( mImpl, end, mImpl.end );
		mImpl.end = end;
	}
	else
	{
		if( capacity() < aRequested )
			reserve_( aRequested /*TODO: DefaultInitTag*/ );

		size_type const spawn = aRequested-current;
		ftl::uninitialized_default_construct_pn( mImpl, mImpl.end, spawn );
	}
}


FLUXTMP_VECTOR_TEMPL_ inline
void FLUXTMP_VECTOR_CLASS_::clear() noexcept
{
	ftl::destroy( mImpl, mImpl.beg, mImpl.end );
	mImpl.end = mImpl.beg;
}

FLUXTMP_VECTOR_TEMPL_ inline
void FLUXTMP_VECTOR_CLASS_::assign( size_type aCount, value_type const& aVal )
{
	// Need this if aVal is a value inside ourselves.
	value_type value( aVal );
	
	ftl::destroy( mImpl, mImpl.beg, mImpl.end );
	mImpl.end = mImpl.beg;

	if( capacity() < aCount )
		reserve_( aCount );

	FLUX_ASSERT( mImpl.end );

	ftl::uninitialized_fill_p( mImpl, mImpl.end, mImpl.beg+aCount-1, value );
	ATraits_::construct( mImpl, compat::to_address(mImpl.end), std::move(value) );
	++mImpl.end;
}
FLUXTMP_VECTOR_TEMPL_ inline
void FLUXTMP_VECTOR_CLASS_::assign( std::initializer_list<value_type> aList )
{
	using Cat_ = typename compat::iterator_traits<decltype(aList.begin())>::iterator_category;
	assign_copy_( aList.begin(), aList.end(), Cat_{} );
}
FLUXTMP_VECTOR_TEMPL_ template< typename tIter, class tCat > inline
void FLUXTMP_VECTOR_CLASS_::assign( tIter aBeg, tIter aEnd )
{
	assign_copy_( aBeg, aEnd, tCat{} );
}

FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::insert( const_iterator aIt, value_type&& aVal ) -> iterator
{
	return emplace( aIt, std::move(aVal) );
}
FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::insert( const_iterator aIt, value_type const& aVal ) -> iterator
{
	return emplace( aIt, aVal );
}
FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::insert( const_iterator aIt, size_type aCount, value_type const& aVal ) -> iterator
{
	//TODO: consider exception mechanics....
	if( 0 == aCount )
		return const_cast<iterator>(aIt);
	
	pointer it = pointer(const_cast<iterator>(aIt));

	if( it == mImpl.end )
		return insert_back_( aCount, aVal );

	FLUX_ASSERT( ftl::in_buffer( mImpl.beg, mImpl.end, it ) );

	pointer ret = it;
	if( capacity() >= size() + aCount )
	{
		value_type value{ aVal };

		pointer cleanupBeg = mImpl.end, cleanupEnd = mImpl.end;
		FLUX_UTIL_ON_SCOPE_EXIT
		{
			ftl::destroy( cleanupBeg, cleanupEnd );
		};

		if( aCount < size() )
		{
			pointer at = mImpl.end - aCount;
			ftl::uninitialized_move_if_noexcept_p( mImpl, at, mImpl.end, cleanupEnd );

			pointer bt = mImpl.end - aCount;
			ftl::initialized_move_rev_p( it, bt, at );

			ftl::initialized_fill_pn( it, aCount-1, value );
			*it = std::move(value);
			
			//pointer at = mImpl.end-aCount;
			//mImpl.end = ftl::uninitialized_move_if_noexcept( mImpl, mImpl.end-aCount, mImpl.end, mImpl.end );
		}
		else
		{
			printf( "Hello thar\n" );
			//TODO 
			//TODO 
			//TODO 
			//TODO 
			throw 55;
		}

#	if 0
		//TODO TODO TODO: buggy.
		pointer at = mImpl.end-aCount;
		ftl::uninitialized_move_if_noexcept_p( mImpl, at, mImpl.end+aCount, 
		
		pointer at = it, bt = mImpl.end+aCount;
		ftl::initialized_move_rev_p( at, mImpl.end, bt ); //TODO
		ftl::initialized_fill_pn( it, aCount-1, value );

		*it = std::move(value);
#	endif
	}
	else
	{
		size_type const grown = size() + aCount;
		pointer buf = ATraits_::allocate( mImpl, grown );
		if( FLUX_HINT_UNLIKELY(!buf) )
			detail::vec_throw_bad_alloc();
		
		pointer beg = buf, end = buf;
		size_type size = grown;
		FLUX_UTIL_ON_SCOPE_EXIT
		{
			ftl::destroy( mImpl, beg, end );
			if( buf ) ATraits_::deallocate( mImpl, buf, size );
		};

		pointer bt = mImpl.beg;
		ftl::uninitialized_move_if_noexcept_p( mImpl, bt, it, end );

		ret = end;
		ftl::uninitialized_fill_pn( mImpl, end, aCount, aVal );

		pointer itt = it;
		ftl::uninitialized_move_if_noexcept_p( mImpl, itt, mImpl.end, end );

		if( !in_place() )
		{
			size = capacity();
			buf = mImpl.beg;
		}
		else
		{
			size = 0;
			buf = nullptr;
		}

		std::swap( beg, mImpl.beg );
		std::swap( end, mImpl.end );
		mImpl.capacity.set_flag( true );
		mImpl.capacity.set_pointer( mImpl.beg+grown );
	}

	return iterator(ret);
}

FLUXTMP_VECTOR_TEMPL_ template< typename tIter, class tCat >
auto FLUXTMP_VECTOR_CLASS_::insert( const_iterator aIt, tIter aBeg, tIter aEnd ) -> iterator
{
	if( aBeg == aEnd )
		return const_cast<iterator>(aIt);

	return insert_( aIt, aBeg, aEnd, tCat{} );
}

FLUXTMP_VECTOR_TEMPL_ template< typename... tArgs > inline
auto FLUXTMP_VECTOR_CLASS_::emplace( const_iterator aIt, tArgs&&... aArg ) -> iterator
{
	pointer it = pointer(const_cast<iterator>(aIt));

	if( it == mImpl.end )
	{
		emplace_back( std::forward<tArgs>(aArg)... );
		return iterator(mImpl.end-1);
	}

	FLUX_ASSERT( ftl::in_buffer( mImpl.beg, mImpl.end, it ) );

	pointer ret = it;
	if( FLUX_HINT_LIKELY(mImpl.end != mImpl.capacity.pointer()) )
	{
		value_type value( std::forward<tArgs>(aArg)... );

		auto at = mImpl.end-1;
		ftl::uninitialized_move_if_noexcept_p( mImpl, at, mImpl.end, mImpl.end );
		ftl::initialized_move_if_noexcept_rev( it, mImpl.end-2, mImpl.end-1 );

		*it = std::move(value);
	}
	else
	{
		size_type const grown = vector_grow( capacity(), sizeof(tType) );
		pointer buf = ATraits_::allocate( mImpl, grown );
		if( FLUX_HINT_UNLIKELY(!buf) )
			detail::vec_throw_bad_alloc();
		
		pointer beg = buf, end = buf;
		size_type size = grown;
		FLUX_UTIL_ON_SCOPE_EXIT
		{
			ftl::destroy( mImpl, beg, end );
			if( buf ) ATraits_::deallocate( mImpl, buf, size );
		};

		pointer bt = mImpl.beg;
		ftl::uninitialized_move_if_noexcept_p( mImpl, bt, it, end );

		ATraits_::construct( mImpl, compat::to_address(end), std::forward<tArgs>(aArg)... );
		ret = end++;

		pointer itt = it;
		ftl::uninitialized_move_if_noexcept_p( mImpl, itt, mImpl.end, end );

		if( !in_place() )
		{
			size = capacity();
			buf = mImpl.beg;
		}
		else
		{
			size = 0;
			buf = nullptr;
		}

		std::swap( beg, mImpl.beg );
		std::swap( end, mImpl.end );
		mImpl.capacity.set_flag( true );
		mImpl.capacity.set_pointer( mImpl.beg+grown );
	}

	return iterator(ret);
}

FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::erase( const_iterator aIt ) -> iterator
{
	pointer it = pointer(const_cast<iterator>(aIt));
	FLUX_ASSERT( ftl::in_buffer( mImpl.beg, mImpl.end, it ) );

	//VTraits_::shift_down( mImpl, it+1, mImpl.end, it, mImpl.end );
	
	pointer at = it+1, bt = it;
	ftl::initialized_move_if_noexcept_p( at, mImpl.end, bt );
	ftl::destroy( mImpl, bt, mImpl.end );
	mImpl.end = bt;

	return iterator(it);
}
FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::erase( const_iterator aBeg, const_iterator aEnd ) -> iterator
{
	pointer beg = pointer(const_cast<iterator>(aBeg));
	pointer end = pointer(const_cast<iterator>(aEnd));

	FLUX_ASSERT( ftl::in_buffer( mImpl.beg, mImpl.end, beg, end ) );

	if( beg == end )
		return iterator(end);

	//pointer ptr = mImpl.end;
	//VTraits_::shift_down( mImpl, end, mImpl.end, beg, ptr );
	//mImpl.end = ptr;
	
	pointer at = end, bt = beg;
	ftl::initialized_move_if_noexcept_p( at, mImpl.end, bt );
	ftl::destroy( mImpl, bt, mImpl.end );
	mImpl.end = bt;

	return iterator(beg);
}

FLUXTMP_VECTOR_TEMPL_ inline
void FLUXTMP_VECTOR_CLASS_::push_back( value_type&& aVal )
{
	emplace_back_( typename VTraits_::has_realloc{}, std::move(aVal) );
}
FLUXTMP_VECTOR_TEMPL_ inline
void FLUXTMP_VECTOR_CLASS_::push_back( value_type const& aVal )
{
	emplace_back_( typename VTraits_::has_realloc{}, aVal );
}

FLUXTMP_VECTOR_TEMPL_ template< typename... tArgs > inline
auto FLUXTMP_VECTOR_CLASS_::emplace_back( tArgs&&... aArgs ) -> reference
{
	return emplace_back_( typename VTraits_::has_realloc{}, std::forward<tArgs>(aArgs)... );
}

FLUXTMP_VECTOR_TEMPL_ inline
void FLUXTMP_VECTOR_CLASS_::pop_back() noexcept
{
	FLUX_ASSERT( mImpl.end != mImpl.beg );

	--mImpl.end;
	ftl::destroy_at( mImpl, compat::to_address(mImpl.end) );
}

FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::at( size_type aIndex ) -> reference
{
	if( FLUX_HINT_UNLIKELY(size() <= aIndex) )
		detail::vec_throw_out_of_range( "VectorBase::at", aIndex, size() );

	return mImpl.beg[aIndex];
}
FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::at( size_type aIndex)  const -> const_reference
{
	if( FLUX_HINT_UNLIKELY(size() <= aIndex) )
		detail::vec_throw_out_of_range( "VectorBase::at", aIndex, size() );

	return mImpl.beg[aIndex];
}

FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::front() noexcept -> reference
{
	FLUX_ASSERT( mImpl.beg != mImpl.end );
	return *mImpl.beg;
}
FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::front() const noexcept -> const_reference
{
	FLUX_ASSERT( mImpl.beg != mImpl.end );
	return *mImpl.beg;
}
FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::back() noexcept -> reference
{
	FLUX_ASSERT( mImpl.beg != mImpl.end );
	return *(mImpl.end-1);
}
FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::back() const noexcept -> const_reference
{
	FLUX_ASSERT( mImpl.beg != mImpl.end );
	return *(mImpl.end-1);
}

FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::begin() noexcept -> iterator
{
	return iterator(mImpl.beg);
}
FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::begin() const noexcept -> const_iterator
{
	return const_iterator(mImpl.beg);
}
FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::cbegin() const noexcept -> const_iterator
{
	return const_iterator(mImpl.beg);
}
FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::end() noexcept -> iterator
{
	return iterator(mImpl.end);
}
FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::end() const noexcept -> const_iterator
{
	return const_iterator(mImpl.end);
}
FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::cend() const noexcept -> const_iterator
{
	return const_iterator(mImpl.end);
}

FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::rbegin() noexcept -> reverse_iterator
{
	return reverse_iterator(mImpl.end);
}
FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::rbegin() const noexcept -> const_reverse_iterator
{
	return const_reverse_iterator(mImpl.end);
}
FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::crbegin() const noexcept -> const_reverse_iterator
{
	return const_reverse_iterator(mImpl.end);
}
FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::rend() noexcept -> reverse_iterator
{
	return reverse_iterator(mImpl.beg);
}
FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::rend() const noexcept -> const_reverse_iterator
{
	return const_reverse_iterator(mImpl.beg);
}
FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::crend() const noexcept -> const_reverse_iterator
{
	return const_reverse_iterator(mImpl.beg);
}

FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::data() noexcept -> value_type*
{
	return mImpl.beg ? compat::to_address(mImpl.beg) : nullptr;
}
FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::data() const noexcept -> value_type const*
{
	return mImpl.beg ? compat::to_address(mImpl.beg) : nullptr;
}

FLUXTMP_VECTOR_TEMPL_ inline
void FLUXTMP_VECTOR_CLASS_::swap( VectorBase& aOther )
{
	if( (!in_place() || empty()) && (!aOther.in_place() || aOther.empty()) )
	{
		// just swap the held buffers
		std::swap( mImpl.beg, aOther.mImpl.beg );
		std::swap( mImpl.end, aOther.mImpl.end );
		std::swap( mImpl.capacity, aOther.mImpl.capacity );

		// and the allocator
		std::swap( static_cast<tAlloc&>(mImpl), static_cast<tAlloc&>(aOther.mImpl) );
		return;
	}

	// TODO- unhandled cases
	//   - both in_place but different sizes and insufficient capacity
	//   - probably: larger in_place but not smaller one

	auto* a = this;     // smaller vector
	auto* b = &aOther;  // larger vector

	if( a->size() > b->size() )
		std::swap( a, b );

	// sufficient capacity.
	if( a->capacity() >= b->size() )
	{
		auto bb = ftl::initialized_swap( a->mImpl.beg, a->mImpl.end, b->mImpl.beg );
		
		auto bt = bb;
		ftl::uninitialized_relocate_if_noexcept_p( b->mImpl, bb, b->mImpl.end, a->mImpl.end );
		b->mImpl.end = bt;

		std::swap( static_cast<tAlloc&>(mImpl), static_cast<tAlloc&>(aOther.mImpl) );
		return;
	}

	// is b out of place? if so, steal it
	if( !b->in_place() )
	{
		FLUX_ASSERT( a->in_place() );

		// Allocate space for A and transfer elements there
		using ATraits_ = std::allocator_traits<tAlloc>;

		auto const asize = a->size();
		auto ptr = ATraits_::allocate( a->mImpl, asize );
		if( FLUX_HINT_UNLIKELY(!ptr) )
			detail::vec_throw_bad_alloc();

		FLUX_UTIL_ON_SCOPE_EXIT
		{
			if( ptr ) ATraits_::deallocate( a->mImpl, ptr, asize );
		};
		
		auto end = ftl::uninitialized_relocate_if_noexcept( a->mImpl, a->mImpl.beg, a->mImpl.end, ptr );

		// Move over B to A
		a->mImpl.beg = b->mImpl.beg;
		a->mImpl.end = b->mImpl.end;
		a->mImpl.capacity = b->mImpl.capacity;

		// Initialize B
		b->mImpl.beg = ptr;
		b->mImpl.end = end;
		b->mImpl.capacity.set_flag( true );
		b->mImpl.capacity.set_pointer( ptr + asize );

		// Make sure ptr doesn't get nuked
		ptr = nullptr;
	}
	else
	{
		throw std::logic_error( "Not yet implemented" );
	}

	std::swap( static_cast<tAlloc&>(mImpl), static_cast<tAlloc&>(aOther.mImpl) );
}

FLUXTMP_VECTOR_TEMPL_ inline 
auto FLUXTMP_VECTOR_CLASS_::get_allocator() const -> allocator_type
{
	return static_cast<allocator_type const&>(mImpl);
}

	// internal -{{{3-
FLUXTMP_VECTOR_TEMPL_ inline
void FLUXTMP_VECTOR_CLASS_::fill_def_( size_type aCount, ValueInitTag )
{
	if( capacity() < aCount )
		reserve_( aCount );
	
	FLUX_ASSERT( mImpl.end == mImpl.beg );
	ftl::uninitialized_value_construct_pn( mImpl, mImpl.end, aCount );
}
FLUXTMP_VECTOR_TEMPL_ inline
void FLUXTMP_VECTOR_CLASS_::fill_def_( size_type aCount, DefaultInitTag )
{
	if( capacity() < aCount )
		reserve_( aCount );
	
	FLUX_ASSERT( mImpl.end == mImpl.beg );
	ftl::uninitialized_default_construct_pn( mImpl, mImpl.end, aCount );
}

FLUXTMP_VECTOR_TEMPL_ inline
void FLUXTMP_VECTOR_CLASS_::fill_val_( size_type aCount, value_type const& aVal )
{
	if( capacity() < aCount )
		reserve_( aCount );

	FLUX_ASSERT( mImpl.end == mImpl.beg );
	ftl::uninitialized_fill_pn( mImpl, mImpl.end, aCount, aVal );
}

FLUXTMP_VECTOR_TEMPL_ template< typename tIter > inline
void FLUXTMP_VECTOR_CLASS_::assign_copy_( tIter aBeg, tIter aEnd, std::input_iterator_tag )
{
	pointer beg = mImpl.beg;
	ftl::initialized_copy_p( aBeg, aEnd, beg, mImpl.end );

	if( aBeg == aEnd )
	{
		ftl::destroy( mImpl, beg, mImpl.end );
		mImpl.end = beg;
	}
	else
	{
		FLUX_ASSERT( beg == mImpl.end );
		for( ; aBeg != aEnd; ++aBeg )
			emplace_back( *aBeg );
	}
}
FLUXTMP_VECTOR_TEMPL_ template< typename tIter > inline
void FLUXTMP_VECTOR_CLASS_::assign_copy_( tIter aBeg, tIter aEnd, std::random_access_iterator_tag )
{
	size_type const req = std::distance( aBeg, aEnd );

	if( capacity() >= req )
	{
		//XXX
		//XXX
		//XXX  operator= below might throw, and mess up our exception guarantee
		//XXX  currently basic??
		//XXX
		//XXX
		pointer beg = mImpl.beg;
		ftl::initialized_copy_p( aBeg, aEnd, beg, mImpl.end );

		if( aBeg == aEnd )
		{
			ftl::destroy( mImpl, beg, mImpl.end );
			mImpl.end = beg;
		}
		else
		{
			FLUX_ASSERT( beg == mImpl.end );
			uninitialized_copy_p( mImpl, aBeg, aEnd, mImpl.end );
		}
	}
	else
	{
		size_type size = req;
		pointer buf = ATraits_::allocate( mImpl, size );
		if( FLUX_HINT_UNLIKELY(!buf) )
			detail::vec_throw_bad_alloc();

		pointer beg = buf, end = buf;

		FLUX_UTIL_ON_SCOPE_EXIT
		{
			ftl::destroy( mImpl, beg, end );
			if( buf ) ATraits_::deallocate( mImpl, buf, size );
		};
		
		uninitialized_copy_p( mImpl, aBeg, aEnd, end );

		if( !in_place() )
		{
			size = capacity();
			buf = mImpl.beg;
		}
		else
		{
			size = 0;
			buf = nullptr;
		}

		std::swap( mImpl.beg, beg );
		std::swap( mImpl.end, end );
		mImpl.capacity.set_flag( true );
		mImpl.capacity.set_pointer( mImpl.beg+req );
	}
}

FLUXTMP_VECTOR_TEMPL_ inline
void FLUXTMP_VECTOR_CLASS_::assign_move_other_( VectorBase&& aOther, std::true_type, bool aStealAllocator )
{
	/* first, clean up current elements and deallocate current buffer with the
	 * active allocator
	 */
	ftl::destroy( mImpl, mImpl.beg, mImpl.end );

	if( !in_place() )
	{
		ATraits_::deallocate( mImpl, mImpl.beg, capacity() );
		mImpl.beg  = nullptr;
		mImpl.capacity.set_pointer( nullptr );
	}

	mImpl.end = mImpl.beg;

	// move the allocator
	if( aStealAllocator )
		static_cast<tAlloc&>(mImpl) = std::move(static_cast<tAlloc&>(aOther.mImpl));

	// grab the buffer or move individual elements
	if( aOther.empty() )
		return;

	if( !aOther.in_place() )
	{
		mImpl.beg = aOther.mImpl.beg;
		mImpl.end = aOther.mImpl.end;
		mImpl.capacity = aOther.mImpl.capacity;

		aOther.mImpl.capacity.set_pointer( nullptr );
		aOther.mImpl.end = aOther.mImpl.beg = nullptr;
	}
	else
	{
		if( capacity() < aOther.size() )
			reserve_( aOther.size() );
		
		FLUX_ASSERT( mImpl.beg == mImpl.end );

		auto beg = aOther.mImpl.beg;
		ftl::uninitialized_relocate_if_noexcept_p( mImpl, beg, aOther.mImpl.end, mImpl.end );
		aOther.mImpl.end = aOther.mImpl.beg;
	}
}
FLUXTMP_VECTOR_TEMPL_ inline
void FLUXTMP_VECTOR_CLASS_::assign_move_other_( VectorBase&& aOther, std::false_type )
{
	if( static_cast<tAlloc&>(mImpl) == static_cast<tAlloc&>(aOther.mImpl) )
	{
		assign_move_other_( std::move(aOther), std::true_type{}, false );
		return;
	}

	// clean up current elements
	ftl::destroy( mImpl, mImpl.beg, mImpl.end );
	mImpl.end = mImpl.beg;

	// move over elements
	if( capacity() < aOther.size() )
		reserve_( aOther.size() );
	
	auto beg = aOther.mImpl.beg;
	ftl::uninitialized_relocate_if_noexcept_p( mImpl, beg, aOther.mImpl.end, mImpl.end );
	aOther.mImpl.end = aOther.mImpl.beg;
}

FLUXTMP_VECTOR_TEMPL_ inline
void FLUXTMP_VECTOR_CLASS_::grow_()
{
	size_type const newSize = vector_grow( capacity(), sizeof(value_type) );

	FLUX_ASSERT( newSize > capacity() );
	reserve_( newSize );
}
FLUXTMP_VECTOR_TEMPL_ inline
void FLUXTMP_VECTOR_CLASS_::reserve_( size_type aCount )
{
	pointer old = nullptr;
	size_type os = 0;

	if( !in_place() )
	{
		old = mImpl.beg;
		os = capacity();
	}
	
	pointer end;
	pointer buf = VTraits_::realloc( mImpl, aCount, old, os, mImpl.beg, mImpl.end, end );

	FLUX_ASSERT( buf );

	mImpl.beg = buf;
	mImpl.end = end;
	mImpl.capacity.set_flag( true );
	mImpl.capacity.set_pointer( buf + aCount );
}

FLUXTMP_VECTOR_TEMPL_ template< typename... tArgs > inline
auto FLUXTMP_VECTOR_CLASS_::emplace_back_( std::true_type, tArgs&&... aArgs ) -> reference
{
	if( FLUX_HINT_LIKELY(mImpl.end != mImpl.capacity.pointer()) )
	{
		ATraits_::construct( mImpl, compat::to_address(mImpl.end), std::forward<tArgs>(aArgs)... );
		return *mImpl.end++;
	}

	size_type const grown = vector_grow( capacity(), sizeof(tType) );

	pointer ob = nullptr;
	size_type os = 0;

	if( !in_place() )
	{
		ob = mImpl.beg;
		os = capacity();
	}

	value_type value( std::forward<tArgs>(aArgs)... );

	pointer end;
	pointer buf = VTraits_::realloc( mImpl, grown, ob, os, mImpl.beg, mImpl.end, end );

	mImpl.beg = buf;
	mImpl.end = end;
	mImpl.capacity.set_flag( true );
	mImpl.capacity.set_pointer( buf + grown );

	ATraits_::construct( mImpl, compat::to_address(mImpl.end), std::move(value) );
	return *mImpl.end++;
}
FLUXTMP_VECTOR_TEMPL_ template< typename... tArgs > inline
auto FLUXTMP_VECTOR_CLASS_::emplace_back_( std::false_type, tArgs&&... aArgs ) -> reference
{
	if( FLUX_HINT_LIKELY(mImpl.end != mImpl.capacity.pointer()) )
	{
		ATraits_::construct( mImpl, compat::to_address(mImpl.end), std::forward<tArgs>(aArgs)... );
		return *mImpl.end++;
	}

	size_type const grown = vector_grow( capacity(), sizeof(tType) );
	pointer buf = ATraits_::allocate( mImpl, grown );
	if( FLUX_HINT_UNLIKELY(!buf) )
		detail::vec_throw_bad_alloc();
	
	pointer beg = buf, end = buf;
	size_type size = grown;

	FLUX_UTIL_ON_SCOPE_EXIT
	{
		if( buf ) ATraits_::deallocate( mImpl, buf, size );
	};

	value_type value( std::forward<tArgs>(aArgs)... );

	auto bt = mImpl.beg;
	ftl::uninitialized_relocate_if_noexcept_p( mImpl, bt, mImpl.end, end );

	if( !in_place() )
	{
		size = capacity();
		buf = mImpl.beg;
	}
	else
	{
		size = 0;
		buf = nullptr;
	}

	mImpl.beg = beg;
	mImpl.end = end;
	mImpl.capacity.set_flag( true );
	mImpl.capacity.set_pointer( mImpl.beg + grown );

	ATraits_::construct( mImpl, compat::to_address(mImpl.end), std::move(value) );
	return *mImpl.end++;

#	if 0
	// This is clever, and avoids an extra move, but would require an
	// additiona `uninitialized_relocate_if_noexcept_rev_p()` method.
	FLUX_UTIL_ON_SCOPE_EXIT
	{
		ftl::destroy( mImpl, beg, end );
		if( buf ) ATraits_::deallocate( mImpl, buf, size );
	};
	
	pointer target = buf + (mImpl.end - mImpl.beg);
	ATraits_::construct( mImpl, compat::to_address(target), std::forward<tArgs>(aArgs)... );

	beg = target;
	end = target+1;

	VTraits_::move_ctor_rev( mImpl, mImpl.beg, mImpl.end, beg, beg );
	FLUX_ASSERT( beg == buf );

	if( !in_place() )
	{
		size = capacity();
		buf = mImpl.beg;
	}
	else
	{
		size = 0;
		buf = nullptr;
	}

	std::swap( beg, mImpl.beg );
	std::swap( end, mImpl.end );
	mImpl.capacity.set_flag( true );
	mImpl.capacity.set_pointer( mImpl.beg+grown );

	return *target;
#	endif
}

FLUXTMP_VECTOR_TEMPL_ inline
auto FLUXTMP_VECTOR_CLASS_::insert_back_( size_type aCount, value_type const& aVal ) -> iterator
{
	size_type const req = size() + aCount;
	if( capacity() < req )
		reserve_( req );

	pointer ret = mImpl.end;
	ftl::uninitialized_fill_pn( mImpl, mImpl.end, aCount, aVal );
	return ret;
}


FLUXTMP_VECTOR_TEMPL_ template< typename tIter >
auto FLUXTMP_VECTOR_CLASS_::insert_( const_iterator aIt, tIter aBeg, tIter aEnd, std::input_iterator_tag ) -> iterator
{
	pointer it = pointer(const_cast<iterator>(aIt));

	FLUX_ASSERT( std::less_equal<pointer>{}( mImpl.beg, it ) );
	FLUX_ASSERT( std::less_equal<pointer>{}( it, mImpl.end ) );

	size_type const offs = it - mImpl.beg;
	if( it == mImpl.end )
	{
		for( ; aBeg != aEnd; ++aBeg )
			emplace_back( *aBeg );
	}
	else
	{
		for( iterator jt = iterator(it); aBeg != aEnd; ++aBeg, ++jt )
			jt = emplace( jt, *aBeg );
	}

	return iterator(mImpl.beg+offs);
}
FLUXTMP_VECTOR_TEMPL_ template< typename tIter >
auto FLUXTMP_VECTOR_CLASS_::insert_( const_iterator aIt, tIter aBeg, tIter aEnd, std::random_access_iterator_tag ) -> iterator
{
	pointer it = pointer(const_cast<iterator>(aIt));

	FLUX_ASSERT( std::less_equal<pointer>{}( mImpl.beg, it ) );
	FLUX_ASSERT( std::less_equal<pointer>{}( it, mImpl.end ) );

	if( it == mImpl.end )
	{
		size_type const offs = it - mImpl.beg;

		size_type const count = std::distance( aBeg, aEnd );
		size_type const req = size() + count;

		if( capacity() < req )
			reserve_( req );

		ftl::uninitialized_copy_p( mImpl, aBeg, aEnd, mImpl.end );
	
		return iterator(mImpl.beg+offs);
	}

	//TODO
	//TODO
	//TODO
	//TODO if space: shift once + insert; else single alloc with move-copy-move
	//TODO
	//TODO
	//TODO
	return insert_( aIt, aBeg, aEnd, std::input_iterator_tag{} );
}

#undef FLUXTMP_VECTOR_TEMPL_
#undef FLUXTMP_VECTOR_CLASS_

//--    d: VecInlineStore                   ///{{{2///////////////////////////
namespace detail
{
	template< bool tFlag, std::size_t tSize, std::size_t tAlign > inline constexpr
	std::nullptr_t VecInlineStore<tFlag,tSize,tAlign>::store_beg_( VecInlineStore* ) noexcept
	{
		return nullptr;
	}
	template< bool tFlag, std::size_t tSize, std::size_t tAlign > inline constexpr
	std::nullptr_t VecInlineStore<tFlag,tSize,tAlign>::store_end_( VecInlineStore* ) noexcept
	{
		return nullptr;
	}


	template< std::size_t tSize, std::size_t tAlign > inline constexpr
	void* VecInlineStore<true,tSize,tAlign>::store_beg_( VecInlineStore* aStore ) noexcept
	{
		return &aStore->mStorage;
	}
	template< std::size_t tSize, std::size_t tAlign > inline constexpr
	void* VecInlineStore<true,tSize,tAlign>::store_end_( VecInlineStore* aStore ) noexcept
	{
		return 1+&aStore->mStorage;
	}
}

//--    Vector<>                            ///{{{2///////////////////////////
#define FLUXTMP_VECTOR_CLASS_ Vector<tType,tBytes,tAlloc>
#define FLUXTMP_VECTOR_TEMPL_ template< typename tType, std::size_t tBytes, class tAlloc > 

	// [cd]tor -{{{3-
FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_::Vector()
	: Base_( Init_{}, this->store_beg_(this), this->store_end_(this) )
{}

FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_::Vector( size_type aSize, value_type const& aValue, tAlloc const& aAlloc )
	: Base_( Init_{}, this->store_beg_(this), this->store_end_(this), aSize, aValue, aAlloc )
{}

FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_::Vector( tAlloc const& aAlloc )
	: Base_( Init_{}, this->store_beg_(this), this->store_end_(this), 0, aAlloc )
{}
FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_::Vector( size_type aSize, tAlloc const& aAlloc )
	: Base_( Init_{}, this->store_beg_(this), this->store_end_(this), aSize, aAlloc )
{}
FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_::Vector( size_type aSize, DefaultInitTag, tAlloc const& aAlloc )
	: Base_( Init_{}, this->store_beg_(this), this->store_end_(this), aSize, DefaultInitTag{}, aAlloc )
{}


FLUXTMP_VECTOR_TEMPL_ template< typename tIter, class tCat > inline
FLUXTMP_VECTOR_CLASS_::Vector( tIter aBeg, tIter aEnd, tAlloc const& aAlloc )
	: Base_( Init_{}, this->store_beg_(this), this->store_end_(this), aBeg, aEnd, aAlloc )
{}


FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_::Vector( Base_&& aOther, tAlloc const& aAlloc )
	: Base_( Init_{}, this->store_beg_(this), this->store_end_(this), 0, aAlloc )
{
	Base_::operator=( std::move(aOther) );
}
FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_::Vector( Base_ const& aOther, tAlloc const& aAlloc )
	: Base_( Init_{}, this->store_beg_(this), this->store_end_(this), 0, aAlloc )
{
	Base_::operator=( aOther );
}

FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_::Vector( std::initializer_list<value_type> aList, tAlloc const& aAlloc )
	: Base_( Init_{}, this->store_beg_(this), this->store_end_(this), aList.begin(), aList.end(), aAlloc )
{}

	// copy/move -{{{3-
FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_::Vector( Vector const& aOther )
	: Base_( Init_{}, this->store_beg_(this), this->store_end_(this) )
{
	Base_::operator=( aOther );
}
FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_& FLUXTMP_VECTOR_CLASS_::operator= (Vector const& aOther)
{
	Base_::operator=( aOther );
	return *this;
}


FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_::Vector( Vector&& aOther ) noexcept(isNothrowMovable)
	: Base_( Init_{}, this->store_beg_(this), this->store_end_(this) )
{
	//TODO: provide a ctor that does this:
	static_cast<tAlloc&>(this->mImpl) = std::move(static_cast<tAlloc&>(aOther.mImpl));

	if( aOther.empty() )
		return;

	if( !aOther.in_place() )
	{
		this->mImpl.beg = aOther.mImpl.beg;
		this->mImpl.end = aOther.mImpl.end;
		this->mImpl.capacity = aOther.mImpl.capacity;

		aOther.mImpl.beg = aOther.mImpl.end = nullptr;
		aOther.mImpl.capacity.set_pointer( nullptr );
	}
	else
	{
		FLUX_ASSERT( this->capacity() >= aOther.size() );

		auto beg = aOther.mImpl.beg;
		ftl::uninitialized_relocate_if_noexcept_p( this->mImpl, beg, aOther.mImpl.end, this->mImpl.end );
		aOther.mImpl.end = aOther.mImpl.beg;
	}

}
FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_& FLUXTMP_VECTOR_CLASS_::operator= (Vector&& aOther) noexcept(isNothrowMovable)
{
	if( !this->in_place() && !aOther.in_place() )
	{
		std::swap( this->mImpl.beg, aOther.mImpl.beg );
		std::swap( this->mImpl.end, aOther.mImpl.end );
		std::swap( this->mImpl.capacity, aOther.mImpl.capacity );
	}
	else if( this->in_place() && aOther.in_place() )
	{
		Vector* u = this;
		Vector* v = &aOther;

		if( u->size() > v->size() ) std::swap( u, v );

		auto* split = v->mImpl.beg + u->size();
		FLUX_ASSERT( split <= v->mImpl.end );
		
		initialized_swap( v->mImpl.beg, split, u->mImpl.beg );
		auto* temp = split;
		uninitialized_relocate_if_noexcept_p( v->mImpl, temp, v->mImpl.end, u->mImpl.end );
		v->mImpl.end = split;
	}
	else
	{
		Vector* u = this; // out-of-place
		Vector* v = &aOther; // in-place

		if( !v->in_place() ) std::swap( u, v );
		FLUX_ASSERT( !u->in_place() && v->in_place() );

		auto* uipb = static_cast<tType*>(u->store_beg_(u));
		FLUX_ASSERT( uipb + v->size() <= static_cast<tType*>(u->store_end_(u)) );

		auto* uipe = uninitialized_relocate_if_noexcept( v->mImpl, v->mImpl.beg, v->mImpl.end, uipb );

		v->mImpl.beg = u->mImpl.beg;
		v->mImpl.end = u->mImpl.end;
		v->mImpl.capacity = u->mImpl.capacity;

		u->mImpl.beg = uipb;
		u->mImpl.end = uipe;
		u->mImpl.capacity.set_flag( false );
		u->mImpl.capacity.set_pointer( static_cast<tType*>(u->store_end_(u)) );
	}

	std::swap( static_cast<tAlloc&>(this->mImpl), static_cast<tAlloc&>(aOther.mImpl) );
	return *this;
}

FLUXTMP_VECTOR_TEMPL_ inline
FLUXTMP_VECTOR_CLASS_& FLUXTMP_VECTOR_CLASS_::operator= (std::initializer_list<value_type> aList )
{
	Base_::operator=( aList );
	return *this;
}

#undef FLUXTMP_VECTOR_TEMPL_
#undef FLUXTMP_VECTOR_CLASS_

//--    vector_grow()                       ///{{{2///////////////////////////
inline std::size_t vector_grow( std::size_t aCap, std::size_t aElemSize )
{
	if( 0 == aCap )
		return (64+aElemSize-1)/aElemSize;

	return aCap + (aCap+1)/2;

#	if 0
	if( aCap < 4 || aCap * aElemSize < 64 )
		return std::max<std::size_t>( 4, (64+aElemSize-1)/aElemSize );

	return aCap + (aCap+1)/2;
#	endif
}

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
