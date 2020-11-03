/*-******************************************************* -- HEADER -{{{1- */
/*-	C++14 and later constexpr family
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_CONSTEXPR_HPP_85C44093_17FD_4C38_80CF_8D7A07E0CEDF
#define FLUX_COMPAT_CONSTEXPR_HPP_85C44093_17FD_4C38_80CF_8D7A07E0CEDF

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/langfeat.hpp>

//--    constexpr                   ///{{{1///////////////////////////////////

/** Generic `constexpr` if extended (C++14) constexpr supported
 *
 * `FLUX_CONSTEXPR_EX` expands to `constexpr` if the C++14 extended constexpr
 * are supported, and to nothing otherwise.
 *
 * Example:
   \code
   // See https://isocpp.org/wiki/faq/cpp14-language#extended-constexpr
   FLUX_CONSTEXPR_EX int my_strcmp( char const* str1, char const* str2 )
   {
      std::size_t i = 0;
      for( ; str1[i] && str2[i] && str1[i] == str2[i]; ++i )
   	     ;
   
   	  if( str1[i] == str2[i] ) return 0;
   	  if( str1[i] < str2[i] ) return -1;
   	  return 1;
   }
   \endcode
 *
 * \see FLUX_LANGFEAT_EXTENDED_CONSTEXPR
 */
#if FLUX_LANGFEAT_EXTENDED_CONSTEXPR
#	define FLUX_CONSTEXPR_EX constexpr
#else // !EXTENDED_CONSTEXPR
#	define FLUX_CONSTEXPR_EX
#endif // ~ EXTENDED_CONSTEXPR

//--    consteval                   ///{{{1///////////////////////////////////

/** Mark function as `consteval`
 *
 * `FLUX_CONSTEVAL` marks a function as `consteval`, if supported by the
 * compiler. On compilers lacking `consteval`, `FLUX_CONSTEVAL` will fall back
 * to plain `constexpr` (`consteval` is at the time of writing only
 * permitted on functions.)
 *
 * Example:
   \code
   FLUX_CONSTEVAL int sqr( int n ) { return n*n; }
   \endcode
 *
 * \warning The `constexpr`-based fallback does not have semantics identical to
 * the full `consteval`, i.e., with `constexpr` the function remains callable
 * with runtime values and does result in a record in the compiled code.
 * 
 * \see FLUX_LANGFEAT_CONSTEVAL
 */
#if FLUX_LANGFEAT_CONSTEVAL
#	define FLUX_CONSTEVAL consteval
#else // !LANGFEAT_CONSTEVAL
#	define FLUX_CONSTEVAL constexpr
#endif // ~ LANGFEAT_CONSTEVAL

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_CONSTEXPR_HPP_85C44093_17FD_4C38_80CF_8D7A07E0CEDF
