/*-******************************************************* -- HEADER -{{{1- */
/*-	Small-object optimized storage buffer
 */
/*-***************************************************************** -}}}1- */
#
#ifndef FLUX_FTL_SOBUFFER_HPP_E7EAF2A3_4260_45D1_A618_8C470534B157
#define FLUX_FTL_SOBUFFER_HPP_E7EAF2A3_4260_45D1_A618_8C470534B157

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/static_assert.hpp>

#include <flux/compat/constexpr.hpp>

#include <cstddef>
#include <type_traits>

//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    SOBuffer<>                          ///{{{2///////////////////////////
namespace detail
{
	template< typename tType, std::size_t, std::size_t >
	struct SOBufferEligible;
}

/** Small-object optimized buffer
 *
 * Small-object optimized storage buffer. Provides a storage buffer for
 * arbitrary objects, where small objects are stored in-place.
 *
 * Objects of size up to \a tSize, and with alignment requirements compatible
 * with \a tAlign will be stored in-place in the `SOBuffer`. Larger objects,
 * and/or objects with incompatible alignment will instead be placed in
 * dynamically allocated memory.
 *
 * \note Doesn't provide any higher-level support features, such as storing
 * information on whether or not an object is currently being held by the
 * `SOBuffer`, or indeed what type such an object would have. The `SOBuffer` is
 * intended to be used by higher-level objects that store this information
 * elsewhere (or indeed know it upfront).
 *
 * \warning Objects placed in the `SOBuffer` **must** be manually destroyed
 * using the `destroy()` or `destroy_with_allocator()` method with the correct
 * `tType` argument.
 */
template< std::size_t tSize = sizeof(void*), std::size_t tAlign = alignof(void*) >
class SOBuffer final
{
	public:
		/** Is \a tType eligible for in-place storage? 
		 *
		 * Meta-function that indicates whether or not \a tType is eligible for
		 * in-place storage. Inherits from `std::integral_constant<bool,...>`,
		 * so it can be implicitly converted to either `std::true_type` or
		 * `std::false_type`.
		 */
		template< typename tType > 
		struct IsInPlace 
			: std::integral_constant< 
				bool, 
				detail::SOBufferEligible<tType,tSize,tAlign>::value
			>
		{};

		/** Is \a tType nothrow constructible?
		 *
		 * Meta-function that indicates whether or not \a tType is nothrow
		 * constructible from \a tArgs into the `SOBuffer`. \a tType is nothrow
		 * constructible if it is eligible for in-place storage, and if it is
		 * nothrow constructible from \a tArgs.
		 *
		 * \see IsInPlace, std::is_nothrow_constructible
		 */
		template< typename tType, typename... tArgs >
		struct IsNothrowConstructible
			: std::integral_constant<
				bool,
				   IsInPlace<tType>::value 
				&& std::is_nothrow_constructible<tType,tArgs...>::value
			>
		{};
		/** Is \a tType nothrow destructible?
		 *
		 * Meta-function that indicates whether or not a held \a tType is
		 * nothrow destructible. \a tType is nothrow destructible if it is
		 * eligible for in-place storage, and if it is nothrow destructible in
		 * the first place.
		 *
		 * \see IsInPlace
		 */
		template< typename tType >
		struct IsNothrowDestructible
			: std::integral_constant<
				bool,
				   IsInPlace<tType>::value 
				&& std::is_nothrow_destructible<tType>::value
			>
		{};

	
	public:
		SOBuffer() = default;

		SOBuffer( SOBuffer const& ) = delete;
		SOBuffer& operator= (SOBuffer const&) = delete;
	
	public:
		template< typename tType, typename... tArgs > FLUX_CONSTEXPR_EX
		tType* construct( tArgs&&... ) noexcept((IsNothrowConstructible<tType,tArgs...>::value));
		template< typename tType, class tAlloc, typename... tArgs > FLUX_CONSTEXPR_EX
		tType* construct_with_allocator( tAlloc&, tArgs&&... );

		template< typename tType > FLUX_CONSTEXPR_EX
		void destroy() noexcept(IsNothrowDestructible<tType>::value);
		template< typename tType, class tAlloc > FLUX_CONSTEXPR_EX
		void destroy_with_allocator( tAlloc& );

		template< typename tType > FLUX_CONSTEXPR_EX
		tType& reference() & noexcept;
		template< typename tType > FLUX_CONSTEXPR_EX
		tType&& reference() && noexcept;
		template< typename tType > FLUX_CONSTEXPR_EX
		tType const& reference() const& noexcept;

		// "raw" access for use in type-erased context
		FLUX_CONSTEXPR_EX void* get_raw_ptr( std::true_type ) noexcept;
		FLUX_CONSTEXPR_EX void* get_raw_ptr( std::false_type ) noexcept;

		FLUX_CONSTEXPR_EX void const* get_raw_ptr( std::true_type ) const noexcept;
		FLUX_CONSTEXPR_EX void const* get_raw_ptr( std::false_type ) const noexcept;

		void* allocate_raw( std::size_t, std::true_type ) noexcept;
		void* allocate_raw( std::size_t, std::false_type );

		void free_raw( std::true_type ) noexcept;
		void free_raw( std::false_type );

	private:
		using Buffer_ = typename std::aligned_storage<tSize,tAlign>::type;
		Buffer_ mBuffer;
};

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/sobuffer.inl"
#endif // FLUX_FTL_SOBUFFER_HPP_E7EAF2A3_4260_45D1_A618_8C470534B157
