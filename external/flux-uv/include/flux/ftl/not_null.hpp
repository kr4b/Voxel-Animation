/*-******************************************************* -- HEADER -{{{1- */
/*-	not_null<>
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_NOT_NULL_HPP_317846AB_5295_445C_9B71_0F4D7352CDF6
#define FLUX_STD_NOT_NULL_HPP_317846AB_5295_445C_9B71_0F4D7352CDF6

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/enable_if.hpp>
#include <flux/std/stdexcept.hpp>

#include <flux/ftl/meta_bool.hpp>
#include <flux/ftl/is_bitwise.hpp>

#include <flux/util/attrib.hpp>
#include <flux/compat/type_traits.hpp>

//#include <iosfwd>
#include <type_traits>

#include <cstddef>

//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl);
//--    forward                     ///{{{1///////////////////////////////////
namespace aspect
{
	/** `not_null` aspect: Unsafe
	 *
	 * Unsafe policy for `not_null<>`. With the unsafe policy, checks may be
	 * completely elided from `not_null<>` (although if `FLUX_DEBUG` is 
	 * enabled, assertions will still be included).
	 *
	 * Also see `not_null_unsafe<>`, which reduces to a raw unchecked pointer
	 * in release mode.
	 * 
	 * \see not_null_unsafe
	 */
	struct NotNullUnsafe;

	/** `not_null` aspect: throw exceptions
	 *
	 * Policy for `not_null<>` where constructing a `not_null` pointer with a
	 * runtime-null causes an exception of type `error::NullError` to be 
	 * thrown.
	 */
	struct NotNullExcept;
	/** `not_null` aspect: assert and/or throw exceptions
	 *
	 * Policy for `not_null<>` where constructing a `not_null` pointer with a
	 * runtime-null causes an assertion fault in debug mode (`FLUX_DEBUG`) and
	 * an exception of type `error::NullError` to be thrown in release mode.
	 *
	 * This is the default aspect.
	 */
	struct NotNullExceptAssert;
}

template< typename, class = aspect::NotNullExceptAssert > 
class not_null;

namespace detail
{
	template< bool > struct OptCheckHelper;
	template< bool > struct UncheckedHelper;
}

//--    meta-functions              ///{{{1///////////////////////////////////

/** Meta function: is instance of `not_null<>`?
 *
 * Boolean meta function: checks if \a typename is an instance of `not_null<>`.
 */
template< typename > struct is_not_null;
/** Meta function: unwrap `not_null<>`
 *
 * Meta function that unwraps \a typename if it is an instance of `not_null<>`,
 * and evaluates to the wrapped (unchecked) pointer type.
 */
template< typename > struct remove_not_null;

//--    not_null<>                  ///{{{1///////////////////////////////////

/** Pointer wrapper: non-null pointer
 *
 * The `not_null<>` pointer wrapper annotates a pointer that may not be null.
 * It attempts to check for this statically where possible, and may perform
 * run-time checks otherwise (and where necessary).
 *
 * `not_null<>` is intended to be mainly used in interfaces (e.g., as an
 * annotation for function parameters). The goal is to move errors closer to
 * the site where they occur (i.e., passing a null value to a function not
 * expecting such will error at the function call site). `not_null<>` can help
 * avoid repeated checks -- passing a `not_null<>` to a different `not_null<>`
 * will elide checks when possible/desirable (release mode).
 *
 * This `not_null<>` implementation is inspired and based on the implementation
 * in Principia:
 * > https://github.com/mockingbirdnest/Principia/blob/master/base/not_null.hpp
 * 
 * \warning There are a few key decisions that differ from other
 * implementations (e.g., GSL), mainly `not_null<>` *may* be moved from (and as
 * such, it supports `unique_ptr<>`). A moved-from `not_null<>` can end up
 * being null; like other moved-from objects, it is the programmers
 * responsibility to avoid accesses there. In debug mode (`FLUX_DEBUG`)
 * additional checks are introduced to catch this situation.
 * 
 * Example:
   \code
   not_null<int*> p = ...;
   not_null<std::unique_ptr<int>> q = ...;
  
   void f( not_null<float*> aArray ) { ... }

   ...

   std::vector<float> buffer; 
   ...
   f( buffer.data() );
   \endcode
 *
 * Use `make_not_null<>`when the implicit conversions fail (e.g., for templated
 * functions).
 *
 * \see make_not_null, not_null_unsafe
 */
template< typename tType, class tAspect >
class not_null
{
	public:
		using pointer = typename remove_not_null<tType>::type;
		using element_type = typename std::pointer_traits<tType>::element_type;

		template< typename, class >
		friend class not_null;

