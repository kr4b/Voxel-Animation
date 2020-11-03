/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	DLAPI error category
 */
/*-***************************************************************** -}}}1- */

#include <flux_dlapi/os/dlapi_category.hpp>
#include <flux/std/format.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::dlapi::os >>>         ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(dlapi,os)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	struct DlapiCategory_ : compat::error_category
	{
		char const* name() const noexcept override;
		std::string message( int ) const override;
		compat::error_condition default_error_condition( int ) const noexcept override;
	};
}

//--    dlapi_category()                    ///{{{2///////////////////////////
compat::error_category const& dlapi_category() noexcept
{
	static DlapiCategory_ cat;
	return cat;
}

//--    make_dlapi_error_code()             ///{{{2///////////////////////////
compat::error_code make_dlapi_error_code( EDLAPIError aDlapiError ) noexcept
{
	return compat::error_code( static_cast<int>(aDlapiError), dlapi_category() );
}

//--    l: DlapiCategory_                   ///{{{2///////////////////////////
namespace
{
	char const* DlapiCategory_::name() const noexcept
	{
		return "flux-dlapi";
	}

	std::string DlapiCategory_::message( int aValue ) const
	{
		auto const value = static_cast<EDLAPIError>(aValue);
		switch( value )
		{
			case EDLAPIError::success: return "(no error)";
			case EDLAPIError::unavailable: return "API not available";
			case EDLAPIError::notLoaded: return "API not loaded";
		}

		return flux::format( "dlapi-error({:d})", aValue );
	};

	compat::error_condition DlapiCategory_::default_error_condition( int aValue ) const noexcept
	{
		if( 0 == aValue ) 
			return compat::error_condition( 0, compat::generic_category() );

		return compat::error_condition( aValue, *this );
	}
}

//--    <<< ~ flux::dlapi::os namespace <<<         ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(dlapi,os)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
