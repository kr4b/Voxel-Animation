/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- initialialized_*() implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/std/assert.hpp>

#include <flux/compat/constexpr.hpp>
#include <flux/compat/type_traits.hpp>

#include <flux/ftl/meta_bool.hpp>
#include <flux/ftl/is_bitwise.hpp>
#include <flux/ftl/uninitialized.hpp> // for destroy() & co
#include <flux/ftl/contiguous_iterator.hpp>

#include <flux/util/diagnostics.hpp>

#include <memory>
#include <utility>
#include <iterator>
#include <type_traits>

#include <cstring>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    d: helpers                          ///{{{2///////////////////////////
FLUX_DIAGNOSTIC_STATE_PUSH_AND_SET(IGNORED,CLASS_MEMACCESS)

namespace detail
{
	// optimized version predicates {{{3
	template< typename tInputIt, typename tForwardIt >
	struct ICopyCanMemcpy_
	{
		using In_ = typename std::iterator_traits<tInputIt>::value_type;
		using Of_ = typename std::iterator_traits<tForwardIt>::value_type;

		static constexpr bool value = ftl::meta::All<
			ftl::is_contiguous_iterator<tInputIt>,
			ftl::is_contiguous_iterator<tForwardIt>,
			std::is_same<In_, Of_>,
			ftl::is_bitwise_duplicable<In_>,
			ftl::is_bitwise_dismissible<In_>
		>::value;
	};
	template< typename tInputIt, typename tForwardIt >
	struct IMoveCanMemcpy_
	{
		using In_ = typename std::iterator_traits<tInputIt>::value_type;
		using Of_ = typename std::iterator_traits<tForwardIt>::value_type;

		static constexpr bool value = ftl::meta::All<
			ftl::is_contiguous_iterator<tInputIt>,
			ftl::is_contiguous_iterator<tForwardIt>,
			std::is_same<In_, Of_>,
			ftl::is_bitwise_duplicable<In_>,
			ftl::is_bitwise_dismissible<In_>
		>::value;
	};
	template< typename tInputIt, typename tForwardIt >
	struct IRelocCanMemcpy_
	{
		using In_ = typename std::iterator_traits<tInputIt>::value_type;
		using Of_ = typename std::iterator_traits<tForwardIt>::value_type;

		static constexpr bool value = ftl::meta::All<
			ftl::is_contiguous_iterator<tInputIt>,
			ftl::is_contiguous_iterator<tForwardIt>,
			std::is_same<In_, Of_>,
			ftl::is_bitwise_relocatable<In_>,
			ftl::is_bitwise_dismissible<In_>
		>::value;
	};
	template< typename tInputA, typename tInputB >
	struct ISwapCanMemcpy_
	{
		using InA_ = typename std::iterator_traits<tInputA>::value_type;
		using InB_ = typename std::iterator_traits<tInputB>::value_type;

		static constexpr bool value = ftl::meta::All<
			ftl::is_contiguous_iterator<tInputA>,
			ftl::is_contiguous_iterator<tInputB>,
			std::is_same<InA_, InB_>,
			ftl::is_bitwise_relocatable<InA_>
		>::value;
	};

	// noexcept predicates {{{3
	template< typename tForwardIt, typename tType >
	struct IsNothrowInCtor
	{
		using Of_ = typename std::iterator_traits<tForwardIt>::value_type;
		
		static constexpr bool value = ftl::meta::Any<
			std::is_nothrow_assignable<Of_,tType>
		>::value;
	};
	
	template< typename tInputIt, typename tForwardIt >
	struct IsNothrowInCopy
	{
		using In_ = typename std::iterator_traits<tInputIt>::value_type;
		using Of_ = typename std::iterator_traits<tForwardIt>::value_type;
		
		static constexpr bool value = ftl::meta::Any<
			std::is_nothrow_assignable<Of_,In_ const&>,
			ICopyCanMemcpy_<tInputIt,tForwardIt>
		>::value;
	};
	template< typename tInputIt, typename tForwardIt >
	struct IsNothrowInMove
	{
		using In_ = typename std::iterator_traits<tInputIt>::value_type;
		using Of_ = typename std::iterator_traits<tForwardIt>::value_type;
		
		static constexpr bool value = ftl::meta::Any<
			std::is_nothrow_assignable<Of_,In_&&>,
			IMoveCanMemcpy_<tInputIt,tForwardIt>
		>::value;
	};
	template< typename tInputIt, typename tForwardIt >
	struct IsNothrowInReloc
	{
		using In_ = typename std::iterator_traits<tInputIt>::value_type;
		using Of_ = typename std::iterator_traits<tForwardIt>::value_type;
		
		static constexpr bool value = ftl::meta::Any<
			std::is_nothrow_assignable<Of_,In_&&>,
			IRelocCanMemcpy_<tInputIt,tForwardIt>
		>::value;
	};
	template< typename tInputA, typename tInputB >
	struct IsNothrowInSwap
	{
		using InA_ = typename std::iterator_traits<tInputA>::value_type;
		using InB_ = typename std::iterator_traits<tInputB>::value_type;
		
		static constexpr bool value = ftl::meta::Any<
			ftl::meta::All<
				std::is_nothrow_constructible<InB_,InA_&&>, //TODO:: is_nothrow_swap<>??
				std::is_nothrow_assignable<InB_,InA_&&>,
				std::is_nothrow_assignable<InA_,InB_&&>
			>,
			IRelocCanMemcpy_<tInputA,tInputB>
		>::value;
	};

	template< typename tInputA, typename tInputB >
	struct InitSwapElements
	{
		using InA_ = typename std::iterator_traits<tInputA>::value_type;
		using InB_ = typename std::iterator_traits<tInputB>::value_type;

		static constexpr std::size_t size_ = sizeof(InA_) > sizeof(InB_) ? sizeof(InA_) : sizeof(InB_);
		static constexpr std::size_t value_ = aspect::kInitializedSwapBufferBytes / size_;
		static constexpr std::size_t value = value_ >= 1 ? value_ : 1;

		static inline constexpr StaticSize<value> def() { return {}; }
	};
}

//--    $ local                             ///{{{2///////////////////////////
namespace detail
{
	// Opportunistic debug checks {{{3
#	if FLUX_DEBUG
#		define FLUXTMP_DEBUG_CHECK_ORDER_(a,b) detail::debug_check_in_iter_order(a,b)

	template< typename tIterator > inline FLUX_CONSTEXPR_EX
	void debug_check_in_iter_order( tIterator aBeg, tIterator aEnd, std::random_access_iterator_tag )
	{
		FLUX_ASSERT( aBeg <= aEnd );
		(void)aBeg;
		(void)aEnd;
	}
	template< typename tIterator, class tCat > inline FLUX_CONSTEXPR_EX
	void debug_check_in_iter_order( tIterator, tIterator, tCat )
	{}

	template< typename tIterator > inline FLUX_CONSTEXPR_EX
	void debug_check_in_iter_order( tIterator aBeg, tIterator aEnd )
	{
		using Cat_ = typename std::iterator_traits<tIterator>::iterator_category;
		debug_check_in_iter_order( aBeg, aEnd, Cat_{} );
	}
#	else // !FLUX_DEBUG
#		define FLUXTMP_DEBUG_CHECK_ORDER_(a, b) /*EMPTY*/
#	endif // ~ FLUX_DEBUG
	
	// Helper: Initialized Copy {{{3
	template< bool >
	struct InitializedCopy_
	{
		template< typename tInputIt, typename tForwardIt > static inline
		tForwardIt enact( tInputIt aBeg, tInputIt aEnd, tForwardIt aOut )
		{
			return std::copy( aBeg, aEnd, aOut );
		}
		template< typename tInputIt, typename tForwardIt > static inline
		std::pair<tInputIt,tForwardIt> enact_n( tInputIt aBeg, std::size_t aCount, tForwardIt aOut )
		{
			for( std::size_t i = 0; i < aCount; ++i, ++aOut, ++aBeg )
					*aOut = *aBeg;

			return std::make_pair( aBeg, aOut );
		}

