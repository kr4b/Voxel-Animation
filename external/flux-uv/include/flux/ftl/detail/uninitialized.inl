/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- uninitialialized_*() implementation
 *
 * TODO: use nothrow tag dispatch on Copy::enact() to avoid exception handling
 *    if target is nothrow_copy_constructible.
 */
/*-***************************************************************** -}}}1- */

#include <flux/std/assert.hpp>

#include <flux/compat/restrict.hpp>
#include <flux/compat/constexpr.hpp>
#include <flux/compat/to_address.hpp>
#include <flux/compat/type_traits.hpp>
#include <flux/compat/uninitialized.hpp>

#include <flux/ftl/meta_bool.hpp>
#include <flux/ftl/is_bitwise.hpp>
#include <flux/ftl/contiguous_iterator.hpp>

#include <flux/util/diagnostics.hpp>

#include <memory>
#include <utility>
#include <iterator>

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
	struct UCopyCanMemcpy_
	{
		using In_ = typename std::iterator_traits<tInputIt>::value_type;
		using Of_ = typename std::iterator_traits<tForwardIt>::value_type;

		static constexpr bool value = ftl::meta::All<
			ftl::is_contiguous_iterator<tInputIt>,
			ftl::is_contiguous_iterator<tForwardIt>,
			std::is_same<In_, Of_>,
			ftl::is_bitwise_duplicable<In_>
		>::value;
	};
	template< typename tInputIt, typename tForwardIt >
	struct UMoveCanMemcpy_
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
	template< typename tInputIt, typename tForwardIt >
	struct URelocCanMemcpy_
	{
		using In_ = typename std::iterator_traits<tInputIt>::value_type;
		using Of_ = typename std::iterator_traits<tForwardIt>::value_type;

		static constexpr bool value = ftl::meta::All<
			ftl::is_contiguous_iterator<tInputIt>,
			ftl::is_contiguous_iterator<tForwardIt>,
			std::is_same<In_, Of_>,
			ftl::is_bitwise_relocatable<In_>
		>::value;
	};

	// noexcept predicates {{{3
	template< typename tInputIt, typename tType >
	struct IsNothrowUnCtor
	{
		using In_ = typename std::iterator_traits<tInputIt>::value_type;

		static constexpr bool value = std::is_nothrow_constructible<In_,tType const&>::value;
	};
	
	template< typename tInputIt, typename tForwardIt >
	struct IsNothrowUnCopy
	{
		using In_ = typename std::iterator_traits<tInputIt>::value_type;
		using Of_ = typename std::iterator_traits<tForwardIt>::value_type;
		
		static constexpr bool value = ftl::meta::Any<
			std::is_nothrow_constructible<Of_,In_ const&>,
			UCopyCanMemcpy_<tInputIt,tForwardIt>
		>::value;
	};
	template< typename tInputIt, typename tForwardIt >
	struct IsNothrowUnMove
	{
		using In_ = typename std::iterator_traits<tInputIt>::value_type;
		using Of_ = typename std::iterator_traits<tForwardIt>::value_type;
		
		static constexpr bool value = ftl::meta::Any<
			std::is_nothrow_constructible<Of_,In_&&>,
			UMoveCanMemcpy_<tInputIt,tForwardIt>
		>::value;
	};
	template< typename tInputIt, typename tForwardIt >
	struct IsNothrowUnReloc
	{
		using In_ = typename std::iterator_traits<tInputIt>::value_type;
		using Of_ = typename std::iterator_traits<tForwardIt>::value_type;

		//TODO: should this also require nothrow_destructible<>??? Or do we just
		//always assume that that is the case anyway?
		
		static constexpr bool value = ftl::meta::Any<
			std::is_nothrow_constructible<Of_,In_&&>,
			URelocCanMemcpy_<tInputIt,tForwardIt>
		>::value;
	};

	template< typename tForwardIt >
	struct IsNothrowDtor
	{
		using Ty_ = typename std::iterator_traits<tForwardIt>::value_type;
		static constexpr bool value = ftl::meta::Any<
			std::is_nothrow_destructible<Ty_>,
			ftl::is_bitwise_dismissible<Ty_>
		>::value;
	};
	template< typename tForwardIt >
	struct IsNothrowValueCtor
	{
		using Ty_ = typename std::iterator_traits<tForwardIt>::value_type;
		
		static constexpr bool value = ftl::meta::Any<
			std::is_nothrow_default_constructible<Ty_>,
			ftl::meta::All<
				ftl::is_contiguous_iterator<tForwardIt>,
				ftl::is_bitwise_conjurable<Ty_>
			>
		>::value;
	};
	template< typename tForwardIt >
	struct IsNothrowDefaultCtor
	{
		using Ty_ = typename std::iterator_traits<tForwardIt>::value_type;
		
		static constexpr bool value = ftl::meta::Any<
			std::is_nothrow_default_constructible<Ty_>,
			ftl::is_bitwise_awakeable<Ty_>
		>::value;
	};
}

//--    $ local                             ///{{{2///////////////////////////
namespace detail
{
	// Opportunistic debug checks {{{3
#	if FLUX_DEBUG
#		define FLUXTMP_DEBUG_CHECK_ORDER_(a, b) detail::debug_check_un_iter_order( a, b )

	template< typename tIterator > inline FLUX_CONSTEXPR_EX
	void debug_check_un_iter_order( tIterator aBeg, tIterator aEnd, std::random_access_iterator_tag )
	{
		FLUX_ASSERT( aBeg <= aEnd );
		(void)aBeg;
		(void)aEnd;
	}
	template< typename tIterator, class tCat > inline FLUX_CONSTEXPR_EX
	void debug_check_un_iter_order( tIterator, tIterator, tCat )
	{}

	template< typename tIterator > inline FLUX_CONSTEXPR_EX
	void debug_check_un_iter_order( tIterator aBeg, tIterator aEnd )
	{
		using Cat_ = typename std::iterator_traits<tIterator>::iterator_category;
		debug_check_un_iter_order( aBeg, aEnd, Cat_{} );
	}
#	else // !FLUX_DEBUG
#		define FLUXTMP_DEBUG_CHECK_ORDER_(a, b) /*EMPTY*/
#	endif // ~ FLUX_DEBUG
	
	// Helper: Uninitialized Copy {{{3
	template< bool >
	struct UninitializedCopy_
	{
		template< typename tInputIt, typename tForwardIt > static inline
		tForwardIt enact( tInputIt aBeg, tInputIt aEnd, tForwardIt aOut )
		{
			return std::uninitialized_copy( aBeg, aEnd, aOut );
		}
		template< class tAlloc, typename tInputIt, typename tForwardIt > static inline
		tForwardIt enact( tAlloc& FLUX_RESTRICT aAlloc, tInputIt aBeg, tInputIt aEnd, tForwardIt aOut, std::false_type )
		{
			using ATraits_ = std::allocator_traits<tAlloc>;

			tForwardIt cur = aOut;
			try
			{
				for( ; aBeg != aEnd; ++cur, ++aBeg )
					ATraits_::construct( aAlloc, compat::to_address(cur), *aBeg );
			}
			catch( ... )
			{
				ftl::destroy( aAlloc, aOut, cur );
				throw;
			}

			return cur;
		}
		template< class tAlloc, typename tInputIt, typename tForwardIt > static inline
		tForwardIt enact( tAlloc& FLUX_RESTRICT aAlloc, tInputIt aBeg, tInputIt aEnd, tForwardIt aOut, std::true_type ) noexcept
		{
			using ATraits_ = std::allocator_traits<tAlloc>;
			for( ; aBeg != aEnd; ++aOut, ++aBeg )
				ATraits_::construct( aAlloc, compat::to_address(aOut), *aBeg );
			return aOut;
		}


		template< typename tInputIt, typename tForwardIt > static inline
		std::pair<tInputIt,tForwardIt> enact_n( tInputIt aBeg, std::size_t aCount, tForwardIt aOut )
		{
			//return std::uninitialized_copy_n( aBeg, aCount, aOut );
			
			using Ty_ = typename std::iterator_traits<tForwardIt>::value_type;

			tForwardIt cur = aOut;
			try
			{
				for( std::size_t i = 0; i < aCount; ++i, ++cur, ++aBeg )
					new (compat::to_address(cur)) Ty_( *aBeg );
			}
			catch( ... )
			{
				ftl::destroy( aOut, cur );
				throw;
			}

			return std::make_pair( aBeg, cur );
		}
		template< class tAlloc, typename tInputIt, typename tForwardIt > static inline
		std::pair<tInputIt,tForwardIt> enact_n( tAlloc& FLUX_RESTRICT aAlloc,tInputIt aBeg, std::size_t aCount, tForwardIt aOut )
		{
			using ATraits_ = std::allocator_traits<tAlloc>;

			tForwardIt cur = aOut;
			try
			{
				for( std::size_t i = 0; i < aCount; ++i, ++cur, ++aBeg )
					ATraits_::construct( aAlloc, compat::to_address(cur), *aBeg );
			}
			catch( ... )
			{
				ftl::destroy( aOut, cur );
				throw;
			}

			return std::make_pair( aBeg, cur );
		}



