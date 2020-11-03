/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- error::Exception implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/std/exception.hpp>
#include <flux/std/detail/except_data.hxx>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::error >>>             ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(error)
//--    Exception                           ///{{{2///////////////////////////

	// [cd]tor -{{{3-
Exception::Exception() noexcept = default;
Exception::~Exception() noexcept = default;

	// protected -{{{3-
void Exception::ensure_edata_() const
{
	if( !mData )
		mData = make_shared<detail::ExceptData>();
}

//--    <<< ~ flux::error namespace <<<             ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(error)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
