/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- Intent implementations
 */
/*-***************************************************************** -}}}1- */

#include <flux/setup.pkg>
#include <flux/setup/compiler.hpp>

#include <flux/std/assert.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    *LIKELY()                   ///{{{1///////////////////////////////////
#if !FLUXCFG_UTIL_IGNORE_LIKELY
#	if FLUX_COMPILER_FAMILY == FLUX_COMPILER_GCC
#		undef FLUX_HINT_LIKELY
#		define FLUX_HINT_LIKELY(...) __builtin_expect(!!(__VA_ARGS__), true)

#		undef FLUX_HINT_UNLIKELY
#		define FLUX_HINT_UNLIKELY(...) __builtin_expect(!!(__VA_ARGS__), false)

#	elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_CLANG
#		if __has_builtin(__builtin_expect)
#			undef FLUX_HINT_LIKELY
#			define FLUX_HINT_LIKELY(...) __builtin_expect(!!(__VA_ARGS__), true)

#			undef FLUX_HINT_UNLIKELY
#			define FLUX_HINT_UNLIKELY(...) __builtin_expect(!!(__VA_ARGS__), false)
#		endif // ~ __has_builtin(__builtin_expect)

#	else // other compiler
/* Default no-op implementation for FLUX_COMPILER_MSVC */
/* Default no-op implementation for other compilers */
#	endif // ~ FLUX_COMPILER_FAMILY

#endif // ~ CFG_UTIL_IGNORE_LIKELY

//--    UNPREDICTABLE()             ///{{{1///////////////////////////////////
#if !FLUXCFG_UTIL_IGNORE_LIKELY
#	if FLUX_COMPILER_FAMILY == FLUX_COMPILER_CLANG
#		if __has_builtin(__builtin_unpredictable)
#			undef FLUX_HINT_UNPREDICTABLE
#			define FLUX_HINT_UNPREDICTABLE(...) __builtin_unpredictable(!!(__VA_ARGS__))
#		endif // ~ __has_builtin(__builtin_unpredictable)
#	else // other compilers
/* Default no-op implementation for FLUX_COMPILER_GCC */
/* Default no-op implementation for FLUX_COMPILER_MSVC */
/* Default no-op implementation for other compilers */
#	endif // ~ COMPILER_FAMILY
#endif // ~ CFG_UTIL_IGNORE_LIKELY

//--    ALIGNED()                   ///{{{1///////////////////////////////////

#if FLUX_COMPILER_FAMILY == FLUX_COMPILER_GCC
#	if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)
#		define FLUX_HINT_ALIGNED_IMPL_(p,a) __builtin_assume_aligned((p),(a))
#	endif // ~ GCC version

	/* NVCC 5.5 doesn't support this. XXX - do later versions? */
#	if FLUX_COMPILER_COMPAT == FLUX_COMPILER_NVCC
#		if CUDART_VERSION < 6050
#			undef FLUX_HINT_ALIGNED_IMPL_
#			define FLUX_HINT_ALIGNED_IMPL_(p,a) (p)
#		endif // ~ v6.5
#	endif // ~ NVCC

#elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_CLANG
#	if __has_builtin(__builtin_assume_aligned)
#		define FLUX_HINT_ALIGNED_IMPL_(p,a) __builtin_assume_aligned((p),(a))
#	endif // ~ __has_builtin(__builtin_assume_aligned)

#elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_MSVC
#	define FLUX_HINT_ALIGNED_IMPL_(p,a) (__assume(uintptr_t((p))%(a) == 0),p)

	/* TODO: don't know if this works... Test! */

#endif // ~ FLUX_COMPILER_FAMILY

// Not yet defined? Define it.
#if !defined(FLUX_HINT_ALIGNED_IMPL_)
#	define FLUX_HINT_ALIGNED_IMPL_(p,a) (p)
#endif // ~ !defined(FLUX_HINT_ALIGNED_IMPL_)


/* In debug mode, include an ASSERT() that checks that the alignmen is in fact
 * at least what is claimed via the hint. In release mode, omit the check.
 */

#if FLUX_DEBUG
#	undef FLUX_HINT_ALIGNED
#	define FLUX_HINT_ALIGNED(p,a)                      \
		(FLUX_ASSERT_EX( uintptr_t((p))%(a) == 0,      \
			"FLUX_HINT_ALIGNED on unaliged ptr"),      \
		 FLUX_HINT_ALIGNED_IMPL_(p,a))                 \
	/*ENDM*/
#else // !FLUX_DEBUG
#	undef FLUX_HINT_ALIGNED
#	define FLUX_HINT_ALIGNED(p,a) FLUX_HINT_ALIGNED_IMPL_(p,a)
#endif // ~ FLUX_DEBUG

//--    ASSUME()                    ///{{{1///////////////////////////////////

#if FLUX_COMPILER_FAMILY == FLUX_COMPILER_GCC
#	if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)
#		define FLUX_HINT_ASSUME_IMPL_(...) do {           \
			if( !(__VA_ARGS__) ) __builtin_unreachable(); \
		} while(0) /*ENDM*/
#	endif // ~ GCC version

	/* NVCC 5.5 doesn't support this. XXX - do later versions? */
#	if FLUX_COMPILER_COMPAT == FLUX_COMPILER_NVCC
#		if CUDART_VERSION < 6050
#			undef FLUX_HINT_ASSUME_IMPL_
#			define FLUX_HINT_ASSUME_IMPL_(...) do {} while(0)
#		endif // ~ v6.5
#	endif // ~ NVCC

#elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_CLANG
	// __builtin_assume() from Clang 3.6 onward.
#	if __has_builtin(__builtin_assume)
#		define FLUX_HINT_ASSUME_IMPL_(...) __builtin_assume((__VA_ARGS__))
#	endif // ~ __has_builtin(__builtin_assume)

#elif FLUX_COMPILER_FAMILY == FLUX_COMPILER_MSVC
#	define FLUX_HINT_ASSUME_IMPL_(...) __assume((__VA_ARGS__))

#else
// NOTE: Intel ICC: recognizes __assume(), but seems to ignore it
// NOTE: PGI: recognizes builtins, but seems to ignore them

#endif // ~ FLUX_COMPILER_FAMILY

// Not yet defined? Define it.
#if !defined(FLUX_HINT_ASSUME_IMPL_)
#	define FLUX_HINT_ASSUME_IMPL_(...) do {} while(0)
#endif // ~ !defined(FLUX_HINT_ASSUME_IMPL_)


/* In debug mode, include an ASSERT() that checks that the assumption does
 * in fact hold.
 */

#if FLUX_DEBUG
#	undef FLUX_HINT_ASSUME
#	define FLUX_HINT_ASSUME(...)                       \
	 	do { FLUX_ASSERT_EX( (__VA_ARGS__),            \
			"FLUX_HINT_ASSUME() does not hold");       \
		 FLUX_HINT_ASSUME_IMPL_(__VA_ARGS__); }        \
		while(0)                                       \
	/*ENDM*/
#else // !FLUX_DEBUG
#	undef FLUX_HINT_ASSUME
#	define FLUX_HINT_ASSUME(...) FLUX_HINT_ASSUME_IMPL_(__VA_ARGS__)
#endif // ~ FLUX_DEBUG

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