		template< typename tInputIt, typename tForwardIt > static inline
		void enact_p( tInputIt& FLUX_RESTRICT aBeg, tInputIt aEnd, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnCopy<tInputIt,tForwardIt>::value))
		{
			using Ty_ = typename std::iterator_traits<tForwardIt>::value_type;
			
			for( ; aBeg != aEnd; ++aBeg, ++aOut )
				new (compat::to_address(aOut)) Ty_( *aBeg );
		}
		template< class tAlloc, typename tInputIt, typename tForwardIt > static inline
		void enact_p( tAlloc& FLUX_RESTRICT aAlloc, tInputIt& FLUX_RESTRICT aBeg, tInputIt aEnd, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnCopy<tInputIt,tForwardIt>::value))
		{
			using ATraits_ = std::allocator_traits<tAlloc>;
			
			for( ; aBeg != aEnd; ++aBeg, ++aOut )
				ATraits_::construct( aAlloc, compat::to_address(aOut), *aBeg );
		}

		template< typename tInputIt, typename tForwardIt > static inline
		void enact_pn( tInputIt& FLUX_RESTRICT aBeg, std::size_t aCount, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnCopy<tInputIt,tForwardIt>::value))
		{
			using Ty_ = typename std::iterator_traits<tForwardIt>::value_type;
			
			for( std::size_t i = 0; i < aCount; ++i, ++aBeg, ++aOut )
				new (compat::to_address(aOut)) Ty_( *aBeg );
		}
		template< class tAlloc, typename tInputIt, typename tForwardIt > static inline
		void enact_pn( tAlloc& FLUX_RESTRICT aAlloc, tInputIt& FLUX_RESTRICT aBeg, std::size_t aCount, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnCopy<tInputIt,tForwardIt>::value))
		{
			using ATraits_ = std::allocator_traits<tAlloc>;
			
			for( std::size_t i = 0; i < aCount; ++i, ++aBeg, ++aOut )
				ATraits_::construct( aAlloc, compat::to_address(aOut), *aBeg );
		}
	};
	template<>
	struct UninitializedCopy_<true> 
	{
		template< typename tInputIt, typename tForwardIt > static inline
		tForwardIt enact( tInputIt aBeg, tInputIt aEnd, tForwardIt aOut ) noexcept
		{
			if( aBeg != aEnd )
			{
				auto beg = to_pointer(aBeg);
				auto end = to_pointer(aEnd);
				auto out = to_pointer(aOut);
				std::memcpy( out, beg, (end-beg)*sizeof(*beg) );
				return aOut + (end-beg);
			}

			return aOut;
		}
		template< class tAlloc, typename tInputIt, typename tForwardIt, typename tBool > static inline
		tForwardIt enact( tAlloc& FLUX_RESTRICT, tInputIt aBeg, tInputIt aEnd, tForwardIt aOut, tBool ) noexcept
		{
			return enact( aBeg, aEnd, aOut );
		}


		template< typename tInputIt, typename tForwardIt > static inline
		std::pair<tInputIt,tForwardIt> enact_n( tInputIt aBeg, std::size_t aCount, tForwardIt aOut )
		{
			if( aCount > 0 )
			{
				auto beg = to_pointer(aBeg);
				auto out = to_pointer(aOut);
				std::memcpy( out, beg, aCount*sizeof(*beg) );
			}
			return std::make_pair(aBeg + aCount, aOut + aCount);
		}
		template< class tAlloc, typename tInputIt, typename tForwardIt > static inline
		std::pair<tInputIt,tForwardIt> enact_n( tAlloc& FLUX_RESTRICT, tInputIt aBeg, std::size_t aCount, tForwardIt aOut )
		{
			return enact_n( aBeg, aCount, aOut );
		}


		template< typename tInputIt, typename tForwardIt > static inline
		void enact_p( tInputIt& FLUX_RESTRICT aBeg, tInputIt aEnd, tForwardIt& FLUX_RESTRICT aOut ) noexcept
		{
			if( aBeg != aEnd )
			{
				auto beg = to_pointer(aBeg);
				auto end = to_pointer(aEnd);
				auto out = to_pointer(aOut);
			
				auto const count = end-beg;
				std::memcpy( out, beg, count*sizeof(*beg) );

				aBeg = aEnd;
				aOut += count;
			}
		}
		template< class tAlloc, typename tInputIt, typename tForwardIt > static inline
		void enact_p( tAlloc& FLUX_RESTRICT, tInputIt& FLUX_RESTRICT aBeg, tInputIt aEnd, tForwardIt& FLUX_RESTRICT aOut ) noexcept
		{
			enact_p( aBeg, aEnd, aOut );
		}
		template< typename tInputIt, typename tForwardIt > static inline
		void enact_pn( tInputIt& FLUX_RESTRICT aBeg, std::size_t aCount, tForwardIt& FLUX_RESTRICT aOut ) noexcept
		{
			if( aCount > 0 )
			{
				auto beg = to_pointer(aBeg);
				auto out = to_pointer(aOut);
				std::memcpy( out, beg, aCount*sizeof(*beg) );

				aBeg += aCount;
				aOut += aCount;
			}
		}
		template< class tAlloc, typename tInputIt, typename tForwardIt > static inline
		void enact_pn( tAlloc& FLUX_RESTRICT, tInputIt& FLUX_RESTRICT aBeg, std::size_t aCount, tForwardIt& FLUX_RESTRICT aOut ) noexcept
		{
			enact_pn( aBeg, aCount, aOut );
		}
	};

	// Helper: Uninitialized Move {{{3
	template< bool >
	struct UninitializedMove_
	{
		template< typename tInputIt, typename tForwardIt > static inline
		tForwardIt enact( tInputIt aBeg, tInputIt aEnd, tForwardIt aOut )
		{
			return compat::uninitialized_move( aBeg, aEnd, aOut );
		}
		template< class tAlloc, typename tInputIt, typename tForwardIt > static inline
		tForwardIt enact( tAlloc& FLUX_RESTRICT aAlloc, tInputIt aBeg, tInputIt aEnd, tForwardIt aOut, std::false_type )
		{
			using ATraits_ = std::allocator_traits<tAlloc>;

			tForwardIt cur = aOut;
			try
			{
				for( ; aBeg != aEnd; ++cur, ++aBeg )
					ATraits_::construct( aAlloc, compat::to_address(cur), std::move(*aBeg) );
			}
			catch( ... )
			{
				ftl::destroy( aAlloc, aOut, cur );
				throw;
			}

			return cur;
		}
		template< typename tInputIt, typename tForwardIt > static inline
		std::pair<tInputIt,tForwardIt> enact_n( tInputIt aBeg, std::size_t aCount, tForwardIt aOut )
		{
			return compat::uninitialized_move_n( aBeg, aCount, aOut );
		}
		template< class tAlloc, typename tInputIt, typename tForwardIt > static inline
		tForwardIt enact( tAlloc& FLUX_RESTRICT aAlloc, tInputIt aBeg, tInputIt aEnd, tForwardIt aOut, std::true_type ) noexcept
		{
			using ATraits_ = std::allocator_traits<tAlloc>;

			for( ; aBeg != aEnd; ++aOut, ++aBeg )
				ATraits_::construct( aAlloc, compat::to_address(aOut), std::move(*aBeg) );

			return aOut;
		}

		template< class tAlloc, typename tInputIt, typename tForwardIt > static inline
		std::pair<tInputIt,tForwardIt> enact_n( tAlloc& FLUX_RESTRICT aAlloc, tInputIt aBeg, std::size_t aCount, tForwardIt aOut, std::false_type )
		{
			using ATraits_ = std::allocator_traits<tAlloc>;

			tForwardIt cur = aOut;
			try
			{
				for( std::size_t i = 0; i < aCount; ++i, ++cur, ++aBeg )
					ATraits_::construct( aAlloc, compat::to_address(cur), std::move(*aBeg) );
			}
			catch( ... )
			{
				ftl::destroy( aOut, cur );
				throw;
			}

			return std::make_pair( aBeg, cur );
		}
		template< class tAlloc, typename tInputIt, typename tForwardIt > static inline
		std::pair<tInputIt,tForwardIt> enact_n( tAlloc& FLUX_RESTRICT aAlloc, tInputIt aBeg, std::size_t aCount, tForwardIt aOut, std::true_type ) noexcept
		{
			using ATraits_ = std::allocator_traits<tAlloc>;

			for( std::size_t i = 0; i < aCount; ++i, ++aOut, ++aBeg )
				ATraits_::construct( aAlloc, compat::to_address(aOut), std::move(*aBeg) );

			return std::make_pair( aBeg, aOut );
		}

		template< typename tInputIt, typename tForwardIt > static inline
		void enact_p( tInputIt& FLUX_RESTRICT aBeg, tInputIt aEnd, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnMove<tInputIt,tForwardIt>::value))
		{
			using Ty_ = typename std::iterator_traits<tForwardIt>::value_type;
			
			for( ; aBeg != aEnd; ++aBeg, ++aOut )
				new (compat::to_address(aOut)) Ty_( std::move(*aBeg) );
		}
		template< typename tInputIt, typename tForwardIt > static inline
		void enact_pn( tInputIt& FLUX_RESTRICT aBeg, std::size_t aCount, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnMove<tInputIt,tForwardIt>::value))
		{
			using Ty_ = typename std::iterator_traits<tForwardIt>::value_type;
			
			for( std::size_t i = 0; i < aCount; ++i, ++aBeg, ++aOut )
				new (compat::to_address(aOut)) Ty_( std::move(*aBeg) );
		}

		template< class tAlloc, typename tInputIt, typename tForwardIt > static inline
		void enact_p( tAlloc& FLUX_RESTRICT aAlloc, tInputIt& FLUX_RESTRICT aBeg, tInputIt aEnd, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnMove<tInputIt,tForwardIt>::value))
		{
			using ATraits_ = std::allocator_traits<tAlloc>;
			
			for( ; aBeg != aEnd; ++aBeg, ++aOut )
				ATraits_::construct( aAlloc, compat::to_address(aOut), std::move(*aBeg) );
		}
		template< class tAlloc, typename tInputIt, typename tForwardIt > static inline
		void enact_pn( tAlloc& FLUX_RESTRICT aAlloc, tInputIt& FLUX_RESTRICT aBeg, std::size_t aCount, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnMove<tInputIt,tForwardIt>::value))
		{
			using ATraits_ = std::allocator_traits<tAlloc>;
			
			for( std::size_t i = 0; i < aCount; ++i, ++aBeg, ++aOut )
				ATraits_::construct( aAlloc, compat::to_address(aOut), std::move(*aBeg) );
		}
	};
	template<>
	struct UninitializedMove_<true> 
	{
		template< typename tInputIt, typename tForwardIt > static inline
		tForwardIt enact( tInputIt aBeg, tInputIt aEnd, tForwardIt aOut ) noexcept
		{
			if( aBeg != aEnd )
			{
				auto beg = to_pointer(aBeg);
				auto end = to_pointer(aEnd);
				auto out = to_pointer(aOut);
				std::memcpy( out, beg, (end-beg)*sizeof(*beg) );
				return aOut + (end-beg);
			}
			
			return aOut;
		}
		template< class tAlloc, typename tInputIt, typename tForwardIt, typename tBool > static inline
		tForwardIt enact( tAlloc& FLUX_RESTRICT, tInputIt aBeg, tInputIt aEnd, tForwardIt aOut, tBool ) noexcept
		{
			return enact( aBeg, aEnd, aOut );
		}
		template< typename tInputIt, typename tForwardIt > static inline
		std::pair<tInputIt,tForwardIt> enact_n( tInputIt aBeg, std::size_t aCount, tForwardIt aOut )
		{
			if( aCount > 0 )
			{
				auto beg = to_pointer(aBeg);
				auto out = to_pointer(aOut);
				std::memcpy( out, beg, aCount*sizeof(*beg) );
			}
			return std::make_pair(aBeg + aCount, aOut + aCount);
		}
		template< class tAlloc, typename tInputIt, typename tForwardIt, typename tBool > static inline
		std::pair<tInputIt,tForwardIt> enact_n( tAlloc& FLUX_RESTRICT, tInputIt aBeg, std::size_t aCount, tForwardIt aOut, tBool )
		{
			return enact_n( aBeg, aCount, aOut );
		}

		template< typename tInputIt, typename tForwardIt > static inline
		void enact_p( tInputIt& FLUX_RESTRICT aBeg, tInputIt aEnd, tForwardIt& FLUX_RESTRICT aOut ) noexcept
		{
			if( aBeg != aEnd )
			{
				auto beg = to_pointer(aBeg);
				auto end = to_pointer(aEnd);
				auto out = to_pointer(aOut);
			
				auto const count = end-beg;
				std::memcpy( out, beg, count*sizeof(*beg) );

				aBeg = aEnd;
				aOut += count;
			}
		}
		template< class tAlloc, typename tInputIt, typename tForwardIt > static inline
		void enact_p( tAlloc& FLUX_RESTRICT, tInputIt& FLUX_RESTRICT aBeg, tInputIt aEnd, tForwardIt& FLUX_RESTRICT aOut ) noexcept
		{
			enact_p( aBeg, aEnd, aOut );
		}
		template< typename tInputIt, typename tForwardIt > static inline
		void enact_pn( tInputIt& FLUX_RESTRICT aBeg, std::size_t aCount, tForwardIt& FLUX_RESTRICT aOut ) noexcept
		{
			if( aCount > 0 )
			{
				auto beg = to_pointer(aBeg);
				auto out = to_pointer(aOut);
				std::memcpy( out, beg, aCount*sizeof(*beg) );

				aBeg += aCount;
				aOut += aCount;
			}
		}
		template< class tAlloc, typename tInputIt, typename tForwardIt > static inline
		void enact_pn( tAlloc& FLUX_RESTRICT, tInputIt& FLUX_RESTRICT aBeg, std::size_t aCount, tForwardIt& FLUX_RESTRICT aOut ) noexcept
		{
			enact_pn( aBeg, aCount, aOut );
		}
	};

	// Helper: Uninitialized MoveIfNoexcept {{{3
	template< bool >
	struct UninitializedMoveIfNoexcept_
	{
		template< typename tFwdIn, typename tFwdOut > static inline
		tFwdOut enact( tFwdIn aBeg, tFwdIn aEnd, tFwdOut aOut )
		{
			return ftl::uninitialized_copy( aBeg, aEnd, aOut );
		}
		template< class tAlloc, typename tFwdIn, typename tFwdOut > static inline
		tFwdOut enact( tAlloc& FLUX_RESTRICT aAlloc, tFwdIn aBeg, tFwdIn aEnd, tFwdOut aOut )
		{
			return ftl::uninitialized_copy( aAlloc, aBeg, aEnd, aOut );
		}

		template< typename tFwdIn, typename tFwdOut > static inline
		std::pair<tFwdIn,tFwdOut> enact_n( tFwdIn aBeg, std::size_t aCount, tFwdOut aOut )
		{
			return ftl::uninitialized_copy_n( aBeg, aCount, aOut );
		}
		template< class tAlloc, typename tFwdIn, typename tFwdOut > static inline
		std::pair<tFwdIn,tFwdOut> enact_n( tAlloc& FLUX_RESTRICT aAlloc, tFwdIn aBeg, std::size_t aCount, tFwdOut aOut )
		{
			return ftl::uninitialized_copy_n( aAlloc, aBeg, aCount, aOut );
		}

		template< typename tFwdIn, typename tFwdOut > static inline
		void enact_p( tFwdIn& FLUX_RESTRICT aBeg, tFwdIn aEnd, tFwdOut& FLUX_RESTRICT aOut )
		{
			ftl::uninitialized_copy_p( aBeg, aEnd, aOut );
		}
		template< class tAlloc, typename tFwdIn, typename tFwdOut > static inline
		void enact_p( tAlloc& FLUX_RESTRICT aAlloc, tFwdIn& FLUX_RESTRICT aBeg, tFwdIn aEnd, tFwdOut& FLUX_RESTRICT aOut )
		{
			ftl::uninitialized_copy_p( aAlloc, aBeg, aEnd, aOut );
		}

		template< typename tFwdIn, typename tFwdOut > static inline
		void enact_pn( tFwdIn& FLUX_RESTRICT aBeg, std::size_t aCount, tFwdOut& FLUX_RESTRICT aOut )
		{
			ftl::uninitialized_copy_pn( aBeg, aCount, aOut );
		}
		template< class tAlloc, typename tFwdIn, typename tFwdOut > static inline
		void enact_pn( tAlloc& FLUX_RESTRICT aAlloc, tFwdIn& FLUX_RESTRICT aBeg, std::size_t aCount, tFwdOut& FLUX_RESTRICT aOut )
		{
			ftl::uninitialized_copy_pn( aAlloc, aBeg, aCount, aOut );
		}
	};

	template<>
	struct UninitializedMoveIfNoexcept_<true>
	{
		template< typename tFwdIn, typename tFwdOut > static inline
		tFwdOut enact( tFwdIn aBeg, tFwdIn aEnd, tFwdOut aOut ) noexcept
		{
			return ftl::uninitialized_move( aBeg, aEnd, aOut );
		}
		template< class tAlloc, typename tFwdIn, typename tFwdOut > static inline
		tFwdOut enact( tAlloc& FLUX_RESTRICT aAlloc, tFwdIn aBeg, tFwdIn aEnd, tFwdOut aOut ) noexcept
		{
			return ftl::uninitialized_move( aAlloc, aBeg, aEnd, aOut );
		}

		template< typename tFwdIn, typename tFwdOut > static inline
		std::pair<tFwdIn,tFwdOut> enact_n( tFwdIn aBeg, std::size_t aCount, tFwdOut aOut ) noexcept
		{
			return ftl::uninitialized_move_n( aBeg, aCount, aOut );
		}
		template< class tAlloc, typename tFwdIn, typename tFwdOut > static inline
		std::pair<tFwdIn,tFwdOut> enact_n( tAlloc& FLUX_RESTRICT aAlloc, tFwdIn aBeg, std::size_t aCount, tFwdOut aOut ) noexcept
		{
			return ftl::uninitialized_move_n( aAlloc, aBeg, aCount, aOut );
		}

		template< typename tFwdIn, typename tFwdOut > static inline
		void enact_p( tFwdIn& FLUX_RESTRICT aBeg, tFwdIn aEnd, tFwdOut& FLUX_RESTRICT aOut ) noexcept
		{
			ftl::uninitialized_move_p( aBeg, aEnd, aOut );
		}
		template< class tAlloc, typename tFwdIn, typename tFwdOut > static inline
		void enact_p( tAlloc& FLUX_RESTRICT aAlloc, tFwdIn& FLUX_RESTRICT aBeg, tFwdIn aEnd, tFwdOut& FLUX_RESTRICT aOut ) noexcept
		{
			ftl::uninitialized_move_p( aAlloc, aBeg, aEnd, aOut );
		}

		template< typename tFwdIn, typename tFwdOut > static inline
		void enact_pn( tFwdIn& FLUX_RESTRICT aBeg, std::size_t aCount, tFwdOut& FLUX_RESTRICT aOut ) noexcept
		{
			ftl::uninitialized_move_pn( aBeg, aCount, aOut );
		}
		template< class tAlloc, typename tFwdIn, typename tFwdOut > static inline
		void enact_pn( tAlloc& FLUX_RESTRICT aAlloc, tFwdIn& FLUX_RESTRICT aBeg, std::size_t aCount, tFwdOut& FLUX_RESTRICT aOut ) noexcept
		{
			ftl::uninitialized_move_pn( aAlloc, aBeg, aCount, aOut );
		}
	};

	// Helper: Uninitialized Relocate {{{3
	template< bool >
	struct UninitializedRelocate_
	{
		template< typename tFwdIn, typename tFwdOut > static inline
		tFwdOut enact( tFwdIn aBeg, tFwdIn aEnd, tFwdOut aOut ) noexcept((detail::IsNothrowUnReloc<tFwdIn,tFwdOut>::value))
		{
			auto ret = ftl::uninitialized_move( aBeg, aEnd, aOut );
			ftl::destroy( aBeg, aEnd );
			return ret;
		}
		template< class tAlloc, typename tFwdIn, typename tFwdOut > static inline
		tFwdOut enact( tAlloc& aAlloc, tFwdIn aBeg, tFwdIn aEnd, tFwdOut aOut ) noexcept((detail::IsNothrowUnReloc<tFwdIn,tFwdOut>::value))
		{
			auto ret = ftl::uninitialized_move( aAlloc, aBeg, aEnd, aOut );
			ftl::destroy( aAlloc, aBeg, aEnd );
			return ret;
		}

		template< typename tFwdIn, typename tFwdOut > static inline
		std::pair<tFwdIn,tFwdOut> enact_n( tFwdIn aBeg, std::size_t aCount, tFwdOut aOut )
		{
			auto ret = ftl::uninitialized_move_n( aBeg, aCount, aOut );
			ftl::destroy_n( aBeg, aCount );
			return ret;
		}
		template< class tAlloc, typename tFwdIn, typename tFwdOut > static inline
		std::pair<tFwdIn,tFwdOut> enact_n( tAlloc& FLUX_RESTRICT aAlloc, tFwdIn aBeg, std::size_t aCount, tFwdOut aOut )
		{
			auto ret = ftl::uninitialized_move_n( aAlloc, aBeg, aCount, aOut );
			ftl::destroy_n( aAlloc, aBeg, aCount );
			return ret;
		}

		
		template< typename tInputIt, typename tForwardIt > static inline
		void enact_p( tInputIt& FLUX_RESTRICT aBeg, tInputIt aEnd, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnReloc<tInputIt,tForwardIt>::value))
		{
			using InTy_ = typename std::iterator_traits<tInputIt>::value_type;
			using OfTy_ = typename std::iterator_traits<tForwardIt>::value_type;

			for( ; aBeg != aEnd; ++aBeg )
			{
				InTy_& in = *aBeg;
				::new (compat::to_address(aOut)) OfTy_(std::move(in));
				++aOut;
				destroy_at( &in );
			}
		}
		template< class tAlloc, typename tInputIt, typename tForwardIt > static inline
		void enact_p( tAlloc& FLUX_RESTRICT aAlloc, tInputIt& FLUX_RESTRICT aBeg, tInputIt aEnd, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnReloc<tInputIt,tForwardIt>::value))
		{
			using ATraits_ = std::allocator_traits<tAlloc>;
			using InTy_ = typename std::iterator_traits<tInputIt>::value_type;

			for( ; aBeg != aEnd; ++aBeg )
			{
				InTy_& in = *aBeg;
				ATraits_::construct( aAlloc, compat::to_address(aOut), std::move(in) );
				++aOut;
				destroy_at( aAlloc, &in );
			}
		}

		template< typename tInputIt, typename tForwardIt > static inline
		void enact_pn( tInputIt& FLUX_RESTRICT aBeg, std::size_t aCount, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnReloc<tInputIt,tForwardIt>::value))
		{
			using InTy_ = typename std::iterator_traits<tInputIt>::value_type;
			using OfTy_ = typename std::iterator_traits<tForwardIt>::value_type;

			for( std::size_t i = 0; i < aCount; ++i, ++aBeg )
			{
				InTy_& in = *aBeg;
				::new (compat::to_address(aOut)) OfTy_(std::move(in));
				++aOut;
				//in.~InTy_(); // workaround for NVCC, which barfs on this.
				destroy_at( &in );
			}
		}
		template< class tAlloc, typename tInputIt, typename tForwardIt > static inline
		void enact_pn( tAlloc& FLUX_RESTRICT aAlloc, tInputIt& FLUX_RESTRICT aBeg, std::size_t aCount, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnReloc<tInputIt,tForwardIt>::value))
		{
			using ATraits_ = std::allocator_traits<tAlloc>;
			using InTy_ = typename std::iterator_traits<tInputIt>::value_type;

			for( std::size_t i = 0; i < aCount; ++i, ++aBeg )
			{
				InTy_& in = *aBeg;
				ATraits_::construct( aAlloc, compat::to_address(aOut), std::move(in) );
				++aOut;
				destroy_at( aAlloc, &in );
			}
		}
	};
	template<>
	struct UninitializedRelocate_<true> 
	{
		template< typename tInputIt, typename tForwardIt > static inline
		tForwardIt enact( tInputIt aBeg, tInputIt aEnd, tForwardIt aOut ) noexcept
		{
			if( aBeg != aEnd )
			{
				auto beg = to_pointer(aBeg);
				auto end = to_pointer(aEnd);
				auto out = to_pointer(aOut);
			
				std::memcpy( out, beg, (end-beg)*sizeof(*beg) );
				return aOut + (end-beg);
			}
			
			return aOut;
		}
		template< class tAlloc, typename tInputIt, typename tForwardIt > static inline
		tForwardIt enact( tAlloc& FLUX_RESTRICT, tInputIt aBeg, tInputIt aEnd, tForwardIt aOut ) noexcept
		{
			return enact( aBeg, aEnd, aOut );
		}

		template< typename tInputIt, typename tForwardIt > static inline
		std::pair<tInputIt,tForwardIt> enact_n( tInputIt aBeg, std::size_t aCount, tForwardIt aOut )
		{
			if( aCount > 0 )
			{
				auto beg = to_pointer(aBeg);
				auto out = to_pointer(aOut);
				std::memcpy( out, beg, aCount*sizeof(*beg) );
			}
			return std::make_pair(aBeg + aCount, aOut + aCount);
		}
		template< class tAlloc, typename tInputIt, typename tForwardIt > static inline
		std::pair<tInputIt,tForwardIt> enact_n( tAlloc& FLUX_RESTRICT, tInputIt aBeg, std::size_t aCount, tForwardIt aOut )
		{
			return enact_n( aBeg, aCount, aOut );
		}

		template< typename tInputIt, typename tForwardIt > static inline
		void enact_p( tInputIt& FLUX_RESTRICT aBeg, tInputIt aEnd, tForwardIt& FLUX_RESTRICT aOut ) noexcept
		{
			if( aBeg != aEnd )
			{
				auto beg = to_pointer(aBeg);
				auto end = to_pointer(aEnd);
				auto out = to_pointer(aOut);
			
				auto const count = end-beg;
				std::memcpy( out, beg, count*sizeof(*beg) );

				aBeg = aEnd;
				aOut += count;
			}
		}
		template< class tAlloc, typename tInputIt, typename tForwardIt > static inline
		void enact_p( tAlloc& FLUX_RESTRICT, tInputIt& FLUX_RESTRICT aBeg, tInputIt aEnd, tForwardIt& FLUX_RESTRICT aOut ) noexcept
		{
			enact_p( aBeg, aEnd, aOut );
		}


		template< typename tInputIt, typename tForwardIt > static inline
		void enact_pn( tInputIt& FLUX_RESTRICT aBeg, std::size_t aCount, tForwardIt& FLUX_RESTRICT aOut ) noexcept
		{
			if( aCount > 0 )
			{
				auto beg = to_pointer(aBeg);
				auto out = to_pointer(aOut);
				std::memcpy( out, beg, aCount*sizeof(*beg) );

				aBeg += aCount;
				aOut += aCount;
			}
		}
		template< class tAlloc, typename tInputIt, typename tForwardIt > static inline
		void enact_pn( tAlloc& FLUX_RESTRICT, tInputIt& FLUX_RESTRICT aBeg, std::size_t aCount, tForwardIt& FLUX_RESTRICT aOut ) noexcept
		{
			enact_pn( aBeg, aCount, aOut );
		}
	};

	// Helper: Uninitialized RelocateIfNoexcept {{{3
	template< bool >
	struct UninitializedRelocateIfNoexceptImpl_
	{
		template< typename tFwdIn, typename tFwdOut > static inline
		void enact_p( tFwdIn& FLUX_RESTRICT aBeg, tFwdIn aEnd, tFwdOut& FLUX_RESTRICT aOut ) //TODO: noexcept if noexcept-copy-construct
		{
			auto temp = aBeg;
			ftl::uninitialized_copy_p( temp, aEnd, aOut );
			ftl::destroy( aBeg, aEnd );
			aBeg = temp;
		}
		template< class tAlloc, typename tFwdIn, typename tFwdOut > static inline
		void enact_p( tAlloc& FLUX_RESTRICT aAlloc, tFwdIn& FLUX_RESTRICT aBeg, tFwdIn aEnd, tFwdOut& FLUX_RESTRICT aOut ) //TODO: noexcept if noexcept-copy-construct
		{
			auto temp = aBeg;
			ftl::uninitialized_copy_p( aAlloc, temp, aEnd, aOut );
			ftl::destroy( aAlloc, aBeg, aEnd );
			aBeg = temp;
		}

		template< typename tFwdIn, typename tFwdOut > static inline
		void enact_pn( tFwdIn& FLUX_RESTRICT aBeg, std::size_t aCount, tFwdOut& FLUX_RESTRICT aOut ) //TODO: noexcept if noexcept-copy-construct
		{
			auto temp = aBeg;
			ftl::uninitialized_copy_pn( temp, aCount, aOut );
			aBeg = ftl::destroy_n( aBeg, aCount );
		}
		template< class tAlloc, typename tFwdIn, typename tFwdOut > static inline
		void enact_pn( tAlloc& FLUX_RESTRICT aAlloc, tFwdIn& FLUX_RESTRICT aBeg, std::size_t aCount, tFwdOut& FLUX_RESTRICT aOut ) //TODO: noexcept if noexcept-copy-construct
		{
			auto temp = aBeg;
			ftl::uninitialized_copy_pn( aAlloc, temp, aCount, aOut );
			aBeg = ftl::destroy_n( aAlloc, aBeg, aCount );
		}
	};
	template<>
	struct UninitializedRelocateIfNoexceptImpl_<true>
		: UninitializedRelocate_<false>
	{};
	
	template< bool, typename tInIt, typename tOutIt >
	struct UninitializedRelocateIfNoexcept_
		: UninitializedRelocateIfNoexceptImpl_<detail::IsNothrowUnMove<tInIt,tOutIt>::value>
	{
		template< typename tFwdIn, typename tFwdOut > static inline
		tFwdOut enact( tFwdIn aBeg, tFwdIn aEnd, tFwdOut aOut ) noexcept((detail::IsNothrowUnReloc<tFwdIn,tFwdOut>::value))
		{
			auto ret = ftl::uninitialized_move_if_noexcept( aBeg, aEnd, aOut );
			ftl::destroy( aBeg, aEnd );
			return ret;
		}
		template< class tAlloc, typename tFwdIn, typename tFwdOut > static inline
		tFwdOut enact( tAlloc& FLUX_RESTRICT aAlloc, tFwdIn aBeg, tFwdIn aEnd, tFwdOut aOut ) noexcept((detail::IsNothrowUnReloc<tFwdIn,tFwdOut>::value))
		{
			auto ret = ftl::uninitialized_move_if_noexcept( aAlloc, aBeg, aEnd, aOut );
			ftl::destroy( aAlloc, aBeg, aEnd );
			return ret;
		}

		template< typename tFwdIn, typename tFwdOut > static inline
		std::pair<tFwdIn,tFwdOut> enact_n( tFwdIn aBeg, std::size_t aCount, tFwdOut aOut )
		{
			auto ret = ftl::uninitialized_move_if_noexcept_n( aBeg, aCount, aOut );
			ftl::destroy_n( aBeg, aCount );
			return ret;
		}
		template< class tAlloc, typename tFwdIn, typename tFwdOut > static inline
		std::pair<tFwdIn,tFwdOut> enact_n( tAlloc& FLUX_RESTRICT aAlloc, tFwdIn aBeg, std::size_t aCount, tFwdOut aOut )
		{
			auto ret = ftl::uninitialized_move_if_noexcept_n( aAlloc, aBeg, aCount, aOut );
			ftl::destroy_n( aAlloc, aBeg, aCount );
			return ret;
		}
	};

	template< typename tFwdIn, typename tFwdOut >
	struct UninitializedRelocateIfNoexcept_<true, tFwdIn, tFwdOut>
		: UninitializedRelocate_<true>
	{};

	// Helper: Value Construct {{{3
	template< bool >
	struct ValueConstruct_
	{
		template< typename tForwardIt > static inline
		void enact( tForwardIt aBeg, tForwardIt aEnd )
		{
			compat::uninitialized_value_construct( aBeg, aEnd );
		}
		template< class tAlloc, typename tForwardIt > static inline
		void enact( tAlloc& FLUX_RESTRICT aAlloc, tForwardIt aBeg, tForwardIt aEnd, std::false_type )
		{
			using ATraits_ = std::allocator_traits<tAlloc>;
			
			auto cur = aBeg;
			try
			{
				for( ; cur != aEnd; ++cur )
					ATraits_::construct( aAlloc, compat::to_address(cur) ); 
			}
			catch( ... )
			{
				destroy( aAlloc, aBeg, cur );
				throw;
			}
		}
		template< class tAlloc, typename tForwardIt > static inline
		void enact( tAlloc& FLUX_RESTRICT aAlloc, tForwardIt aBeg, tForwardIt aEnd, std::true_type ) noexcept
		{
			using ATraits_ = std::allocator_traits<tAlloc>;
			
			for( ; aBeg != aEnd; ++aBeg )
				ATraits_::construct( aAlloc, compat::to_address(aBeg) ); 
		}

		template< typename tForwardIt > static inline
		tForwardIt enact_n( tForwardIt aBeg, std::size_t aCount )
		{
			return compat::uninitialized_value_construct_n( aBeg, aCount );
		}
		template< class tAlloc, typename tForwardIt > static inline
		tForwardIt enact_n( tAlloc& FLUX_RESTRICT aAlloc, tForwardIt aBeg, std::size_t aCount, std::false_type )
		{
			using ATraits_ = std::allocator_traits<tAlloc>;
			
			auto cur = aBeg;
			try
			{
				for( std::size_t i = 0; i < aCount; ++i, ++cur )
					ATraits_::construct( aAlloc, compat::to_address(cur) ); 
			}
			catch( ... )
			{
				destroy( aAlloc, aBeg, cur );
				throw;
			}
			return cur;
		}
		template< class tAlloc, typename tForwardIt > static inline
		tForwardIt enact_n( tAlloc& FLUX_RESTRICT aAlloc, tForwardIt aBeg, std::size_t aCount, std::true_type ) noexcept
		{
			using ATraits_ = std::allocator_traits<tAlloc>;
			
			for( std::size_t i = 0; i < aCount; ++i, ++aBeg )
				ATraits_::construct( aAlloc, compat::to_address(aBeg) ); 

			return aBeg;
		}


		template< typename tForwardIt > static inline
		void enact_p( tForwardIt& FLUX_RESTRICT aBeg, tForwardIt aEnd ) noexcept((detail::IsNothrowValueCtor<tForwardIt>::value))
		{
			using Ty_ = typename std::iterator_traits<tForwardIt>::value_type;
			for( ; aBeg != aEnd; ++aBeg )
				new (compat::to_address(aBeg)) Ty_();
		}
		template< class tAlloc, typename tForwardIt > static inline
		void enact_p( tAlloc& FLUX_RESTRICT aAlloc, tForwardIt& FLUX_RESTRICT aBeg, tForwardIt aEnd ) noexcept((detail::IsNothrowValueCtor<tForwardIt>::value))
		{
			using ATraits_ = std::allocator_traits<tAlloc>;
			
			for( ; aBeg != aEnd; ++aBeg )
				ATraits_::construct( aAlloc, compat::to_address(aBeg) ); 
		}
		template< typename tForwardIt > static inline
		void enact_pn( tForwardIt& FLUX_RESTRICT aBeg, std::size_t aCount ) noexcept((detail::IsNothrowValueCtor<tForwardIt>::value))
		{
			using Ty_ = typename std::iterator_traits<tForwardIt>::value_type;
			
			for( std::size_t i = 0; i < aCount; ++i, ++aBeg )
				new (compat::to_address(aBeg)) Ty_();
		}
		template< class tAlloc, typename tForwardIt > static inline
		void enact_pn( tAlloc& FLUX_RESTRICT aAlloc, tForwardIt& FLUX_RESTRICT aBeg, std::size_t aCount ) noexcept((detail::IsNothrowValueCtor<tForwardIt>::value))
		{
			using ATraits_ = std::allocator_traits<tAlloc>;
			
			for( std::size_t i = 0; i < aCount; ++i, ++aBeg )
				ATraits_::construct( aAlloc, compat::to_address(aBeg) ); 
		}
	};
	template<>
	struct ValueConstruct_<true> 
	{
		template< typename tForwardIt > static inline
		void enact( tForwardIt aBeg, tForwardIt aEnd ) noexcept
		{
			if( aBeg != aEnd )
			{
				auto beg = to_pointer(aBeg);
				auto end = to_pointer(aEnd);
				std::memset( beg, 0, (end-beg)*sizeof(*beg) );
			}
		}
		template< class tAlloc, typename tForwardIt, typename tBool > static inline
		void enact( tAlloc& FLUX_RESTRICT, tForwardIt aBeg, tForwardIt aEnd, tBool ) noexcept
		{
			enact( aBeg, aEnd );
		}

		template< typename tForwardIt > static inline
		tForwardIt enact_n( tForwardIt aBeg, std::size_t aCount ) noexcept
		{
			if( aCount > 0 )
			{
				auto beg = to_pointer(aBeg);
				std::memset( beg, 0, aCount*sizeof(*beg) );
			}
			return aBeg + aCount;
		}
		template< class tAlloc, typename tForwardIt, typename tBool > static inline
		tForwardIt enact_n( tAlloc& FLUX_RESTRICT, tForwardIt aBeg, std::size_t aCount, tBool ) noexcept
		{
			return enact_n( aBeg, aCount );
		}

		template< typename tForwardIt > static inline
		void enact_p( tForwardIt& FLUX_RESTRICT aBeg, tForwardIt aEnd ) noexcept
		{
			if( aBeg != aEnd )
			{
				auto beg = to_pointer(aBeg);
				auto end = to_pointer(aEnd);
				std::memset( beg, 0, (end-beg)*sizeof(*beg) );
				aBeg = aEnd;
			}
		}
		template< class tAlloc, typename tForwardIt > static inline
		void enact_p( tAlloc& FLUX_RESTRICT, tForwardIt& FLUX_RESTRICT aBeg, tForwardIt aEnd ) noexcept
		{
			enact_p( aBeg, aEnd );
		}

		template< typename tForwardIt > static inline
		void enact_pn( tForwardIt& FLUX_RESTRICT aBeg, std::size_t aCount ) noexcept
		{
			if( aCount > 0 )
			{
				auto beg = to_pointer(aBeg);
				std::memset( beg, 0, aCount*sizeof(*beg) );
				aBeg += aCount;
			}
		}
		template< class tAlloc, typename tForwardIt > static inline
		void enact_pn( tAlloc& FLUX_RESTRICT, tForwardIt& FLUX_RESTRICT aBeg, std::size_t aCount ) noexcept
		{
			enact_pn( aBeg, aCount );
		}
	};

	// Helper: Default Construct {{{3
	template< bool >
	struct DefaultConstruct_
	{
		template< typename tForwardIt > static inline
		void enact( tForwardIt aBeg, tForwardIt aEnd )
		{
			compat::uninitialized_default_construct( aBeg, aEnd );
		}
		template< class tAlloc, typename tForwardIt > static inline
		void enact( tAlloc& FLUX_RESTRICT aAlloc, tForwardIt aBeg, tForwardIt aEnd, std::false_type ) 
		{
			using ATraits_ = std::allocator_traits<tAlloc>;

			auto cur = aBeg;
			try
			{
				for( ; cur != aEnd; ++cur )
					ATraits_::construct( aAlloc, compat::to_address(cur) );
			}
			catch( ... )
			{
				destroy( aAlloc, aBeg, cur );
				throw;
			}
		}
		template< class tAlloc, typename tForwardIt > static inline
		void enact( tAlloc& FLUX_RESTRICT aAlloc, tForwardIt aBeg, tForwardIt aEnd, std::true_type ) 
		{
			using ATraits_ = std::allocator_traits<tAlloc>;
			for( ; aBeg != aEnd; ++aBeg )
				ATraits_::construct( aAlloc, compat::to_address(aBeg) );
		}
		template< typename tForwardIt, typename tTag > static inline
		tForwardIt enact_n( tForwardIt aBeg, std::size_t aCount, tTag )
		{
			return compat::uninitialized_default_construct_n( aBeg, aCount );
		}
		template< class tAlloc, typename tForwardIt, typename tTag > static inline
		tForwardIt enact_n( tAlloc& FLUX_RESTRICT aAlloc, tForwardIt aBeg, std::size_t aCount, tTag, std::false_type ) 
		{
			using ATraits_ = std::allocator_traits<tAlloc>;

			auto cur = aBeg;
			try
			{
				for( std::size_t i = 0; i < aCount; ++i, ++cur )
					ATraits_::construct( aAlloc, compat::to_address(cur) );
			}
			catch( ... )
			{
				destroy( aAlloc, aBeg, cur );
				throw;
			}
			return cur;
		}
		template< class tAlloc, typename tForwardIt, typename tTag > static inline
		tForwardIt enact_n( tAlloc& FLUX_RESTRICT aAlloc, tForwardIt aBeg, std::size_t aCount, tTag, std::true_type ) 
		{
			using ATraits_ = std::allocator_traits<tAlloc>;
			for( std::size_t i = 0; i < aCount; ++i, ++aBeg )
				ATraits_::construct( aAlloc, compat::to_address(aBeg) );
			return aBeg;
		}

		template< typename tForwardIt > static inline
		void enact_p( tForwardIt& FLUX_RESTRICT aBeg, tForwardIt aEnd ) noexcept((detail::IsNothrowDefaultCtor<tForwardIt>::value))
		{
			using Ty_ = typename std::iterator_traits<tForwardIt>::value_type;
			for( ; aBeg != aEnd; ++aBeg )
				new (compat::to_address(aBeg)) Ty_;
		}
		template< class tAlloc, typename tForwardIt > static inline
		void enact_p( tAlloc& FLUX_RESTRICT aAlloc, tForwardIt& FLUX_RESTRICT aBeg, tForwardIt aEnd ) noexcept((detail::IsNothrowDefaultCtor<tForwardIt>::value))
		{
			using ATraits_ = std::allocator_traits<tAlloc>;
			
			for( ; aBeg != aEnd; ++aBeg )
				ATraits_::construct( aAlloc, compat::to_address(aBeg) ); 
		}
		template< typename tForwardIt, typename tTag > static inline
		void enact_pn( tForwardIt& FLUX_RESTRICT aBeg, std::size_t aCount, tTag ) noexcept((detail::IsNothrowDefaultCtor<tForwardIt>::value))
		{
			using Ty_ = typename std::iterator_traits<tForwardIt>::value_type;
			
			for( std::size_t i = 0; i < aCount; ++i, ++aBeg )
				new (compat::to_address(aBeg)) Ty_;
		}
		template< class tAlloc, typename tForwardIt, typename tTag > static inline
		void enact_pn( tAlloc& FLUX_RESTRICT aAlloc, tForwardIt& FLUX_RESTRICT aBeg, std::size_t aCount, tTag ) noexcept((detail::IsNothrowDefaultCtor<tForwardIt>::value))
		{
			using ATraits_ = std::allocator_traits<tAlloc>;
			
			for( std::size_t i = 0; i < aCount; ++i, ++aBeg )
				ATraits_::construct( aAlloc, compat::to_address(aBeg) ); 
		}
	};
	template<>
	struct DefaultConstruct_<true> 
	{
		template< typename tForwardIt > static inline
		void enact( tForwardIt, tForwardIt ) noexcept
		{}
		template< class tAlloc, typename tForwardIt, typename tBool > static inline
		void enact( tAlloc& FLUX_RESTRICT, tForwardIt, tForwardIt, tBool ) noexcept
		{}
		template< typename tForwardIt > static inline
		tForwardIt enact_n( tForwardIt aBeg, std::size_t aCount, std::random_access_iterator_tag ) noexcept
		{
			return aBeg + aCount;
		}
		template< typename tForwardIt, typename tTag > static inline
		tForwardIt enact_n( tForwardIt aBeg, std::size_t aCount, tTag ) noexcept
		{
			for( std::size_t i = 0; i < aCount; ++i, ++aBeg )
				;

			return aBeg;
		}
		template< class tAlloc, typename tForwardIt, typename tTag, typename tBool > static inline
		tForwardIt enact_n( tAlloc& FLUX_RESTRICT, tForwardIt aBeg, std::size_t aCount, tTag, tBool ) noexcept
		{
			return  enact_n( aBeg, aCount, tTag{} );
		}

		template< typename tForwardIt > static inline
		void enact_p( tForwardIt& FLUX_RESTRICT aBeg, tForwardIt aEnd ) noexcept
		{
			aBeg = aEnd;
		}
		template< class tAlloc, typename tForwardIt > static inline
		void enact_p( tAlloc& FLUX_RESTRICT, tForwardIt& FLUX_RESTRICT aBeg, tForwardIt aEnd ) noexcept
		{
			aBeg = aEnd;
		}

		template< typename tForwardIt > static inline
		void enact_pn( tForwardIt& FLUX_RESTRICT aBeg, std::size_t aCount, std::random_access_iterator_tag ) noexcept
		{
			aBeg += aCount;
		}
		template< typename tForwardIt, typename tTag > static inline
		void enact_pn( tForwardIt& FLUX_RESTRICT aBeg, std::size_t aCount, tTag ) noexcept
		{
			for( std::size_t i = 0; i < aCount; ++i, ++aBeg )
				;

			return aBeg;
		}

		template< class tAlloc, typename tForwardIt, typename tTag > static inline
		void enact_pn( tAlloc& FLUX_RESTRICT, tForwardIt& FLUX_RESTRICT aBeg, std::size_t aCount, tTag ) noexcept
		{
			enact_pn( aBeg, aCount, tTag{} );
		}
	};

	// Helper: Uninitialized Fill {{{3
	struct UninitializedFill_
	{
#		if FLUX_STDQUIRK_CXX11_OLD_UNINITIALIZED
		template< typename tForwardIt, typename tType > static inline
		tForwardIt enact_n( tForwardIt aBeg, std::size_t aCount, tType const& aValue, std::false_type )
		{
			auto cur = aBeg;
			try
			{
				for( std::size_t i = 0; i < aCount; ++i, ++cur )
					new (compat::to_address(cur)) tType(aValue);
			}
			catch( ... )
			{
				destroy( aBeg, cur );
				throw;
			}
			return cur;
		}
		template< typename tForwardIt, typename tType > static inline
		tForwardIt enact_n( tForwardIt aBeg, std::size_t aCount, tType const& aValue, std::true_type ) noexcept
		{
			for( std::size_t i = 0; i < aCount; ++i, ++aBeg )
				new (compat::to_address(aBeg)) tType(aValue);
			return aBeg;
		}
#		endif // ~ STDQUIRK_CXX11_OLD_UNINITIALZIED

		template< class tAlloc, typename tForwardIt, typename tType > static inline
		void enact( tAlloc& FLUX_RESTRICT aAlloc, tForwardIt aBeg, tForwardIt aEnd, tType const& FLUX_RESTRICT aValue, std::false_type )
		{
			using ATraits_ = std::allocator_traits<tAlloc>;

			auto cur = aBeg;
			try
			{
				for( ; cur != aEnd; ++cur )
					ATraits_::construct( aAlloc, compat::to_address(cur), aValue );
			}
			catch( ... )
			{
				destroy( aBeg, cur );
				throw;
			}
		}
		template< class tAlloc, typename tForwardIt, typename tType > static inline
		void enact( tAlloc& FLUX_RESTRICT aAlloc, tForwardIt aBeg, tForwardIt aEnd, tType const& FLUX_RESTRICT aValue, std::true_type ) noexcept
		{
			using ATraits_ = std::allocator_traits<tAlloc>;
			for( ; aBeg != aEnd; ++aBeg )
				ATraits_::construct( aAlloc, compat::to_address(aBeg), aValue );
		}

		template< class tAlloc, typename tForwardIt, typename tType > static inline
		tForwardIt enact_n( tAlloc& FLUX_RESTRICT aAlloc, tForwardIt aBeg, std::size_t aCount, tType const& FLUX_RESTRICT aValue, std::false_type )
		{
			using ATraits_ = std::allocator_traits<tAlloc>;

			auto cur = aBeg;
			try
			{
				for( std::size_t i = 0; i < aCount; ++i, ++cur )
					ATraits_::construct( aAlloc, compat::to_address(cur), aValue );
			}
			catch( ... )
			{
				destroy( aBeg, cur );
				throw;
			}
			return cur;
		}
		template< class tAlloc, typename tForwardIt, typename tType > static inline
		tForwardIt enact_n( tAlloc& FLUX_RESTRICT aAlloc, tForwardIt aBeg, std::size_t aCount, tType const& FLUX_RESTRICT aValue, std::true_type ) noexcept
		{
			using ATraits_ = std::allocator_traits<tAlloc>;
			for( std::size_t i = 0; i < aCount; ++i, ++aBeg )
				ATraits_::construct( aAlloc, compat::to_address(aBeg), aValue );
			return aBeg;
		}

		template< typename tForwardIt, typename tType > static inline
		void enact_p( tForwardIt& FLUX_RESTRICT aBeg, tForwardIt aEnd, tType const& FLUX_RESTRICT aValue ) noexcept((detail::IsNothrowUnCtor<tForwardIt,tType>::value))
		{
			using Ty_ = typename std::iterator_traits<tForwardIt>::value_type;
			for( ; aBeg != aEnd; ++aBeg )
				new (compat::to_address(aBeg)) Ty_( aValue );
		}
		template< class tAlloc, typename tForwardIt, typename tType > static inline
		void enact_p( tAlloc& FLUX_RESTRICT aAlloc, tForwardIt& FLUX_RESTRICT aBeg, tForwardIt aEnd, tType const& FLUX_RESTRICT aValue ) noexcept((detail::IsNothrowUnCtor<tForwardIt,tType>::value))
		{
			using ATraits_ = std::allocator_traits<tAlloc>;
			for( ; aBeg != aEnd; ++aBeg )
				ATraits_::construct( aAlloc, compat::to_address(aBeg), aValue );
		}

		template< typename tForwardIt, typename tType > static inline
		void enact_pn( tForwardIt& FLUX_RESTRICT aBeg, std::size_t aCount, tType const& FLUX_RESTRICT aValue ) noexcept((detail::IsNothrowUnCtor<tForwardIt,tType>::value))
		{
			using Ty_ = typename std::iterator_traits<tForwardIt>::value_type;
			for( std::size_t i = 0; i < aCount; ++i, ++aBeg )
				new (compat::to_address(aBeg)) Ty_( aValue );
		}
		template< class tAlloc, typename tForwardIt, typename tType > static inline
		void enact_pn( tAlloc& FLUX_RESTRICT aAlloc, tForwardIt& FLUX_RESTRICT aBeg, std::size_t aCount, tType const& FLUX_RESTRICT aValue ) noexcept((detail::IsNothrowUnCtor<tForwardIt,tType>::value))
		{
			using ATraits_ = std::allocator_traits<tAlloc>;
			for( std::size_t i = 0; i < aCount; ++i, ++aBeg )
				ATraits_::construct( aAlloc, compat::to_address(aBeg), aValue );
		}
	};

	// Helper: Destroy {{{3
	template< bool >
	struct Destroy_
	{
		template< typename tForwardIt > static inline
		void enact( tForwardIt aBeg, tForwardIt aEnd )
		{
			compat::destroy( aBeg, aEnd );
		}
		template< typename tForwardIt, typename tTag > static inline
		tForwardIt enact_n( tForwardIt aBeg, std::size_t aCount, tTag )
		{
			return compat::destroy_n( aBeg, aCount );
		}
		template< typename tType > static inline
		void enact_at( tType* FLUX_RESTRICT aPtr )
		{
			compat::destroy_at( aPtr );
		}

		template< class tAlloc, typename tForwardIt > static inline
		void enact( tAlloc& FLUX_RESTRICT aAlloc, tForwardIt aBeg, tForwardIt aEnd )
		{
			using ATraits_ = std::allocator_traits<tAlloc>;
			for( ; aBeg != aEnd; ++aBeg )
				ATraits_::destroy( aAlloc, compat::to_address(aBeg) );
		}
		template< class tAlloc, typename tForwardIt, typename tTag > static inline
		tForwardIt enact_n( tAlloc& FLUX_RESTRICT aAlloc, tForwardIt aBeg, std::size_t aCount, tTag )
		{
			using ATraits_ = std::allocator_traits<tAlloc>;
			for( std::size_t i = 0; i < aCount; ++i, ++aBeg )
				ATraits_::destroy( aAlloc, compat::to_address(aBeg) );

			return aBeg;
		}
		template< class tAlloc, typename tType > static inline
		void enact_at( tAlloc& FLUX_RESTRICT aAlloc, tType* FLUX_RESTRICT aPtr )
		{
			using ATraits_ = std::allocator_traits<tAlloc>;
			ATraits_::destroy( aAlloc, aPtr );
		}

	};
	template<>
	struct Destroy_<true> 
	{
		template< typename tForwardIt > static inline
		void enact( tForwardIt, tForwardIt ) noexcept
		{}
		template< typename tForwardIt > static inline
		tForwardIt enact_n( tForwardIt aBeg, std::size_t aCount, std::random_access_iterator_tag ) noexcept
		{
			return aBeg + aCount;
		}
		template< typename tForwardIt, typename tTag > static inline
		tForwardIt enact_n( tForwardIt aBeg, std::size_t aCount, tTag ) noexcept
		{
			for( std::size_t i = 0; i < aCount; ++i, ++aBeg )
				;

			return aBeg;
		}
		template< typename tType > static inline
		void enact_at( tType* ) noexcept
		{}


		template< class tAlloc, typename tForwardIt > static inline
		void enact( tAlloc& FLUX_RESTRICT, tForwardIt, tForwardIt ) noexcept
		{}
		template< class tAlloc, typename tForwardIt > static inline
		tForwardIt enact_n( tAlloc& FLUX_RESTRICT, tForwardIt aBeg, std::size_t aCount, std::random_access_iterator_tag ) noexcept
		{
			return aBeg + aCount;
		}
		template< class tAlloc, typename tForwardIt, typename tTag > static inline
		tForwardIt enact_n( tAlloc& FLUX_RESTRICT, tForwardIt aBeg, std::size_t aCount, tTag ) noexcept
		{
			for( std::size_t i = 0; i < aCount; ++i, ++aBeg )
				;

			return aBeg;
		}
		template< class tAlloc, typename tType > static inline
		void enact_at( tAlloc&, tType* ) noexcept
		{}
	};
};