		template< typename tInputIt, typename tForwardIt > static inline
		void enact_p( tInputIt& aBeg, tInputIt aEnd, tForwardIt& aOut ) noexcept((detail::IsNothrowInCopy<tInputIt,tForwardIt>::value))
		{
			for( ; aBeg != aEnd; ++aBeg, ++aOut )
				*aOut = *aBeg;
		}
		template< typename tInputIt, typename tForwardIt > static inline
		void enact_pn( tInputIt& aBeg, std::size_t aCount, tForwardIt& aOut ) noexcept((detail::IsNothrowInCopy<tInputIt,tForwardIt>::value))
		{
			for( std::size_t i = 0; i < aCount; ++i, ++aBeg, ++aOut )
				*aOut = *aBeg;
		}
		template< typename tInputIt, typename tForwardIt > static inline
		void enact_p( tInputIt& aBeg, tInputIt aEnd, tForwardIt& aOut, tForwardIt aOutEnd ) noexcept((detail::IsNothrowInCopy<tInputIt,tForwardIt>::value))
		{
			for( ; aBeg != aEnd && aOut != aOutEnd; ++aBeg, ++aOut )
				*aOut = *aBeg;
		}

		template< typename tBidiItA, typename tBidiItB > static inline
		tBidiItB enact_r( tBidiItA aBeg, tBidiItA aEnd, tBidiItB aOutEnd )
		{
			return std::copy_backward( aBeg, aEnd, aOutEnd );
		}
		template< typename tBidiItA, typename tBidiItB > static inline
		std::pair<tBidiItA,tBidiItB> enact_r_n( tBidiItA aEnd, std::size_t aCount, tBidiItB aOutEnd )
		{
			for( std::size_t i = 0; i < aCount; ++i )
					*(--aOutEnd) = *(--aEnd);

			return std::make_pair( aEnd, aOutEnd );
		}

		template< typename tBidiItA, typename tBidiItB > static inline
		void enact_r_p( tBidiItA aBeg, tBidiItA& aEnd, tBidiItB& aOutEnd ) noexcept((detail::IsNothrowInCopy<tBidiItA,tBidiItB>::value))
		{
			while( aBeg != aEnd )
				*(--aOutEnd) = *(--aEnd);
		}
		template< typename tBidiItA, typename tBidiItB > static inline
		void enact_r_pn( tBidiItA& aEnd, std::size_t aCount, tBidiItB& aOutEnd ) noexcept((detail::IsNothrowInCopy<tBidiItA,tBidiItB>::value))
		{
			for( std::size_t i = 0; i < aCount; ++i )
				*(--aOutEnd) = *(--aEnd);
		}
	};
	template<>
	struct InitializedCopy_<true> 
	{
		template< typename tInputIt, typename tForwardIt > static inline
		tForwardIt enact( tInputIt aBeg, tInputIt aEnd, tForwardIt aOut ) noexcept
		{
			if( aBeg != aEnd )
			{
				auto beg = to_pointer(aBeg);
				auto end = to_pointer(aEnd);
				auto out = to_pointer(aOut);
				std::memmove( out, beg, (end-beg)*sizeof(*beg) );
				return aOut + (end-beg);
			}

			return aOut;
		}
		template< typename tInputIt, typename tForwardIt > static inline
		std::pair<tInputIt,tForwardIt> enact_n( tInputIt aBeg, std::size_t aCount, tForwardIt aOut )
		{
			if( aCount > 0 )
			{
				auto beg = to_pointer(aBeg);
				auto out = to_pointer(aOut);
				std::memmove( out, beg, aCount*sizeof(*beg) );
			}
			return std::make_pair(aBeg + aCount, aOut + aCount);
		}

		template< typename tInputIt, typename tForwardIt > static inline
		void enact_p( tInputIt& aBeg, tInputIt aEnd, tForwardIt& aOut ) noexcept
		{
			if( aBeg != aEnd )
			{
				auto beg = to_pointer(aBeg);
				auto end = to_pointer(aEnd);
				auto out = to_pointer(aOut);
			
				auto const count = end-beg;
				std::memmove( out, beg, count*sizeof(*beg) );

				aBeg = aEnd;
				aOut += count;
			}
		}
		template< typename tInputIt, typename tForwardIt > static inline
		void enact_pn( tInputIt& aBeg, std::size_t aCount, tForwardIt& aOut ) noexcept
		{
			if( aCount > 0 )
			{
				auto beg = to_pointer(aBeg);
				auto out = to_pointer(aOut);
				std::memmove( out, beg, aCount*sizeof(*beg) );

				aBeg += aCount;
				aOut += aCount;
			}
		}
		template< typename tInputIt, typename tForwardIt > static inline
		void enact_p( tInputIt& aBeg, tInputIt aEnd, tForwardIt& aOut, tForwardIt aOutEnd ) noexcept
		{
			if( aBeg != aEnd )
			{
				auto beg = to_pointer(aBeg);
				auto end = to_pointer(aEnd);
				auto out = to_pointer(aOut);
				auto ond = to_pointer(aOutEnd);
			
				auto const count = std::min( end-beg, ond-out );
				std::memmove( out, beg, count*sizeof(*beg) );

				aBeg += count;
				aOut += count;
			}
		}


		template< typename tBidiItA, typename tBidiItB > static inline
		tBidiItB enact_r( tBidiItA aBeg, tBidiItA aEnd, tBidiItB aOutEnd ) noexcept
		{
			if( aBeg != aEnd )
			{
				auto beg = to_pointer(aBeg);
				auto end = to_pointer(aEnd);
				auto ond = to_pointer(aOutEnd);

				auto const count = end-beg;
				std::memmove( ond-count, beg, count*sizeof(*beg) );
				return aOutEnd - count;
			}

			return aOutEnd;
		}
		template< typename tBidiItA, typename tBidiItB > static inline
		std::pair<tBidiItA,tBidiItB> enact_r_n( tBidiItA aEnd, std::size_t aCount, tBidiItB aOutEnd )
		{
			if( aCount > 0 )
			{
				auto end = to_pointer(aEnd);
				auto ond = to_pointer(aOutEnd);
				std::memmove( ond-aCount, end-aCount, aCount*sizeof(*end) );
			}
			return std::make_pair(aEnd - aCount, aOutEnd - aCount);
		}

		template< typename tBidiItA, typename tBidiItB > static inline
		void enact_r_p( tBidiItA aBeg, tBidiItA& aEnd, tBidiItB& aOutEnd ) noexcept
		{
			if( aBeg != aEnd )
			{
				auto beg = to_pointer(aBeg);
				auto end = to_pointer(aEnd);
				auto ond = to_pointer(aOutEnd);
			
				auto const count = end-beg;
				std::memmove( ond-count, beg, count*sizeof(*beg) );

				aEnd = aBeg;
				aOutEnd -= count;
			}
		}
		template< typename tBidiItA, typename tBidiItB > static inline
		void enact_r_pn( tBidiItA& aEnd, std::size_t aCount, tBidiItB& aOutEnd ) noexcept
		{
			if( aCount > 0 )
			{
				auto end = to_pointer(aEnd);
				auto ond = to_pointer(aOutEnd);
				std::memmove( ond-aCount, end-aCount, aCount*sizeof(*end) );

				aEnd -= aCount;
				aOutEnd -= aCount;
			}
		}
	};

