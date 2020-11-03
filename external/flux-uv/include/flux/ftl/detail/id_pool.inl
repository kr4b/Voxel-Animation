/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- IdPool implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/std/assert.hpp>

#include <flux/util/hint.hpp>
#include <flux/util/intent.hpp>

#include <cstdlib> // free(), realloc()
#include <cstring> // memmove()

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    $ local                             ///{{{2///////////////////////////
namespace detail
{
	template< bool tSigned >
	struct IdPoolIsNegative_
	{
		template< typename tInt >
		static inline bool test( tInt aInt ) {
			return aInt < 0;
		}
	};
	template<> struct IdPoolIsNegative_<false> {
		template< typename tInt >
		static inline bool test( tInt ) {
			return false;
		}
	};

	template< typename tInt >
	inline bool id_pool_is_negative_( tInt aInt )
	{
		return IdPoolIsNegative_< std::is_signed<tInt>::value >::test(aInt);
	}
};

//--    IdPool                              ///{{{2///////////////////////////

	// [cd]tor -{{{3-
template< typename tId > inline
IdPool<tId>::IdPool( IntType aPoolSize )
	: mRanges(nullptr)
	, mRangeCount(1)
	, mRangeCap(1)
	, mLast(aPoolSize-1)
{
	FLUX_ASSERT( !detail::id_pool_is_negative_( aPoolSize ) );
	
	mRanges = static_cast<Range_*>(std::realloc( nullptr, sizeof(Range_)*mRangeCap ));
	mRanges[0].first = 0;
	mRanges[0].last = mLast;
}

template< typename tId > inline
IdPool<tId>::~IdPool()
{
	std::free( mRanges );
}

	// public -{{{3-
template< typename tId > inline
auto IdPool<tId>::count_available() const noexcept -> IntType
{
	// clever: ranges are inclusive, so there's one extra item per range; this
	// offsets the straight subtraction below.
	IntType count = IntType(mRangeCount);

	for( std::size_t i = 0; i < mRangeCount; ++i )
	{
		count += mRanges[i].last - mRanges[i].first;
	}

	return count;
}
template< typename tId > inline
auto IdPool<tId>::count_allocated() const noexcept -> IntType
{
	IntType count = 0, seen = 0;

	for( std::size_t i = 0; i < mRangeCount; ++i )
	{
		count += mRanges[i].first - seen;
		seen = mRanges[i].last+1;
	}

	return count;
}

template< typename tId > inline
bool IdPool<tId>::is_id( tId aId ) const noexcept
{
	auto const id = static_cast<IntType>(aId);
	if( id > mLast || detail::id_pool_is_negative_(id) )
		return false;

	// Binary search
	for( IntType bot = 0, top = IntType(mRangeCount-1) ;; )
	{
		IntType mid = (bot+top)/2;

		if( id < mRanges[mid].first )
		{
			if( mid == bot ) return true;
			top = mid - 1;
		}
		else if( id > mRanges[mid].last )
		{
			if( mid == top ) return true;
			bot = mid + 1;
		}
		else
		{
			// inside a free block, not an allocated ID
			return false;
		}
	}

	FLUX_INTENT_UNREACHABLE();
}

template< typename tId > inline
auto IdPool<tId>::alloc() -> Id
{
	Id ret;

	if( FLUX_HINT_UNLIKELY(!try_alloc( ret )) )
		detail::throw_id_pool_exhausted();

	return ret;
}
template< typename tId > inline
auto IdPool<tId>::alloc_range( IntType aCount ) -> Id
{
	Id ret;

	if( FLUX_HINT_UNLIKELY(!try_alloc_range( ret, aCount )) )
		detail::throw_id_pool_exhausted();

	return ret;
}

template< typename tId > inline
bool IdPool<tId>::try_alloc( Id& aRet ) noexcept
{
	if( mRanges[0].first <= mRanges[0].last )
	{
		aRet = static_cast<Id>(mRanges[0].first);

		if( mRanges[0].first == mRanges[0].last && mRangeCount > 1 )
		{
			delete_range_(0);
		}
		else
		{
			++mRanges[0].first;
		}
		
		return true;
	}

	return false;
}
template< typename tId > inline
bool IdPool<tId>::try_alloc_range( Id& aRet, IntType aCount ) noexcept
{
	FLUX_ASSERT( aCount > 0 );

	for( std::size_t i = 0; i < mRangeCount; ++i )
	{
		IntType const inRange = mRanges[i].last - mRanges[i].first /*+1, but overflows*/;
		if( aCount-1 /*compensate missing +1 above*/ <= inRange )
		{
			aRet = static_cast<Id>(mRanges[i].first);

			if( inRange == aCount-1 /*comp. missing +1 above*/ && i+1 < mRangeCount )
			{
				delete_range_(0);
			}
			else
			{
				mRanges[i].first += aCount;
			}

			return true;
		}
	}

	return false;
}

