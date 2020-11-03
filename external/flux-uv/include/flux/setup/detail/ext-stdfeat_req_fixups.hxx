/*-******************************************** -- <INTERNAL> HEADER -{{{1- */
/*-	Apply fixups and workarounds
 */
/*-***************************************************************** -}}}1- */

#ifndef DETAIL_EXT_STDFEAT_REQ_FIXUPS_HXX_430FCE26_BBDD_46E3_A5BF_CE1F28D556C7
#define DETAIL_EXT_STDFEAT_REQ_FIXUPS_HXX_430FCE26_BBDD_46E3_A5BF_CE1F28D556C7

//--//////////////////////////////////////////////////////////////////////////
//--    Clang                       ///{{{1///////////////////////////////////
#if FLUX_COMPILER_FAMILY == FLUX_COMPILER_CLANG
/* Some clang before 3.5 would have trouble with parts of the GNU libstd++; 
 * disable those parts that were found to be problematic.
 */
#	if __clang_major__ < 3 || (__clang_major__ == 3 && __clang_minor__ < 5)
#		if FLUX_STDLIB == FLUX_STDLIB_STDCPP
/* <thread> causes compiler errors in <chrono> (but standalone chrono seems 
 * fine). Disable <thread> for now :-(
 */
#			undef FLUX_STDFEAT_CXX11_THREAD
#			define FLUX_STDFEAT_CXX11_THREAD 0
#		endif // ~ STDLIB == STDCPP
#	endif // __clang__ version

/* Clang generates a SIGILL in std::future<>, see
 * https://llvm.org/bugs/show_bug.cgi?id=15483
 *
 * This occurs with Clang 3.5.0 and libstdc++ from GCC 4.7.x. But not 4.8.x
 *   - __GLIBCXX__ 20140522 == 4.8.3
 */
#	if FLUX_STDLIB == FLUX_STDLIB_STDCPP
#		if __GLIBCXX__ < 20140522 /*4.8.3 -- TODO test with earlier 4.8.x*/
#			undef FLUX_STDFEAT_CXX11_FUTURE
#			define FLUX_STDFEAT_CXX11_FUTURE 0
#		endif // ~ __GLIBCXX__ version
#	endif // ~ STDLIB == STDCPP
#endif // ~ COMPILER_FAMILY == CLANG

//--    Integrity                   ///{{{1///////////////////////////////////
/* <chrono> requires a matching <ratio>. Make sure that <chrono> is only
 * enabled if <ratio> also is available.
 */
#if !FLUX_STDFEAT_CXX11_RATIO
#	undef FLUX_STDFEAT_CXX11_CHRONO
#	define FLUX_STDFEAT_CXX11_CHRONO 0
#endif // ~ CXX11_RATIO

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_EXT_STDFEAT_REQ_FIXUPS_HXX_430FCE26_BBDD_46E3_A5BF_CE1F28D556C7
