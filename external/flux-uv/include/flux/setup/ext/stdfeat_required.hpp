/*-******************************************************* -- HEADER -{{{1- */
/*-	Required standard library features
 *
 * (deprecated - do not use)
 *
 * Formerly (flux-gianttoe) optional, but now required, standard library
 * features. Kept around for reference.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SETUP_EXT_STDFEAT_REQUIRED_HPP_67149DF6_40BF_46E7_8BCA_C4F5BFADA017
#define FLUX_SETUP_EXT_STDFEAT_REQUIRED_HPP_67149DF6_40BF_46E7_8BCA_C4F5BFADA017

//--//////////////////////////////////////////////////////////////////////////
//--    Include                     ///{{{1///////////////////////////////////

#include "../compiler.hpp"
#include "../platform.hpp"

//--    C++11 lib features          ///{{{1///////////////////////////////////

#define FLUX_STDFEAT_CXX11_ATOMIC        0 /*OVERRIDDEN_ELSEWHERE*/
#define FLUX_STDFEAT_CXX11_CHRONO        0 /*OVERRIDDEN_ELSEWHERE*/
#define FLUX_STDFEAT_CXX11_EXCEPTPTR     0 /*OVERRIDDEN_ELSEWHERE*/
#define FLUX_STDFEAT_CXX11_FUNCTIONAL    0 /*OVERRIDDEN_ELSEWHERE*/
#define FLUX_STDFEAT_CXX11_FUTURE        0 /*OVERRIDDEN_ELSEWHERE*/
#define FLUX_STDFEAT_CXX11_PTR_TRAITS    0 /*OVERRIDDEN_ELSEWHERE*/
#define FLUX_STDFEAT_CXX11_RANDOM        0 /*OVERRIDDEN_ELSEWHERE*/
#define FLUX_STDFEAT_CXX11_RATIO         0 /*OVERRIDDEN_ELSEWHERE*/
#define FLUX_STDFEAT_CXX11_SHAREDPTR     0 /*OVERRIDDEN_ELSEWHERE*/
#define FLUX_STDFEAT_CXX11_THREAD        0 /*OVERRIDDEN_ELSEWHERE*/
#define FLUX_STDFEAT_CXX11_TUPLE         0 /*OVERRIDDEN_ELSEWHERE*/
#define FLUX_STDFEAT_CXX11_TYPE_TRAITS   0 /*OVERRIDDEN_ELSEWHERE*/
#define FLUX_STDFEAT_CXX11_UNIQUEPTR     0 /*OVERRIDDEN_ELSEWHERE*/
#define FLUX_STDFEAT_CXX11_UNORDERED     0 /*OVERRIDDEN_ELSEWHERE*/

/* After dropping VS2013 support: */

/** Is `std::bad_array_new_length` defined?
 *
 * Available as `flux::compat::bad_array_new_length`, with a fallback if not
 * supported.
 */
#define FLUX_STDFEAT_CXX11_BADARRAYNEW   0 /*OVERRIDDEN_ELSEWHERE*/
/** Is there a working `std::regex` & co? 
 *
 * Early versions of libstdc++ provide stubs that unconditionally throw when
 * called. `STDFEAT_CXX11_REGEX` guarantees that `std::regex` (and related)
 * are available and also do something sensible.
 */
#define FLUX_STDFEAT_CXX11_REGEX         0 /*OVERRIDDEN_ELSEWHERE*/

//--    C++11 lib quirks            ///{{{1///////////////////////////////////

/* Note: stdquirks listed here are no longer handled, and must thus not be
 * present. (I.e., they are not required!)
 */

/** Is the old name for the monotonic clock used?
 *
 * C++0x / GNU libstdc++ compatibility. Early versions used the name
 * `std::chrono::monotonic_clock`, later versions use
 * `std::chrono::steady_clock`. If `FLUX_STDFEAT_CXX11_CHRONO` is available,
 * `FLUX_STDQUIRK_CXX11_MONOTONIC_CLOCK` identifies under which name this clock
 * is available.
 *
 * If `FLUX_STDQUIRK_CXX11_MONOTONIC_CLOCK` is true, the old name is still
 * used; otherwise the (correct) new name is used.
 *
 * Either requires `FLUX_STDFEAT_CXX11_CHRONO` to be available.
 *
 * \see FLUX_STDFEAT_CXX11_CHRONO
 */
