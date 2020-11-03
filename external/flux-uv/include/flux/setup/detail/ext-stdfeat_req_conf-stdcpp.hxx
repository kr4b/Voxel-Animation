/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	GNU libstdc++ required standard library features
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_EXT_STDFEAT_REQ_CONF_STDCPP_HXX_65825E97_8049_475E_99BD_CA8B1FF21D40
#define DETAIL_EXT_STDFEAT_REQ_CONF_STDCPP_HXX_65825E97_8049_475E_99BD_CA8B1FF21D40

//--//////////////////////////////////////////////////////////////////////////
//--    Fixups                      ///{{{1///////////////////////////////////

/* Let's look at __GLIBCXX__, which is a ISO date, and do some internet 
 * archeology. This is probably inexact at some points (i.e., the first TR1
 * features started appearing with GCC 4.0 in 2007, but archive.org only
 * goes back to 2009.
 *
 * (Yes, this is probably a huge WTF. I blame libstdc++ for not having a sane
 * versioning system.)
 *
 * See https://gcc.gnu.org/onlinedocs/libstdc++/manual/abi.html#abi.versioning
 * item 7 "Incremental bumping ..." for (some) GCC <-> GLIBCXX mappings.
 */

// ensure that we've got __GLIBCXX__
#include <cstddef>

#if !defined(__GLIBCXX__)
#	if !defined(__GLIBCPP__)
#		error "libstdc++ : can't detect version."
#	else
#		define __GLIBCXX__ __GLIBCPP__
#	endif // ~ __GLIBCPP__
#endif // ~ __GLIBCXX__

//--    C++11                       ///{{{1///////////////////////////////////

/* C++11 features require either full C++11 language level, or that 
 * __GXX_EXPERIMENTAL_CXX0X__ is defined.
 */
#if FLUX_LANGLEVEL >= FLUX_LANGLEVEL_CXX11 || defined(__GXX_EXPERIMENTAL_CXX0X__)

#	if __GLIBCXX__ >= 20090420
#		undef FLUX_STDFEAT_CXX11_TYPE_TRAITS
#		define FLUX_STDFEAT_CXX11_TYPE_TRAITS   1

#		undef FLUX_STDFEAT_CXX11_FUNCTIONAL
#		define FLUX_STDFEAT_CXX11_FUNCTIONAL    1

#		undef FLUX_STDFEAT_CXX11_TUPLE
#		define FLUX_STDFEAT_CXX11_TUPLE         1

#		undef FLUX_STDFEAT_CXX11_UNIQUEPTR
#		define FLUX_STDFEAT_CXX11_UNIQUEPTR     1

#		undef FLUX_STDFEAT_CXX11_SHAREDPTR
#		define FLUX_STDFEAT_CXX11_SHAREDPTR     1

#		undef FLUX_STDFEAT_CXX11_UNORDERED
#		define FLUX_STDFEAT_CXX11_UNORDERED     1

#		undef FLUX_STDFEAT_CXX11_RANDOM
#		define FLUX_STDFEAT_CXX11_RANDOM        1

		/* QUIRK: system_clock::to_time_t broken in and up to GCC 4.7. Good times.
		 */
#		undef FLUX_STDFEAT_CXX11_CHRONO
#		define FLUX_STDFEAT_CXX11_CHRONO        1

		/* <ratio> seems to be incomplete up until GCC 4.7
		 */
//#		undef FLUX_STDFEAT_CXX11_RATIO
//#		define FLUX_STDFEAT_CXX11_RATIO         1

		/* Note: __GLIBCXX__ until (at least) 20120301 (GCC 4.6.3) has a buggy
		 * std::atomic<> specialization for pointers. In particular, the
		 * following produces incorrect results:
		 *
		 *   int val = 0;
		 *   std::atomic<int*> ptr(&val);
		 *   ptr.fetch_add(1);
		 *   int* next = ptr.load();
		 *
		 * EXPECTED: next == val+1
		 * ACTUAL: next == (int*)(((uintptr_t)next)+1)
		 *
		 * From 20120920 (GCC 4.7.2) and forward this has been fixed.
		 */
//#		undef FLUX_STDFEAT_CXX11_ATOMIC
//#		define FLUX_STDFEAT_CXX11_ATOMIC        1

		/* Up until 4.7.x, exception_ptr lacks the contextual conversion to
		 * bool.
		 */
