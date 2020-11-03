/*-******************************************************* -- HEADER -{{{1- */
/*-	AnyT<> & Any
 *
 * `Any`, a object that can hold any other (copyable) type. This predates the
 * std::any / std::experimental::any; it's instead somewhat inspired by
 * boost::any. The API differs, though, and unlike std::any, ftl::Any can be
 * made to support comparison, ordering and hashing (which makes it eligible
 * as a key for most standard associative containers in C++).
 *
 *  TODO: make noexcept movable
 *      . never store non-noexcept movable objects in-place
 *      . then we can just move the storage pointer, and not have to move the
 *        object ever.
 * 
 *
 * TODO: noexcept() assign and so on if we know that
 *   - the type fits in the inline storage
 *   - and the corresponding constructor is NOEXCEPT
 * TODO-FIXME: support for allocators
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FTL_ANY_HPP_E052D231_3F1E_4A48_B3CF_38D846F513E6
#define FLUX_FTL_ANY_HPP_E052D231_3F1E_4A48_B3CF_38D846F513E6

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/identity.hpp>
#include <flux/std/enable_if.hpp>
#include <flux/std/static_assert.hpp>

#include <flux/ftl/hash.hpp>
#include <flux/ftl/meta_set.hpp>
#include <flux/ftl/ptr_and_flag.hpp>

#include <flux/compat/type_traits.hpp>

#include <flux/util/attrib.hpp>

#include <memory>
#include <typeinfo>
#include <typeindex>
#include <functional>
#include <type_traits>

#include <cstddef>

#include "detail/any-meta.hxx"

//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    constants                           ///{{{2///////////////////////////

constexpr std::size_t kAnyDefaultSize = 16;
constexpr std::size_t kAnyDefaultAlign = alignof(void*);

//--    aspects                             ///{{{2///////////////////////////
namespace aspect
{
	/** `AnyT` feature: equality comparable
	 * 
	 * If the \a tFeatures set of `AnyT<>` includes `aspect::AnyFeatEqual`, the
	 * resulting `AnyT` type will support comparing for equality and inequality
	 * using operators `==` and `!=`. Any type stored in such an `AnyT` must
	 * however also support comparison for equality using `operator==`.
	 */
	using AnyFeatEqual = ftl::detail::AnyFeatEqual_;
	/** `AnyT` feature: less-than comparable
	 *
	 * If the \a tFeatures set of `AnyT<>` includes `aspect::AnyFeatLess`, the
	 * resulting `AnyT` type will support comparing objects using `operator <`.
	 * Any type stored in such an `AnyT` must be comparable using `std::less`.
	 *
	 * \note: `AnyFeatLess` should typically be combined with `AnyFeatEqual`.
	 * This is further required if the resulting `AnyT` type is intended to be
	 * used with `std::set` or as a key for `std::map`.
	 */
	using AnyFeatLess = ftl::detail::AnyFeatLess_;
	/** `AnyT` feature: hashable
	 *
	 * If the \a tFeatures set of `AnyT<>` includes `aspect::AnyFeatHash`, the
	 * resulting `AnyT` will be hashable using the `hash()` function.  Any type
	 * stored in such an `AnyT` must be hashable itself, i.e., there must exist
	 * a `hash()` method that can be located using ADL, or must be supported by
	 * `ftl::hash()` (any type that is hashable with `std::hash()` is thus
	 * accepted).
	 *
	 * \note `AnyFeatHash` must be combined with `AnyFeatEqual` for the `AnyT`
	 * to be useable with `std::unordered_set` or `std::unordered_map`.
	 *
	 * \see ftl::hash
	 */
	using AnyFeatHash = ftl::detail::AnyFeatHash_;


	/** `Any`: Default type aspect
	 *
	 * The default aspect provides the following members that define how `Any`
	 * deals with the held objects.
	 *
	 *  - `Features` (a `meta::Set<>` containing zero or more of the feature
	 *    aspects `aspect::AnyFeat*`)
	 *
	 *  - `static void destroy( tType* )` (destruct instance of `tType`)
	 *  - `static void construct( void* aDest, tArgs&&... )` (construct new
	 *    `tType` in memory pointed to by `aDest` from the provided `tArgs`)
	 *
	 *  - `bool equal( tType const&, tType const& )` (only if `Features` 
	 *    contains `aspect::AnyFeatEqual`. Compares two `tType`s for equality.)
	 *  - `bool less( tType const&, tType const& )` (only if `Features` 
	 *    contains `aspect::AnyFeatLess`. Returns true of the first `tType` is
	 *    less than the second.)
	 *  - `ftl::Hash hash( tType const& )` (only if `Features` contains
	 *    `aspect::AnyFeatHash`. Computes hash of `tType`.)
	 */
	template< typename tType >
	struct AnyTypeDefault;

