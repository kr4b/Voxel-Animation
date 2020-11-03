/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- Catalog implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/std/assert.hpp>
#include <flux/std/optional.hpp>
#include <flux/std/shared_ptr.hpp>
#include <flux/std/unique_ptr.hpp>
#include <flux/std/observer_ptr.hpp>

#include <typeinfo>
#include <functional>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    $ detail                            ///{{{2///////////////////////////
namespace detail
{
	// Catalog Value: internal representations
	template< typename tType >
	struct CatValueInternalRep_
	{
		using type = tType;
	};
	template< typename tType >
	struct CatValueInternalRep_< tType& >
	{
		using type = std::reference_wrapper<tType>;
	};
	template< typename tType >
	struct CatValueInternalRep_< tType const& >
	{
		using type = std::reference_wrapper<tType const>;
	};

	// Catalog Value: untyped pointer to handle
	template< typename tHandle, typename tType > 
	struct CatValuePtrToHandle_
	{
		constexpr
		static tHandle enact( void* aPtr ) noexcept
		{
			return FLUX_ASSERT(aPtr), *reinterpret_cast<tType*>(aPtr);
		}
	};

	template< typename tType >
	struct CatValuePtrToHandle_< tType*, tType >
	{
		constexpr
		static tType* enact( void* aPtr ) noexcept
		{
			return FLUX_ASSERT(aPtr), reinterpret_cast<tType*>(aPtr);
		}
	};
	template< typename tType >
	struct CatValuePtrToHandle_< tType const*, tType >
	{
		constexpr
		static tType const* enact( void* aPtr ) noexcept
		{
			return FLUX_ASSERT(aPtr), reinterpret_cast<tType const*>(aPtr);
		}
	};
	template< typename tType >
	struct CatValuePtrToHandle_< observer_ptr<tType>, tType >
	{
		constexpr
		static observer_ptr<tType> enact( void* aPtr ) noexcept
		{
			return FLUX_ASSERT(aPtr), observer_ptr<tType>(reinterpret_cast<tType*>(aPtr));
		}
	};
	template< typename tType >
	struct CatValuePtrToHandle_< shared_ptr<tType>, tType >
	{
		constexpr
		static shared_ptr<tType> const& enact( void* aPtr ) noexcept
		{
			return FLUX_ASSERT(aPtr), *reinterpret_cast<shared_ptr<tType>*>(aPtr);
		}
	};
	template< typename tType >
	struct CatValuePtrToHandle_< optional<tType>, tType >
	{
		constexpr
		static optional<tType> enact( void* aPtr ) noexcept
		{
			return FLUX_ASSERT(aPtr), optional<tType>(*reinterpret_cast<tType*>(aPtr));
		}
	};

	template< typename tType >
	struct CatValuePtrToHandle_< shared_ptr<tType>, weak_ptr<tType> >
	{
		constexpr
		static shared_ptr<tType> enact( void* aPtr ) noexcept
		{
			return FLUX_ASSERT(aPtr), reinterpret_cast<weak_ptr<tType>*>(aPtr)->lock();
		}
	};
	template< typename tType >
	struct CatValuePtrToHandle_< tType*, unique_ptr<tType> >
	{
		constexpr
		static tType* enact( void* aPtr ) noexcept
		{
			return FLUX_ASSERT(aPtr), reinterpret_cast<unique_ptr<tType>*>(aPtr)->get();
		}
	};
	template< typename tType >
	struct CatValuePtrToHandle_< observer_ptr<tType>, unique_ptr<tType> >
	{
		constexpr
		static observer_ptr<tType> enact( void* aPtr ) noexcept
		{
			return FLUX_ASSERT(aPtr), observer_ptr<tType>( reinterpret_cast<unique_ptr<tType>*>(aPtr)->get() );
		}
	};