//#		undef FLUX_STDFEAT_CXX11_EXCEPTPTR
//#		define FLUX_STDFEAT_CXX11_EXCEPTPTR     1

		/* <syste_error> is available, but problematic when used with a newer
		 * libstdc++ library version. It's not really possible to detect this
		 * (the dynamic linked-against libstdc++ is determined at runtime), so
		 * disable SYSERROR until 5.x.
		 *
		 * With 5.x the problem seems to go away.
		 */
//#		undef FLUX_STDFEAT_CXX11_SYSERROR
//#		define FLUX_STDFEAT_CXX11_SYSERROR 1
#	endif // ~ __GLIBCXX__ >= 20090420

	/* <thread> actually available? */
#	if __GLIBCXX__ >= 20100606
		/* Or not. Even with GCC 4.7, <thread> and related things break
		 * in lots of funky ways.
		 *
		 *   - 4.7.0 : wait_until() fails to compile with custom clocks
		 *   - 4.7.3 : wait_for() returns no_timeout when it clearly times out
		 *   - 4.7.4 : wait_for() doesn't time out and "never" (=RODNF) returns anything
		 *
		 * So, we get a pile of QUIRKs.
		 */
#		undef FLUX_STDFEAT_CXX11_THREAD
#		define FLUX_STDFEAT_CXX11_THREAD        1

		/* Relies on <thread> internally. See above for problems.
		 * Sepcifically, std::promise<> will internally use the broken 
		 * wait_for() and wait_until() methods of std::condition_variable
		 *
		 * More QUIRKs.
		 */
#		undef FLUX_STDFEAT_CXX11_FUTURE
#		define FLUX_STDFEAT_CXX11_FUTURE        1
#	endif // ~ __GLIBCXX__ >= 20100606

#	if __GLIBCXX__ >= 20120322 /* GCC 4.7.0 */
		// exclude 20120702 = 4.5.4 and 20130412 = 4.6.4
#		if __GLIBCXX__ != 20120702 /* 4.5.4 */ && __GLIBCXX__ != 20130412 /* 4.6.4 */
		/* <ratio> is now complete
		 */
#			undef FLUX_STDFEAT_CXX11_RATIO
#			define FLUX_STDFEAT_CXX11_RATIO         1

		/* exception_ptr now has the contextual conversion to bool
		 */
#			undef FLUX_STDFEAT_CXX11_EXCEPTPTR
#			define FLUX_STDFEAT_CXX11_EXCEPTPTR     1

		/* Also, std::atomic<> with pointer types is no longer buggy. Yay!
		 */
#			undef FLUX_STDFEAT_CXX11_ATOMIC
#			define FLUX_STDFEAT_CXX11_ATOMIC 1

		/* GCC 4.7+ should have std::underlying_type
		 */
#			undef FLUX_STDQUIRK_CXX11_NO_UNDERLYING_TYPE
#			define FLUX_STDQUIRK_CXX11_NO_UNDERLYING_TYPE 0

		/* GCC 4.7 seems to have std::pointer_traits, including ::rebind<T>. 
		 * Verified on 4.7.4 and 4.8.5 (plus a few newer ones).
		 */
#			undef FLUX_STDFEAT_CXX11_PTR_TRAITS
#			define FLUX_STDFEAT_CXX11_PTR_TRAITS 1

#		endif // ~ specific __GLIBCXX__
#	endif // ~ __GLIBCXX__ >= 20120920

#endif // ~ full or experimental C++11 language support

/* Added after dropping VS2013 support: */

/* C++11 features require either full C++11 language level, or that 
 * __GXX_EXPERIMENTAL_CXX0X__ is defined.
 */
#if FLUX_LANGLEVEL >= FLUX_LANGLEVEL_CXX11 || defined(__GXX_EXPERIMENTAL_CXX0X__)
	/* Initially, the `steady_clock` was called `monotonic_clock`. */
#	undef FLUX_STDQUIRK_CXX11_MONOTONIC_CLOCK
#	define FLUX_STDQUIRK_CXX11_MONOTONIC_CLOCK 1

	/* Initial implementations used a time source that advances by ~1us,
	 * which is far less accurate than `clock_gettime()`.
	 */
#	undef FLUX_STDQUIRK_CXX11_INACCURATE_CLOCKS
#	define FLUX_STDQUIRK_CXX11_INACCURATE_CLOCKS 1

	/* Early libstdc++ at times problematic with to_time_t()
	 */
