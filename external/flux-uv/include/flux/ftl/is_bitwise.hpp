/*-******************************************************* -- HEADER -{{{1- */
/*-	Meta function: is_bitwise_*<>
 * 
 * Related discussion:
 *  - http://stackoverflow.com/a/36164788
 *
 * XXX: the _t versions should really be _v versions??
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_FTL_IS_BITWISE_HPP_B2BC88FE_5B4E_4D91_A5EA_797F62E5DB64
#define FLUX_FTL_IS_BITWISE_HPP_B2BC88FE_5B4E_4D91_A5EA_797F62E5DB64

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    is_bitwise_*<>                      ///{{{2///////////////////////////

/** \brief Trait: is bitwise conjuration permissible?
 *
 * `is_bitwise_conjurable<>` determines if it permissible to conjure an object
 * into existence without going through the usual incantations (that is, by
 * constructing it via constructors). An object is bitwise conjurable if zeroed
 * out memory produces a valid object on which the object's destructor may be
 * called.
 *
 * The default value is derieved from
 * `std::is_trivially_default_constructible<>`.
 *
 * The default value may be overridden by declaring a function `TrueOrFalse
 * tag_is_bitwise_conjurable(type)`, where `TrueOrFalse` is either
 * `std::true_type` or `std::false_type`. If such a function is found via an
 * ADL lookup, `is_bitwise_conjurable<>` will evaluate to true or false,
 * respectively.
 *
 * \see is_bitwise_materializable, is_bitwise_awakeable
 */
template< typename tType >
struct is_bitwise_conjurable;

#if 0
/** \brief Trait: is bitwise materializable?
 *
 * `is_bitwise_materializable<>` determines if it is permissible to materialize
 * an object by copying another object's bits into uninitialized memory (that
 * is, an object can be constructed by `std::memcpy()` from another (valid)
 * object of the same type).
 *
 * The default value is derived from the combination of
 *   - `std::is_trivially_constructibe<T,T const&>`
 *
 * The default value can be overriden by declaring a free function
 * `std::true_type tag_is_bitwise_materializable( T const& )`. If such a
 * function is found via an ADL lookup, `is_bitwise_materializable<>` will
 * evaluate to true.
 *
 * \note This probably ends up in IB/UD land on occasion. Note especially that
 * zero-initialized memory *must* produce a valid object on which a destructor
 * can be called.
 *
 * \see is_bitwise_conjurable, is_bitwise_materializable
 */
template< typename tType >
struct is_bitwise_materializable;
#endif

/** \brief Trait: is bitwise awakening permissible?

 * `is_bitwise_awakeable<>` determines if it is permissible to "awaken" an
 * object into existence in pre-initialized memory without the usual
 * incantations (i.e., without invoking the constructor). Unlike
 * `is_bitwise_materializable`, the memory may be initialized to arbitrary bit
 * patterns, but it must nevertheless be permissible to call the destructor on
 * such an awakened objects. The default value is derived from 
 * `std::is_trivially_destructible`.
 * 
 * Example:
   \code
   AlignedUninitalizedStorageForX a; // = random bits
   std::launder(reinterpret_cast<X*>(&a))->~X();
   \endcode
 *
 * The default value may be overridden by declaring a function `TrueOrFalse
 * tag_is_bitwise_awakeable(type)`, where `TrueOrFalse` is either
 * `std::true_type` or `std::false_type`. If such a function is found via an
 * ADL lookup, `is_bitwise_awakeable<>` will evaluate to true or false,
 * respectively.
 *
 * \note This probably ends up in IB/UD land on occasion. Note especially that
 * any bit pattern *must* produce a valid object on which a destructor can be
 * called.
 *
 * \see is_bitwise_conjurable, is_bitwise_materializable
 */
template< typename tType > 
struct is_bitwise_awakeable;