	public:
		not_null( not_null const& );

		// copy constructors
		template< 
			typename tOtherType,
			class tOtherAspect,
			typename = FLUX_ENABLE_IF( 
				std::is_convertible<tOtherType,pointer> 
			)(int) 
		> 
		not_null( not_null<tOtherType,tOtherAspect> const& );

		// move constructors
		not_null( not_null&& );

		template< 
			typename tOtherType,
			class tOtherAspect,
			typename = FLUX_ENABLE_IF( 
				std::is_convertible<tOtherType,pointer> 
			)(int) 
		> 
		not_null( not_null<tOtherType,tOtherAspect>&& );

		// initialize from raw pointer
		template<
			typename tPointer,
			typename = FLUX_ENABLE_IF( meta::All<
				std::is_convertible<tPointer,pointer>,
				meta::Not< is_not_null<tPointer> >
			> )(int)
		> 
		not_null( tPointer&& );

		// for static_cast<> etc (requires decltype)
		template< 
			typename tOtherType,
			class tOtherAspect,
			typename = FLUX_DISABLE_IF( 
				std::is_convertible<tOtherType,pointer> 
			)(int),
			typename = decltype(static_cast<pointer>(std::declval<tOtherType>()))
		> 
		explicit not_null( not_null<tOtherType,tOtherAspect> const& );
		template< 
			typename tOtherType,
			class tOtherAspect,
			typename = FLUX_DISABLE_IF( 
				std::is_convertible<tOtherType,pointer> 
			)(int),
			typename = decltype(static_cast<pointer>(std::declval<tOtherType>()))
		> 
		explicit not_null( not_null<tOtherType,tOtherAspect>&& );

		// No default constructor from nullptr
		not_null() = delete;
		not_null( std::nullptr_t ) = delete;

	public:
		// copy assignment
		not_null& operator= (not_null const&);

		template< 
			typename tOtherType, 
			class tOtherAspect,
			typename = FLUX_ENABLE_IF( 
				std::is_convertible<tOtherType,pointer> 
			)(int) 
		> 
		not_null& operator= (not_null<tOtherType,tOtherAspect> const&);

		// move assignment
		not_null& operator= (not_null&&);

		template< 
			typename tOtherType, 
			class tOtherAspect,
			typename = FLUX_ENABLE_IF( 
				std::is_convertible<tOtherType,pointer> 
			)(int) 
		> 
		not_null& operator= (not_null<tOtherType,tOtherAspect>&&);

		// implicit conversion to pointer type
		operator pointer&&() &&;
		operator pointer const&&() const&;

		template<
			typename tOtherPtr,
			typename = FLUX_ENABLE_IF( meta::All<
				std::is_convertible<pointer,tOtherPtr>,
				meta::Not< std::is_same<tOtherPtr, pointer> >,
				meta::Not< is_not_null<tOtherPtr> >
			> )(int)
		> 
		operator tOtherPtr() &&;

		template<
			typename tOtherPtr,
			typename = FLUX_ENABLE_IF( meta::All<
				std::is_convertible<pointer,tOtherPtr>,
				meta::Not< std::is_same<tOtherPtr, pointer> >,
				meta::Not< is_not_null<tOtherPtr> >
			> )(int)
		> 
		operator tOtherPtr() const&;

		compat::add_pointer_t<element_type> operator->() const;
		compat::add_lvalue_reference_t<element_type> operator*() const;
		
		// special comparisons
		explicit operator bool() const;

		bool operator== (std::nullptr_t) const;
		bool operator!= (std::nullptr_t) const;

		bool operator== (pointer const&) const;
		bool operator== (not_null const&) const;
		bool operator!= (pointer const&) const;
		bool operator!= (not_null const&) const;
		
		bool operator< (not_null const&) const;
		bool operator<= (not_null const&) const;
		bool operator> (not_null const&) const;
		bool operator>= (not_null const&) const;

		// no assignment from nullptr
		not_null& operator= (std::nullptr_t) = delete;
		
	public:
		template< 
			typename tPtr = pointer, 
			typename = decltype(std::declval<tPtr>().get()) 
		> 
		not_null<decltype(std::declval<tPtr>().get()),tAspect> get() const;

		template<
			typename tNewPtr,
			typename tPtr = pointer,
			typename = decltype(std::declval<tPtr>().reset())
		> 
		void reset( tNewPtr&& );
		
	private:
		struct Unchecked_ {};
		not_null( pointer, Unchecked_ const& );

		template< bool > friend
		struct detail::UncheckedHelper;

		template< typename tNewPtr >
		void reset_checked_( tNewPtr&& );
		template< typename tNewPtr >
		void reset_unchecked_( tNewPtr&& );