//--    uninitialized_copy*()               ///{{{2///////////////////////////
template< typename tInputIt, typename tForwardIt > inline
tForwardIt uninitialized_copy( tInputIt aBeg, tInputIt aEnd, tForwardIt aOut )
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	return detail::UninitializedCopy_<
		detail::UCopyCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact( aBeg, aEnd, aOut );
}
template< class tAlloc, typename tInputIt, typename tForwardIt > inline
tForwardIt uninitialized_copy( tAlloc& FLUX_RESTRICT aAlloc, tInputIt aBeg, tInputIt aEnd, tForwardIt aOut ) noexcept((detail::IsNothrowUnCopy<tInputIt,tForwardIt>::value))
{
	using Nothrow_ = std::integral_constant<bool,detail::IsNothrowUnCopy<tInputIt,tForwardIt>::value>;

	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	return detail::UninitializedCopy_<
		detail::UCopyCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact( aAlloc, aBeg, aEnd, aOut, Nothrow_{} );
}

template< typename tInputIt, typename tForwardIt > inline
std::pair<tInputIt,tForwardIt> uninitialized_copy_n( tInputIt aBeg, std::size_t aCount, tForwardIt aOut )
{
	return detail::UninitializedCopy_<
		detail::UCopyCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_n( aBeg, aCount, aOut );
}
template< class tAlloc, typename tInputIt, typename tForwardIt > inline
std::pair<tInputIt,tForwardIt> uninitialized_copy_n( tAlloc& FLUX_RESTRICT aAlloc, tInputIt aBeg, std::size_t aCount, tForwardIt aOut )
{
	return detail::UninitializedCopy_<
		detail::UCopyCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_n( aAlloc, aBeg, aCount, aOut );
}