template< typename tId > inline
void IdPool<tId>::release( Id aId )
{
	release_range( aId, 1 );
}
template< typename tId > inline
void IdPool<tId>::release_range( Id aId, IntType aCount )
{
	FLUX_ASSERT( !detail::id_pool_is_negative_(aCount)  );
	
	auto const id = static_cast<IntType>(aId);

	using detail::id_pool_is_negative_;
	if( FLUX_HINT_UNLIKELY(id > mLast - aCount) || id_pool_is_negative_(id) )
		detail::throw_id_pool_not_allocated( id, std::size_t(aCount) );
	
	auto const endId = id + aCount;

	for( std::size_t bot = 0, top = mRangeCount-1 ;; )
	{
		std::size_t mid = (bot+top) / 2;

		if( id < mRanges[mid].first )
		{
			// overlaps neighbouring range?
			if( endId >= mRanges[mid].first )
			{
				if( FLUX_HINT_UNLIKELY(endId != mRanges[mid].first) )
					detail::throw_id_pool_not_allocated( id, std::size_t(aCount) );
					
				// check if neighbouring prev. range too
				if( mid > bot && id - 1 == mRanges[mid-1].last )
				{
					// merge ranges
					mRanges[mid-1].last = mRanges[mid].last;
					delete_range_(mid);
				}
				else
				{
					// grow range
					mRanges[mid].first = id;
				}
				return;
			}
			else
			{
				if( mid != bot )
				{
					top = mid-1;
				}
				else
				{
					insert_range_(mid);
					mRanges[mid].first = id;
					mRanges[mid].last = endId-1;
					return;
				}
			}
		}
		else if( id > mRanges[mid].last )
		{
			// overlaps neighbouring range?
			if( id-1 == mRanges[mid].last )
			{
				// overlaps next range too?
				if( mid < top && endId == mRanges[mid+1].first )
				{
					// merge ranges
					mRanges[mid].last = mRanges[mid+1].last;
					delete_range_(mid+1);
				}
				else
				{
					// grow range
					mRanges[mid].last += aCount;
				}
				return;
			}
			else
			{
				if( mid != top )
				{
					bot = mid+1;
				}
				else
				{
					// insert deleted range into list
					insert_range_(mid+1);
					mRanges[mid+1].first = id;
					mRanges[mid+1].last = endId-1;
				}
			}
		}
		else
		{
			// found range that contains the to-be-deleted range, which
			// therefore already is free.
			detail::throw_id_pool_not_allocated( id, std::size_t(aCount) );
		}
	}

	FLUX_INTENT_UNREACHABLE();
}

	// private -{{{3-
template< typename tId > inline
void IdPool<tId>::insert_range_( std::size_t aRangeIdx )
{
	FLUX_ASSERT( aRangeIdx < mRangeCount );
	FLUX_ASSERT( mRangeCount <= mRangeCap );

	if( mRangeCount >= mRangeCap )
	{
		// growth multiplier : 2x
		std::size_t const newCap = mRangeCap*2;
		if( FLUX_HINT_UNLIKELY(newCap*sizeof(Range_) > limits::kMaxIdAllocBytes) )
			detail::throw_id_pool_mem_limit();
			
		Range_* ranges = static_cast<Range_*>(std::realloc( mRanges, sizeof(Range_)*newCap ));

		if( FLUX_HINT_UNLIKELY(!ranges) )
			detail::throw_id_pool_mem_limit();

		mRanges = ranges;
		mRangeCap = newCap;
	}

	std::memmove( mRanges+aRangeIdx+1, mRanges+aRangeIdx, (mRangeCount-aRangeIdx) * sizeof(Range_) );
	++mRangeCount;
}
template< typename tId > inline
void IdPool<tId>::delete_range_( std::size_t aRangeIdx ) noexcept
{
	FLUX_ASSERT( aRangeIdx < mRangeCount );
	FLUX_ASSERT( mRangeCount <= mRangeCap );

	std::memmove( mRanges+aRangeIdx, mRanges+aRangeIdx+1, (mRangeCount-aRangeIdx-1) * sizeof(Range_) );
	--mRangeCount;
}

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
