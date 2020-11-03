/*-******************************************************* -- HEADER -{{{1- */
/*-	Helpers: throw errors
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_THROW_HPP_1ECC99E3_34F6_4852_B4E3_0845D4876BD9
#define FLUX_STD_THROW_HPP_1ECC99E3_34F6_4852_B4E3_0845D4876BD9

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/stdfeat.hpp>

#include <flux/std/exception.hpp>
#include <flux/std/except/einfo.hpp>

#include <flux/util/hint.hpp>
#include <flux/util/attrib.hpp>
#include <flux/pp/stringize.hpp>
#include <flux/compat/current_func.hpp>

#include <utility>
#include <exception>

//--    macros                      ///{{{1///////////////////////////////////

#define FLUX_EXCEPT_THROW( ec, ... )                                       \
	throw FLUX_ENABLE_EXCEPTION_INFO(ec( __VA_ARGS__ ))                    \
		<< ::flux::einfo::SourceLine( __LINE__ )                           \
		<< ::flux::einfo::SourceFile( __FILE__ )                           \
		<< ::flux::einfo::SourceFunc( FLUX_COMPAT_FUNCTION_PROTO )         \
	/*ENDM*/

#define FLUX_EXCEPT_THROW_WITH_NESTED( ec, ... )                           \
	::flux::detail_throw::ThrowWithNested{} <<=                            \
		FLUX_ENABLE_EXCEPTION_INFO(ec( __VA_ARGS__ ))                      \
		<< ::flux::einfo::SourceLine( __LINE__ )                           \
		<< ::flux::einfo::SourceFile( __FILE__ )                           \
		<< ::flux::einfo::SourceFunc( FLUX_COMPAT_FUNCTION_PROTO )         \
	/*ENDM*/


/* Workaround for MSVC __VA_ARGS__ non-expansion */
#define FLUX_THROW_EXPAND_( arg ) arg

/* Convenience: */
#define FLUX_THROW( ... )                                                  \
	FLUX_THROW_EXPAND_( FLUX_EXCEPT_THROW(__VA_ARGS__) )                   \
	/*ENDM*/
#define FLUX_THROW_WITH_NESTED( ... )                                      \
	FLUX_THROW_EXPAND_( FLUX_EXCEPT_THROW_WITH_NESTED(__VA_ARGS__) )       \
	/*ENDM*/

/* LIKELY(): if we end up throwing an exception, performance is going to 
 * suck anyway. (It's likely that the branch prediction of the compiler
 * catches the branch ending in `throw` anyway, and does the right thing
 * even without the LIKELY() hint.
 */
#define FLUX_THROW_IF( cond, ... )                                         \
	if( FLUX_HINT_LIKELY(!(cond)) ) {} else FLUX_THROW(__VA_ARGS__)        \
		<< ::flux::einfo::ErrorExpr( FLUX_PP_STRINGIZE(cond) )             \
	/*ENDM*/

//--    >>> namespace = flux::detail_throw >>>      ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(detail_throw)
//--    ThrowWithNested                     ///{{{2///////////////////////////

/* Helper: "operatorified" std::throw_with_nested()
 *
 * Problem: `std::throw_with_nested()` takes the exception as its argument, so
 * the syntax is very different from the standard `throw` operator. We (ab)use
 * the operator syntax to allow users of the `THROW()` macros to attach more
 * information, i.e., like `THROW_WITH_NESTED(Error) << einfo::Something(...)`.
 *
 * This is not compatible with the function syntax, through. `ThrowWithNested`
 * works around this by providing an operator `<<=` with the same precedence as
 * the standard `throw` operator.
 */
struct ThrowWithNested
{};

/* operator<<=() has the same precedence as throw, so it's more or less 
 * perfect for this. 
 */

template< class tExcept > inline FLUX_ATTR_NORETURN
void operator<<= (ThrowWithNested const&, tExcept&& aExcept)
{
	std::throw_with_nested( std::forward<tExcept>(aExcept) );
}

//--    <<< ~ flux::detail_throw namespace <<<      ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(detail_throw)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_STD_THROW_HPP_1ECC99E3_34F6_4852_B4E3_0845D4876BD9
