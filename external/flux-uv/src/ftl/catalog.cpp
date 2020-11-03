/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Catalog implementation
 *
 * TODO: see if it's possible to make the locking more granular. I.e., can we
 * avoid holding the lock when a user-function is running? (If yes, we can
 * maybe avoid using a recursive_mutex too...)
 */
/*-***************************************************************** -}}}1- */

#include <flux/ftl/catalog.hpp>
#include <flux/ftl/errors.hpp>

#include <flux/std/throw.hpp>

#include <tuple>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
}

//--    Catalog                             ///{{{2///////////////////////////

	// [cd]tor --{{{3-
Catalog::Catalog( ECatalogIdent aIdent ) noexcept
	: mIdent(aIdent)
{}

Catalog::~Catalog()
{
	clear_force_();
}

	// public --{{{3-
bool Catalog::empty() const noexcept
{
	return mItems.empty();
}
void Catalog::clear( bool aForce )
{
	if( aForce ) clear_force_();
	else clear_();
}

	// private --{{{3-
bool Catalog::has_( std::type_index aIndex ) noexcept
{
	std::unique_lock<std::recursive_mutex> lock(mMutex);
	return mItems.end() != mItems.find( aIndex );
}
bool Catalog::dismiss_( std::type_index aIndex ) noexcept
{
	std::unique_lock<std::recursive_mutex> lock(mMutex);
	
	auto it = mItems.find( aIndex );
	if( mItems.end() == it )
		return false;
		
	//TODO: LOG() when count is zero and dismiss() is called.
	if( it->second.count > 0 )
		--it->second.count;

	//TODO: should we delete the object now? depends on lifetime...
	
	return true;
}

void* Catalog::get_ptr_( std::type_index aIndex, CatalogKeyAutoconf const& aAConf, bool aRetain, std::true_type )
{
	std::unique_lock<std::recursive_mutex> lock(mMutex);

	auto it = mItems.find( aIndex );
	if( mItems.end() == it )
	{
		if( &kCatalogKeyNoAutoconf != &aAConf && (ECatalogIdent::indistinct == aAConf.ident || aAConf.ident == mIdent) )
		{
			void* ptr = alloc_value_store_( aIndex, aAConf.typeSize, aRetain, std::true_type{}, aAConf.teardown );
			FLUX_ASSERT( ptr );

			try
			{
				aAConf.setup( ptr, *this );
			}
			catch( ... )
			{
				dealloc_value_store_( aIndex, std::true_type{} );
				throw;
			}

			return ptr;
		}

		return nullptr;
	}

	if( aRetain ) ++it->second.count;
	return it->second.item.get_raw_ptr( std::true_type{} );
}
void* Catalog::get_ptr_( std::type_index aIndex, CatalogKeyAutoconf const& aAConf, bool aRetain, std::false_type )
{
	std::unique_lock<std::recursive_mutex> lock(mMutex);

	auto it = mItems.find( aIndex );
	if( mItems.end() == it )
	{
		if( &kCatalogKeyNoAutoconf != &aAConf && (ECatalogIdent::indistinct == aAConf.ident || aAConf.ident == mIdent) )
		{
			void* ptr = alloc_value_store_( aIndex, aAConf.typeSize, aRetain, std::false_type{}, aAConf.teardown );
			FLUX_ASSERT( ptr );

			try
			{
				aAConf.setup( ptr, *this );
			}
			catch( ... )
			{
				dealloc_value_store_( aIndex, std::false_type{} );
				throw;
			}

			return ptr;
		}

		return nullptr;
	}

	if( aRetain ) ++it->second.count;
	return it->second.item.get_raw_ptr( std::false_type{} );
}

void* Catalog::alloc_value_store_( std::type_index aIndex, std::size_t aSize, bool aRetain, std::true_type, DestroyPtr_ aDestroy )
{
	std::unique_lock<std::recursive_mutex> lock(mMutex);

	auto const rs = mItems.emplace(
		std::piecewise_construct,
		std::forward_as_tuple(aIndex),
		std::forward_as_tuple()
	);

	auto it = std::get<0>(rs);
	if( !std::get<1>(rs) )
	{
		FLUX_THROW_IF( it->second.count != 0, ftl::error::CatalogKeyRetained )
			<< einfo::ErrorDesc( "Catalog item has been retained and cannot be changed" )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("CatalogKey = '{}', count = {:d}"), aIndex.name(), it->second.count ) 
		;
		
		void* ret = it->second.item.get_raw_ptr( std::true_type{} );
		it->second.destroy( ret, *this );
		return ret;
	}

	it->second.inplace = true;
	it->second.count = (aRetain ? 1:0);
	it->second.destroy = aDestroy;
	return it->second.item.allocate_raw( aSize, std::true_type{} );
}
void* Catalog::alloc_value_store_( std::type_index aIndex, std::size_t aSize, bool aRetain, std::false_type, DestroyPtr_ aDestroy )
{
	std::unique_lock<std::recursive_mutex> lock(mMutex);

	auto const rs = mItems.emplace(
		std::piecewise_construct,
		std::forward_as_tuple(aIndex),
		std::forward_as_tuple()
	);

	auto it = std::get<0>(rs);
	if( !std::get<1>(rs) )
	{
		FLUX_THROW_IF( it->second.count != 0, ftl::error::CatalogKeyRetained )
			<< einfo::ErrorDesc( "Catalog item has been retained and cannot be changed" )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("CatalogKey = '{}', count = {:d}"), aIndex.name(), it->second.count ) 
		;
		
		void* ret = it->second.item.get_raw_ptr( std::false_type{} );
		it->second.destroy( ret, *this );
		return ret;
	}

	it->second.inplace = false;
	it->second.count = (aRetain ? 1:0);
	it->second.destroy = aDestroy;
	return it->second.item.allocate_raw( aSize, std::false_type{} );
}

