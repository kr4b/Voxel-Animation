/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Wrapper around getaddrinfo()
 *
 * TODO: limit number of threads used by the timeout version.
 */
/*-***************************************************************** -}}}1- */

#include <flux/sys/addrinfo.hpp>

#include <flux/std/log.hpp>
#include <flux/std/assert.hpp>

#include <flux/sys/gai_category.hpp>
#include <flux/sys/errno_category.hpp>
#include <flux/sys/win32_category.hpp>

#include <flux/util/narrow.hpp>
#include <flux/util/scope_exit.hpp>

#include <flux/ftl/vector.hpp>
#include <flux/compat/exchange.hpp>

#include <flux_dlapi/os/dlapi_category.hpp>

#if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
#	include <flux/compat/variant.hpp>

#	include <future>

#	include <errno.h>
#	include <netdb.h>
#	include <signal.h>
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <arpa/inet.h>

#else // PLATFORM_WIN32
#	include <flux_dlapi/os/winsock.hpp>

#endif // ~ PLATFORM

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	template< typename tAddrInfo >
	std::size_t count_( void const* aInfo ) noexcept
	{
		auto const* info = reinterpret_cast<tAddrInfo const*>(aInfo);

		std::size_t ret = 0;
		for( ; info; ++ret, info = info->ai_next )
			;
		return ret;
	}
	template< typename tAddrInfo >
	std::size_t count_( void const* aInfo, void const* aAct ) noexcept
	{
		auto const* info = reinterpret_cast<tAddrInfo const*>(aInfo);
		auto const* act = reinterpret_cast<tAddrInfo const*>(aAct);
			
		std::size_t ret = 0;
		for( ; info != act; ++ret, info = info->ai_next )
			FLUX_ASSERT( info );
		return ret;
	}

#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
#	else // PLATFORM_WIN
	ftl::Vector<dlapi::os::Win32::WCHAR> utf8_to_wchar_( 
		dlapi::os::Win32api const*,
		char const*,
		compat::error_code*
	);
#	endif // ~ PLATFORM_WIN32
}

//--    AddrInfo::EType                     ///{{{2///////////////////////////
enum class AddrInfo::EType : unsigned
{
	standard,
#	if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
	extendedW
#	endif // ~ PLATFORM_WIN32
};

//--    get_addr_info()                     ///{{{2///////////////////////////
optional<AddrInfo> get_addr_info( char const* aNode, char const* aService, AddrHints const& aHints, compat::error_code* aErrorOut ) noexcept
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	::addrinfo hints{};
	hints.ai_flags     = static_cast<int>(aHints.flags);
	hints.ai_family    = static_cast<int>(aHints.family);
	hints.ai_socktype  = static_cast<int>(aHints.type);
	hints.ai_protocol  = static_cast<int>(aHints.proto);

	::addrinfo* result = nullptr;

	auto ret = ::getaddrinfo( aNode, aService, &hints, &result );
	if( 0 != ret )
	{
		if( aErrorOut ) *aErrorOut = make_gai_error_code( ret );
		return {};
	}

	return AddrInfo( result, AddrInfo::EType::standard );
	
#	else // PLATFORM_WIN32
	auto const* ws2 = dlapi::os::ws2();
	if( !ws2 )
	{
		if( aErrorOut )
			*aErrorOut = dlapi::os::make_dlapi_error_code( dlapi::os::EDLAPIError::notLoaded );

		return {};
	}

	namespace Ws2_ = dlapi::os::Ws2;
	Ws2_::addrinfo hints{};
	hints.ai_flags     = static_cast<int>(aHints.flags);
	hints.ai_family    = static_cast<int>(aHints.family);
	hints.ai_socktype  = static_cast<int>(aHints.type);
	hints.ai_protocol  = static_cast<int>(aHints.proto);

	Ws2_::addrinfo* result = nullptr;

	auto ret = ws2->getaddrinfo( aNode, aService, &hints, &result );
	if( 0 != ret )
	{
		if( aErrorOut ) *aErrorOut = make_gai_error_code( ret );
		return {};
	}

	return AddrInfo( result, AddrInfo::EType::standard );
#	endif // ~ PLATFORM
}

