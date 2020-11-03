/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Standard library features from feature macros
 *
 * The feature test macros are documented  at cppreference.com:
 *   https://en.cppreference.com/w/cpp/experimental/feature_test
 *   https://en.cppreference.com/w/User:D41D8CD98F/feature_testing_macros
 *
 * Note that there is a bit of a mess going on with this, as evidenced by
 * GCC Bug 79433 (https://gcc.gnu.org/bugzilla/show_bug.cgi?id=79433).
 *
 * The current recommendation seems to be to
 *   1. check if the header is available (__has_include)
 *   2. #include the header (!!!!)
 *   3. check the macro value (__cpp_lib_xyz)
 * (this matches the fact that library feature macros __cpp_lib_* aren't
 * predefined, but rather something found in the corresponding headers.)
 *
 * This has the unfortunate side-effect of including a ton of standard headers
 * all the time. :-(
 *
 * *Edit*: Bonus fun time: this does *not* work on newer VS2017 (and perhaps
 * later ones). These support __has_include(), but error when certain headers
 * are actually included. E.g.:
 *    __has_include(<optional>)  ==> true
 *    #include <optional> ==> #error class template optional is only available
 *                             with C++17.
 * 
 * _MSC_FULL_VER == 191526726 OK (although prints warning)
 * _MSC_FULL_VER == 191426423 problematic
 * _MSC_FULL_VER == 191326128 problematic
 * _MSC_FULL_VER == 191025017 doesn't have __has_include
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_STDFEAT_CONF_FEATURE_HXX_356217F4_1F5A_4CD6_AFAB_29B2A42DC865
#define DETAIL_STDFEAT_CONF_FEATURE_HXX_356217F4_1F5A_4CD6_AFAB_29B2A42DC865

//--//////////////////////////////////////////////////////////////////////////
//--    Visual Studio Workaround    ///{{{1///////////////////////////////////

/* Read the description in the header. Just assume that Visual Studio doesn't
 * have a usable __has_include(). Bleh.
 */
#if FLUX_COMPILER == FLUX_COMPILER_MSVC && _MSC_FULL_VER < 191526726
#	define FLUXTMP_USE_HAS_INCLUDE_ 0
#else // Other compilers that behave ... better
#	if defined(__has_include)
#		define FLUXTMP_USE_HAS_INCLUDE_ 1
#	else
#		define FLUXTMP_USE_HAS_INCLUDE_ 0
#	endif
#endif // ~ COMPILER

//--    Check for <version>         ///{{{1///////////////////////////////////
#define FLUXTMP_NEED_INDIVIDUAL_HEADERS_ 1

#if FLUXTMP_USE_HAS_INCLUDE_
#	if __has_include(<version>)
#		include <version>

#		undef FLUXTMP_NEED_INDIVIDUAL_HEADERS_
#		define FLUXTMP_NEED_INDIVIDUAL_HEADERS_ 0
#	endif // ~ __has_include
#endif //  USE_HAS_INCLUDE

//--    C++11                       ///{{{1///////////////////////////////////

// No C++11 standard library features exposed this way... ?

//--    C++14                       ///{{{1///////////////////////////////////

#if FLUXTMP_NEED_INDIVIDUAL_HEADERS_
#	include <memory>
#endif // ~ NEED_INDIVIDUAL_HEADERS_
#if defined(__cpp_lib_make_unique) && __cpp_lib_make_unique >= 201304
#	undef FLUX_STDFEAT_CXX14_MAKE_UNIQUE
#	define FLUX_STDFEAT_CXX14_MAKE_UNIQUE 1
#endif // ~ __cpp_lib_make_unique

#if FLUXTMP_NEED_INDIVIDUAL_HEADERS_
#	include <utility>
#endif // ~ NEED_INDIVIDUAL_HEADERS_
#if defined(__cpp_lib_tuples_by_type) && __cpp_lib_tuples_by_type >= 201304
#	undef FLUX_STDFEAT_CXX14_TUPLES_BY_TYPE
#	define FLUX_STDFEAT_CXX14_TUPLES_BY_TYPE 1
#endif // ~ __cpp_lib_tuples_by_type

//#include <utility> // already included
#if defined(__cpp_lib_integer_sequence) && __cpp_lib_integer_sequence >= 210304
#	undef FLUX_STDFEAT_CXX14_INTEGER_SEQ
#	define FLUX_STDFEAT_CXX14_INTEGER_SEQ 1
#endif // ~ __cpp_lib_integer_sequence

//#include <utility> // already included
#if defined(__cpp_lib_exchange_function) && __cpp_lib_exchange_function >= 201304
#	undef FLUX_STDFEAT_CXX14_EXCHANGE
#	define FLUX_STDFEAT_CXX14_EXCHANGE 1
#endif // ~ __cpp_lib_exchange_function

//MISSING/not yet implemented: CXX14_TYPE_TRAITS

//--    C++17                       ///{{{1///////////////////////////////////

/* Note: these are not finalized at the time of writing, so the names and
 * values might change (the features may go missing allthogether).
 */

#if FLUXTMP_NEED_INDIVIDUAL_HEADERS_
#	include <cstddef>
#endif // ~ NEED_INDIVIDUAL_HEADERS_
#if defined(__cpp_lib_byte) && __cpp_lib_byte >= 201604
#	undef FLUX_STDFEAT_CXX17_BYTE
#	define FLUX_STDFEAT_CXX17_BYTE 1
#endif // ~ __cpp_lib_byte

#if FLUXTMP_NEED_INDIVIDUAL_HEADERS_
#	include <algorithm>
#endif // ~ NEED_INDIVIDUAL_HEADERS_
#if defined(__cpp_lib_clamp) && __cpp_lib_clamp >= 201603
#	undef FLUX_STDFEAT_CXX17_CLAMP
#	define FLUX_STDFEAT_CXX17_CLAMP 1
#endif // ~ __cpp_lib_clamp

#if FLUXTMP_NEED_INDIVIDUAL_HEADERS_
#	include <type_traits>
#endif // ~ NEED_INDIVIDUAL_HEADERS_
#if defined(__cpp_lib_void_t) && __cpp_lib_void_t >= 201411
#	undef FLUX_STDFEAT_CXX17_VOIDT
#	define FLUX_STDFEAT_CXX17_VOIDT 1
#endif // ~ __cpp_lib_void_t

#if FLUXTMP_USE_HAS_INCLUDE_ && FLUXTMP_NEED_INDIVIDUAL_HEADERS_
#	if __has_include(<optional>)
#		include <optional>
#	endif // ~ has <optional>
#endif // ~ USE_HAS_INCLUDE_
#if defined(__cpp_lib_optional) && __cpp_lib_optional >= 201606
#	undef FLUX_STDFEAT_CXX17_OPTIONAL
#	define FLUX_STDFEAT_CXX17_OPTIONAL 1
#endif // ~ __cpp_lib_optional

#if FLUXTMP_NEED_INDIVIDUAL_HEADERS_
#	include <new>
#endif // ~ NEED_INDIVIDUAL_HEADERS_
#if defined(__cpp_lib_launder) && __cpp_lib_launder >= 201606
/* Proposed name by P0137, see
 * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0137r1.html
 */
#	undef FLUX_STDFEAT_CXX17_LAUNDER
#	define FLUX_STDFEAT_CXX17_LAUNDER 1
#endif // ~ __cpp_lib_launder

#if FLUXTMP_USE_HAS_INCLUDE_ && FLUXTMP_NEED_INDIVIDUAL_HEADERS_
#	if __has_include(<filesystem>)
#		include <filesystem>
#	endif // ~ has <filesystem>
#endif // ~ USE_HAS_INCLUDE_
#if defined(__cpp_lib_filesystem)
#	undef FLUX_STDFEAT_CXX17_FILESYSTEM
#	define FLUX_STDFEAT_CXX17_FILESYSTEM 1
#endif // ~ __cpp_lib_filesystem

#if 0
#if defined(__cpp_lib_filesystem) && __cpp_lib_filesystem >= 201603
#	undef FLUX_STDFEAT_CXX17_FILESYSTEM
#	define FLUX_STDFEAT_CXX17_FILESYSTEM 1
#endif // ~ __cpp_lib_filesystem
#endif

#if FLUXTMP_USE_HAS_INCLUDE_ && FLUXTMP_NEED_INDIVIDUAL_HEADERS_
#	if __has_include(<string_view>)
#		include <string_view>
#	endif // ~ has <string_view>
#endif // ~ USE_HAS_INCLUDE_
#if defined(__cpp_lib_string_view) && __cpp_lib_string_view >= 201603
#	undef FLUX_STDFEAT_CXX17_STRING_VIEW
#	define FLUX_STDFEAT_CXX17_STRING_VIEW 1
#endif // ~ __cpp_lib_string_view

// <type_traits> already included
#if defined(__cpp_lib_type_trait_variable_templates) && __cpp_lib_type_trait_variable_templates >= 201510
#	undef FLUX_STDFEAT_CXX17_TYPE_TRAITS
#	define FLUX_STDFEAT_CXX17_TYPE_TRAITS 1
#endif // ~ __cpp_lib_type_trait_variable_templates


#if FLUXTMP_NEED_INDIVIDUAL_HEADERS_
#	include <functional>
// <type_traits> already included
#endif // ~ NEED_INDIVIDUAL_HEADERS_
#if defined(__cpp_lib_invoke) && __cpp_lib_invoke >= 201411 && defined(__cpp_lib_invocable) && __cpp_lib_invocable >= 201703
#	undef FLUX_STDFEAT_CXX17_INVOKE
#	define FLUX_STDFEAT_CXX17_INVOKE 1
#endif // ~ __cpp_lib_invoke

#if FLUXTMP_USE_HAS_INCLUDE_ && FLUXTMP_NEED_INDIVIDUAL_HEADERS_
#	if __has_include(<variant>)
#		include <variant>
#	endif // ~ has <variant>
#endif // ~ USE_HAS_INCLUDE_
#if defined(__cpp_lib_variant) && __cpp_lib_variant >= 201606
#	undef FLUX_STDFEAT_CXX17_VARIANT
#	define FLUX_STDFEAT_CXX17_VARIANT 1
#endif // ~ __cpp_lib_variant

#if FLUXTMP_NEED_INDIVIDUAL_HEADERS_
#	include <exception>
#endif // ~ NEED_INDIVIDUAL_HEADERS_
#if defined(__cpp_lib_uncaught_exceptions) && __cpp_lib_uncaught_exceptions >= 201411
#	undef FLUX_STDFEAT_CXX17_UNCAUGHT_EXCEPTIONS
#	define FLUX_STDFEAT_CXX17_UNCAUGHT_EXCEPTIONS 1
#endif // ~ __cpp_lib_uncaught_exceptions

#if FLUXTMP_USE_HAS_INCLUDE_ && FLUXTMP_NEED_INDIVIDUAL_HEADERS_
#	if __has_include(<shared_mutex>)
#		include <shared_mutex>
#	endif // ~ has <shared_mutex>
#endif // ~ USE_HAS_INCLUDE_
#if defined(__cpp_lib_shared_mutex) && __cpp_lib_shared_mutex >= 201505
#	undef FLUX_STDFEAT_CXX17_SHARED_MUTEX
#	define FLUX_STDFEAT_CXX17_SHARED_MUTEX 1
#endif // ~ __cpp_lib_shared_mutex

//MISSING/not yet implemented: CXX17_ITERATOR_TRAITS
//MISSING/no feature test: CXX17_UNINITIALIZED

//--    C++20/2a                    ///{{{1///////////////////////////////////

//MISSING/not yet implemented: CXX20_SPAN
//MISSING/not yet implemented: CXX20_TO_ADDRESS

#if FLUXTMP_USE_HAS_INCLUDE_ && FLUXTMP_NEED_INDIVIDUAL_HEADERS_
#	if __has_include(<latch>) && __has_include(<barrier>)
#		include <latch>
#		include <barrier>
#	endif // ~ has <latch> && <barrier>
#endif // ~ USE_HAS_INCLUDE_
#if defined(__cpp_lib_latch) && defined(__cpp_lib_barrier) //TODO-version
#	undef FLUX_STDFEAT_CXX20_BARRIER
#	define FLUX_STDFEAT_CXX20_BARRIER 1
#endif // ~ __cpp_lib_latch && __cpp_lib_barrier

#if FLUXTMP_USE_HAS_INCLUDE_ && FLUXTMP_NEED_INDIVIDUAL_HEADERS_
#	if __has_include(<bit>)
#		include <bit>
#	endif // ~ has <bit>
#endif // ~ USE_HAS_INCLUDE_
#if defined(__cpp_lib_bit_cast) && __cpp_lib_bit_cast >= 201806
#	undef FLUX_STDFEAT_CXX20_BIT_CAST
#	define FLUX_STDFEAT_CXX20_BIT_CAST 1
#endif // ~ __cpp_lib_bit_cast


// <type_traits> already included
#if defined(__cpp_lib_is_constant_evaluated) && __cpp_lib_is_constant_evaluated >= 201811
#	undef FLUX_STDFEAT_CXX20_IS_CONSTEVAL
#	define FLUX_STDFEAT_CXX20_IS_CONSTEVAL 1
#endif // ~ __cpp_lib_invoke

#if FLUXTMP_USE_HAS_INCLUDE_ && FLUXTMP_NEED_INDIVIDUAL_HEADERS_
#	if __has_include(<source_location>)
#		include <source_location>
#	endif // ~ has <source_location>
#endif // ~ USE_HAS_INCLUDE_
#if defined(__cpp_lib_source_location) && __cpp_lib_source_location >= 201907
#	undef FLUX_STDFEAT_CXX20_SOURCE_LOCATION
#	define FLUX_STDFEAT_CXX20_SOURCE_LOCATION 1
#endif // ~ __cpp_lib_source_location


//--    experimental features       ///{{{1///////////////////////////////////

#if defined(__cpp_lib_experimental_optional) && __cpp_lib_experimental_optional >= 201411
#	undef FLUX_STDFEAT_XP_OPTIONAL
#	define FLUX_STDFEAT_XP_OPTIONAL 1
#endif // ~ __cpp_lib_experimental_optional

#if defined(__cpp_lib_experimental_filesystem) && __cpp_lib_experimental_filesystem >= 201406
#	undef FLUX_STDFEAT_XP_FILESYSTEM
#	define FLUX_STDFEAT_XP_FILESYSTEM 1
#endif // ~ __cpp_lib_experimental_filesystem

#if defined(__cpp_lib_experimental_source_location) && __cpp_lib_experimental_source_location >= 201505
#	undef FLUX_STDFEAT_XP_SOURCE_LOCATION
#	define FLUX_STDFEAT_XP_SOURCE_LOCATION 1
#endif // ~ __cpp_lib_experimental_source_location

#if defined(__cpp_lib_experimental_observer_ptr) && __cpp_lib_experimental_observer_ptr >= 201411
#	undef FLUX_STDFEAT_XP_OBSERVER_PTR
#	define FLUX_STDFEAT_XP_OBSERVER_PTR 1
#endif // ~ __cpp_lib_experimental_observer_ptr

//--    cleanup                     ///{{{1///////////////////////////////////

#undef FLUXTMP_NEED_INDIVIDUAL_HEADERS_
#undef FLUXTMP_USE_HAS_INCLUDE_

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_STDFEAT_CONF_FEATURE_HXX_356217F4_1F5A_4CD6_AFAB_29B2A42DC865
