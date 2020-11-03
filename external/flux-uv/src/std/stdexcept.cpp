/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Standard exceptions
 */
/*-***************************************************************** -}}}1- */

#include <flux/std/stdexcept.hpp>
#include <flux/std/except/einfo.hpp>
#include <flux/std/except/query.hpp>

#include <sstream>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::error >>>             ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(error)
//--    $ detail                            ///{{{2///////////////////////////
namespace detail
{
	runtime_error::runtime_error()
		: std::runtime_error("")
	{}
	logic_error::logic_error()
		: std::logic_error("")
	{}
}

//--    *Error                              ///{{{2///////////////////////////

Error::~Error() noexcept = default;

	// RuntimeError
char const* RuntimeError::what() const noexcept try
{
	return einfo::query_diagnostic_info_what(*this);
}
catch( ... )
{
	return "<exception in query_diagnostic_info_what()>";
}

	// LogicError
char const* LogicError::what() const noexcept try
{
	return einfo::query_diagnostic_info_what(*this);
}
catch( ... )
{
	return "<exception in query_diagnostic_info_what()>";
}

//--    FatalException                      ///{{{2///////////////////////////
char const* FatalException::what() const noexcept try
{
	return einfo::query_diagnostic_info_what(*this);
}
catch( ... )
{
	return "<exception in query_diagnostic_info_what()>";
}

//--    d: set_api_error()                  ///{{{2///////////////////////////
namespace detail
{
	void set_api_error( Exception const& aExcept, compat::error_code&& aCode ) noexcept
	{
		aExcept << einfo::ApiError( std::move(aCode) );
	}
}

//--    <<< ~ flux::error namespace <<<             ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(error)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
