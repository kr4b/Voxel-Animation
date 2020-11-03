/*-******************************************************* -- HEADER -{{{1- */
/*-	Wrapper around getaddrinfo()
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_SYS_ADDRINFO_HPP_4CA9CF24_0191_4245_B7CE_FEFA14D33CEF
#define FLUX_SYS_ADDRINFO_HPP_4CA9CF24_0191_4245_B7CE_FEFA14D33CEF

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/platform.hpp>

#include <flux/std/optional.hpp>
#include <flux/std/unique_ptr.hpp>

#include <flux/compat/system_error.hpp>

#include <flux/util/enum_bitfield.hpp>

#include <chrono>
#include <string>

#if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
#	include <netdb.h>
#	include <sys/types.h>
#	include <sys/socket.h>

#else // Windows
#	include <flux_dlapi/os/winsock.hpp>

#endif // ~ platform


//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    types                       ///{{{1///////////////////////////////////

#if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
using SockAddr = ::sockaddr;
using SockAddrIn4 = ::sockaddr_in;
using SockAddrIn6 = ::sockaddr_in6;

#else // PLATFORM_WIN32
using SockAddr = dlapi::os::Ws2::sockaddr;
using SockAddrIn4 = dlapi::os::Ws2::sockaddr_in;
using SockAddrIn6 = dlapi::os::Ws2::sockaddr_in6;

#endif // ~ PLATFORM

//--    constants                   ///{{{1///////////////////////////////////

#if FLUX_PLATFORM != FLUX_PLATFORM_WIN32
enum class EAddrFamily : int
{
	unspec  = AF_UNSPEC,
	inet4   = AF_INET,
	inet6   = AF_INET6
};
enum class ESockType : int
{
	any     = 0,
	stream  = SOCK_STREAM,
	dgram   = SOCK_DGRAM
};
enum class EIpProto : int
{
	any  = 0,
	tcp  = IPPROTO_TCP,
	udp  = IPPROTO_UDP
};

enum class EResolveFlags : int
{
	none         = 0,
	all          = AI_ALL,
	passive      = AI_PASSIVE,
	v4mapped     = AI_V4MAPPED,
	addrconfig   = AI_ADDRCONFIG,
	numerichost  = AI_NUMERICHOST,
	numericserv  = AI_NUMERICSERV,
	canonname    = AI_CANONNAME
};
FLUX_UTIL_DEFINE_ENUM_BITFIELD( EResolveFlags );

#else // PLATFORM_WIN32
enum class EAddrFamily : int
{
	unspec  = dlapi::os::Ws2::af_unspec,
	inet4   = dlapi::os::Ws2::af_inet,
	inet6   = dlapi::os::Ws2::af_inet6
};
enum class ESockType : int
{
	any     = 0,
	stream  = dlapi::os::Ws2::sock_stream,
	dgram   = dlapi::os::Ws2::sock_dgram
};
enum class EIpProto : int
{
	any  = 0,
	tcp  = dlapi::os::Ws2::ipproto_tcp,
	udp  = dlapi::os::Ws2::ipproto_udp
};

enum class EResolveFlags : int
{
	none         = 0,
	all          = dlapi::os::Ws2::ai_all,
	passive      = dlapi::os::Ws2::ai_passive,
	v4mapped     = dlapi::os::Ws2::ai_v4mapped,
	addrconfig   = dlapi::os::Ws2::ai_addrconfig,
	numerichost  = dlapi::os::Ws2::ai_numerichost,
	canonname    = dlapi::os::Ws2::ai_canonname
};
FLUX_UTIL_DEFINE_ENUM_BITFIELD( EResolveFlags );

#endif // ~ PLATFORM

//--    AddrHints                   ///{{{1///////////////////////////////////
struct AddrHints
{
	EResolveFlags flags  = EResolveFlags::none;
	EAddrFamily family   = EAddrFamily::unspec;
	ESockType type       = ESockType::any;
	EIpProto proto       = EIpProto::any;
};

//--    AddrInfo                    ///{{{1///////////////////////////////////
class AddrInfo final
{
	public:
		enum class EType : unsigned;

	public:
		AddrInfo() noexcept, AddrInfo( void*, EType ) noexcept;
		~AddrInfo();

		AddrInfo( AddrInfo const& ) = delete;
		AddrInfo& operator= (AddrInfo const&) = delete;

		AddrInfo( AddrInfo&& ) noexcept;
		AddrInfo& operator= (AddrInfo&&) noexcept;

	public:
		int address_family() const noexcept;
		int socktype() const noexcept;
		int protocol() const noexcept;

		std::size_t address_length() const noexcept;
		SockAddr const* address() const noexcept;

		//std::string canonname() const;  //TODO

		// Helpers for SockAddrX?

		std::size_t size() const noexcept; // Warning: O(N)
		std::size_t current() const noexcept; // Warning: O(N)

		bool next() noexcept;
		void rewind() noexcept;

		//TODO: check if current is valid
		//TODO: handle current & next when have reached last element.

	private:
		void* mBase;
		void* mActive;

		EType mType;
};

//--    functions                   ///{{{1///////////////////////////////////

optional<AddrInfo> get_addr_info(
	char const* aNode,
	char const* aService = nullptr, 
	AddrHints const& = AddrHints(),
	compat::error_code* aErrorOut = nullptr
) noexcept;

optional<AddrInfo> get_addr_info_timeout(
	char const* aNode,
	char const* aService, 
	std::chrono::microseconds aTimeout,
	AddrHints const& = AddrHints(),
	compat::error_code* aErrorOut = nullptr,
	/* This is for <= Windows 7 and other systems where there is no mechanism
	 * for asynchronous address queries. On those systems, gai_timeout() will
	 * remain usable, but *MAY* block.
	 */
	bool aAllowBlockingFallback = true
) noexcept;


std::string format_addr( AddrInfo const&, compat::error_code* = nullptr ) noexcept;
std::string format_addr( SockAddr const*, compat::error_code* = nullptr ) noexcept;
std::string format_addr( SockAddrIn4 const*, compat::error_code* = nullptr ) noexcept;
std::string format_addr( SockAddrIn6 const*, compat::error_code* = nullptr ) noexcept;

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_SYS_ADDRINFO_HPP_4CA9CF24_0191_4245_B7CE_FEFA14D33CEF
