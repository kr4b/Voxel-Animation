/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Apply fixups and workarounds
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_STDFEAT_FIXUPS_HXX_F14068FA_82F0_4B74_9C7D_B1805A1F86F5
#define DETAIL_STDFEAT_FIXUPS_HXX_F14068FA_82F0_4B74_9C7D_B1805A1F86F5

#include "../arch.hpp"

//--//////////////////////////////////////////////////////////////////////////
//--    libstdc++                   ///{{{1///////////////////////////////////
#if FLUX_STDLIB == FLUX_STDLIB_STDCPP
/* The cygwin version of libstdc++ seems to be missing timed_mutex and
 * and timed_recursive_mutex for some reason.
 *
 * Observed in __GLIBCXX__
 * 	- version 20131016
 */
#	if defined(__CYGWIN__)
#		undef FLUX_STDQUIRK_CXX11_NO_TIMED_MUTEX
#		define FLUX_STDQUIRK_CXX11_NO_TIMED_MUTEX 1
#	endif // ~ __CYGWIN__ 


/* BUG 62258 ( https://gcc.gnu.org/bugzilla/show_bug.cgi?id=62258 ):
 * uncaught_exception() always returns true after rethrow_exception().
 *
 * Tested:
 * 	- GCC4.7.2 / 20120920 - not affected
 * 	- GCC4.7.3 / 20130411 - affected!
 * 	- GCC4.8.1 / 20130531 - not affected
 * 	- GCC4.8.2 / 20131016 - not affected
 * 	- GCC4.9.0 / 20140422 - affected!
 * 	- GCC4.8.3 / 20140522 - affected!
 * 	- GCC4.7.4 / 20140612 - maybe affected (*)
 * 	- GCC4.9.1 / 20140716 - affected!
 * 	- GCC4.9.2 / 20141030 - affected!
 * 	- GCC4.8.4 / 20141219 - affected!
 * 	- GCC5.1.0 / 20150522 - maybe affected (*)
 * 	- GCC4.8.5 / 20150623 - maybe affected (*)
 * 	- GCC4.9.3 / 20150626 - maybe affected (*)
 * 	- GCC5.2.0 / 20150716 - maybe affected (*)
 * 	- GCC5.3.0 / 20151204 - not affected
 * 	- GCC6.1.0 / 20160427 - not affected
 * 	- GCC5.4.0 / 20160603 - not affected
 * 	- GCC4.9.4 / 20160726 - not affected
 * 	- GCC6.2.0 / 20160822 - not affected
 * 	- GCC6.3.0 / 20161221 - not affected
 *
 * (*) Bug when run on Wandbox, but not on my machine? :wtf:
 */
#	if __GLIBCXX__ > 20120920 /* 4.7.2 */ && __GLIBCXX__ < 20151204 /* 5.3.0 */ && __GLIBCXX__ != 20130531 /* 4.8.1 */ && __GLIBCXX__ != 20131016 /* 4.8.2 */
#		undef FLUX_STDQUIRK_CXX11_UNCAUGHT_RETHROW
#		define FLUX_STDQUIRK_CXX11_UNCAUGHT_RETHROW 1
#	endif // ~ __GLIBCXX__ > 20120920


/* libstdc++ has problems with `call_once` deadlocking. This specifically 
 * occurs with GLIBC 2.21 on x86[_64] and apparently most versions of GLIBC
 * on other architectures. On x86[_64] this is OK in versions before 2.21. 
 * Verified to be broken on 2.22 too.
 *
 * It's unclear whether libstdc++ will attempt to fix this, or if they wait
 * for GLIBC to modify the underlying `pthread_once()`.
 *
 * Assume broken by default. Other libc implementations may not support that
 * either (so, Bionic on Android specifically states that it doesn't deal with
 * exceptions).
 *
 * Either way, it's not fixed with GCC 7.0.1 20170215 (sorry wandbox!) and the
 * associated libraries.
 */
#	undef FLUX_STDQUIRK_CXX11_CALL_ONCE_EXCEPT
#	define FLUX_STDQUIRK_CXX11_CALL_ONCE_EXCEPT 1