/** \brief Trait: is bitwise duplication permissible?
 * 
 * `is_bitwise_duplicable<>` determines if it is permissible to create a
 * bitwise copy of \a typename, e.g. via `std::memcpy()`, into uninitialized
 * memory. It derives it's default value from `std::is_trivially_copyable`.
 * Note that both the original and the new copies must be destroyed as
 * appropriate (i.e., by calling their destructor, unless this isn't required,
 * see `is_bitwise_dismissible`).
 *
 * The default value may be overridden by declaring a function `TrueOrFalse
 * tag_is_bitwise_duplicable(type)`, where `TrueOrFalse` is either
 * `std::true_type` or `std::false_type`. If such a function is found via an
 * ADL lookup, `is_bitwise_duplicable<>` will evaluate to true or false,
 * respectively.
 */
template< typename tType > 
struct is_bitwise_duplicable;

/** \brief Trait: is bitwise relocation permissible?
 * 
 * `is_bitwise_relocatable<>` determines if it is permissible to relocate an
 * existing object by creating a bitwise copy (`std::memcpy()` and similar) and
 * dropping the old object on the floor. There is no standard equivalent, and
 * this certainly leads to IB and UD, but one would imagine that most types
 * (e.g., `std::unique_ptr<>`) actually permit this in practice. The default
 * value is true if both `is_bitwise_duplicable<>` and
 * `is_bitwise_dismissible<>` are true.
 *
 * Example:
   \code
   AlignedUninitalizedStorageForX a, b;
   ...
   new (&a) X();
   ...
   std::memcpy( &b, &a, sizeof(X) );
   ...
   std::launder(reinterpret_cast<X*>(&b))->~X();
   \endcode
 *
 * The default value may be overridden by declaring a function `TrueOrFalse
 * tag_is_bitwise_relocatable(type)`, where `TrueOrFalse` is either
 * `std::true_type` or `std::false_type`. If such a function is found via an
 * ADL lookup, `is_bitwise_relocatable<>` will evaluate to true or false,
 * respectively.
 *
 * \note In particular, bitwise relocatable objects may be moved by 
 * `std::realloc()`.
 */
template< typename tType >
struct is_bitwise_relocatable;

/** \brief Trait: is bitwise dismissal permissible?
 *   
 * `is_bitwise_dismissible<>` determines if it is permissible to dismiss an
 * object into non-existence without the usual incantations (calling its
 * destructor). This derives its default value from
 * `std::is_trivially_destructible<>`.
 *
 * The default value may be overridden by declaring a function `TrueOrFalse
 * tag_is_bitwise_dismissible(type)`, where `TrueOrFalse` is either
 * `std::true_type` or `std::false_type`. If such a function is found via an
 * ADL lookup, `is_bitwise_dismissible<>` will evaluate to true or false,
 * respectively.
 *
 * \note This probably ends up in IB/UD land on occasion. An object's lifetime
 * is determined by construction and destruction; not destroying objects may
 * confuse compilers (rightly so), so careful testing is advised.
 */
template< typename tType >
struct is_bitwise_dismissible;


//--    documentation                       ///{{{2///////////////////////////

/** \defgroup FTLBitwiseTags Meta: `is_bitwise_*` tagging
 * \brief Manual overrides for `is_bitwise_*` traits
 *
 * The `is_bitwise_*` traits in `flux::ftl` can be overridden for selected
 * types by defining functions that can be found through ADL for the specified
 * type. The return type of such a function determines the value of the
 * corresponding property.
 *
 * The functions are named `tag_<trait`. For example, the value of
 * `is_bitwise_relocatable<>` may be overridden by defining the function
   \code
   std::true_type tag_is_bitwise_relocatable( MyType const& ) { return {}; }
   //OR: std::false_type tag_is_bitwise_relocatable( MyType const& ) { return {}; }
   \endcode
 * 
 * \note Defining `FLUXCFG_IS_BITWISE` to either `FLUXCFG_IS_BITWISE_TRIVIAL`
 * or `FLUXCFG_IS_BITWISE_NEVER` causes the manual overrides to be ignored. The
 * default is `FLUXCFG_IS_BITWISE_USER` where the manual overrides are active.
 */

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/is_bitwise.inl"
#endif // FLUX_FTL_IS_BITWISE_HPP_B2BC88FE_5B4E_4D91_A5EA_797F62E5DB64