	template< typename tType >
	struct CatValuePtrToHandle_< tType&, tType* >
	{
		constexpr
		static tType& enact( void* aPtr ) noexcept
		{
			return FLUX_ASSERT(aPtr), **reinterpret_cast<tType**>(aPtr); //XXX-verify
		}
	};
	template< typename tType >
	struct CatValuePtrToHandle_< tType const&, tType const* >
	{
		constexpr
		static tType const& enact( void* aPtr ) noexcept
		{
			return FLUX_ASSERT(aPtr), **reinterpret_cast<tType**>(aPtr); //XXX-verify
		}
	};
	//TODO: convert other points to references


	//TODO: convert to const versions of shared_ptr, observer_ptr (e.g.,
	// observer_ptr<tType const> etc)


	// Catalog Value: null pointer to null-handle
	template< typename tHandle, class tKey >
	struct CatNullPtrToHandle_
	{
		static tHandle enact()
		{
			throw_catalog_unknown_key( typeid(tKey) );
		}
	};
	template< typename tType, class tKey >
	struct CatNullPtrToHandle_< tType*, tKey >
	{
		constexpr
		static tType* enact() noexcept
		{
			return nullptr;
		}
	};
	template< typename tType, class tKey >
	struct CatNullPtrToHandle_< shared_ptr<tType>, tKey >
	{
		constexpr
		static shared_ptr<tType> enact() noexcept
		{
			return shared_ptr<tType>();
		}
	};
	template< typename tType, class tKey >
	struct CatNullPtrToHandle_< optional<tType>, tKey >
	{
		constexpr
		static optional<tType> enact() noexcept
		{
			return optional<tType>();
		}
	};

	// NestedCatalog helpers
	template< typename tPar > inline
	bool ncat_pempty_( tPar* aPar, bool (tPar::*aFn)() const, bool ) noexcept
	{
		return aPar && (aPar->*aFn)();
	}
	template< typename tPar> inline
	bool ncat_pempty_( tPar* aPar, bool (tPar::*aFn)(bool) const, bool aRec ) noexcept
	{
		return aPar && (aPar->*aFn)( aRec );
	}

	template< typename tPar > inline
	void ncat_pclear_( tPar* aPar, void (tPar::*aFn)(bool), bool aForce, bool )
	{
		if( aPar ) (aPar->*aFn)( aForce );
	}
	template< typename tPar> inline
	void ncat_pclear_( tPar* aPar, bool (tPar::*aFn)(bool,bool), bool aForce, bool aRec )
	{
		if( aPar ) (aPar->*aFn)( aForce, aRec );
	}

}

//--    Catalog                             ///{{{2///////////////////////////

	// -public- {{{3
template< class tKey > inline
bool Catalog::has( Identity<tKey> ) noexcept
{
	return has_( std::type_index(typeid(tKey)) );
}

template< class tKey > inline
auto Catalog::acquire( Identity<tKey> ) -> typename tKey::Handle
{
	return acquire_( Identity<tKey>{}, catalog_key_is_value<tKey>{}, false );
}

template< class tKey, typename tArg > inline
FLUX_ENABLE_IF( catalog_key_is_value<tKey> )
(void) Catalog::configure( tArg&& aArg, Identity<tKey> )
{
	using Ty_ = typename detail::CatValueInternalRep_< typename tKey::Type >::type;
	using InPlace_ = Buffer_::template IsInPlace<Ty_>;
	using FPtr_ = void (*)(void*, Catalog&);

	auto const ti = std::type_index(typeid(tKey));
	auto* ptr = alloc_value_store_( ti, sizeof(Ty_), false, InPlace_{},
		static_cast<FPtr_>([](void* aPtr, Catalog&) {
			reinterpret_cast<Ty_*>(aPtr)->~Ty_();
	}) );
	FLUX_ASSERT( ptr );
	
	try
	{
		new (ptr) Ty_( std::forward<tArg>(aArg) );
	}
	catch( ... )
	{
		dealloc_value_store_( ti, InPlace_{} );
		throw;
	}
}
template< class tKey, typename... tArgs > inline
FLUX_ENABLE_IF( catalog_key_is_value<tKey> )
(void) Catalog::configure( Identity<tKey>, tArgs&&... aArgs )
{
	using Ty_ = typename detail::CatValueInternalRep_< typename tKey::Type >::type;
	using InPlace_ = Buffer_::template IsInPlace<Ty_>;
	using FPtr_ = void (*)(void*, Catalog&);

	auto const ti = std::type_index(typeid(tKey));
	auto* ptr = alloc_value_store_( ti, sizeof(Ty_), false, InPlace_{}, 
		static_cast<FPtr_>([](void* aPtr, Catalog&) {
			reinterpret_cast<Ty_*>(aPtr)->~Ty_();
	}) );
	FLUX_ASSERT( ptr );
	
	try
	{
		new (ptr) Ty_( std::forward<tArgs>(aArgs)... );
	}
	catch( ... )
	{
		dealloc_value_store_( ti, InPlace_{} );
		throw;
	}
}


