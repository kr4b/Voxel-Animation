/*-******************************************************* -- HEADER -{{{1- */
/*-	ON_SCOPE_EXIT()
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_UTIL_SCOPE_EXIT_HPP_14C3EADF_8E7F_45DF_8628_7181390C758C
#define FLUX_UTIL_SCOPE_EXIT_HPP_14C3EADF_8E7F_45DF_8628_7181390C758C

//--//////////////////////////////////////////////////////////////////////////
//--    macros                      ///{{{1///////////////////////////////////

/** Execute code on scope exit
 *
 * Executes the specified code when the current scope is exited:
   \code
   ...
  
   {
   		void* x = strdup( ... );
   		FLUX_UTIL_ON_SCOPE_EXIT 
		{
			free(x) 
		};
   }
   ...
   \endcode
 *
 * \warning Note the semicolon at the end of the brace!
 */
#define FLUX_UTIL_ON_SCOPE_EXIT            /*IMPLEMENTED_ELSEWHERE*/

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/scope_exit.inl"
#endif // FLUX_UTIL_SCOPE_EXIT_HPP_14C3EADF_8E7F_45DF_8628_7181390C758C