//--    get_addr_info_timeout()             ///{{{2///////////////////////////
optional<AddrInfo> get_addr_info_timeout( char const* aNode, char const* aService, std::chrono::microseconds aTimeout, AddrHints const& aHints, compat::error_code* aErrorOut, bool aAllowBlockingFallback ) noexcept
{
#	if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
	/* Notes regarding this handrolled threaded implementation:
	 *   - originally implemented with getaddrinfo_a() from libanl. However,
	 *     getaddrinfo_a() is problematic in a few different ways. Internally,
	 *     getaddrinfo_a() spawns a number of threads. The main problems were
	 *     related to this. On one hand, getaddrinfo_a() passed pointers to the
	 *     worker thread(s) as-is, which would cause illegal memory accesses
	 *     when such variables went out-of-scope. A possible solution would
	 *     entail dynamically allocating everything (as the manual soution
	 *     does) and using the SIGEV_THREAD method to free resources when a
	 *     query completes. The callback from SIGEV_THREAD spawns a new thread
	 *     for each callback (!), so that's a bit overkill. On the other hand,
	 *     because there is no control over the libanl threads, the library
	 *     could never be unloaded (the latter issue is somewhat specific to
	 *     flux).
	 *   - std::async() may cause std::future::~future() to block. Rolling our
	 *     own avoids that particular gotcha.
	 */
	(void)aAllowBlockingFallback;

	using Ret_ = compat::variant<
		AddrInfo,
		compat::error_code
	>;

	auto const node = aNode ? std::string(aNode) : std::string();
	auto const serv = aService ? std::string(aService) : std::string();
	AddrHints hint = aHints;

	//TODO
	//TODO:  if C++14 gen lambda capture, std::move() node and serv
	//TODO

	std::packaged_task<Ret_()> task( [node,serv,hint] () -> Ret_ {
		compat::error_code ec;
		if( auto res = get_addr_info( node.c_str(), serv.c_str(), hint, &ec ) )
			return std::move(res.value());

		return ec;
	} );

	auto fret = task.get_future();
	std::thread thread( std::move(task) );

	auto wret = fret.wait_for( aTimeout );
	if( std::future_status::deferred == wret || std::future_status::timeout == wret )
	{
		// Not yet finished: detach thread so we don't error
		thread.detach();
		if( aErrorOut ) *aErrorOut = compat::make_error_code(compat::errc::timed_out);
		return {};
	}

	FLUX_ASSERT( std::future_status::ready == wret );
	thread.join();

	auto res = fret.get();
	if( compat::holds_alternative<compat::error_code>(res) )
	{
		if( aErrorOut ) *aErrorOut = compat::get<compat::error_code>(res);
		return {};
	}

	return std::move(compat::get<AddrInfo>(res));
	
#	else // PLATFORM_WIN32
	auto const* ws2 = dlapi::os::ws2();
	auto const* win32 = dlapi::os::win32();
	if( !ws2 || !win32 )
	{
		if( aErrorOut )
			*aErrorOut = dlapi::os::make_dlapi_error_code( dlapi::os::EDLAPIError::notLoaded );

		return {};
	}

	namespace Win32_ = dlapi::os::Win32;

	compat::error_code ec0;
	ftl::Vector<Win32_::WCHAR> node = utf8_to_wchar_( win32, aNode, &ec0 );
	if( ec0 )
	{
		if( aErrorOut ) *aErrorOut = ec0;
		return {};
	}

	compat::error_code ec1;
	ftl::Vector<Win32_::WCHAR> serv = utf8_to_wchar_( win32, aService, &ec1 );
	if( ec1 )
	{
		if( aErrorOut ) *aErrorOut = ec1;
		return {};
	}

	Win32_::OVERLAPPED ol{};
	ol.hEvent = win32->createEventA( nullptr, true, false, nullptr );
	
	if( !ol.hEvent )
	{
		if( aErrorOut )
			*aErrorOut = sys::make_win32_error_code();

		return {};
	}

	FLUX_UTIL_ON_SCOPE_EXIT
	{
		win32->closeHandle( ol.hEvent );
	};

	namespace Ws2_ = dlapi::os::Ws2;
	Ws2_::ADDRINFOEXW hints{};
	hints.ai_flags     = static_cast<int>(aHints.flags);
	hints.ai_family    = static_cast<int>(aHints.family);
	hints.ai_socktype  = static_cast<int>(aHints.type);
	hints.ai_protocol  = static_cast<int>(aHints.proto);

	auto sec = std::chrono::duration_cast<std::chrono::seconds>(aTimeout);

	Ws2_::timeval tv{};
	tv.tv_sec = util::narrow<long>(sec.count());
	tv.tv_usec = util::narrow<long>((aTimeout - sec).count());

	Ws2_::ADDRINFOEXW* result = nullptr;

	auto gairet = ws2->getAddrInfoExW( 
		node.empty() ? nullptr : node.data(),
		serv.empty() ? nullptr : serv.data(),
		Ws2_::ns_dns,
		nullptr,
		&hints,
		&result,
		&tv,
		&ol,
		nullptr, nullptr
	);

	if( Ws2_::wsa_io_pending != gairet )
	{
		//TODO: What about Windows 7 ? (Besides it sucking?)
		//TODO: What about Windows 7 ? (Besides it sucking?)
		//TODO: What about Windows 7 ? (Besides it sucking?)
		// (async lookups only for >= Windows 8)

		if( aErrorOut ) *aErrorOut = make_gai_error_code( gairet );
		return {};
	}

	auto wret = win32->waitForSingleObject( ol.hEvent, Win32_::infinite );
	if( 0 != wret )
	{
		if( aErrorOut ) *aErrorOut = make_win32_error_code( wret );
		return {};
	}

	auto gret = ws2->getAddrInfoExOverlappedResult( &ol );
	if( 0 != gret )
	{
		if( aErrorOut ) *aErrorOut = make_gai_error_code( gret );
		return {};
	}

	return AddrInfo( result, AddrInfo::EType::standard );
#	endif // ~ PLATFORM
}