//	template< typename tType > struct AnyTypeRaw; //TODO
}

//--    meta functions                      ///{{{2///////////////////////////

template< typename tType >
struct is_any;

namespace detail
{
	template< typename, std::size_t, std::size_t >
	struct AnyInPlaceEligible;
	
	/* TODO: determine when these can be NOEXCEPT */
}

//--    AnyT<>                              ///{{{2///////////////////////////
/** Hold anything (more or less)
 *
 * `AnyT` holds objects of any type. `AnyT` retains information about the type,
 * making it somewhat safer (but also more expensive) than `void*` pointers.
 *
 * Example:
   \code
   AnyT<> x, y, z;
  
   x = 5;
   y = std::string( "str" );
   z = x;
  
   if( x.is<int>() ) {
       int value = x.get<int>();
       ...
   }
   if( y.is<std::string>() ) {
       std::string value = y.move<std::string>();
       ...
   }
   if( z.is<int>() ) {
	   ...
   }
   \endcode
 * 
 * `AnyT` is strict about types (with few exceptions). Generally, an object
 * stored in an `AnyT` must be retrieved with exactly the same type as it was
 * originall stored. The same set of CV qualifiers must be used. References are
 * stripped, but pointers remain pointers, and array types remain array types:
   \code
   float f[] = { 1.f, 2.f, 3.f };
   Any<> x( f );

   if( x.is<float [3]>() )
   {
	   float f0 = x.get<float [3]>()[0];
	   ...
   }

   float const* fp = x.get<float*>(); // Error: TypeMismatch
   float const* fp = x.get<float const*>(); // Error: TypeMismatch
   \endcode
 *
 * \warning `char`-based arrays and pointers are treated specially.
 *
 * `char`/`char const` arrays are converted to a `char const*` pointer (<b>and
 * only the pointer will be stored</b>). This enables the somewhat transparent
 * use of string literals with `AnyT`: 
   \code
   Any<> a( "string" );
   char const* str = a.get<char const*>(); // OK!
   \endcode
 * Note that `signed char` and `unsigned char` are not affected by this
 * special treatment.
 * 
 * By default, `AnyT` does not define comparison operators. This can be changed
 * via \a tFeatures. \a tFeatures is a `meta::Set` containing one or more of
 * the following:
 *  - `aspect::AnyFeatEqual` - support comparison via member `equals()`
 *  - `aspect::AnyFeatLess` - support comparison via member `less()`
 *  - `aspect::AnyFeatHash` - support hashing via `ftl::any_value_hash` and
 *    `ftl::hash()`.
 * Note that enabling these features puts additional restricitions on the types
 * that such an `AnyT` can hold -- specifically, the types must support the
 * selected operations.
 */
