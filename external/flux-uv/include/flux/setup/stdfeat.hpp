/*-******************************************************* -- HEADER -{{{1- */
/*-	Standard library features
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SETUP_STDFEAT_HPP_231E1158_3615_464B_AE09_36E997B1D25D
#define FLUX_SETUP_STDFEAT_HPP_231E1158_3615_464B_AE09_36E997B1D25D

//--//////////////////////////////////////////////////////////////////////////
//--    Include                     ///{{{1///////////////////////////////////

#include "defaults.hpp"
#include "compiler.hpp"
#include "platform.hpp"

//--    C++11 lib features          ///{{{1///////////////////////////////////

/** Is there a working `std::system_error`?
 *
 * Available as `flux::compat::system_error` and similar, with fallbacks if
 * not yet supported by the standard library.
 */
#define FLUX_STDFEAT_CXX11_SYSERROR      0 /*OVERRIDDEN_ELSEWHERE*/

//--    C++11 lib quirks            ///{{{1///////////////////////////////////

/** Is std::timed_mutex and std::recursive_timed_mutex missing?
 *
 * The Cygwin libstdc++ seems to be missing
 * 	- `std::timed_mutex`
 * 	- `std::recursive_timed_mutex`
 *
 * This affects `#FLUX_STDFEAT_CXX11_THREAD`.
 */
#define FLUX_STDQUIRK_CXX11_NO_TIMED_MUTEX    0 /*OVERRIDDEN_ELSEWHERE*/

/** Does `std::rethrow_exception()` break `std::uncaught_exception()`?
 *
 * Some versions of libstdc++ fail to reset a counter when
 * `std::rethrow_exception()` is used, which causes `std::uncaught_exception()`
 * to always return true.
 *
 * In particular, this used to break Catch, the unit testing framework that flux
 * uses internally. See https://github.com/philsquared/Catch/issues/352 .
 */
#define FLUX_STDQUIRK_CXX11_UNCAUGHT_RETHROW  0 /*OVERRIDDEN_ELSEWHERE*/

/** Is `set_value_at_thread_exit()` supported?
 *
 * Some versions of libstdc++ lack, in `std::promise<>`, support for
 *   - `set_value_at_thread_exit()`
 *   - `set_exception_at_thread_exit()` (presumably)
 */
#define FLUX_STDQUIRK_CXX11_NO_AT_THREAD_EXIT 0 /*OVERRIDDEN_ELSEWHERE*/

/** Is `std::aligned_union` missing?
 */
#define FLUX_STDQUIRK_CXX11_NO_ALIGNED_UNION  0 /*OVERRIDDEN_ELSEWHERE*/

/** Are the `std::is_trivially_*<>` traits available?
 *
 * `std::is_trivially_*<>` (see list below) seem the be a later addition, and
 * are not always available even if the C++11 type traits are otherwise
 * present.
 *
 * This concerns the traits:
 *
 *  - `std::is_trivially_copyable<>`
 *  - `std::is_trivially_destructible<>`
 *  - `std::is_trivially_constructible<>`
 *  - `std::is_trivially_copy_constructible<>`
 *  - `std::is_trivially_move_constructible<>`
 *  - `std::is_trivially_default_constructible<>`
 *  - `std::is_trivially_assignable<>`
 *  - `std::is_trivially_copy_assignable<>`
 *  - `std::is_trivially_move_assignable<>`
 * 
 * \note Unless otherwise noted, these traits seem to have become available
 * with GCC 5.x and Visual Studio 2015. Visual Studio 2013 provides some of
 * the traits with non-standard names (e.g., `has_trivially_...`).
 *
 * XXX: is_trivially_destructible<> seems to exist on VS2013?
 */
#define FLUX_STDQUIRK_CXX11_NO_TRIVIALLY_TRAITS  0 /*OVERRIDDEN_ELSEWHERE*/

/** Is `assert()` usable inside a `constexpr` function?
 *
 * Early GCC (<= 4.9.x) do not accept `assert()` inside a `constexpr` function,
 * even when such follows the other rules carefully (i.e., single return
 * statement).
 */
#define FLUX_STDQUIRK_CXX11_NONCONSTEXPR_ASSERT   0 /*OVERRIDDEN_ELSEWHERE*/

/** Do `std::unitialized_fill_n()` and similar still provide the pre-C++11 API?
 *
 * Early GCC (<= 4.9.x) retained the old API of `unitialized_fill_n()` and
 * similar that do not return the end iterator.
 */
#define FLUX_STDQUIRK_CXX11_OLD_UNINITIALIZED     0 /*OVERRIDDEN_ELSEWHERE*/

