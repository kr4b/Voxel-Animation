/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	getaddrinfo() error category
 */
/*-***************************************************************** -}}}1- */

#include <flux/sys/gai_category.hpp>

#include <flux/std/format.hpp>
#include <flux/util/diagnostics.hpp>

#include <string>
#include <type_traits>

#if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
#	include <netdb.h>

#else // PLATFORM_WIN32
#	include <flux_dlapi/os/winsock.hpp>
#	include <flux/sys/win32_category.hpp>

#endif // ~ platform

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	struct GaiCategory_ : compat::error_category
	{
		char const* name() const noexcept override;
		std::string message( int ) const override;
		compat::error_condition default_error_condition( int ) const noexcept override;
	};
}

//--    gai_category()                      ///{{{2///////////////////////////
compat::error_category const& gai_category() noexcept
{
	static GaiCategory_ cat;
	return cat;
}

//--    make_gai_error_code()               ///{{{2///////////////////////////
compat::error_code make_gai_error_code( int aGaiError ) noexcept
{
	return compat::error_code( aGaiError, gai_category() );
}

//--    l: GaiCategory_                     ///{{{2///////////////////////////
namespace
{
	char const* GaiCategory_::name() const noexcept
	{
		return "getaddrinfo-error";
	}

	std::string GaiCategory_::message( int aValue ) const
	{
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		return gai_strerror(aValue);
#		else // PLATFORM_WIN32
		if( auto const* ws2 = dlapi::os::ws2() )
			return ws2->gaiStrerror( aValue );
		else
			return format( "getaddrinfo-error({:d})", aValue );
#		endif // ~ platform
	};

	compat::error_condition GaiCategory_::default_error_condition( int aValue ) const noexcept
	{
		if( 0 == aValue ) 
			return compat::error_condition( 0, compat::generic_category() );

#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		switch( aValue )
		{
			case EAI_AGAIN:
				return compat::error_condition( compat::errc::timed_out );
			case EAI_NONAME:
				return compat::error_condition( compat::errc::no_such_device_or_address );
		}
#		else // PLATFORM_WIN32
		namespace Ws2_ = flux::dlapi::os::Ws2;
		switch( aValue )
		{
			case Ws2_::wsaetimedout:
				return compat::error_condition( compat::errc::timed_out );
			case Ws2_::wsahost_not_found:
				return compat::error_condition( compat::errc::no_such_device_or_address );
		}
#		endif // ~ platform
			
		return compat::error_condition( aValue, *this );
	}
}

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