template< class tFeatures = ftl::meta::make_set<>, std::size_t tInPlaceSize = kAnyDefaultSize >
class AnyT final
	: public detail::AnyExtras< AnyT, tFeatures, tInPlaceSize >
{
	public:
		template< typename tType > struct IsInPlace 
			: std::integral_constant<bool, 
				detail::AnyInPlaceEligible<tType,tInPlaceSize,kAnyDefaultAlign>::value
			>
		{};
	
	public:
		AnyT() noexcept;

		AnyT( AnyT&& ) noexcept;
		AnyT& operator= (AnyT&&) noexcept;

		AnyT( AnyT const& );
		AnyT& operator= (AnyT const&);

		template< 
			typename tType,
			typename = FLUX_DISABLE_IF(is_any<compat::decay_t<tType>>)(int)
		>
		explicit AnyT( tType&& );

		template< 
			typename tType,
			typename = FLUX_DISABLE_IF(is_any<compat::decay_t<tType>>)(int)
		> 
		AnyT& operator= ( tType&& );

		template< 
			typename tType, 
			class tAspect,
			typename = FLUX_DISABLE_IF(is_any<compat::decay_t<tType>>)(int)
		>
		explicit AnyT( tType&&, Identity<tAspect> );

		template< class tOtherFeat, size_t tOtherSize >
		AnyT( AnyT<tOtherFeat,tOtherSize>&& );
		template< class tOtherFeat, size_t tOtherSize > inline
		AnyT& operator= ( AnyT<tOtherFeat,tOtherSize>&& );

		template< class tOtherFeat, std::size_t tOtherSize >
		AnyT( AnyT<tOtherFeat,tOtherSize> const& );
		template< class tOtherFeat, size_t tOtherSize > inline
		AnyT& operator= ( AnyT<tOtherFeat,tOtherSize> const& );

		/** Constructor: construct object in-place
		 *
		 * Constructs the `AnyT` holding an object of type \a tType. The held
		 * object is constructed in-place, meaning that no copies or moves of
		 * \a tType take place. The optional arguments \a tArgs are forwarded
		 * to the constructor of \a tType:
		   \code
		   AnyT<> x( Identity<std::string>{}, "Hello" );
		   FLUX_ASSERT( x.is<std::string>() );
		   \endcode
		 */
		template< typename tType, typename... tArgs >
		explicit AnyT( Identity<tType>, tArgs&&... );

		~AnyT();

	public:
		/** Clear object
		 *
		 * Destroys held object and resets `AnyT` to an empty state. Calling
		 * `clear()` on an empty `AnyT` has no effect.
		 */
		void clear();

		/** Check if `AnyT` is empty
		 *
		 * Returns true if the `AnyT` is empty, and false otherwise.
		 */
		bool empty() const noexcept;
		/** Return size of held object
		 *
		 * Returns size of held object, as if `sizeof()` had been called on the
		 * original object's type. If the `AnyT` is empty, `size()` returns
		 * zero.
		 */
		size_t size() const noexcept;

		/** Query contained type
		 *
		 * Returns `std::type_info` for the contained type, as if `typeid()`
		 * had be called on the original object's type. If the `AnyT` is empty,
		 * `type()` returns `typeid(void)`.
		 */
		std::type_info const& type_info() const noexcept;

		/** Query `std::type_index` of contained type.
		 *
		 * Returns `std::type_index` for the contained type, as if by
		 * `std::type_index(typeid(x))`. If the `AnyT` is empty, the returned
		 * `std::type_index` is for `void`.
		 */
		std::type_index type_index() const noexcept;

		/** Check type of held object
		 *
		 * Returns true if the currently held object has type \a tType, and
		 * false otherwise. Empty `AnyT`s always return false.
		 *
		 * Example:
		   \code
		   AnyT<> x( 55 );
		   FLUX_ASSERT( x.is<int>() );
		   FLUX_ASSERT( !x.is<unsigned>() );
		   \endcode
		 * 
		 * \note The type must match exactly, including potential cv
		 * qualifiers. See `AnyT` class documentation for special exceptions.
		 */
		template< typename tType >
		bool is() const noexcept;

		/** Get held object
		 *
		 * Gets held object of type \a tType. Returns a (plain, const or
		 * rvalue) reference to the held object. If the held object does not
		 * have type \a tType, a `ftl::error::AnyTypeMismatch` exception is
		 * thrown.
		 *
		 * Example:
		   \code
		   AnyT<> x( 55 );
		   int i = x->get<int>(); // OK
		   float f = x->get<float>(); // Error: throws error::TypeMismatch
		   \endcode
		 */
		template< typename tType >
		tType& get() &;

		/** \copydoc get() */
		template< typename tType >
		tType&& get() &&;

		/** \copydoc get() */
		template< typename tType >
		tType const& get() const&;

		/** Move held object
		 *
		 * Returns a r-value reference to the held object of type \a tType. If
		 * the held object does not have type \a tType, an exception of type
		 * `ftl::error::AnyTypeMismatch` is thrown.
		 *
		 * `move()` can be used to achieve an similar effect as if `std::move()`
		 * were applied to the held object directly. Example:
		   \code
		   std::vector<int> v{ 1, 2, 3 };
		   AnyT<> x( std::move(v) );
		   ...
		   std::vector<int> v2( x.move<std::vector<int>>() );
		   \endcode
		 * At no point in the above example should a copy of the vector's 
		 * contents be necessary.
		 *
		 * \warning A moved-from `Any` retains its previous type until cleared,
		 * or re-assigned. So, `x` in the above example would retain type
		 * `std::vector<int>()`, and a call to `x->is<std::vector<int>>()`
		 * would return true.
		 */
		template< typename tType > 
		tType&& move();

		/** Get held object (unsafe)
		 *
		 * Gets the held object as if it were an object of type \a tType. If
		 * the held object has a type different from \a tType, behaviour is
		 * undefined (mayhem, death & destruction ahoy!). 
		 *
		 * This is mainly intended to be used in conjunction with `is()`, for
		 * example:
		   \code
		   AnyT<> x( 55 );
		   if( x.is<int>() ) {
		       int i = x.get_unsafe<int>();
		       ...
		   }
		   \endcode
		 *
		 * \see get
		 */
		template< typename tType >
		tType& get_unsafe() & noexcept;
		/** \copydoc get_unsafe() */
		template< typename tType > 
		tType&& get_unsafe() && noexcept;
		/** \copydoc get_unsafe() */
		template< typename tType > 
		tType const& get_unsafe() const& noexcept;

		/** Move held object (unsafe)
		 *
		 * Moves held object as if it were a held object of type \a tType. If
		 * the held object has a type different from \a tType, behaviour is
		 * undefined (mayhem, death & destruction ahoy!). 
		 *
		 * This is mainly useful in conjunction with `is()`:
		   \code
		   std::vector<int> v{ 1, 2, 3 };
		   AnyT<> x( v );
		   if( x.is<std::vector<int>>() ) {
		       std::vector<int> v2 = x.move_unsafe<std:vector<int>>();
		       ...
		   }
		   \endcode
		 *
		 * \see move
		 */
		template< typename tType >
		tType&& move_unsafe() noexcept;

		/** Assign new value to `AnyT`
		 */
		template< typename tType >
		void assign( tType&& );
		/** Assign new value to `AnyT` (with custom aspect)
		 */
		template< typename tType, class tAspect >
		void assign_with_aspect( tType&&, Identity<tAspect> );

		void copy( AnyT&& ) noexcept;
		void copy( AnyT const& );

		template< class tOtherFeat, std::size_t tOtherSize > 
		void copy( AnyT<tOtherFeat,tOtherSize>&& );
		template< class tOtherFeat, std::size_t tOtherSize >
		void copy( AnyT<tOtherFeat,tOtherSize> const& );

		/** Construct object in-place
		 *
		 * Constructs an object of type \a tType in place. The optional
		 * arguments \a tArgs are forwarded to the constructor of \a tType.
		 * This does not incur any copies or moves.
		 */
		template< typename tType, typename... tArgs > 
		void emplace( tArgs&&... );

	private:
		detail::AnyMeta const* meta_() const noexcept;

		void* ptr_() noexcept;
		void const* ptr_() const noexcept;

		template< typename tType > 
		bool check_type_soft_() const noexcept;
		template< typename tType > 
		void check_type_hard_() const;

		template< typename tType >
		tType* ptr_typed_() noexcept;
		template< typename tType >
		tType const* ptr_typed_() const noexcept;

		char const* held_type_name_() const noexcept;

		ftl::Hash hash_() const;

	private:
		using Buffer_ = typename std::aligned_storage< 
			tInPlaceSize, kAnyDefaultAlign 
		>::type;
	
		ftl::PtrAndFlag<detail::AnyMeta const*> mMeta;
		Buffer_ mStore;
	
		FLUX_STATIC_ASSERT( tInPlaceSize >= sizeof(void*) );

	private:
		template< class tFt, std::size_t tSz > friend class AnyT;
		template< class tFt, std::size_t tSz > friend
		FLUX_ENABLE_IF( meta::set_contains_element<tFt, aspect::AnyFeatHash> )
		(ftl::Hash) any_value_hash( AnyT<tFt,tSz> const& );

		template< class, class, template<class,std::size_t> class > friend
		struct detail::AnyInjectExtra;
};

