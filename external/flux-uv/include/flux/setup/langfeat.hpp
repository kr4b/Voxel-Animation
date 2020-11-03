/*-******************************************************* -- HEADER -{{{1- */
/*-	(Optional) language-features supported by compiler
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SETUP_LANGFEAT_HPP_0C7F2D3C_E74E_4F67_92F1_60EBE7227203
#define FLUX_SETUP_LANGFEAT_HPP_0C7F2D3C_E74E_4F67_92F1_60EBE7227203

//--//////////////////////////////////////////////////////////////////////////
//--    Include                     ///{{{1///////////////////////////////////

#include "defaults.hpp"
#include "compiler.hpp"

//--    Required features           ///{{{1///////////////////////////////////

/* List of required C++ features.
 *
 * C++11:
 *  - auto
 *  - decltype
 *  - defaulted & deleted functions (*)
 *  - delegating constructors
 *  - explicit conversion operators
 *  - enum class ("strong enum") + forward enum decls
 *  - initializer list
 *  - static_assert
 *  - lambdas
 *  - template aliases
 *  - variadic templates
 *  - range-based for
 *  - override and final
 *  - rvalue references
 *  - trailing function return types
 *  - nullptr
 *  - default function template parameters
 *  - non-static member initialization
 *
 * Additionally, after dropping support for Visual Studio 2013 around 2020,
 * the following C++11 features are also required:
 *
 *  - alignas(), alignof()
 *  - (basic) constexpr
 *  - inheriting constructors
 *  - inline namespaces
 *  - noexcept
 *  - char16_t, char32_t
 *  - member function reference qualifiers
 *  - [[standard attribute syntax]] (*)
 *  - thread_local (full)
 *  - "user"_literals
 *
 * Further, the following C++14 features are required as well
 *
 *  - [[deprecated]] (**)
 *  - binary literals
 *  - decltype(auto)
 *  - generalized lambda capture ([x = std::move(y)])
 *  - generic lambdas ([](auto x) {...})
 *  - sized deallocations
 *  - single quote as digit separator (***)
 *
 * (*)    However, see `FLUX_LANGQUIRK_PARTIAL_STDATTRIB` below
 * (**)   Flux uses internally a macro for this still, see
 *        `FLUX_DEPRECATED` and `FLUX_DEPRECATED_EX`.
 * (***)  This was never a `LANGFEAT`
 *
 * Older `LANGFEAT`s` that are now required are listed in the
 * `ext/langfeat_required.hpp` header.
 */

//--    C++14 features              ///{{{1///////////////////////////////////

/** Are the extended `constexpr` supported?
 *
 * Extended `constexpr` relaxes the restrictions on `constexpr` functions and
 * methods.
 */
#define FLUX_LANGFEAT_EXTENDED_CONSTEXPR  0 /*OVERRIDDEN_ELSEWHERE*/

/** Are sized deallocations supported?
 */
#define FLUX_LANGFEAT_SIZED_DEALLOCATIONS 0 /*OVERRIDDEN_ELSEWHERE*/

/** Are variable templates supported?
 *
 * Variable template:
   \code
   template< typename T >
   T pi = T(3.14159265358979323846264338327950288419716939937510);
   \endcode
 */
#define FLUX_LANGFEAT_VARIABLE_TEMPLATE   0 /*OVERRIDDEN_ELSEWHERE*/

//--    C++14 language quirks       ///{{{1///////////////////////////////////

/** Are `[[attributes]]` only supported partially? 
 *
 * Several Visual Studios (`_MSC_VER` \f$\le\f$ 1916) do not fully support
 * the `[[attribute]]` syntax, and produce errrors when it's used in certain
 * configurations. Examples:
   \code
   // Applied to type alias
   using T [[deprecated]] = void*;

   // Applied to templated constructor
   struct X
   {
  	template< class T >
    [[deprecated]]
    X( T const& );
   };
   \endcode
 * This will produce an syntax error (but the `__declspec()` method will not).
 */
#define FLUX_LANGQUIRK_PARTIAL_STDATTRIB  0 /*OVERRIDDEN_ELSEWHERE*/

//--    C++17 features              ///{{{1///////////////////////////////////

/** Is the `[[fallthrough]]` attribute supported?
 *
 * The `fallthrough` attribute is used to explicitely mark `case`s that 
 * fallthrough to the next option:
   \code
   switch( x )
   {
   		case foo:
   			[[fallthrough]];
   		case bar:
   			foo_or_bar();
   }
   \endcode
 *
 * Use of the `fallthrough` attribute encourages compilers to warn about cases
 * which fallthrough without being explicitely marked as such.
 * 
 * See http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0188r1.pdf
 *
 * \note Renamed from *flux-gianttoe*
 */
#define FLUX_LANGFEAT_ATTR_FALLTHROUGH    0 /*OVERRIDDEN_ELSEWHERE*/

/** Is the `[[maybe_unused]]` attribute supported?
 *
 * The `maybe_unused` attribute is used to mark variables/functions/typedefs
 * as possibly unused, which aught to suppress emitting warnings when such
 * objects are actually unused. This helps with conditionally-compiled code
 * (via for example `NDEBUG` and `assert()`).
 *
 * Example:
   \code
   [[maybe_unused]] void f( [[maybe_unused]] void* aPtr ) {
   	[[maybe_unused]] void* ptr = g(aPtr);
   	assert( ptr );
   }
   \endcode
 *
 * See http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0212r1.pdf
 *
 * \note Renamed from *flux-gianttoe*
 */