	// Helper: Initialized Move {{{3
	template< bool >
	struct InitializedMove_
	{
		template< typename tInputIt, typename tForwardIt > static inline
		tForwardIt enact( tInputIt aBeg, tInputIt aEnd, tForwardIt aOut ) noexcept((detail::IsNothrowInMove<tInputIt,tForwardIt>::value))
		{
			for( ; aBeg != aEnd; ++aBeg, ++aOut )
				*aOut = std::move(*aBeg);
			return aOut;
		}
		template< typename tInputIt, typename tForwardIt > static inline
		std::pair<tInputIt,tForwardIt> enact_n( tInputIt aBeg, std::size_t aCount, tForwardIt aOut )
		{
			for( std::size_t i = 0; i < aCount; ++i, ++aBeg, ++aOut )
				*aOut = std::move(*aBeg);
			return std::make_pair( aBeg, aOut );
		}

		template< typename tInputIt, typename tForwardIt > static inline
		void enact_p( tInputIt& aBeg, tInputIt aEnd, tForwardIt& aOut ) noexcept((detail::IsNothrowInMove<tInputIt,tForwardIt>::value))
		{
			for( ; aBeg != aEnd; ++aBeg, ++aOut )
				*aOut = std::move(*aBeg);
		}
		template< typename tInputIt, typename tForwardIt > static inline
		void enact_pn( tInputIt& aBeg, std::size_t aCount, tForwardIt& aOut ) noexcept((detail::IsNothrowInMove<tInputIt,tForwardIt>::value))
		{
			for( std::size_t i = 0; i < aCount; ++i, ++aBeg, ++aOut )
				*aOut = std::move(*aBeg);
		}
		template< typename tInputIt, typename tForwardIt > static inline
		void enact_p( tInputIt& aBeg, tInputIt aEnd, tForwardIt& aOut, tForwardIt aOutEnd ) noexcept((detail::IsNothrowInMove<tInputIt,tForwardIt>::value))
		{
			for( ; aBeg != aEnd && aOut != aOutEnd; ++aBeg, ++aOut )
				*aOut = std::move(*aBeg);
		}

		template< typename tBidiItA, typename tBidiItB > static inline
		tBidiItB enact_r( tBidiItA aBeg, tBidiItA aEnd, tBidiItB aOutEnd )noexcept((detail::IsNothrowInMove<tBidiItA,tBidiItB>::value))
		{
			while( aBeg != aEnd )
				*(--aOutEnd) = std::move(*(--aEnd));

			return aOutEnd;
		}
		template< typename tBidiItA, typename tBidiItB > static inline
		std::pair<tBidiItA,tBidiItB> enact_r_n( tBidiItA aEnd, std::size_t aCount, tBidiItB aOutEnd )
		{
			for( std::size_t i = 0; i < aCount; ++i )
					*(--aOutEnd) = std::move(*(--aEnd));

			return std::make_pair( aEnd, aOutEnd );
		}

		template< typename tBidiItA, typename tBidiItB > static inline
		void enact_r_p( tBidiItA aBeg, tBidiItA& aEnd, tBidiItB& aOutEnd ) noexcept((detail::IsNothrowInMove<tBidiItA,tBidiItB>::value))
		{
			while( aBeg != aEnd )
				*(--aOutEnd) = std::move(*(--aEnd));
		}
		template< typename tBidiItA, typename tBidiItB > static inline
		void enact_r_pn( tBidiItA& aEnd, std::size_t aCount, tBidiItB& aOutEnd ) noexcept((detail::IsNothrowInMove<tBidiItA,tBidiItB>::value))
		{
			for( std::size_t i = 0; i < aCount; ++i )
				*(--aOutEnd) = std::move(*(--aEnd));
		}
	};
	template<>
	struct InitializedMove_<true> 
	{
		template< typename tInputIt, typename tForwardIt > static inline
		tForwardIt enact( tInputIt aBeg, tInputIt aEnd, tForwardIt aOut ) noexcept
		{
			if( aBeg != aEnd )
			{
				auto beg = to_pointer(aBeg);
				auto end = to_pointer(aEnd);
				auto out = to_pointer(aOut);
				std::memmove( out, beg, (end-beg)*sizeof(*beg) );
				return aOut + (end-beg);
			}
			
			return aOut;
		}
		template< typename tInputIt, typename tForwardIt > static inline
		std::pair<tInputIt,tForwardIt> enact_n( tInputIt aBeg, std::size_t aCount, tForwardIt aOut )
		{
			if( aCount > 0 )
			{
				auto beg = to_pointer(aBeg);
				auto out = to_pointer(aOut);
				std::memmove( out, beg, aCount*sizeof(*beg) );
			}
			return std::make_pair(aBeg + aCount, aOut + aCount);
		}

		template< typename tInputIt, typename tForwardIt > static inline
		void enact_p( tInputIt& aBeg, tInputIt aEnd, tForwardIt& aOut ) noexcept
		{
			if( aBeg != aEnd )
			{
				auto beg = to_pointer(aBeg);
				auto end = to_pointer(aEnd);
				auto out = to_pointer(aOut);
			
				auto const count = end-beg;
				std::memmove( out, beg, count*sizeof(*beg) );

				aBeg = aEnd;
				aOut += count;
			}
		}
		template< typename tInputIt, typename tForwardIt > static inline
		void enact_pn( tInputIt& aBeg, std::size_t aCount, tForwardIt& aOut ) noexcept
		{
			if( aCount > 0 )
			{
				auto beg = to_pointer(aBeg);
				auto out = to_pointer(aOut);
				std::memmove( out, beg, aCount*sizeof(*beg) );

				aBeg += aCount;
				aOut += aCount;
			}
		}
		template< typename tInputIt, typename tForwardIt > static inline
		void enact_p( tInputIt& aBeg, tInputIt aEnd, tForwardIt& aOut, tForwardIt aOutEnd ) noexcept
		{
			if( aBeg != aEnd )
			{
				auto beg = to_pointer(aBeg);
				auto end = to_pointer(aEnd);
				auto out = to_pointer(aOut);
				auto ond = to_pointer(aOutEnd);
			
				auto const count = std::min( end-beg, ond-out );
				std::memmove( out, beg, count*sizeof(*beg) );

				aBeg += count;
				aOut += count;
			}
		}


		template< typename tBidiItA, typename tBidiItB > static inline
		tBidiItB enact_r( tBidiItA aBeg, tBidiItA aEnd, tBidiItB aOutEnd ) noexcept
		{
			if( aBeg != aEnd )
			{
				auto beg = to_pointer(aBeg);
				auto end = to_pointer(aEnd);
				auto ond = to_pointer(aOutEnd);

				auto const count = end-beg;
				std::memmove( ond-count, beg, count*sizeof(*beg) );
				return aOutEnd - count;
			}

			return aOutEnd;
		}
		template< typename tBidiItA, typename tBidiItB > static inline
		std::pair<tBidiItA,tBidiItB> enact_r_n( tBidiItA aEnd, std::size_t aCount, tBidiItB aOutEnd )
		{
			if( aCount > 0 )
			{
				auto end = to_pointer(aEnd);
				auto ond = to_pointer(aOutEnd);
				std::memmove( ond-aCount, end-aCount, aCount*sizeof(*end) );
			}
			return std::make_pair(aEnd - aCount, aOutEnd - aCount);
		}

		template< typename tBidiItA, typename tBidiItB > static inline
		void enact_r_p( tBidiItA aBeg, tBidiItA& aEnd, tBidiItB& aOutEnd ) noexcept
		{
			if( aBeg != aEnd )
			{
				auto beg = to_pointer(aBeg);
				auto end = to_pointer(aEnd);
				auto ond = to_pointer(aOutEnd);
			
				auto const count = end-beg;
				std::memmove( ond-count, beg, count*sizeof(*beg) );

				aEnd = aBeg;
				aOutEnd -= count;
			}
		}
		template< typename tBidiItA, typename tBidiItB > static inline
		void enact_r_pn( tBidiItA& aEnd, std::size_t aCount, tBidiItB& aOutEnd ) noexcept
		{
			if( aCount > 0 )
			{
				auto end = to_pointer(aEnd);
				auto ond = to_pointer(aOutEnd);
				std::memmove( ond-aCount, end-aCount, aCount*sizeof(*end) );

				aEnd -= aCount;
				aOutEnd -= aCount;
			}
		}
	};