//--    Any                                 ///{{{2///////////////////////////

/** Default `Any`
 *
 * The default `Any` type. See `%AnyT`.
 */
using Any = AnyT<>;

//--    operators                           ///{{{2///////////////////////////
namespace detail
{
	template< class tSetA, class tSetB >
	struct AnyBothHaveEqual_
		: meta::All< 
			meta::set_contains_element< tSetA, aspect::AnyFeatEqual >,
			meta::set_contains_element< tSetB, aspect::AnyFeatEqual >
		>
	{};
}

	// equality AnyT
template< class tLFeat, std::size_t tLSize, class tRFeat, std::size_t tRSize >
FLUX_ENABLE_IF( detail::AnyBothHaveEqual_< tLFeat, tRFeat > )
(bool) operator== (AnyT<tLFeat,tLSize> const&, AnyT<tRFeat,tRSize> const&);
template< class tLFeat, std::size_t tLSize, class tRFeat, std::size_t tRSize >
FLUX_ENABLE_IF( detail::AnyBothHaveEqual_< tLFeat, tRFeat > )
(bool) operator!= (AnyT<tLFeat,tLSize> const&, AnyT<tRFeat,tRSize> const&);

//TODO: operators with AnyT to AnyT

	// equality with tType
template< typename tType, class tFeat, std::size_t tSize > 
FLUX_ENABLE_IF( meta::set_contains_element<tFeat, aspect::AnyFeatEqual> )
(bool) operator== (tType const&, AnyT<tFeat,tSize> const&);
template<class tFeat, std::size_t tSize, typename tType > 
FLUX_ENABLE_IF( meta::set_contains_element<tFeat, aspect::AnyFeatEqual> )
(bool) operator== (AnyT<tFeat,tSize> const&, tType const&);

