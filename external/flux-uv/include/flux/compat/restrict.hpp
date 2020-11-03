/*-******************************************************* -- HEADER -{{{1- */
/*-	C99 restrict
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_RESTRICT_HPP_D2D01494_B837_4DB8_B744_E3030B36E768
#define FLUX_COMPAT_RESTRICT_HPP_D2D01494_B837_4DB8_B744_E3030B36E768

//--//////////////////////////////////////////////////////////////////////////
//--    restrict                    ///{{{1///////////////////////////////////

/** Mark pointer/reference as non-aliased
 *
 * Informs the compiler that objects accessed through a pointer are only 
 * accessed through that pointer during its lifetime.
 *
 * Example:
   \code
   float* FLUX_RESTRICT ptr = ...;
   \endcode
 *
 * Restrict can be applied to
 *   - local pointers/references
 *   - member pointers/references
 *   - function arguments (pointers/references)
 *   - member functions (affects `this`)
 *   - pointer/reference typedefs
 */
#define FLUX_RESTRICT                   /*OVERRIDDEN_ELSEWHERE*/

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/restrict.inl"
#endif // FLUX_COMPAT_RESTRICT_HPP_D2D01494_B837_4DB8_B744_E3030B36E768
