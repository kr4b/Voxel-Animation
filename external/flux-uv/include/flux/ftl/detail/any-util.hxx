/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	AnyT<> utilities & helpers
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_240899EC_37BA_4657_8BF1_81CD7AF2DE50
#define DETAIL_240899EC_37BA_4657_8BF1_81CD7AF2DE50

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/compat/constexpr.hpp>
#include <flux/compat/type_traits.hpp>

//--    >>> namespace = flux::ftl::detail >>>       ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(ftl,detail)
//--    any_type_map_t                      ///{{{2///////////////////////////
template< typename tType >
struct AnyTypeMap_
{
	using type = compat::remove_reference_t<tType>;
};

template< std::size_t tExtent >
struct AnyTypeMap_< char[tExtent] >
{
	using type = char const*;
};
template< std::size_t tExtent >
struct AnyTypeMap_< char const[tExtent] >
{
	using type = char const*;
};
template< std::size_t tExtent >
struct AnyTypeMap_< char (&) [tExtent] >
{
	using type = char const*;
};
template< std::size_t tExtent >
struct AnyTypeMap_< char const (&) [tExtent] >
{
	using type = char const*;
};

template< typename tRet, typename... tArgs >
struct AnyTypeMap_< tRet(tArgs...) >
{
	using type = tRet (*) (tArgs...);
};


template< typename tType >
using any_type_map_t = typename detail::AnyTypeMap_<tType>::type;

//--    CheckAnyAspect                      ///{{{2///////////////////////////
template< class tAspect, class tFeatures, typename tType >
struct CheckAnyAspect
{
	static inline void check() noexcept {}

	//TODO: check for operator==, operator< and free-form hash()
	//


};

//--    any_in_place_possible()             ///{{{2///////////////////////////

#if 0
template< typename tType, std::size_t tSize, std::size_t tAlign > inline constexpr
bool any_in_place_possible()
{
	return sizeof(tType) <= tSize && (tAlign%alignof(tType) == 0);
}
#endif

//--    AnyInPlaceEligible                  ///{{{2///////////////////////////
template< typename tType, std::size_t tSize, std::size_t tAlign >
struct AnyInPlaceEligible
	: std::integral_constant<
		bool,
		   sizeof(tType) <= tSize
		&& ((tAlign%alignof(tType)) == 0)
		&& std::is_nothrow_move_constructible<tType>::value /* XXX XXX IsNothrowUnReloc!*/
		&& std::is_nothrow_move_assignable<tType>::value
	>

	//TODO FIXME: use ftl::is_nothrow_relocatable<> or similar.
{};

//--    AnyConstruct                        ///{{{2///////////////////////////
template< bool tInPlace >
struct AnyConstruct
{
	template< class tAspect, typename tType, typename tBuffer, typename... tArgs > inline FLUX_CONSTEXPR_EX
	static void construct( tBuffer& aBuffer, tArgs&&... aArgs )
	{
		tAspect::construct( &aBuffer, std::forward<tArgs>(aArgs)... );
	}
};

template<>
struct AnyConstruct<false>
{
	template< class tAspect, typename tType, typename tBuffer, typename... tArgs > static inline
	void construct( tBuffer& aBuffer, tArgs&&... aArgs ) noexcept(noexcept(tAspect::construct(std::declval<void*>(),std::forward<tArgs>(aArgs)...)))
	{
		using Tag_ = std::integral_constant<
			bool,
			noexcept(tAspect::construct(std::declval<void*>(),std::forward<tArgs>(aArgs)...))
		>;

		construct_<tAspect,tType>( Tag_{}, aBuffer, std::forward<tArgs>(aArgs)... );
	}


	template< class tAspect, typename tType, typename tBuffer, typename... tArgs > static inline
	void construct_( std::true_type, tBuffer& aBuffer, tArgs&&... aArgs ) noexcept
	{
		using VoidPtr_ = void*;

		void* ptr = ::operator new(sizeof(tType));
		tAspect::construct( ptr, std::forward<tArgs>(aArgs)... );
		new (&aBuffer) VoidPtr_(ptr);
	}
	template< class tAspect, typename tType, typename tBuffer, typename... tArgs > static inline
	void construct_( std::false_type, tBuffer& aBuffer, tArgs&&... aArgs )
	{
		using VoidPtr_ = void*;

		void* ptr = ::operator new(sizeof(tType));
		try
		{
			tAspect::construct( ptr, std::forward<tArgs>(aArgs)... );
			new (&aBuffer) VoidPtr_(ptr);
		}
		catch( ... )
		{
			::operator delete(ptr);
			throw;
		}
	}
};

//--    <<< ~ flux::ftl::detail namespace <<<       ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(ftl,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_240899EC_37BA_4657_8BF1_81CD7AF2DE50
