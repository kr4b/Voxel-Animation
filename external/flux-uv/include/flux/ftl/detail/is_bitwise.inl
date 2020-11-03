/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- is_bitwise_*<> implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/ftl/defaults.hpp>
#include <flux/compat/is_trivially.hpp>

#include <utility>
#include <type_traits>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    $ local                             ///{{{2///////////////////////////
namespace detail
{
	struct BitwiseNeutralType {};
	
	namespace def
	{
		template< typename tType >
		BitwiseNeutralType tag_is_bitwise_conjurable( tType const& );
	//	template< typename tType >
	//	BitwiseNeutralType tag_is_bitwise_materializable( tType const& );
		template< typename tType >
		BitwiseNeutralType tag_is_bitwise_awakeable( tType const& );
		template< typename tType >
		BitwiseNeutralType tag_is_bitwise_dismissible( tType const& );

		template< typename tType >
		BitwiseNeutralType tag_is_bitwise_duplicable( tType const& );
		template< typename tType >
		BitwiseNeutralType tag_is_bitwise_relocatable( tType const& );
	};

	template< typename tType, class >
	struct IsBitwiseConjurableImpl
	{
		using type = compat::is_trivially_default_constructible<tType>;
	};
#	if FLUXCFG_FTL_BITWISE == FLUX_FTL_BITWISE_USER
	template< typename tType >
	struct IsBitwiseConjurableImpl<tType, std::true_type>
	{
		using type = std::true_type;
	};
	template< typename tType >
	struct IsBitwiseConjurableImpl<tType, std::false_type>
	{
		using type = std::false_type;
	};
#	endif // ~ BITWISE_USER

#if 0
	template< typename tType, class >
	struct IsBitwiseMaterializableImpl
	{
		using type = compat::is_trivially_constructible<tType,tType const&>;
	};
#	if FLUXCFG_FTL_BITWISE == FLUX_FTL_BITWISE_USER
	template< typename tType >
	struct IsBitwiseMaterializableImpl<tType, std::true_type>
	{
		using type = std::true_type;
	};
	template< typename tType >
	struct IsBitwiseMaterializableImpl<tType, std::false_type>
	{
		using type = std::false_type;
	};
#	endif // ~ BITWISE_USER
#endif

	template< typename tType, class >
	struct IsBitwiseAwakeableImpl
	{
		using type = compat::is_trivially_destructible<tType>;
	};
#	if FLUXCFG_FTL_BITWISE == FLUX_FTL_BITWISE_USER
	template< typename tType >
	struct IsBitwiseAwakeableImpl<tType, std::true_type>
	{
		using type = std::true_type;
	};
	template< typename tType >
	struct IsBitwiseAwakeableImpl<tType, std::false_type>
	{
		using type = std::false_type;
	};
#	endif // ~ BITWISE_USER

	template< typename tType, class >
	struct IsBitwiseDismissibleImpl
	{
		using type = compat::is_trivially_destructible<tType>;
	};
#	if FLUXCFG_FTL_BITWISE == FLUX_FTL_BITWISE_USER
	template< typename tType >
	struct IsBitwiseDismissibleImpl<tType, std::true_type>
	{
		using type = std::true_type;
	};
	template< typename tType >
	struct IsBitwiseDismissibleImpl<tType, std::false_type>
	{
		using type = std::false_type;
	};
#	endif // ~ BITWISE_USER


	template< typename tType, class >
	struct IsBitwiseDuplicableImpl
	{
		using type = compat::is_trivially_copyable<tType>;
	};
#	if FLUXCFG_FTL_BITWISE == FLUX_FTL_BITWISE_USER
	template< typename tType >
	struct IsBitwiseDuplicableImpl<tType, std::true_type>
	{
		using type = std::true_type;
	};
	template< typename tType >
	struct IsBitwiseDuplicableImpl<tType, std::false_type>
	{
		using type = std::false_type;
	};
#	endif // ~ BITWISE_USER



