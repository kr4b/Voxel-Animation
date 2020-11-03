/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	dlopen() / LoadLibrary() & co
 */
/*-***************************************************************** -}}}1- */

#include <flux/sys/dylib.hpp>
#include <flux/setup/platform.hpp>

#if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
#	include <dlfcn.h>

#else // PLATFORM_WIN32
#	include <flux/sys/win32/windows.h>

#endif // ~ PLATFORM

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    dylib_open()                        ///{{{2///////////////////////////

/*
 * WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNIGN
 * WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNIGN
 *
 *  This code cannot use the error handling from win32_category, as it uses a
 *  dynamically loaded Win32 API instance. The dynamic loading relies on this
 *  code to load the API!
 *
 * WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNIGN
 * WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNIGN
 */

DylibHandle dylib_open( char const* aName )
{
#	if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
	if( auto mod = ::LoadLibraryA( aName ) )
		return reinterpret_cast<DylibHandle>(mod);

	return nullptr;
#	else // !PLATFORM_WIN32
	if( auto so = ::dlopen( aName, RTLD_LAZY|RTLD_LOCAL ) )
		return reinterpret_cast<DylibHandle>(so);

	return nullptr;
#	endif // ~ PLATFORM
}
DylibHandle dylib_open( SysPathStr const& aName )
{
#	if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
	if( auto mod = ::LoadLibraryW( aName.c_str() ) )
		return reinterpret_cast<DylibHandle>(mod);

	return nullptr;
#	else // !PLATFORM_WIN32
	if( auto so = ::dlopen( aName.c_str(), RTLD_LAZY|RTLD_LOCAL ) )
		return reinterpret_cast<DylibHandle>(so);

	return nullptr;
#	endif // ~ PLATFORM
}

//--    dylib_close()                       ///{{{2///////////////////////////
void dylib_close( DylibHandle aHandle )
{
#	if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
	::FreeLibrary( reinterpret_cast<HMODULE>(aHandle) );
#	else // !PLATFORM_WIN32
	::dlclose( aHandle );
#	endif // ~ PLATFORM
}

//--    dylib_get_*()                       ///{{{2///////////////////////////
void* dylib_get_var( DylibHandle aHandle, char const* aName )
{
#	if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
	HMODULE mod = reinterpret_cast<HMODULE>(aHandle);
	if( !mod )
		mod = ::GetModuleHandleA( nullptr );
	
	return ::GetProcAddress( mod, aName );
#	else // !PLATFORM_WIN32
	void* mod = aHandle ? aHandle : RTLD_DEFAULT;
	return ::dlsym( mod, aName );
#	endif // ~ PLATFORM
}

DylibProc dylib_get_proc( DylibHandle aHandle, char const* aName )
{
#	if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
	HMODULE mod = reinterpret_cast<HMODULE>(aHandle);
	if( !mod )
		mod = ::GetModuleHandleA( nullptr );

	return reinterpret_cast<DylibProc>(::GetProcAddress( mod, aName ));
#	else // !PLATFORM_WIN32
	void* mod = aHandle ? aHandle : RTLD_DEFAULT;
	return reinterpret_cast<DylibProc>(::dlsym( mod, aName ));
#	endif // ~ PLATFORM
}

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