//--    format_addr()                       ///{{{2///////////////////////////
std::string format_addr( AddrInfo const& aAI, compat::error_code* aEC ) noexcept
{
	return format_addr( aAI.address(), aEC );
}
std::string format_addr( SockAddr const* aSA, compat::error_code* aEC ) noexcept
{
	if( aSA )
	{
		switch( static_cast<EAddrFamily>(aSA->sa_family) )
		{
			case EAddrFamily::inet4: 
				return format_addr( reinterpret_cast<SockAddrIn4 const*>(aSA), aEC );
			case EAddrFamily::inet6: 
				return format_addr( reinterpret_cast<SockAddrIn6 const*>(aSA), aEC );

			// Can't print AF_UNSPEC.
			case EAddrFamily::unspec: break;
		}

		if( aEC ) *aEC = compat::make_error_code( compat::errc::address_family_not_supported );
		return {};
	}

	if( aEC ) *aEC = compat::make_error_code( compat::errc::invalid_argument );
	return {};
}

std::string format_addr( SockAddrIn4 const* aSA4, compat::error_code* aEC ) noexcept
{
	if( aSA4 )
	{
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		char buff[64];
		auto str = ::inet_ntop( aSA4->sin_family, &aSA4->sin_addr, buff, 64 );
		if( !str )
		{
			if( aEC ) *aEC = sys::make_errno_error_code();
			return {};
		}

		return str;
#		else // PLATFORM_WIN32
		if( auto const* ws2 = dlapi::os::ws2() )
		{
			char buff[64];
			auto str = ws2->inetNtoP( 
				aSA4->sin_family,
				&aSA4->sin_addr,
				buff,
				64
			);

			if( !str )
			{
				// XXX-note: I think Win32 error category is OK here... Verify
				if( aEC ) *aEC = sys::make_win32_error_code( ws2->wsaGetLastError() );
				return {};
			}

			return str;
		}
		else
		{
			if( aEC ) *aEC = dlapi::os::make_dlapi_error_code( dlapi::os::EDLAPIError::notLoaded );
			return {};
		}
#		endif // ~ platform
	}

	if( aEC ) *aEC = compat::make_error_code( compat::errc::invalid_argument );
	return {};
}
std::string format_addr( SockAddrIn6 const* aSA6, compat::error_code* aEC ) noexcept
{
	if( aSA6 )
	{
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		char buff[64];
		auto str = ::inet_ntop( aSA6->sin6_family, &aSA6->sin6_addr, buff, 64 );
		if( !str )
		{
			if( aEC ) *aEC = sys::make_errno_error_code();
			return {};
		}

		return str;
#		else // PLATFORM_WIN32
		if( auto const* ws2 = dlapi::os::ws2() )
		{
			char buff[64];
			auto str = ws2->inetNtoP( 
				aSA6->sin6_family,
				&aSA6->sin6_addr,
				buff,
				64
			);

			if( !str )
			{
				// XXX-note: I think Win32 error category is OK here... Verify
				if( aEC ) *aEC = sys::make_win32_error_code( ws2->wsaGetLastError() );
				return {};
			}

			return str;
		}
		else
		{
			if( aEC ) *aEC = dlapi::os::make_dlapi_error_code( dlapi::os::EDLAPIError::notLoaded );
			return {};
		}
#		endif // ~ platform
	}

	if( aEC ) *aEC = compat::make_error_code( compat::errc::invalid_argument );
	return {};
}