#	undef FLUX_STDQUIRK_CXX11_SYSTEM_CLOCK_TO_TIMET
#	define FLUX_STDQUIRK_CXX11_SYSTEM_CLOCK_TO_TIMET 1

	/* std::condition_variable::wait_{for,until} break on several machines
	 * in weird and wonderful ways: they return immediately with no_timeout.
	 *
	 * XXX-TODO: check wait_until() fails to compile with custom clocks
	 */
#	undef FLUX_STDQUIRK_CXX11_CONDVAR_WAIT
#	define FLUX_STDQUIRK_CXX11_CONDVAR_WAIT 1

	/* `is_nothrow_destructible<>` generally missing until GCC 4.8 */
#	undef FLUX_STDQUIRK_CXX11_NO_NOTHROW_DESTRUCTIBLE
#	define FLUX_STDQUIRK_CXX11_NO_NOTHROW_DESTRUCTIBLE 1
	/* `max_align_t` misplaced until GCC 4.9 */
#	undef FLUX_STDQUIRK_CXX11_MISPLACED_MAXALIGN
#	define FLUX_STDQUIRK_CXX11_MISPLACED_MAXALIGN 1

#	if __GLIBCXX__ >= 20120322 /* GCC 4.7.0 */ && __GLIBCXX__ != 20120702 /* 4.5.4 */ && __GLIBCXX__ != 20130412 /* 4.6.4 */
	/* From here onwards, it's been renamed to the standard name,
	 * std::chrono::steady_clock.
	 */
#		undef FLUX_STDQUIRK_CXX11_MONOTONIC_CLOCK
#		define FLUX_STDQUIRK_CXX11_MONOTONIC_CLOCK 0
#	endif // ~ __GLIBCXX__ >= 20120920


#	if __GLIBCXX__ >= 20130604 /* GCC 4.8.1 */ && __GLIBCXX__ != 20140612 /* 4.7.4 */
	/* Whatever problem wait_for / wait_until had seems to be fixed.
	 */
#		undef FLUX_STDQUIRK_CXX11_CONDVAR_WAIT
#		define FLUX_STDQUIRK_CXX11_CONDVAR_WAIT 0

	/* GCC 4.8.1+ confirmed to use clock_gettime() (or something equilvalent)
	 * for the `std::steady_clock` and the `std::high_resolution_clock`.
	 */
#		undef FLUX_STDQUIRK_CXX11_INACCURATE_CLOCKS
#		define FLUX_STDQUIRK_CXX11_INACCURATE_CLOCKS 0

	/* Fixed (?) to_time_t()
	 */
#		undef FLUX_STDQUIRK_CXX11_SYSTEM_CLOCK_TO_TIMET
#		define FLUX_STDQUIRK_CXX11_SYSTEM_CLOCK_TO_TIMET 0

	/* is_nothrow_destructible<> OK
	 */
#		undef FLUX_STDQUIRK_CXX11_NO_NOTHROW_DESTRUCTIBLE
#		define FLUX_STDQUIRK_CXX11_NO_NOTHROW_DESTRUCTIBLE 0
#	endif // __GLIBCXX__ >= 20130604


#	if __GLIBCXX__ >= 20140422 /* 4.9.0 */  && __GLIBCXX__ != 20150623 /* 4.8.5 */  && __GLIBCXX__ != 20141219 && __GLIBCXX__ != 20150426 /* 4.8.4 */  && __GLIBCXX__ != 20140522 /* 4.8.3 on Cygwin */ && __GLIBCXX__ != 20140612 /* 4.7.4 */
#		undef FLUX_STDFEAT_CXX11_BADARRAYNEW
#		define FLUX_STDFEAT_CXX11_BADARRAYNEW 1

#		undef FLUX_STDFEAT_CXX11_REGEX
#		define FLUX_STDFEAT_CXX11_REGEX 1

#		undef FLUX_STDQUIRK_CXX11_MISPLACED_MAXALIGN
#		define FLUX_STDQUIRK_CXX11_MISPLACED_MAXALIGN 0
#	endif // ~ __GLIBCXX__ >= 20140422
	 
#endif // ~ full or experimental C++11 language support

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_EXT_STDFEAT_REQ_CONF_STDCPP_HXX_65825E97_8049_475E_99BD_CA8B1FF21D40
