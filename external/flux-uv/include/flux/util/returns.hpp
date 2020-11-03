/*-******************************************************* -- HEADER -{{{1- */
/*-	RETURNS(...) macro
 *
 * Turn auto f(...) noexcept(noexcept(blah)) -> decltype(blah) {return blah;}
 * into auto f(...) RETURNS(blah)
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_UTIL_RETURNS_HPP_27458340_5C68_46A6_9D1F_958D06B914A6
#define FLUX_UTIL_RETURNS_HPP_27458340_5C68_46A6_9D1F_958D06B914A6

//--//////////////////////////////////////////////////////////////////////////
//--    macros                      ///{{{1///////////////////////////////////

/** Macro: avoid boilerplate when defining simple functions
 *
 * `FLUX_RETURNS()` may be used to avoid some boilerplate when defining simple
 * functions that propagate the return value an `noexcept` status. For example,
 * the function
   \code
   auto f() noexcept(noexcept(some_expression)) -> decltype(some_expression)
   {
   	return some_expression;
   }
   \endcode
 * can instead be written as
   \code
   auto f() RETURNS(some_expression)
   \endcode
 * where `some_expression` is an arbitrary C++ expression.
 */
#define FLUX_RETURNS(...)             \
	noexcept(noexcept(__VA_ARGS__))   \
	-> decltype(__VA_ARGS__)          \
	{ return __VA_ARGS__; }           \
/*ENDM*/

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_UTIL_RETURNS_HPP_27458340_5C68_46A6_9D1F_958D06B914A6