	private:
		pointer mPointer;
};

/* Prohibited instantiations */
// Use `not_null<tPointer,tAspect> const` instead. NOTE: this is not the same as
// e.g., `not_null<tPointer const*,tAspect>`! 
template< typename tPointer, class tAspect >
class not_null<tPointer const, tAspect>;

// Use `not_null<tPointer,tAspect>&` instead.
template< typename tPointer, class tAspect >
class not_null<tPointer&, tAspect>;

// Use `not_null<tPointer,tAspect>&&` instead.
template< typename tPointer, class tAspect >
class not_null<tPointer&&, tAspect>;

/* Define is_bitwise_* traits for not_null:
 *  - conjurable/awakeable: no. could lead to NULL not_null<>s
 *  - duplicable/relocatable: depends on underlying type
 *  - dismissable: depends on underlying type
 *
 * Note: these are left undefined, they should never actually be called.
 */
template< typename tPointer, class tAspect >
std::false_type tag_is_bitwise_conjurable( not_null<tPointer,tAspect> const& );
template< typename tPointer, class tAspect >
std::false_type tag_is_bitwise_awakeable( not_null<tPointer,tAspect> const& );

template< typename tPointer, class tAspect >
auto tag_is_bitwise_duplicable( not_null<tPointer,tAspect> const& )
	-> typename is_bitwise_duplicable<tPointer>::type;
template< typename tPointer, class tAspect >
auto tag_is_bitwise_relocatable( not_null<tPointer,tAspect> const& )
	-> typename is_bitwise_relocatable<tPointer>::type;

template< typename tPointer, class tAspect >
auto tag_is_bitwise_dismissible( not_null<tPointer,tAspect> const& )
	-> typename is_bitwise_dismissible<tPointer>::type;

//--    not_null_unsafe<>           ///{{{1///////////////////////////////////

/** Pointer wrapper that indicates that pointers may be assumed to not be null
 *
 * Unlike `not_null<>`, `not_null_unsafe<>` only verifies the pointer values in
 * debug mode (`FLUX_DEBUG`). Outside of debug mode, the `not_null_unsafe<T>`
 * becomes a straight alias of `T`.
 *
 * Normally, `not_null<>` should be preferred over `not_null_unsafe<>`.
 * However, in some cases (hot code paths), the additional branch required by
 * `not_null<>` may be undesirable. In this case `not_null_unsafe<>` may be
 * appropriate.
 * 
 * \see not_null, FLUX_DEBUG
 */
#if FLUX_DEBUG
template< typename tType >
using not_null_unsafe = not_null< tType, aspect::NotNullUnsafe >;
#	else // !DEBUG
template< typename tType >
using not_null_unsafe = tType;
#endif // ~ DEBUG

//--    exceptions                  ///{{{1///////////////////////////////////
namespace error
{
	struct NullError : flux::error::LogicError
	{};
}

//--    functions                   ///{{{1///////////////////////////////////

/** Factory method for `not_null<>`
 *
 * Factory method for `not_null<>` that may be used when a pointer cannot be
 * converted to `not_null<>` implicitly (e.g., when calling a function whose
 * pointer type has been templated).
 */
template< typename tPointer >
not_null<tPointer> make_not_null( tPointer&& );
/** Factory method for `not_null_unsafe<>`
 *
 * Factory method for `not_null_unsafe<>` that may be used when a pointer 
 * cannot be converted to `not_null_unsafe<>` implicitly (e.g., when calling a
 * function whose pointer type has been templated).
 */
template< typename tPointer >
not_null_unsafe<tPointer> make_not_null_unsafe( tPointer&& );


//TODO: make_not_null_unqiue_ptr()
//TODO: make_not_null_unsafe_unqiue_ptr()

namespace detail
{
	//void null_handler_break();
	FLUX_ATTR_NORETURN void null_handler_throw();
	FLUX_ATTR_NORETURN void null_handler_assert();
}

//--    operators                   ///{{{1///////////////////////////////////

template< typename tPointer, class tAspect >
bool operator== (std::nullptr_t, not_null<tPointer, tAspect> const& );
template< typename tPointer, class tAspect >
bool operator!= (std::nullptr_t, not_null<tPointer, tAspect> const& );

template< typename tPointer, class tAspect >
bool operator== (tPointer const&, not_null<tPointer, tAspect> const&);
template< typename tPointer, class tAspect >
bool operator!= (tPointer const&, not_null<tPointer, tAspect> const&);


//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl);
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/not_null.inl"
#endif // FLUX_STD_NOT_NULL_HPP_317846AB_5295_445C_9B71_0F4D7352CDF6
