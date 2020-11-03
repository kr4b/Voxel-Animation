/*-******************************************************* -- HEADER -{{{1- */
/*-	Required language features
 *
 * Formerly optional, but now required, language features. This file is mainly
 * kept for reference and for diagnostic purposes (self-tests, when porting to
 * new compilers).
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SETUP_EXT_LANGFEAT_REQUIRED_HPP_B1CFCE99_F49F_4A7B_8CD4_7B1147A664F9
#define FLUX_SETUP_EXT_LANGFEAT_REQUIRED_HPP_B1CFCE99_F49F_4A7B_8CD4_7B1147A664F9

//--//////////////////////////////////////////////////////////////////////////
//--    Include                     ///{{{1///////////////////////////////////

#include "../defaults.hpp"
#include "../compiler.hpp"

//--    C++11 features              ///{{{1///////////////////////////////////

/** Support for `decltype`
 */
#define FLUX_LANGFEAT_DECLTYPE            0 /*OVERRIDDEN_ELSEWHERE*/
/** Support for `auto` as a type specifier
 */
#define FLUX_LANGFEAT_AUTO_TYPE           0 /*OVERRIDDEN_ELSEWHERE*/
/** Support for range-based `for`
 *
 * Range-based `for`:
   \code
   int array[] = { 1, 2, 3 };
   for( int i : array ) { printf( "%d\n", i ); }
   \endcode
 */
#define FLUX_LANGFEAT_RANGE_FOR           0 /*OVERRIDDEN_ELSEWHERE*/
/** Support for `static_assert()`
 */
#define FLUX_LANGFEAT_STATIC_ASSERT       0 /*OVERRIDDEN_ELSEWHERE*/
/** Support for C++11 initializer lists
 */
#define FLUX_LANGFEAT_INITIALIZER_LIST    0 /*OVERRIDDEN_ELSEWHERE*/
/** Support for r-value references (&&)
 */
#define FLUX_LANGFEAT_RVALUE_REFERENCE    0 /*OVERRIDDEN_ELSEWHERE*/
/** Support for template aliases
 * 
 * Template alias:
   \code
   template< typename T >
   using Iterator = typename std::vector<T>::iterator;
   \endcode
 */
#define FLUX_LANGFEAT_TEMPLATE_ALIASES    0 /*OVERRIDDEN_ELSEWHERE*/
/** Support for variadic templates
 */
#define FLUX_LANGFEAT_VARIADIC_TEMPLATES  0 /*OVERRIDDEN_ELSEWHERE*/
/** Support for lambdas
 */
#define FLUX_LANGFEAT_LAMBDA_EXPRESSIONS  0 /*OVERRIDDEN_ELSEWHERE*/
/** Support for defaulted/deleted constructors etc
 *
 * Deleted copy-constructor:
   \code
   struct Type
   {
   		Type() = delete;
   		Type& operator= (Type const&) = delete;
   };
   \endcode
 *
 * Explicit default constructor:
   \code
   struct Type
   {
   		Type() = default;
   		Type( int ) { ... };
   };
   \endcode
 */
#define FLUX_LANGFEAT_EXPLICIT_CTORS      0 /*OVERRIDDEN_ELSEWHERE*/
/** Support for explicit convert operators
 *
 * Explicit convert operator:
   \code
   struct Type
   {
   		explicit operator std::string() const;
   };
   \endcode
 */
#define FLUX_LANGFEAT_EXPLICIT_CONVOPS    0 /*OVERRIDDEN_ELSEWHERE*/
/** Support for virtual override control
 *
 * Explicit override control:
   \code
   struct CantInherit final { ... };
  
   struct Base
   {
   		virtual void f() = 0;
   };
   struct DerivedA : Base
   {
   		void f() override;
   };
   struct DerivedB : Base
   {
   		void f() final;
   };
   \endcode
 */
#define FLUX_LANGFEAT_OVERRIDE_CONTROL    0 /*OVERRIDDEN_ELSEWHERE*/

/** Support for trailing return type
 *
 * Trailing return types:
   \code
   auto f() -> float;
   auto f() -> float { ... };
   \endcode
 */
#define FLUX_LANGFEAT_TRAILING_RETURN     0 /*OVERRIDDEN_ELSEWHERE*/
/** Support for strongly typed enums
 *
 * Strongly typed enums:
   \code
   enum class EFoo
   {
   	 bar,
   	 baz
   };
   enum class EFoo : uint32_t
   {
	 bar,
	 baz
   };
   \endcode
 */
#define FLUX_LANGFEAT_TYPED_ENUM          0 /*OVERRIDDEN_ELSEWHERE*/
/** Support for `nullptr`
 *
 * Support for `nullptr` and `std::nullptr_t`:
   \code
   #include <cstddef>
   void f( std::nullptr_t ) {}
   void g() {
       f(nullptr);
   }
   \endcode
 */
#define FLUX_LANGFEAT_NULLPTR             0 /*OVERRIDDEN_ELSEWHERE*/

/** Support for default arguments for function templates
 *
 * Example:
   \code
   template< typename tType = int >
   void f( tType = 0 )
   {}
   \endcode
 */
#define FLUX_LANGFEAT_DEF_FN_TEMPLATE     0 /*OVERRIDDEN_ELSEWHERE*/

/*  ADDED AFTER DROPPING VS2013 SUPPORT  */

/** Are `alignas` and `alignof` supported?
 */
#define FLUX_LANGFEAT_ALIGNMENT           0 /*OVERRIDDEN_ELSEWHERE*/