#define FLUX_STDQUIRK_CXX11_MONOTONIC_CLOCK   0 /*OVERRIDDEN_ELSEWHERE*/

/** Are the C++11 clocks usable?
 *
 * C++0x/C++11 defines `std::steady_clock` and `std::high_resolution_clock`,
 * which are aimed at high-resolution timing. Unfortunately, not all
 * implementations of these two clocks are correct, nor do they always use the
 * most accurate times available on the various platforms.
 *
 * E.g.:
 *  
 *   - some versions of GNU libstdc++ use clock that advance by at least a 
 *     microsecond, while native timers (`clock_gettime()`) seem to be 
 *     capable of resolutions in the tens of nanoseconds.
 *
 *   - MSVC:s `steady_clock` is apparently not actually steady:
 *     https://connect.microsoft.com/VisualStudio/feedback/details/753115/
 *   - MSVC:s `high_resolution_clock` is apparently not really high
 *     resolution:
 *     https://connect.microsoft.com/VisualStudio/feedback/details/719443/
 *
 * Newer versions of GNU libstdc++ seem to use `clock_gettime()` (or something
 * of similar accuracy). The bugs in Visual Studio are scheduled to be fixed,
 * albeit possibly only in releases after Visual Studio 2013.
 */
#define FLUX_STDQUIRK_CXX11_INACCURATE_CLOCKS   0 /*OVERRIDDEN_ELSEWHERE*/

/** Is `system_clock::to_time_t` unusable?
 *
 * `system_clock::to_time_t()` returns a bad result on several of my machines
 * with GCC 4.7 and earlier. (Curiously, it doesn't on Wandbox, however).
 */
#define FLUX_STDQUIRK_CXX11_SYSTEM_CLOCK_TO_TIMET   0 /* OVERRIDDEN_ELSEWHERE*/

/** Is `std::condition_variable::wait_for()` unusable?
 *
 * GCC 4.7.x's libstdc++ has problematic implementations of `wait_for()` and
 * `wait_until()` of `std::condition_variable`: given any timeout, it returns
 * immediately and with a return value of `no_timeout`.  (Curiously, again,
 * this doesn't occur on Wandbox.)
 *
 * This also affects `std::promise` whose `wait_for()` and `wait_until()` 
 * methods presumably rely on the `std::condition_variable` internally.
 *
 * TODO: Check if std::condition_variable compiles with a custom clock.
 */
#define FLUX_STDQUIRK_CXX11_CONDVAR_WAIT   0 /*OVERRIDDEN_ELSEWHERE*/

/** Is `std::is_nothrow_destructible` available?
 *
 * GCC 4.7.x is missing `std::is_nothrow_destructible`.
 */
#define FLUX_STDQUIRK_CXX11_NO_NOTHROW_DESTRUCTIBLE  0 /*OVERRIDDEN_ELSEWHERE*/

/** Are nested exceptions unsupported?
 *
 * MSVC 2013 is missing some functions related to nested exceptions, such as
 * `throw_with_nested()` and `rethrow_if_nested()`.
 *
 * See https://blogs.msdn.microsoft.com/vcblog/2014/06/11/c1114-feature-tables-for-visual-studio-14-ctp1/
 */
#define FLUX_STDQUIRK_CXX11_NO_NESTED_EXCEPT  0 /*OVERRIDDEN_ELSEWHERE*/

/** Is `max_align_t` misplaced in the global namespace?
 *
 * libstdc++ misplaces `max_align_t` in versions earlier than GCC 4.9 -- it's
 * in the global namespace, rather than `std`.
 */
#define FLUX_STDQUIRK_CXX11_MISPLACED_MAXALIGN  0 /*OVERRIDDEN_ELSEWHERE*/

/** May `std::alignment_of<>` not be applied to abstract classes?
 *
 * VS2013 tries to figure out the alignment of a type by instantiating it
 * (see _Get_align<>), which fails for obvious reasons when said type is an
 * abstract class. (Abstract classes do typically have non-trivial alignment
 * requirements by default, likely thanks to the vtable pointer or similar.)
 */
#define FLUX_STDQUIRK_CXX11_ALIGNMENT_OF_ABSTRACT  0 /*OVERRIDDEN_ELSEWHERE*/

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "../detail/ext-stdfeat_required.hxx"
#endif // FLUX_SETUP_EXT_STDFEAT_REQUIRED_HPP_67149DF6_40BF_46E7_8BCA_C4F5BFADA017