template< typename tInputIt, typename tForwardIt > inline
void uninitialized_copy_p( tInputIt& FLUX_RESTRICT aBeg, tInputIt aEnd, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnCopy<tInputIt,tForwardIt>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	detail::UninitializedCopy_<
		detail::UCopyCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_p( aBeg, aEnd, aOut );
}
template< class tAlloc, typename tInputIt, typename tForwardIt > inline
void uninitialized_copy_p( tAlloc& FLUX_RESTRICT aAlloc, tInputIt& FLUX_RESTRICT aBeg, tInputIt aEnd, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnCopy<tInputIt,tForwardIt>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	detail::UninitializedCopy_<
		detail::UCopyCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_p( aAlloc, aBeg, aEnd, aOut );
}

template< typename tInputIt, typename tForwardIt > inline
void uninitialized_copy_pn( tInputIt& FLUX_RESTRICT aBeg, std::size_t aCount, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnCopy<tInputIt,tForwardIt>::value))
{
	detail::UninitializedCopy_<
		detail::UCopyCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_pn( aBeg, aCount, aOut );
}
template< class tAlloc, typename tInputIt, typename tForwardIt > inline
void uninitialized_copy_pn( tAlloc& FLUX_RESTRICT aAlloc, tInputIt& FLUX_RESTRICT aBeg, std::size_t aCount, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnCopy<tInputIt,tForwardIt>::value))
{
	detail::UninitializedCopy_<
		detail::UCopyCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_pn( aAlloc, aBeg, aCount, aOut );
}