/** Is `constexpr` supported?
 */
#define FLUX_LANGFEAT_CONSTEXPR           0 /*OVERRIDDEN_ELSEWHERE*/

/** Is inheriting constructors supported?
 */
#define FLUX_LANGFEAT_INHERIT_CTOR        0 /*OVERRIDDEN_ELSEWHERE*/

/** Are inline namespaces supported?
 */
#define FLUX_LANGFEAT_INLINE_NAMESPACE    0 /*OVERRIDDEN_ELSEWHERE*/

/** Is the `noexcept` keyword supported?
 *
 * Noexcept:
   \code
   void f() noexcept;
   void g() noexcept(noexcept(f));
  
   void h() { if( noexcept(f) ) { ... } }
   \endcode
 *
 * \note Some compilers only support the unparameterized style of `noexecept`,
 * and disallow its use as an operator. `LANGFEAT_NOEXCEPT` will nevertheless
 * be set true. `#FLUX_LANGQUIRK_PARTIAL_NOEXCEPT` indicates if this particular
 * quirk is present.
 */
#define FLUX_LANGFEAT_NOEXCEPT            0 /*OVERRIDDEN_ELSEWHERE*/

/** Are the `char16_t` and `char32_t` types supported?
 *
 * New C++11 character types:
   \code
   char16_t a16;
   char32_t a32;
   \endcode
 *
 * \note Renamed from *flux-gianttoe*
 */
#define FLUX_LANGFEAT_NEW_CHARS           0 /*OVERRIDDEN_ELSEWHERE*/

/** Are reference qualified functions supported?
 *
 * Example:
   \code
   struct Foo
   {
	   void f() &&;
	   void f() const&;
   };
   {}
   \endcode
 *
 * \note Renamed from *flux-gianttoe*.
 */
#define FLUX_LANGFEAT_REF_QUALIFIERS      0 /*OVERRIDDEN_ELSEWHERE*/

/** Is the C++11 attribute syntax supported?
 */
#define FLUX_LANGFEAT_STDATTRIBUTE        0 /*OVERRIDDEN_ELSEWHERE*/

/** Is the `thread_local` storage specifier supported?
 *
 * Thread local:
   \code
   thread_local int value = 0;
   \endcode
 *
 * \note Several compilers implement `thread_local` only partially: it may
 * only be applied to primitive types and with value initializers. If this
 * is the case, `#FLUX_LANGQUIRK_PARTIAL_THREAD_LOCAL` is set to true.
 *
 * \see FLUX_LANGQUIRK_PARTIAL_THREAD_LOCAL
 */
#define FLUX_LANGFEAT_THREAD_LOCAL        0 /*OVERRIDDEN_ELSEWHERE*/

/** Are user literals (=suffix operators) supported?
 */
#define FLUX_LANGFEAT_USER_LITERAL        0 /*OVERRIDDEN_ELSEWHERE*/

//--    C++11 language quirks       ///{{{1///////////////////////////////////

/* Note: langquirks listed here are no longer handled, and must thus not be
 * present. (I.e., they are not required!)
 */

/** Is there only partial support for `noexcept`?
 *
 * Indicates if the compiler supportrs the `noexcept` keyword, but only when
 * used in its simple unparameterized form and/or if the compiler lacks support
 * for the operator form of `noexcept`.
 */
#define FLUX_LANGQUIRK_PARTIAL_NOEXCEPT          0 /*OVERRIDDEN_ELSEWHERE*/

/** Is there only partial support for `thread_local`?
 *
 * Indicates that the compiler implements `thread_local` with support for
 * primitive types with value initializers only.
 */
#define FLUX_LANGQUIRK_PARTIAL_THREAD_LOCAL      0 /*OVERRIDDEN_ELSEWHERE*/

/** Are defaulted move constructors supported?
 *
 * Indicates compilers that do not allow move constructors and assignments to
 * be defaulted.
 */
#define FLUX_LANGQUIRK_NO_DEFAULT_MOVE           0 /*OVERRIDDEN_ELSEWHERE*/

//--    C++14 features              ///{{{1///////////////////////////////////

/** Is the `[[deprecated]]` attribute supported?
 *
 * Deprecated attribute:
   \code
   [[deprecated]] int f();
   [[deprecated("this function is deprecated, it is deemed unsafe")]]
   float g() { return 0.0f/0.0f; }
   \endcode
 *
 * \note Renamed from *flux-gianttoe*
 */
#define FLUX_LANGFEAT_ATTR_DEPRECATED     0 /*OVERRIDDEN_ELSEWHERE*/

/** Are binary literals supported?
 *
 * Binary literals:
   \code
   int mask = 0b00100011
   \endcode
 */
#define FLUX_LANGFEAT_BINARY_LITERAL      0 /*OVERRIDDEN_ELSEWHERE*/

/** Is `decltype(auto)` supported?
 */
#define FLUX_LANGFEAT_DECLTYPE_AUTO       0 /*OVERRIDDEN_ELSEWHERE*/

/** Are generalized lambda captures supported?
 */
#define FLUX_LANGFEAT_GENERALIZED_LAMBDA  0 /*OVERRIDDEN_ELSEWHERE*/

/** Are generic lambdas supported?
 */
#define FLUX_LANGFEAT_GENERIC_LAMBDA      0 /*OVERRIDDEN_ELSEWHERE*/

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "../detail/ext-langfeat_required.hxx"
#endif // FLUX_SETUP_EXT_LANGFEAT_REQUIRED_HPP_B1CFCE99_F49F_4A7B_8CD4_7B1147A664F9
