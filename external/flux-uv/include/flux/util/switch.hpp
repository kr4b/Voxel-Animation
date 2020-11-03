/*-******************************************************* -- HEADER -{{{1- */
/*-	Annotations for `switch()` statements
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_UTIL_SWITCH_HPP_FC66B535_E736_41CC_B4AE_8E6E27F6AD2A
#define FLUX_UTIL_SWITCH_HPP_FC66B535_E736_41CC_B4AE_8E6E27F6AD2A

//--//////////////////////////////////////////////////////////////////////////
//--    macros                      ///{{{1///////////////////////////////////

/** Annotation: `case` may fallthrough
 *
 * Mark that a `case` is intended to fall through to the following `case`- or
 * `default` statement. Use of this annotation encourages the compiler to warn
 * about `case`s that fall through without this annotation.
 *
 * Example:
   \code
   switch( x )
   {
       case a: FLUX_SWITCH_FALLTHROUGH();
       case b:
           do_something_for_a_or_b();
           break;
   }
   \endcode
 * 
 * \see FLUX_LANGFEAT_ATTR_FALLTHROUGH
 */
#define FLUX_SWITCH_FALLTHROUGH() FLUX_DEFINED_ELSEWHERE


/** Annotation: unreachable `case`
 *
 * Mark a value as unreachable in a `switch()` statement. If the value never-
 * theless occurs, the behaviour is undefined. (In debug mode, an assertion
 * fault is trigged.)
 * 
 * Example:
   \code
   enum X { A, B, C };
  
   X x = ...;
   
   if( C == x ) return;
  
   switch( x )
   {
   	case A: ...; break;
   	case B: ...; break;
  
   	FLUX_SWITCH_UNREACHABLE(C);
   }
   \endcode
 *
 * `FLUX_SWITCH_UNREACHABLE()` avoids warnings when switching on enumerations,
 * when it is known that certain values will never occur. It may allow the 
 * compiler to perform additional optimizations.
 *
 * \see FLUX_SWITCH_UNREACHABLE_DEFAULT
 */
#define FLUX_SWITCH_UNREACHABLE(x) FLUX_DEFINED_ELSEWHERE

/** Annotation: unreachable `default`
 *
 * Mark the `default` as unreachable in a `switch()` statement. Effectively,
 * this guarantees that all possible values have been listed in the body. 
 * Behaviour is undefined should an unhandled value nevertheless occur. (In
 * debug mode, an assertion fault is triggered).
 * 
 * Example:
   \code
   void f( unsigned aX )
   {
    if( aX >= 2 ) return;
  
   	switch( aX )
   	{
   		case 0: foo(); break;
   		case 1: bar(); break;
  
   		FLUX_SWITCH_UNREACHABLE_DEFAULT();
   	}
   }
   \endcode
 *
 * \note `FLUX_INTENT_DEFAULT_UNREACHABLE()` primary purpose is to aid compiler
 * optimizations. See http://stackoverflow.com/questions/9504206/are-measurable-performance-gains-possible-from-using-vcs-assume
 *
 * \see FLUX_SWITCH_UNREACHABLE
 */
#define FLUX_INTENT_UNREACHABLE_DEFAULT() FLUX_DEFINED_ELSEWHERE

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/switch.inl"
#endif // FLUX_UTIL_SWITCH_HPP_FC66B535_E736_41CC_B4AE_8E6E27F6AD2A