//--    AddrInfo                            ///{{{2///////////////////////////
AddrInfo::AddrInfo() noexcept
	: mBase( nullptr )
{}
AddrInfo::AddrInfo( void* aBase, EType aType ) noexcept
	: mBase( aBase )
	, mActive( aBase )
	, mType( aType )
{}

AddrInfo::~AddrInfo()
{
	if( mBase )
	{
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		::freeaddrinfo( reinterpret_cast<::addrinfo*>(mBase) );
#		else // PLATFORM_WIN32
		auto const* ws2 = dlapi::os::ws2();
		FLUX_ASSERT( ws2 );

		switch( mType )
		{
			case EType::standard:
				ws2->freeaddrinfo( reinterpret_cast<dlapi::os::Ws2::addrinfo*>(mBase) );
				break;
			case EType::extendedW:
				ws2->freeAddrInfoExW( reinterpret_cast<dlapi::os::Ws2::ADDRINFOEXW*>(mBase) );
				break;
		}
#		endif // ~ platform
	}
}

AddrInfo::AddrInfo( AddrInfo&& aOther ) noexcept
	: mBase( compat::exchange( aOther.mBase, nullptr ) )
	, mActive( compat::exchange( aOther.mActive, nullptr ) )
	, mType( compat::exchange( aOther.mType, EType::standard ) )
{}
AddrInfo& AddrInfo::operator= (AddrInfo&& aOther) noexcept
{
	std::swap( mBase, aOther.mBase );
	std::swap( mActive, aOther.mActive );
	std::swap( mType, aOther.mType );
	return *this;
}

int AddrInfo::address_family() const noexcept
{
	if( mBase ) 
	{
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		return reinterpret_cast<::addrinfo const*>(mActive)->ai_family;
#		else // PLATFORM_WIN32
		switch( mType )
		{
			case EType::standard:
				return reinterpret_cast<dlapi::os::Ws2::addrinfo const*>(mActive)->ai_family;
			case EType::extendedW:
				return reinterpret_cast<dlapi::os::Ws2::ADDRINFOEXW const*>(mActive)->ai_family;
		}
#		endif // ~ platform
	}

	return 0;
}
int AddrInfo::socktype() const noexcept
{
	if( mBase ) 
	{
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		return reinterpret_cast<::addrinfo const*>(mActive)->ai_socktype;
#		else // PLATFORM_WIN32
		switch( mType )
		{
			case EType::standard:
				return reinterpret_cast<dlapi::os::Ws2::addrinfo const*>(mActive)->ai_socktype;
			case EType::extendedW:
				return reinterpret_cast<dlapi::os::Ws2::ADDRINFOEXW const*>(mActive)->ai_socktype;
		}
#		endif // ~ platform
	}

	return 0;
}
int AddrInfo::protocol() const noexcept
{
	if( mBase ) 
	{
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		return reinterpret_cast<::addrinfo const*>(mActive)->ai_protocol;
#		else // PLATFORM_WIN32
		switch( mType )
		{
			case EType::standard:
				return reinterpret_cast<dlapi::os::Ws2::addrinfo const*>(mActive)->ai_protocol;
			case EType::extendedW:
				return reinterpret_cast<dlapi::os::Ws2::ADDRINFOEXW const*>(mActive)->ai_protocol;
		}
#		endif // ~ platform
	}

	return 0;
}
std::size_t AddrInfo::address_length() const noexcept
{
	if( mBase ) 
	{
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		return reinterpret_cast<::addrinfo const*>(mActive)->ai_addrlen;
#		else // PLATFORM_WIN32
		switch( mType )
		{
			case EType::standard:
				return reinterpret_cast<dlapi::os::Ws2::addrinfo const*>(mActive)->ai_addrlen;
			case EType::extendedW:
				return reinterpret_cast<dlapi::os::Ws2::ADDRINFOEXW const*>(mActive)->ai_addrlen;
		}
	#	endif // ~ platform
	}

	return 0;
}
SockAddr const* AddrInfo::address() const noexcept
{
	if( mBase ) 
	{
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		return reinterpret_cast<::addrinfo const*>(mActive)->ai_addr;
#		else // PLATFORM_WIN32
		switch( mType )
		{
			case EType::standard:
				return reinterpret_cast<dlapi::os::Ws2::addrinfo const*>(mActive)->ai_addr;
			case EType::extendedW:
				return reinterpret_cast<dlapi::os::Ws2::ADDRINFOEXW const*>(mActive)->ai_addr;
		}
#		endif // ~ platform
	}

	return 0;
}

