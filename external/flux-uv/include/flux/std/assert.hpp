/*-******************************************************* -- HEADER -{{{1- */
/*-	FLUX_ASSERT()
 *
 * Constexpr-enabled compile time assert, and run time assertion.
 *
 * Based on off the following: 
 *
 *  - https://akrzemi1.wordpress.com/2017/05/18/asserts-in-constexpr-functions/
 *  - https://gist.github.com/oliora/928424f7675d58fadf49c70fdba70d2f
 *
 * TODO: lot's of warnings with NVCC.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_ASSERT_HPP_B461B87E_04AB_4B7C_8EEE_CD6B0C9E82A5
#define FLUX_STD_ASSERT_HPP_B461B87E_04AB_4B7C_8EEE_CD6B0C9E82A5

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/compiler.hpp>
#include <flux/setup/namespace.hpp>

#include <flux/util/hint.hpp>
#include <flux/util/attrib.hpp>
#include <flux/util/intent.hpp>


#include <cassert>
#include <utility>

//--    FLUX_ASSERT_TRIGGER()       ///{{{1///////////////////////////////////

#define FLUX_ASSERT_TRIGGER(msg) \
	::flux::detail_assert::constexpr_assert_failed([]{assert(!msg);}) \
	/*ENDM*/

//--    FLUX_ASSERT*()              ///{{{1///////////////////////////////////
#if FLUX_NVCC_ACTIVE_PASS == FLUX_NVCC_PASS_DEVICE
/* In NVCC device code, we should just use the bog standard assert() if we're
 * targeting devices with compute_20 or higher. 
 *
 * For devices before compute_20, assert() is unsupported.
 *
 * TODO: make these too constexpr enabled?
 */

#	if __CUDA_ARCH__ >= 200
#		define FLUX_ASSERT(expr) assert( expr )
#		define FLUX_ASSERT_EX(expr,msg) assert( expr )

#	else // __CUDA_ARCH__ < 200
#		define FLUX_ASSERT(expr) ((void)0)
#		define FLUX_ASSERT_EX(expr,msg) ((void)0)

#	endif // ~ __CUDA_ARCH__

#else // !FLUX_NVCC_PASS_DEVICE

	/* NVCC has a problem with __builtin_expect() in FLUX_HINT_LIKELY() -- it
	 * doesn't recognize it as `constexpr`. If compiled with NVCC, we can't use
	 * FLUX_HINT_LIKELY()` here.
	 */
#	if FLUX_COMPILER == FLUX_COMPILER_NVCC
#		define FLUX_ASSERT_LIKELY_(expr) (expr)
#	else // !COMPILER_NVCC
#		define FLUX_ASSERT_LIKELY_(expr) FLUX_HINT_LIKELY(expr)
#	endif // ~ COMPILER_NVCC

#	define FLUX_ASSERT(expr) (FLUX_ASSERT_LIKELY_(expr) \
		? void(0) \
		: ::flux::detail_assert::constexpr_assert_failed([]{assert(!#expr);}) \
	) /*ENDM*/
#	define FLUX_ASSERT_EX(expr,msg) (FLUX_HINT_LIKELY(expr) \
		? void(0) \
		: ::flux::detail_assert::constexpr_assert_failed([]{assert(!#expr ": " msg);} ) \
	) /*ENDM*/

#endif // ~ NVCC_DEVICE_PASS

//--    d: constexpr_assert_*()     ///{{{1///////////////////////////////////
FLUX_NAMESPACE_ENTER1(detail_assert)
/* Use "detail_assert" instead of "detail", as the latter has a tendency to
 * result in ambiguous namespace problems.
 */

template< class tAssert > FLUX_ATTR_ALWAYS_INLINE FLUX_ATTR_NORETURN
void constexpr_assert_failed( tAssert&& aAssert ) noexcept
{
	/* WARNING: this is a bit fragile - in FLUX_DEBUG, INTENT_UNREACHABLE()
	 * uses FLUX_ASSERT(). A FLUX_ASSERT() here will result in an infinite
	 * recursion, and is thus a bad idea(tm).
	 *
	 * OTOH- for GCC and Clang we want a __builtin_unreachable() as there will
	 * otherwise be tons of spam about noreturn functions returning. :-(
	 */
#	if FLUX_DEBUG
	std::forward<tAssert>(aAssert)();
#		if FLUX_COMPILER_FAMILY == FLUX_COMPILER_GCC || FLUX_COMPILER_FAMILY == FLUX_COMPILER_CLANG
	__builtin_unreachable();
#		endif // ~ COMPILER_GCC || COMPILER_CLANG
#	else
	FLUX_INTENT_UNUSED(aAssert);
	FLUX_INTENT_UNREACHABLE();
#	endif
}

FLUX_NAMESPACE_LEAVE1(detail_assert)

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_STD_ASSERT_HPP_B461B87E_04AB_4B7C_8EEE_CD6B0C9E82A5
