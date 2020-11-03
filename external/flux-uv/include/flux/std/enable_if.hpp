/*-******************************************************* -- HEADER -{{{1- */
/*-	FLUX_ENABLE_IF() & co.
 *
 * Make enable_if<> use slightly more bearable (IMO - YMMV).
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_ENABLE_IF_HPP_E9267D19_BCCE_4F63_9C39_5428CF2D3AFA
#define FLUX_STD_ENABLE_IF_HPP_E9267D19_BCCE_4F63_9C39_5428CF2D3AFA

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <type_traits>

//--    FLUX_ENABLE_IF*()           ///{{{1///////////////////////////////////

/** std::enable_if<> wrapper (meta functions)
 *
 * Enable function overload based on boolean meta-function. (Use 
 * `FLUX_ENABLE_IF_C()` for boolean compile-time constants.)
 *
 * Example:
   \code
   template< typename tType >
   FLUX_ENABLE_IF( std::is_arithmetic<tType> )
   (void) function( tType aFoo )
   {
       ...
   }
   \endcode
 *
 * <b>Important:</b> the parantheses around the return type `(void)`.
 *
 * \see FLUX_DISABLE_IF FLUX_ENABLE_IF_C
 */
#define FLUX_ENABLE_IF(...)                                          \
	typename std::enable_if< __VA_ARGS__::value,                     \
	FLUX_ENABLE_IF_ERROR_RETURN_TYPE_IS_MISSING_PARENTHESES_         \
	/*END*/

/** std::enable_if<> wrapper (constants)
 *
 * Enable function overload based on boolean compile-time constant. (Use
 * `FLUX_ENABLE_IF()` for boolean meta-functions.)
 *
 * Example:
   \code
   template< typename tType >
   FLUX_ENABLE_IF_C( !std::is_arithmetic<tType>::value )
   (void) function( tType aFoo )
   {
       ...
   }
   \endcode
 *
 * <b>Important:</b> the parantheses around the return type `(void)`.
 *
 * \see FLUX_DISABLE_IF_C FLUX_ENABLE_IF
 */
#define FLUX_ENABLE_IF_C(...)                                        \
	typename std::enable_if< (__VA_ARGS__),                          \
	FLUX_ENABLE_IF_ERROR_RETURN_TYPE_IS_MISSING_PARENTHESES_         \
	/*END*/

//--    FLUX_DISABLE_IF*()          ///{{{1///////////////////////////////////

/** std::enable_if<> wrapper (meta functions)
 *
 * Disable function overload based on boolean meta-function. (Use 
 * `FLUX_DISABLE_IF_C()` for boolean compile-time constants.)
 *
 * Example:
   \code
   template< typename tType >
   FLUX_DISABLE_IF( std::is_arithmetic<tType> )
   (void) function( tType aFoo )
   {
       ...
   }
   \endcode
 *
 * <b>Important:</b> the parantheses around the return type `(void)`.
 *
 * \see FLUX_ENABLE_IF FLUX_DISABLE_IF_C
 */
#define FLUX_DISABLE_IF(...)                                         \
	typename std::enable_if< !(__VA_ARGS__::value),                  \
	FLUX_ENABLE_IF_ERROR_RETURN_TYPE_IS_MISSING_PARENTHESES_         \
	/*END*/

/** std::enable_if<> wrapper (constants)
 *
 * Disable function overload based on boolean compile-time constant. (Use
 * `FLUX_DISABLE_IF()` for boolean meta-functions.)
 *
 * Example:
   \code
   template< typename tType >
   FLUX_DISABLE_IF_C( !std::is_arithmetic<tType>::value )
   (void) function( tType aFoo )
   {
       ...
   }
   \endcode
 *
 * <b>Important:</b> the parantheses around the return type `(void)`.
 *
 * \see FLUX_ENABLE_IF_C FLUX_DISABLE_IF
 */

#define FLUX_DISABLE_IF_C(...)                                       \
	typename std::enable_if< !(__VA_ARGS__),                         \
	FLUX_ENABLE_IF_ERROR_RETURN_TYPE_IS_MISSING_PARENTHESES_         \
	/*END*/

//--    FLUX_ENABLE_IF_ERROR...()   ///{{{1///////////////////////////////////

#if !defined(__DOXYGEN__)

/* Hi there. If you're here because of an error regarding 
 *   FLUX_ENABLE_IF_ERROR_RETURN_TYPE_IS_MISSING_PARENTHESES_
 * you've probably forgotten some parentheses. Check out the following usage
 * example:
 *
 *	template< typename tType >
 *	FLUX_ENABLE_IF( std::is_arithmetic<tType> )
 *	(void) function( tType aFoo )
 *	{
 *		...
 *	}
 *
 * Important: the parentheses around the return type, that is, `(void)'.
 */
#define FLUX_ENABLE_IF_ERROR_RETURN_TYPE_IS_MISSING_PARENTHESES_(...)     \
	__VA_ARGS__ >::type                                                   \
	/*END*/

#endif // ~ __DOXYGEN__

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_STD_ENABLE_IF_HPP_E9267D19_BCCE_4F63_9C39_5428CF2D3AFA