void Catalog::dealloc_value_store_( std::type_index aIndex, std::true_type )
{
	std::unique_lock<std::recursive_mutex> lock(mMutex);

	auto const it = mItems.find( aIndex );
	if( it != mItems.end() )
	{
		it->second.item.free_raw( std::true_type{} ); // NOTE: this is a noop!
		mItems.erase( it );
	}
}
void Catalog::dealloc_value_store_( std::type_index aIndex, std::false_type )
{
	std::unique_lock<std::recursive_mutex> lock(mMutex);

	auto const it = mItems.find( aIndex );
	if( it != mItems.end() )
	{
		it->second.item.free_raw( std::false_type{} );
		mItems.erase( it );
	}
}

bool Catalog::destroy_and_dealloc_value_( std::type_index aIndex, std::true_type )
{
	std::unique_lock<std::recursive_mutex> lock(mMutex);

	auto const it = mItems.find( aIndex );
	if( it == mItems.end() )
		return false;

	FLUX_THROW_IF( it->second.count != 0, ftl::error::CatalogKeyRetained )
		<< einfo::ErrorDesc( "Catalog item has been retained and cannot be destroyed" )
		<< einfo::ErrorMeta( FLUX_FMT_STRING("CatalogKey = '{}', count = {:d}"), aIndex.name(), it->second.count ) 
	;

	it->second.destroy( it->second.item.get_raw_ptr( std::true_type{} ), *this );
	it->second.item.free_raw( std::true_type{} );
	mItems.erase( it );

	return true;
}
bool Catalog::destroy_and_dealloc_value_( std::type_index aIndex, std::false_type )
{
	std::unique_lock<std::recursive_mutex> lock(mMutex);

	auto const it = mItems.find( aIndex );
	if( it == mItems.end() )
		return false;

	FLUX_THROW_IF( it->second.count != 0, ftl::error::CatalogKeyRetained )
		<< einfo::ErrorDesc( "Catalog item has been retained and cannot be destroyed" )
		<< einfo::ErrorMeta( FLUX_FMT_STRING("CatalogKey = '{}', count = {:d}"), aIndex.name(), it->second.count ) 
	;

	it->second.destroy( it->second.item.get_raw_ptr( std::false_type{} ), *this );
	it->second.item.free_raw( std::false_type{} );
	mItems.erase( it );

	return true;
}


void Catalog::clear_force_() noexcept
{
	std::unique_lock<std::recursive_mutex> lock(mMutex);

	/* This is slightly lame. */

	std::size_t maxCount = 0;
	do
	{
		std::size_t minCount = std::numeric_limits<std::size_t>::max();
		for( auto it = mItems.begin(); it != mItems.end(); )
		{
			auto& entry = *it;
			
			if( entry.second.count > maxCount )
			{
				minCount = std::min( minCount, entry.second.count );
				++it;
				continue;
			}

			//TODO: log this item if entry.second.count > 0
			
			auto& item = entry.second;
			if( item.inplace )
			{
				item.destroy( item.item.get_raw_ptr( std::true_type{} ), *this );
				item.item.free_raw( std::true_type{} );
			}
			else
			{
				item.destroy( item.item.get_raw_ptr( std::false_type{} ), *this );
				item.item.free_raw( std::false_type{} );
			}

			it = mItems.erase( it );
			minCount = 0;
		}

		maxCount = minCount;
	} while( !mItems.empty() );

	mItems.clear();
}
void Catalog::clear_()
{
	std::unique_lock<std::recursive_mutex> lock(mMutex);

	bool changed;
	do
	{
		changed = false;
		
		for( auto it = mItems.begin(); it != mItems.end(); )
		{
			auto& entry = *it;
			if( entry.second.count )
			{
				++it;
				continue;
			}
			
			auto& item = entry.second;
			if( item.inplace )
			{
				item.destroy( item.item.get_raw_ptr( std::true_type{} ), *this );
				item.item.free_raw( std::true_type{} );
			}
			else
			{
				item.destroy( item.item.get_raw_ptr( std::false_type{} ), *this );
				item.item.free_raw( std::false_type{} );
			}

			it = mItems.erase( it );
			changed = true;
		}
	} while( !mItems.empty() && changed );

	FLUX_THROW_IF( !mItems.empty(), ftl::error::CatalogKeyRetained )
		<< einfo::ErrorDesc( "Cannot clear Catalog, contains retained items" )
	;

	mItems.clear();
}

//--    d::throw_catalog_*()                ///{{{2///////////////////////////
namespace detail
{
	FLUX_ATTR_NORETURN
	void throw_catalog_unknown_key( std::type_info const& aInfo )
	{
		FLUX_THROW( error::CatalogKeyInvalid )
			<< einfo::ErrorMeta( FLUX_FMT_STRING("CatalogKey = '{}'"), aInfo.name() )
		;
	}
}

//--    <<< ~ flux::base namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(base)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
