/*-******************************************************* -- HEADER -{{{1- */
/*-	Provide hints
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_UTIL_HINT_HPP_6C2B13A4_B733_4AEB_8204_9357E1101978
#define FLUX_UTIL_HINT_HPP_6C2B13A4_B733_4AEB_8204_9357E1101978

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include "defaults.hpp"

//--    macros                      ///{{{1///////////////////////////////////

/** Hint compiler that \a expr is likely true
 *
 * Indicate that a boolean expression is likely to be true. The compiler may
 * optimize differently based on this information. 
 *
 * Example:
   \code
   int x = some_value();
   if( FLUX_HINT_LIKELY(x < 0) )
	   return f();

   return g();
   \endcode
 *
 * The above example causes the generated code to be organized in such a way
 * that the call to `f()` occurs in the straight line code, and the call to
 * `g()` is placed in a forward-branch (on x86, and GCC/Clang). Prediction will
 * therefore prefer to speculatively execute the jump to `f()`.
 * 
 * \warning Incorrect use of this hint may decrease performance. The GCC manual
 *	states the following on this topic: <i>"In general, you should prefer to
 *	use actual profile feedback for this (-fprofile-arcs), as programmers are
 *	notoriously bad at predicting how their programs actually perform."</i>
 *
 * \see FLUX_HINT_UNLIKELY
 */
#define FLUX_HINT_LIKELY(...)           (__VA_ARGS__) /*OVERRIDDEN_ELSEWHERE*/

/** Hint compiler that \a expr is likely false
 *
 * Indicate that a boolean expression is unlikely to be true. The compiler may
 * optimize differently based on this information.
 *
 * Example:
   \code
   int x = some_value();
   if( FLUX_HINT_UNLIKELY(x < 0) )
	   return f();

   return g();
   \endcode
 *
 * See `FLUX_HINT_LIKELY()` for notes on the example. In this case, the hint is
 * unnecessary in GCC and Clang, since their heuristics arrive at the same
 * assumption.
 * 
 * \warning Incorrect use of this hint may decrease performance. The GCC manual
 *	states the following on this topic: <i>"In general, you should prefer to
 *	use actual profile feedback for this (-fprofile-arcs), as programmers are
 *	notoriously bad at predicting how their programs actually perform."</i>
 *
 * \see FLUX_HINT_LIKELY
 */
#define FLUX_HINT_UNLIKELY(...)         (__VA_ARGS__) /*OVERRIDDEN_ELSEWHERE*/

/** Hint compiler that \a expr is unpredictable
 *
 * Indicate that the boolean expression cannot be predicted. The compiler may
 * choose to optimize differently based on this information (i.e., it may 
 * prefer to generate conditionally executed instructions instead of a true 
 * branch).
 *
 * Example:
   \code
   int x = random_integer();
   int y = 0;
   int a = f(), b = g();
   if( FLUX_HINT_UNPREDICTABLE(x != 0) )
   	y = a;
   else
    y = b;
   return y;
   \endcode
 *
 * Note that `FLUX_HINT_UNPREDICTABLE()` is unnecessary in this example, as the
 * compilers typically already generate conditional move instructions instead
 * of branches.
 *
 * \note Currently untested, as this seems to be a very recent addition to Clang.
 *
 * \warning  Incorrect use of this hint may decrease performance. The GCC manual
 *	states the following on this topic: <i>"In general, you should prefer to
 *	use actual profile feedback for this (-fprofile-arcs), as programmers are
 *	notoriously bad at predicting how their programs actually perform."</i>
 *
 * 
 * \see FLUX_HINT_LIKELY, FLUX_HINT_UNLIKELY
 */
#define FLUX_HINT_UNPREDICTABLE(...)    (__VA_ARGS__) /*OVERRIDDEN_ELSEWHERE*/


/** Hint compiler about alignment of pointer
 *
 * The macro returns a copy of the original pointer that the compiler knows
 * to be aligned to \a align bytes. Behaviour is undefined if the original
 * pointer is not aligned to \a align.
 *
 * Example:
   \code
   char const* ptr = get_16aligned_ptr();
   char const* aligned = FLUX_HINT_ALIGNED(ptr,16);
   \endcode
 *
 * This functionality differs from the C++11 `alignas()` in that it can be
 * applied to an existing pointer, as a hint for local code generation.
 *
 * \note If compiled with `FLUX_DEBUG` enabled, `FLUX_HINT_ALIGNED()` includes
 * a check (`FLUX_ASSERT()`) that ensures that the pointer actually is aligned
 * as specified.
 *
 * On GCC and Clang, this is implemented via the `__builtin_assume_aligned()`.
 * The MSVC implementation tries to use the builtin `__assume()`, but currently
 * shows mixed results (though, the worst case seems to be that the hint does
 * nothing). Other (unsupported) compilers ignore the hint.
 */
#define FLUX_HINT_ALIGNED(ptr,align)     (ptr) /*OVERRIDDEN_ELSEWHERE*/


/** Provide compiler with additional information
 *
 * Provide compiler with additional information and assumptions that are known
 * to hold. Typically, this comes in the form of additional knowledge that the
 * programmer possesses, but that is not immediately expressed in the code.
 *
 * Example:
   \code
   FLUX_HINT_ASSUME( n > 0 );
   for( int i = 0; i < n; ++i )
   {
      ...;
   }
   \endcode
 * In the above example, we know that the loop will execute at least once. GCC
 * generates different code:
   \code
	; with FLUX_HINT_ASSUME()
		xorl	%eax, %eax
		movl	$n, %ecx
	.L2:
		...
		addl	$1, %eax
		cmpl	%ecx, %eax
		jne	.L2
   \endcode
 * compared to:
   \code
 	; without FLUX_HINT_ASSUME()
		xorl	%eax, %eax
		movl	$n, %ecx
		testl	%ecx, %ecx
		jle	.L7
	.L5:
		...
		addl	$1, %eax
		cmpl	%ecx, %eax
		jne	.L5
	.L7:
   \endcode
 * (on any optimization level).
 *
 * \warning Providing incorrect assumptions results in undefined behaviour! 
 *   Make sure that any information provided via `FLUX_HINT_ASSUME()` is 
 *   correct.
 *  
 * \note See http://en.chys.info/2010/07/counterpart-of-assume-in-gcc/ for GCC
 * implementation notes. Clang uses `__builtin_assume()`, available from
 * Clang 3.6 and onward. Visual Studio uses `__assume()`. GCC & Clang produce
 * similar results given the above example. For MSVC the above example doesn't
 * change (but simpler examples do). Intel & PGI seem to lack correspnding
 * builtins.
 */
#define FLUX_HINT_ASSUME(...)           /*OVERRIDEN_ELSEWHERE*/

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/hint.inl"
#endif // FLUX_UTIL_HINT_HPP_6C2B13A4_B733_4AEB_8204_9357E1101978