template< class tKey > inline
void Catalog::forget( Identity<tKey> )
{
	forget_( Identity<tKey>{}, catalog_key_is_value<tKey>{} );
}

template< class tKey > inline
auto Catalog::retain( Identity<tKey> ) -> typename tKey::Handle
{
	return acquire_( Identity<tKey>{}, catalog_key_is_value<tKey>{}, true );
}
template< class tKey > inline
void Catalog::dismiss( Identity<tKey> ) noexcept
{
	dismiss_( std::type_index(typeid(tKey)) );
}


	// -private- {{{3
template< class tKey > inline
auto Catalog::acquire_( Identity<tKey>, std::true_type, bool aRetain ) -> typename tKey::Handle
{
	using Ty_ = typename detail::CatValueInternalRep_< typename tKey::Type >::type;
	using Ha_ = typename tKey::Handle;
	using InPlace_ = Buffer_::template IsInPlace<Ty_>;

	if( auto* ptr = get_ptr_( std::type_index(typeid(tKey)), tKey::aconf(), aRetain, InPlace_{} ) )
		return detail::CatValuePtrToHandle_<Ha_,Ty_>::enact(ptr);

	return detail::CatNullPtrToHandle_<Ha_,tKey>::enact();
}
template< class tKey > inline
auto Catalog::acquire_( Identity<tKey>, std::false_type, bool ) -> typename tKey::Handle
{
	//TODO: factory type.
	printf( "ACQUIRE FACTORY (%s)!\n", typeid(tKey).name() );
	using Ha_ = typename tKey::Handle;
	return Ha_{};
}

template< class tKey > inline
void Catalog::forget_( Identity<tKey>, std::true_type )
{
	//using Ty_ = typename tKey::Type;
	using Ty_ = typename detail::CatValueInternalRep_< typename tKey::Type >::type;
	using InPlace_ = Buffer_::template IsInPlace<Ty_>;

	destroy_and_dealloc_value_( std::type_index(typeid(tKey)), InPlace_{} );
}
template< class tKey > inline
void Catalog::forget_( Identity<tKey>, std::false_type )
{
	//TODO: factory type
}

//--    NestedCatalog                       ///{{{2///////////////////////////

	// [cd]tor -{{{3-
template< class tParent > inline
NestedCatalog<tParent>::NestedCatalog( Parent* aParent, ECatalogIdent aIdent ) noexcept
	: mCurrent( aIdent )
	, mParent( aParent )
{}

#if 0
template< class tParent > inline
NestedCatalog<tParent>::~NestedCatalog()
{
	/* This is a bit roundabout at the moment. We first try to nicely clear
	 * the current catalog as much as possible, then the parent, then again
	 * the current etc etc etc.
	 *
	 * This is a shitty way of resolving inter-catalog dependencies.
	 */
}
#endif

	// public -{{{3-
