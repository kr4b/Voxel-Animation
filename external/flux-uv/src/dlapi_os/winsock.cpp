/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Winsock api loader implementation
 * 
 */
/*-***************************************************************** -}}}1- */

#include <flux_dlapi/os/winsock.hpp>

#include <flux/sys/dylib.hpp>

#include <flux_dlapi/os/win32.hpp>

#include <cassert>

#include "load_util.hxx"

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = dlapi::os >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(dlapi,os)
//--    $ local                         ///{{{2///////////////////////////////
namespace
{
	// Only on Windows.
	constexpr char const* kDefaultWs2LibName = "ws2_32.dll";

	detail::ApiRecord<Ws2api> gWs2Api_;
	//std::mutex gWs2ApiMutex_;
	//std::atomic<Ws2api const*> gWs2Api_;

	Ws2api* load_api_( void* (*)(char const*,void*), void* );

	// gai_strerror() is implemented as gai_strerrorA() in the ws2tcpip.h
	// header. Therefore, we can't load that function from the ws2_32.dll
	// (it's not exported). Provide our own fallback (which is a teensy
	// bit more threadsafe).
	char* gai_strerror_( int ) noexcept;
}

//--    ws2()                           ///{{{2///////////////////////////////
Ws2api const* ws2( bool aAutoload ) noexcept
{
	if( auto ptr = gWs2Api_.api() )
		return ptr;

	return aAutoload ? gWs2Api_.try_autoload() : nullptr;
}

//--    acquire_ws2()                  ///{{{2////////////////////////////////
bool acquire_ws2( char const* aDlibName )
{
	return gWs2Api_.acquire( aDlibName );
}
bool acquire_ws2( void* (*aGetFn)(char const*,void*), void* aUser )
{
	return gWs2Api_.acquire( aGetFn, aUser );
}

//--    release_ws2()                   ///{{{2///////////////////////////////
void release_ws2()
{
	return gWs2Api_.release();
}

//--    ex::unload()                    ///{{{2///////////////////////////////
namespace ex
{
	void unload( Ws2api const* aWs2 )
	{
		Ws2api* mptr = const_cast<Ws2api*>(aWs2);
		assert( mptr );

		mptr->valid = false;
		
		release( aWs2 );
	}
}

//--    ex::acquire()                   ///{{{2///////////////////////////////
namespace ex
{
	void acquire( Ws2api const* aWs2 )
	{
		assert( aWs2 );
		++const_cast<Ws2api*>(aWs2)->referenceCount_;
	}
}

//--    ex::release()                   ///{{{2///////////////////////////////
namespace ex
{
	void release( Ws2api const* aWs2 )
	{
		assert( aWs2 );
		if( 0 == --const_cast<Ws2api*>(aWs2)->referenceCount_ )
		{
			aWs2->wsaCleanup();
			
			if( aWs2->libHandle_ )
				sys::dylib_close( aWs2->libHandle_ );

			delete aWs2;
		}
	}
}

//--    ex::load_ws2()                  ///{{{2///////////////////////////////
namespace ex
{
	Ws2api const* load( Identity<Ws2api>, void* (*aGetFn)(char const*,void*), void* aUser )
	{
		return load_api_( aGetFn, aUser );
	}

	Ws2api const* load( Identity<Ws2api>, char const* aDLibName )
	{
		if( !aDLibName ) aDLibName = kDefaultWs2LibName;
		if( auto const handle = sys::dylib_open( aDLibName ) )
		{
			using FnType_ = void* (*)(char const*, void*);
			FnType_ const get = [] (char const* aName, void* aHandle) {
				auto const handle = static_cast<sys::DylibHandle>(aHandle);
				return (void*)sys::dylib_get_proc( handle, aName );
			};

			if( auto const ret = load_api_( get, handle ) )
			{
				ret->libHandle_ = handle;
				return ret;
			}

			sys::dylib_close( handle );
		}

		return nullptr;
	}
}

//--    $ load_api_()                   ///{{{2///////////////////////////////
namespace
{
	Ws2api* load_api_( void* (*aGetFn)(char const*,void*), void* aUser )
	{
		assert( aGetFn );
		auto load = [&] (char const* aName) {
			return detail::FnPtrProxy(aGetFn( aName, aUser ));
		};

		Ws2api* ws2 = new Ws2api();
		ws2->libHandle_ = nullptr;

		bool ok = true;

#		define LOADFN_( fld, name ) ok = !!(ws2->fld = load( name )) && ok

		LOADFN_( getaddrinfo, "getaddrinfo" );
		LOADFN_( freeaddrinfo, "freeaddrinfo" );
		ws2->gaiStrerror = &gai_strerror_;
		LOADFN_( getAddrInfoExW, "GetAddrInfoExW" );
		LOADFN_( getAddrInfoExCancel, "GetAddrInfoExCancel" );
		LOADFN_( getAddrInfoExOverlappedResult, "GetAddrInfoExOverlappedResult" );
		LOADFN_( freeAddrInfoExW, "FreeAddrInfoExW" );
		LOADFN_( inetNtoP, "inet_ntop" );

		LOADFN_( socket, "socket" );
		LOADFN_( connect, "connect" );
		LOADFN_( bind, "bind" );
		LOADFN_( listen, "listen" );
		LOADFN_( accept, "accept" );
		LOADFN_( shutdown, "shutdown" );
		LOADFN_( closesocket, "closesocket" );

		LOADFN_( getsockopt, "getsockopt" );
		LOADFN_( setsockopt, "setsockopt" );
		LOADFN_( ioctlsocket, "ioctlsocket" );

		LOADFN_( recv, "recv" );
		LOADFN_( recvfrom, "recvfrom" );
		LOADFN_( send, "send" );
		LOADFN_( sendto, "sendto" );

		LOADFN_( select, "select" );

		LOADFN_( wsaGetLastError, "WSAGetLastError" );

		LOADFN_( wsaCleanup, "WSACleanup" );
		LOADFN_( wsaStartup, "WSAStartup" );

#		undef LOADFN_

		// done.
		if( ok )
		{
			// Perform WSA Startup
			Ws2::WSADATA data{};
			if( 0 == ws2->wsaStartup( Ws2::makeword(2,2), &data ) )
			{
				// Done
				ws2->valid = true;
				ws2->referenceCount_ = 1;
				return ws2;
			}
		}

		delete ws2;
		return nullptr;
	}
}

//--    $ gai_strerror_()               ///{{{2///////////////////////////////
namespace
{
	constexpr std::size_t kStrErrorSize_ = 1024;
	thread_local char sStrErrorBuff_[kStrErrorSize_+1];

	char* gai_strerror_( int aErrCode ) noexcept
	{
		// If the win32 API was loaded already, use that. Otherwise, just
		// print an error message with the error code.
		if( auto const* w32 = win32( false ) )
		{
			w32->formatMessageA(
				Win32::format_message_from_system | Win32::format_message_ignore_inserts | Win32::format_message_max_width_mask,
				nullptr,
				aErrCode,
				Win32::makelangid(Win32::lang_neutral,Win32::sublang_default),
				sStrErrorBuff_,
				kStrErrorSize_,
				nullptr
			);
		}
		else
		{
			std::snprintf( sStrErrorBuff_, kStrErrorSize_, "getaddrinfo-win32-error(0x%x)", aErrCode );
		}

		return sStrErrorBuff_;
	}
}

//--    <<< ~ dlapi::os namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(dlapi,os)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
