/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	aspect::AnyType*
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_AE124D8C_D64C_4BAF_A300_27E070CB701B
#define DETAIL_AE124D8C_D64C_4BAF_A300_27E070CB701B

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/ftl/hash.hpp>
#include <flux/ftl/uninitialized.hpp>

#include <flux/util/returns.hpp>

#include <flux/compat/constexpr.hpp>
#include <flux/compat/type_traits.hpp>

#include <functional>
#include <type_traits>

//--    >>> namespace = flux::ftl::aspect >>>       ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(ftl,aspect)
//--    $ local                             ///{{{2///////////////////////////
namespace detail
{
	struct AnyLocal_ {};
	struct AnyAnything_
	{
		template< typename tType >
		AnyAnything_( tType&& ) {}
	};

	// any_has_equal_ -{{{3-
	namespace hide
	{
		template< class tType, 
			class = decltype(std::declval<tType>() == std::declval<tType>())
		> 
		std::true_type any_check_equal_( tType const& );
		std::false_type any_check_equal_( ... );
	}

	template< typename tType >
	struct any_has_equal_ : decltype(hide::any_check_equal_(std::declval<tType>()))
	{};
	
	// any_has_less_ -{{{3-
	namespace hide
	{
		template< typename tType,
			class = decltype(std::declval<tType>() < std::declval<tType>())
		> 
		std::true_type any_check_less_( tType const& );
		std::false_type any_check_less_( ... );
	}

	template< typename tType >
	struct any_has_less_ : decltype(hide::any_check_less_(std::declval<tType>()))
	{};
	
	// any_is_hashable_ -{{{3-
	namespace hide
	{
		using flux::ftl::hash;

		template< class tType, 
			class = decltype(hash(std::declval<tType>()))
		> 
		std::true_type any_check_hash_( tType const& );
		std::false_type any_check_hash_( ... );
	}
	

	template< typename tType >
	struct any_is_hashable_ : decltype(hide::any_check_hash_(std::declval<tType>()))
	{};

	// AnyInjectEqual*_ -{{{3-
	template< typename tType >
	struct AnyInjectEqualPrimed_
	{
		static inline FLUX_CONSTEXPR_EX
		auto equal( tType const& aLHS, tType const& aRHS )
		FLUX_RETURNS( aLHS == aRHS )
	};
	template< typename tType >
	struct AnyInjectEqualNull_
	{
		static inline constexpr
		bool equal( tType const&, tType const& ) noexcept
		{
			return false;
		}
	};

	template< typename tType >
	struct AnyInjectEqual_
		: std::conditional< detail::any_has_equal_<tType>::value,
			detail::AnyInjectEqualPrimed_<tType>,
			detail::AnyInjectEqualNull_<tType>
		>::type
	{};

	// AnyInjectLess*_ -{{{3-
	template< typename tType >
	struct AnyInjectLessPrimed_
	{
		static inline FLUX_CONSTEXPR_EX
		auto less( tType const& aLHS, tType const& aRHS )
		FLUX_RETURNS( std::less<tType>{}( aLHS, aRHS ) )
	};
	template< typename tType >
	struct AnyInjectLessNull_
	{
		static inline constexpr
		bool less( tType const&, tType const& ) noexcept
		{
			return false;
		}
	};

	template< typename tType >
	struct AnyInjectLess_
		: std::conditional< detail::any_has_less_<tType>::value,
			detail::AnyInjectLessPrimed_<tType>,
			detail::AnyInjectLessNull_<tType>
		>::type
	{};
	
	// AnyInjectHash*_ -{{{3-
	template< typename tType > inline FLUX_CONSTEXPR_EX
	ftl::Hash any_hash_impl_( tType const& aX ) noexcept(noexcept(hash(aX)))
	{
		using ftl::hash;
		return hash( aX );
	}
	
	template< typename tType >
	struct AnyInjectHashPrimed_
	{
		static inline FLUX_CONSTEXPR_EX
		auto hash( tType const& aX ) 
		FLUX_RETURNS( any_hash_impl_( aX ) )
	};
	template< typename tType >
	struct AnyInjectHashNull_
	{
		static inline constexpr
		ftl::Hash hash( tType const& ) noexcept
		{
			return ftl::Hash(0);
		}
	};

	template< typename tType >
	struct AnyInjectHash_
		: std::conditional< detail::any_is_hashable_<tType>::value,
			detail::AnyInjectHashPrimed_<tType>,
			detail::AnyInjectHashNull_<tType>
		>::type
	{};
}

//--    AnyTypeDefault                      ///{{{2///////////////////////////
namespace detail
{
	struct AnyFeatDummy_ { static constexpr unsigned mask = 0u; };
};