template< class tParent > template< class tKey > inline
bool NestedCatalog<tParent>::has( bool aRecurse, Identity<tKey> ) noexcept
{
	return mCurrent.has( Identity<tKey>{} )
		|| (aRecurse && mParent && mParent->has( Identity<tKey>{} ))
	;
}

template< class tParent > template< class tKey > inline
auto NestedCatalog<tParent>::acquire( Identity<tKey> ) -> typename tKey::Handle
{
	return acquire_( Identity<tKey>{}, catalog_key_is_value<tKey>{}, false );
}

template< class tParent > template< class tKey, typename tArg > inline
FLUX_ENABLE_IF( catalog_key_is_value<tKey> )
(void) NestedCatalog<tParent>::configure( tArg&& aArg, Identity<tKey> )
{
	return mCurrent.configure( std::forward<tArg>(aArg), Identity<tKey>{} );
}
template< class tParent > template< class tKey, typename... tArgs > inline
FLUX_ENABLE_IF( catalog_key_is_value<tKey> )
(void) NestedCatalog<tParent>::configure( Identity<tKey>, tArgs&&... aArgs )
{
	return mCurrent.configure( Identity<tKey>{}, std::forward<tArgs>(aArgs)... );
}

template< class tParent > template< class tKey > inline
void NestedCatalog<tParent>::forget( Identity<tKey> )
{
	forget_( Identity<tKey>{}, catalog_key_is_value<tKey>{} );
}

template< class tParent > template< class tKey > inline
auto NestedCatalog<tParent>::retain( Identity<tKey> ) -> typename tKey::Handle
{
	return acquire_( Identity<tKey>{}, catalog_key_is_value<tKey>{}, true );
}

template< class tParent > template< class tKey > inline
void NestedCatalog<tParent>::dismiss( Identity<tKey> ) noexcept
{
	if( !mCurrent.dismiss_( std::type_index(typeid(tKey)) ) )
	{
		if( mParent ) mParent->dismiss( Identity<tKey>{} );
	}
}


template< class tParent > inline
bool NestedCatalog<tParent>::empty( bool aRecurse ) const noexcept
{
	return mCurrent.empty()
		&& (!aRecurse || detail::ncat_pempty_( mParent, &Parent::empty, aRecurse ) )
	;
}
template< class tParent > inline
void NestedCatalog<tParent>::clear( bool aForce, bool aRecurse )
{
	mCurrent.clear( aForce );

	if( aRecurse )
		detail::ncat_pclear_( mParent, &Parent::clear, aForce, aRecurse );
}


	// private -{{{3-
template< class tParent > template< class tKey > inline
auto NestedCatalog<tParent>::acquire_( Identity<tKey>, std::true_type, bool aRetain ) -> typename tKey::Handle
{
	/* TODO: only for values?? */
	using Ty_ = typename detail::CatValueInternalRep_< typename tKey::Type >::type;
	using Ha_ = typename tKey::Handle;
	using InPlace_ = Catalog::Buffer_::template IsInPlace<Ty_>;

	auto const ti = std::type_index(typeid(tKey));
	if( auto* ptr = mCurrent.get_ptr_( ti, tKey::aconf(), aRetain, InPlace_{} ) )
		return detail::CatValuePtrToHandle_<Ha_,Ty_>::enact(ptr);

	return mParent
		? mParent->acquire( Identity<tKey>{} )
		: detail::CatNullPtrToHandle_<Ha_,tKey>::enact()
	;
}

template< class tParent > template< class tKey > inline
void NestedCatalog<tParent>::forget_( Identity<tKey>, std::true_type )
{
	using Ty_ = typename detail::CatValueInternalRep_< typename tKey::Type >::type;
	using InPlace_ = Catalog::Buffer_::template IsInPlace<Ty_>;

	auto const ti = std::type_index(typeid(tKey));
	if( !mCurrent.destroy_and_dealloc_value_( ti, InPlace_{} ) )
	{
		if( mParent ) mParent->forget( Identity<tKey>{} );
	}
}

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
