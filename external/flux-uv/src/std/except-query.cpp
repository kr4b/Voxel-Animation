/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- error::detail::query_* implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/std/except/query.hpp>
#include <flux/std/detail/except_data.hxx>

#include <flux/std/format.hpp>
#include <flux/util/demangle.hpp>

#include <sstream>
#include <typeinfo>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::einfo::detail >>>     ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(einfo,detail)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	template< class tExcept >
	void try_format_nested_( std::ostream&, int, tExcept const& );

	void format_dinfo_( std::ostream&, int, error::Exception const& );
	void format_dinfo_( std::ostream&, int, std::exception const& );
}

//--    query_diag_info_*()                 ///{{{2///////////////////////////
std::string query_diagnostic_info_( error::Exception const& aEx )
{
	std::ostringstream oss;
	format_dinfo_( oss, 0, aEx );
	return oss.str();
}
char const* query_diagnostic_info_what_( error::Exception const& aEx ) noexcept try
{
	std::ostringstream oss;
	format_dinfo_( oss, 0, aEx );
	return error::detail::exception_data(aEx).what_string(oss.str());
}
catch( ... )
{
	return "<<exception in diagnostic_info_what()>>\n";
}

std::string query_diagnostic_info_( std::exception const& aEx )
{
	std::ostringstream oss;
	format_dinfo_( oss, 0, aEx );
	return oss.str();
}
char const* query_diagnostic_info_what_( std::exception const& aEx ) noexcept
{
	// Uh, we'd like to do something sensible, but there's not really any
	// good place to store the sensible string that we need to return.
	return aEx.what();
}

//--    $ try_format_nested_()              ///{{{2///////////////////////////
namespace
{
	template< class tExcept >
	void try_format_nested_( std::ostream& aOf, int aIn, tExcept const& aEx )
	{
		try
		{
			std::rethrow_if_nested(aEx);
		}
		catch( error::Exception const& eErr )
		{
			print( aOf, FLUX_FMT_STRING("{1:>{0}}  with nested "), aIn, "" );
			format_dinfo_( aOf, aIn+2, eErr );
		}
		catch( std::exception const& eErr )
		{
			print( aOf, FLUX_FMT_STRING("{1:>{0}}  with nested "), aIn, "" );
			format_dinfo_( aOf, aIn+2, eErr );
		}
		catch( ... )
		{
			print( aOf, FLUX_FMT_STRING("{1:>{0}}  with unknown nested exception type\n"), aIn, "" );
		}
	}
}

//--    $ format_dinfo_()                   ///{{{2///////////////////////////
namespace
{
	void format_dinfo_( std::ostream& aOf, int aIn, error::Exception const& aEx )
	{
		print( aOf, FLUX_FMT_STRING("exception [{:p}]:\n"), static_cast<void const*>(&aEx) );
		print( aOf, FLUX_FMT_STRING("{1:>{0}}  - type = {2}\n"), aIn, "", util::demangle(typeid(aEx).name()) );

		auto const& edata = error::detail::exception_data(aEx);
		edata.diagnostic_output( aOf, aIn );

		try_format_nested_( aOf, aIn, aEx );
	}
	void format_dinfo_( std::ostream& aOf, int aIn, std::exception const& aEx )
	{
		print( aOf, FLUX_FMT_STRING("exception [{:p}]:\n"), static_cast<void const*>(&aEx) );
		print( aOf, FLUX_FMT_STRING("{1:>{0}}  - type = {2}\n"), aIn, "", util::demangle(typeid(aEx).name()) );
		print( aOf, FLUX_FMT_STRING("{1:>{0}}  - what = {{ {2} }}\n"), aIn, "", aEx.what() );

		try_format_nested_( aOf, aIn, aEx );
	}
}

//--    <<< ~ flux::einfo::detail namespace <<<     ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(einfo,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
