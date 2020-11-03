/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Win32 api loader implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux_dlapi/os/win32.hpp>

#include <flux/compat/exchange.hpp>

#include <flux/sys/dylib.hpp>
#include <flux/ftl/vector.hpp>
#include <flux/util/scope_exit.hpp>

#include <cassert>

#include "load_util.hxx"

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = dlapi::os >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(dlapi,os)
//--    $ local                         ///{{{2///////////////////////////////
namespace
{
	detail::ApiRecord<Win32api> gWin32Api_;

	Win32api* load_api_( void* (*)(char const*,void*), void* );
}

//--    constants                       ///{{{2///////////////////////////////

/* Most functions are found in user32.dll, followed by kernel32.dll. These
 * are therefore listed first.
 *
 * WaitOnAddress() et al. are claimed to be in kernel32.dll by the Microsoft
 * documentation, but as [1] very eloquently puts it, the docs are lying.
 * Instead, the APIs are found in "api-ms-win-core-synch-l1-2-0.dll"; thanks
 * to [1] for figuring that one out.
 *
 * [1] https://github.com/Amanieu/parking_lot/blob/master/core/src/thread_parker/windows/waitaddress.rs
 */

char const* const kDefaultWin32DLLs = 
	    "kernel32.dll"
	":" "user32.dll"
	":" "api-ms-win-core-synch-l1-2-0.dll"
;

//--    win32()                         ///{{{2///////////////////////////////
Win32api const* win32( bool aAutoload ) noexcept
{
	if( auto ptr = gWin32Api_.api() )
		return ptr;

	return aAutoload ? gWin32Api_.try_autoload() : nullptr;
}

//--    acquire_win32()                 ///{{{2///////////////////////////////
bool acquire_win32( char const* aDlibNames )
{
	return gWin32Api_.acquire( aDlibNames );
}
bool acquire_win32( void* (*aGetFn)(char const*,void*), void* aUser )
{
	return gWin32Api_.acquire( aGetFn, aUser );
}

//--    release_win32()                 ///{{{2///////////////////////////////
void release_win32()
{
	return gWin32Api_.release();
}

//--    ex::unload()                    ///{{{2///////////////////////////////
namespace ex
{
	void unload( Win32api const* aWin32 )
	{
		Win32api* mptr = const_cast<Win32api*>(aWin32);
		assert( mptr );

		mptr->valid = false;
		
		release( aWin32 );
	}
}

//--    ex::acquire()                   ///{{{2///////////////////////////////
namespace ex
{
	void acquire( Win32api const* aWin32 )
	{
		assert( aWin32 );
		++const_cast<Win32api*>(aWin32)->referenceCount_;
	}
}

//--    ex::release()                   ///{{{2///////////////////////////////
namespace ex
{
	void release( Win32api const* aWin32 )
	{
		assert( aWin32 );
		if( 0 == --const_cast<Win32api*>(aWin32)->referenceCount_ )
		{
			for( auto* dylib = aWin32->libs_; *dylib; ++dylib )
				sys::dylib_close( *dylib );

			delete [] aWin32->libs_;
			delete aWin32;
		}
	}
}

//--    ex::load()                      ///{{{2///////////////////////////////
namespace ex
{
	Win32api const* load( Identity<Win32api>, void* (*aGetFn)(char const*,void*), void* aUser )
	{
		return load_api_( aGetFn, aUser );
	}

	Win32api const* load( Identity<Win32api>, char const* aDylibNames )
	{
		if( !aDylibNames ) aDylibNames = kDefaultWin32DLLs;

		auto const len = std::strlen( aDylibNames );
		ftl::Vector<char> names( len+1, '\0' );
		std::memcpy( names.data(), aDylibNames, len );

		ftl::Vector<sys::DylibHandle> libs;
		FLUX_UTIL_ON_SCOPE_EXIT
		{
			for( auto& lib : libs )
			{
				if( lib ) sys::dylib_close( lib );
			}
		};

		char* begin = names.data();
		do
		{
			char* next = std::strchr( begin, ':' );
			if( next ) *next++ = '\0';

			if( auto lib = sys::dylib_open( begin ) )
				libs.emplace_back( lib );

			begin = next;
		} while( begin );

		if( !libs.empty() )
		{
			libs.emplace_back( nullptr ); // Sentinel element

			using FnType_ = void* (*)(char const*, void*);
			FnType_ const get = [] (char const* aName, void* aHandles) -> void* {
				//auto const& handles = *static_cast<Handles_ const*>(aHandles);
				auto libs = static_cast<sys::DylibHandle const*>(aHandles);
				for( ; *libs; ++libs )
				{
					if( auto ptr = (void*)sys::dylib_get_proc( *libs, aName ) )
						return ptr;
				}

				return nullptr;
			};

			if( auto const ret = load_api_( get, libs.data() ) )
			{
				ret->libs_ = new sys::DylibHandle[ libs.size() ];
				std::copy( libs.begin(), libs.end(), ret->libs_ );
				libs.clear();

				return ret;
			}
		}

		return nullptr;
	}
}