	using def::tag_is_bitwise_conjurable;
	//using def::tag_is_bitwise_materializable;
	using def::tag_is_bitwise_awakeable;
	using def::tag_is_bitwise_dismissible;
	using def::tag_is_bitwise_duplicable;

#	if FLUXCFG_FTL_BITWISE != FLUX_FTL_BITWISE_NEVER
	template< typename tType >
	struct IsBitwiseConjurable
		: IsBitwiseConjurableImpl< 
			tType,
			decltype(tag_is_bitwise_conjurable(std::declval<tType>()))
		>::type
	{};
#if 0
	template< typename tType >
	struct IsBitwiseMaterializable
		: IsBitwiseMaterializableImpl< 
			tType,
			decltype(tag_is_bitwise_materializable(std::declval<tType>()))
		>::type
	{};
#endif
	template< typename tType >
	struct IsBitwiseAwakeable
		: IsBitwiseAwakeableImpl< 
			tType,
			decltype(tag_is_bitwise_awakeable(std::declval<tType>()))
		>::type
	{};

	template< typename tType >
	struct IsBitwiseDismissible
		: IsBitwiseDismissibleImpl< 
			tType,
			decltype(tag_is_bitwise_dismissible(std::declval<tType>()))
		>::type
	{};

	template< typename tType >
	struct IsBitwiseDuplicable
		: IsBitwiseDuplicableImpl< 
			tType,
			decltype(tag_is_bitwise_duplicable(std::declval<tType>()))
		>::type
	{};

#	else // BITWISE_NEVER
	template< typename tType >
	struct IsBitwiseConjurable
		: std::false_type
	{};
#if 0
	template< typename tType >
	struct IsBitwiseMaterializable
		: std::false_type
	{};
#endif
	template< typename tType >
	struct IsBitwiseAwakeable
		: std::false_type
	{};

	template< typename tType >
	struct IsBitwiseDismissible
		: std::false_type
	{};

	template< typename tType >
	struct IsBitwiseDuplicable
		: std::false_type
	{};
#	endif // ~ CFG_BITWISE
};

//--    is_bitwise_*<>                      ///{{{2///////////////////////////
template< typename tType >
struct is_bitwise_conjurable
	: detail::IsBitwiseConjurable<tType>
{};
#if 0
template< typename tType >
struct is_bitwise_materializable
	: detail::IsBitwiseMaterializable<tType>
{};
#endif
template< typename tType >
struct is_bitwise_awakeable
	: detail::IsBitwiseAwakeable<tType>
{};

template< typename tType >
struct is_bitwise_dismissible
	: detail::IsBitwiseDismissible<tType>
{};

template< typename tType >
struct is_bitwise_duplicable
	: detail::IsBitwiseDuplicable<tType>
{};

//--    $ local                             ///{{{2///////////////////////////
namespace detail
{
	template< typename tType, class >
	struct IsBitwiseRelocatableImpl
	{
		using type = std::integral_constant< bool,
			is_bitwise_duplicable<tType>::value &&
			is_bitwise_dismissible<tType>::value
		>;
	};

#	if FLUXCFG_FTL_BITWISE == FLUX_FTL_BITWISE_USER
	template< typename tType >
	struct IsBitwiseRelocatableImpl<tType, std::true_type>
	{
		using type = std::true_type;
	};
	template< typename tType >
	struct IsBitwiseRelocatableImpl<tType, std::false_type>
	{
		using type = std::false_type;
	};
#	endif // ~ BITWISE_USER

	using def::tag_is_bitwise_relocatable;

	template< typename tType >
	struct IsBitwiseRelocatable
		: IsBitwiseRelocatableImpl< 
			tType,
			decltype(tag_is_bitwise_relocatable(std::declval<tType>()))
		>::type 
	{};
}

//--    is_bitwise_*<>                      ///{{{2///////////////////////////
template< typename tType >
struct is_bitwise_relocatable
	: detail::IsBitwiseRelocatable<tType>
{};

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
