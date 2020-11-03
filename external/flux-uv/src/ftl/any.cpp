/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Any
 */
/*-***************************************************************** -}}}1- */

#include <flux/ftl/any.hpp>
#include <flux/ftl/errors.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/assert.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::ftl::detail >>>       ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(ftl,detail)
//--    throw_any_type_incompat()           ///{{{2///////////////////////////
FLUX_ATTR_NORETURN void throw_any_type_incompat( unsigned aReq, detail::AnyMeta const* aMeta )
{
	FLUX_THROW( ftl::error::AnyTypeIncompatible )
		<< einfo::ErrorDesc( FLUX_FMT_STRING("Incompatible: require features {:#x}, but type '{}' has {:#x}"), aReq, aMeta ? aMeta->type_info().name() : "", aMeta ? aMeta->features() : 0u )
	;
}

//--    throw_any_type_mismatch()           ///{{{2///////////////////////////
FLUX_ATTR_NORETURN void throw_any_type_mismatch( char const* aHeld, char const* aReq )
{
	FLUX_THROW( ftl::error::AnyTypeMismatch )
		<< einfo::ErrorDesc( FLUX_FMT_STRING("Mismatch: held '{}', requested '{}'"), aHeld, aReq )
	;
}

//--    <<< ~ flux::ftl::detail namespace <<<       ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(ftl,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