	// Helper: Initialized MoveIfNoexcept {{{3
	template< bool >
	struct InitializedMoveIfNoexcept_
	{
		template< typename tFwdIn, typename tFwdOut > static inline
		tFwdOut enact( tFwdIn aBeg, tFwdIn aEnd, tFwdOut aOut )
		{
			return ftl::initialized_copy( aBeg, aEnd, aOut );
		}
		template< typename tFwdIn, typename tFwdOut > static inline
		std::pair<tFwdIn,tFwdOut> enact_n( tFwdIn aBeg, std::size_t aCount, tFwdOut aOut )
		{
			return ftl::initialized_copy_n( aBeg, aCount, aOut );
		}

		template< typename tFwdIn, typename tFwdOut > static inline
		void enact_p( tFwdIn& aBeg, tFwdIn aEnd, tFwdOut& aOut )
		{
			ftl::initialized_copy_p( aBeg, aEnd, aOut );
		}
		template< typename tFwdIn, typename tFwdOut > static inline
		void enact_pn( tFwdIn& aBeg, std::size_t aCount, tFwdOut& aOut )
		{
			ftl::initialized_copy_pn( aBeg, aCount, aOut );
		}
		template< typename tFwdIn, typename tFwdOut > static inline
		void enact_p( tFwdIn& aBeg, tFwdIn aEnd, tFwdOut& aOut, tFwdOut aOutEnd )
		{
			ftl::initialized_copy_p( aBeg, aEnd, aOut, aOutEnd );
		}

		template< typename tBidiItA, typename tBidiItB > static inline
		tBidiItB enact_r( tBidiItA aBeg, tBidiItA aEnd, tBidiItB aOutEnd )
		{
			return ftl::initialized_copy_rev( aBeg, aEnd, aOutEnd );
		}
		template< typename tBidiItA, typename tBidiItB > static inline
		std::pair<tBidiItA,tBidiItB> enact_r_n( tBidiItA aEnd, std::size_t aCount, tBidiItB aOutEnd )
		{
			return ftl::initialized_copy_rev_n( aEnd, aCount, aOutEnd );
		}

		template< typename tBidiItA, typename tBidiItB > static inline
		void enact_r_p( tBidiItA aBeg, tBidiItA& aEnd, tBidiItB& aOutEnd )
		{
			ftl::initialized_copy_rev_p( aBeg, aEnd, aOutEnd );
		}
		template< typename tBidiItA, typename tBidiItB > static inline
		void enact_r_pn( tBidiItA& aEnd, std::size_t aCount, tBidiItB& aOutEnd )
		{
			ftl::initialized_copy_rev_pn( aEnd, aCount, aOutEnd );
		}
	};

	template<>
	struct InitializedMoveIfNoexcept_<true>
	{
		template< typename tFwdIn, typename tFwdOut > static inline
		tFwdOut enact( tFwdIn aBeg, tFwdIn aEnd, tFwdOut aOut ) noexcept
		{
			return ftl::initialized_move( aBeg, aEnd, aOut );
		}
		template< typename tFwdIn, typename tFwdOut > static inline
		std::pair<tFwdIn,tFwdOut> enact_n( tFwdIn aBeg, std::size_t aCount, tFwdOut aOut ) noexcept
		{
			return ftl::initialized_move_n( aBeg, aCount, aOut );
		}

		template< typename tFwdIn, typename tFwdOut > static inline
		void enact_p( tFwdIn& aBeg, tFwdIn aEnd, tFwdOut& aOut ) noexcept
		{
			ftl::initialized_move_p( aBeg, aEnd, aOut );
		}
		template< typename tFwdIn, typename tFwdOut > static inline
		void enact_pn( tFwdIn& aBeg, std::size_t aCount, tFwdOut& aOut ) noexcept
		{
			ftl::initialized_move_pn( aBeg, aCount, aOut );
		}
		template< typename tFwdIn, typename tFwdOut > static inline
		void enact_p( tFwdIn& aBeg, tFwdIn aEnd, tFwdOut& aOut, tFwdOut aOutEnd ) noexcept
		{
			ftl::initialized_move_p( aBeg, aEnd, aOut, aOutEnd );
		}

		template< typename tBidiItA, typename tBidiItB > static inline
		tBidiItB enact_r( tBidiItA aBeg, tBidiItA aEnd, tBidiItB aOutEnd ) noexcept
		{
			return ftl::initialized_move_rev( aBeg, aEnd, aOutEnd );
		}
		template< typename tBidiItA, typename tBidiItB > static inline
		std::pair<tBidiItA,tBidiItB> enact_r_n( tBidiItA aEnd, std::size_t aCount, tBidiItB aOutEnd ) noexcept
		{
			return ftl::initialized_move_rev_n( aEnd, aCount, aOutEnd );
		}

		template< typename tBidiItA, typename tBidiItB > static inline
		void enact_r_p( tBidiItA aBeg, tBidiItA& aEnd, tBidiItB& aOutEnd ) noexcept
		{
			ftl::initialized_move_rev_p( aBeg, aEnd, aOutEnd );
		}
		template< typename tBidiItA, typename tBidiItB > static inline
		void enact_r_pn( tBidiItA& aEnd, std::size_t aCount, tBidiItB& aOutEnd ) noexcept
		{
			ftl::initialized_move_rev_pn( aEnd, aCount, aOutEnd );
		}
	};

	// Helper: Initialized Relocate {{{3
	template< bool >
	struct InitializedRelocate_
	{
		template< typename tFwdIn, typename tFwdOut > static inline
		tFwdOut enact( tFwdIn aBeg, tFwdIn aEnd, tFwdOut aOut ) noexcept((detail::IsNothrowInReloc<tFwdIn,tFwdOut>::value))
		{
			auto ret = ftl::initialized_move( aBeg, aEnd, aOut );
			ftl::destroy( aBeg, aEnd );
			return ret;
		}
		template< typename tFwdIn, typename tFwdOut > static inline
		std::pair<tFwdIn,tFwdOut> enact_n( tFwdIn aBeg, std::size_t aCount, tFwdOut aOut )
		{
			auto ret = ftl::initialized_move_n( aBeg, aCount, aOut );
			ftl::destroy_n( aBeg, aCount );
			return ret;
		}
		