#	if FLUX_ARCH == FLUX_ARCH_X86_32 || FLUX_ARCH == FLUX_ARCH_X86_64
#		if defined(__GLIBC__) && (__GLIBC__<2 || (__GLIBC__==2 && __GLIBC_MINOR__<21))
#			undef FLUX_STDQUIRK_CXX11_CALL_ONCE_EXCEPT
#			define FLUX_STDQUIRK_CXX11_CALL_ONCE_EXCEPT 0
#		endif
#	endif // ~ ARCH = x86_{32,64}

#endif // ~ libstdc++

//--    GCC                         ///{{{1///////////////////////////////////
#if FLUX_COMPILER_FAMILY == FLUX_COMPILER_GCC
#endif // ~ COMPILER_FAMILY == GCC

//--    Clang                       ///{{{1///////////////////////////////////
#if FLUX_COMPILER_FAMILY == FLUX_COMPILER_CLANG

/* Clang seems to be stuck with a libstdc++ without C++17 features, even when
 * Clang supports and is instructed to use C++17 features. In particular, Clang
 * will claim to support C++17 library features like <optional> and
 * <string_view> via feature testing macros, but actually attempting to use
 * these will fail. It is possible to detect this by checking for these headers
 * with `__has_include()`.
 */
#	if FLUX_STDLIB == FLUX_STDLIB_STDCPP
#		if FLUX_STDFEAT_CXX17_OPTIONAL
#			if !__has_include(<optional>)
#				undef FLUX_STDQUIRK_CXX17_MISSING_STDLIB17
#				define FLUX_STDQUIRK_CXX17_MISSING_STDLIB17 1
#			endif // ~ __has_include(<optional>)
#		endif // ~ STDFEAT_CXX17_OPTIONAL

#		if FLUX_STDFEAT_CXX17_STRING_VIEW
#			if !__has_include(<string_view>)
#				undef FLUX_STDQUIRK_CXX17_MISSING_STDLIB17
#				define FLUX_STDQUIRK_CXX17_MISSING_STDLIB17 1
#			endif // ~ __has_include(<string_view>)
#		endif // ~ STDFEAT_CXX17_STRING_VIEW
#	endif // ~ STDLIB_STDCPP

	/* The following have been observed to be problematic under the above
	 * conditions. So, pretend that they weren't present in the first place.
	 */
#	if FLUX_STDQUIRK_CXX17_MISSING_STDLIB17
#		undef FLUX_STDFEAT_CXX17_BYTE
#		undef FLUX_STDFEAT_CXX17_OPTIONAL
#		undef FLUX_STDFEAT_CXX17_CLAMP
#		undef FLUX_STDFEAT_CXX17_LAUNDER
#		undef FLUX_STDFEAT_CXX17_TYPE_TRAITS
#		undef FLUX_STDFEAT_CXX17_UNINITIALIZED
#		undef FLUX_STDFEAT_CXX17_INVOKE
#		undef FLUX_STDFEAT_CXX17_VARIANT
#		undef FLUX_STDFEAT_CXX17_STRING_VIEW

#		define FLUX_STDFEAT_CXX17_BYTE 0
#		define FLUX_STDFEAT_CXX17_OPTIONAL 0
#		define FLUX_STDFEAT_CXX17_CLAMP 0
#		define FLUX_STDFEAT_CXX17_LAUNDER 0
#		define FLUX_STDFEAT_CXX17_TYPE_TRAITS 0
#		define FLUX_STDFEAT_CXX17_UNINITIALIZED 0
#		define FLUX_STDFEAT_CXX17_INVOKE 0
#		define FLUX_STDFEAT_CXX17_VARIANT 0
#		define FLUX_STDFEAT_CXX17_STRING_VIEW 0
#	endif // ~ STDQUIRK_CXX17_MISSING_STDLIB17

#endif // ~ COMPILER_FAMILY == CLANG

//--    Cygwin                      ///{{{1///////////////////////////////////

/* Cygwin doesn't support <experimental/filesystem> -- at least, it was not
 * available with GCC 6.3 around 2017-04.
 *
 * TODO: when (if ever) does it become available?
 */
#if defined(__CYGWIN__)
#	if FLUX_STDLIB == FLUX_STDLIB_STDCPP
#		undef FLUX_STDFEAT_XP_FILESYSTEM
#		define FLUX_STDFEAT_XP_FILESYSTEM 0
#	endif // ~ libstdc++
#endif // ~ Cygwin

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_STDFEAT_FIXUPS_HXX_F14068FA_82F0_4B74_9C7D_B1805A1F86F5
