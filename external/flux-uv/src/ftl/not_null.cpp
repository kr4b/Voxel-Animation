/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	not_null<>
 */
/*-***************************************************************** -}}}1- */

#include <flux/ftl/not_null.hpp>

#include <flux/std/assert.hpp>
#include <flux/std/exception.hpp>

//#include <flux/util/debug_break.hpp> //TODO TODO TODO TODO TODO TODO

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    d: null_handler_throw()             ///{{{2///////////////////////////
namespace detail
{
	FLUX_ATTR_NORETURN void null_handler_throw()
	{
		throw error::NullError();
	}
}

//--    d: null_handler_assert()            ///{{{2///////////////////////////
namespace detail
{
	FLUX_ATTR_NORETURN void null_handler_assert()
	{
		//util::conditional_debug_break(); //TODO TODO TODO TODO
		FLUX_ASSERT_TRIGGER( "not_null<>: initialized with nullptr" );
		FLUX_INTENT_UNREACHABLE();
	}
}

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