		template< typename tInputIt, typename tForwardIt > static inline
		void enact_p( tInputIt& aBeg, tInputIt aEnd, tForwardIt& aOut ) noexcept((detail::IsNothrowInReloc<tInputIt,tForwardIt>::value))
		{
			using InTy_ = typename std::iterator_traits<tInputIt>::value_type;

			for( ; aBeg != aEnd; ++aBeg )
			{
				InTy_& in = *aBeg;
				*aOut = std::move(in);
				++aOut;
				destroy_at( &in );
			}
		}
		template< typename tInputIt, typename tForwardIt > static inline
		void enact_pn( tInputIt& aBeg, std::size_t aCount, tForwardIt& aOut ) noexcept((detail::IsNothrowInReloc<tInputIt,tForwardIt>::value))
		{
			using InTy_ = typename std::iterator_traits<tInputIt>::value_type;

			for( std::size_t i = 0; i < aCount; ++i, ++aBeg )
			{
				InTy_& in = *aBeg;
				*aOut = std::move(in);
				++aOut;
				destroy_at( &in );
			}
		}
		template< typename tInputIt, typename tForwardIt > static inline
		void enact_p( tInputIt& aBeg, tInputIt aEnd, tForwardIt& aOut, tForwardIt aOutEnd ) noexcept((detail::IsNothrowInReloc<tInputIt,tForwardIt>::value))
		{
			using InTy_ = typename std::iterator_traits<tInputIt>::value_type;

			for( ; aBeg != aEnd && aOut != aOutEnd; ++aBeg )
			{
				InTy_& in = *aBeg;
				*aOut = std::move(in);
				++aOut;
				destroy_at( &in );
			}
		}

	};
	template<>
	struct InitializedRelocate_<true> 
	{
		template< typename tInputIt, typename tForwardIt > static inline
		tForwardIt enact( tInputIt aBeg, tInputIt aEnd, tForwardIt aOut ) noexcept
		{
			if( aBeg != aEnd )
			{
				auto beg = to_pointer(aBeg);
				auto end = to_pointer(aEnd);
				auto out = to_pointer(aOut);
			
				std::memmove( out, beg, (end-beg)*sizeof(*beg) );
				return aOut + (end-beg);
			}
			
			return aOut;
		}
		template< typename tInputIt, typename tForwardIt > static inline
		std::pair<tInputIt,tForwardIt> enact_n( tInputIt aBeg, std::size_t aCount, tForwardIt aOut )
		{
			if( aCount > 0 )
			{
				auto beg = to_pointer(aBeg);
				auto out = to_pointer(aOut);
				std::memmove( out, beg, aCount*sizeof(*beg) );
			}
			return std::make_pair(aBeg + aCount, aOut + aCount);
		}

		template< typename tInputIt, typename tForwardIt > static inline
		void enact_p( tInputIt& aBeg, tInputIt aEnd, tForwardIt& aOut ) noexcept
		{
			if( aBeg != aEnd )
			{
				auto beg = to_pointer(aBeg);
				auto end = to_pointer(aEnd);
				auto out = to_pointer(aOut);
			
				auto const count = end-beg;
				std::memmove( out, beg, count*sizeof(*beg) );

				aBeg = aEnd;
				aOut += count;
			}
		}
		template< typename tInputIt, typename tForwardIt > static inline
		void enact_pn( tInputIt& aBeg, std::size_t aCount, tForwardIt& aOut ) noexcept
		{
			if( aCount > 0 )
			{
				auto beg = to_pointer(aBeg);
				auto out = to_pointer(aOut);
				std::memmove( out, beg, aCount*sizeof(*beg) );

				aBeg += aCount;
				aOut += aCount;
			}
		}
		template< typename tInputIt, typename tForwardIt > static inline
		void enact_p( tInputIt& aBeg, tInputIt aEnd, tForwardIt& aOut, tForwardIt aOutEnd ) noexcept
		{
			if( aBeg != aEnd )
			{
				auto beg = to_pointer(aBeg);
				auto end = to_pointer(aEnd);
				auto out = to_pointer(aOut);
				auto ond = to_pointer(aOutEnd);
			
				auto const count = std::min( end-beg, ond-out );
				std::memmove( out, beg, count*sizeof(*beg) );

				aBeg += count;
				aOut += count;
			}
		}
	};

	// Helper: Initialized RelocateIfNoexcept {{{3
	template< bool >
	struct InitializedRelocateIfNoexceptImpl_
	{
		template< typename tFwdIn, typename tFwdOut > static inline
		void enact_p( tFwdIn& aBeg, tFwdIn aEnd, tFwdOut& aOut ) //TODO: noexcept if noexcept-copy-construct
		{
			auto temp = aBeg;
			ftl::initialized_copy_p( temp, aEnd, aOut );
			ftl::destroy( aBeg, aEnd );
			aBeg = temp;
		}
		template< typename tFwdIn, typename tFwdOut > static inline
		void enact_pn( tFwdIn& aBeg, std::size_t aCount, tFwdOut& aOut ) //TODO: noexcept if noexcept-copy-construct
		{
			auto temp = aBeg;
			ftl::initialized_copy_pn( temp, aCount, aOut );
			aBeg = ftl::destroy_n( aBeg, aCount );
		}
		template< typename tFwdIn, typename tFwdOut > static inline
		void enact_p( tFwdIn& aBeg, tFwdIn aEnd, tFwdOut& aOut, tFwdOut aOutEnd ) //TODO: noexcept if noexcept-copy-construct
		{
			auto temp = aBeg;
			ftl::initialized_copy_p( temp, aEnd, aOut, aOutEnd );
			ftl::destroy( aBeg, temp );
			aBeg = temp;
		}
	};
	template<>
	struct InitializedRelocateIfNoexceptImpl_<true>
		: InitializedRelocate_<false>
	{};
	
	template< bool, typename tInIt, typename tOutIt >
	struct InitializedRelocateIfNoexcept_
		: InitializedRelocateIfNoexceptImpl_<detail::IsNothrowInMove<tInIt,tOutIt>::value>
	{
		template< typename tFwdIn, typename tFwdOut > static inline
		tFwdOut enact( tFwdIn aBeg, tFwdIn aEnd, tFwdOut aOut ) noexcept((detail::IsNothrowInReloc<tFwdIn,tFwdOut>::value))
		{
			auto ret = ftl::initialized_move_if_noexcept( aBeg, aEnd, aOut );
			ftl::destroy( aBeg, aEnd );
			return ret;
		}

		template< typename tFwdIn, typename tFwdOut > static inline
		std::pair<tFwdIn,tFwdOut> enact_n( tFwdIn aBeg, std::size_t aCount, tFwdOut aOut )
		{
			auto ret = ftl::initialized_move_if_noexcept_n( aBeg, aCount, aOut );
			ftl::destroy_n( aBeg, aCount );
			return ret;
		}
	};

	template< typename tFwdIn, typename tFwdOut >
	struct InitializedRelocateIfNoexcept_<true, tFwdIn, tFwdOut>
		: InitializedRelocate_<true>
	{};

	// Helper: Initialized Swap {{{3
	template< bool, typename, typename, std::size_t >
	struct InitializedSwap_
	{
		template< typename tInA, typename tInB >  static inline
		tInB enact( tInA aBeg, tInA aEnd, tInB aOut ) noexcept((detail::IsNothrowInSwap<tInA,tInB>::value))
		{
			using std::swap;
			for( ; aBeg != aEnd; ++aBeg, ++aOut )
				swap( *aBeg, *aOut );

			return aOut;
		}
		template< typename tInA, typename tInB >  static inline
		std::pair<tInA,tInB> enact_n( tInA aBeg, std::size_t aCount, tInB aOut ) noexcept((detail::IsNothrowInSwap<tInA,tInB>::value))
		{
			using std::swap;
			for( std::size_t i = 0; i < aCount; ++i, ++aBeg, ++aOut )
				swap( *aBeg, *aOut );
			
			return std::make_pair(aBeg,aOut);
		}

		template< typename tInA, typename tInB > static inline
		void enact_p( tInA& aBeg, tInA aEnd, tInB& aOut ) noexcept((detail::IsNothrowInSwap<tInA,tInB>::value))
		{
			using std::swap;
			for( ; aBeg != aEnd; ++aBeg, ++aOut )
				swap( *aBeg, *aOut );
		}
		template< typename tInA, typename tInB > static inline
		void enact_pn( tInA& aBeg, std::size_t aCount, tInB& aOut ) noexcept((detail::IsNothrowInSwap<tInA,tInB>::value))
		{
			using std::swap;
			for( std::size_t i = 0; i < aCount; ++i, ++aBeg, ++aOut )
				swap( *aBeg, *aOut );
		}
	};
	template< typename tItA, typename tItB, std::size_t tBufferCount >
	struct InitializedSwap_<true, tItA, tItB, tBufferCount>
	{
		using Ty_ = typename std::iterator_traits<tItB>::value_type;
		static_assert( std::is_same<Ty_, typename std::iterator_traits<tItA>::value_type>::value, "initialized_swap(): types mismatch" );

