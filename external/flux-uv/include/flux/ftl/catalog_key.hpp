/*-******************************************************* -- HEADER -{{{1- */
/*-	CatalogKey
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FTL_CATALOG_KEY_HPP_CB6262AA_D337_45B3_B18E_DFF6324853B8
#define FLUX_FTL_CATALOG_KEY_HPP_CB6262AA_D337_45B3_B18E_DFF6324853B8

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/inttypes.hpp>

#include <flux/compat/constexpr.hpp>

//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    forward                             ///{{{2///////////////////////////

class Catalog;

enum class ECatalogIdent : uint32_t;

//--    constants & types                   ///{{{2///////////////////////////

enum class ECatalogKeyKind
{
	value,
	factory
};

/*
enum class ECatalogItemLifetime
{
	normal,
	ephemeral
};*/

struct CatalogKeyAutoconf
{
	ECatalogIdent ident;
	std::size_t typeSize;
	void (*setup)( void*, Catalog& );
	void (*teardown)( void*, Catalog& );
};

extern CatalogKeyAutoconf const kCatalogKeyNoAutoconf;

//--    meta                                ///{{{2///////////////////////////

template< class > struct is_catalog_key;

template< class > struct catalog_key_is_value;
template< class > struct catalog_key_is_factory;


//template< class > struct catalog_must_release; // boolean //TODO

//--    CatalogKey                          ///{{{2///////////////////////////

/** %Catalog key: identifies an item in a `Catalog`
 *
 * Defines a key into a `Catalog`. The key is used to identify a specific item
 * in a `Catalog`.
 *
 * The properties of the `Catalog` item are defined by the key. There are two
 * fundamentally different types of items:
 *   - Value-items
 *   - Factory-items
 *
 * Value-items refer to a single instance of a value/object that is stored by
 * the `Catalog`. \a tType defines the type of the stored object. \a tHandle
 * defines how this object is returned. By default, \a tHandle is equal to 
 * \a tType meaning that acquiring the object will return a copy of it. For
 * example:
   \code
   using VVKey_ = CatalogKey< int, int, ECatalogKeyKind::value, struct ExampleKeyTag_ >;
   
   Catalog c;
   c.configure<VVKey_>( 5 );
   auto x = c.acquire<VVKey_>(); // returns an integer with value 5

   c.forget<VVKey_>();
   auto v = c.acquire<VVKey_>(); // throws ftl::errors::CatalogKeyInvalid
   \endcode
 * The following combinations of \a tType and \a tHandle are also possible:
 *  - \a tType = value, \a tHandle = reference: `acquire()` returns reference
 *    to the internally stored instance of type \a tType
 *  - \a tType = value, \a tHandle = pointer: `acquire()` returns pointer
 *    to the internally stored instance of type \a tType
 *  - \a tType = value, \a tHandle = `observer_ptr`: same as raw pointer
 *  - \a tType = value, \a tHandle = `optional<tType>`: `acquire()` returns
 *    copy of the internally stored object or empty `optional<>` if no value
 *    was associated with the provided key.
 *  - \a tType = reference, \a tHandle = value: `Catalog` stores a reference;
 *    `acquire()` returns copies of the referenced object.
 *  - \a tType = reference, \a tHandle = reference: `Catalog` stores a
 *    reference; `acquire()` returns this reference.
 *  - \a tType = pointer, \a tHandle = pointer: `Catalog` stores a pointer;
 *    `acquire()` returns this pointer. Pointer is not deleted when the 
 *    `Catalog` is destroyed.
 *  - \a tType = `unique_ptr`, \a tHandle = pointer: `Catalog` stores an
 *    `unique_ptr`; `acquire()` returns corresponding raw pointer. (Like 
 *    other objects, `unique_ptr` is destroyed when `Catalog` is destroyed.)
 *  - \a tType = `unique_ptr`, \a tHandle = `observer_ptr`: same as raw pointer
 *  - \a tType = `shared_ptr`, \a tHandle = `shared_ptr`: as expcted
 *  - \a tType = `weak_ptr`, \a tHandle = `shared_ptr`: `Catalog` stores a 
 *    `weak_ptr`, `acquire()` `lock()`s the pointer and returns the result.
 *
 *
 * Factory-items create new instances of values/objects whenever they are
 * retrieved from the `Catalog`.
 *
 * TODO
 *
 *
 * \note The tag \a tTag may be used to create unique `CatalogKey`s for the
 * same combination of properties.
 */
template<
	typename tType,
	typename tHandle = tType,

	class tTag = tType,

	ECatalogKeyKind tKind = ECatalogKeyKind::value,

	CatalogKeyAutoconf const& tAutoConf = kCatalogKeyNoAutoconf

	//ECatalogItemLifetime tLifetime = ECatalogItemLifetime::normal
	//TODO: autoconfig
	
> 
struct CatalogKey
{
	using Type = tType;
	using Handle = tHandle;
	
	static constexpr ECatalogKeyKind kind = tKind;
	//static constexpr CatalogKeyAutoconf const& aconf = tAutoConf;
	//static constexpr ECatalogItemLifetime lifetime = tLifetime;

	static FLUX_CONSTEXPR_EX 
	CatalogKeyAutoconf const& aconf() noexcept
	{
		return tAutoConf;
	}
};


/* TODO: different catalog key properties:
 *
 *   - Values
 *     X handle: value, ref, ptr, shared_ptr
 *     X    maybe: add handle = observer_ptr
 *     X    maybe: add type = uptr + handle = (above)
 *     X    maybe: add type = weak_ptr + handle = shared_ptr
 *     X    maybe: add handle = optional<thing> for returning values without exceptions
 *     
 *     - TODO: reference counting to guarantee destruction order.
 *          - either optional argument to acquire() that increases ref count
 *          - or reserve() that is like acquire but also increments ref count
 *          - release() reduces it
 *          
 *        ISSUE: does this make sense for *all* types? shared_ptr<>?
 *     
 *     - TODO: values that always need to be cleaned or that can just be dropped
 *             on application exit.
 *             
 *
 *
 *   - Factories:
 *     - handle: ptr, shared_ptr, unique_ptr, ?custom unique_ptr(*)
 *
 *    (*) Maybe special unique_ptr with custom deleter that release()s via Catalog
 *
 *   - Autoconfig
 *
 *
 * Rules of thumb: 
 * 	- returned-by-reference doesn't need to be released
 * 	- smart pointers don't need to be released
 *  - pointers do need to be released
 *  - values????  => Yes: easy to return custom handles, No: RAII handles
 *     => NO: easier API
 */

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/catalog_key.inl"
#endif // FLUX_FTL_CATALOG_KEY_HPP_CB6262AA_D337_45B3_B18E_DFF6324853B8