std::size_t AddrInfo::current() const noexcept
{
	if( mBase ) 
	{
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		return count_<::addrinfo>( mBase, mActive );
#		else // PLATFORM_WIN32
		switch( mType )
		{
			case EType::standard:
				return count_<dlapi::os::Ws2::addrinfo>( mBase, mActive );
			case EType::extendedW:
				return count_<dlapi::os::Ws2::ADDRINFOEXW>( mBase, mActive );
		}
#		endif // ~ platform
	}

	return 0;
}
std::size_t AddrInfo::size() const noexcept
{
	if( mBase ) 
	{
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		return count_<::addrinfo>( mBase );
#		else // PLATFORM_WIN32
		switch( mType )
		{
			case EType::standard:
				return count_<dlapi::os::Ws2::addrinfo>( mBase );
			case EType::extendedW:
				return count_<dlapi::os::Ws2::ADDRINFOEXW>( mBase );
		}
#		endif // ~ platform
	}

	return 0;
}


bool AddrInfo::next() noexcept
{
	if( mBase ) 
	{
#		if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
		mActive = reinterpret_cast<::addrinfo const*>(mActive)->ai_next;
#		else // PLATFORM_WIN32
		switch( mType )
		{
			case EType::standard:
				mActive = reinterpret_cast<dlapi::os::Ws2::addrinfo*>(mActive)->ai_next;
				break;
			case EType::extendedW:
				mActive = reinterpret_cast<dlapi::os::Ws2::ADDRINFOEXW*>(mActive)->ai_next;
				break;
		}
#		endif // ~ platform

		return !!mActive;
	}

	return false;
}
void AddrInfo::rewind() noexcept
{
	mActive = mBase;
}

//--    $ utf8_to_wchar_()                  ///{{{2///////////////////////////
namespace
{
#	if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
	ftl::Vector<dlapi::os::Win32::WCHAR> utf8_to_wchar_( dlapi::os::Win32api const* aWin32, char const* aStr, compat::error_code* aEC )
	{
		namespace Win32_ = dlapi::os::Win32;
		FLUX_ASSERT( aWin32 );

		if( !aStr )
			return {};

		ftl::Vector<Win32_::WCHAR> wstr;
		auto needed = aWin32->multiByteToWideChar( 
			Win32_::cp_utf8,
			Win32_::mb_err_invalid_chars,
			aStr, -1,
			nullptr, 0
		);

		if( 0 == needed )
		{
			if( aEC )
				*aEC = sys::make_win32_error_code();

			return {};
		}

		wstr.resize( util::narrow<std::size_t>(needed) );
		auto ret = aWin32->multiByteToWideChar(
			Win32_::cp_utf8,
			Win32_::mb_err_invalid_chars,
			aStr, -1,
			wstr.data(), util::narrow<int>(wstr.size())
		);

		if( 0 == needed )
		{
			if( aEC )
				*aEC = sys::make_win32_error_code();

			return {};
		}

		return wstr;
	}
#	endif // ~ PLATFORM_WIN32
}

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