#define FLUX_LANGFEAT_ATTR_MAYBE_UNUSED   0 /*OVERRIDDEN_ELSEWHERE*/

/** Is the `[[nodiscard]]` attribute supported?
 *
 * The `nodiscard` attribute is used to mark return values that aught not to
 * be ignored (=discarded), because doing so may have unintended consequences.
 * 
 * The attribute may be applied to both function declarations and to class/enum
 * declarations:
   \code
   struct [[nodiscard]] PreciousInfo { ... };
  
   PreciousInfo do_something();
  
   [[nodiscard]] void* get_raw_handle();
  
   ..
  
   do_something(); // Emit warning -- PreciousInfo should never be discarded
   get_raw_handle(); // Emit warning -- return value of get_raw_handle() aught
                     // not to be ignored.
   \endcode
 *
 * See http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0189r1.pdf
 *
 * \note Renamed from *flux-gianttoe*
 */
#define FLUX_LANGFEAT_ATTR_NODISCARD      0 /*OVERRIDDEN_ELSEWHERE*/

/** Is `if constexpr` supported?
 *
 * Supports the `constexpr` `if` statement:
 * \code
 * if constexpr( somecondition ) { ... }
 * \endcode
 *
 * See http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0292r2.html
 */
#define FLUX_LANGFEAT_CONSTEXPR_IF        0 /*OVERRIDDEN_ELSEWHERE*/

/** Are initialization statments in `if` and `switch` supported?
 *
 * Supports initialization statements for `if` and `switch` statements:
   \code
   if( auto x = f(); x != 5 )
   {
      ...
   }
   \endcode
 *
 * See http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0305r1.html
 */
#define FLUX_LANGFEAT_INIT_STATEMENT      0 /*OVERRIDDEN_ELSEWHERE*/

/** Are nested namespace definitions supported?
 *
 * Support for nested namespace definitions:
   \code
   namespace x::y::z { ... }
   \endcode
 *
 * See http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4230.html
 */
#define FLUX_LANGFEAT_NESTED_NAMESPACES   0 /*OVERRIDDEN_ELSEWHERE*/

/** Is the short version of `static_assert()` supported?
 *
 * Supports `static_assert()` without a message.
 */
#define FLUX_LANGFEAT_SHORT_STATIC_ASSERT 0 /*OVERRIDDEN_ELSEWHERE*/

/** Are structured bindings supported?
 *
 * Supports structured bindings:
   \code
   struct X { int a, b; float c; };
   
   X x = { ... };
   auto [a, b, c] = x;
   \endcode
 *
 * See http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0217r3.html
 */
#define FLUX_LANGFEAT_STRUCTURED_BINDINGS 0 /*OVERRIDDEN_ELSEWHERE*/

/** Are `auto`-template parameters supported?
 *
 * `auto` template parameters may be used to create template parameters of an
 * arbitrary type; their type can be retrieved via `decltype()`:
   \code
   template< auto tValue > struct Struct { ... };
   \endcode
 *
 * See http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0127r2.html
 */
#define FLUX_LANGFEAT_TEMPLATE_AUTO       0 /*OVERRIDDEN_ELSEWHERE*/

/** Are hexadecimal floating point constants supported?
 *
 * Supports hexadecimal floating point constants:
   \code
   float x = 0x1.de3d42p-8f;
   \endcode
 *
 * See http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0245r1.html
 *
 * \note Several compilers support these even outside of C++17.
 */
#define FLUX_LANGFEAT_HEXFLOAT            0 /*OVERRIDDEN_ELSEWHERE*/

/** Are inline variables supported?
 *
 * Supports inline variables, e.g.
   \code
   template< class tList >
   inline constexpr std::size_t list_size_v = list_size<tList>::value;
   \endcode
 */
#define FLUX_LANGFEAT_INLINE_VARIABLE     0 /*OVERRIDDEN_ELSEWHERE*/

/** Is `noexcept` supported in a type alias / typedef?
 *
 * Supports function pointer type aliases / typedefs that are `noexcept`:
   \code
   using TypeAlias = void (*)() noexcept;
   typdef void (*TypeDef)() noexcept;
   \endcode
 *
 * Before C++17, it was possible to define a function pointer to a function
 * that doesn't throw, but the `noexcept` was not part of the type, meaning
 * that it was impossible to define function pointer types that were 
 * `noexcept`.
 *
 * flux-compat defines `FLUX_TYPEDEF_NOEXCEPT` which evaluates to `noexcept`
 * if supported and nothing otherwise.
 */
#define FLUX_LANGFEAT_TYPEDEF_NOEXCEPT    0 /*OVERRIDDEN_ELSEWHERE*/

/* TODO:
 *  
 *  - fold expressions
 *  - __has_include
 *  - template argument deduction for class templates
 *  - constexpr lambda
 */

//--    C++20 features              ///{{{1///////////////////////////////////

/** Are immediate functions (`consteval`) supported?
 *
 * Supports immediate functions via the `consteval` keyword, e.g.
   \code
   consteval int sqr( int n ) { return n*n; }
   \endcode
 *
 * \note `<flux/compat/constexpr.hpp>` defines `FLUX_CONSTEVAL` that results in
 * either `consteval` or `constexpr`, depending on whether the former is
 * supported.
 */
#define FLUX_LANGFEAT_CONSTEVAL           0 /*OVERRIDDEN_ELSEWHERE*/

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/langfeat.hxx"
#endif // FLUX_SETUP_LANGFEAT_HPP_0C7F2D3C_E74E_4F67_92F1_60EBE7227203
