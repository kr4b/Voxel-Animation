/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	`miniz.h` error category
 */
/*-***************************************************************** -}}}1- */

#include <flux_ext/support/miniz_category.hpp>
#include <flux_ext/miniz.hpp>

#include <flux/std/format.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux_ext::support >>>       ///{{{1///////////////////
namespace flux_ext { namespace support {
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	struct MinizCategory_ : flux::compat::error_category
	{
		char const* name() const noexcept override;
		std::string message( int ) const override;
		flux::compat::error_condition default_error_condition( int ) const noexcept override;
	};
}

//--    miniz_category()                    ///{{{2///////////////////////////
flux::compat::error_category const& miniz_category() noexcept
{
	static MinizCategory_ cat;
	return cat;
}

//--    make_miniz_error_code()             ///{{{2///////////////////////////
flux::compat::error_code make_miniz_error_code( int aErr ) noexcept
{
	return flux::compat::error_code( aErr, miniz_category() );
}

//--    l: MinizCategory_                   ///{{{2///////////////////////////
namespace
{
	char const* MinizCategory_::name() const noexcept
	{
		return "miniz.h";
	}

	std::string MinizCategory_::message( int aValue ) const
	{
		if( char const* emsg = mz_error(aValue) )
			return emsg;

		return flux::format( "miniz-error-{:d}", aValue );
	};

	flux::compat::error_condition MinizCategory_::default_error_condition( int aValue ) const noexcept
	{
		if( 0 == aValue ) 
			return flux::compat::error_condition( 0, flux::compat::generic_category() );

		return flux::compat::error_condition( aValue, *this );
	}
}

//--    <<< ~ flux_ext::support namespace <<<       ///{{{1///////////////////
} }
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