/** Does `call_once` handle exceptions?
 *
 * The C++11 standard requires `std::call_once()` to handle exceptions in a 
 * very specific way. Exceptions need to be propagated to the caller of 
 * `call_once`, and future executions of `call_once` should re-attempt to run
 * the specified method until it succeeds (by returning normally).
 *
 * Initially most standard libraries/compilers did not support this. 
 *   - Visual Studio 2012 seems to leak a mutex.
 *   - libstdc++ calls `pthread_once()` and seems to deadlock on the second
 *     call to `call_once()`.
 *
 * TODO: status in VS2013?
 */
#define FLUX_STDQUIRK_CXX11_CALL_ONCE_EXCEPT  0 /*OVERRIDDEN_ELSEWHERE*/

//--    C++14 lib features          ///{{{1///////////////////////////////////

/** Is the C++14 `%std::make_unique()` helper available?
 *
 * Available as `flux::compat::make_unique()`, with a fallback should the
 * function not yet be defined by the standard library.
 */
#define FLUX_STDFEAT_CXX14_MAKE_UNIQUE        0 /*OVERRIDDEN_ELSEWHERE*/

/** Is the C++14 `%std::get()` with a type available?
 *
 * Available as `flux::compat::get()`, with a fallback should the function not
 * yet be defined by the standard library.
 */
#define FLUX_STDFEAT_CXX14_TUPLES_BY_TYPE     0 /*OVERRIDDEN_ELSEWHERE*/

/** Are the short C++14 type traits names available?
 *
 * Are the short C++14 type trait names available, such that `std::decay_t<...>`
 * may be used instead of `typename std::decay<...>::%type`?
 *
 * Available as `flux::compat::decay_t` with fallbacks defined if not yet
 * supported by the standard library.
 */
#define FLUX_STDFEAT_CXX14_TYPE_TRAITS        0 /*OVERRIDDEN_ELSEWHERE*/

/** Is the C++14 `integer_sequence<>` available?
 *
 * C++14 introduces `std::integer_sequence<>` and `make_integer_sequence<>`,
 * which help when dealing with `constexpr` arrays and other constexpr/meta-
 * programming related tasks.
 *
 * Available as `flux::compat::integer_sequence` et al., and with fallbacks
 * defined if not yet supported by the standard library.
 */
#define FLUX_STDFEAT_CXX14_INTEGER_SEQ        0 /*OVERRIDDEN_ELSEWHERE*/

/** Is the C++14 `std::exchange` available?
 *
 * From cppreference.com: "Replaces the value of obj with new_value and 
 * returns the old value of obj." This is somewhat useful in move constructors.
 */
#define FLUX_STDFEAT_CXX14_EXCHANGE           0 /*OVERRIDDEN_ELSEWHERE*/


//TODO: std::less<> , i.e., std::less<void>...

//--    C++17 lib features          ///{{{1///////////////////////////////////

/** Is the C++17 `%std::clamp()` helper available?
 *
 * Available as `flux::compat::clamp` with a pre-C++17 fallback ddefined if not
 * yet availabe in the standard library.
 */
#define FLUX_STDFEAT_CXX17_CLAMP              0 /*OVERRIDDEN_ELSEWHERE*/

/** Is the C++17 `%std::void_t` helper available?
 *
 * Available as `flux::compat::void_t` with a pre-C++17 fallback defined if not
 * yet supported by the standard library.
 */
#define FLUX_STDFEAT_CXX17_VOIDT              0 /*OVERRIDDEN_ELSEWHERE*/

/** Is the C++17 `std::byte` available?
 *
 * Available as `flux::compat::byte` and `flux::byte`, with pre-C++17 fallbacks
 * if not yet supported by the standard library.
 */
#define FLUX_STDFEAT_CXX17_BYTE               0 /*OVERRIDDEN_ELSEWHERE*/

/** Is `<optional>` available?
 *
 * Available as `flux::compat::optional` and similar, with pre-C++17 fallbacks
 * if not yet supported by the standard library.
 */
#define FLUX_STDFEAT_CXX17_OPTIONAL           0 /*OVERRIDDEN_ELSEWHERE*/

/** Is `%std::launder()` available?
 *
 * Available as `flux::compat::launder`, with a pre-C++17 fallback/stub if not
 * yet supported by the standard library.
 */
#define FLUX_STDFEAT_CXX17_LAUNDER            0 /*OVERRIDDEN_ELSEWHERE*/

/** Are the short C++17 type trait variable templates available?
 *
 * Are the short C++17 type trait variable templates available, such that
 * `std::is_same_v<...>` may be used instead of `std::decay<...>::%tvalue`?
 */
#define FLUX_STDFEAT_CXX17_TYPE_TRAITS        0 /*OVERRIDDEN_ELSEWHERE*/