		using StackBuffer_ = typename std::aligned_storage<
			tBufferCount * sizeof(Ty_),
			alignof(Ty_)
		>::type;

		template< typename tInA, typename tInB >  static inline
		tInB enact( tInA aBeg, tInA aEnd, tInB aOut ) noexcept
		{
			if( aBeg != aEnd )
			{
				StackBuffer_ buffer;

				auto beg = to_pointer(aBeg);
				auto end = to_pointer(aEnd);
				auto out = to_pointer(aOut);
			
				std::size_t count = end-beg;
				for( ; count >= tBufferCount; count -= tBufferCount )
				{
					std::memmove( &buffer, beg, tBufferCount*sizeof(Ty_) );
					std::memmove( beg, out, tBufferCount*sizeof(Ty_) );
					std::memmove( out, &buffer, tBufferCount*sizeof(Ty_) );
					beg += tBufferCount;
					out += tBufferCount;
				}

				if( count )
				{
					std::memmove( &buffer, beg, count*sizeof(Ty_) );
					std::memmove( beg, out, count*sizeof(Ty_) );
					std::memmove( out, &buffer, count*sizeof(Ty_) );
				}

				return aOut + (count + out - to_pointer(aOut));
			}

			return aOut;
		}
		template< typename tInA, typename tInB >  static inline
		std::pair<tInA,tInB> enact_n( tInA aBeg, std::size_t aCount, tInB aOut )
		{
			if( aCount )
			{
				StackBuffer_ buffer;

				auto beg = to_pointer(aBeg);
				auto out = to_pointer(aOut);
			
				for( ; aCount >= tBufferCount; aCount -= tBufferCount )
				{
					std::memmove( &buffer, beg, tBufferCount*sizeof(Ty_) );
					std::memmove( beg, out, tBufferCount*sizeof(Ty_) );
					std::memmove( out, &buffer, tBufferCount*sizeof(Ty_) );
					beg += tBufferCount;
					out += tBufferCount;
				}

				if( aCount )
				{
					std::memmove( &buffer, beg, aCount*sizeof(Ty_) );
					std::memmove( beg, out, aCount*sizeof(Ty_) );
					std::memmove( out, &buffer, aCount*sizeof(Ty_) );
				}

				auto total = aCount + (out - to_pointer(aOut));
				return std::make_pair( aBeg+total, aOut+total );
			}

			return std::make_pair(aBeg,aOut);
		}

		template< typename tInA, typename tInB >  static inline
		void enact_p( tInA& aBeg, tInA aEnd, tInB& aOut ) noexcept
		{
			aOut = enact( aBeg, aEnd, aOut );
			aBeg = aEnd;
		}
		template< typename tInA, typename tInB >  static inline
		void enact_pn( tInA& aBeg, std::size_t aCount, tInB& aOut ) noexcept
		{
			if( aCount )
			{
				StackBuffer_ buffer;

				auto beg = to_pointer(aBeg);
				auto out = to_pointer(aOut);
			
				for( ; aCount >= tBufferCount; aCount -= tBufferCount )
				{
					std::memmove( &buffer, beg, tBufferCount*sizeof(Ty_) );
					std::memmove( beg, out, tBufferCount*sizeof(Ty_) );
					std::memmove( out, &buffer, tBufferCount*sizeof(Ty_) );
					beg += tBufferCount;
					out += tBufferCount;
				}

				if( aCount )
				{
					std::memmove( &buffer, beg, aCount*sizeof(Ty_) );
					std::memmove( beg, out, aCount*sizeof(Ty_) );
					std::memmove( out, &buffer, aCount*sizeof(Ty_) );
				}

				auto const total = aCount + (out - to_pointer(aOut));
				aBeg += total;
				aOut += total;
			}
		}
	};
};

//--    initialized_copy*()                 ///{{{2///////////////////////////
template< typename tInputIt, typename tForwardIt > inline
tForwardIt initialized_copy( tInputIt aBeg, tInputIt aEnd, tForwardIt aOut )
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	return detail::InitializedCopy_<
		detail::ICopyCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact( aBeg, aEnd, aOut );
}

template< typename tInputIt, typename tForwardIt > inline
std::pair<tInputIt,tForwardIt> initialized_copy_n( tInputIt aBeg, std::size_t aCount, tForwardIt aOut )
{
	return detail::InitializedCopy_<
		detail::ICopyCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_n( aBeg, aCount, aOut );
}