//--    uninitialized_move*()               ///{{{2///////////////////////////
template< typename tInputIt, typename tForwardIt > inline
tForwardIt uninitialized_move( tInputIt aBeg, tInputIt aEnd, tForwardIt aOut )
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	return detail::UninitializedMove_<
		detail::UMoveCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact( aBeg, aEnd, aOut );
}
template< typename tInputIt, typename tForwardIt > inline
std::pair<tInputIt,tForwardIt> uninitialized_move_n( tInputIt aBeg, std::size_t aCount, tForwardIt aOut )
{
	return detail::UninitializedMove_<
		detail::UMoveCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_n( aBeg, aCount, aOut );
}

template< class tAlloc, typename tInputIt, typename tForwardIt > inline
tForwardIt uninitialized_move( tAlloc& FLUX_RESTRICT aAlloc, tInputIt aBeg, tInputIt aEnd, tForwardIt aOut ) noexcept((detail::IsNothrowUnMove<tInputIt,tForwardIt>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	using Nothrow_ = std::integral_constant<bool, detail::IsNothrowUnMove<tInputIt,tForwardIt>::value>;
	return detail::UninitializedMove_<
		detail::UMoveCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact( aAlloc, aBeg, aEnd, aOut, Nothrow_{} );
}
template< class tAlloc, typename tInputIt, typename tForwardIt > inline
std::pair<tInputIt,tForwardIt> uninitialized_move_n( tAlloc& FLUX_RESTRICT aAlloc, tInputIt aBeg, std::size_t aCount, tForwardIt aOut )
{
	using Nothrow_ = std::integral_constant<bool, detail::IsNothrowUnMove<tInputIt,tForwardIt>::value>;
	return detail::UninitializedMove_<
		detail::UMoveCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_n( aAlloc, aBeg, aCount, aOut, Nothrow_{} );
}


template< typename tInputIt, typename tForwardIt > inline
void uninitialized_move_p( tInputIt& FLUX_RESTRICT aBeg, tInputIt aEnd, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnMove<tInputIt,tForwardIt>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	detail::UninitializedMove_<
		detail::UMoveCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_p( aBeg, aEnd, aOut );
}
template< typename tInputIt, typename tForwardIt > inline
void uninitialized_move_pn( tInputIt& FLUX_RESTRICT aBeg, std::size_t aCount, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnMove<tInputIt,tForwardIt>::value))
{
	detail::UninitializedMove_<
		detail::UMoveCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_pn( aBeg, aCount, aOut );
}