/** Is `<filesystem>` available?
 *
 * Is the proper C++17 `<filesystem>` available, with the corresponding
 * functions and classes placed in the `std::filesystem` namespace?
 * 
 * Alternatively, see `#FLUX_STDFEAT_XP_FILESYSTEM`
 */
#define FLUX_STDFEAT_CXX17_FILESYSTEM         0 /*OVERRIDDEN_ELSEWHERE*/

/** Is `iterator_traits<>` SFINAE-friendly?
 *
 * The C++11 `std::iterator_traits<>` isn't necessarily SFINAE friendly; this
 * is a requirement in C++17, though. (libstdc++'s implementation seems to be
 * SFINAE friendly from the get-go, though.)
 *
 * Guaranteed to-be-SFINAE-friendly `iterator_traits` are available as
 * `flux::compat::iterator_traits`.
 *
 * \note `std::iterator_traits<>` not working properly in a SFINAE-like context
 * was observed in ftl::Vector<> under Visual Studio 2013. Google pointed
 * towards several other people having similar problems and to the conclusion
 * that `std::iterator_traits<>` may not be SFINAE-friendly in VS 2013.
 */
#define FLUX_STDFEAT_CXX17_ITERATOR_TRAITS    0 /*OVERRIDDEN_ELSEWHERE*/

/** Are the C++17 `std::uninitialized_*()` functions available?
 *
 * C++17 adds several methods:
 *   - `std::uninitialized_move()`
 *   - `std::uninitialized_move_n()`
 *   - `std::uninitialized_value_construct()`
 *   - `std::uninitialized_value_construct_n()`
 *   - `std::uninitialized_default_construct()`
 *   - `std::uninitialized_default_construct_n()`
 *   - `std::destroy()`
 *   - `std::destroy_n()`
 *   - `std::destroy_at()`
 * to the previously available `uninitialized_copy` and `uninitialized_fill()`.
 * `FLUX_STDFEAT_CXX17_UNINITIALIZED` indicates that the new methods are 
 * available.
 *
 * Available as `flux::compat::uninitialized_move` et al., with pre-C++17
 * fallbacks available if not yet supported by the standard library.
 */
#define FLUX_STDFEAT_CXX17_UNINITIALIZED      0 /*OVERRIDDEN_ELSEWHERE*/

/** Is the C++17 `std::basic_string_view` available?
 *
 * A.k.a. `std::string_view` et al.
 *
 * Available as `flux::compat::string_view` and similar, with pre-C++17
 * fallbacks available if not yet supported by the standard library.
 */
#define FLUX_STDFEAT_CXX17_STRING_VIEW        0 /*OVERRIDDEN_ELSEWHERE*/

/** Is the C++17 `std::invoke` available?
 *
 * Also covers `std::invoke_result` and `std::is_nothrow_invocable`.
 *
 * Available as `flux::compat::invoke` with a pre-C++17 fallback if not yet
 * supported by the standard library.
 */
#define FLUX_STDFEAT_CXX17_INVOKE             0 /*OVERRIDDEN_ELSEWHERE*/

/** Is the C++17 `std::variant` available?
 *
 * Available as `flux::compat::variant` with a pre-C++17 fallback if not yet
 * supported by the standard library.
 */
#define FLUX_STDFEAT_CXX17_VARIANT            0 /*OVERRIDDEN_ELSEWHERE*/

/** Is the C++17 `std::uncaught_exceptions()` (plural) available?
 *
 * C++17 deprecates `std::uncaught_exception()` (singular) in favour of 
 * `std::uncaught_exceptions()` (plural). MSVC complains about the former.
 */
#define FLUX_STDFEAT_CXX17_UNCAUGHT_EXCEPTIONS  0 /*OVERRIDDEN_ELSEWHERE*/

/** Is the C++17 `std::shared_mutex` et al. available?
 */
#define FLUX_STDFEAT_CXX17_SHARED_MUTEX       0 /*OVERRIDDEN_ELSEWHERE*/

//--    C++17 lib quirks            ///{{{1///////////////////////////////////

/** Is the C++17 stdlib partially missing?
 *
 * This quirk identifies the situation where the compiler claims to be C++17
 * capable, but gets stuck on a pre-C++17 standard library. This issue occurs
 * with Clang, where the compiler itself indeed is C++17 capable, but uses
 * an older `libstdc++` that does not include full C++17 capabilities.
 */
#define FLUX_STDQUIRK_CXX17_MISSING_STDLIB17  0 /*OVERRIDDEN_ELSEWHERE*/

//--    C++20/2a lib features       ///{{{1///////////////////////////////////