template< typename tType, class tFeat, std::size_t tSize > 
FLUX_ENABLE_IF( meta::set_contains_element<tFeat, aspect::AnyFeatEqual> )
(bool) operator!= (tType const&, AnyT<tFeat,tSize> const&);
template< class tFeat, std::size_t tSize, typename tType > 
FLUX_ENABLE_IF( meta::set_contains_element<tFeat, aspect::AnyFeatEqual> )
(bool) operator!= (AnyT<tFeat,tSize> const&, tType const&);

	// ordering with tType
template< class tFeat, std::size_t tSize, typename tType > 
FLUX_ENABLE_IF( meta::set_contains_element<tFeat, aspect::AnyFeatLess> )
(bool) operator< (AnyT<tFeat,tSize> const&, tType const&);
template< typename tType, class tFeat, std::size_t tSize > 
FLUX_ENABLE_IF( meta::set_contains_element<tFeat, aspect::AnyFeatLess> )
(bool) operator<= (tType const&, AnyT<tFeat,tSize> const&);

//TODO: other relational operators (note: these require both FeatLess and FeatEqual

//--    functions                           ///{{{2///////////////////////////

/** \name any_cast<>
 * \brief Access values held by `AnyT<>`
 *
 * Accessors to values held by `AnyT<>`, inspired by the C++14 proposal for
 * `std::experimental::%any`.
 *
 * \see AnyT::get()
 */
//@{
template< typename tType, class tFeat, std::size_t tSize >
tType any_cast( AnyT<tFeat,tSize>& );
template< typename tType, class tFeat, std::size_t tSize >
tType any_cast( AnyT<tFeat,tSize>&& );
template< typename tType, class tFeat, std::size_t tSize >
tType any_cast( AnyT<tFeat,tSize> const& );

template< typename tType, class tFeat, std::size_t tSize >
tType* any_cast( AnyT<tFeat,tSize>* ) noexcept;
template< typename tType, class tFeat, std::size_t tSize >
tType const* any_cast( AnyT<tFeat,tSize> const* ) noexcept;
//@}

/** Computes hash of object held by `AnyT`
 *
 * Computes the hash of the object held by \a AnyT. The `AnyT` must support
 * `aspect::AnyFeatHash`. Returns zero if the `AnyT` is empty.
 *
 * \note `any_value_hash()` only considers the contents of the \a AnyT. The
 * type of the held object is ignored. It is thus likely unsuitable for use
 * as a key into containers and similar.
 * 
 * \see hash()
 */
template< class tFeat, std::size_t tSize >
FLUX_ENABLE_IF( meta::set_contains_element<tFeat, aspect::AnyFeatHash> )
(ftl::Hash) any_value_hash( AnyT<tFeat,tSize> const& );

/** Computes hash of `AnyT` 
 *
 * Computes a hash from an `AnyT`. The hash considers both the type of the
 * held object, and the held object's contents (if any). 
 *
 * \see any_value_hash()
 */
template< class tFeat, std::size_t tSize >
FLUX_ENABLE_IF( meta::set_contains_element<tFeat, aspect::AnyFeatHash> )
(ftl::Hash) hash( AnyT<tFeat,tSize> const& );


namespace detail
{
	FLUX_ATTR_NORETURN void throw_any_type_incompat( unsigned, AnyMeta const* );
	FLUX_ATTR_NORETURN void throw_any_type_mismatch( char const*, char const* );
}

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--    >>> namespace = std >>>                     ///{{{1///////////////////
namespace std {
//--    hash<>                              ///{{{2///////////////////////////
/* We are permitted to specialize std::hash<> for custom types, such as AnyT.
 * TODO-FIXME: enable this specialization only when AnyFeatHash is present 
 * in the tFeat feature set.
 */
template< class tFeat, std::size_t tSize >
struct hash< flux::ftl::AnyT<tFeat,tSize> >
{
	using argument_type = flux::ftl::AnyT<tFeat,tSize>;
	using result_type = std::size_t;

	result_type operator() (argument_type const& aAny) const {
		return flux::ftl::hash( aAny );
	}
};

//--    <<< ~ std namespace <<<                     ///{{{1///////////////////
} // ~ std
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/any.inl"
#endif // FLUX_FTL_ANY_HPP_E052D231_3F1E_4A48_B3CF_38D846F513E6