template< class tAlloc, typename tInputIt, typename tForwardIt > inline
void uninitialized_move_p( tAlloc& FLUX_RESTRICT aAlloc, tInputIt& FLUX_RESTRICT aBeg, tInputIt aEnd, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnMove<tInputIt,tForwardIt>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	detail::UninitializedMove_<
		detail::UMoveCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_p( aAlloc, aBeg, aEnd, aOut );
}
template< class tAlloc, typename tInputIt, typename tForwardIt > inline
void uninitialized_move_pn( tAlloc& FLUX_RESTRICT aAlloc, tInputIt& FLUX_RESTRICT aBeg, std::size_t aCount, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnMove<tInputIt,tForwardIt>::value))
{
	detail::UninitializedMove_<
		detail::UMoveCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_pn( aAlloc, aBeg, aCount, aOut );
}

//--    uninitialized_move_if_noexcept*()   ///{{{2///////////////////////////
template< typename tInputIt, typename tForwardIt > inline
tForwardIt uninitialized_move_if_noexcept( tInputIt aBeg, tInputIt aEnd, tForwardIt aOut ) noexcept((detail::IsNothrowUnMove<tInputIt,tForwardIt>::value))
{
	return detail::UninitializedMoveIfNoexcept_<
		detail::IsNothrowUnMove<tInputIt,tForwardIt>::value
	>::enact( aBeg, aEnd, aOut );
}
template< typename tInputIt, typename tForwardIt > inline
std::pair<tInputIt,tForwardIt> uninitialized_move_if_noexcept_n( tInputIt aBeg, std::size_t aCount, tForwardIt aOut )
{
	return detail::UninitializedMoveIfNoexcept_<
		detail::IsNothrowUnMove<tInputIt,tForwardIt>::value
	>::enact_n( aBeg, aCount, aOut );
}
template< class tAlloc, typename tInputIt, typename tForwardIt > inline
tForwardIt uninitialized_move_if_noexcept( tAlloc& FLUX_RESTRICT aAlloc, tInputIt aBeg, tInputIt aEnd, tForwardIt aOut ) noexcept((detail::IsNothrowUnMove<tInputIt,tForwardIt>::value))
{
	return detail::UninitializedMoveIfNoexcept_<
		detail::IsNothrowUnMove<tInputIt,tForwardIt>::value
	>::enact( aAlloc, aBeg, aEnd, aOut );
}
template< class tAlloc, typename tInputIt, typename tForwardIt > inline
std::pair<tInputIt,tForwardIt> uninitialized_move_if_noexcept_n( tAlloc& FLUX_RESTRICT aAlloc, tInputIt aBeg, std::size_t aCount, tForwardIt aOut )
{
	return detail::UninitializedMoveIfNoexcept_<
		detail::IsNothrowUnMove<tInputIt,tForwardIt>::value
	>::enact_n( aAlloc, aBeg, aCount, aOut );
}

template< typename tInputIt, typename tForwardIt > inline
void uninitialized_move_if_noexcept_p( tInputIt& FLUX_RESTRICT aBeg, tInputIt aEnd, tForwardIt& FLUX_RESTRICT aOut ) 
	noexcept((detail::IsNothrowUnMove<tInputIt,tForwardIt>::value))
{
	detail::UninitializedMoveIfNoexcept_<
		detail::IsNothrowUnMove<tInputIt,tForwardIt>::value
	>::enact_p( aBeg, aEnd, aOut );
}
template< typename tInputIt, typename tForwardIt > inline
void uninitialized_move_if_noexcept_pn( tInputIt& FLUX_RESTRICT aBeg, std::size_t aCount, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnMove<tInputIt,tForwardIt>::value))
{
	detail::UninitializedMoveIfNoexcept_<
		detail::IsNothrowUnMove<tInputIt,tForwardIt>::value
	>::enact_pn( aBeg, aCount, aOut );
}
template< class tAlloc, typename tInputIt, typename tForwardIt > inline
void uninitialized_move_if_noexcept_p( tAlloc& FLUX_RESTRICT aAlloc, tInputIt& FLUX_RESTRICT aBeg, tInputIt aEnd, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnMove<tInputIt,tForwardIt>::value))
{
	detail::UninitializedMoveIfNoexcept_<
		detail::IsNothrowUnMove<tInputIt,tForwardIt>::value
	>::enact_p( aAlloc, aBeg, aEnd, aOut );
}
template< class tAlloc, typename tInputIt, typename tForwardIt > inline
void uninitialized_move_if_noexcept_pn( tAlloc& FLUX_RESTRICT aAlloc, tInputIt& FLUX_RESTRICT aBeg, std::size_t aCount, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnMove<tInputIt,tForwardIt>::value))
{
	detail::UninitializedMoveIfNoexcept_<
		detail::IsNothrowUnMove<tInputIt,tForwardIt>::value
	>::enact_pn( aAlloc, aBeg, aCount, aOut );
}


