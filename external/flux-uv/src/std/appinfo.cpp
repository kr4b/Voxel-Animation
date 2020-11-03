/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Application info
 */
/*-***************************************************************** -}}}1- */

#include <flux/std/appinfo.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/stdexcept.hpp>

#include <flux/pp/stringize.hpp>
#include <flux/setup/version.hpp>

#include <mutex>
#include <string>
#include <cstddef>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux >>>                    ///{{{1///////////////////
FLUX_NAMESPACE_ENTER()
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	constexpr std::size_t kAppNameMax_   = 128;
	constexpr std::size_t kAppIdentMax_  =  64;

	char gAppInfoName[kAppNameMax_] 
		= "Unnamed flux-" FLUX_PP_STRINGIZE(FLUX_VERSION_NAME) " Project"
	;
	char gAppInfoIdent[kAppIdentMax_] 
		= "flux-" FLUX_PP_STRINGIZE(FLUX_VERSION_NAME) "-default"
	;

	std::mutex gAppInfoMut;
}

//--    appinfo_*()                         ///{{{2///////////////////////////
char const* appinfo_name() noexcept
{
	std::unique_lock<std::mutex> lock(gAppInfoMut);
	return gAppInfoName;
}
char const* appinfo_ident() noexcept
{
	std::unique_lock<std::mutex> lock(gAppInfoMut);
	return gAppInfoIdent;
}

//--    set_appinfo()                       ///{{{2///////////////////////////
void set_appinfo( char const* aName, char const* aIdent )
{
	FLUX_THROW_IF( !aName || !aIdent, error::LogicError )
		<< einfo::ErrorDesc( "appinfo: must set name and identifier" )
		<< einfo::ErrorDesc( FLUX_FMT_STRING(" name = {:p}, ident = {:p}"), static_cast<void const*>(aName), static_cast<void const*>(aIdent) )
	;
	FLUX_THROW_IF( std::strlen(aName) > kAppNameMax_-1, error::LogicError )
		<< einfo::ErrorDesc( "appinfo: name too long" )
		<< einfo::ErrorDesc( FLUX_FMT_STRING(" name = '{}' ({:d} > {:d})"), aName, std::strlen(aName), kAppNameMax_-1 )
	;
	FLUX_THROW_IF( std::strlen(aIdent) > kAppIdentMax_-1, error::LogicError )
		<< einfo::ErrorDesc( "appinfo: identifier too long" )
		<< einfo::ErrorDesc( FLUX_FMT_STRING(" ident = '{}' ({:d} > {:d})"), aIdent, std::strlen(aIdent), kAppIdentMax_-1 )
	;

	{
		std::unique_lock<std::mutex> lock(gAppInfoMut);
		std::strncpy( gAppInfoName, aName, kAppNameMax_-1 ); 
		gAppInfoName[kAppNameMax_-1] = '\0';

		std::strncpy( gAppInfoIdent, aIdent, kAppIdentMax_-1 ); 
		gAppInfoName[kAppIdentMax_-1] = '\0';
	}
}

//--    <<< ~ flux namespace <<<                    ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE()
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
