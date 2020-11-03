/*-******************************************************* -- HEADER -{{{1- */
/*-	Allocate integer IDs from a fixed size pool
 *
 * Allocate (integer) IDs. Based on Humus' code from
 *   - http://www.humus.name/index.php?page=News&ID=368
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FTL_ID_POOL_HPP_6131FD04_F3CE_4426_AF45_71828082FAB6
#define FLUX_FTL_ID_POOL_HPP_6131FD04_F3CE_4426_AF45_71828082FAB6

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/util/attrib.hpp>

#include <flux/std/identity.hpp>
#include <flux/std/inttypes.hpp>

#include <flux/compat/type_traits.hpp>

#include <limits>
#include <type_traits>

//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    limits                              ///{{{2///////////////////////////
namespace limits
{
	/** Limit: maximum number of bytes allocated by `IdPool`
	 *
	 * `IdPool` will never allocate more than `kMaxIdAllocBytes` bytes for the
	 * internal list of ranges over free IDs. Any attempt to do so will raise
	 * a `error::IdPoolMemLimit` exception.
	 *
	 * \see `error::IdPoolMemLimit`
	 */
	constexpr std::size_t kMaxIdAllocBytes = 4*1024*1024;
}

//--    IdPool<>                            ///{{{2///////////////////////////
namespace detail
{
	template< bool, typename tType >
	struct IdPoolIntTypeImpl_ : std::underlying_type<tType>
	{};
	template< typename tType >
	struct IdPoolIntTypeImpl_<false,tType> : Identity<tType>
	{};
	
	template< typename tType >
	struct IdPoolIntType
		: IdPoolIntTypeImpl_< std::is_enum<tType>::value, tType >
	{};
}

/** Integer ID Pool
 *
 * Allocate integer IDs from a fixed size pool.
 *
 * Based on (and largely a copy of) Humus' `MakeID`, originally published at
 * http://www.humus.name/index.php?page=News&ID=368.
 */
template< typename tId >
class IdPool
{
	public:
		using Id = tId;
		using IntType = typename detail::IdPoolIntType<tId>::type;	

	public:
		/** Construct `IdPool`
		 *
		 * Constructs `IdPool` with a specified amount of free IDs \f$N\f$, 
		 * ranging from \f$\left[0 ... N\right)\f$. If not specified, \f$N\f$
		 * defaults to `std::numeric_limits<IntType>::%max()`.
		 */
		explicit 
		IdPool( IntType = std::numeric_limits<IntType>::max() );

		~IdPool();

		IdPool( IdPool const& ) = delete;
		IdPool& operator= (IdPool const&) = delete;

	public:
		/** Test if ID is allocated
		 */
		bool is_id( Id ) const noexcept;

		/** Allocate ID
		 * 
		 * Allocates and returns free ID. If no further IDs can be allocated,
		 * `alloc()` throws `error::IdPoolExhausted`.
		 *
		 * \note `alloc()` never performs any memory allocations.
		 *  
		 * \see try_alloc
		 */
		Id alloc();
		/** Allocate consecutive range of IDs
		 *
		 * Allocates consecutive range of free IDs. Returns the value of the
		 * lowest ID in the allocated range. If no sufficiently large free
		 * range of IDs exists, the allocation fails, and `alloc_range()`
		 * throws `error::IdPoolExhausted`.
		 *
		 * \note `alloc_range()` never performs any memory allocations.
		 *  
		 * \see try_alloc_range
		 */
		Id alloc_range( IntType );

		/** Try to allocate ID
		 *
		 * Tries to allocate free ID. On success, returns true and stores the
		 * allocated ID in the variable referred to by the argument. If no
		 * further IDs can be allocated, false is returned, and the argument
		 * remains unmodified.
		 *
		 * \note `try_alloc()` never performs any memory allocations.
		 *  
		 * \see alloc
		 */
		bool try_alloc( Id& ) noexcept;
		/** Try to allocate a range of IDs
		 *
		 * Tries to allocate a consecutive range of free ID. Returns true on
		 * success, and places the lowest ID of the allocated range in the
		 * varaible referred to by the first argument. If the range cannot be
		 * allocated, false is returned and the argument remains unmodified.
		 *
		 * \note `try_alloc_range()` never performs any memory allocations.
		 *  
		 * \see alloc_range
		 */
		bool try_alloc_range( Id&, IntType ) noexcept;

		/** Release previously allocated ID
		 *
		 * Releases previously allocated ID and makes it available for future
		 * allocations. If an attempt to release an unallocated ID (i.e, never
		 * before allocated, already released or an ID outside of the `IdPool`s
		 * range) is made, `release()` throws `error::IdNotAllocated`.
		 *
		 * `release()` occasionally allocates memory, and can therefore throw
		 * `error::IdPoolMemLimit`. This also occurs if total memory required
		 * to hold free ID ranges by this `IdPool` instance exceeds
		 * `limits::kMaxIdAllocBytes`.
		 */
		void release( Id );
		/** Release consecutive range of allocated IDs
		 *
		 * Releases the consecutive range of IDs \f$\left[\mbox{aStart} ...
		 * \mbox{aStart}+\mbox{aEnd}\right)\f$. If any ID in that range is not
		 * currently allocated, `release_range()` throws
		 * `error::IdPoolNotAllocated`. 
		 *
		 * `release_range()` occasionally allocates memory; see `release()`
		 * for information on this.
		 *
		 * \see release
		 */
		void release_range( Id, IntType );

		/** Return number of allocated IDs
		 */
		IntType count_allocated() const noexcept;
		/** Return number of available (unallocated) IDs
		 */
		IntType count_available() const noexcept;

	private:
		void insert_range_( std::size_t );
		void delete_range_( std::size_t ) noexcept;

	private:
		struct Range_
		{
			IntType first, last;
		};

	private:
		Range_* mRanges;
		std::size_t mRangeCount, mRangeCap;
		IntType mLast;
};


//--    functions                           ///{{{2///////////////////////////

namespace detail
{
	FLUX_ATTR_NORETURN void throw_id_pool_mem_limit();
	FLUX_ATTR_NORETURN void throw_id_pool_exhausted();
	FLUX_ATTR_NORETURN void throw_id_pool_not_allocated( int64_t, std::size_t );
}

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/id_pool.inl"
#endif // FLUX_FTL_ID_POOL_HPP_6131FD04_F3CE_4426_AF45_71828082FAB6