//--    uninitialized_relocate*()           ///{{{2///////////////////////////
template< typename tFwdIn, typename tFwdOut > inline
tFwdOut uninitialized_relocate( tFwdIn aBeg, tFwdIn aEnd, tFwdOut aOut )  noexcept((detail::IsNothrowUnReloc<tFwdIn,tFwdOut>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	return detail::UninitializedRelocate_<
		detail::URelocCanMemcpy_<tFwdIn,tFwdOut>::value
	>::enact( aBeg, aEnd, aOut );
}
template< typename tFwdIn, typename tFwdOut > inline
std::pair<tFwdIn,tFwdOut> uninitialized_relocate_n( tFwdIn aBeg, std::size_t aCount, tFwdOut aOut )
{
	return detail::UninitializedRelocate_<
		detail::URelocCanMemcpy_<tFwdIn,tFwdOut>::value
	>::enact_n( aBeg, aCount, aOut );
}

template< class tAlloc, typename tFwdIn, typename tFwdOut > inline
tFwdOut uninitialized_relocate( tAlloc& FLUX_RESTRICT aAlloc, tFwdIn aBeg, tFwdIn aEnd, tFwdOut aOut )  noexcept((detail::IsNothrowUnReloc<tFwdIn,tFwdOut>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	return detail::UninitializedRelocate_<
		detail::URelocCanMemcpy_<tFwdIn,tFwdOut>::value
	>::enact( aAlloc, aBeg, aEnd, aOut );
}
template< class tAlloc, typename tFwdIn, typename tFwdOut > inline
std::pair<tFwdIn,tFwdOut> uninitialized_relocate_n( tAlloc& FLUX_RESTRICT aAlloc, tFwdIn aBeg, std::size_t aCount, tFwdOut aOut )
{
	return detail::UninitializedRelocate_<
		detail::URelocCanMemcpy_<tFwdIn,tFwdOut>::value
	>::enact_n( aAlloc, aBeg, aCount, aOut );
}

template< typename tInputIt, typename tForwardIt > inline
void uninitialized_relocate_p( tInputIt& FLUX_RESTRICT aBeg, tInputIt aEnd, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnReloc<tInputIt,tForwardIt>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	detail::UninitializedRelocate_<
		detail::URelocCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_p( aBeg, aEnd, aOut );
}
template< typename tInputIt, typename tForwardIt > inline
void uninitialized_relocate_pn( tInputIt& FLUX_RESTRICT aBeg, std::size_t aCount, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnReloc<tInputIt,tForwardIt>::value))
{
	detail::UninitializedRelocate_<
		detail::URelocCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_pn( aBeg, aCount, aOut );
}

template< class tAlloc, typename tInputIt, typename tForwardIt > inline
void uninitialized_relocate_p( tAlloc& aAlloc, tInputIt& FLUX_RESTRICT aBeg, tInputIt aEnd, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnReloc<tInputIt,tForwardIt>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	detail::UninitializedRelocate_<
		detail::URelocCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_p( aAlloc, aBeg, aEnd, aOut );
}
template< class tAlloc, typename tInputIt, typename tForwardIt > inline
void uninitialized_relocate_pn( tAlloc& aAlloc, tInputIt& FLUX_RESTRICT aBeg, std::size_t aCount, tForwardIt& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnReloc<tInputIt,tForwardIt>::value))
{
	detail::UninitializedRelocate_<
		detail::URelocCanMemcpy_<tInputIt,tForwardIt>::value
	>::enact_pn( aAlloc, aBeg, aCount, aOut );
}

//--    uninitialized_relocate_if_noexcept*()   ///{{{2///////////////////////
template< typename tFwdIn, typename tFwdOut > inline
tFwdOut uninitialized_relocate_if_noexcept( tFwdIn aBeg, tFwdIn aEnd, tFwdOut aOut )  noexcept((detail::IsNothrowUnReloc<tFwdIn,tFwdOut>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	return detail::UninitializedRelocateIfNoexcept_<
		detail::URelocCanMemcpy_<tFwdIn,tFwdOut>::value,
		tFwdIn, tFwdOut
	>::enact( aBeg, aEnd, aOut );
}
template< typename tFwdIn, typename tFwdOut > inline
std::pair<tFwdIn,tFwdOut> uninitialized_relocate_if_noexcept_n( tFwdIn aBeg, std::size_t aCount, tFwdOut aOut )
{
	return detail::UninitializedRelocateIfNoexcept_<
		detail::URelocCanMemcpy_<tFwdIn,tFwdOut>::value,
		tFwdIn, tFwdOut
	>::enact_n( aBeg, aCount, aOut );
}

template< class tAlloc, typename tFwdIn, typename tFwdOut > inline
tFwdOut uninitialized_relocate_if_noexcept( tAlloc& FLUX_RESTRICT aAlloc, tFwdIn aBeg, tFwdIn aEnd, tFwdOut aOut )  noexcept((detail::IsNothrowUnReloc<tFwdIn,tFwdOut>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	return detail::UninitializedRelocateIfNoexcept_<
		detail::URelocCanMemcpy_<tFwdIn,tFwdOut>::value,
		tFwdIn, tFwdOut
	>::enact( aAlloc, aBeg, aEnd, aOut );
}
template< class tAlloc, typename tFwdIn, typename tFwdOut > inline
std::pair<tFwdIn,tFwdOut> uninitialized_relocate_if_noexcept_n( tAlloc& FLUX_RESTRICT aAlloc, tFwdIn aBeg, std::size_t aCount, tFwdOut aOut )
{
	return detail::UninitializedRelocateIfNoexcept_<
		detail::URelocCanMemcpy_<tFwdIn,tFwdOut>::value,
		tFwdIn, tFwdOut
	>::enact_n( aAlloc, aBeg, aCount, aOut );
}

template< typename tFwdIn, typename tFwdOut > inline
void uninitialized_relocate_if_noexcept_p( tFwdIn& FLUX_RESTRICT aBeg, tFwdIn aEnd, tFwdOut& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnReloc<tFwdIn,tFwdOut>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	detail::UninitializedRelocateIfNoexcept_<
		detail::URelocCanMemcpy_<tFwdIn,tFwdOut>::value,
		tFwdIn, tFwdOut
	>::enact_p( aBeg, aEnd, aOut );
}
template< typename tFwdIn, typename tFwdOut > inline
void uninitialized_relocate_if_noexcept_pn( tFwdIn& FLUX_RESTRICT aBeg, std::size_t aCount, tFwdOut& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnReloc<tFwdIn,tFwdOut>::value))
{
	detail::UninitializedRelocateIfNoexcept_<
		detail::URelocCanMemcpy_<tFwdIn,tFwdOut>::value,
		tFwdIn, tFwdOut
	>::enact_pn( aBeg, aCount, aOut );
}

template< class tAlloc, typename tFwdIn, typename tFwdOut > inline
void uninitialized_relocate_if_noexcept_p( tAlloc& FLUX_RESTRICT aAlloc, tFwdIn& FLUX_RESTRICT aBeg, tFwdIn aEnd, tFwdOut& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnReloc<tFwdIn,tFwdOut>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	detail::UninitializedRelocateIfNoexcept_<
		detail::URelocCanMemcpy_<tFwdIn,tFwdOut>::value,
		tFwdIn, tFwdOut
	>::enact_p( aAlloc, aBeg, aEnd, aOut );
}
template< class tAlloc, typename tFwdIn, typename tFwdOut > inline
void uninitialized_relocate_if_noexcept_pn( tAlloc& FLUX_RESTRICT aAlloc, tFwdIn& FLUX_RESTRICT aBeg, std::size_t aCount, tFwdOut& FLUX_RESTRICT aOut ) noexcept((detail::IsNothrowUnReloc<tFwdIn,tFwdOut>::value))
{
	detail::UninitializedRelocateIfNoexcept_<
		detail::URelocCanMemcpy_<tFwdIn,tFwdOut>::value,
		tFwdIn, tFwdOut
	>::enact_pn( aAlloc, aBeg, aCount, aOut );
}

//--    uninitialized_value_construct()     ///{{{2///////////////////////////
template< typename tForward > inline
void uninitialized_value_construct( tForward aBeg, tForward aEnd )
{
	using Ty_ = typename std::iterator_traits<tForward>::value_type;

	using CanMemset_ = ftl::meta::All<
		ftl::is_contiguous_iterator<tForward>,
		ftl::is_bitwise_conjurable<Ty_>
	>;
	
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );

	detail::ValueConstruct_<CanMemset_::value>::enact( aBeg, aEnd );
}
template< class tAlloc, typename tForward > inline
void uninitialized_value_construct( tAlloc& FLUX_RESTRICT aAlloc, tForward aBeg, tForward aEnd ) noexcept(detail::IsNothrowValueCtor<tForward>::value)
{
	using Ty_ = typename std::iterator_traits<tForward>::value_type;

	using Nothrow_ = std::integral_constant<bool,detail::IsNothrowValueCtor<tForward>::value>;
	using CanMemset_ = ftl::meta::All<
		ftl::is_contiguous_iterator<tForward>,
		ftl::is_bitwise_conjurable<Ty_>
	>;
	
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );

	detail::ValueConstruct_<CanMemset_::value>::enact( aAlloc, aBeg, aEnd, Nothrow_{} );
}

template< typename tForward > inline
tForward uninitialized_value_construct_n( tForward aBeg, std::size_t aCount )
{
	using Ty_ = typename std::iterator_traits<tForward>::value_type;
	using CanMemset_ = ftl::meta::All<
		ftl::is_contiguous_iterator<tForward>,
		ftl::is_bitwise_conjurable<Ty_>
	>;

	return detail::ValueConstruct_<CanMemset_::value>::enact_n( aBeg, aCount );
}
template< class tAlloc, typename tForward > inline
tForward uninitialized_value_construct_n( tAlloc& FLUX_RESTRICT aAlloc, tForward aBeg, std::size_t aCount ) noexcept(detail::IsNothrowValueCtor<tForward>::value)
{
	using Ty_ = typename std::iterator_traits<tForward>::value_type;
	using Nothrow_ = std::integral_constant<bool,detail::IsNothrowValueCtor<tForward>::value>;
	using CanMemset_ = ftl::meta::All<
		ftl::is_contiguous_iterator<tForward>,
		ftl::is_bitwise_conjurable<Ty_>
	>;

	return detail::ValueConstruct_<CanMemset_::value>::enact_n( aAlloc, aBeg, aCount, Nothrow_{} );
}

