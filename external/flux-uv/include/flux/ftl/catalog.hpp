/*-******************************************************* -- HEADER -{{{1- */
/*-	Catalog
 *
 * TODO: alternative to prohibiting forget()/clear()/configure() on retained
 *   keys: register with an optional Signal<> that indicates when such occurs
 *     - muchos complexities. Catalog = already a bit too complex for my taste.
 *
 *
 * TODO: inter-catalog dependencies in e.g. destructor / clear().
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FTL_CATALOG_HPP_E6A01560_6F56_4EF0_A6AF_47DF1671E59D
#define FLUX_FTL_CATALOG_HPP_E6A01560_6F56_4EF0_A6AF_47DF1671E59D

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/identity.hpp>
#include <flux/std/enable_if.hpp>
#include <flux/std/function.hpp>


#include <flux/ftl/sobuffer.hpp>
#include <flux/ftl/catalog_key.hpp>

#include <flux/util/attrib.hpp>

#include <mutex> //TODO: behind config or something?
#include <typeindex>
#include <unordered_map>

//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    constants                           ///{{{2///////////////////////////

enum class ECatalogIdent : uint32_t
{
	indistinct = 0
};

//--    Catalog                             ///{{{2///////////////////////////
/** Object catalog
 *
 * `Catalog` contains a number of items, each identified by a corresponding
 * `CatalogKey`. Items can be added to a `Catalog` via `configure()`. Once
 * configured, items can be retrieved using `acquire()` or `retain()'. Items
 * may be removed from the `Catalog` via `forget()` or by `clear()`in the 
 * whole `Catalog`.
 *
 * \note A key can be set to auto-configure itself when the key's item is
 * retrieved from the `Catalog`. In this case, `acquire()`ing or `retain()`ing
 * the key without it being previously `configure()`ed explicitely will insert
 * a default-instance of the item into the `Catalog`. See `CatalogKey` for
 * further information.
 *
 * Items are reference counted. The reference count is incremented by
 * `retain()` and decremented by `dismiss()`. An item with a non-zero reference
 * count may no be removed via `forget()` or reconfigured via `configure()`.
 * The reference count may be used to model dependencies between multiple
 * objects in a catalog: when a catalog is destroyed, contained items will be
 * destroyed in the order of lowest reference count first.
 *
 * TODO: docs+example
 *
 *
 * TODO:
 * TODO:
 * TODO:  factories + release. (release only for factories)
 * TODO:
 * TODO:  default catalogs
 * TODO:
 * TODO:
 *
 * \see CatalogKey
 */
class Catalog final
{
	public:
		explicit Catalog( ECatalogIdent = ECatalogIdent::indistinct ) noexcept;

		Catalog( Catalog const& ) = delete;
		Catalog& operator= (Catalog const&) = delete;

		~Catalog();

	public:
		/** Check if \a tKey is present in `Catalog`
		 *
		 * \note If \a tKey is eligible for auto-configuration in the current
		 * `Catalog` but has not yet been `acquire()`d, `has()` will return 
		 * `false`. (I.e., it will not auto-configure items even if that would
		 * be possible.)
		 *
		 * \see acquire()
		 */
		template< class tKey >
		bool has( Identity<tKey> = {} ) noexcept;
		
		/** Acquire item with \a tKey
		 *
		 * Acquire the item identified by \a tKey assuming that the item was
		 * previously initialized via `configure()`, or that it is eligible for
		 * auto-configuration.
		 *
		 * The return value of `acquire()` is determined by the key's handle
		 * type. If \a tKey is not present in the current Catalog and
		 * auto-configuration is not possible, `acquire()` will either
		 *   - return an appropriate value (`nullptr`, empty `optional`)
		 *   - throw `errors::CatalogInvalidKey`
		 */
		template< class tKey >
		auto acquire( Identity<tKey> = {} ) -> typename tKey::Handle;

		//template< class tKey > //TODO only for factories!
		//void release( typename tKey::Handle, Identity<tKey> = {} );

		template< class tKey, typename tArg >
		FLUX_ENABLE_IF( catalog_key_is_value<tKey> )
		(void) configure( 
			tArg&&, 
			Identity<tKey> = {} 
		);
		template< class tKey, typename... tArgs >
		FLUX_ENABLE_IF( catalog_key_is_value<tKey> )
		(void) configure( 
			Identity<tKey>, 
			tArgs&&... 
		);
		//TODO: value configure with user destructor function.


		/*template< class tKey >
		void configure( 
			typename tKey::Config,
			typename tKey::Traits,
			Identity<tKey> = {} 
		);*/

		template< class tKey >
		void forget( Identity<tKey> = {} );

