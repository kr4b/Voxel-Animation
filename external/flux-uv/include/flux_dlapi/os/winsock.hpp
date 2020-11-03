/*-******************************************************* -- HEADER -{{{1- */
/*- Winsock api subset
 *
 * See notes in win32.hpp.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUXDLAPI_OS_WINSOCK_HPP_8E9CCC5C_A9E6_435C_BBC6_0DA41985F1C3
#define FLUXDLAPI_OS_WINSOCK_HPP_8E9CCC5C_A9E6_435C_BBC6_0DA41985F1C3

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/setup/arch.hpp>

#include <flux/std/identity.hpp>
#include <flux/std/inttypes.hpp>

#include <flux/compat/system_error.hpp>

#include <flux/sys/dylib.hpp>

#include <flux_dlapi/os/win32.hpp>

#include <cstddef>

//--    >>> namespace = dlapi::os >>>           ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(dlapi,os)
//--    macros                          ///{{{2///////////////////////////////

#if !defined(FLUXWS2_API)
#	if FLUX_PLATFORM == FLUX_PLATFORM_WIN32 || FLUX_PLATCAP_CYGWIN
#		define FLUXWS2_API __stdcall
#	else
#		define FLUXWS2_API
#	endif
#endif // ~ defined(FLUXWS2_API)

//--    types                           ///{{{2///////////////////////////////
namespace Ws2
{
	// standard sockets
	using SOCKET = Win32::UINT_PTR;

	struct in_addr
	{
		union
		{
			uint32_t S_addr;
		} S_un;
	};
	struct in6_addr
	{
		union
		{
			uint8_t Byte[16];
			uint16_t Word[8];
		} u;
	};

	using IN6_ADDR = in6_addr;
	using PIN6_ADDR = in6_addr*;
	using LPIN6_ADDR = in6_addr*;

	struct sockaddr
	{
		uint16_t sa_family;
		char sa_data[14];
	};
	struct sockaddr_in 
	{
		int16_t sin_family;
		uint16_t sin_port;
		in_addr sin_addr;
		char sin_zero[8];
	};
	struct sockaddr_in6
	{
		int16_t sin6_family;
		uint16_t sin6_port;
		uint32_t sin6_flowinfo;
		in6_addr sin6_addr;
		uint32_t sin6_scope_id;
	};

	// select()
	struct fd_set
	{
		static constexpr std::size_t fd_setsize = 64;
		
		uint32_t fd_count;
		SOCKET fd_array[fd_setsize];
	};

	// address resolution
	// Note: only have ADDRINFOA for the "normal" 8-bit char (blocking) lookups,
	// and the ADDRINFOEXW for e.g. non-blocking lookups. Unfortunately, for
	// the latter the non-wide version is (according to docs) seriously gimped,
	// for instance, not supporting the timeout and similar options.
	struct ADDRINFOA
	{
		int ai_flags;
		int ai_family;
		int ai_socktype;
		int ai_protocol;
		size_t ai_addrlen;
		char* ai_canonname;
		sockaddr* ai_addr;
		ADDRINFOA* ai_next;
	};

	using PADDRINFOA = ADDRINFOA*;
	using addrinfo = ADDRINFOA;

	struct ADDRINFOEXW
	{
		int ai_flags;
		int ai_family;
		int ai_socktype;
		int ai_protocol;
		int ai_addrlen;
		Win32::PWSTR ai_canonname;
		sockaddr* ai_addr;
		void* ai_blob;
		size_t ai_bloblen;
		Win32::LPGUID ai_provider;
		ADDRINFOEXW* ai_next;
	};

	using PADDRINFOEXW = ADDRINFOEXW*;
	using addrinfoexW = ADDRINFOEXW;

	using LPWSAOVERLAPPED = Win32::LPOVERLAPPED;
	using LPLOOKUPSERVICE_COMPLETION_ROUTINE = void (FLUXWS2_API*)( 
		Win32::DWORD, 
		Win32::DWORD,
		LPWSAOVERLAPPED
	);

	// various
	struct timeval
	{
		long tv_sec;
		long tv_usec;
	};

	// winsock2 specifics
	struct WSADATA
	{
		Win32::WORD wVersion;
		Win32::WORD wHighVersion;

#		if FLUX_ARCH == FLUX_ARCH_X86_32
		char szDescription[256+1];
		char szSystemStatus[128+1];
		unsigned short iMaxSockets;
		unsigned short iMaxUdpDg;
		char* lpVendorInfo;
#		else // FLUX_ARCH_X86_64 (and others??) 
		unsigned short iMaxSockets;
		unsigned short imaxUdpDg;
		char* lpVendorInfo;
		char szDescription[256+1];
		char szSystemStatus[128+1];
#		endif // ~ FLUX_ARCH
	};

	using LPWSADATA = WSADATA*;
}

//--    constants                       ///{{{2///////////////////////////////
namespace Ws2
{	
	constexpr int af_unspec   =  0;
	constexpr int af_inet     =  2;
	constexpr int af_netbios  = 17;
	constexpr int af_inet6    = 23;
	constexpr int af_irda     = 26;
	constexpr int af_bth      = 32;

	constexpr int sock_stream     = 1;
	constexpr int sock_dgram      = 2;
	constexpr int sock_raw        = 3;
	constexpr int sock_rdm        = 4;
	constexpr int sock_seqpacket  = 5;

	constexpr int ipproto_tcp  =   6;
	constexpr int ipproto_udp  =  17;
	
	constexpr SOCKET invalid_socket = (SOCKET)(~0);
	constexpr int socket_error = -1;

	// address resolution
	constexpr int ai_passive                 =     0x1;
	constexpr int ai_canonname               =     0x2;
	constexpr int ai_numerichost             =     0x4;
	constexpr int ai_all                     =   0x100;
	constexpr int ai_addrconfig              =   0x400;
	constexpr int ai_v4mapped                =   0x800;
	constexpr int ai_non_authoritative       =  0x4000;
	constexpr int ai_secure                  =  0x8000;
	constexpr int ai_return_preferred_names  = 0x10000;
	constexpr int ai_fqdn                    = 0x20000;
	constexpr int ai_fileserver              = 0x40000;

	constexpr Win32::DWORD ns_all        =  0;
	constexpr Win32::DWORD ns_dns        = 12;
	constexpr Win32::DWORD ns_netbt      = 13;
	constexpr Win32::DWORD ns_wins       = 14;
	constexpr Win32::DWORD ns_nla        = 15;
	constexpr Win32::DWORD ns_bth        = 16;
	constexpr Win32::DWORD ns_ntds       = 32;
	constexpr Win32::DWORD ns_email      = 37;
	constexpr Win32::DWORD ns_pnrpname   = 38;
	constexpr Win32::DWORD ns_pnrpcloud  = 39;

	// ioctlsocket()
	constexpr std::size_t iocparam_mask  = 0x7f;

	constexpr uint32_t ioc_void   = 0x20000000u;
	constexpr uint32_t ioc_out    = 0x40000000u;
	constexpr uint32_t ioc_in     = 0x80000000u;
	constexpr uint32_t ioc_inout  = (ioc_in|ioc_out);
	
	constexpr uint32_t fioasync = ioc_in
		| ((sizeof(uint32_t)&iocparam_mask)<<16)
		| ('f'<<8)
		| 125
	;
	constexpr uint32_t fionbio = ioc_in
		| ((sizeof(uint32_t)&iocparam_mask)<<16)
		| ('f'<<8)
		| 126
	;
	constexpr uint32_t fionread = ioc_out
		| ((sizeof(uint32_t)&iocparam_mask)<<16)
		| ('f'<<8)
		| 127
	;

	// shutdown
	constexpr int sd_receive  = 0;
	constexpr int sd_send     = 1;
	constexpr int sd_both     = 2;

	// helpers
	constexpr
	Win32::WORD makeword( Win32::WORD lo, Win32::WORD hi ) {
		return (Win32::WORD)((Win32::BYTE)lo) | (((Win32::WORD)((Win32::BYTE)hi)) << 8);
	}

	constexpr Win32::WORD winsock_version  = makeword(2,2);

	// errors (selected)
	constexpr Win32::INT wsaeinval          = 10022;
	constexpr Win32::INT wsaewouldblock     = 10035;
	constexpr Win32::INT wsaeinprogress     = 10036;
	constexpr Win32::INT wsaetimedout       = 10060;

	constexpr Win32::INT wsanotinitialised  = 10093;
	constexpr Win32::INT wsahost_not_found  = 11001;
	constexpr Win32::INT wsatry_again       = 11002;

	constexpr Win32::INT wsa_io_pending     = Win32::error_io_pending;
}

//--    prot                            ///{{{2///////////////////////////////
namespace Ws2proto
{
	// address resolution
	using GetaddrinfoFn = Win32::INT (FLUXWS2_API*)( 
		Win32::PCSTR,
		Win32::PCSTR,
		Ws2::ADDRINFOA const*,
		Ws2::PADDRINFOA*
	);
	using FreeaddrinfoFn = void (FLUXWS2_API*)( Ws2::PADDRINFOA );
	using GaiStrerrorFn = char* (*)( int );

	using GetAddrInfoExWFn = Win32::INT (FLUXWS2_API*)(
		Win32::PCWSTR,
		Win32::PCWSTR,
		Win32::DWORD,
		Win32::LPGUID,
		Ws2::ADDRINFOEXW const*,
		Ws2::PADDRINFOEXW*,
		Ws2::timeval*,
		Win32::LPOVERLAPPED,
		Ws2::LPLOOKUPSERVICE_COMPLETION_ROUTINE,
		Win32::LPHANDLE
	);
	using GetAddrInfoExCancelFn = Win32::INT (FLUXWS2_API*)( Win32::LPHANDLE );
	using GetAddrInfoExOverlappedResultFn = Win32::INT (FLUXWS2_API*)( Win32::LPOVERLAPPED );
	using FreeAddrInfoExWFn = void (FLUXWS2_API*)( Ws2::PADDRINFOEXW );

	using InetNtopFn = Win32::PCSTR (FLUXWS2_API*)(
		Win32::INT,
		void const*,
		Win32::PSTR,
		size_t
	);

	// standard sockets
	using SocketFn = Ws2::SOCKET (FLUXWS2_API*)( int, int, int );
	using ConnectFn = int (FLUXWS2_API*)( Ws2::SOCKET, Ws2::sockaddr const*, int );
	using BindFn = int (FLUXWS2_API*)( Ws2::SOCKET, Ws2::sockaddr const*, int );
	using ListenFn = int (FLUXWS2_API*)( Ws2::SOCKET, int );
	using AcceptFn = Ws2::SOCKET (FLUXWS2_API*)( Ws2::SOCKET, Ws2::sockaddr*, int* );
	using ShutdownFn = int (FLUXWS2_API*)( Ws2::SOCKET, int );
	using ClosesocketFn = int (FLUXWS2_API*)( Ws2::SOCKET );

	using GetsockoptFn = int (FLUXWS2_API*)( Ws2::SOCKET, int, int, char*, int* );
	using SetsockoptFn = int (FLUXWS2_API*)( Ws2::SOCKET, int, int, char const*, int );
	using IoctlsocketFn = int (FLUXWS2_API*)( Ws2::SOCKET, uint32_t, uint64_t* );

	using RecvFn = int (FLUXWS2_API*)( Ws2::SOCKET, char*, int, int );
	using RecvfromFn = int (FLUXWS2_API*)(
		Ws2::SOCKET, 
		char*, int, 
		int, 
		Ws2::sockaddr*, int*
	);
	using SendFn = int (FLUXWS2_API*)( Ws2::SOCKET, char const*, int, int );
	using SendtoFn = int (FLUXWS2_API*)( 
		Ws2::SOCKET, 
		char const*, int, 
		int, 
		Ws2::sockaddr const*, int*
	);

	using SelectFn = int (FLUXWS2_API*)( 
		int,
		Ws2::fd_set*,
		Ws2::fd_set*,
		Ws2::fd_set*,
		Ws2::timeval const*
	);

	// Error handling
	using WSAGetLastErrorFn = int (FLUXWS2_API*)();
	
	// Initialization and cleanup
	// NOTE: these are automatically called when the Ws2 API is loaded and 
	// unloaded respectively.
	using WSACleanupFn = int (FLUXWS2_API*)();
	using WSAStartupFn = int (FLUXWS2_API*)( Win32::WORD, Ws2::LPWSADATA );
}

//--    Ws2api                          ///{{{2///////////////////////////////
struct Ws2api
{
	bool valid;

	Ws2proto::GetaddrinfoFn getaddrinfo;
	Ws2proto::FreeaddrinfoFn freeaddrinfo;
	Ws2proto::GaiStrerrorFn gaiStrerror;

	Ws2proto::GetAddrInfoExWFn getAddrInfoExW;
	Ws2proto::GetAddrInfoExCancelFn getAddrInfoExCancel;
	Ws2proto::GetAddrInfoExOverlappedResultFn getAddrInfoExOverlappedResult;
	Ws2proto::FreeAddrInfoExWFn freeAddrInfoExW;
	Ws2proto::InetNtopFn inetNtoP;

	Ws2proto::SocketFn socket;
	Ws2proto::ConnectFn connect;
	Ws2proto::BindFn bind;
	Ws2proto::ListenFn listen;
	Ws2proto::AcceptFn accept;
	Ws2proto::ShutdownFn shutdown;
	Ws2proto::ClosesocketFn closesocket;

	Ws2proto::GetsockoptFn getsockopt;
	Ws2proto::SetsockoptFn setsockopt;
	Ws2proto::IoctlsocketFn ioctlsocket;

	Ws2proto::RecvFn recv;
	Ws2proto::RecvfromFn recvfrom;
	Ws2proto::SendFn send;
	Ws2proto::SendtoFn sendto;

	Ws2proto::SelectFn select;

	Ws2proto::WSAGetLastErrorFn wsaGetLastError;

	Ws2proto::WSACleanupFn wsaCleanup;
	Ws2proto::WSAStartupFn wsaStartup;

	// fd_sets()
	static void fd_zero( Ws2::fd_set* aSet ) noexcept
	{
		aSet->fd_count = 0;
	}
	static void fd_set( Ws2::SOCKET aFd, Ws2::fd_set* aSet ) noexcept
	{
		std::size_t i = 0;
		for( ; i < aSet->fd_count; ++i )
		{
			if( aFd == aSet->fd_array[i] ) 
				return;
		}
		
		if( i < Ws2::fd_set::fd_setsize )
		{
			aSet->fd_array[i] = aFd;
			++aSet->fd_count;
		}
	}
	static void fd_clr( Ws2::SOCKET aFd, Ws2::fd_set* aSet ) noexcept
	{
		for( std::size_t i = 0; i < aSet->fd_count; ++i )
		{
			if( aFd == aSet->fd_array[i] )
			{
				for( ++i; i < aSet->fd_count; ++i )
					aSet->fd_array[i-1] = aSet->fd_array[i];

				--aSet->fd_count;
				return;
			}
		}
	}

	static bool fd_isset( Ws2::SOCKET aFd, Ws2::fd_set const* aSet ) noexcept
	{
		for( std::size_t i = 0; i < aSet->fd_count; ++i )
		{
			if( aFd == aSet->fd_array[i] )
				return true;
		}

		return false;
	};

	std::size_t referenceCount_;
	sys::DylibHandle libHandle_; //TODO: Hide?
};

//--    functions                       ///{{{2///////////////////////////////

//TODO-note: Will call WSAStartup() automagically. Same with WSACleanup().

Ws2api const* ws2( bool aAutoload = true ) noexcept;

bool acquire_ws2( char const* = nullptr );
bool acquire_ws2( 	
	void* (*aGetFn)( char const* aFnName, void* aUser ),
	void* aUser
);

void release_ws2();

namespace ex
{
	Ws2api const* load( Identity<Ws2api>, char const* = nullptr );
	Ws2api const* load( 
		Identity<Ws2api>,
		void* (*aGetFn)( char const* aFnName, void* aUser ),
		void* aUser
	);

	void unload( Ws2api const* );

	void acquire( Ws2api const* );
	void release( Ws2api const* );
}

//--    <<< ~ dlapi::os namespace <<<          ///{{{1////////////////////////
FLUX_NAMESPACE_LEAVE2(dlapi,os)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUXDLAPI_OS_WINSOCK_HPP_8E9CCC5C_A9E6_435C_BBC6_0DA41985F1C3