template< typename tForward >
void uninitialized_value_construct_p( tForward& FLUX_RESTRICT aBeg, tForward aEnd ) noexcept((detail::IsNothrowValueCtor<tForward>::value))
{
	using Ty_ = typename std::iterator_traits<tForward>::value_type;
	using CanMemset_ = ftl::meta::All<
		ftl::is_contiguous_iterator<tForward>,
		ftl::is_bitwise_conjurable<Ty_>
	>;
	
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );

	detail::ValueConstruct_<CanMemset_::value>::enact_p( aBeg, aEnd );
}
template< typename tForward >
void uninitialized_value_construct_pn( tForward& FLUX_RESTRICT aBeg, std::size_t aCount ) noexcept((detail::IsNothrowValueCtor<tForward>::value))
{
	using Ty_ = typename std::iterator_traits<tForward>::value_type;
	using CanMemset_ = ftl::meta::All<
		ftl::is_contiguous_iterator<tForward>,
		ftl::is_bitwise_conjurable<Ty_>
	>;

	detail::ValueConstruct_<CanMemset_::value>::enact_pn( aBeg, aCount );
}
template< class tAlloc, typename tForward >
void uninitialized_value_construct_p( tAlloc& FLUX_RESTRICT aAlloc, tForward& FLUX_RESTRICT aBeg, tForward aEnd ) noexcept((detail::IsNothrowValueCtor<tForward>::value))
{
	using Ty_ = typename std::iterator_traits<tForward>::value_type;
	using CanMemset_ = ftl::meta::All<
		ftl::is_contiguous_iterator<tForward>,
		ftl::is_bitwise_conjurable<Ty_>
	>;
	
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );

	detail::ValueConstruct_<CanMemset_::value>::enact_p( aAlloc, aBeg, aEnd );
}
template< class tAlloc, typename tForward >
void uninitialized_value_construct_pn( tAlloc& FLUX_RESTRICT aAlloc, tForward& FLUX_RESTRICT aBeg, std::size_t aCount ) noexcept(detail::IsNothrowValueCtor<tForward>::value)
{
	using Ty_ = typename std::iterator_traits<tForward>::value_type;
	using CanMemset_ = ftl::meta::All<
		ftl::is_contiguous_iterator<tForward>,
		ftl::is_bitwise_conjurable<Ty_>
	>;

	detail::ValueConstruct_<CanMemset_::value>::enact_pn( aAlloc, aBeg, aCount );
}

//--    uninitialized_default_construct()   ///{{{2///////////////////////////
template< typename tForward > inline
void uninitialized_default_construct( tForward aBeg, tForward aEnd ) noexcept(detail::IsNothrowDefaultCtor<tForward>::value)
{
	using Ty_ = typename std::iterator_traits<tForward>::value_type;
	
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );

	detail::DefaultConstruct_<
		ftl::is_bitwise_awakeable<Ty_>::value
	>::enact( aBeg, aEnd );
}
template< typename tForward > inline
tForward uninitialized_default_construct_n( tForward aBeg, std::size_t aCount ) noexcept(detail::IsNothrowDefaultCtor<tForward>::value)
{
	using Ty_ = typename std::iterator_traits<tForward>::value_type;
	using Cat_ = typename std::iterator_traits<tForward>::iterator_category;

	return detail::DefaultConstruct_<
		ftl::is_bitwise_awakeable<Ty_>::value
	>::enact_n( aBeg, aCount, Cat_{} );
}
template< class tAlloc, typename tForward > inline
void uninitialized_default_construct( tAlloc& aAlloc, tForward aBeg, tForward aEnd ) noexcept(detail::IsNothrowDefaultCtor<tForward>::value)
{
	using Ty_ = typename std::iterator_traits<tForward>::value_type;
	using Nothrow_ = std::integral_constant<bool, detail::IsNothrowDefaultCtor<tForward>::value>;
	
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );

	detail::DefaultConstruct_<
		ftl::is_bitwise_awakeable<Ty_>::value
	>::enact( aAlloc, aBeg, aEnd, Nothrow_{} );
}
template< class tAlloc, typename tForward > inline
tForward uninitialized_default_construct_n( tAlloc& FLUX_RESTRICT aAlloc, tForward aBeg, std::size_t aCount ) noexcept(detail::IsNothrowDefaultCtor<tForward>::value)
{
	using Ty_ = typename std::iterator_traits<tForward>::value_type;
	using Cat_ = typename std::iterator_traits<tForward>::iterator_category;
	using Nothrow_ = std::integral_constant<bool, detail::IsNothrowDefaultCtor<tForward>::value>;

	return detail::DefaultConstruct_<
		ftl::is_bitwise_awakeable<Ty_>::value
	>::enact_n( aAlloc, aBeg, aCount, Cat_{}, Nothrow_{} );
}

template< typename tForward >
void uninitialized_default_construct_p( tForward& FLUX_RESTRICT aBeg, tForward aEnd ) noexcept((detail::IsNothrowDefaultCtor<tForward>::value))
{
	using Ty_ = typename std::iterator_traits<tForward>::value_type;
	
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );

	detail::DefaultConstruct_<
		ftl::is_bitwise_awakeable<Ty_>::value
	>::enact_p( aBeg, aEnd );
}
template< typename tForward >
void uninitialized_default_construct_pn( tForward& FLUX_RESTRICT aBeg, std::size_t aCount ) noexcept((detail::IsNothrowDefaultCtor<tForward>::value))
{
	using Ty_ = typename std::iterator_traits<tForward>::value_type;
	using Cat_ = typename std::iterator_traits<tForward>::iterator_category;

	detail::DefaultConstruct_<
		ftl::is_bitwise_awakeable<Ty_>::value
	>::enact_pn( aBeg, aCount, Cat_{} );
}
template< class tAlloc, typename tForward >
void uninitialized_default_construct_p( tAlloc& FLUX_RESTRICT aAlloc, tForward& FLUX_RESTRICT aBeg, tForward aEnd ) noexcept((detail::IsNothrowDefaultCtor<tForward>::value))
{
	using Ty_ = typename std::iterator_traits<tForward>::value_type;
	
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );

	detail::DefaultConstruct_<
		ftl::is_bitwise_awakeable<Ty_>::value
	>::enact_p( aAlloc, aBeg, aEnd );
}
template< class tAlloc, typename tForward >
void uninitialized_default_construct_pn( tAlloc& FLUX_RESTRICT aAlloc, tForward& FLUX_RESTRICT aBeg, std::size_t aCount ) noexcept(detail::IsNothrowDefaultCtor<tForward>::value)
{
	using Ty_ = typename std::iterator_traits<tForward>::value_type;
	using Cat_ = typename std::iterator_traits<tForward>::iterator_category;

	detail::DefaultConstruct_<
		ftl::is_bitwise_awakeable<Ty_>::value
	>::enact_pn( aAlloc, aBeg, aCount, Cat_{} );
}

//--    uninitialized_fill()                ///{{{2///////////////////////////
template< typename tForward, typename tType > inline
void uninitialized_fill( tForward aBeg, tForward aEnd, tType const& aValue )
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	std::uninitialized_fill( aBeg, aEnd, aValue );
}
template< typename tForward, typename tType > inline
tForward uninitialized_fill_n( tForward aBeg, std::size_t aCount, tType const& FLUX_RESTRICT aValue )
{
#	if FLUX_STDQUIRK_CXX11_OLD_UNINITIALIZED
	using Tag_ = typename std::integral_constant<bool,detail::IsNothrowUnCtor<tForward,tType>::value>;
	return detail::UninitializedFill_::enact_n( aBeg, aCount, aValue, Tag_{} );
#	else // !STDQUIRK_CXX11_OLD_UNINITIALZIED
	return std::uninitialized_fill_n( aBeg, aCount, aValue );
#	endif // ~ STDQUIRK_CXX11_OLD_UNINITIALZIED
}

template< class tAlloc, typename tForward, typename tType > inline
void uninitialized_fill( tAlloc& FLUX_RESTRICT aAlloc, tForward aBeg, tForward aEnd, tType const& FLUX_RESTRICT aValue ) noexcept((detail::IsNothrowUnCtor<tForward,tType>::value))
{
	using Tag_ = typename std::integral_constant<bool,detail::IsNothrowUnCtor<tForward,tType>::value>;
	
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	detail::UninitializedFill_::enact( aAlloc, aBeg, aEnd, aValue, Tag_{} );
}
template< class tAlloc, typename tForward, typename tType > inline
tForward uninitialized_fill_n( tAlloc& FLUX_RESTRICT aAlloc, tForward aBeg, std::size_t aCount, tType const& FLUX_RESTRICT aValue ) noexcept((detail::IsNothrowUnCtor<tForward,tType>::value))
{
	using Tag_ = typename std::integral_constant<bool,detail::IsNothrowUnCtor<tForward,tType>::value>;
	
	return detail::UninitializedFill_::enact_n( aAlloc, aBeg, aCount, aValue, Tag_{} );
}

template< typename tForward, typename tType > inline
void uninitialized_fill_p( tForward& FLUX_RESTRICT aBeg, tForward aEnd, tType const& FLUX_RESTRICT aValue ) noexcept((detail::IsNothrowUnCtor<tForward,tType>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	detail::UninitializedFill_::enact_p( aBeg, aEnd, aValue );
}
template< typename tForward, typename tType > inline
void uninitialized_fill_pn( tForward& FLUX_RESTRICT aBeg, std::size_t aCount, tType const& FLUX_RESTRICT aValue ) noexcept((detail::IsNothrowUnCtor<tForward,tType>::value))
{
	detail::UninitializedFill_::enact_pn( aBeg, aCount, aValue );
}
template< class tAlloc, typename tForward, typename tType > inline
void uninitialized_fill_p( tAlloc& FLUX_RESTRICT aAlloc, tForward& FLUX_RESTRICT aBeg, tForward aEnd, tType const& FLUX_RESTRICT aValue ) noexcept((detail::IsNothrowUnCtor<tForward,tType>::value))
{
	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );
	detail::UninitializedFill_::enact_p( aAlloc, aBeg, aEnd, aValue );
}
template< class tAlloc, typename tForward, typename tType > inline
void uninitialized_fill_pn( tAlloc& FLUX_RESTRICT aAlloc, tForward& FLUX_RESTRICT aBeg, std::size_t aCount, tType const& FLUX_RESTRICT aValue ) noexcept((detail::IsNothrowUnCtor<tForward,tType>::value))
{
	detail::UninitializedFill_::enact_pn( aAlloc, aBeg, aCount, aValue );
}

//--    destroy()                           ///{{{2///////////////////////////
template< typename tForward > inline
void destroy( tForward aBeg, tForward aEnd ) noexcept(detail::IsNothrowDtor<tForward>::value)
{
	using Ty_ = typename std::iterator_traits<tForward>::value_type;

	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );

	detail::Destroy_<
		ftl::is_bitwise_dismissible<Ty_>::value
	>::enact( aBeg, aEnd );
}
template< typename tForward > inline
tForward destroy_n( tForward aBeg, std::size_t aCount ) noexcept(detail::IsNothrowDtor<tForward>::value)
{
	using Ty_ = typename std::iterator_traits<tForward>::value_type;
	using Cat_ = typename std::iterator_traits<tForward>::iterator_category;

	return detail::Destroy_<
		ftl::is_bitwise_dismissible<Ty_>::value
	>::enact_n( aBeg, aCount, Cat_{} );
}
template< typename tType > inline
void destroy_at( tType* aPtr ) noexcept(std::is_nothrow_destructible<tType>::value)
{
	detail::Destroy_<
		ftl::is_bitwise_dismissible<tType>::value
	>::enact_at( aPtr );
}

template< class tAlloc, typename tForward > inline
void destroy( tAlloc& FLUX_RESTRICT aAlloc, tForward aBeg, tForward aEnd ) noexcept(detail::IsNothrowDtor<tForward>::value)
{
	using Ty_ = typename std::iterator_traits<tForward>::value_type;

	FLUXTMP_DEBUG_CHECK_ORDER_( aBeg, aEnd );

	detail::Destroy_<
		ftl::is_bitwise_dismissible<Ty_>::value
	>::enact( aAlloc, aBeg, aEnd );
}
template< class tAlloc, typename tForward > inline
tForward destroy_n( tAlloc& FLUX_RESTRICT aAlloc, tForward aBeg, std::size_t aCount ) noexcept(detail::IsNothrowDtor<tForward>::value)
{
	using Ty_ = typename std::iterator_traits<tForward>::value_type;
	using Cat_ = typename std::iterator_traits<tForward>::iterator_category;

	return detail::Destroy_<
		ftl::is_bitwise_dismissible<Ty_>::value
	>::enact_n( aAlloc, aBeg, aCount, Cat_{} );
}
template< class tAlloc, typename tType > inline
void destroy_at( tAlloc& FLUX_RESTRICT aAlloc, tType* aPtr ) noexcept(std::is_nothrow_destructible<tType>::value)
{
	detail::Destroy_<
		ftl::is_bitwise_dismissible<tType>::value
	>::enact_at( aAlloc, aPtr );
}

//--    d: cleanup                          ///{{{2///////////////////////////

#undef FLUXTMP_DEBUG_CHECK_ORDER_

FLUX_DIAGNOSTIC_STATE_POP_OR_DEFAULT(CLASS_MEMACCESS)

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