		/** Acquire and retain \a tKey's item
		 *
		 * Returns the item assocated with \a tKey, like `acquire()`.
		 * Additionally increases the items reference count. Items with a
		 * non-zero reference count cannot be reconfigured, forgotten or
		 * cleared.
		 *
		 * A `retain()`ed item must be `dismiss()`ed to decrement the item's
		 * reference count.
		 *
		 * \see dismiss()
		 */
		template< class tKey >
		auto retain( Identity<tKey> = {} ) -> typename tKey::Handle;
		/** Dismiss previously retained item
		 *
		 * Decrement the reference count of an item previously `retain()`ed.
		 *
		 * \see retain()
		 */
		template< class tKey >
		void dismiss( Identity<tKey> = {} ) noexcept;

		bool empty() const noexcept;
		void clear( bool = false );
		//void drop_on_floor(); // TODO
	

	private:
		using Buffer_ = ftl::SOBuffer< 32 /*TODO*/ >;
		using DestroyPtr_ = void (*)(void*, Catalog&);

		struct ItemData_
		{
			Buffer_ item;
			flux::unique_function<void (void*, Catalog&)> destroy;
			std::size_t count;
			bool inplace;
		};

	private:
		template< class tKey >
		auto acquire_( Identity<tKey>, std::true_type, bool ) -> typename tKey::Handle;
		template< class tKey >
		auto acquire_( Identity<tKey>, std::false_type, bool ) -> typename tKey::Handle;
		

		template< class tKey >
		void forget_( Identity<tKey>, std::true_type );
		template< class tKey >
		void forget_( Identity<tKey>, std::false_type );
	
		//TODO: methods for the followign cases
		//  - factories??
		
		bool has_( std::type_index ) noexcept;
		bool dismiss_( std::type_index ) noexcept;
		
		void* get_ptr_( std::type_index, CatalogKeyAutoconf const&, bool, std::true_type );
		void* get_ptr_( std::type_index, CatalogKeyAutoconf const&, bool, std::false_type );

		void* alloc_value_store_( std::type_index, std::size_t, bool, std::true_type, DestroyPtr_ = nullptr );
		void* alloc_value_store_( std::type_index, std::size_t, bool, std::false_type, DestroyPtr_ = nullptr );

		void dealloc_value_store_( std::type_index, std::true_type );
		void dealloc_value_store_( std::type_index, std::false_type );

		bool destroy_and_dealloc_value_( std::type_index, std::true_type );
		bool destroy_and_dealloc_value_( std::type_index, std::false_type );
	
		void clear_();
		void clear_force_() noexcept;

	private:
		// Allow NestedCatalog use get_ptr_() et al. This makes the implementation
		// a bit more straight-forward (and we don't need to rely on exceptions).
		template< class > friend class NestedCatalog;
		
	private:
		std::recursive_mutex mMutex;
		std::unordered_map<std::type_index,ItemData_> mItems;
		ECatalogIdent mIdent;
};

//--    NestedCatalog                       ///{{{2///////////////////////////
template< class tParent = Catalog >
class NestedCatalog final
{
	public:
		using Parent = tParent;
	
	public:
		explicit NestedCatalog(
			Parent*,
			ECatalogIdent = ECatalogIdent::indistinct
		) noexcept;

		//~NestedCatalog();

	public:
		template< class tKey >
		bool has( bool aRecurse = true, Identity<tKey> = {} ) noexcept;

		template< class tKey >
		auto acquire( Identity<tKey> = {} ) -> typename tKey::Handle;

		template< class tKey, typename tArg >
		FLUX_ENABLE_IF( catalog_key_is_value<tKey> )
		(void) configure( 
			tArg&&, 
			Identity<tKey> = {} 
		);
		template< class tKey, typename... tArgs >
		FLUX_ENABLE_IF( catalog_key_is_value<tKey> )
		(void) configure( 
			Identity<tKey>, 
			tArgs&&... 
		);

		template< class tKey >
		void forget( Identity<tKey> = {} );


		template< class tKey >
		auto retain( Identity<tKey> = {} ) -> typename tKey::Handle;
		template< class tKey >
		void dismiss( Identity<tKey> = {} ) noexcept;

		bool empty( bool aRecurse = false ) const noexcept;
		void clear( bool aForce = false, bool aRecurse = false );
		//void drop_on_floor(); // TODO
	
	private:
		template< class tKey >
		auto acquire_( Identity<tKey>, std::true_type, bool ) -> typename tKey::Handle;

		template< class tKey >
		void forget_( Identity<tKey>, std::true_type );

	private:
		Catalog mCurrent;
		Parent* mParent;
};

//--    functions                           ///{{{2///////////////////////////
namespace detail
{
	FLUX_ATTR_NORETURN void throw_catalog_unknown_key( std::type_info const& );
}

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/catalog.inl"
#endif // FLUX_FTL_CATALOG_HPP_E6A01560_6F56_4EF0_A6AF_47DF1671E59D