template< typename tInputIt, typename tForwardIt > inline
void initialized_copy_p( tInputIt& aBeg, tInputIt aEnd, tForwardIt& aOut ) noexcept((detail::IsNothrowInCopy<tInputIt,tForwardIt>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	detail::InitializedCopy_<
		detail::ICopyCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_p( aBeg, aEnd, aOut );
}

template< typename tInputIt, typename tForwardIt > inline
void initialized_copy_pn( tInputIt& aBeg, std::size_t aCount, tForwardIt& aOut ) noexcept((detail::IsNothrowInCopy<tInputIt,tForwardIt>::value))
{
	detail::InitializedCopy_<
		detail::ICopyCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_pn( aBeg, aCount, aOut );
}

template< typename tInputIt, typename tForwardIt > inline
void initialized_copy_p( tInputIt& aBeg, tInputIt aEnd, tForwardIt& aOut, tForwardIt aOutEnd ) noexcept((detail::IsNothrowInCopy<tInputIt,tForwardIt>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	detail::InitializedCopy_<
		detail::ICopyCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_p( aBeg, aEnd, aOut, aOutEnd );
}

//--    initialized_copy_rev*()             ///{{{2///////////////////////////
template< typename tBidiItA, typename tBidiItB > inline
tBidiItB initialized_copy_rev( tBidiItA aBeg, tBidiItA aEnd, tBidiItB aEndOut )
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	return detail::InitializedCopy_<
		detail::ICopyCanMemcpy_<tBidiItA,tBidiItB>::value
	>::enact_r( aBeg, aEnd, aEndOut );
}

template< typename tBidiItA, typename tBidiItB > inline
std::pair<tBidiItA,tBidiItB> initialized_copy_rev_n( tBidiItA aEnd, std::size_t aCount, tBidiItB aEndOut )
{
	return detail::InitializedCopy_<
		detail::ICopyCanMemcpy_<tBidiItA,tBidiItB>::value
	>::enact_r_n( aEnd, aCount, aEndOut );
}

template< typename tBidiItA, typename tBidiItB > inline
void initialized_copy_rev_p( tBidiItA aBeg, tBidiItA& aEnd, tBidiItB& aEndOut ) noexcept((detail::IsNothrowInCopy<tBidiItA,tBidiItB>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	detail::InitializedCopy_<
		detail::ICopyCanMemcpy_<tBidiItA,tBidiItB>::value
	>::enact_r_p( aBeg, aEnd, aEndOut );
}
template< typename tBidiItA, typename tBidiItB >
void initialized_copy_rev_pn( tBidiItA& aEnd, std::size_t aCount, tBidiItB& aEndOut ) noexcept((detail::IsNothrowInCopy<tBidiItA,tBidiItB>::value))
{
	detail::InitializedCopy_<
		detail::ICopyCanMemcpy_<tBidiItA,tBidiItB>::value
	>::enact_r_pn( aEnd, aCount, aEndOut );
}

//--    initialized_move*()                 ///{{{2///////////////////////////
template< typename tInputIt, typename tForwardIt > inline
tForwardIt initialized_move( tInputIt aBeg, tInputIt aEnd, tForwardIt aOut ) noexcept((detail::IsNothrowInMove<tInputIt,tForwardIt>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	return detail::InitializedMove_<
		detail::IMoveCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact( aBeg, aEnd, aOut );
}
template< typename tInputIt, typename tForwardIt > inline
std::pair<tInputIt,tForwardIt> initialized_move_n( tInputIt aBeg, std::size_t aCount, tForwardIt aOut )
{
	return detail::InitializedMove_<
		detail::IMoveCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_n( aBeg, aCount, aOut );
}

template< typename tInputIt, typename tForwardIt > inline
void initialized_move_p( tInputIt& aBeg, tInputIt aEnd, tForwardIt& aOut ) noexcept((detail::IsNothrowInMove<tInputIt,tForwardIt>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	detail::InitializedMove_<
		detail::IMoveCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_p( aBeg, aEnd, aOut );
}
template< typename tInputIt, typename tForwardIt > inline
void initialized_move_pn( tInputIt& aBeg, std::size_t aCount, tForwardIt& aOut ) noexcept((detail::IsNothrowInMove<tInputIt,tForwardIt>::value))
{
	detail::InitializedMove_<
		detail::IMoveCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_pn( aBeg, aCount, aOut );
}
template< typename tInputIt, typename tForwardIt > inline
void initialized_move_p( tInputIt& aBeg, tInputIt aEnd, tForwardIt& aOut, tForwardIt aOutEnd ) noexcept((detail::IsNothrowInMove<tInputIt,tForwardIt>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	detail::InitializedMove_<
		detail::IMoveCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_p( aBeg, aEnd, aOut, aOutEnd );
}

//--    initialized_move_rev*()             ///{{{2///////////////////////////
template< typename tBidiItA, typename tBidiItB > inline
tBidiItB initialized_move_rev( tBidiItA aBeg, tBidiItA aEnd, tBidiItB aEndOut ) noexcept((detail::IsNothrowInMove<tBidiItA,tBidiItB>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	return detail::InitializedMove_<
		detail::IMoveCanMemcpy_<tBidiItA,tBidiItB>::value
	>::enact_r( aBeg, aEnd, aEndOut );
}
template< typename tBidiItA, typename tBidiItB > inline
std::pair<tBidiItA,tBidiItB> initialized_move_rev_n( tBidiItA aEnd, std::size_t aCount, tBidiItB aEndOut )
{
	return detail::InitializedMove_<
		detail::IMoveCanMemcpy_<tBidiItA,tBidiItB>::value
	>::enact_r_n( aEnd, aCount, aEndOut );
}

template< typename tBidiItA, typename tBidiItB > inline
void initialized_move_rev_p( tBidiItA aBeg, tBidiItA& aEnd, tBidiItB& aEndOut ) noexcept((detail::IsNothrowInMove<tBidiItA,tBidiItB>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	detail::InitializedMove_<
		detail::IMoveCanMemcpy_<tBidiItA,tBidiItB>::value
	>::enact_r_p( aBeg, aEnd, aEndOut );
}
template< typename tBidiItA, typename tBidiItB >
void initialized_move_rev_pn( tBidiItA& aEnd, std::size_t aCount, tBidiItB& aEndOut ) noexcept((detail::IsNothrowInMove<tBidiItA,tBidiItB>::value))
{
	detail::InitializedMove_<
		detail::IMoveCanMemcpy_<tBidiItA,tBidiItB>::value
	>::enact_r_pn( aEnd, aCount, aEndOut );
}

//--    initialized_move_if_noexcept*()     ///{{{2///////////////////////////
template< typename tInputIt, typename tForwardIt > inline
tForwardIt initialized_move_if_noexcept( tInputIt aBeg, tInputIt aEnd, tForwardIt aOut ) noexcept((detail::IsNothrowInMove<tInputIt,tForwardIt>::value))
{
	return detail::InitializedMoveIfNoexcept_<
		detail::IsNothrowInMove<tInputIt,tForwardIt>::value
	>::enact( aBeg, aEnd, aOut );
}
template< typename tInputIt, typename tForwardIt > inline
std::pair<tInputIt,tForwardIt> initialized_move_if_noexcept_n( tInputIt aBeg, std::size_t aCount, tForwardIt aOut )
{
	return detail::InitializedMoveIfNoexcept_<
		detail::IsNothrowInMove<tInputIt,tForwardIt>::value
	>::enact_n( aBeg, aCount, aOut );
}
template< typename tInputIt, typename tForwardIt > inline
void initialized_move_if_noexcept_p( tInputIt& aBeg, tInputIt aEnd, tForwardIt& aOut ) 
	noexcept((detail::IsNothrowInMove<tInputIt,tForwardIt>::value))
{
	detail::InitializedMoveIfNoexcept_<
		detail::IsNothrowInMove<tInputIt,tForwardIt>::value
	>::enact_p( aBeg, aEnd, aOut );
}
template< typename tInputIt, typename tForwardIt > inline
void initialized_move_if_noexcept_pn( tInputIt& aBeg, std::size_t aCount, tForwardIt&  aOut ) noexcept((detail::IsNothrowInMove<tInputIt,tForwardIt>::value))
{
	detail::InitializedMoveIfNoexcept_<
		detail::IsNothrowInMove<tInputIt,tForwardIt>::value
	>::enact_pn( aBeg, aCount, aOut );
}
template< typename tInputIt, typename tForwardIt > inline
void initialized_move_if_noexcept_p( tInputIt& aBeg, tInputIt aEnd, tForwardIt& aOut, tForwardIt aOutEnd ) 
	noexcept((detail::IsNothrowInMove<tInputIt,tForwardIt>::value))
{
	detail::InitializedMoveIfNoexcept_<
		detail::IsNothrowInMove<tInputIt,tForwardIt>::value
	>::enact_p( aBeg, aEnd, aOut, aOutEnd );
}
//
//--    initialized_move_if_noexcept_rev*() ///{{{2///////////////////////////
template< typename tBidiItA, typename tBidiItB > inline
tBidiItB initialized_move_if_noexcept_rev( tBidiItA aBeg, tBidiItA aEnd, tBidiItB aEndOut ) noexcept((detail::IsNothrowInMove<tBidiItA,tBidiItB>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	return detail::InitializedMoveIfNoexcept_<
		detail::IsNothrowInMove<tBidiItA,tBidiItB>::value
	>::enact_r( aBeg, aEnd, aEndOut );
}
template< typename tBidiItA, typename tBidiItB > inline
std::pair<tBidiItA,tBidiItB> initialized_move_if_noexcept_rev_n( tBidiItA aEnd, std::size_t aCount, tBidiItB aEndOut )
{
	return detail::InitializedMoveIfNoexcept_<
		detail::IsNothrowInMove<tBidiItA,tBidiItB>::value
	>::enact_r_n( aEnd, aCount, aEndOut );
}

template< typename tBidiItA, typename tBidiItB > inline
void initialized_move_if_noexcept_rev_p( tBidiItA aBeg, tBidiItA& aEnd, tBidiItB& aEndOut ) noexcept((detail::IsNothrowInMove<tBidiItA,tBidiItB>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	detail::InitializedMoveIfNoexcept_<
		detail::IsNothrowInMove<tBidiItA,tBidiItB>::value
	>::enact_r_p( aBeg, aEnd, aEndOut );
}
template< typename tBidiItA, typename tBidiItB >
void initialized_move_if_noexcept_rev_pn( tBidiItA& aEnd, std::size_t aCount, tBidiItB& aEndOut ) noexcept((detail::IsNothrowInMove<tBidiItA,tBidiItB>::value))
{
	detail::InitializedMoveIfNoexcept_<
		detail::IsNothrowInMove<tBidiItA,tBidiItB>::value
	>::enact_r_pn( aEnd, aCount, aEndOut );
}

//--    initialized_relocate*()             ///{{{2///////////////////////////
template< typename tFwdIn, typename tFwdOut > inline
tFwdOut initialized_relocate( tFwdIn aBeg, tFwdIn aEnd, tFwdOut aOut )  noexcept((detail::IsNothrowInReloc<tFwdIn,tFwdOut>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	return detail::InitializedRelocate_<
		detail::IRelocCanMemcpy_<tFwdIn,tFwdOut>::value
	>::enact( aBeg, aEnd, aOut );
}
template< typename tFwdIn, typename tFwdOut > inline
std::pair<tFwdIn,tFwdOut> initialized_relocate_n( tFwdIn aBeg, std::size_t aCount, tFwdOut aOut )
{
	return detail::InitializedRelocate_<
		detail::IRelocCanMemcpy_<tFwdIn,tFwdOut>::value
	>::enact_n( aBeg, aCount, aOut );
}

template< typename tInputIt, typename tForwardIt > inline
void initialized_relocate_p( tInputIt& aBeg, tInputIt aEnd, tForwardIt& aOut ) noexcept((detail::IsNothrowInReloc<tInputIt,tForwardIt>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	detail::InitializedRelocate_<
		detail::IRelocCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_p( aBeg, aEnd, aOut );
}
template< typename tInputIt, typename tForwardIt > inline
void initialized_relocate_pn( tInputIt& aBeg, std::size_t aCount, tForwardIt& aOut ) noexcept((detail::IsNothrowInReloc<tInputIt,tForwardIt>::value))
{
	detail::InitializedRelocate_<
		detail::IRelocCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_pn( aBeg, aCount, aOut );
}
template< typename tInputIt, typename tForwardIt > inline
void initialized_relocate_p( tInputIt& aBeg, tInputIt aEnd, tForwardIt& aOut, tForwardIt aOutEnd ) noexcept((detail::IsNothrowInReloc<tInputIt,tForwardIt>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	detail::InitializedRelocate_<
		detail::IRelocCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_p( aBeg, aEnd, aOut, aOutEnd );
}

//--    initialized_relocate_if_noexcept*()     ///{{{2///////////////////////
template< typename tFwdIn, typename tFwdOut > inline
tFwdOut initialized_relocate_if_noexcept( tFwdIn aBeg, tFwdIn aEnd, tFwdOut aOut )  noexcept((detail::IsNothrowInReloc<tFwdIn,tFwdOut>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	return detail::InitializedRelocateIfNoexcept_<
		detail::IRelocCanMemcpy_<tFwdIn,tFwdOut>::value,
		tFwdIn, tFwdOut
	>::enact( aBeg, aEnd, aOut );
}
template< typename tFwdIn, typename tFwdOut > inline
std::pair<tFwdIn,tFwdOut> initialized_relocate_if_noexcept_n( tFwdIn aBeg, std::size_t aCount, tFwdOut aOut )
{
	return detail::InitializedRelocateIfNoexcept_<
		detail::IRelocCanMemcpy_<tFwdIn,tFwdOut>::value,
		tFwdIn, tFwdOut
	>::enact_n( aBeg, aCount, aOut );
}

template< typename tFwdIn, typename tFwdOut > inline
void initialized_relocate_if_noexcept_p( tFwdIn& aBeg, tFwdIn aEnd, tFwdOut& aOut ) noexcept((detail::IsNothrowInReloc<tFwdIn,tFwdOut>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	detail::InitializedRelocateIfNoexcept_<
		detail::IRelocCanMemcpy_<tFwdIn,tFwdOut>::value,
		tFwdIn, tFwdOut
	>::enact_p( aBeg, aEnd, aOut );
}
template< typename tFwdIn, typename tFwdOut > inline
void initialized_relocate_if_noexcept_pn( tFwdIn& aBeg, std::size_t aCount, tFwdOut& aOut ) noexcept((detail::IsNothrowInReloc<tFwdIn,tFwdOut>::value))
{
	detail::InitializedRelocateIfNoexcept_<
		detail::IRelocCanMemcpy_<tFwdIn,tFwdOut>::value,
		tFwdIn, tFwdOut
	>::enact_pn( aBeg, aCount, aOut );
}
template< typename tFwdIn, typename tFwdOut > inline
void initialized_relocate_if_noexcept_p( tFwdIn& aBeg, tFwdIn aEnd, tFwdOut& aOut, tFwdOut aOutEnd ) noexcept((detail::IsNothrowInReloc<tFwdIn,tFwdOut>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	detail::InitializedRelocateIfNoexcept_<
		detail::IRelocCanMemcpy_<tFwdIn,tFwdOut>::value,
		tFwdIn, tFwdOut
	>::enact_p( aBeg, aEnd, aOut, aOutEnd );
}

//--    initialized_swap*()                 ///{{{2///////////////////////////
template< typename tInputA, typename tInputB, std::size_t tSize > inline
tInputB initialized_swap( tInputA aBeg, tInputA aEnd, tInputB aOut, StaticSize<tSize> ) noexcept((detail::IsNothrowInSwap<tInputA,tInputB>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	return detail::InitializedSwap_<
		detail::ISwapCanMemcpy_<tInputA,tInputB>::value,
		tInputA, tInputB, tSize
	>::enact( aBeg, aEnd, aOut );
}
template< typename tInputA, typename tInputB, std::size_t tSize > inline
std::pair<tInputA,tInputB> initialized_swap_n( tInputA aBeg, std::size_t aCount, tInputB aOut, StaticSize<tSize> )
{
	return detail::InitializedSwap_<
		detail::ISwapCanMemcpy_<tInputA,tInputB>::value,
		tInputA, tInputB, tSize
	>::enact_n( aBeg, aCount, aOut );
}

template< typename tInputA, typename tInputB, std::size_t tSize > inline
void initialized_swap_p( tInputA& aBeg, tInputA aEnd, tInputB& aOut, StaticSize<tSize> ) noexcept((detail::IsNothrowInSwap<tInputA,tInputB>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	detail::InitializedSwap_<
		detail::ISwapCanMemcpy_<tInputA,tInputB>::value,
		tInputA, tInputB, tSize
	>::enact_p( aBeg, aEnd, aOut );

}
template< typename tInputA, typename tInputB, std::size_t tSize > inline
void initialized_swap_pn( tInputA& aBeg, std::size_t aCount, tInputB& aOut, StaticSize<tSize> ) noexcept((detail::IsNothrowInSwap<tInputA,tInputB>::value))
{
	detail::InitializedSwap_<
		detail::ISwapCanMemcpy_<tInputA,tInputB>::value,
		tInputA, tInputB, tSize
	>::enact_pn( aBeg, aCount, aOut );
}

//--    initialized_fill*()                 ///{{{2///////////////////////////
template< typename tForwardIt, typename tType > inline
void initialized_fill( tForwardIt aBeg, tForwardIt aEnd, tType const& aVal )
{
	std::fill( aBeg, aEnd, aVal );
}
template< typename tForwardIt, typename tType > inline
tForwardIt initialized_fill_n( tForwardIt aBeg, std::size_t aCount, tType const& aVal )
{
	return std::fill_n( aBeg, aCount, aVal );
}

template< typename tForwardIt, typename tType >
void initialized_fill_p( tForwardIt& aBeg, tForwardIt aEnd, tType const& aVal ) noexcept((detail::IsNothrowInCtor<tForwardIt,tType const&>::value))
{
	for( ; aBeg != aEnd; ++aBeg )
		*aBeg = aVal;
}
template< typename tForwardIt, typename tType >
void initialized_fill_pn( tForwardIt& aBeg, std::size_t aCount, tType const& aVal ) noexcept((detail::IsNothrowInCtor<tForwardIt,tType const&>::value))
{
	for( std::size_t i = 0; i < aCount; ++i, ++aBeg )
		*aBeg = aVal;
}

//--    d: cleanup                          ///{{{2///////////////////////////

#undef FLUXTMP_DEBUG_CHECK_ORDER_

FLUX_DIAGNOSTIC_STATE_POP_OR_DEFAULT(CLASS_MEMACCESS)

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