template< typename tType >
struct AnyTypeDefault
	: detail::AnyInjectEqual_<tType>
	, detail::AnyInjectLess_<tType>
	, detail::AnyInjectHash_<tType>
{
	using Features = meta::make_set<
		typename std::conditional< detail::any_has_equal_<tType>::value,
		  	aspect::AnyFeatEqual,
		  	detail::AnyFeatDummy_
		>::type,
		typename std::conditional< detail::any_has_less_<tType>::value,
		  	aspect::AnyFeatLess,
		  	detail::AnyFeatDummy_
		>::type,
		typename std::conditional< detail::any_is_hashable_<tType>::value,
			aspect::AnyFeatHash,
			detail::AnyFeatDummy_
		>::type
	>;
	
	static inline FLUX_CONSTEXPR_EX
	void destroy( tType* aPtr ) noexcept(noexcept(ftl::destroy_at(aPtr)))
	{
		FLUX_ASSERT( aPtr );
		ftl::destroy_at( aPtr );
	}

	template< typename... tArgs > static inline FLUX_CONSTEXPR_EX
	void construct( void* aDest, tArgs&&... aArgs ) noexcept(noexcept(tType( std::forward<tArgs>(aArgs)... )))
	{
		FLUX_ASSERT( aDest );
		new (aDest) tType( std::forward<tArgs>(aArgs)... );
	}
};

// Specialization for arrays:
template< typename tType, std::size_t tExtent >
struct AnyTypeDefault< tType[tExtent] >
	: detail::AnyInjectEqual_<tType>
	, detail::AnyInjectLess_<tType>
	, detail::AnyInjectHash_<tType>
{
	using Features = meta::make_set<
		typename std::conditional< detail::any_has_equal_<tType>::value,
		  	aspect::AnyFeatEqual,
		  	detail::AnyFeatDummy_
		>::type,
		typename std::conditional< detail::any_is_hashable_<tType>::value,
			aspect::AnyFeatHash,
			detail::AnyFeatDummy_
		>::type
	>;
	
	using Array_ = tType[tExtent];

	static inline FLUX_CONSTEXPR_EX
	void destroy( Array_* aPtr ) noexcept(noexcept(ftl::destroy_n(aPtr,tExtent)))
	{
		FLUX_ASSERT( aPtr );
		ftl::destroy_n( aPtr, tExtent );
	}

	static inline FLUX_CONSTEXPR_EX
	void construct( void* aDest, Array_&& aSrc ) noexcept(noexcept(ftl::uninitialized_move_n( aSrc, tExtent, std::declval<tType*>() )))
	{
		FLUX_ASSERT( aDest );

		using Type_ = compat::remove_cv_t<tType>;
		Type_* ptr = static_cast<Type_*>(aDest);
		ftl::uninitialized_move_n( aSrc, tExtent, ptr );
	}
	static inline FLUX_CONSTEXPR_EX
	void construct( void* aDest, Array_ const& aSrc ) noexcept(noexcept(ftl::uninitialized_copy_n( aSrc, tExtent, std::declval<tType*>() )))
	{
		FLUX_ASSERT( aDest );

		using Type_ = compat::remove_cv_t<tType>;
		Type_* ptr = static_cast<Type_*>(aDest);
		ftl::uninitialized_copy_n( aSrc, tExtent, ptr );
	}

	static inline FLUX_CONSTEXPR_EX
	bool equal( Array_ const& aLHS, Array_ const& aRHS ) noexcept(noexcept(detail::AnyInjectEqual_<tType>::equal(aLHS[0],aRHS[0])))
	{
		for( std::size_t i = 0; i < tExtent; ++i )
		{
			if( !detail::AnyInjectEqual_<tType>::equal( aLHS[i], aRHS[i] ) )
				return false;
		}

		return true;
	}
	static inline FLUX_CONSTEXPR_EX
	ftl::Hash hash( Array_ const& aX ) /*TODO: noexcept stuff*/
	{
		using flux::ftl::hash_combine;
		
		ftl::Hash h = 0;
		if( tExtent > 0 )
		{
			h = detail::AnyInjectHash_<tType>::hash( aX[0] );
			for( std::size_t i = 1; i < tExtent; ++i )
				h = hash_combine( h, detail::AnyInjectHash_<tType>::hash( aX[i] ) );
		}
		
		return h;
	}
};

// Specialization for char const*:
template<>
struct AnyTypeDefault< char const* >
{
	using Features = meta::make_set< 
			aspect::AnyFeatEqual, 
			aspect::AnyFeatLess, 
			aspect::AnyFeatHash 
	>;

	static inline
	void destroy( char const** ) noexcept
	{}

	static inline
	void construct( void* aDest, char const* aSrc ) noexcept
	{
		using CharPtr_ = char const*;
		new (aDest) CharPtr_( aSrc );
	}

	static inline
	bool equal( char const* aLHS, char const* aRHS ) noexcept
	{
		if( !aLHS || !aRHS ) return !aLHS && !aRHS;
		return 0 == std::strcmp( aLHS, aRHS );
	}
	static inline
	bool less( char const* aLHS, char const* aRHS ) noexcept
	{
		if( !aLHS ) return !!aRHS;
		if( !aRHS ) return false;
		return std::strcmp( aLHS, aRHS ) < 0;
	}

	static inline
	ftl::Hash hash( char const* aStr ) noexcept
	{
		using flux::ftl::hash;
		return hash( aStr );
	}
};

//--    <<< ~ flux::ftl::aspect namespace <<<       ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(ftl,aspect)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_AE124D8C_D64C_4BAF_A300_27E070CB701B