/** Is the C++20 `std::span` available?
 *
 * See https://en.cppreference.com/w/cpp/container/span
 *
 * `<flux/compat/span.hpp>` provides `compat::span`, which is an alias for
 * `std::span` (when it is available) and a fallback implementation otherwise.
 */
#define FLUX_STDFEAT_CXX20_SPAN               0 /*OVERRIDDEN_ELSEWHERE*/

/** Is the C++20 `std::bit_cast` available?
 *
 * See https://en.cppreference.com/w/cpp/numeric/bit_cast
 *
 * `<flux/compat/bit_cast.hpp>` provides `compat::bit_cast`, which is an alias
 * for `std::bit_cast` (when it is available) and a fallback implementation
 * otherwise. Note that the fallback implementation relies on `std::memcpy()`
 * and is therefore not `constexpr`.
 */
#define FLUX_STDFEAT_CXX20_BIT_CAST           0 /*OVERRIDDEN_ELSEWHERE*/

/** Is the C++20 `std::to_address()` available?
 *
 * See http://en.cppreference.com/w/cpp/memory/to_address
 *
 * `<flux/compat/to_address.hpp>` provides `compat::to_address`, which is an
 * alias for `std::to_address` (when it is available) and a fallback
 * implementation otherwise.
 */
#define FLUX_STDFEAT_CXX20_TO_ADDRESS         0 /*OVERRIDDEN_ELSEWHERE*/

/** Is the C++20 `std::is_constant_evaluated()` available?
 *
 * See https://en.cppreference.com/w/cpp/types/is_constant_evaluated
 *
 * \note At the time of writing, the compiler support pages on cppreference
 * list this under language features (although the feature test macro name
 * indicates that it should be considered a library feature). It's not possible
 * to implement without compiler magic. Flux provides a fallback
 * (`compat::is_constant_evaluated()`) that defaults to `false` when the C++20
 * version is unavailable.
 */
#define FLUX_STDFEAT_CXX20_IS_CONSTEVAL       0 /*OVERRIDDEN_ELSEWHERE*/

/** Is `std::source_location` available?
 *
 * See <a href="https://en.cppreference.com/w/cpp/utility/source_location">
 * cppreference: std::source_location</a>.
 *
 * Available as `flux::compat::source_location` via `#flux::compat`.
 */
#define FLUX_STDFEAT_CXX20_SOURCE_LOCATION    0 /*OVERRIDDEN_ELSEWHERE*/

/** Is `std::barrier` & `std::latch  available?
 *
 * TODO: not yet on cppreference 
 *
 * Available as `flux::compat::barrier` / `flux::compat::latch` via
 * `#flux::compat`.
 *
 * \note This combines both latches and barriers, despite them having different
 * feature macros and residing in different headers.
 */
#define FLUX_STDFEAT_CXX20_BARRIER           0 /*OVERRIDDEN_ELSEWHERE*/

//--    experimental features       ///{{{1///////////////////////////////////

/** Is `<experimental/optional>` available?
 *
 * Alternatively, see `#FLUX_STDFEAT_CXX17_OPTIONAL`. Available as 
 * `flux::compat::optional` via `#flux::compat`.
 */
#define FLUX_STDFEAT_XP_OPTIONAL              0 /*OVERRIDDEN_ELSEWHERE*/

/** Is `<experimental/filesystem>` available?
 *
 * Alternatively, see `#FLUX_STDFEAT_CXX17_FILESYSTEM`.
 */
#define FLUX_STDFEAT_XP_FILESYSTEM            0 /*OVERRIDDEN_ELSEWHERE*/

/** Is `<experimental/source_location>` available?
 *
 * See <a href="http://en.cppreference.com/w/cpp/experimental/source_location">
 * cppreference: experimental::source_location</a>.
 *
 * Available as `flux::compat::source_location` via `#flux::compat`.
 */
#define FLUX_STDFEAT_XP_SOURCE_LOCATION       0 /*OVERRIDDEN_ELSEWHERE*/

/** Is `std::experimental::observer_ptr` available?
 *
 * Is `std::experimental::observer_ptr` available in `<experimental/memory>`?
 * See <a href="http://en.cppreference.com/w/cpp/experimental/observer_ptr">
 * cppreference: experimental::observer_ptr</a>.
 *
 * Available as `flux::compat::observer_ptr` via `#flux::compat`. Also 
 * available as `flux::observer_ptr` for convenience.
 */
#define FLUX_STDFEAT_XP_OBSERVER_PTR          0 /*OVERRIDDEN_ELSEWHERE*/

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/stdfeat.hxx"
#endif // FLUX_SETUP_STDFEAT_HPP_231E1158_3615_464B_AE09_36E997B1D25D