//--    $ load_api_()                   ///{{{2///////////////////////////////
namespace
{
	Win32api* load_api_( void* (*aGetFn)(char const*,void*), void* aUser )
	{
		assert( aGetFn );
		auto load = [&] (char const* aName) {
			return detail::FnPtrProxy(aGetFn( aName, aUser ));
		};

		Win32api* win32 = new Win32api();
		std::memset( win32, 0, sizeof(Win32api) );

		bool loaded = false;

#		define LOADFN_( fld, name ) ok = !!(win32->fld = load( name )) && ok

		// Windows 2000 / Server 2000
		{
			bool ok = true;
			LOADFN_( closeHandle, "CloseHandle" );

			LOADFN_( getSystemInfo, "GetSystemInfo" );
			LOADFN_( getVersionExA, "GetVersionExA" );
			LOADFN_( getComputerNameA, "GetComputerNameA" );

			LOADFN_( queryPerformanceCounter, "QueryPerformanceCounter" );
			LOADFN_( queryPerformanceFrequency, "QueryPerformanceFrequency" );

			LOADFN_( multiByteToWideChar, "MultiByteToWideChar" );

			LOADFN_( getStdHandle, "GetStdHandle" );
			LOADFN_( getConsoleMode, "GetConsoleMode" );
			LOADFN_( setConsoleMode, "SetConsoleMode" );
			LOADFN_( getConsoleCP, "GetConsoleCP" );
			LOADFN_( getConsoleOutputCP, "GetConsoleOutputCP" );
			LOADFN_( setConsoleCP, "SetConsoleCP" );
			LOADFN_( setConsoleOutputCP, "SetConsoleOutputCP" );
			LOADFN_( getConsoleScreenBufferInfo, "GetConsoleScreenBufferInfo" );

			LOADFN_( getSystemMetrics, "GetSystemMetrics" );

			win32->VERSION_Win2k = ok;
			loaded = loaded || ok;
		}

		// Windows XP / Server 2003
		{
			bool ok = true;
			LOADFN_( getLastError, "GetLastError" );

			LOADFN_( getProcAddress, "GetProcAddress" );
			LOADFN_( getModuleHandleA, "GetModuleHandleA" );
			LOADFN_( getCurrentProcess, "GetCurrentProcess" );

			LOADFN_( localFree, "LocalFree" );

			LOADFN_( getFileType, "GetFileType" );
			LOADFN_( globalMemoryStatusEx, "GlobalMemoryStatusEx" );

			LOADFN_( formatMessageA, "FormatMessageA" );

			LOADFN_( createEventA, "CreateEventA" );
			LOADFN_( resetEvent, "ResetEvent" );
			LOADFN_( waitForSingleObject, "WaitForSingleObject" );

			LOADFN_( getCurrentThread, "GetCurrentThread" );
			LOADFN_( getCurrentThreadId, "GetCurrentThreadId" );
			LOADFN_( getPriorityClass, "GetPriorityClass" );
			LOADFN_( setPriorityClass, "SetPriorityClass" );
			LOADFN_( getThreadPriority, "GetThreadPriority" );
			LOADFN_( setThreadPriority, "SetThreadPriority" );
			LOADFN_( getProcessAffinityMask, "GetProcessAffinityMask" );
			LOADFN_( setThreadAffinityMask, "SetThreadAffinityMask" );

			win32->VERSION_WinXP = ok;
			loaded = loaded || ok;
		}

		// Windows Vista / Server 2008
		{
			bool ok = true;
			LOADFN_( getTickCount64, "GetTickCount64" );
			LOADFN_( getCurrentProcessorNumber, "GetCurrentProcessorNumber" );

			LOADFN_( getFileInformationByHandleEx, "GetFileInformationByHandleEx" );

			win32->VERSION_WinVista = ok;
			loaded = loaded || ok;
		}

		// Windows 7 / Server 2008 R2
		{
			bool ok = true;
			LOADFN_( getLogicalProcessorInformationEx, "GetLogicalProcessorInformationEx" );

			win32->VERSION_Win7 = ok;
			loaded = loaded || ok;
		}

		// Windows 8 / Server 2012
		{
			bool ok = true;
			LOADFN_( waitOnAddress, "WaitOnAddress" );
			LOADFN_( wakeByAddressSingle, "WakeByAddressSingle" );
			LOADFN_( wakeByAddressAll, "WakeByAddressAll" );

			win32->VERSION_Win8 = ok;
			loaded = loaded || ok;
		}
#		undef LOADFN_

		// done.
		if( loaded )
		{
			win32->valid = true;
			win32->referenceCount_ = 1;
			return win32;
		}

		delete win32;
		return nullptr;
	}
}

//--    <<< ~ dlapi::os namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(dlapi,os)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
